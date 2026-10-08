#!/usr/bin/env python3
"""SDK-dependent object probes for all validated historical languages."""
import argparse
import subprocess
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.compiler import command, fingerprint, language_flags
from tools.pirates.sdk import require_sdk, header_flags
from tools.pirates.macho import MachO
from tools.pirates.util import ToolError, load_json, write_json, sha256
ROOT = Path(__file__).resolve().parents[1]


def validate(root, profile_path, sdk):
    root, sdk = Path(root), Path(sdk).resolve()
    profile = load_json(root / profile_path)
    info = fingerprint(profile, root, sdk)
    if profile['family'] != 'llvmgcc42' or not info['validated']:
        raise ToolError('SDK probes require a validated historical compiler')
    identity = require_sdk(sdk)
    output = root / 'build/sdk-validation.json'
    write_json(output, {'validated': False, 'reason': 'SDK probes have not completed'})
    bodies = {
        'c': '#include <stdlib.h>\n#include <OpenGLES/ES2/gl.h>\nint sdk_probe(const char *text){return (int)strtol(text,0,10)+(int)glCreateShader(GL_VERTEX_SHADER);}',
        'cpp': '#include <vector>\n#include <string>\nextern "C" unsigned sdk_probe(const char *text){std::vector<int> values(3,41);std::string value(text);return values[1]+value.size();}',
        'm': '#import <Foundation/Foundation.h>\nint sdk_probe(id object){return [[object description] length];}',
        'mm': '#import <Foundation/Foundation.h>\n#include <vector>\nextern "C" unsigned sdk_probe(id object){std::vector<int> values(3,41);return values[1]+[[object description] length];}'
    }
    probes = []
    for extension, body in bodies.items():
        for mode in ('arm', 'thumb'):
            hashes = []
            for repeat in ('a', 'b'):
                folder = root / 'build/sdk-probes' / extension / mode / repeat
                folder.mkdir(parents=True, exist_ok=True)
                source = folder / ('probe.' + extension)
                source.write_text('#include "value.h"\n' + body + '\n')
                (folder / 'value.h').write_text('#define PROBE_HEADER 1\n')
                flags = language_flags(source) + profile['flags'] + ['-O2', '-m' + mode] + header_flags(profile, sdk, source)
                result = subprocess.run(command(profile, root, sdk) + flags + ['-MMD', '-MF', str((folder / 'probe.d').relative_to(root)),
                    '-c', str(source.relative_to(root)), '-o', str((folder / 'probe.o').relative_to(root))], cwd=root,
                    capture_output=True, text=True, timeout=300)
                (folder / 'diagnostics.txt').write_text(result.stdout + result.stderr)
                if result.returncode:
                    raise ToolError('SDK ' + extension + '/' + mode + ' compilation failed; see ' + str(folder.relative_to(root)))
                obj = MachO((folder / 'probe.o').read_bytes())
                if obj.filetype != 1 or not any(s.name == '_sdk_probe' and s.defined and s.thumb == (mode == 'thumb') for s in obj.symbols):
                    raise ToolError('SDK probe has unexpected object/function mode')
                if 'value.h' not in (folder / 'probe.d').read_text():
                    raise ToolError('SDK compiler probe did not emit dependencies')
                hashes.append(sha256(folder / 'probe.o'))
            if hashes[0] != hashes[1]:
                raise ToolError('SDK objects differ across build directories: ' + extension + '/' + mode)
            probes.append({'language': language_flags(source)[1], 'mode': mode, 'sha256': hashes[0],
                           'flags': flags, 'source_sha256': sha256(source)})
    write_json(output, {'validated': True, 'sdk': identity, 'compiler': info, 'probes': probes,
                       'scope': 'Object compilation; no linking or exact original compiler equivalence claimed'})
    print('Validated SDK-dependent C, C++, Objective-C and Objective-C++ ARM/Thumb objects.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--profile', default='build/toolchain/compiler.json')
    parser.add_argument('--sdk', type=Path, default=ROOT / 'build/sdk/iPhoneOS5.1.sdk')
    args = parser.parse_args()
    try:
        validate(ROOT, args.profile, args.sdk)
    except (ToolError, OSError, subprocess.SubprocessError) as error:
        sys.exit(str(error))
