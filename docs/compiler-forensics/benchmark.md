# Representative source benchmark

43 functions / 14 original groups / 19,352 original bytes. Existing unit profiles only. Status and resolved assembly similarity are separate from source semantics and runtime equivalence.

| Function | Original group | Bytes | Current LLVM-GCC | Nearby Clang 318.0.45 | Modern control |
|---|---|---:|---|---|---|
| `PVRTMatrixIdentityF(PVRTMATRIXf&)` | libOGLES2Tools.a(PVRTMatrixF.o) | 76 | matched / 100.00% | matched / 100.00% | different / 3.92% |
| `PVRTMatrixTranslationF(PVRTMATRIXf&, float, float, float)` | libOGLES2Tools.a(PVRTMatrixF.o) | 76 | different / 0.00% | different / 0.00% | different / 0.00% |
| `PVRTMatrixScalingF(PVRTMATRIXf&, float, float, float)` | libOGLES2Tools.a(PVRTMatrixF.o) | 76 | different / 31.58% | different / 31.58% | different / 6.45% |
| `PVRTMatrixVec3LerpF(PVRTVECTOR3f&, PVRTVECTOR3f const&, PVRTVECTOR3f const&, float)` | libOGLES2Tools.a(PVRTMatrixF.o) | 84 | different / 92.68% | different / 9.76% | different / 15.79% |
| `PVRTMatrixMultiplyF(PVRTMATRIXf&, PVRTMATRIXf const&, PVRTMATRIXf const&)` | libOGLES2Tools.a(PVRTMatrixF.o) | 20 | different / 61.54% | different / 61.54% | matched / 100.00% |
| `PVRTVec3::PVRTVec3(PVRTVec4 const&)` | libOGLES2Tools.a(PVRTVector.o) | 28 | matched / 100.00% | matched / 100.00% | different / 57.14% |
| `PVRTMat3::PVRTMat3(PVRTMat4 const&)` | libOGLES2Tools.a(PVRTVector.o) | 76 | matched / 100.00% | matched / 100.00% | matched / 100.00% |
| `PVRTMat4::operator*(PVRTMat4 const&) const` | libOGLES2Tools.a(PVRTVector.o) | 28 | different / 85.71% | matched / 100.00% | different / 85.71% |
| `PVRTMat3::RotationZ(float)` | libOGLES2Tools.a(PVRTVector.o) | 108 | matched / 100.00% | matched / 100.00% | different / 31.37% |
| `FPhono::SetDopplerFactor(float)` | FPhono.o | 36 | different / 88.89% | different / 88.89% | different / 66.67% |
| `FPhono::CloseStream(int)` | FPhono.o | 36 | different / 88.89% | different / 77.78% | different / 55.56% |
| `FPhono::Shutdown()` | FPhono.o | 20 | matched / 100.00% | matched / 100.00% | different / 60.00% |
| `ISE::ISEAABBBox::merge(float, float, float)` | libISELib.a(ISEAABB.o) | 164 | different / 85.37% | different / 64.20% | different / 27.16% |
| `ISE::ISEAABBBox::transform(PVRTMat4 const&)` | libISELib.a(ISEAABB.o) | 1404 | different / 39.12% | different / 10.18% | unresolved / 16.35% |
| `ISE::ISEAABBBox::getCenterPoint()` | libISELib.a(ISEAABB.o) | 72 | different / 17.14% | different / 11.43% | different / 45.71% |
| `ISE::ISECamera::updateFrustumPlane()` | libISELib.a(ISECamera.o) | 3080 | different / 29.93% | different / 22.28% | different / 13.51% |
| `ISE::ISECamera::IsVisible(ISE::ISEAABBBox&)` | libISELib.a(ISECamera.o) | 520 | different / 55.60% | different / 22.67% | different / 12.64% |
| `ISE::ISEPlane::GetDistance(ISE::ISEVector3&)` | libISELib.a(ISEPlane.o) | 52 | different / 25.00% | different / 33.33% | compile_error |
| `ISE::ISEPlane::GetDistance(float, float, float)` | libISELib.a(ISEPlane.o) | 292 | different / 44.76% | different / 29.17% | compile_error |
| `ISE::ISEPlane::ClassifyPoint(ISE::ISEVector3 const&, ISE::ISEVector3 const&)` | libISELib.a(ISEPlane.o) | 360 | different / 40.00% | different / 32.58% | compile_error |
| `ISE::ISEPlane::CreateFromTriangle(ISE::ISEVector3&, ISE::ISEVector3&, ISE::ISEVector3&)` | libISELib.a(ISEPlane.o) | 528 | different / 69.37% | different / 29.57% | compile_error |
| `ISE::ISEPlane::ISEPlane()` | libISELib.a(ISEPlane.o) | 40 | matched / 100.00% | different / 90.91% | compile_error |
| `ISE::ISEVector3::operator=(ISE::ISEVector3&)` | libISELib.a(ISEVector.o) | 28 | matched / 100.00% | matched / 100.00% | matched / 100.00% |
| `ISE::ISEVector3::operator+(ISE::ISEVector3&)` | libISELib.a(ISEVector.o) | 56 | different / 96.30% | different / 96.30% | different / 22.22% |
| `ISE::ISEVector3::absDotProduct(ISE::ISEVector3 const&)` | libISELib.a(ISEVector.o) | 80 | different / 20.00% | different / 30.00% | different / 38.89% |
| `ISE::ISEVector3::CrossProduct(ISE::ISEVector3&)` | libISELib.a(ISEVector.o) | 80 | different / 16.67% | different / 30.77% | different / 5.56% |
| `ISE::ISEVector3::Normalize()` | libISELib.a(ISEVector.o) | 72 | different / 10.53% | different / 36.84% | different / 22.22% |
| `ISE::ControllerSequence::~ControllerSequence()` | libISELib.a(ISEControllerSequence.o) | 1104 | different / 86.80% | unresolved / 51.28% | unresolved / 39.78% |
| `ISE::ControllerSequence::GetNodeMatrix(int, PVRTMATRIXf&)` | libISELib.a(ISEControllerSequence.o) | 220 | different / 98.18% | different / 55.05% | different / 19.64% |
| `ISE::ControllerSequence::CreateSequenceFromMemoryVer1(char const*, int)` | libISELib.a(ISEControllerSequence.o) | 4484 | unresolved / 5.58% | unresolved / 7.70% | unresolved / 8.33% |
| `ISE::ControllerSequence::CreateSequenceFromMemoryVer2(char const*, int)` | libISELib.a(ISEControllerSequence.o) | 4976 | unresolved / 8.48% | unresolved / 9.44% | unresolved / 9.40% |
| `ISE::ControllerSequence::CreateSequenceFromMemory(char const*, int)` | libISELib.a(ISEControllerSequence.o) | 124 | different / 64.71% | different / 32.84% | different / 19.61% |
| `ISE::ControllerSequence::CreateSequenceFromFile(char const*)` | libISELib.a(ISEControllerSequence.o) | 644 | unresolved / 63.44% | unresolved / 17.03% | unresolved / 20.60% |
| `NiTimeController::ItemsInList() const` | TempIncludeCpp4.o | 32 | different / 42.11% | different / 12.50% | different / 10.53% |
| `NiParticlesData::SetActiveVertexCount(unsigned short)` | TempIncludeCpp4.o | 20 | matched / 100.00% | matched / 100.00% | different / 40.00% |
| `InitRenderStates()` | libISELib.a(ISELog.o) | 136 | matched / 100.00% | matched / 100.00% | different / 94.12% |
| `NiTMapBase<DFALL<int>, char const*, int>::KeyToHashIndex(char const*) const` | PiratesIncludeCpp2.o | 28 | matched / 100.00% | matched / 100.00% | different / 0.00% |
| `NiTMapBase<DFALL<NiPointer<Object3d>>, char const*, NiPointer<Object3d>>::KeyToHashIndex(char const*) const` | PiratesIncludeCpp2.o | 28 | matched / 100.00% | matched / 100.00% | different / 0.00% |
| `NiTMapBase<NiTPointerAllocator<unsigned int>, Object3d*, int>::KeyToHashIndex(Object3d*) const` | PiratesIncludeCpp2.o | 28 | matched / 100.00% | matched / 100.00% | different / 0.00% |
| `-[CGameCenter achievementResetResult:]` | GC_GameCenter.o | 4 | matched / 100.00% | matched / 100.00% | matched / 100.00% |
| `-[CGameCenter showAlertWithTitle:message:]` | GC_GameCenter.o | 4 | matched / 100.00% | matched / 100.00% | matched / 100.00% |
| `ISE::ISETexture::IsAlphaEnabled()` | libISELib.a(ISETexture.o) | 12 | different / 57.14% | different / 33.33% | different / 33.33% |
| `NiObject::GetRTTI() const` | TempIncludeCpp2.o | 16 | different / 50.00% | matched / 100.00% | matched / 100.00% |

All original instructions and full relocated listings remain local. No diagnostic row awards progress credit. Animation source semantics are speculative; see the main report and sample annotations. Genuine Xcode 4.3.2 compilers are unavailable.
