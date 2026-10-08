#pragma once

int RemapPoint(int value, int sourceOrigin, int sourceExtent,
               int destinationExtent, int destinationOrigin);
bool WMapXY_Shi(float worldX, float worldY, float scale, int offsetX,
                  int offsetY, int* screenX, int* screenY);
