#include "UicPullReefCtrl.h"

// Recovered bodies from PiratesIncludeCpp2.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

void UicPullReefCtrl::SetIsFullSail(bool fullSail) { m_fullSail = fullSail; }

int UicPullReefCtrl::GetCommandKey() { return m_active ? 0x5e : 0; }
