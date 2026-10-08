#include "PVRShell.h"

// Complete constant-return bodies in the original PVRShell.o, rather than
// placeholders for missing implementations. See docs/easy-candidates.md.
bool PVRShell::InitApplication() { return true; }
bool PVRShell::QuitApplication() { return true; }
bool PVRShell::InitView() { return true; }
bool PVRShell::ReleaseView() { return true; }
bool PVRShell::RenderScene() { return true; }

#include "PVRShellInput.cpp"
