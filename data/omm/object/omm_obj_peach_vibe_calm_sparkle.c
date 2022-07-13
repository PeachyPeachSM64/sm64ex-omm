#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Vtx omm_peach_vibe_calm_sparkle_vertices[] = {
    OMM_VTX_RECT(32, 0, 0, 1),
};

static const Gfx omm_peach_vibe_calm_sparkle_triangles[] = {
    OMM_GFX_QUAD(omm_peach_vibe_calm_sparkle_vertices, 0),
    gsSPEndDisplayList(),
    gsSPEndDisplayList(),
    gsSPEndDisplayList(),
};

OMM_GFX_DL(omm_peach_vibe_calm_sparkle, OMM_TEXTURE_EFFECT_VIBE_CALM_SPARKLE, omm_peach_vibe_calm_sparkle_triangles, 0);
OMM_GFX_DL(omm_peach_vibe_calm_sparkle, OMM_TEXTURE_EFFECT_VIBE_CALM_SPARKLE, omm_peach_vibe_calm_sparkle_triangles, 1);

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_calm_sparkle[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_sparkle_0_gfx),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_peach_vibe_calm_sparkle_1_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_peach_vibe_calm_sparkle_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer < 30) {
        f32 *marioRootPos = geo_get_marios_root_pos();
        o->oPosX = marioRootPos[0] + o->oHomeX + o->oVelX * o->oTimer;
        o->oPosY = marioRootPos[1] + o->oHomeY + o->oVelY * o->oTimer;
        o->oPosZ = marioRootPos[2] + o->oHomeZ + o->oVelZ * o->oTimer;
        o->oAnimState++;
    } else {
        obj_mark_for_deletion(o);
    
    }
}

const BehaviorScript omm_bhv_peach_vibe_calm_sparkle[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_calm_sparkle_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_peach_vibe_calm_sparkle(struct Object *o) {
    struct Object *sparkle = obj_spawn_from_geo(o, omm_geo_peach_vibe_calm_sparkle, omm_bhv_peach_vibe_calm_sparkle);
    obj_set_always_rendered(sparkle, true);
    obj_set_angle(sparkle, 0, 0, 0);
    obj_scale(sparkle, lerp_f(random_float(), 0.5f, 1.f));
    f32 r = lerp_f(random_float(), 0.f, 100.f);
    s16 a = random_u16();
    sparkle->oHomeX = r * sins(a);
    sparkle->oHomeY = lerp_f(random_float(), -gMarioObject->hitboxHeight / 2.f, 0.f);
    sparkle->oHomeZ = r * coss(a);
    sparkle->oVelX  = 0.f;
    sparkle->oVelY  = lerp_f(random_float(), 1.f, 8.f);
    sparkle->oVelZ  = 0.f;
    return sparkle;
}
