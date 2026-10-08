#pragma once

// Observed ABI views only; do not instantiate these partial types. Virtual slots
// before GetType, inheritance, ownership and complete object sizes are unknown.
class NiProperty {
public:
    virtual void Unknown00(); virtual void Unknown01();
    virtual void Unknown02(); virtual void Unknown03();
    virtual void Unknown04(); virtual void Unknown05();
    virtual void Unknown06(); virtual void Unknown07();
    virtual void Unknown08(); virtual void Unknown09();
    virtual void Unknown10(); virtual void Unknown11();
    virtual void Unknown12(); virtual void Unknown13();
    virtual int ObservedType(); // observed vtable +0x38
};

struct NiPropertyListItem {
    NiPropertyListItem* next;
    NiPropertyListItem* previous;
    NiProperty* property;
};
struct BattleGridColor { float r, g, b, a; };
struct BattleGridGeometryData {
    unsigned char unknown_00[0x28];
    BattleGridColor* colors;
    unsigned char unknown_2c[6];
    unsigned short dirtyFlags;
    unsigned char unknown_34[12];
    unsigned short* indices;
};
class NiAVObject {
public:
    NiProperty* GetProperty(int type);
    void SetAppCulled(bool culled) {
        unsigned short value = flags;
        flags = culled ? (value | 1) : (value & ~1);
    }
    unsigned char unknown_00[0x20];
    unsigned short flags;
    unsigned char unknown_22[0x82];
    NiPropertyListItem* properties;
    unsigned char unknown_a8[0x14];
    BattleGridGeometryData* geometryData; // observed NiGeometry-derived view +0xbc
};
