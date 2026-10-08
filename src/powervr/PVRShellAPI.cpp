#include "PVRShellAPI.h"

// The EAGL implementation in this executable rejects every integer API
// preference. Signature evidence and flag experiments: docs/first-candidates.md.
bool PVRShellInit::ApiSet(prefNameIntEnum, int) {
    return false;
}
