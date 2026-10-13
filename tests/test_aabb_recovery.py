import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


@unittest.skipUnless(shutil.which('clang++'), 'Clang is required for host geometry checks')
class AABBRecoveryTests(unittest.TestCase):
    def test_bounds_merge_and_saved_extents(self):
        # A host check of the reconstructed behavior, separate from historical
        # ARM byte verification. Saved bounds are intentionally untouched until
        # saveExtents, as in the original constructors.
        source = r'''
#include "src/ise/ISEAABB.h"
#include <cassert>
#include <cstring>
#include <limits>
#include <new>

static void equal(const PVRTVec3& value, float x, float y, float z) {
    assert(value.x == x && value.y == y && value.z == z);
}

int main() {
    static_assert(sizeof(ISE::ISEAABBBox) == 52, "observed float/bool layout");
    alignas(ISE::ISEAABBBox) unsigned char storage[sizeof(ISE::ISEAABBBox)];
    std::memset(storage, 0xa5, sizeof(storage));
    ISE::ISEAABBBox& box = *new (storage) ISE::ISEAABBBox;
    for (unsigned i = 24; i < 48; ++i) assert(storage[i] == 0xa5);
    PVRTVec3 upper, lower;
    box.getBox(upper, lower);
    equal(upper, 0, 0, 0);
    equal(lower, 0, 0, 0);
    box.merge(2, -3, 4);
    box.getBox(upper, lower);
    equal(upper, 2, -3, 4);
    equal(lower, 2, -3, 4);
    box.merge(-2, 5, 1);
    box.getBox(upper, lower);
    equal(upper, 2, 5, 4);
    equal(lower, -2, -3, 1);
    equal(box.getCenterPoint(), 0, 1, 2.5f);
    equal(box.getHalfSize(), 2, 4, 1.5f);
    box.saveExtents();
    float saved[6];
    std::memcpy(saved, storage + 24, sizeof(saved));
    assert(saved[0] == 2 && saved[1] == 5 && saved[2] == 4);
    assert(saved[3] == -2 && saved[4] == -3 && saved[5] == 1);
    float nan = std::numeric_limits<float>::quiet_NaN();
    box.merge(nan, nan, nan);
    box.getBox(upper, lower);
    equal(upper, 2, 5, 4);
    equal(lower, -2, -3, 1);
    ISE::ISEAABBBox supplied(PVRTVec3(9, 8, 7), PVRTVec3(3, 2, 1));
    supplied.getBox(upper, lower);
    equal(upper, 9, 8, 7);
    equal(lower, 3, 2, 1);
    // The observed constructor retains the empty flag even with supplied
    // bounds, so the first merge resets both ends.
    supplied.merge(1, 2, 3);
    supplied.getBox(upper, lower);
    equal(upper, 1, 2, 3);
    equal(lower, 1, 2, 3);

    // Translation and reflection must use the saved bounds on every call.
    // A non-unit output w must not cause perspective division.
    PVRTMat4 matrix;
    const float values[16] = {-2,0,0,0, 0,3,0,0, 0,0,1,0, 3,-4,2,2};
    std::memcpy(matrix.f, values, sizeof(values));
    box.transform(matrix);
    box.getBox(upper, lower);
    equal(upper, 7, 11, 6);
    equal(lower, -1, -13, 3);
    box.transform(matrix);
    box.getBox(upper, lower);
    equal(upper, 7, 11, 6);
    equal(lower, -1, -13, 3);
    assert(std::memcmp(saved, storage + 24, sizeof(saved)) == 0);
    ISE::ISEAABBBox empty;
    unsigned char before[sizeof(empty)];
    std::memcpy(before, &empty, sizeof(empty));
    empty.transform(matrix);
    assert(std::memcmp(before, &empty, sizeof(empty)) == 0);
}
'''
        with tempfile.TemporaryDirectory(prefix='pirates aabb behavior ') as directory:
            probe = Path(directory) / 'probe.cpp'
            binary = Path(directory) / 'probe'
            probe.write_text(source)
            compile_run = subprocess.run(
                ['clang++', '-std=c++11', '-O2', '-I', str(ROOT), str(probe),
                 str(ROOT / 'src/ise/ISEAABB.cpp'),
                 str(ROOT / 'src/powervr/NEONMatrix.cpp'), '-o', str(binary)],
                capture_output=True, text=True)
            self.assertEqual(compile_run.returncode, 0, compile_run.stdout + compile_run.stderr)
            run = subprocess.run([str(binary)], capture_output=True, text=True)
            self.assertEqual(run.returncode, 0, run.stdout + run.stderr)
