#include "ISEShader.h"

// Original compilation group o-1c2022cc2ba67a063a49.
namespace ISE {

void ISEShader::SetViewProjMatrix(PVRTMat4 const&) {  }

void ISEShader::SetWorldMatrixInv(PVRTMat4 const&) {  }

void ISEShader::SetModelViewIT(PVRTMat3 const&) {  }

void ISEShader::SetAmbient(PVRTVec4 const&) {  }

void ISEShader::SetDiffuse(unsigned int, PVRTVec3 const&) {  }

void ISEShader::SetLightDir(unsigned int, PVRTVec3 const&) {  }

void ISEShader::SetLightPosition(unsigned int, PVRTVec3 const&) {  }
}
