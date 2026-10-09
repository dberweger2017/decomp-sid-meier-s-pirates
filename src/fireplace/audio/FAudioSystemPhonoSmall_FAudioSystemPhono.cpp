#include "FAudioSystemPhono.h"

// Original compilation group o-d165ee5f38eff74c59ca.
bool FAudioSystemPhono::InitListener() { return 1; }
bool FAudioSystemPhono::PrimeBuffer(FAudioSystem::ESoundType, int) { return 0; }
bool FAudioSystemPhono::Get3DSoundPosition(FAudioSystem::ESoundType, int, NiPoint3&) { return 1; }
bool FAudioSystemPhono::Get3DSoundVelocity(FAudioSystem::ESoundType, int, NiPoint3&) { return 1; }
bool FAudioSystemPhono::Get3DSoundDistanceValues(FAudioSystem::ESoundType, int, float&, float&) { return 1; }
bool FAudioSystemPhono::Get3DSoundConeValues(FAudioSystem::ESoundType, int, int&, int&, float&) { return 1; }
bool FAudioSystemPhono::Set3DSoundConeValues(FAudioSystem::ESoundType, int, int, int, float) { return 1; }
bool FAudioSystemPhono::Get3DSoundOrientation(FAudioSystem::ESoundType, int, NiPoint3&, NiPoint3&) { return 1; }
unsigned int FAudioSystemPhono::CalculateNumSamples() { return 1; }
