#include "BattleGrid.h"
#include "../../gamebryo/NiMaterialProperty.h"

void BattleGrid::SetSquareEnable(unsigned short layer, unsigned short x,
                                unsigned short y, bool enabled, float alpha) {
    if (((enabledLayers[x][y] >> layer) & 1) == enabled ||
        (locked[x][y] & (1 << layer)))
        return;
    if (enabled)
        enabledLayers[x][y] |= 1 << layer;
    else
        enabledLayers[x][y] &= ~(1 << layer);
    BattleGridGeometryData* data = geometry[layer]->geometryData;
    BattleGridColor* colors = data->colors;
    unsigned short* indices = data->indices;
    NiMaterialProperty* material = static_cast<NiMaterialProperty*>(geometry[layer]->GetProperty(3));
    material->SetAlpha(enabled ? 1.0f : 0.0f);
    unsigned short begin = ranges[layer][x][y].begin;
    if (begin < ranges[layer][x][y].end) {
        if (!enabled) alpha = 0.0f;
        int i = begin;
        do {
            colors[indices[i]].a = alpha;
        } while (++i < ranges[layer][x][y].end);
    }
    geometry[layer]->geometryData->dirtyFlags |= 4;
}


void BattleGrid::SetGridEnable(bool enabled) {
    for (int i = 0; i < 6; ++i)
        if (geometry[i]) geometry[i]->SetAppCulled(!(enabled && !layerDisabled[i]));
}

void BattleGrid::SetLayerEnable(unsigned short layer, bool enabled) {
    if (geometry[layer]) geometry[layer]->SetAppCulled(!enabled);
    layerDisabled[layer] = !enabled;
}
