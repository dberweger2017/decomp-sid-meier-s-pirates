// Small EAGLView property accessors from the original EAGLView.o group.
typedef void * PiratesEAGLObject;

#define EAGL_IVAR(symbol, name, value) \
    int name __asm__(symbol) __attribute__((section("__DATA,__objc_ivar"))) = value
EAGL_IVAR("_OBJC_IVAR_$_EAGLView._hang", pirates_eagl_ivar_hang, 0x78);
EAGL_IVAR("_OBJC_IVAR_$_EAGLView.m_pPVRShellInit", pirates_eagl_ivar_shell_init, 0x6c);
EAGL_IVAR("_OBJC_IVAR_$_EAGLView._context", pirates_eagl_ivar_context, 0x40);
EAGL_IVAR("_OBJC_IVAR_$_EAGLView._depthFormat", pirates_eagl_ivar_depth_format, 0x34);
EAGL_IVAR("_OBJC_IVAR_$_EAGLView._format", pirates_eagl_ivar_format, 0x30);
EAGL_IVAR("_OBJC_IVAR_$_EAGLView._framebuffer", pirates_eagl_ivar_framebuffer, 0x44);
EAGL_IVAR("_OBJC_IVAR_$_EAGLView._autoresize", pirates_eagl_ivar_autoresize, 0x3c);
EAGL_IVAR("_OBJC_IVAR_$_EAGLView._delegate", pirates_eagl_ivar_delegate, 0x60);

extern "C" void eagl_hang(PiratesEAGLObject, void *, unsigned char)
    __asm__("-[EAGLView Hang:]");
extern "C" void eagl_hang(PiratesEAGLObject self, void *, unsigned char value) {
    *reinterpret_cast<unsigned char *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_hang) = value;
}

extern "C" void eagl_set_shell_init(PiratesEAGLObject, void *, PiratesEAGLObject)
    __asm__("-[EAGLView setPVRShellInit:]");
extern "C" void eagl_set_shell_init(PiratesEAGLObject self, void *, PiratesEAGLObject value) {
    *reinterpret_cast<PiratesEAGLObject *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_shell_init) = value;
}

extern "C" PiratesEAGLObject eagl_context(PiratesEAGLObject)
    __asm__("-[EAGLView context]");
extern "C" PiratesEAGLObject eagl_context(PiratesEAGLObject self) {
    return *reinterpret_cast<PiratesEAGLObject *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_context);
}

extern "C" int eagl_depth_format(PiratesEAGLObject)
    __asm__("-[EAGLView depthFormat]");
extern "C" int eagl_depth_format(PiratesEAGLObject self) {
    return *reinterpret_cast<int *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_depth_format);
}

extern "C" int eagl_pixel_format(PiratesEAGLObject)
    __asm__("-[EAGLView pixelFormat]");
extern "C" int eagl_pixel_format(PiratesEAGLObject self) {
    return *reinterpret_cast<int *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_format);
}

extern "C" unsigned int eagl_framebuffer(PiratesEAGLObject)
    __asm__("-[EAGLView framebuffer]");
extern "C" unsigned int eagl_framebuffer(PiratesEAGLObject self) {
    return *reinterpret_cast<unsigned int *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_framebuffer);
}

extern "C" signed char eagl_autoresizes_surface(PiratesEAGLObject)
    __asm__("-[EAGLView autoresizesSurface]");
extern "C" signed char eagl_autoresizes_surface(PiratesEAGLObject self) {
    return *reinterpret_cast<signed char *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_autoresize);
}

extern "C" void eagl_set_autoresizes_surface(PiratesEAGLObject, void *, unsigned char)
    __asm__("-[EAGLView setAutoresizesSurface:]");
extern "C" void eagl_set_autoresizes_surface(PiratesEAGLObject self, void *, unsigned char value) {
    *reinterpret_cast<unsigned char *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_autoresize) = value;
}

extern "C" PiratesEAGLObject eagl_delegate(PiratesEAGLObject)
    __asm__("-[EAGLView delegate]");
extern "C" PiratesEAGLObject eagl_delegate(PiratesEAGLObject self) {
    return *reinterpret_cast<PiratesEAGLObject *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_delegate);
}

extern "C" void eagl_set_delegate(PiratesEAGLObject, void *, PiratesEAGLObject)
    __asm__("-[EAGLView setDelegate:]");
extern "C" void eagl_set_delegate(PiratesEAGLObject self, void *, PiratesEAGLObject value) {
    *reinterpret_cast<PiratesEAGLObject *>(reinterpret_cast<char *>(self) + pirates_eagl_ivar_delegate) = value;
}
