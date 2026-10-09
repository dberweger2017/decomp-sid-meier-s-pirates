#include "FSoundtrack.h"

// Original group o-fb8215a5a1f2d4ef8e04 (FSoundtrack.o).

void FSoundtrack::SetSongGroup(const FSoundtrackGroupNode *group, int index) { m_songGroup = group; m_songGroupIndex = index; }

void FSoundtrack::SetTaperVolume(float volume) { m_taperVolume = volume; }

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
}
