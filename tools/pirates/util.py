import hashlib
import json
import os
from pathlib import Path


class ToolError(ValueError):
    pass


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for block in iter(lambda: f.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def stable_id(*parts):
    return hashlib.sha256('\0'.join(map(str, parts)).encode()).hexdigest()[:20]


def load_json(path):
    return json.loads(Path(path).read_text())


def write_json(path, value):
    """Atomic, deterministic, and restat-friendly (no timestamps or host paths)."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    data = json.dumps(value, indent=2, sort_keys=True, ensure_ascii=True) + '\n'
    if path.exists() and path.read_text() == data:
        return
    tmp = path.with_name(path.name + '.tmp')
    tmp.write_text(data)
    os.replace(tmp, path)


def local_path(root, value):
    path = (Path(root) / value).resolve()
    if not path.is_relative_to(Path(root).resolve()):
        raise ToolError('Paths must remain inside the workspace: ' + value)
    return path


def ninja_command():
    """Use the hash-pinned Python package, even without shell activation."""
    import importlib.metadata
    try:
        if importlib.metadata.version('ninja') != '1.11.1.3':
            raise ToolError('Install the pinned Ninja package from requirements.txt')
        import ninja
        binary = Path(ninja.BIN_DIR) / 'ninja'
        if not binary.is_file():
            raise ToolError('Pinned Ninja binary is missing')
        return [str(binary)]
    except (ImportError, importlib.metadata.PackageNotFoundError) as e:
        raise ToolError('Missing Ninja. Install requirements.txt in a virtual environment.') from e


def depfile_paths(path):
    import shlex
    path = Path(path)
    if not path.exists():
        return set()
    text = path.read_text().replace('\\\n', ' ')
    first = text.split('\n', 1)[0]
    if ':' not in first:
        return set()
    lexer = shlex.shlex(first.split(':', 1)[1], posix=True)
    lexer.whitespace_split, lexer.commenters = True, ''
    return set(lexer)
