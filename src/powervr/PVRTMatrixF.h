#pragma once
#include "PVRTMathTypes.h"

void PVRTMatrixIdentityF(PVRTMATRIXf &matrix);
void PVRTMatrixTranslationF(PVRTMATRIXf &matrix, float x, float y, float z);
void PVRTMatrixScalingF(PVRTMATRIXf &matrix, float x, float y, float z);
void PVRTMatrixVec3LerpF(PVRTVECTOR3f &result, const PVRTVECTOR3f &a,
                       const PVRTVECTOR3f &b, float t);
void PVRTMatrixMultiplyF(PVRTMATRIXf &result, const PVRTMATRIXf &left,
                         const PVRTMATRIXf &right);

// Used by recovered wrappers. Their bodies are not supplied as stubs.
void PVRTMatrixRotationXF(PVRTMATRIXf &matrix, float angle);
void PVRTMatrixRotationYF(PVRTMATRIXf &matrix, float angle);
void PVRTMatrixRotationZF(PVRTMATRIXf &matrix, float angle);
