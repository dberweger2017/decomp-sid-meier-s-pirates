"""Content-bound SDK identity; no host paths or timestamps in reports."""
import hashlib
import json
import plistlib
from pathlib import Path
from .util import ToolError, sha256

_CACHE = {}


def inspect_sdk(directory):
    if not directory:
        return None
    root = Path(directory).resolve()
    try:
        if not root.is_dir():
            raise ToolError('SDK directory is missing')
        paths = sorted(root.rglob('*'))
        signature = []
        for path in paths:
            stat = path.lstat()
            signature.append((str(path.relative_to(root)), stat.st_mode, stat.st_size,
                              stat.st_mtime_ns, stat.st_ctime_ns, stat.st_ino))
        cached = _CACHE.get(str(root))
        if cached and cached[0] == signature:
            return cached[1].copy()
        files, dangling = {}, []
        for path in paths:
            name = str(path.relative_to(root))
            if path.is_symlink():
                if not path.resolve().is_relative_to(root):
                    raise ToolError('SDK symlink escapes its root: ' + name)
                files[name] = {'symlink': str(path.readlink())}
                if not path.exists():
                    dangling.append(name)
            elif path.is_file():
                files[name] = {'sha256': sha256(path)}
        digest = hashlib.sha256(json.dumps(files, sort_keys=True, separators=(',', ':')).encode()).hexdigest()
        settings = plistlib.loads((root / 'SDKSettings.plist').read_bytes())
        system = plistlib.loads((root / 'System/Library/CoreServices/SystemVersion.plist').read_bytes())
        if not isinstance(settings, dict) or not isinstance(system, dict):
            raise ToolError('Malformed SDK metadata: expected dictionaries')
        if settings.get('Version') != '5.1' or settings.get('CanonicalName') != 'iphoneos5.1':
            raise ToolError('Expected iphoneos5.1 SDK version 5.1')
        if system.get('ProductVersion') != '5.1' or system.get('ProductBuildVersion') != '9B176':
            raise ToolError('Expected iPhoneOS SDK build 9B176')
        result = {'validated': True, 'version': '5.1', 'build': '9B176', 'sha256': digest,
                  'file_records': len(files), 'dangling_symlinks': dangling}
        _CACHE[str(root)] = (signature, result)
        return result.copy()
    except (OSError, ValueError, RuntimeError, plistlib.InvalidFileException) as error:
        return {'validated': False, 'reason': 'Malformed or unavailable SDK: ' + str(error).replace(str(root), '<sdk>')}


def require_sdk(directory, expected=None):
    result = inspect_sdk(directory)
    if not result or not result['validated']:
        raise ToolError((result or {}).get('reason', 'Supply a local iPhoneOS5.1.sdk'))
    if expected and result['sha256'] != expected:
        raise ToolError('SDK content fingerprint differs from the pinned mirror')
    return result


def header_flags(profile, directory, source, editor=False):
    if not directory:
        return []
    prefix = str(directory) if editor or not profile.get('container') else '/sdk'
    flags = ['-isysroot', prefix, '-isystem', prefix + '/usr/include']
    if profile['family'] == 'llvmgcc42' and Path(source).suffix.lower() not in ('.c', '.m'):
        flags += ['-isystem', prefix + '/usr/include/c++/4.2.1',
                  '-isystem', prefix + '/usr/include/c++/4.2.1/backward']
    return flags
