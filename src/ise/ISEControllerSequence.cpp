#include "ISEControllerSequence.h"
#include "../powervr/PVRTMatrixF.h"
#include "ISESequenceMemory.h"
#include <string.h>

namespace ISE {
// Only the size and virtual array cleanup are established at this checkpoint.
class SequenceFloatController {
public:
    virtual ~SequenceFloatController() throw();
private:
    unsigned char m_unrecovered[52];
};

ControllerSequence::~ControllerSequence() {
    delete [] m_name;
    if (m_properties) {
        for (int i = 0; i < m_propertyCount; ++i) delete [] m_properties[i].name;
        delete [] m_properties;
    }
    if (m_nodes) {
        for (int i = 0; i < m_nodeCount; ++i) delete [] m_nodes[i].name;
        delete [] m_nodes;
    }
    delete [] m_keyframes;
    if (m_floatNames) {
        for (int i = 0; i < m_floatCount; ++i) delete [] m_floatNames[i];
        delete [] m_floatNames;
    }
    delete [] m_floats;
    if (m_secondaryFloatNames) {
        for (int i = 0; i < m_secondaryFloatCount; ++i) delete [] m_secondaryFloatNames[i];
        delete [] m_secondaryFloatNames;
    }
    delete [] m_secondaryFloats;
}

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
