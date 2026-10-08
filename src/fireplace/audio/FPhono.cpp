#include "FPhono.h"
#include "phono2/PAudioSystem.h"

// Original group: o-78c5a9284f33537c011b (FPhono.o).
// Only named source calls are reconstructed. Phono2 targets stay unresolved at
// replacement link time until recovered; comparison resolves their original
// symbol addresses and verifies the encoded call targets.

unsigned int FPhono::GetPlaybackTotal_Stream(int stream) {
    return Phono2::PAudioSystem::getSingletonPtr()->GetStreamPlaybackTotal(stream);
}

unsigned int FPhono::GetPlaybackPosition_Stream(int stream) {
    return Phono2::PAudioSystem::getSingletonPtr()->GetStreamPlaybackPosition(stream);
}
