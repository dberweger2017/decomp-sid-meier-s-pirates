// Hand-recovered bodies for original group o-56583387bbf08f43e2e5.
// f-91f30455fa4dadb6f053 — store the two ship pointers in the ship-info view.
extern "C" void pirates_ship_info_setter_91f30455fa4dadb6f053(void * a0, void * a1, void * a2)
    __asm__("__ZN17ShipBattleUIScene11SetShipInfoEP8Object3dS1_");
extern "C" void pirates_ship_info_setter_91f30455fa4dadb6f053(void * a0, void * a1, void * a2) {
    void * shipInfo = *reinterpret_cast<void **>(static_cast<unsigned char *>(a0) + 0x40);
    void ** fields = reinterpret_cast<void **>(static_cast<unsigned char *>(shipInfo) + 0xec);
    fields[0] = a1;
    fields[1] = a2;
}

// f-fd1c80047a8cb18d1c14 — clear the observed flag at complete-object +0x18b4.
extern "C" void pirates_show_my_ship_text_fd1c80047a8cb18d1c14(void * a0)
    __asm__("__ZN6UicCSB13ShowMyShipTxtEv");
extern "C" void pirates_show_my_ship_text_fd1c80047a8cb18d1c14(void * a0) {
    *reinterpret_cast<unsigned int *>(static_cast<unsigned char *>(a0) + 0x18b4) = 0;
}
