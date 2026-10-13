#include "NiTimeController.h"

// Original compilation group o-6031b6c2de40a8188a31.

void NiTimeController::OnPreDisplay() {  }

unsigned int NiTimeController::ItemsInList() const {
    const NiTimeController* controller = this;
    unsigned int count = 0;
    while (controller) {
        controller = controller->m_nextController;
        ++count;
    }
    return count;
}
