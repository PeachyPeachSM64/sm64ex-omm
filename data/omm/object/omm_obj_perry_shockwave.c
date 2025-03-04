#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define OMM_PERRY_SHOCKWAVE_WAVE_NUM_POINTS   (16)
#define OMM_PERRY_SHOCKWAVE_WAVE_RADIUS       (100.f)
#define OMM_PERRY_SHOCKWAVE_WAVE_SEGMENTS     (array_of(f32) { 0.f, 0.9f, 1.f })
#define OMM_PERRY_SHOCKWAVE_GLOW_NUM_POINTS   (16)
#define OMM_PERRY_SHOCKWAVE_GLOW_RADIUS       (128.f)
#define OMM_PERRY_SHOCKWAVE_GLOW_SEGMENTS     (array_of(f32) { 0.f, 0.25f, 1.f })

//
// Gfx data
//

static const Gfx omm_perry_shockwave_wave_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_PERRY_SHOCKWAVE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(NULL),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPEndDisplayList(),
};

static const Gfx omm_perry_shockwave_glow_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPDisplayList(NULL),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    Gfx *displayLists[2];
    struct {
        Gfx gfx[array_length(omm_perry_shockwave_wave_gfx)];
        Gfx tri[4 * OMM_PERRY_SHOCKWAVE_WAVE_NUM_POINTS + 1];
        Vtx vtx[8 * OMM_PERRY_SHOCKWAVE_WAVE_NUM_POINTS];
    } wave;
    struct {
        Gfx gfx[array_length(omm_perry_shockwave_glow_gfx)];
        Gfx tri[4 * OMM_PERRY_SHOCKWAVE_GLOW_NUM_POINTS + 1];
        Vtx vtx[8 * OMM_PERRY_SHOCKWAVE_GLOW_NUM_POINTS];
    } glow;
} OmmPeachPerryShockwaveGeoData;

static const u32 sOmmPeachPerryShockwaveGeoDataDisplayListsOffsets[] = {
    offsetof(OmmPeachPerryShockwaveGeoData, wave.gfx),
    offsetof(OmmPeachPerryShockwaveGeoData, glow.gfx),
};

//
// Geo layout
//

const GeoLayout omm_geo_perry_shockwave[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(0, geo_link_geo_data),
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_ASM(1, geo_link_geo_data),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_perry_shockwave_explode(struct Object *o, f32 x, f32 y, f32 z) {
    o->oPosX = x;
    o->oPosY = y;
    o->oPosZ = z;
    if (o->oPerryShockwaveBlast) {
        omm_obj_spawn_perry_blast(o, o->oPerryType);
        obj_mark_for_deletion(o);
    } else {
        o->oTimer = 0;
        o->oAction = 2;
    }
}

static void bhv_omm_perry_shockwave_perform_step(struct Object *o) {
    f32 halfRadius = OMM_PERRY_SHOCKWAVE_RADIUS / 2.f;
    f32 halfHeight = OMM_PERRY_SHOCKWAVE_HEIGHT / 2.f;
    for (s32 i = 0; i != 8; ++i) {
        o->oFloor = NULL;
        o->oPosX += o->oVelX / 8.f;
        o->oPosZ += o->oVelZ / 8.f;

        // Wall collision
        struct WallCollisionData colData = { .x = o->oPosX, .y = o->oPosY, .z = o->oPosZ, .offsetY = 0, .radius = halfRadius };
        if (find_wall_collisions(&colData)) {
            bhv_omm_perry_shockwave_explode(o, colData.x, colData.y, colData.z);
            return;
        }

        // Floor collision
        struct Surface *floor;
        f32 floorY = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
        if (o->oPosY < floorY + halfHeight) {
            if (floor && floor->normal.y >= 0.95f) {
                o->oPosY = floorY + halfHeight;
                o->oFloor = floor;
            } else if (o->oPosY < floorY) {
                bhv_omm_perry_shockwave_explode(o, o->oPosX, floorY + halfHeight, o->oPosZ);
                return;
            }
        }

        // Ceil collision
        struct Surface *ceil;
        f32 ceilY = find_ceil(o->oPosX, o->oPosY, o->oPosZ, &ceil);
        if (ceilY > floorY) {
            if (o->oPosY > ceilY - halfHeight) {
                if (ceil && ceil->normal.y <= -0.95f) {
                    o->oPosY = ceilY - halfHeight;
                    o->oFloor = ceil;
                } else if (o->oPosY > ceilY) {
                    bhv_omm_perry_shockwave_explode(o, o->oPosX, ceilY - halfHeight, o->oPosZ);
                    return;
                }
            }
        }
    }
}

static void bhv_omm_perry_shockwave_process_interactions(struct Object *o) {

    // Update hitbox
    f32 maxScale = OMM_PERRY_SHOCKWAVE_RADIUS / 100.f;
    obj_reset_hitbox(o,
        OMM_PERRY_SHOCKWAVE_RADIUS / maxScale,
        OMM_PERRY_SHOCKWAVE_HEIGHT / maxScale, 0, 0, 0,
        OMM_PERRY_SHOCKWAVE_HEIGHT / (maxScale * 2.f)
    );

    // Handle interactions
    struct Object *interacted = omm_obj_process_interactions(o, OMM_PERRY_SHOCKWAVE_INT_FLAGS);
    if (interacted && !omm_obj_is_collectible(interacted) && (!o->oPerryShockwaveBlast || !omm_obj_is_weak(interacted))) {
        bhv_omm_perry_shockwave_explode(o, o->oPosX, o->oPosY, o->oPosZ);
    }
}

static void bhv_omm_perry_shockwave_update() {
    struct Object *o = gCurrentObject;

    // Actions
    switch (o->oAction) {

        // Grows during delay frames
        case 0: {
            f32 t = invlerp_0_1_s(o->oTimer, 0, o->oPerryShockwaveDelay);
            obj_scale(o, t * OMM_PERRY_SHOCKWAVE_RADIUS / 100.f);
            o->oOpacity = t * 0xFF;
            o->oPosX = gMarioState->pos[0];
            o->oPosY = gMarioState->pos[1] + 80.f * gMarioObject->oScaleY;
            o->oPosZ = gMarioState->pos[2];
            if (t == 1.f) {
                o->oAction = 1;
                o->oTimer = 0;
                o->oVelX = OMM_PERRY_SHOCKWAVE_FORWARD_VEL * sins(gMarioState->faceAngle[1]);
                o->oVelZ = OMM_PERRY_SHOCKWAVE_FORWARD_VEL * coss(gMarioState->faceAngle[1]);
            }
        } break;

        // Moves and interacts
        case 1: {
            obj_scale(o, OMM_PERRY_SHOCKWAVE_RADIUS / 100.f);
            o->oOpacity = 0xFF;
            bhv_omm_perry_shockwave_perform_step(o);
            bhv_omm_perry_shockwave_process_interactions(o);
            if (o->oTimer >= OMM_PERRY_SHOCKWAVE_DURATION) {
                bhv_omm_perry_shockwave_explode(o, o->oPosX, o->oPosY, o->oPosZ);
            }
        } break;

        // Vanishes and unloads
        case 2: {
            obj_scale(o, relerp_0_1_f(o->oTimer, 0, 15, 1.f, 3.f) * OMM_PERRY_SHOCKWAVE_RADIUS / 100.f);
            o->oOpacity = clamp_s(invlerp_0_1_s(o->oTimer, 15, 0) * 0x140, 0x00, 0xFF);
            o->oIntangibleTimer = -1;
            if (o->oTimer >= 15) {
                obj_mark_for_deletion(o);
            }
        } break;

        // Trail
        case 3: {
            o->oOpacity = relerp_0_1_s(o->oTimer, 0, 15, 0xFF, 0x00);
            obj_scale(o, relerp_0_1_f(o->oTimer, 0, 15, o->oPerryShockwaveBaseScale, 0.f));
            if (o->oTimer >= 15) {
                obj_mark_for_deletion(o);
            }
        } break;
    }

    // Spawn trail
    if (o->oAction == 1 && o->oPerryShockwaveBlast) {
        struct Object *trail = omm_obj_spawn_perry_shockwave(o, 0, o->oPerryType, 0);
        trail->oPerryShockwaveBaseScale = o->oScaleX;
        trail->oIntangibleTimer = -1;
        trail->oAction = 3;
        omm_obj_spawn_sparkle(o,
            lerp_s(0.5f, OMM_PERRY_COLOR_FRONT[0], 0xFF),
            lerp_s(0.5f, OMM_PERRY_COLOR_FRONT[1], 0xFF),
            lerp_s(0.5f, OMM_PERRY_COLOR_FRONT[2], 0xFF),
            o->oScaleX * 40.f,
            o->oScaleX / 15.f,
            o->oScaleX / 1.5f
        );
    }

    // Update gfx
    OmmPeachPerryShockwaveGeoData *data = geo_get_geo_data(o,
        sizeof(OmmPeachPerryShockwaveGeoData),
        sOmmPeachPerryShockwaveGeoDataDisplayListsOffsets,
        array_length(sOmmPeachPerryShockwaveGeoDataDisplayListsOffsets)
    );
    o->oPerryShockwaveAngleYaw += 0x1800 * o->oPerryShockwaveAngleDir;
    obj_set_angle(o, 0, 0, 0);

    // Wave
    Vtx *waveVtx = data->wave.vtx;
    Gfx *waveTri = data->wave.tri;
    if (o->oAction != 3) {

        // Axes
        Vec3f n, e1, e2, up;
        if (o->oFloor) {
            f32 d = (o->oFloor->normal.y < 0.f ? -1.f : +1.f);
            up[0] = (o->oFloor->normal.x * d);
            up[1] = (o->oFloor->normal.y * d);
            up[2] = (o->oFloor->normal.z * d);
        } else {
            up[0] = 0.f;
            up[1] = 1.f;
            up[2] = 0.f;
        }
        vec3f_get_nullspace(n, e1, e2, up);

        // Wave
        for (s32 j = 0; j != 2; ++j) {
            f32 t0 = OMM_PERRY_SHOCKWAVE_WAVE_SEGMENTS[j + 0];
            f32 t1 = OMM_PERRY_SHOCKWAVE_WAVE_SEGMENTS[j + 1];
            f32 r0 = OMM_PERRY_SHOCKWAVE_WAVE_RADIUS * t0;
            f32 r1 = OMM_PERRY_SHOCKWAVE_WAVE_RADIUS * t1;

            // Points
            for (s32 i = 0; i != OMM_PERRY_SHOCKWAVE_WAVE_NUM_POINTS; ++i) {
                s16 a0 = ((i + 0) * 0x10000) / OMM_PERRY_SHOCKWAVE_WAVE_NUM_POINTS;
                s16 a1 = ((i + 1) * 0x10000) / OMM_PERRY_SHOCKWAVE_WAVE_NUM_POINTS;

                // Vertex coords
                Vec3f p0, p1;
                vec3f_rotate_around_n(p0, e1, up, a0 + o->oPerryShockwaveAngleYaw);
                vec3f_rotate_around_n(p1, e1, up, a1 + o->oPerryShockwaveAngleYaw);

                // Tex coords
                s32 u00 = (sins(a0) * t0 * 64 + 64) * 32;
                s32 v00 = (coss(a0) * t0 * 64 + 64) * 32;
                s32 u01 = (sins(a1) * t0 * 64 + 64) * 32;
                s32 v01 = (coss(a1) * t0 * 64 + 64) * 32;
                s32 u10 = (sins(a0) * t1 * 64 + 64) * 32;
                s32 v10 = (coss(a0) * t1 * 64 + 64) * 32;
                s32 u11 = (sins(a1) * t1 * 64 + 64) * 32;
                s32 v11 = (coss(a1) * t1 * 64 + 64) * 32;

                // Triangles
                gSPVertex(waveTri++, waveVtx, 4, 0);
                gSP2Triangles(waveTri++, 0, 1, 2, 0, 2, 1, 3, 0);

                // Vertices
                *(waveVtx++) = (Vtx) { { { r0 * p0[0], r0 * p0[1], r0 * p0[2] }, 0, { u00, v00 * o->oPerryShockwaveAngleDir }, { OMM_PERRY_COLOR(2 - (j + 0), 0), OMM_PERRY_COLOR(2 - (j + 0), 1), OMM_PERRY_COLOR(2 - (j + 0), 2), o->oOpacity * (o->oAction != 2) } } };
                *(waveVtx++) = (Vtx) { { { r0 * p1[0], r0 * p1[1], r0 * p1[2] }, 0, { u01, v01 * o->oPerryShockwaveAngleDir }, { OMM_PERRY_COLOR(2 - (j + 0), 0), OMM_PERRY_COLOR(2 - (j + 0), 1), OMM_PERRY_COLOR(2 - (j + 0), 2), o->oOpacity * (o->oAction != 2) } } };
                *(waveVtx++) = (Vtx) { { { r1 * p0[0], r1 * p0[1], r1 * p0[2] }, 0, { u10, v10 * o->oPerryShockwaveAngleDir }, { OMM_PERRY_COLOR(2 - (j + 1), 0), OMM_PERRY_COLOR(2 - (j + 1), 1), OMM_PERRY_COLOR(2 - (j + 1), 2), o->oOpacity * (o->oAction != 2) } } };
                *(waveVtx++) = (Vtx) { { { r1 * p1[0], r1 * p1[1], r1 * p1[2] }, 0, { u11, v11 * o->oPerryShockwaveAngleDir }, { OMM_PERRY_COLOR(2 - (j + 1), 0), OMM_PERRY_COLOR(2 - (j + 1), 1), OMM_PERRY_COLOR(2 - (j + 1), 2), o->oOpacity * (o->oAction != 2) } } };
            }
        }
    }
    gSPEndDisplayList(waveTri++);
    gfx_copy_and_fill_null(data->wave.gfx, omm_perry_shockwave_wave_gfx, sizeof(omm_perry_shockwave_wave_gfx), data->wave.tri);

    // Glow
    Vtx *glowVtx = data->glow.vtx;
    Gfx *glowTri = data->glow.tri;
    f32 glowScale = 1.f;
    if (o->oAction != 2) {
        Vec3f v; vec3f_normalize(vec3f_dif(v, gCamera->focus, gCamera->pos));
        glowScale = relerp_0_1_f(abs_f(v[1]), 0.f, 1.f, 1.f / 3.f, 1.f);
    }
    for (s32 j = 0; j != 2; ++j) {
        f32 t0 = OMM_PERRY_SHOCKWAVE_GLOW_SEGMENTS[j + 0];
        f32 t1 = OMM_PERRY_SHOCKWAVE_GLOW_SEGMENTS[j + 1];
        f32 r0 = OMM_PERRY_SHOCKWAVE_GLOW_RADIUS * t0;
        f32 r1 = OMM_PERRY_SHOCKWAVE_GLOW_RADIUS * t1;

        // Points
        for (s32 i = 0; i != OMM_PERRY_SHOCKWAVE_GLOW_NUM_POINTS; ++i) {
            s16 a0 = ((i + 0) * 0x10000) / OMM_PERRY_SHOCKWAVE_GLOW_NUM_POINTS;
            s16 a1 = ((i + 1) * 0x10000) / OMM_PERRY_SHOCKWAVE_GLOW_NUM_POINTS;

            // Vertex coords
            f32 x0 = sins(a0);
            f32 y0 = coss(a0);
            f32 x1 = sins(a1);
            f32 y1 = coss(a1);

            // Triangles
            gSPVertex(glowTri++, glowVtx, 4, 0);
            gSP2Triangles(glowTri++, 0, 1, 2, 0, 2, 1, 3, 0);

            // Vertices
            *(glowVtx++) = (Vtx) { { { r0 * x0, r0 * y0 * glowScale, 0 }, 0, { 0, 0 }, { OMM_PERRY_COLOR(j + 0, 0), OMM_PERRY_COLOR(j + 0, 1), OMM_PERRY_COLOR(j + 0, 2), o->oOpacity * (1.f - t0) } } };
            *(glowVtx++) = (Vtx) { { { r0 * x1, r0 * y1 * glowScale, 0 }, 0, { 0, 0 }, { OMM_PERRY_COLOR(j + 0, 0), OMM_PERRY_COLOR(j + 0, 1), OMM_PERRY_COLOR(j + 0, 2), o->oOpacity * (1.f - t0) } } };
            *(glowVtx++) = (Vtx) { { { r1 * x0, r1 * y0 * glowScale, 0 }, 0, { 0, 0 }, { OMM_PERRY_COLOR(j + 1, 0), OMM_PERRY_COLOR(j + 1, 1), OMM_PERRY_COLOR(j + 1, 2), o->oOpacity * (1.f - t1) } } };
            *(glowVtx++) = (Vtx) { { { r1 * x1, r1 * y1 * glowScale, 0 }, 0, { 0, 0 }, { OMM_PERRY_COLOR(j + 1, 0), OMM_PERRY_COLOR(j + 1, 1), OMM_PERRY_COLOR(j + 1, 2), o->oOpacity * (1.f - t1) } } };
        }
    }
    gSPEndDisplayList(glowTri++);
    gfx_copy_and_fill_null(data->glow.gfx, omm_perry_shockwave_glow_gfx, sizeof(omm_perry_shockwave_glow_gfx), data->glow.tri);
}

const BehaviorScript bhvOmmPerryShockwave[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_perry_shockwave_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_perry_shockwave(struct Object *o, s32 delay, s32 type, bool clockwise) {
    struct Object *wave = obj_spawn_from_geo(o, omm_geo_perry_shockwave, bhvOmmPerryShockwave);
    obj_set_always_rendered(wave, true);
    obj_set_angle(wave, 0, 0, 0);
    obj_set_params(wave, 0, 0, 0, 0, true);
    obj_reset_hitbox(wave, 0, 0, 0, 0, 0, 0);
    wave->oPerryType = type;
    wave->oPerryShockwaveDelay = max_s(1, delay);
    wave->oPerryShockwaveAngleYaw = gMarioState->faceAngle[1] + 0x2000 * (delay - 7 + 2 * clockwise);
    wave->oPerryShockwaveAngleDir = (clockwise ? -1 : +1);
    if (gOmmPerryBlast) {
        omm_sound_play(OMM_SOUND_EFFECT_PERRY_BEAM, o->oCameraToObject);
        wave->oPerryShockwaveBlast = true;
        gOmmPerryBlast = false;
    } else {
        wave->oPerryShockwaveBlast = false;
    }
    return wave;
}
