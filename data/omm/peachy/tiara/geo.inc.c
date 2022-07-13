#ifdef VSCODE
#include "model.inc.c"
#endif

const GeoLayout omm_geo_peachs_cap[] = {
    GEO_SHADOW(SHADOW_CIRCLE_9_VERTS, 0xB4, 60),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x6800),
        GEO_OPEN_NODE(),
            GEO_ROTATION_NODE(0, 0, 90, 90),
            GEO_OPEN_NODE(),
                GEO_ASM(10, geo_update_layer_transparency),
                GEO_SWITCH_CASE(2, geo_switch_anim_state),
                GEO_OPEN_NODE(),
                    GEO_TRANSLATE_NODE(LAYER_OPAQUE, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_CULLING_RADIUS(13), // Switch between Peach's crown and Tiara
                        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_peachs_cap_crown_gfx),
                        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_peachs_cap_tiara_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(LAYER_TRANSPARENT, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_CULLING_RADIUS(13), // Switch between Peach's crown and Tiara
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peachs_cap_crown_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peachs_cap_tiara_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout omm_geo_peachs_metal_cap[] = {
    GEO_SHADOW(SHADOW_CIRCLE_9_VERTS, 0xB4, 60),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x6800),
        GEO_OPEN_NODE(),
            GEO_ROTATION_NODE(0, 0, 90, 90),
            GEO_OPEN_NODE(),
                GEO_ASM(10, geo_update_layer_transparency),
                GEO_SWITCH_CASE(2, geo_switch_anim_state),
                GEO_OPEN_NODE(),
                    GEO_TRANSLATE_NODE(LAYER_OPAQUE, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_CULLING_RADIUS(13), // Switch between Peach's crown and Tiara
                        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_peachs_metal_cap_crown_gfx),
                        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_peachs_metal_cap_tiara_gfx),
                    GEO_CLOSE_NODE(),
                    GEO_TRANSLATE_NODE(LAYER_TRANSPARENT, 0, 0, 0),
                    GEO_OPEN_NODE(),
                        GEO_CULLING_RADIUS(13), // Switch between Peach's crown and Tiara
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peachs_metal_cap_crown_gfx),
                        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peachs_metal_cap_tiara_gfx),
                    GEO_CLOSE_NODE(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
