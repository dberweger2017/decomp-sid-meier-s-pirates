import copy
import json
import subprocess
import sys
import tempfile
from pathlib import Path
import unittest
from tools.cohort import select, summarize, markdown
from tools.pirates.util import ToolError


class CohortTests(unittest.TestCase):
    def setUp(self):
        self.inventory = {'functions': [
            {'id': 'large', 'symbol': 'gameplay', 'size': 400, 'group_id': 'unity'},
            {'id': 'callee', 'symbol': 'helper', 'size': 100, 'group_id': 'engine'},
            {'id': 'missing', 'symbol': 'gameplayMissing', 'size': 500, 'group_id': 'unity'}],
            'groups': [{'id': 'unity', 'name': 'GameUnity.o'}, {'id': 'engine', 'name': 'Engine.o'}]}
        self.report = {'functions': [dict(f, status='different', byte_verified=False, candidate_source='game.cpp',
                                        candidate_size=f['size'], similarity=90.0, reasons=[]) for f in self.inventory['functions']]}
        self.report['functions'][1].update(status='matched', byte_verified=True, similarity=100.0)
        self.report['functions'][2].update(status='missing', candidate_source=None, similarity=None)
        self.cohort = {'version': 1, 'name': 'Game', 'function_ids': ['large', 'callee', 'missing']}

    def test_high_fuzzy_large_function_earns_no_exact_credit(self):
        result = summarize(self.cohort, self.inventory, self.report)
        self.assertEqual(result['metrics']['exact_bytes'], 100)
        self.assertEqual(result['metrics']['compared_byte_weighted_similarity'], 92.0)
        self.assertEqual(result['metrics']['cohort_byte_weighted_similarity'], 46.0)
        self.assertFalse(result['source_pass_complete'])
        self.assertEqual(result['replacement_link_credit'], 0)
        self.assertEqual(result['groups'][0]['name'], 'GameUnity.o')

    def test_unresolved_target_cannot_improve_resolved_similarity(self):
        self.report['functions'][0].update(status='unresolved', similarity=100.0)
        result = summarize(self.cohort, self.inventory, self.report)
        self.assertEqual(result['metrics']['compared_byte_weighted_similarity'], 100.0)
        self.assertEqual(result['metrics']['cohort_byte_weighted_similarity'], 10.0)
        self.assertEqual(result['metrics']['exact_functions'], 1)
        self.assertFalse(result['source_pass_complete'])

    def test_inventory_or_report_coverage_loss_fails(self):
        for key in ['inventory', 'report']:
            inventory, report = copy.deepcopy(self.inventory), copy.deepcopy(self.report)
            (inventory if key == 'inventory' else report)['functions'].pop()
            with self.assertRaisesRegex(ToolError, 'coverage disappeared'):
                summarize(self.cohort, inventory, report)

    def test_group_or_size_change_fails(self):
        for field, value in [('group_id', 'engine'), ('size', 399)]:
            report = copy.deepcopy(self.report)
            report['functions'][0][field] = value
            with self.assertRaisesRegex(ToolError, 'inventory identity'):
                summarize(self.cohort, self.inventory, report)

    def test_false_verified_status_is_rejected(self):
        self.report['functions'][0]['byte_verified'] = True
        with self.assertRaisesRegex(ToolError, 'Inconsistent verified-match'):
            summarize(self.cohort, self.inventory, self.report)

    def test_selection_and_report_are_deterministic(self):
        self.assertEqual(select(self.inventory, 'Game', 'gameplay')['function_ids'], ['large', 'missing'])
        left = summarize(self.cohort, self.inventory, self.report)
        self.inventory['functions'].reverse()
        self.report['functions'].reverse()
        self.cohort['function_ids'].reverse()
        self.assertEqual(left, summarize(self.cohort, self.inventory, self.report))
        self.assertIn('not a time estimate', markdown(left))

    def test_required_exact_cohort_exports_reports_on_failure_and_passes_complete_source(self):
        script = Path(__file__).resolve().parents[1] / 'tools/cohort.py'
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / 'build').mkdir()
            (root / 'build/inventory.json').write_text(json.dumps(self.inventory))
            (root / 'cohort.json').write_text(json.dumps(self.cohort))
            for complete in (False, True):
                if complete:
                    for f in self.report['functions']:
                        f.update(status='matched', byte_verified=True, candidate_source='game.cpp', similarity=100.0)
                (root / 'build/report.json').write_text(json.dumps(self.report))
                output = root / 'result.json'
                run = subprocess.run([sys.executable, str(script), '--workspace', str(root),
                                      'report', str(root / 'cohort.json'), '--output', str(output),
                                      '--require-exact'], capture_output=True, text=True)
                self.assertEqual(run.returncode, 0 if complete else 1)
                self.assertTrue(output.is_file())
                self.assertTrue(output.with_suffix('.md').is_file())
                if not complete:
                    self.assertIn('1/3 verified source entries', run.stderr)

    def test_invalid_or_empty_selection(self):
        with self.assertRaises(ToolError):
            select(self.inventory, 'None', 'unknown')
        for ids in [[], ['large', 'large']]:
            with self.assertRaises(ToolError):
                summarize(dict(self.cohort, function_ids=ids), self.inventory, self.report)


if __name__ == '__main__':
    unittest.main()
