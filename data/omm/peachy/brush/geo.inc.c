#ifdef VSCODE
#include "model.inc.c"
#endif

const GeoLayout omm_geo_brush[] = {
    GEO_SCALE(0, 0x2000),
    GEO_OPEN_NODE(),
        GEO_ASM(10, geo_update_layer_transparency),
        GEO_SWITCH_CASE(2, geo_switch_anim_state),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_brush_opaque_gfx),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_brush_transparent_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
