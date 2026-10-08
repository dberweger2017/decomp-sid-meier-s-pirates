#include "WorldMapProjection.h"
extern "C" double modf(double value, double* integral);

__attribute__((noinline)) int RemapPoint(int value, int sourceOrigin, int sourceExtent,
               int destinationExtent, int destinationOrigin) {
    double integral;
    double fraction = modf(float(value - sourceOrigin) / sourceExtent * destinationExtent, &integral);
    if (fraction < 0.0)
        integral -= fraction < -0.5 ? 1.0 : 0.0;
    else
        integral += fraction > 0.5 ? 1.0 : 0.0;
    return int(integral) + destinationOrigin;
}

// The original exported name is WMapXY_Shi (ten characters), not Shift.
bool WMapXY_Shi(float worldX, float worldY, float scale, int offsetX,
                int offsetY, int* screenX, int* screenY) {
    *screenX = RemapPoint(int(worldX), 0, 462,
                         int(scale * 800.0f * 4.0f), int(scale * 100.0f * 4.0f)) - offsetX;
    *screenY = RemapPoint(int(293.0f - worldY), 0, 293,
                         int(scale * 640.0f * 4.0f), int(scale * 5.0f * 4.0f)) - offsetY;
    return true;
}
