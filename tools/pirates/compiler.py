import hashlib
import json
import shutil
import re
import subprocess
from pathlib import Path
from .util import ToolError, load_json, sha256


def command(profile, root, sdk=None):
    if 'container' in profile:
        image = profile['container'].get('image')
        if not image or not re.fullmatch(r'(?:[^\s]+@)?sha256:[0-9a-f]{64}', image):
            raise ToolError('Pinned historical Linux image is not validated; see toolchain/feasibility.json')
        cmd = ['docker', 'run', '--rm', '--platform', 'linux/amd64', '--network', 'none',
               '-v', str(Path(root).resolve()) + ':/work', '-w', '/work']
        if sdk:
            cmd += ['-v', str(sdk) + ':/sdk:ro']
        return cmd + [image, profile['container']['compiler']]
    replacements = {'root': str(Path(root).resolve()), 'sdk': str(sdk or '')}
    return [arg.format(**replacements) for arg in profile['command']]


def fingerprint(profile, root, sdk=None):
    result = {'family': profile['family'], 'profile': profile['name'], 'validated': False,
              'version': None, 'sha256': None, 'reason': None}
    try:
        cmd = command(profile, root, sdk)
        container = profile.get('container')
        if container:
            result['sha256'] = container['image'].split('sha256:', 1)[1]
        binary = shutil.which(cmd[0]) or (Path(root) / cmd[0])
        if not Path(binary).is_file():
            raise ToolError('Compiler executable is missing: ' + cmd[0])
        if not container:
            result['sha256'] = sha256(binary)
        run = subprocess.run(cmd + ['--version'], cwd=root, capture_output=True, text=True, timeout=20)
        if run.returncode:
            raise ToolError('Compiler --version failed')
        result['version'] = run.stdout.strip()
        if profile['family'] == 'clang-fixture':
            result['reason'] = 'Modern Clang is for synthetic fixtures only'
            return result
        if profile['family'] != 'llvmgcc42':
            raise ToolError('Unsupported matching compiler family')
        validation = load_json(Path(root) / profile['validation'])
        if not validation.get('validated') or validation.get('compiler_sha256') != result['sha256']:
            raise ToolError('Compiler has no matching validation fingerprint')
        invocation = profile.get('container') or profile['command']
        if validation.get('command_sha256') != hashlib.sha256(json.dumps(invocation, sort_keys=True, separators=(',', ':')).encode()).hexdigest():
            raise ToolError('Validation belongs to a different compiler invocation')
        if not all(validation.get(k) for k in ('arm_probe', 'thumb_probe', 'reproducible_objects', 'cxx_probe')):
            raise ToolError('Historical compiler smoke probes are incomplete')
        if 'LLVM' not in result['version'] or '2336.9' not in result['version'] or '4.2.1' not in result['version']:
            raise ToolError('Historical compiler version does not match 2336.9 hypothesis')
        result.update(validated=True)
    except (ToolError, FileNotFoundError, OSError, subprocess.TimeoutExpired, ValueError) as e:
        result['reason'] = str(e)
    return result


def permitted(config):
    info = config['compiler_fingerprint']
    if config['provenance']['kind'] == 'synthetic' and config['compiler']['family'] == 'clang-fixture':
        return True, None
    if not info['validated']:
        return False, 'Historical matching compiler is not validated: ' + (info['reason'] or 'missing validation')
    return True, None
