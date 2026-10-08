#!/usr/bin/env python3
"""Export the function browser without original inputs or write endpoints."""
import argparse
from pathlib import Path
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.site import export_site
from tools.pirates.util import ToolError


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--commit', required=True)
    parser.add_argument('--updated-at', required=True)
    parser.add_argument('--run-url')
    args = parser.parse_args()
    try:
        manifest = export_site(args.workspace, args.output, args.commit, args.updated_at, args.run_url)
    except (ToolError, OSError, ValueError) as error:
        parser.exit(1, str(error) + '\n')
    print(f"Exported {len(manifest['files'])} read-only files for {manifest['commit']}")


if __name__ == '__main__':
    main()
