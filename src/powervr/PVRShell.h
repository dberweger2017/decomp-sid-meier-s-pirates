#pragma once

// Callback interface only. The complete PVRShell layout, constructor,
// destructor and remaining virtual interface are not recovered here.
// These default methods neither read fields nor construct a shell instance.
class PVRShell {
public:
    virtual ~PVRShell();
    virtual bool InitApplication();
    virtual bool QuitApplication();
    virtual bool InitView();
    virtual bool ReleaseView();
    virtual bool RenderScene();
};
