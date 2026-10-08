#pragma once

#include "FxDragWidget.h"

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class FxScrollbar {
public:
    bool IsThumbDragging();

private:
    unsigned char m_unknown_00[664];
    FxDragWidget * m_thumb; // +0x298
};
