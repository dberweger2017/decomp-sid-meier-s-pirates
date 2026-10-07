from pathlib import Path
import plistlib
import zipfile
import hashlib
from .macho import MachO
from .inventory import recover
from .util import ToolError, sha256, write_json


def import_ipa(path, root, identity):
    actual = sha256(path)
    if actual != identity['ipa_sha256']:
        raise ToolError(f'IPA SHA-256 mismatch: expected {identity["ipa_sha256"]}, got {actual}')
    try:
        with zipfile.ZipFile(path) as archive:
            names = archive.namelist()
            for name in (identity['executable_path'], identity['plist_path']):
                if names.count(name) != 1:
                    raise ToolError('Missing or duplicate IPA member: ' + name)
            data = archive.read(identity['executable_path'])
            plist = plistlib.loads(archive.read(identity['plist_path']))
    except (zipfile.BadZipFile, KeyError, plistlib.InvalidFileException) as e:
        raise ToolError('Malformed IPA: ' + str(e)) from e
    executable_hash = hashlib.sha256(data).hexdigest()
    if executable_hash != identity['executable_sha256']:
        raise ToolError('Executable SHA-256 mismatch')
    for key, expected in {'CFBundleIdentifier': identity['bundle_id'], 'CFBundleVersion': identity['version'],
                          'DTCompiler': identity['observed_build']['compiler'],
                          'DTXcode': identity['observed_build']['xcode'], 'DTSDKName': identity['observed_build']['sdk']}.items():
        if plist.get(key) != expected:
            raise ToolError('IPA metadata mismatch: ' + key)
    macho = MachO(data)
    if macho.filetype != 2:
        raise ToolError('IPA input is not an executable')
    inventory = recover(macho)
    if len(inventory['functions']) != identity['expected_functions'] or len(inventory['groups']) != identity['expected_groups']:
        raise ToolError('Inventory coverage differs from recorded identity')
    target = Path(root) / 'build/inputs/Pirates'
    target.parent.mkdir(parents=True, exist_ok=True)
    if not target.exists() or target.read_bytes() != data:
        tmp = target.with_suffix('.tmp')
        tmp.write_bytes(data)
        tmp.replace(target)
    provenance = {'kind': 'game', 'ipa_sha256': actual, 'executable_sha256': executable_hash,
                  'identity': identity, 'input': 'build/inputs/Pirates', 'cryptid': macho.cryptid, 'uuid': macho.uuid}
    write_json(Path(root) / 'build/inputs/provenance.json', provenance)
    return provenance, inventory


def import_fixture(path, root):
    data = Path(path).read_bytes()
    macho = MachO(data)
    if macho.filetype != 2:
        raise ToolError('Fixture reference must be an executable')
    inventory = recover(macho)
    if not inventory['functions']:
        raise ToolError('Fixture reference has no STABS function records')
    out = Path(root) / 'build/inputs/Pirates'
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(data)
    provenance = {'kind': 'synthetic', 'executable_sha256': hashlib.sha256(data).hexdigest(), 'input': 'build/inputs/Pirates'}
    write_json(Path(root) / 'build/inputs/provenance.json', provenance)
    return provenance, inventory
