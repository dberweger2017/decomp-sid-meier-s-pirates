#include "ISEControllerSequence.h"
#include "../powervr/PVRTMatrixF.h"
#include "ISESequenceMemory.h"
#include "ISESequenceReader.h"
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

template<int Version>
inline __attribute__((always_inline)) ControllerSequence *ControllerSequence::ReadSequence(const char *memory) {
    using namespace sequence_reader;
    const char *cursor = memory + 16;
    const unsigned int nameLength = Word(cursor);
    ControllerSequence *sequence = new ControllerSequence();
    sequence->m_name = new char[nameLength + 1]();
    strncpy(sequence->m_name, cursor, nameLength);
    cursor += nameLength;
    sequence->m_unknown04 = Word(cursor);
    sequence->m_unknown08 = Word(cursor);
    sequence->m_propertyCount = Word(cursor);
    if (sequence->m_propertyCount > 0) {
        sequence->m_properties = new Node[sequence->m_propertyCount];
        for (int i = 0; i < sequence->m_propertyCount; ++i) {
            sequence->m_properties[i].name = Name(cursor);
            sequence->m_properties[i].parent = Word(cursor);
        }
    }
    sequence->m_nodeCount = Word(cursor);
    if (sequence->m_nodeCount > 0) {
        sequence->m_nodes = new Node[sequence->m_nodeCount];
        sequence->m_keyframes = new KeyframeController[sequence->m_nodeCount];
        for (int i = 0; i < sequence->m_nodeCount; ++i) {
            sequence->m_nodes[i].name = Name(cursor);
            sequence->m_nodes[i].parent = Word(cursor);
        }
        for (int i = 0; i < sequence->m_nodeCount; ++i) {
            KeyframeController &controller = sequence->m_keyframes[i];
            for (int field = 0; field < 4; ++field) controller.m_metadata[field] = Word(cursor);
            Channel(cursor, controller.m_translation.count, controller.m_translation.times, controller.m_translation.values, false);
            Channel(cursor, controller.m_rotation.count, controller.m_rotation.times, controller.m_rotation.values, true);
            Channel(cursor, controller.m_scale.count, controller.m_scale.times, controller.m_scale.values, true);
            if (Version >= 2)
                Channel(cursor, controller.m_visibility.count, controller.m_visibility.times, controller.m_visibility.values, true);
        }
    }
    return sequence;
}

ControllerSequence *ControllerSequence::CreateSequenceFromMemoryVer1(const char *memory, int) {
    return ReadSequence<1>(memory);
}
ControllerSequence *ControllerSequence::CreateSequenceFromMemoryVer2(const char *memory, int size) {
    if (reinterpret_cast<const unsigned int *>(memory)[2] == 1)
        return CreateSequenceFromMemoryVer1(memory, size);
    return ReadSequence<2>(memory);
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
