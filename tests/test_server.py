import json
import shutil
import threading
import time
import unittest
import urllib.request
import urllib.error
import tempfile
from pathlib import Path
from tests.test_workflow import fixture_workspace
from tools.pirates.server import BrowserServer, editable_files
from tools.pirates.util import load_json


@unittest.skipUnless(shutil.which('clang') and shutil.which('ninja'), 'Clang and Ninja required')
class ServerTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.fid = fixture_workspace(self.root)
        self.server = BrowserServer(self.root, 0, .05)
        self.server.watcher.start()
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
        self.thread.start()
        self.url = f'http://127.0.0.1:{self.server.server_port}'
        self.wait_for(lambda: (self.root / 'build/report.json').exists())
        self.uid = load_json(self.root / 'build/report.json')['functions'][0]['group_id']

    def tearDown(self):
        self.server.shutdown()
        self.server.server_close()
        self.temp.cleanup()

    def wait_for(self, predicate):
        deadline = time.monotonic() + 10
        while time.monotonic() < deadline:
            if predicate():
                return
            time.sleep(.05)
        self.fail('Timed out waiting for watcher')

    def get(self, path):
        with urllib.request.urlopen(self.url + path) as r:
            return json.load(r)

    def post(self, body, token=None):
        request = urllib.request.Request(self.url + '/api/source', json.dumps(body).encode(),
                                         {'Content-Type': 'application/json', 'X-Pirates-Token': token or self.server.token}, method='POST')
        with urllib.request.urlopen(request) as response:
            return json.load(response)

    def test_source_and_header_edit_refresh_same_function(self):
        body = {'unit': self.uid, 'path': 'src/probe.c', 'previous': (self.root / 'src/probe.c').read_text(),
                'content': '#include "value.h"\nint probe(int x) { return x + VALUE + 1; }\n'}
        self.post(body)
        self.wait_for(lambda: self.get('/api/function?id=' + self.fid)['status'] == 'different')
        files = self.get('/api/files?unit=' + self.uid)
        self.assertIn('src/value.h', files)
        self.post({'unit': self.uid, 'path': 'src/value.h', 'previous': '#define VALUE 41\n', 'content': '#define VALUE 40\n'})
        self.wait_for(lambda: self.get('/api/report')['metrics']['matched_functions'] == 1 and not self.server.watcher.building)
        self.assertEqual(self.get('/api/function?id=' + self.fid)['id'], self.fid)
        self.assertEqual(self.get('/api/report')['metrics']['matched_functions'], 1)
        self.assertGreaterEqual(self.get('/api/status')['revision'], 6)

    def test_edit_conflicts_and_workspace_boundaries(self):
        body = {'unit': self.uid, 'path': 'src/probe.c', 'previous': 'stale source', 'content': 'bad'}
        with self.assertRaises(urllib.error.HTTPError) as e:
            self.post(body)
        self.assertEqual(e.exception.code, 409)
        body['path'] = '../outside.cpp'
        with self.assertRaises(urllib.error.HTTPError) as e:
            self.post(body)
        self.assertEqual(e.exception.code, 400)
        body['path'] = 'src/probe.c'
        with self.assertRaises(urllib.error.HTTPError) as e:
            self.post(body, 'wrong token')
        self.assertEqual(e.exception.code, 403)

    def test_external_header_edit_triggers_build(self):
        (self.root / 'src/value.h').write_text('#define VALUE 19\n')
        self.wait_for(lambda: self.get('/api/report')['functions'][0]['status'] == 'different')
        detail = self.get('/api/function?id=' + self.fid)
        self.assertEqual(detail['id'], self.fid)
        self.assertTrue(any(row['differences'] for row in detail['rows']))
