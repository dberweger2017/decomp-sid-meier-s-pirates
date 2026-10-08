import tempfile
import hashlib
import json
import subprocess
import unittest
from pathlib import Path
from unittest.mock import patch
from tools import toolchain, linker
from tools.pirates.linking import digest
from tools.pirates.util import ToolError, load_json, write_json, sha256


class ToolchainTests(unittest.TestCase):
    def imported_fixture(self, root):
        directory = root / 'artifact'
        directory.mkdir()
        (directory / 'runtime-image.tar.gz').write_bytes(b'synthetic compiler archive')
        (root / 'toolchain').mkdir()
        (root / 'toolchain/sources.lock.json').write_text('{}\n')
        write_json(root / 'config/compiler.json', {'family': 'llvmgcc42', 'validation': 'build/toolchain/validation.json'})
        write_json(directory / 'compiler.json', {})
        write_json(directory / 'validation.json', {'probe_sha256': {'arm': 'known-probe'}})
        manifest = b'{"compiler": "known-artifact"}\n'
        write_json(directory / 'runtime-export.json', {'validated': True, 'image': 'sha256:' + 'a' * 64,
                   'sha256': sha256(directory / 'runtime-image.tar.gz'),
                   'sources_lock_sha256': sha256(root / 'toolchain/sources.lock.json'),
                   'artifacts_manifest_sha256': hashlib.sha256(manifest).hexdigest()})
        image = 'sha256:' + 'b' * 64
        responses = [subprocess.CompletedProcess([], 0, stdout='Loaded image ID: ' + image + '\n'),
                     subprocess.CompletedProcess([], 0, stdout=json.dumps([{'Os': 'linux', 'Architecture': 'amd64'}])),
                     subprocess.CompletedProcess([], 0, stdout=manifest)]
        return directory, image, responses

    def test_import_rejects_corrupt_archive_before_docker(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            directory, _, _ = self.imported_fixture(root)
            (directory / 'runtime-image.tar.gz').write_bytes(b'corrupt')
            with patch.object(toolchain, 'ROOT', root), patch.object(toolchain.subprocess, 'run') as run:
                with self.assertRaisesRegex(ToolError, 'SHA-256 differs'):
                    toolchain.import_runtime(directory)
                run.assert_not_called()

    def test_import_pins_converted_identity_only_after_fresh_probes(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            directory, image, responses = self.imported_fixture(root)
            def validate(receiving_root, profile_path):
                profile = load_json(receiving_root / profile_path)
                self.assertEqual(profile['container']['image'], image)
                self.assertFalse(load_json(root / 'build/toolchain/runtime.json')['validated'])
                write_json(root / profile['validation'], {'validated': True, 'probe_sha256': {'arm': 'known-probe'}})
            with patch.object(toolchain, 'ROOT', root), patch.object(toolchain.subprocess, 'run', side_effect=responses), patch('tools.validate_compiler.validate', side_effect=validate):
                toolchain.import_runtime(directory)
            self.assertTrue(load_json(root / 'build/toolchain/runtime.json')['validated'])
            self.assertEqual(load_json(root / 'build/toolchain/compiler.json')['container']['image'], image)

    def test_import_probe_difference_cannot_keep_validation(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            directory, _, responses = self.imported_fixture(root)
            def validate(receiving_root, profile_path):
                write_json(root / 'build/toolchain/validation.json', {'validated': True, 'probe_sha256': {'arm': 'different'}})
            with patch.object(toolchain, 'ROOT', root), patch.object(toolchain.subprocess, 'run', side_effect=responses), patch('tools.validate_compiler.validate', side_effect=validate):
                with self.assertRaisesRegex(ToolError, 'probes differ'):
                    toolchain.import_runtime(directory)
            self.assertFalse(load_json(root / 'build/toolchain/runtime.json')['validated'])
            self.assertFalse(load_json(root / 'build/toolchain/validation.json')['validated'])

    def test_incomplete_installation_cannot_keep_old_runtime_validation(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            write_json(root / 'build/toolchain/runtime.json', {'validated': True})
            with patch.object(toolchain, 'ROOT', root), patch.object(toolchain, 'fetch'):
                with self.assertRaisesRegex(ToolError, 'Historical installation is incomplete'):
                    toolchain.package()
            self.assertFalse(load_json(root / 'build/toolchain/runtime.json')['validated'])
            with patch.object(toolchain, 'ROOT', root):
                with self.assertRaisesRegex(ToolError, 'Refusing to export an unvalidated runtime'):
                    toolchain.export()


class LinkerImportTests(unittest.TestCase):
    def imported_fixture(self, root):
        directory = root / 'artifact'
        directory.mkdir()
        (directory / 'runtime-image.tar.gz').write_bytes(b'synthetic linker archive')
        template = {'family': 'ld64', 'validation': 'build/linker/validation.json'}
        write_json(root / 'config/linker.json', template)
        exported_image, receiving_image = ('sha256:' + char * 64 for char in ('a', 'b'))
        profile = dict(template, template_sha256=digest(template),
                       container={'image': exported_image, 'linker': '/opt/bin/ld'})
        previous_identity = {'family': 'ld64', 'version': 'ld64-134.9', 'binary_sha256': 'c' * 64,
                             'invocation_sha256': digest(profile['container'])}
        probes = [{'language': 'c', 'mode': 'arm', 'image_sha256': 'd' * 64,
                   'structure': {'entry': 'start', 'architecture': 'armv7'}}]
        write_json(directory / 'linker.json', profile)
        write_json(directory / 'validation.json', {'validated': True, 'identity': previous_identity, 'probes': probes})
        write_json(directory / 'runtime-export.json', {'validated': True, 'image': exported_image,
                   'sha256': sha256(directory / 'runtime-image.tar.gz'), 'profile_sha256': digest(profile)})
        received_identity = dict(previous_identity, invocation_sha256='receiving-invocation')
        responses = [subprocess.CompletedProcess([], 0, stdout='Loaded image ID: ' + receiving_image + '\n'),
                     subprocess.CompletedProcess([], 0, stdout=json.dumps([{'Id': receiving_image, 'Os': 'linux', 'Architecture': 'amd64'}]))]
        return directory, receiving_image, received_identity, probes, responses

    def test_converted_image_is_pinned_after_identical_binary_and_fresh_probes(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            directory, image, identity, probes, responses = self.imported_fixture(root)
            def validate(profile_path, compiler_path, sdk):
                self.assertEqual(load_json(root / profile_path)['container']['image'], image)
                self.assertFalse(load_json(root / 'build/linker/validation.json')['validated'])
                write_json(root / 'build/linker/validation.json', {'validated': True, 'identity': identity, 'probes': probes})
            with patch.object(linker, 'ROOT', root), patch.object(linker.subprocess, 'run', side_effect=responses), patch.object(linker, 'identity', return_value=identity), patch.object(linker, 'validate', side_effect=validate):
                linker.import_runtime(directory, 'compiler.json', 'sdk')
            imported = load_json(root / 'build/linker/runtime-import.json')
            self.assertTrue(imported['validated'])
            self.assertTrue(imported['identical_probes'])
            self.assertEqual(imported['receiving_image'], image)
            self.assertNotEqual(imported['exported_image'], image)

    def test_changed_linked_image_clears_receiving_validation(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            directory, _, identity, probes, responses = self.imported_fixture(root)
            write_json(root / 'build/linker/runtime-import.json', {'validated': True})
            probes[0]['image_sha256'] = 'e' * 64
            def validate(*args):
                write_json(root / 'build/linker/validation.json', {'validated': True, 'identity': identity, 'probes': probes})
            with patch.object(linker, 'ROOT', root), patch.object(linker.subprocess, 'run', side_effect=responses), patch.object(linker, 'identity', return_value=identity), patch.object(linker, 'validate', side_effect=validate):
                with self.assertRaisesRegex(ToolError, 'probes differ'):
                    linker.import_runtime(directory, 'compiler.json', 'sdk')
            self.assertFalse(load_json(root / 'build/linker/validation.json')['validated'])
            self.assertFalse(load_json(root / 'build/linker/runtime-import.json')['validated'])

    def test_changed_linker_binary_is_rejected_before_probe_validation(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            directory, _, identity, _, responses = self.imported_fixture(root)
            identity['binary_sha256'] = 'e' * 64
            with patch.object(linker, 'ROOT', root), patch.object(linker.subprocess, 'run', side_effect=responses), patch.object(linker, 'identity', return_value=identity), patch.object(linker, 'validate') as validate:
                with self.assertRaisesRegex(ToolError, 'binary fingerprint differs'):
                    linker.import_runtime(directory, 'compiler.json', 'sdk')
                validate.assert_not_called()
            self.assertFalse(load_json(root / 'build/linker/validation.json')['validated'])

    def test_corrupt_linker_archive_is_rejected_before_docker(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            directory, _, _, _, _ = self.imported_fixture(root)
            (directory / 'runtime-image.tar.gz').write_bytes(b'corrupt')
            with patch.object(linker, 'ROOT', root), patch.object(linker.subprocess, 'run') as run:
                with self.assertRaisesRegex(ToolError, 'archive/manifest identity differs'):
                    linker.import_runtime(directory, 'compiler.json', 'sdk')
                run.assert_not_called()
