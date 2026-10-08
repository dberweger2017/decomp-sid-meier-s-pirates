#include "ISEControllerSequence.h"
#include "../powervr/PVRTMatrixF.h"

namespace ISE {
void ControllerSequence::GetNodeMatrix(int node, PVRTMATRIXf &matrix) {
    if (node < 0 || node >= m_nodeCount) return;
    KeyframeController *controller = &m_keyframes[node];
    PVRTMATRIXf temporary;
    if (controller) {
        controller->GetScaleMatrix(m_time, matrix);
        controller->GetRotationMatrix(m_time, temporary);
        PVRTMatrixMultiplyF(matrix, matrix, temporary);
        controller->GetTranslationMatrix(m_time, temporary);
        PVRTMatrixMultiplyF(matrix, matrix, temporary);
    }
    int parent = m_nodes[node].parent;
    if (parent != -1) {
        GetNodeMatrix(parent, temporary);
        PVRTMatrixMultiplyF(matrix, matrix, temporary);
    }
}

ControllerSequence *ControllerSequence::CreateSequenceFromMemory(const char *memory, int size) {
    const unsigned int version = reinterpret_cast<const unsigned int *>(memory)[2];
    switch (version) {
    case 1: return CreateSequenceFromMemoryVer1(memory, size);
    case 2: return CreateSequenceFromMemoryVer2(memory, size);
    case 3: return CreateSequenceFromMemoryVer3(memory, size);
    case 4: return CreateSequenceFromMemoryVer4(memory, size);
    case 5: return CreateSequenceFromMemoryVer5(memory, size);
    case 6: return CreateSequenceFromMemoryVer6(memory, size);
    default: return 0;
    }
}
} // namespace ISE
