#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Gfx data
//

static const Lights1 omm_rising_lava_light = gdSPDefLights1(
    0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF,
    0x28, 0x28, 0x28
);

static const Vtx omm_rising_lava_vertices[] = {
    { { { -256, 0, -256 }, 0, { 0, 0 }, { 0, 127, 0, 255 } } },
    { { { -256, 0, +256 }, 0, { 4096, 0 }, { 0, 127, 0, 255 } } },
    { { { +256, 0, +256 }, 0, { 4096, 4096 }, { 0, 127, 0, 255 } } },
    { { { +256, 0, -256 }, 0, { 0, 4096 }, { 0, 127, 0, 255 } } },
};

static const Gfx omm_rising_lava_triangles[] = {
    gsSPVertex(omm_rising_lava_vertices, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_rising_lava_gfx[] = {
    gsDPSetCombineLERP(TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, ENVIRONMENT, TEXEL0, SHADE, TEXEL0_ALPHA, SHADE, 0, 0, 0, ENVIRONMENT),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_LAVA, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPLight(&omm_rising_lava_light.l, 1),
    gsSPLight(&omm_rising_lava_light.a, 2),
    gsSPDisplayList(omm_rising_lava_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_rising_lava[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_rising_lava_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_rising_lava_update() {
    struct Object *o = gCurrentObject;
    obj_scale(o, o->oRisingLavaRadius / 256.f);
    o->oCollisionDistance = o->oRisingLavaRadius;
    o->oFaceAngleYaw += o->oRisingLavaRotVel;
    switch (o->oAction) {

        // Idle
        case 0: {
            o->oPosY = clamp_f(o->oPosY, o->oRisingLavaMinY, o->oRisingLavaMaxY);
        } break;

        // Rising
        case 1: {
            set_environmental_camera_shake(o->oRisingLavaShake);
            o->oPosY += abs_f(o->oRisingLavaVelY);
            if (o->oPosY >= o->oRisingLavaMaxY) {
                o->oPosY = o->oRisingLavaMaxY;
                o->oAction = 3;
            }
        } break;

        // Decreasing
        case 2: {
            o->oPosY -= abs_f(o->oRisingLavaVelY);
            if (o->oPosY <= o->oRisingLavaMinY) {
                o->oPosY = o->oRisingLavaMinY;
                o->oAction = 4;
            }
        } break;

        // Stopped (max)
        case 3: {
            set_environmental_camera_shake(o->oRisingLavaShake);
            o->oPosY = o->oRisingLavaMaxY + 15.f * sins(gGlobalTimer * 0x200);
        } break;

        // Stopped (min)
        case 4: {
            o->oPosY = o->oRisingLavaMinY + 15.f * sins(gGlobalTimer * 0x200);
        } break;
    }
}

static const Collision omm_rising_lava_collision[] = {
    COL_INIT(),
    COL_VERTEX_INIT(4),
    COL_VERTEX(-256, 0, -256),
    COL_VERTEX(-256, 0, +256),
    COL_VERTEX(+256, 0, +256),
    COL_VERTEX(+256, 0, -256),
    COL_TRI_INIT(SURFACE_BURNING, 2),
    COL_TRI(0, 1, 2),
    COL_TRI(0, 2, 3),
    COL_TRI_STOP(),
    COL_END(),
};

const BehaviorScript omm_bhv_rising_lava[] = {
    OBJ_TYPE_SURFACE,
    0x11012041,
    0x2A000000, (uintptr_t) omm_rising_lava_collision,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_rising_lava_update,
    0x10050000,
    0x0C000000, (uintptr_t) load_object_collision_model,
    0x102B0000,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_rising_lava(struct Object *o, f32 x, f32 y, f32 z, f32 yMin, f32 yMax, f32 yVel, f32 radius, f32 rotVel, s32 shakeEnv) {
    struct Object *lava = obj_spawn_from_geo(o, omm_geo_rising_lava, omm_bhv_rising_lava);
    obj_set_pos(lava, x, y, z);
    obj_set_angle(lava, 0, 0, 0);
    obj_set_always_rendered(lava, true);
    lava->oRisingLavaMinY   = yMin;
    lava->oRisingLavaMaxY   = yMax;
    lava->oRisingLavaVelY   = yVel;
    lava->oRisingLavaRadius = radius;
    lava->oRisingLavaRotVel = rotVel;
    lava->oRisingLavaShake  = shakeEnv;
    lava->oAction           = 0;
    return lava;
}
