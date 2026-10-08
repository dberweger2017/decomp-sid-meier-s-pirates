#include "NiAVObject.h"

NiProperty* NiAVObject::GetProperty(int type) {
    NiPropertyListItem* position = properties;
    while (position) {
        NiProperty* property = position->property;
        position = position->next;
        if (property && property->ObservedType() == type)
            return property;
    }
    return 0;
}
