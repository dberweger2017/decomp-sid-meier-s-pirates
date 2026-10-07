#!/usr/bin/env python3
"""Exercise the validated historical compiler through the synthetic Ninja loop."""
import argparse
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.compiler import command, fingerprint, language_flags
from tools.pirates.configure import configure
from tools.pirates.inventory import recover
from tools.pirates.macho import MachO
from tools.pirates.report import regression
from tools.pirates.util import ToolError, load_json, write_json, ninja_command
from tests.fixtures import reference

ROOT = Path(__file__).resolve().parents[1]


def smoke(profile_path):
    profile = load_json(ROOT / profile_path)
    info = fingerprint(profile, ROOT)
    if profile['family'] != 'llvmgcc42' or not info['validated']:
        raise ToolError('Historical integration requires a validated LLVM-GCC compiler: ' + str(info['reason']))
    cache = ROOT / 'build/toolchain'
    cache.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='historical loop ', dir=cache) as temp:
        root = Path(temp)
        shutil.copytree(ROOT / 'tools', root / 'tools', ignore=shutil.ignore_patterns('__pycache__'))
        shutil.copyfile(ROOT / 'configure.py', root / 'configure.py')
        (root / 'config').mkdir()
        (root / 'src').mkdir()
        write_json(root / 'config/identity.json', {})
        write_json(root / profile['validation'], load_json(ROOT / profile['validation']))
        # Freestanding synthetic probes need no SDK. Production profiles retain
        # their explicit SDK requirement and original flag uncertainty.
        profile['sdk_required'] = False
        profile['flags'] += ['-O2', '-mthumb', '-ffreestanding']
        write_json(root / 'config/compiler.json', profile)
        (root / 'src/value.h').write_text('#define VALUE 41\n')
        (root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x){return x+VALUE;}\n')
        result = subprocess.run(command(profile, root) + language_flags('src/probe.c') + profile['flags'] +
                                ['-c', 'src/probe.c', '-o', 'golden.o'], cwd=root, capture_output=True, text=True)
        if result.returncode:
            raise ToolError('Historical reference probe failed: ' + result.stdout + result.stderr)
        obj = MachO((root / 'golden.o').read_bytes())
        symbol = next(s for s in obj.symbols if s.name == '_probe')
        section = obj.section(symbol.section)
        size = section.size - ((symbol.value & ~1) - section.address)
        (root / 'fixture.macho').write_bytes(reference(obj.bytes_at(symbol.value & ~1, size, symbol.section), 'thumb'))
        inv = recover(MachO((root / 'fixture.macho').read_bytes()))
        write_json(root / 'config/candidates.json', {'version': 1, 'units': [
            {'group_id': inv['groups'][0]['id'], 'source': 'src/probe.c', 'flags': []}]})
        configure(root, fixture=root / 'fixture.macho')
        subprocess.run(ninja_command(), cwd=root, check=True)
        base = load_json(root / 'build/report.json')
        if base['metrics']['matched_functions'] != 1:
            raise ToolError('Historical synthetic candidate was not verified: ' + str(base['functions']))
        (root / 'src/value.h').write_text('#define VALUE 42\n')
        subprocess.run(ninja_command(), cwd=root, check=True)
        head = load_json(root / 'build/report.json')
        delta = regression(base, head)
        if head['metrics']['matched_functions'] != 0 or not delta['regressions'] or not delta['failures']:
            raise ToolError('Historical header regression was not detected')
        for name, value in (('base', base), ('head', head), ('regression', delta)):
            write_json(cache / ('historical-smoke-' + name + '.json'), value)
    print('Historical Ninja loop verified: synthetic match, included-header rebuild, and regression gate.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--profile', default='build/toolchain/compiler.json')
    args = parser.parse_args()
    try:
        smoke(args.profile)
    except (ToolError, OSError, subprocess.CalledProcessError) as error:
        parser.exit(1, str(error) + '\n')
