import copy
import json
import os
import plistlib
import shutil
import subprocess
import sys
import tempfile
import time
import unittest
from unittest.mock import patch
from pathlib import Path
from tools.pirates.macho import MachO
from tools.pirates.inventory import recover
from tools.pirates.configure import configure
from tools.pirates.build import comparisons
from tools.pirates.compiler import profile_digest
from tools.pirates.report import regression, objdiff_adapter
from tools.pirates.util import write_json, load_json, ToolError, ninja_command
from tools.ci import compiler_profile_failure
from tests.fixtures import reference, macho, text, executable_symbols

REPO = Path(__file__).resolve().parents[1]


def fixture_workspace(root, compiler='clang'):
    """Actual modern cross-compilation, explicitly synthetic."""
    root = Path(root)
    shutil.copytree(REPO / 'tools', root / 'tools', ignore=shutil.ignore_patterns('__pycache__'))
    shutil.copy(REPO / 'configure.py', root / 'configure.py')
    (root / 'config').mkdir(exist_ok=True)
    (root / 'src').mkdir(exist_ok=True)
    (root / 'src/value.h').write_text('#define VALUE 41\n')
    (root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x) { return x + VALUE; }\n')
    command = [compiler, '-target', 'armv7-apple-ios4.2', '-O2', '-mthumb']
    subprocess.run(command + ['-c', 'src/probe.c', '-o', 'fixture.o'], cwd=root, check=True, capture_output=True)
    candidate = MachO((root / 'fixture.o').read_bytes())
    symbol = next(s for s in candidate.symbols if s.name == '_probe')
    sec = candidate.section(symbol.section)
    data = candidate.bytes_at(symbol.value & ~1, sec.size - ((symbol.value & ~1) - sec.address), symbol.section)
    (root / 'fixture.macho').write_bytes(reference(data, 'thumb'))
    inv = recover(MachO((root / 'fixture.macho').read_bytes()))
    profile = {'name': 'modern-clang-synthetic-only', 'family': 'clang-fixture', 'command': [compiler],
               'flags': command[1:], 'sdk_required': False}
    write_json(root / 'config/fixture-compiler.json', profile)
    manifest = {'version': 1, 'units': [{'group_id': inv['groups'][0]['id'], 'source': 'src/probe.c', 'flags': []}]}
    write_json(root / 'config/candidates.json', manifest)
    write_json(root / 'config/identity.json', {})  # unused with synthetic fixtures
    configure(root, fixture=root / 'fixture.macho', profile='config/fixture-compiler.json')
    return inv['functions'][0]['id']


@unittest.skipUnless(shutil.which('clang') and shutil.which('ninja'), 'Clang and Ninja are required for synthetic workflow tests')
class WorkflowTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='pirates fixtures ')
        self.root = Path(self.temp.name)
        self.fid = fixture_workspace(self.root)

    def tearDown(self):
        self.temp.cleanup()

    def build(self, ok=True):
        run = subprocess.run(['ninja'], cwd=self.root, capture_output=True, text=True)
        self.assertEqual(run.returncode == 0, ok, run.stdout + run.stderr)
        return load_json(self.root / 'build/report.json'), run

    def test_source_header_incremental_and_compile_failure_recovery(self):
        base, _ = self.build()
        self.assertEqual(base['functions'][0]['status'], 'matched', base)
        nochange, run = self.build()
        self.assertIn('no work to do', run.stdout)
        self.assertEqual(base, nochange)
        status = next((self.root / 'build/units').glob('*.compile.json'))
        before = status.stat().st_mtime_ns
        time.sleep(.02)
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        head, _ = self.build()
        self.assertGreater(status.stat().st_mtime_ns, before)
        self.assertEqual(head['functions'][0]['status'], 'different')
        delta = regression(base, head)
        self.assertIn(self.fid, delta['regressions'])
        self.assertTrue(delta['failures'])
        (self.root / 'src/value.h').write_text('#define VALUE 41\n')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')
        (self.root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x) { return x + VALUE + 1; }\n')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'different')
        (self.root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x) { return x + VALUE; }\n')
        (self.root / 'src/value.h').write_text('#define VALUE this is invalid\n')
        failed, _ = self.build(ok=False)
        self.assertEqual(failed['functions'][0]['status'], 'compile_error')
        self.assertEqual(failed['metrics']['matched_functions'], 0)
        self.assertFalse(list((self.root / 'build/units').glob('*.o')))
        self.assertTrue(list((self.root / 'build/units').glob('*.diagnostics.txt')))
        (self.root / 'src/value.h').write_text('#define VALUE 41\n')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')

    def test_original_byte_body_cannot_earn_match_credit(self):
        from tools.pirates.compare import compare_function
        base, _ = self.build()
        original = MachO((self.root / 'fixture.macho').read_bytes())
        raw = original.bytes_at(0x1000, original.sections[0].size, 1)
        payload = '.text\n.globl _probe\n.thumb\n.thumb_func _probe\n_probe:\n.byte ' + ','.join(str(b) for b in raw) + '\n'
        (self.root / 'src/probe.c').write_text('__asm__(' + json.dumps(payload) + ');\n')
        # Establish the exploit: compiling these bytes directly yields a match.
        subprocess.run(['clang', '-target', 'armv7-apple-ios4.2', '-c', 'src/probe.c', '-o', 'shortcut.o'],
                       cwd=self.root, check=True, capture_output=True)
        inventory = recover(original)
        shortcut = MachO((self.root / 'shortcut.o').read_bytes())
        self.assertEqual(compare_function(original, inventory, inventory['functions'][0], shortcut)['status'], 'matched')
        # The normal build must fail and remove the previous successful object.
        failed, _ = self.build(ok=False)
        self.assertEqual(failed['metrics']['matched_functions'], 0)
        self.assertEqual(failed['functions'][0]['status'], 'compile_error')
        self.assertFalse(failed['functions'][0]['byte_verified'])
        self.assertFalse(list((self.root / 'build/units').glob('*.o')))
        diagnostics = next((self.root / 'build/units').glob('*.diagnostics.txt')).read_text()
        self.assertIn('assembly statements and original-byte payloads', diagnostics)
        self.assertTrue(regression(base, failed)['failures'])
        (self.root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x) { return x + VALUE; }\n')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')

    def test_macro_payload_in_forced_include_cannot_bypass_policy(self):
        self.build()
        (self.root / 'src/payload.h').write_text('#define EMIT(name, body) __##name##__(body)\nEMIT(asm, ".word 0xe12fff1e\\n")\n')
        manifest = load_json(self.root / 'config/candidates.json')
        manifest['units'][0]['flags'] = ['-include', 'src/payload.h']
        write_json(self.root / 'config/candidates.json', manifest)
        configure(self.root)
        failed, _ = self.build(ok=False)
        self.assertEqual(failed['metrics']['matched_functions'], 0)
        diagnostics = next((self.root / 'build/units').glob('*.diagnostics.txt')).read_text()
        self.assertIn('src/payload.h:2', diagnostics)
        self.assertIn('Source policy violation', diagnostics)
        # A rejected include remains a Ninja dependency, so editing it repairs
        # the unit even when the main source and flags remain unchanged.
        (self.root / 'src/payload.h').write_text('/* recovered source only */\n')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')

    def test_cached_object_without_current_policy_cannot_earn_credit(self):
        self.build()
        path = next((self.root / 'build/units').glob('*.compile.json'))
        compiled = load_json(path)
        for evidence in (None, {'version': 0, 'validated': True}, {'version': 1, 'validated': False}):
            compiled['source_policy'] = evidence
            write_json(path, compiled)
            result = comparisons(self.root, self.fid)
            self.assertEqual(result['status'], 'unresolved')
            self.assertFalse(result['byte_verified'])
            self.assertTrue(any('source policy validation' in reason for reason in result['reasons']))
        from tools.pirates.build import report
        self.assertEqual(report(self.root), 1)

    def test_reports_deterministic_across_workspace_paths(self):
        first, _ = self.build()
        with tempfile.TemporaryDirectory(prefix='pirates other host ') as tmp:
            fixture_workspace(Path(tmp))
            subprocess.run(['ninja'], cwd=tmp, check=True, capture_output=True)
            second = load_json(Path(tmp) / 'build/report.json')
            self.assertEqual(first, second)
            self.assertEqual((self.root / 'build/objdiff-report.json').read_bytes(), (Path(tmp) / 'build/objdiff-report.json').read_bytes())

    def test_header_or_flags_changed_cannot_verify_a_stale_object(self):
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        result = comparisons(self.root, self.fid)
        self.assertEqual(result['status'], 'unresolved')
        self.assertTrue(any('dependency changed' in reason for reason in result['reasons']))
        (self.root / 'src/value.h').write_text('#define VALUE 41\n')
        manifest = load_json(self.root / 'config/candidates.json')
        manifest['units'][0]['flags'] = ['-O0']
        write_json(self.root / 'config/candidates.json', manifest)
        configure(self.root)
        result = comparisons(self.root, self.fid)
        self.assertEqual(result['status'], 'unresolved')
        self.assertTrue(any('compile configuration changed' in reason for reason in result['reasons']))

    def test_partial_unit_preserves_missing_functions_on_compile_failure(self):
        original = MachO((self.root / 'fixture.macho').read_bytes())
        data = original.bytes_at(0x1000, original.sections[0].size, 1)
        size = len(data)
        funcs = [('_probe', 0x1000, size, 'thumb', 'unity'),
                 ('_missing', 0x1000 + size, size, 'thumb', 'unity')]
        (self.root / 'fixture.macho').write_bytes(macho([text(data + data, 0x1000)], executable_symbols(funcs), filetype=2))
        manifest = load_json(self.root / 'config/candidates.json')
        manifest['units'][0]['implemented_functions'] = [self.fid]
        write_json(self.root / 'config/candidates.json', manifest)
        configure(self.root, fixture=self.root / 'fixture.macho', profile='config/fixture-compiler.json')
        before, _ = self.build()
        statuses = {f['symbol']: f for f in before['functions']}
        self.assertEqual(statuses['_probe']['status'], 'matched')
        self.assertEqual(statuses['_missing']['status'], 'missing')
        self.assertIsNone(statuses['_missing']['candidate_source'])
        (self.root / 'src/probe.c').write_text('invalid source\n')
        failed, _ = self.build(ok=False)
        self.assertEqual({f['symbol']: f['status'] for f in failed['functions']},
                         {'_probe': 'compile_error', '_missing': 'missing'})
        self.assertEqual(failed['metrics']['missing_candidates'], 1)
        self.assertTrue(regression(before, failed)['failures'])
        manifest['units'][0]['implemented_functions'] = ['unknown']
        write_json(self.root / 'config/candidates.json', manifest)
        with self.assertRaisesRegex(ToolError, 'original object group'):
            configure(self.root)

    def test_doctor_reports_missing_dependencies_without_import_failure(self):
        run = subprocess.run([sys.executable, '-S', 'tools/dev.py', 'doctor', '--json'],
                             cwd=self.root, capture_output=True, text=True)
        self.assertEqual(run.returncode, 1, run.stderr)
        checks = {c['name']: c for c in json.loads(run.stdout)}
        self.assertFalse(checks['capstone']['ok'])
        self.assertIn('Install requirements.txt', checks['capstone']['detail'])

    def test_c_source_retains_c_abi_with_cxx_driver(self):
        driver = shutil.which('clang++')
        if not driver:
            self.skipTest('clang++ is required')
        profile = load_json(self.root / 'config/fixture-compiler.json')
        profile['command'] = [driver]
        write_json(self.root / 'config/fixture-compiler.json', profile)
        configure(self.root)
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')
        editor = load_json(self.root / 'compile_commands.json')[0]['arguments']
        self.assertEqual(editor[1:3], ['-x', 'c'])

    def test_editor_command_cannot_replace_matching_compiler(self):
        profile = load_json(self.root / 'config/fixture-compiler.json')
        profile['editor_command'] = ['syntax-editor-only', '--target=armv7-apple-ios4.2']
        write_json(self.root / 'config/fixture-compiler.json', profile)
        configure(self.root)
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')
        editor = load_json(self.root / 'compile_commands.json')[0]['arguments']
        self.assertEqual(editor[0], 'syntax-editor-only')
        self.assertIn('-std=gnu89', editor)
        original = {k: v for k, v in profile.items() if k != 'editor_command'}
        self.assertEqual(profile_digest(original), profile_digest(profile))

    def test_changed_driver_arguments_invalidate_stale_match(self):
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')
        profile = load_json(self.root / 'config/fixture-compiler.json')
        profile['command'] += ['-DVALUE=99']
        write_json(self.root / 'config/fixture-compiler.json', profile)
        configure(self.root)
        result = comparisons(self.root, self.fid)
        self.assertEqual(result['status'], 'unresolved')
        self.assertTrue(any('Compiler fingerprint changed' in reason for reason in result['reasons']))

    def test_explicit_profile_switch_uses_existing_verified_inputs(self):
        self.build()
        profile = load_json(self.root / 'config/fixture-compiler.json')
        profile['flags'] += ['-O0']
        write_json(self.root / 'config/other-compiler.json', profile)
        configure(self.root, profile='config/other-compiler.json')
        self.assertEqual(load_json(self.root / 'build/config.json')['profile_path'], 'config/other-compiler.json')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'different')
        configure(self.root)
        self.assertEqual(load_json(self.root / 'build/config.json')['profile_path'], 'config/other-compiler.json')

    def test_shared_compiler_change_cannot_silently_reset_existing_candidate_baseline(self):
        path = 'config/fixture-compiler.json'
        profile = load_json(self.root / path)
        self.assertIsNone(compiler_profile_failure(self.root, profile, path))
        profile['flags'] += ['-O0']
        self.assertIn('compiler profile changed', compiler_profile_failure(self.root, profile, path))
        write_json(self.root / 'config/candidates.json', {'version': 1, 'units': []})
        self.assertIsNone(compiler_profile_failure(self.root, profile, path))

    def test_doctor_rejects_wrong_or_malformed_sdk_metadata(self):
        from tools.dev import doctor
        sdk = self.root / 'local.sdk'
        sdk.mkdir()
        system = sdk / 'System/Library/CoreServices/SystemVersion.plist'
        system.parent.mkdir(parents=True)
        system.write_bytes(plistlib.dumps({'ProductVersion': '5.1', 'ProductBuildVersion': '9B176'}))
        profile = load_json(self.root / 'config/fixture-compiler.json')
        profile['sdk_required'] = True
        write_json(self.root / 'config/fixture-compiler.json', profile)
        configure(self.root, sdk=sdk)
        for version, expected in [('5.2', False), ('5.1', True)]:
            (sdk / 'SDKSettings.plist').write_bytes(plistlib.dumps({'Version': version, 'CanonicalName': 'iphoneos' + version}))
            check = next(c for c in doctor(self.root) if c['name'] == 'iOS SDK 5.1')
            self.assertEqual(check['ok'], expected, check)
        (sdk / 'SDKSettings.plist').write_bytes(b'<invalid')
        check = next(c for c in doctor(self.root) if c['name'] == 'iOS SDK 5.1')
        self.assertFalse(check['ok'])
        self.assertIn('Malformed', str(check['detail']))

    def test_generated_runtime_retains_template_baseline_guard(self):
        profile = load_json(self.root / 'config/fixture-compiler.json')
        generated = {**profile, 'template_path': 'config/fixture-compiler.json', 'template_sha256': profile_digest(profile)}
        generated['command'] = ['a-generated-immutable-runtime']
        self.assertIsNone(compiler_profile_failure(self.root, generated, 'build/generated.json'))
        generated['template_sha256'] = profile_digest({**profile, 'flags': ['-O0']})
        self.assertIn('compiler profile changed', compiler_profile_failure(self.root, generated, 'build/generated.json'))

    def test_freestanding_unit_can_explicitly_omit_sdk(self):
        profile = load_json(self.root / 'config/fixture-compiler.json')
        profile['sdk_required'] = True
        write_json(self.root / 'config/fixture-compiler.json', profile)
        self.assertEqual(self.build(ok=False)[0]['functions'][0]['status'], 'compile_error')
        manifest = load_json(self.root / 'config/candidates.json')
        manifest['units'][0]['sdk_required'] = False
        write_json(self.root / 'config/candidates.json', manifest)
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')
        manifest['units'][0]['sdk_required'] = True
        write_json(self.root / 'config/candidates.json', manifest)
        configure(self.root)
        self.assertEqual(comparisons(self.root, self.fid)['status'], 'unresolved')

    def test_sdk_system_header_edit_invalidates_then_incrementally_rebuilds(self):
        from tests.test_sdk import sdk_fixture
        sdk = sdk_fixture(self.root / 'local.sdk')
        (self.root / 'src/probe.c').write_text('#include <value.h>\nint probe(int x){return x+SDK_VALUE;}\n')
        configure(self.root, sdk=sdk)
        base, _ = self.build()
        self.assertEqual(base['functions'][0]['status'], 'matched')
        commands = load_json(self.root / 'compile_commands.json')
        self.assertIn(str(sdk.resolve()) + '/usr/include', commands[0]['arguments'])
        (sdk / 'usr/include/value.h').write_text('#define SDK_VALUE 42\n')
        self.assertEqual(comparisons(self.root, self.fid)['status'], 'unresolved')
        changed, run = self.build()
        self.assertIn('COMPILE', run.stdout)
        self.assertEqual(changed['functions'][0]['status'], 'different')
        self.assertNotEqual(base['sdk'], changed['sdk'])
        self.assertIn('identical SDK', str(regression(base, changed)['failures']))
        unchanged, run = self.build()
        self.assertNotIn('COMPILE', run.stdout)
        self.assertEqual(changed, unchanged)

    def test_sdk_baseline_change_requires_explicit_migration(self):
        from tools.ci import sdk_profile_failure
        write_json(self.root / 'config/sdk-lock.json', {'manifest_sha256': 'old', 'version': '5.1', 'build': '9B176'})
        with tempfile.TemporaryDirectory() as other:
            write_json(Path(other) / 'config/sdk-lock.json', {'manifest_sha256': 'new', 'version': '5.1', 'build': '9B176'})
            self.assertIn('SDK baseline', sdk_profile_failure(self.root, other))
        write_json(self.root / 'config/candidates.json', {'version': 1, 'units': []})
        self.assertIsNone(sdk_profile_failure(self.root, self.root))

    def test_unvalidated_language_cannot_use_a_valid_cxx_compiler_gate(self):
        from tools.pirates.compiler import permitted
        config = {'compiler_fingerprint': {'validated': True, 'languages': ['c', 'c++']},
                  'compiler': {'family': 'llvmgcc42'}, 'provenance': {'kind': 'game'}}
        self.assertTrue(permitted(config, 'probe.cpp')[0])
        self.assertFalse(permitted(config, 'probe.m')[0])
        self.assertFalse(permitted(config, 'probe.mm')[0])

    def test_flag_experiments_preserve_the_active_manifest_and_report(self):
        from tools.flags import sweep
        before, _ = self.build()
        manifest = (self.root / 'config/candidates.json').read_bytes()
        evidence = sweep(self.root, self.fid, optimizations=['-O0', '-O2'], modes=['thumb'])
        self.assertEqual([e['status'] for e in evidence['experiments']], ['different', 'matched'])
        self.assertIn('src/value.h', evidence['experiments'][1]['dependencies'])
        self.assertEqual(manifest, (self.root / 'config/candidates.json').read_bytes())
        self.assertEqual(before, load_json(self.root / 'build/report.json'))

    def test_internal_ninja_is_pinned_independently_of_shell_path(self):
        with patch.dict(os.environ, {'PATH': '/nonexistent-pirates-tools'}):
            run = subprocess.run(ninja_command() + ['--version'], capture_output=True, text=True, check=True)
        self.assertTrue(run.stdout.startswith('1.11.1'), run.stdout)

    def test_adapter_has_proto_json_types_and_linking_stays_zero(self):
        report, _ = self.build()
        adapter = objdiff_adapter(report)
        self.assertIsInstance(adapter['measures']['totalCode'], str)
        self.assertEqual(adapter['measures']['matchedFunctions'], 1)
        self.assertEqual(adapter['measures']['completeCode'], '0')
        self.assertEqual(adapter['units'][0]['metadata']['complete'], False)
        self.assertEqual(adapter['units'][0]['functions'][0]['address'], '0')

    def test_data_source_header_rebuild_and_regression(self):
        # A real compiled C array; the independently encoded original includes
        # its padding/allocation and STABS object ownership.
        (self.root / 'src/probe.c').write_text('#include "value.h"\nint table[2]={VALUE,99};\nint probe(int x){return x+VALUE;}\n')
        cmd = [shutil.which('clang'), '-target', 'armv7-apple-ios4.2', '-O2', '-mthumb', '-c', 'src/probe.c', '-o', 'data-fixture.o']
        subprocess.run(cmd, cwd=self.root, check=True, capture_output=True)
        obj = MachO((self.root / 'data-fixture.o').read_bytes())
        code = next(s for s in obj.symbols if s.name == '_probe' and s.defined)
        table = next(s for s in obj.symbols if s.name == '_table' and s.defined)
        text_sec, data_sec = obj.section(code.section), obj.section(table.section)
        raw = obj.bytes_at(code.value & ~1, text_sec.size, code.section)
        value = obj.bytes_at(table.value, data_sec.size, table.section)
        records = executable_symbols([('_probe', 0x1000, len(raw), 'thumb', 'unity')], [('_table', 0x0f, 2, 0, 0x2000)])
        records.insert(3, ('_table', 0x26, 2, 0, 0x2000))
        (self.root / 'fixture.macho').write_bytes(macho([text(raw, 0x1000), ('__DATA', '__data', 0x2000, value, 0, [])], records, filetype=2))
        configure(self.root, fixture=self.root / 'fixture.macho', profile='config/fixture-compiler.json')
        base, _ = self.build()
        self.assertEqual(base['data_metrics']['matched_bytes'], 8)
        self.assertEqual(base['data'][0]['ownership'], 'STABS')
        self.assertEqual(objdiff_adapter(base)['measures']['totalData'], '8')
        self.assertEqual(objdiff_adapter(base)['measures']['matchedData'], '8')
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        self.assertFalse(comparisons(self.root, base['data'][0]['id'])['byte_verified'])
        head, run = self.build()
        self.assertIn('COMPILE', run.stdout)
        self.assertEqual(head['data_metrics']['matched_bytes'], 0)
        self.assertTrue(any('Verified data match regressed' in f for f in regression(base, head)['failures']))

    def test_linker_baseline_cannot_be_silently_reset(self):
        from tools.ci import linker_profile_failure
        write_json(self.root / 'config/linker.json', {'family': 'ld64', 'name': 'original'})
        write_json(self.root / 'config/link.json', {'version': 1, 'enabled': True})
        with tempfile.TemporaryDirectory() as other:
            write_json(Path(other) / 'config/linker.json', {'family': 'ld64', 'name': 'changed'})
            self.assertIn('linker baseline', linker_profile_failure(self.root, other))

    def test_deleted_object_is_incrementally_rebuilt(self):
        base, _ = self.build()
        uid = base['functions'][0]['group_id']
        (self.root / f'build/units/{uid}.o').unlink()
        self.assertFalse(comparisons(self.root, self.fid)['byte_verified'])
        head, run = self.build()
        self.assertIn('COMPILE', run.stdout)
        self.assertEqual(head['metrics']['matched_functions'], 1)

    def test_inventory_loss_and_candidate_compile_failure_fail_regression_check(self):
        base, _ = self.build()
        head = copy.deepcopy(base)
        head['functions'] = []
        self.assertTrue(any('coverage disappeared' in f for f in regression(base, head)['failures']))
        head = copy.deepcopy(base)
        head['functions'][0]['status'] = 'compile_error'
        self.assertTrue(any('does not compile' in f for f in regression(base, head)['failures']))

    def test_clang_never_validates_historical_matching(self):
        profile = load_json(self.root / 'config/fixture-compiler.json')
        profile.update(family='llvmgcc42', validation='missing-validation.json')
        write_json(self.root / 'config/fixture-compiler.json', profile)
        configure(self.root)
        report, _ = self.build(ok=False)
        self.assertFalse(report['compiler']['validated'])
        self.assertEqual(report['metrics']['matched_functions'], 0)

    def test_missing_candidates_visible(self):
        write_json(self.root / 'config/candidates.json', {'version': 1, 'units': []})
        report, _ = self.build()
        self.assertEqual(report['metrics']['missing_candidates'], 1)
        self.assertEqual(report['metrics']['matched_functions'], 0)

    def test_corrupt_original_rejected(self):
        self.build()
        (self.root / 'build/inputs/Pirates').write_bytes(b'not the original')
        run = subprocess.run([sys.executable, 'tools/build.py', 'report'], cwd=self.root, capture_output=True, text=True)
        self.assertNotEqual(run.returncode, 0)
        self.assertIn('hash mismatch', run.stderr)

    def test_manifest_paths_cannot_escape_workspace(self):
        manifest = load_json(self.root / 'config/candidates.json')
        manifest['units'][0]['source'] = '../outside.cpp'
        write_json(self.root / 'config/candidates.json', manifest)
        with self.assertRaises(ToolError):
            configure(self.root)
