#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS        128
#define OMM_PEACH_VIBE_RAGE_SHOCKWAVE_SPEED         40.f
#define OMM_PEACH_VIBE_RAGE_SHOCKWAVE_WIDTH         40.f
#define OMM_PEACH_VIBE_RAGE_SHOCKWAVE_HEIGHT        120.f
#define OMM_PEACH_VIBE_RAGE_SHOCKWAVE_DURATION      45
#define OMM_PEACH_VIBE_RAGE_SHOCKWAVE_TEX_WIDTH     64
#define OMM_PEACH_VIBE_RAGE_SHOCKWAVE_TEX_HEIGHT    (8 * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_TEX_WIDTH)

//
// Gfx data
//

static const Gfx omm_peach_vibe_rage_shockwave_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(null),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    f32 x, y, z;
    bool onGround;
    bool processed;
} State;

typedef struct {
    Gfx gfx[OMM_ARRAY_SIZE(omm_peach_vibe_rage_shockwave_gfx)];
    Gfx tri[2 * (2 * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS + 8) + 1];
    Vtx vtx[2 * (2 * (OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS + 1))];
    State state[OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS + 1];
} OmmPeachVibeRageShockwaveGeoData;

//
// Geo layout
//

const GeoLayout omm_geo_peach_vibe_rage_shockwave[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(0, geo_link_geo_data),
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void omm_bhv_peach_vibe_rage_shockwave_update() {
    struct Object *o = gCurrentObject;
    OmmPeachVibeRageShockwaveGeoData *data = geo_get_geo_data(o, sizeof(OmmPeachVibeRageShockwaveGeoData), omm_peach_vibe_rage_shockwave_gfx, sizeof(omm_peach_vibe_rage_shockwave_gfx));
    f32 r = o->oTimer * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_SPEED;

    // Update state
    bool airOnly = true;
    for (s32 i = 0; i != OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS; ++i) {
        s32 a = (i * 0x10000) / OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS;
        State *state = &data->state[i];
        state->processed = false;
        state->onGround = false;

        // Update x, z coords
        state->x = sins(a) * r;
        state->z = coss(a) * r;

        // Find floor and update y coord
        f32 y = o->oPosY + state->y;
        struct Surface *floor = NULL;
        f32 fy = find_floor(o->oPosX + state->x, y, o->oPosZ + state->z, &floor);
        if (floor && floor->type != SURFACE_DEATH_PLANE && floor->type != SURFACE_VERTICAL_WIND) {
            if (y < fy + OMM_PEACH_VIBE_RAGE_SHOCKWAVE_SPEED * 1.5f) {
                state->onGround = true;
                airOnly = false;
                y = fy;
            }
        }
        state->y = y - o->oPosY;
    }

    // Smooth the curve between nearest ground points
    if (!airOnly) {
        for (s32 i = 0, g0 = -1, nProcessed = 0; nProcessed < OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS; i = (i + 1) % OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS) {

            // On ground
            if (data->state[i].onGround) {
                if (!data->state[i].processed) {
                    data->state[i].processed = true;
                    nProcessed++;
                }
                g0 = i;
            }
                
            // Airborne, skip until at least one ground point is processed
            else if (g0 != -1) {

                // Find the second nearest ground point
                for (s32 g1 = i;; g1 = (g1 + 1) % OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS) {
                    if (data->state[g1].onGround) {

                        // Interpolate between the two points
                        for (s32 j = (g0 + 1) % OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS; j != g1; j = (j + 1) % OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS) {
                            if (!data->state[j].processed) {
                                data->state[j].y = relerp_0_1_f(
                                    j + (j < g0) * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS,
                                    g0, g1 + (g1 < g0) * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS,
                                    data->state[g0].y, data->state[g1].y
                                );
                                data->state[j].processed = true;
                                nProcessed++;
                            }
                        }
                        i = g1 - 1;
                        break;
                    }
                }
            }
        }
    }
    data->state[OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS] = data->state[0];

    // Gfx data
    f32 flameTexWidth = OMM_PEACH_VIBE_RAGE_SHOCKWAVE_TEX_WIDTH;
    f32 flameTexHeight = OMM_PEACH_VIBE_RAGE_SHOCKWAVE_TEX_HEIGHT / 20.f;
    f32 flameHeight = OMM_PEACH_VIBE_RAGE_SHOCKWAVE_HEIGHT;
    f32 flameWidth = 2.f * (flameTexWidth * flameHeight / flameTexHeight);
    f32 perimeter = 2.f * M_PI * r;
    s32 nbFlames = max_s(1, (s32) (0.5f + (perimeter / flameWidth)));
    f32 du = (nbFlames * flameTexWidth) / OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS;
    f32 v0 = ((o->oTimer % 20) + 0.95f) * flameTexHeight;
    f32 v1 = ((o->oTimer % 20) + 0.05f) * flameTexHeight;
    f32 alpha = relerp_0_1_f(o->oTimer, OMM_PEACH_VIBE_RAGE_SHOCKWAVE_DURATION - 15, OMM_PEACH_VIBE_RAGE_SHOCKWAVE_DURATION, 1.f, 0.f);
    Vtx *vtx = data->vtx;
    Gfx *tri = data->tri;

    // Wave
    State *state = data->state;
    gDPLoadTextureBlock(tri++, OMM_TEXTURE_EFFECT_VIBE_RAGE_SHOCKWAVE_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0);
    for (s32 i = 0; i <= OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS; ++i, state++, vtx += 2) {
        s32 a = (i * 0x10000) / OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS;

        // Vertices
        vtx[0].v.ob[0] = state->x - sins(a) * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_WIDTH;
        vtx[0].v.ob[1] = state->y + 10.f;
        vtx[0].v.ob[2] = state->z - coss(a) * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_WIDTH;
        vtx[0].v.tc[0] = 0x2000 * (i & 1);
        vtx[0].v.tc[1] = 0x0800;
        vtx[0].v.cn[0] = 0xFF;
        vtx[0].v.cn[1] = 0xFF;
        vtx[0].v.cn[2] = 0xFF;
        vtx[0].v.cn[3] = 0xFF * alpha;
        vtx[1].v.ob[0] = state->x + sins(a) * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_WIDTH;
        vtx[1].v.ob[1] = state->y + 10.f;
        vtx[1].v.ob[2] = state->z + coss(a) * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_WIDTH;
        vtx[1].v.tc[0] = 0x2000 * (i & 1);
        vtx[1].v.tc[1] = 0x1800;
        vtx[1].v.cn[0] = 0xFF;
        vtx[1].v.cn[1] = 0xFF;
        vtx[1].v.cn[2] = 0xFF;
        vtx[1].v.cn[3] = 0xFF * alpha;

        // Triangles
        if (i < OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS) {
            gSPVertex(tri++, vtx, 4, 0);
            gSP2Triangles(tri++, 0, 1, 2, 0, 2, 1, 3, 0);
        }
    }

    // Fire
    state = data->state;
    gDPLoadTextureBlock(tri++, OMM_TEXTURE_EFFECT_VIBE_RAGE_SHOCKWAVE_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, OMM_PEACH_VIBE_RAGE_SHOCKWAVE_TEX_WIDTH, OMM_PEACH_VIBE_RAGE_SHOCKWAVE_TEX_HEIGHT, 0, 0, 0, 0, 0, 0, 0);
    for (s32 i = 0; i <= OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS; ++i, state++, vtx += 2) {

        // Vertices
        vtx[0].v.ob[0] = state->x;
        vtx[0].v.ob[1] = state->y + 10.f;
        vtx[0].v.ob[2] = state->z;
        vtx[0].v.tc[0] = (s16) (32.f * (du * i)) - 0x8000;
        vtx[0].v.tc[1] = (s16) (32.f * v0);
        vtx[0].v.cn[0] = 0xFF;
        vtx[0].v.cn[1] = 0xFF;
        vtx[0].v.cn[2] = 0xFF;
        vtx[0].v.cn[3] = 0xFF * alpha;
        vtx[1].v.ob[0] = state->x;
        vtx[1].v.ob[1] = state->y + 10.f + OMM_PEACH_VIBE_RAGE_SHOCKWAVE_HEIGHT;
        vtx[1].v.ob[2] = state->z;
        vtx[1].v.tc[0] = (s16) (32.f * (du * i)) - 0x8000;
        vtx[1].v.tc[1] = (s16) (32.f * v1);
        vtx[1].v.cn[0] = 0xFF;
        vtx[1].v.cn[1] = 0xFF;
        vtx[1].v.cn[2] = 0xFF;
        vtx[1].v.cn[3] = 0xFF * alpha;

        // Triangles
        if (i < OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS) {
            gSPVertex(tri++, vtx, 4, 0);
            gSP2Triangles(tri++, 0, 1, 2, 0, 2, 1, 3, 0);
        }
    }
    gSPEndDisplayList(tri);

    // Update object
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);
    obj_set_params(o, 0, 0, 0, 0, false);
    obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    if (o->oTimer > OMM_PEACH_VIBE_RAGE_SHOCKWAVE_DURATION) {
        obj_mark_for_deletion(o);
        return;
    }

    // Process interactions
    struct Object hitbox = { .parentObj = o, .prevObj = o };
    obj_set_angle(&hitbox, 0, 0, 0);
    obj_set_scale(&hitbox, 1.f, 1.f, 1.f);
    obj_set_params(&hitbox, 0, 0, 0, 0, true);
    obj_reset_hitbox(&hitbox, OMM_PEACH_VIBE_RAGE_SHOCKWAVE_WIDTH, OMM_PEACH_VIBE_RAGE_SHOCKWAVE_HEIGHT, 0, 0, 0, 0);
    for_each_object_in_interaction_lists(obj) {

        // Check horizontal distance
        f32 hdist = obj_get_horizontal_distance(o, obj);
        if (hdist + obj->hitboxRadius < r - OMM_PEACH_VIBE_RAGE_SHOCKWAVE_WIDTH ||
            hdist - obj->hitboxRadius > r + OMM_PEACH_VIBE_RAGE_SHOCKWAVE_WIDTH) {
            continue;
        }

        // Check vertical distance
        u16 angle = (u16) atan2s(obj->oPosZ - o->oPosZ, obj->oPosX - o->oPosX);
        s32 index = (s32) (((f32) (angle * OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS) / (f32) 0x10000) + 0.5f);
        State *point = &data->state[index % OMM_PEACH_VIBE_RAGE_SHOCKWAVE_POINTS];
        if (obj->oPosY - obj->hitboxDownOffset + obj->hitboxHeight < o->oPosY + point->y ||
            obj->oPosY - obj->hitboxDownOffset > o->oPosY + point->y + OMM_PEACH_VIBE_RAGE_SHOCKWAVE_HEIGHT) {
            continue;
        }

        // Process one object interaction with a reduced hitbox
        obj_set_pos(&hitbox, o->oPosX + point->x, o->oPosY + point->y, o->oPosZ + point->z);
        omm_obj_process_one_object_interaction(&hitbox, obj, OBJ_INT_PRESET_PEACH_VIBE_RAGE_SHOCKWAVE);
    }
}

const BehaviorScript omm_bhv_peach_vibe_rage_shockwave[] = {
    OBJ_TYPE_SPECIAL,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_peach_vibe_rage_shockwave_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_peach_vibe_rage_shockwave(struct Object *o) {
    struct Object *wave = obj_spawn_from_geo(o, omm_geo_peach_vibe_rage_shockwave, omm_bhv_peach_vibe_rage_shockwave);
    obj_set_always_rendered(wave, true);
    obj_set_pos(wave, o->oPosX, o->oPosY, o->oPosZ);
    obj_set_home(wave, o->oPosX, o->oPosY, o->oPosZ);
    obj_set_scale(wave, 0.f, 0.f, 0.f);
    return wave;
}
