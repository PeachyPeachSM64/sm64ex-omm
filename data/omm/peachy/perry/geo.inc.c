#ifdef VSCODE
#include "model.inc.c"
#endif

const GeoLayout omm_geo_perry[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(10, geo_update_layer_transparency),
        GEO_SWITCH_CASE(2, geo_switch_anim_state),
        GEO_OPEN_NODE(),
            GEO_TRANSLATE_NODE(LAYER_OPAQUE, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_CULLING_RADIUS(14), // Switch between Perry's closed and open states
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_perry_close_opaque_gfx),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_perry_open_opaque_gfx),
            GEO_CLOSE_NODE(),
            GEO_TRANSLATE_NODE(LAYER_TRANSPARENT, 0, 0, 0),
            GEO_OPEN_NODE(),
                GEO_CULLING_RADIUS(14), // Switch between Perry's closed and open states
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_perry_close_transparent_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_perry_open_transparent_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
