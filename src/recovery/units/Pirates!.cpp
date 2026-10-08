// Original compilation group o-5463601200b8476ad25e.

#include "../OGLESIntroducingPVRShellReleasePirates!.cpp"

#include "../leaves/o-5463601200b8476ad25e.cpp"

// Return the addresses of the observed zero-filled path buffers.
unsigned char pirates_xi_temp_read_path[1024]
    __asm__("_g_pzXiTempReadPath") __attribute__((aligned(16)));
extern "C" char * pirates_xi_temp_get_read_path(void)
    __asm__("__Z17xiTempGetReadPathv");
extern "C" char * pirates_xi_temp_get_read_path(void) {
    return reinterpret_cast<char *>(pirates_xi_temp_read_path);
}

unsigned char pirates_temp_save_data_path[1024]
    __asm__("_g_pTempSaveDataPath") __attribute__((aligned(16)));
extern "C" char * pirates_temp_get_save_data_path(void)
    __asm__("__Z19TempGetSaveDataPathv");
extern "C" char * pirates_temp_get_save_data_path(void) {
    return reinterpret_cast<char *>(pirates_temp_save_data_path);
}
