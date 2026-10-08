#!/usr/bin/env python3
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.build import compile_unit, report
from tools.pirates.util import ToolError

if __name__ == '__main__':
    root = Path.cwd()
    try:
        if len(sys.argv) == 3 and sys.argv[1] == 'compile':
            compile_unit(root, sys.argv[2])
        elif sys.argv[1:] == ['report']:
            sys.exit(report(root))
        elif sys.argv[1:] == ['link']:
            from tools.pirates.linking import run_link
            # Retain a report even when linking fails. report() gates Ninja/CI.
            run_link(root)
        else:
            raise ToolError('Usage: tools/build.py compile <unit-id> | report')
    except (ToolError, OSError, KeyError) as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)
