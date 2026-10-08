// RTTI getters return the class-local m_RTTI allocation observed in the
// original unity object. These candidates preserve that address relationship.

static unsigned char pirates_ni_object_net_rtti[8]
    __asm__("__ZN11NiObjectNET6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_object_net_get_rtti(void const *)
    __asm__("__ZNK11NiObjectNET7GetRTTIEv");
extern "C" const void * pirates_ni_object_net_get_rtti(void const *) {
    return pirates_ni_object_net_rtti;
}

static unsigned char pirates_ni_av_object_rtti[8]
    __asm__("__ZN10NiAVObject6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_av_object_get_rtti(void const *)
    __asm__("__ZNK10NiAVObject7GetRTTIEv");
extern "C" const void * pirates_ni_av_object_get_rtti(void const *) {
    return pirates_ni_av_object_rtti;
}

static unsigned char pirates_ni_dynamic_effect_rtti[8]
    __asm__("__ZN15NiDynamicEffect6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_dynamic_effect_get_rtti(void const *)
    __asm__("__ZNK15NiDynamicEffect7GetRTTIEv");
extern "C" const void * pirates_ni_dynamic_effect_get_rtti(void const *) {
    return pirates_ni_dynamic_effect_rtti;
}

static unsigned char pirates_ni_geometry_data_rtti[8]
    __asm__("__ZN14NiGeometryData6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_geometry_data_get_rtti(void const *)
    __asm__("__ZNK14NiGeometryData7GetRTTIEv");
extern "C" const void * pirates_ni_geometry_data_get_rtti(void const *) {
    return pirates_ni_geometry_data_rtti;
}

static unsigned char pirates_ni_accumulator_rtti[8]
    __asm__("__ZN13NiAccumulator6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_accumulator_get_rtti(void const *)
    __asm__("__ZNK13NiAccumulator7GetRTTIEv");
extern "C" const void * pirates_ni_accumulator_get_rtti(void const *) {
    return pirates_ni_accumulator_rtti;
}

static unsigned char pirates_ni_geometry_rtti[8]
    __asm__("__ZN10NiGeometry6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_geometry_get_rtti(void const *)
    __asm__("__ZNK10NiGeometry7GetRTTIEv");
extern "C" const void * pirates_ni_geometry_get_rtti(void const *) {
    return pirates_ni_geometry_rtti;
}

static unsigned char pirates_ni_material_property_rtti[8]
    __asm__("__ZN18NiMaterialProperty6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_material_property_get_rtti(void const *)
    __asm__("__ZNK18NiMaterialProperty7GetRTTIEv");
extern "C" const void * pirates_ni_material_property_get_rtti(void const *) {
    return pirates_ni_material_property_rtti;
}

static unsigned char pirates_ni_back_to_front_accumulator_rtti[8]
    __asm__("__ZN24NiBackToFrontAccumulator6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_back_to_front_accumulator_get_rtti(void const *)
    __asm__("__ZNK24NiBackToFrontAccumulator7GetRTTIEv");
extern "C" const void * pirates_ni_back_to_front_accumulator_get_rtti(void const *) {
    return pirates_ni_back_to_front_accumulator_rtti;
}

static unsigned char pirates_ni_alpha_accumulator_rtti[8]
    __asm__("__ZN18NiAlphaAccumulator6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_alpha_accumulator_get_rtti(void const *)
    __asm__("__ZNK18NiAlphaAccumulator7GetRTTIEv");
extern "C" const void * pirates_ni_alpha_accumulator_get_rtti(void const *) {
    return pirates_ni_alpha_accumulator_rtti;
}

static unsigned char pirates_ni_alpha_property_rtti[8]
    __asm__("__ZN15NiAlphaProperty6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_alpha_property_get_rtti(void const *)
    __asm__("__ZNK15NiAlphaProperty7GetRTTIEv");
extern "C" const void * pirates_ni_alpha_property_get_rtti(void const *) {
    return pirates_ni_alpha_property_rtti;
}

static unsigned char pirates_ni_light_rtti[8]
    __asm__("__ZN7NiLight6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_light_get_rtti(void const *)
    __asm__("__ZNK7NiLight7GetRTTIEv");
extern "C" const void * pirates_ni_light_get_rtti(void const *) {
    return pirates_ni_light_rtti;
}

static unsigned char pirates_ni_ambient_light_rtti[8]
    __asm__("__ZN14NiAmbientLight6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_ambient_light_get_rtti(void const *)
    __asm__("__ZNK14NiAmbientLight7GetRTTIEv");
extern "C" const void * pirates_ni_ambient_light_get_rtti(void const *) {
    return pirates_ni_ambient_light_rtti;
}

static unsigned char pirates_ni_billboard_node_rtti[8]
    __asm__("__ZN15NiBillboardNode6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_billboard_node_get_rtti(void const *)
    __asm__("__ZNK15NiBillboardNode7GetRTTIEv");
extern "C" const void * pirates_ni_billboard_node_get_rtti(void const *) {
    return pirates_ni_billboard_node_rtti;
}

static unsigned char pirates_ni_binary_extra_data_rtti[8]
    __asm__("__ZN17NiBinaryExtraData6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_binary_extra_data_get_rtti(void const *)
    __asm__("__ZNK17NiBinaryExtraData7GetRTTIEv");
extern "C" const void * pirates_ni_binary_extra_data_get_rtti(void const *) {
    return pirates_ni_binary_extra_data_rtti;
}

static unsigned char pirates_ni_palette_rtti[8]
    __asm__("__ZN9NiPalette6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_palette_get_rtti(void const *)
    __asm__("__ZNK9NiPalette7GetRTTIEv");
extern "C" const void * pirates_ni_palette_get_rtti(void const *) {
    return pirates_ni_palette_rtti;
}

static unsigned char pirates_ni_blt_source_rtti[8]
    __asm__("__ZN11NiBltSource6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_blt_source_get_rtti(void const *)
    __asm__("__ZNK11NiBltSource7GetRTTIEv");
extern "C" const void * pirates_ni_blt_source_get_rtti(void const *) {
    return pirates_ni_blt_source_rtti;
}

static unsigned char pirates_ni_boolean_extra_data_rtti[8]
    __asm__("__ZN18NiBooleanExtraData6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_boolean_extra_data_get_rtti(void const *)
    __asm__("__ZNK18NiBooleanExtraData7GetRTTIEv");
extern "C" const void * pirates_ni_boolean_extra_data_get_rtti(void const *) {
    return pirates_ni_boolean_extra_data_rtti;
}

static unsigned char pirates_ni_bsp_node_rtti[8]
    __asm__("__ZN9NiBSPNode6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_bsp_node_get_rtti(void const *)
    __asm__("__ZNK9NiBSPNode7GetRTTIEv");
extern "C" const void * pirates_ni_bsp_node_get_rtti(void const *) {
    return pirates_ni_bsp_node_rtti;
}

static unsigned char pirates_ni_color_extra_data_rtti[8]
    __asm__("__ZN16NiColorExtraData6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_color_extra_data_get_rtti(void const *)
    __asm__("__ZNK16NiColorExtraData7GetRTTIEv");
extern "C" const void * pirates_ni_color_extra_data_get_rtti(void const *) {
    return pirates_ni_color_extra_data_rtti;
}

static unsigned char pirates_ni_directional_light_rtti[8]
    __asm__("__ZN18NiDirectionalLight6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_directional_light_get_rtti(void const *)
    __asm__("__ZNK18NiDirectionalLight7GetRTTIEv");
extern "C" const void * pirates_ni_directional_light_get_rtti(void const *) {
    return pirates_ni_directional_light_rtti;
}
