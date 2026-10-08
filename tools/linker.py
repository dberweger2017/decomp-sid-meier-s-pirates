#!/usr/bin/env python3
"""Build/package a pinned Linux ld64 and validate SDK linking independently."""
import argparse
import gzip
import hashlib
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.linking import command, identity, inspect_image, digest
from tools.pirates.compiler import fingerprint as compiler_fingerprint, command as compiler_command, language_flags
from tools.pirates.sdk import require_sdk, header_flags
from tools.pirates.macho import MachO
from tools.pirates.util import load_json, write_json, sha256, ToolError

ROOT = Path(__file__).resolve().parents[1]


def build():
    from tools.toolchain import fetch
    fetch()
    out = ROOT / 'build/linker'
    out.mkdir(parents=True, exist_ok=True)
    write_json(out / 'validation.json', {'validated': False, 'reason': 'Linker build/probes have not completed'})
    def logged(args, name):
        with (out / name).open('w') as log:
            subprocess.run(args, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT, check=True)
    logged(['docker', 'build', '--platform', 'linux/amd64', '-f', 'toolchain/Dockerfile.linker', '-t', 'pirates-linker-host', '.'], 'host-build.log')
    host = json.loads(subprocess.run(['docker', 'image', 'inspect', 'pirates-linker-host'], capture_output=True, text=True, check=True).stdout)[0]['Id']
    source = ROOT / 'build/toolchain/cctools-port-92671d64f56fb903e000b88fb97f13fccbdf413a/cctools'
    if not (source / 'Makefile').exists():
        logged(['docker', 'run', '--rm', '--platform', 'linux/amd64', '--network', 'none', '-v', str(ROOT) + ':/work',
                host, 'sh', '/work/toolchain/build-assembler.sh'], 'configure-build.log')
    logged(['docker', 'run', '--rm', '--platform', 'linux/amd64', '--network', 'none', '-v', str(ROOT) + ':/work',
            host, 'sh', '/work/toolchain/build-linker.sh'], 'source-build.log')
    if (out / 'packages.lock').read_bytes() != (ROOT / 'toolchain/linker-package-versions.lock').read_bytes():
        raise ToolError('Linker host package inventory differs from lock')
    context = out / 'runtime-context'
    shutil.rmtree(context, ignore_errors=True)
    shutil.copytree(out / 'install', context / 'install')
    provenance = context / 'provenance'
    provenance.mkdir()
    for name in ('build-linker.sh', 'Dockerfile.linker', 'Dockerfile.linker-runtime', 'linker-package-versions.lock'):
        shutil.copy(ROOT / 'toolchain' / name, provenance / name)
    shutil.copy(ROOT / 'build/toolchain/cctools-port.tar.gz', provenance / 'cctools-port.tar.gz')
    write_json(provenance / 'installed-artifacts.json', {'bin/ld': sha256(out / 'install/bin/ld'),
               'recipe_sha256': digest({p.name: sha256(p) for p in provenance.iterdir() if p.is_file()})})
    logged(['docker', 'build', '--platform', 'linux/amd64', '-f', str(ROOT / 'toolchain/Dockerfile.linker-runtime'),
            '-t', 'pirates-ld64:validated-probes-pending', str(context)], 'runtime-build.log')
    image = json.loads(subprocess.run(['docker', 'image', 'inspect', 'pirates-ld64:validated-probes-pending'], capture_output=True, text=True, check=True).stdout)[0]['Id']
    template = load_json(ROOT / 'config/linker.json')
    profile = {**template, 'template_sha256': digest(template), 'template_path': 'config/linker.json',
               'container': {'image': image, 'linker': '/opt/pirates-linker/bin/ld'}}
    profile.pop('command')
    write_json(out / 'linker.json', profile)
    print('Built immutable ld64 image; run validate before using it.')


def validate(profile_path, compiler_path, sdk):
    root, sdk = ROOT, Path(sdk).resolve()
    profile = load_json(root / profile_path)
    output = root / profile['validation']
    write_json(output, {'validated': False, 'reason': 'SDK link probes have not completed'})
    compiler = load_json(root / compiler_path)
    compiler_info = compiler_fingerprint(compiler, root, sdk)
    if compiler['family'] != 'llvmgcc42' or not compiler_info['validated']:
        raise ToolError('Linking probes require the validated historical compiler')
    sdk_identity = require_sdk(sdk)
    linker_identity = identity(profile, root, sdk)
    if 'ld64-134.9' not in linker_identity['version']:
        raise ToolError('Linker version differs from pinned hypothesis')
    from tools.validate_sdk import validate as compile_probes
    compile_probes(root, compiler_path, sdk)
    probes = []
    libraries = {'c': ['-lSystem', '-framework', 'OpenGLES'], 'cpp': ['-lSystem', '-lstdc++', '-lgcc_s.1'],
                 'm': ['-lSystem', '-lobjc', '-framework', 'Foundation'],
                 'mm': ['-lSystem', '-lstdc++', '-lgcc_s.1', '-lobjc', '-framework', 'Foundation']}
    required = {'c': ['_strtol', '_glCreateShader'], 'cpp': ['__Znwm'], 'm': ['_objc_msgSend'], 'mm': ['__Znwm', '_objc_msgSend']}
    targets = {'c': {'_strtol': '/usr/lib/libSystem.B.dylib', '_glCreateShader': '/System/Library/Frameworks/OpenGLES.framework/OpenGLES'},
               'cpp': {'__Znwm': '/usr/lib/libstdc++.6.dylib'}, 'm': {'_objc_msgSend': '/usr/lib/libobjc.A.dylib'},
               'mm': {'__Znwm': '/usr/lib/libstdc++.6.dylib', '_objc_msgSend': '/usr/lib/libobjc.A.dylib'}}
    for extension, libs in libraries.items():
        for mode in ('arm', 'thumb'):
            hashes, structures = [], []
            for repeat in ('a', 'b'):
                obj = f'build/sdk-probes/{extension}/{mode}/{repeat}/probe.o'
                folder = root / f'build/linker/probes/{extension}/{mode}/{repeat}'
                folder.mkdir(parents=True, exist_ok=True)
                image = folder / 'probe'
                objects = [obj]
                # Exercise module constructors and defined Objective-C class
                # metadata, in addition to the primary SDK/import probes.
                cpp = 'volatile unsigned linker_initialized; struct LinkProbeInit { LinkProbeInit(){linker_initialized=41;} }; static LinkProbeInit init;\n'
                objc = '#import <Foundation/Foundation.h>\n@interface LinkProbe : NSObject\n- (unsigned)value;\n@end\n@implementation LinkProbe\n- (unsigned)value {return 41;}\n@end\n'
                support = folder / ('support.' + extension)
                if extension in ('m', 'mm'):
                    main = ('extern "C" unsigned sdk_probe(id);\nextern "C" int main(int argc,char **argv){return sdk_probe(nil);}\n' if extension == 'mm'
                            else 'int sdk_probe(id);\nint main(int argc,char **argv){return sdk_probe(nil);}\n')
                else:
                    main = ('extern "C" unsigned sdk_probe(const char*);\nextern "C" int main(int argc,char **argv){return sdk_probe("41");}\n' if extension == 'cpp'
                            else 'int sdk_probe(const char*);\nint main(int argc,char **argv){return sdk_probe("41");}\n')
                support.write_text((objc if extension in ('m', 'mm') else '') + (cpp if extension in ('cpp', 'mm') else '') + main)
                support_obj = support.with_suffix('.o')
                flags = language_flags(support) + compiler['flags'] + ['-O2', '-m' + mode] + header_flags(compiler, sdk, support)
                built = subprocess.run(compiler_command(compiler, root, sdk) + flags + ['-c', str(support.relative_to(root)),
                                        '-o', str(support_obj.relative_to(root))], cwd=root, capture_output=True, text=True, timeout=120)
                (folder / 'support-diagnostics.txt').write_text(built.stdout + built.stderr)
                if built.returncode:
                    raise ToolError('Link support source compilation failed')
                objects.append(str(support_obj.relative_to(root)))
                objects.insert(0, '/sdk/usr/lib/crt1.3.1.o')
                args = ['-arch', 'armv7', '-ios_version_min', '4.2', '-sdk_version', '5.1', '-syslibroot', '/sdk',
                        '-e', 'start', '-no_uuid', '-o', str(image.relative_to(root))] + objects + libs
                process = subprocess.run(command(profile, root, sdk) + args, cwd=root, capture_output=True, text=True, timeout=120)
                (folder / 'diagnostics.txt').write_text(process.stdout + process.stderr)
                if process.returncode:
                    raise ToolError('SDK link probe failed: ' + extension + '/' + mode + '; see diagnostics')
                structures.append(inspect_image(image.read_bytes(), entry='start', imports=required[extension], min_version=0x40200,
                                                sdk_version=0x50100, expected_bindings=targets[extension]))
                section_names = {s['name'] for s in structures[-1]['sections']}
                if structures[-1]['thread_entry'] is None:
                    raise ToolError('Linked SDK startup entry is absent')
                if extension in ('cpp', 'mm') and '__mod_init_func' not in section_names:
                    raise ToolError('Linked C++ module constructor is absent')
                if extension in ('m', 'mm') and '__objc_classlist' not in section_names:
                    raise ToolError('Linked Objective-C class metadata is absent')
                hashes.append(sha256(image))
                symbol = next(s for s in MachO(image.read_bytes()).symbols if s.name == '_sdk_probe' and s.defined)
                if symbol.thumb != (mode == 'thumb'):
                    raise ToolError('Linked entry ARM/Thumb mode differs')
            if hashes[0] != hashes[1] or structures[0] != structures[1]:
                raise ToolError('Linked probes are not reproducible across build directories')
            probes.append({'language': extension, 'mode': mode, 'image_sha256': hashes[0], 'structure': structures[0]})
    # A missing SDK import must be a hard failure, never dynamic_lookup success.
    negative = subprocess.run(command(profile, root, sdk) + ['-arch', 'armv7', '-ios_version_min', '4.2', '-sdk_version', '5.1',
                '-syslibroot', '/sdk', '-e', '_sdk_probe', '-no_uuid', '-o', 'build/linker/missing-import',
                'build/sdk-probes/c/arm/a/probe.o', '-lSystem'], cwd=root, capture_output=True, text=True, timeout=120)
    (root / 'build/linker/missing-import.txt').write_text(negative.stdout + negative.stderr)
    (root / 'build/linker/missing-import').unlink(missing_ok=True)
    if negative.returncode == 0 or '_glCreateShader' not in negative.stderr:
        raise ToolError('Missing import was not rejected')
    write_json(output, {'validated': True, 'identity': linker_identity, 'profile_sha256': digest(profile),
               'sdk': sdk_identity, 'compiler': compiler_info, 'probes': probes, 'missing_import_rejected': True,
               'scope': 'Reproducible structural SDK linking only; original linker equivalence and runtime behavior are unproven'})
    print('Validated C/C++/Objective-C/Objective-C++ ARM/Thumb SDK linking, bindings and missing-import rejection.')


def export():
    out = ROOT / 'build/linker'
    profile = load_json(out / 'linker.json')
    validation = load_json(out / 'validation.json')
    if not validation.get('validated') or validation['identity'] != identity(profile, ROOT):
        raise ToolError('Refusing to export an unvalidated linker')
    archive = out / 'runtime-image.tar'
    subprocess.run(['docker', 'image', 'save', '-o', str(archive), profile['container']['image']], check=True)
    with archive.open('rb') as source, archive.with_suffix('.tar.gz').open('wb') as output:
        with gzip.GzipFile(fileobj=output, mode='wb', mtime=0) as target:
            shutil.copyfileobj(source, target)
    archive.unlink()
    write_json(out / 'runtime-export.json', {'validated': True, 'image': profile['container']['image'],
               'sha256': sha256(out / 'runtime-image.tar.gz'), 'profile_sha256': digest(profile)})


def import_runtime(directory, compiler_path, sdk):
    directory = Path(directory).resolve()
    exported = load_json(directory / 'runtime-export.json')
    profile = load_json(directory / 'linker.json')
    previous = load_json(directory / 'validation.json')
    archive = directory / 'runtime-image.tar.gz'
    if (not exported.get('validated') or not previous.get('validated') or sha256(archive) != exported.get('sha256')
            or digest(profile) != exported.get('profile_sha256') or profile['container']['image'] != exported.get('image')):
        raise ToolError('Linker archive/manifest identity differs or is unvalidated')
    if profile.get('template_sha256') != digest(load_json(ROOT / 'config/linker.json')):
        raise ToolError('Exported linker template differs from this checkout')
    out = ROOT / 'build/linker'
    write_json(out / 'runtime-import.json', {'validated': False, 'reason': 'Receiving-host import has not completed'})
    write_json(out / 'validation.json', {'validated': False, 'reason': 'Receiving-host link probes have not completed'})
    loaded = subprocess.run(['docker', 'load', '-i', str(archive)], capture_output=True, text=True, check=True)
    identities = set(re.findall(r'Loaded image ID: (sha256:[0-9a-f]{64})', loaded.stdout))
    # Docker/containerd can re-identify an exported image during format
    # conversion. Pin the receiving-host ID, verify the installed binary, and
    # revalidate image/layout hashes rather than trusting a tag or new ID alone.
    if not identities:
        tags = re.findall(r'^Loaded image: (\S+)$', loaded.stdout, re.MULTILINE)
        for tag in tags:
            item = json.loads(subprocess.run(['docker', 'image', 'inspect', tag], capture_output=True, text=True, check=True).stdout)[0]
            identities.add(item['Id'])
    if len(identities) != 1:
        raise ToolError('Expected one loaded linker image identity')
    image = identities.pop()
    metadata = json.loads(subprocess.run(['docker', 'image', 'inspect', image], capture_output=True, text=True, check=True).stdout)[0]
    if metadata.get('Id') != image or metadata.get('Os') != 'linux' or metadata.get('Architecture') != 'amd64':
        raise ToolError('Linker image must be Linux amd64')
    profile['container']['image'] = image
    received = identity(profile, ROOT)
    if any(received[k] != previous['identity'][k] for k in ('family', 'version', 'binary_sha256')):
        raise ToolError('Imported linker binary fingerprint differs')
    path = 'build/linker/linker.json'
    write_json(ROOT / path, profile)
    validate(path, compiler_path, sdk)
    current = load_json(ROOT / profile['validation'])
    if current['probes'] != previous['probes']:
        write_json(ROOT / profile['validation'], {'validated': False, 'reason': 'Receiving-host image hashes/layout differ from exported probes'})
        raise ToolError('Receiving-host link probes differ from native/exported probes')
    write_json(out / 'runtime-import.json', {'validated': True, 'archive_sha256': exported['sha256'],
               'exported_image': exported['image'], 'receiving_image': image,
               'binary_sha256': received['binary_sha256'], 'identical_probes': True})
    print('Imported linker and revalidated identical images/layout on this host.')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=['build', 'validate', 'export', 'import'])
    parser.add_argument('directory', nargs='?')
    parser.add_argument('--profile', default='build/linker/linker.json')
    parser.add_argument('--compiler', default='build/toolchain/compiler.json')
    parser.add_argument('--sdk', default='build/sdk/iPhoneOS5.1.sdk')
    args = parser.parse_args()
    try:
        if args.command == 'build': build()
        elif args.command == 'validate': validate(args.profile, args.compiler, args.sdk)
        elif args.command == 'export': export()
        else:
            if not args.directory:
                raise ToolError('Supply the exported linker artifact directory')
            import_runtime(args.directory, args.compiler, args.sdk)
    except (OSError, ValueError, KeyError, subprocess.SubprocessError) as error:
        parser.exit(1, str(error) + '\n')


if __name__ == '__main__': main()
