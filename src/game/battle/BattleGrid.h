#pragma once
#include "../../gamebryo/NiAVObject.h"

// Partial observed land-battle grid ABI. Array capacities follow the original
// strides, not recovered declarations; full layout and lifetime are unknown.
class BattleGrid {
public:
    void SetGridEnable(bool enabled);
    void SetLayerEnable(unsigned short layer, bool enabled);
    void SetSquareEnable(unsigned short layer, unsigned short x,
                         unsigned short y, bool enabled, float alpha);
    unsigned char unknown_00[5];
    bool layerDisabled[6];
    unsigned char unknown_0b;
    struct Range { unsigned short begin, end; } ranges[6][16][16]; // +0x0c
    struct Pair { unsigned short x, y; } layerSubTextures[6]; // +0x180c
    Pair squareSubTextures[6][16][16]; // +0x1824
    unsigned short locked[16][16]; // +0x3024
    unsigned short enabledLayers[16][16]; // +0x3224
    unsigned short squareSize[6]; // +0x3424
    unsigned short textureSize[6]; // +0x3430
    unsigned short verticesPerSide[6]; // +0x343c
    struct Offset { short x, y; } squareOffset[6][16][16]; // +0x3448
    NiAVObject* geometry[6]; // +0x4c48
    float positionX, positionY; // +0x4c60, +0x4c64
};
