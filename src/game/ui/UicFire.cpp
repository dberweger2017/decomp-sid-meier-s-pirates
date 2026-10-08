#include "UicFire.h"

// Recovered bodies from PiratesIncludeCpp4.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

int UicFire::GetFireState() { return m_state; }

void UicFire::SetCannonLoadInfoL(int loadInfo, int cannonType) {
    m_cannonLoadInfoL = loadInfo;
    if (cannonType >= 1) m_cannonTypeL = cannonType;
}

void UicFire::SetCannonLoadInfoR(int loadInfo, int cannonType) {
    m_cannonLoadInfoR = loadInfo;
    if (cannonType >= 1) m_cannonTypeR = cannonType;
}
