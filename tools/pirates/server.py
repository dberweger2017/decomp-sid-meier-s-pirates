"""Local function browser, source editor, and serialized incremental build watcher."""
from collections import deque
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import secrets
import shlex
import subprocess
import threading
import time
import urllib.parse
from pathlib import Path
from .build import comparisons, configuration
from .util import load_json, ToolError, local_path, write_json, depfile_paths, ninja_command

EDITABLE = {'.c', '.cc', '.cpp', '.cxx', '.m', '.mm', '.h', '.hh', '.hpp', '.hxx', '.inc'}
IGNORED = {'.git', '.venv', 'build', 'research', 'node_modules', '.sdk'}


def dependencies(root, uid):
    return depfile_paths(Path(root) / f'build/units/{uid}.deps')


def editable_files(root, uid):
    config = configuration(root)
    unit = next((u for u in config['units'] if u['id'] == uid), None)
    if not unit:
        return []
    files = {unit['source']} | dependencies(root, uid)
    result = []
    for value in files:
        try:
            path = local_path(root, value)
            rel = path.relative_to(Path(root).resolve())
            if path.suffix.lower() in EDITABLE and path.exists() and not any(p in IGNORED or p.endswith('.sdk') for p in rel.parts):
                result.append(str(rel))
        except (ToolError, ValueError):
            pass
    return sorted(set(result))


class Watcher:
    def __init__(self, root, interval=.3):
        self.root, self.interval = Path(root).resolve(), interval
        self.stop_event = threading.Event()
        self.condition = threading.Condition()
        self.events = deque(maxlen=100)
        self.revision = 0
        self.building = False
        self.output = ''
        self.returncode = 0
        self.thread = None
        self.process = None

    def emit(self, kind, **kwargs):
        with self.condition:
            self.revision += 1
            self.events.append({'revision': self.revision, 'kind': kind, **kwargs})
            self.condition.notify_all()

    def paths(self):
        try:
            config = load_json(self.root / 'build/config.json')
        except (ValueError, OSError):
            return set()
        paths = {self.root / config['candidates_path'], self.root / config['profile_path'],
                 self.root / config['provenance']['input'], self.root / 'configure.py'}
        for unit in config['units']:
            paths.add(self.root / unit['source'])
            for dep in dependencies(self.root, unit['id']):
                paths.add(self.root / dep)
        # Discover new local headers and sources as well as dependencies from
        # the previous successful build (including out-of-tree included files).
        import os
        for directory, dirs, files in os.walk(self.root):
            dirs[:] = [d for d in dirs if d not in IGNORED and not d.endswith('.sdk')]
            for name in files:
                path = Path(directory) / name
                if path.suffix.lower() in EDITABLE or (directory == str(self.root / 'tools/pirates') and path.suffix == '.py'):
                    paths.add(path)
        return paths

    def signature(self):
        signature = {}
        for path in self.paths():
            try:
                stat = path.stat()
                signature[str(path)] = (stat.st_mtime_ns, stat.st_size)
            except OSError:
                signature[str(path)] = None
        return signature

    def build(self):
        self.building = True
        self.emit('building')
        try:
            self.process = subprocess.Popen(ninja_command(), cwd=self.root, stdout=subprocess.PIPE,
                                            stderr=subprocess.STDOUT, text=True, errors='replace')
            output, _ = self.process.communicate(timeout=600)
            self.output, self.returncode = output, self.process.returncode
        except (OSError, ToolError, subprocess.TimeoutExpired) as e:
            if self.process:
                self.process.kill()
                self.process.communicate()
            self.output, self.returncode = str(e), 1
        finally:
            self.process = None
            self.building = False
            self.emit('built', returncode=self.returncode, output=self.output)

    def run(self):
        previous = self.signature()
        self.build()
        # Capture dependency additions without treating them as source edits.
        previous.update({k: v for k, v in self.signature().items() if k not in previous})
        report_stat = None
        while not self.stop_event.wait(self.interval):
            current = self.signature()
            if current != previous:
                previous = current
                # Debounce save/rename bursts. Preserve edits during a build:
                # the next scan compares against its pre-build signature.
                if self.stop_event.wait(.15):
                    break
                previous = self.signature()
                self.build()
                previous.update({k: v for k, v in self.signature().items() if k not in previous})
            try:
                stat = (self.root / 'build/report.json').stat()
                stamp = stat.st_mtime_ns
                if report_stat is not None and report_stat != stamp:
                    self.emit('report')
                report_stat = stamp
            except OSError:
                pass

    def start(self):
        self.thread = threading.Thread(target=self.run, daemon=True)
        self.thread.start()

    def stop(self):
        self.stop_event.set()
        if self.process:
            self.process.terminate()
        with self.condition:
            self.condition.notify_all()
        if self.thread:
            self.thread.join(timeout=5)


class BrowserServer(ThreadingHTTPServer):
    daemon_threads = True

    def __init__(self, root, port=8765, interval=.3):
        self.root = Path(root).resolve()
        self.token = secrets.token_urlsafe(32)
        self.watcher = Watcher(self.root, interval)
        super().__init__(('127.0.0.1', port), Handler)

    def server_close(self):
        self.watcher.stop()
        super().server_close()


class Handler(BaseHTTPRequestHandler):
    def log_message(self, *args):
        pass

    def reply(self, value, status=200, content_type='application/json'):
        data = value.encode() if isinstance(value, str) else json.dumps(value, ensure_ascii=True).encode()
        self.send_response(status)
        self.send_header('Content-Type', content_type + '; charset=utf-8')
        self.send_header('Content-Length', str(len(data)))
        self.send_header('Cache-Control', 'no-store')
        self.send_header('X-Content-Type-Options', 'nosniff')
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        url = urllib.parse.urlparse(self.path)
        query = urllib.parse.parse_qs(url.query)
        try:
            if url.path == '/':
                page = (Path(__file__).resolve().parents[1] / 'web/index.html').read_text()
                self.reply(page.replace('__TOKEN__', self.server.token), content_type='text/html')
            elif url.path in ('/app.js', '/style.css'):
                path = Path(__file__).resolve().parents[1] / 'web' / url.path[1:]
                self.reply(path.read_text(), content_type='text/javascript' if path.suffix == '.js' else 'text/css')
            elif url.path == '/api/report':
                self.reply(load_json(self.server.root / 'build/report.json'))
            elif url.path == '/api/status':
                w = self.server.watcher
                self.reply({'revision': w.revision, 'building': w.building, 'returncode': w.returncode, 'output': w.output})
            elif url.path == '/api/function':
                self.reply(comparisons(self.server.root, details_id=query['id'][0]))
            elif url.path == '/api/files':
                self.reply(editable_files(self.server.root, query['unit'][0]))
            elif url.path == '/api/source':
                uid, path = query['unit'][0], query['path'][0]
                if path not in editable_files(self.server.root, uid):
                    raise ToolError('Only configured sources and local included headers are editable')
                self.reply({'path': path, 'content': local_path(self.server.root, path).read_text()})
            elif url.path == '/api/events':
                self.events()
            else:
                self.reply({'error': 'Not found'}, 404)
        except (ToolError, OSError, KeyError, ValueError) as e:
            self.reply({'error': str(e)}, 400)

    def events(self):
        self.send_response(200)
        self.send_header('Content-Type', 'text/event-stream')
        self.send_header('Cache-Control', 'no-store')
        self.end_headers()
        last = int(self.headers.get('Last-Event-ID', '0'))
        watcher = self.server.watcher
        try:
            while not watcher.stop_event.is_set():
                with watcher.condition:
                    pending = [e for e in watcher.events if e['revision'] > last]
                    if not pending:
                        watcher.condition.wait(timeout=1)
                        pending = [e for e in watcher.events if e['revision'] > last]
                for event in pending:
                    data = f'id: {event["revision"]}\ndata: {json.dumps(event)}\n\n'
                    self.wfile.write(data.encode())
                    last = event['revision']
                if not pending:
                    self.wfile.write(b': heartbeat\n\n')
                self.wfile.flush()
        except (BrokenPipeError, ConnectionResetError):
            pass

    def do_POST(self):
        try:
            if self.path != '/api/source':
                self.reply({'error': 'Not found'}, 404)
                return
            if self.headers.get('X-Pirates-Token') != self.server.token:
                self.reply({'error': 'Invalid edit token'}, 403)
                return
            origin = self.headers.get('Origin')
            if origin and urllib.parse.urlparse(origin).netloc != self.headers.get('Host'):
                self.reply({'error': 'Cross-origin edits are forbidden'}, 403)
                return
            length = int(self.headers.get('Content-Length', '0'))
            if not 0 < length <= 1024 * 1024:
                raise ToolError('Source edit exceeds 1 MiB or has no body')
            request = json.loads(self.rfile.read(length))
            path, uid, content = request['path'], request['unit'], request['content']
            if not isinstance(content, str) or path not in editable_files(self.server.root, uid):
                raise ToolError('Only configured sources and local included headers are editable')
            dest = local_path(self.server.root, path)
            # Optimistic concurrency preserves external editor changes.
            if dest.read_text() != request['previous']:
                self.reply({'error': 'Source changed in another editor. Reload before saving.'}, 409)
                return
            tmp = dest.with_name(dest.name + '.pirates-tmp')
            tmp.write_text(content)
            tmp.replace(dest)
            self.reply({'saved': path})
        except (ToolError, OSError, KeyError, ValueError) as e:
            self.reply({'error': str(e)}, 400)
