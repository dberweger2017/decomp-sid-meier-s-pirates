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
