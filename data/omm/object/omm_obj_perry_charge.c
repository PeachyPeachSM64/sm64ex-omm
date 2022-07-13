#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_PERRY_CHARGE_NUM_FRAMES             8
#define OMM_PERRY_CHARGE_NUM_POINTS_PER_FRAME   4
#define OMM_PERRY_CHARGE_NUM_POINTS             (OMM_PERRY_CHARGE_NUM_FRAMES * OMM_PERRY_CHARGE_NUM_POINTS_PER_FRAME)
#define OMM_TEXTURE_EFFECT_PERRY_CHARGE_GLOW    (!(random_u16() % 3) ? OMM_TEXTURE_EFFECT_PERRY_CHARGE_GLOW_0 : ((random_u16() & 1) ? OMM_TEXTURE_EFFECT_PERRY_CHARGE_GLOW_1 : OMM_TEXTURE_EFFECT_PERRY_CHARGE_GLOW_2))

//
// Gfx data
//

static const Gfx omm_perry_charge_gfx[] = {
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
    Gfx gfx[OMM_ARRAY_SIZE(omm_perry_charge_gfx)];
    Gfx tri[(1 + OMM_PERRY_CHARGE_NUM_FRAMES + OMM_PERRY_CHARGE_NUM_POINTS) * 2 + 22];
    Vtx vtx[(1 + OMM_PERRY_CHARGE_NUM_FRAMES + OMM_PERRY_CHARGE_NUM_POINTS) * 4];
    Vec3f pos[OMM_PERRY_CHARGE_NUM_POINTS];
} OmmPerryChargeGeoData;

//
// Geo layouts
//

const GeoLayout omm_geo_perry_charge[] = {
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

static void omm_bhv_perry_charge_draw_billboard(Vtx **vtx, Gfx **tri, Vec3f pos0, Vec3f pos3d, f32 camOffset, s16 angle, f32 scale, f32 opacity) {

    // Billboard plane
    Vec3f camN, camE1, camE2;
    camN[0] = gCamera->pos[0] - pos3d[0];
    camN[1] = gCamera->pos[1] - pos3d[1];
    camN[2] = gCamera->pos[2] - pos3d[2];
    vec3f_get_nullspace(camN, camE1, camE2, camN);
    
    // Vertices
    Vec3f vz = { 0, 0, 1 };
    (*vtx)[0] = (Vtx) { { { -64 * scale, -64 * scale, 0 }, 0, { 0x0000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
    (*vtx)[1] = (Vtx) { { { +64 * scale, -64 * scale, 0 }, 0, { 0x1000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
    (*vtx)[2] = (Vtx) { { { +64 * scale, +64 * scale, 0 }, 0, { 0x1000, 0x1000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
    (*vtx)[3] = (Vtx) { { { -64 * scale, +64 * scale, 0 }, 0, { 0x0000, 0x1000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } };
    vec3f_rotate_around_n((*vtx)[0].v.ob, (*vtx)[0].v.ob, vz, angle);
    vec3f_rotate_around_n((*vtx)[1].v.ob, (*vtx)[1].v.ob, vz, angle);
    vec3f_rotate_around_n((*vtx)[2].v.ob, (*vtx)[2].v.ob, vz, angle);
    vec3f_rotate_around_n((*vtx)[3].v.ob, (*vtx)[3].v.ob, vz, angle);
    vec2f_to_3d_plane((*vtx)[0].v.ob, (*vtx)[0].v.ob, pos3d, camE1, gVec3fOne, camE2, gVec3fOne);
    vec2f_to_3d_plane((*vtx)[1].v.ob, (*vtx)[1].v.ob, pos3d, camE1, gVec3fOne, camE2, gVec3fOne);
    vec2f_to_3d_plane((*vtx)[2].v.ob, (*vtx)[2].v.ob, pos3d, camE1, gVec3fOne, camE2, gVec3fOne);
    vec2f_to_3d_plane((*vtx)[3].v.ob, (*vtx)[3].v.ob, pos3d, camE1, gVec3fOne, camE2, gVec3fOne);
    vec3f_copy(vz, camN);
    vec3f_normalize(vz);
    vec3f_mul(vz, camOffset);
    vec3f_add((*vtx)[0].v.ob, vz);
    vec3f_add((*vtx)[1].v.ob, vz);
    vec3f_add((*vtx)[2].v.ob, vz);
    vec3f_add((*vtx)[3].v.ob, vz);
    vec3f_sub((*vtx)[0].v.ob, pos0);
    vec3f_sub((*vtx)[1].v.ob, pos0);
    vec3f_sub((*vtx)[2].v.ob, pos0);
    vec3f_sub((*vtx)[3].v.ob, pos0);
    (*vtx)[0].v.cn[3] = 0xFF * opacity;
    (*vtx)[1].v.cn[3] = 0xFF * opacity;
    (*vtx)[2].v.cn[3] = 0xFF * opacity;
    (*vtx)[3].v.cn[3] = 0xFF * opacity;

    // Triangles
    gSPVertex((*tri)++, (*vtx), 4, 0);
    gSP2Triangles((*tri)++, 0, 1, 2, 0, 2, 3, 0, 0);
    (*vtx) += 4;
}

static void omm_bhv_perry_charge_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = omm_peach_get_perry_object();
    struct MarioState *m = gMarioState;
    OmmPerryChargeGeoData *data = NULL;
    if (!p || !OMM_PLAYER_IS_PEACH) {
        obj_mark_for_deletion(o);
        return;
    }

    // Compute Perry values
    Vec3f pos0 = { p->oPosX, p->oPosY, p->oPosZ };
    Vec3f pos1;
    Vec3f scale;
    if (p->oScaleX > 0.f) {
        Vec3f delta = { 0.f, 110.f, 0.f };
        vec3f_rotate_zxy(delta, delta, p->oFaceAnglePitch, p->oFaceAngleYaw, p->oFaceAngleRoll);
        vec3f_mult(delta, delta, p->oGfxScale);
        vec3f_sum(pos1, pos0, delta);
        vec3f_copy(scale, p->oGfxScale);
    } else {
        vec3f_copy(pos1, pos0);
        vec3f_copy(scale, m->marioObj->oGfxScale);
    }

    // Update position
    data = geo_get_geo_data(o, sizeof(OmmPerryChargeGeoData), omm_perry_charge_gfx, sizeof(omm_perry_charge_gfx));
    OMM_MEMMOV(data->pos + OMM_PERRY_CHARGE_NUM_POINTS_PER_FRAME, data->pos, sizeof(Vec3f) * OMM_PERRY_CHARGE_NUM_POINTS_PER_FRAME * (OMM_PERRY_CHARGE_NUM_FRAMES - 1));
    vec3f_copy(data->pos[0], pos0);
    Vtx *vtx = data->vtx;
    Gfx *tri = data->tri;
    
    // Compute interpolated points
    for (s32 i = 1; i < OMM_PERRY_CHARGE_NUM_POINTS_PER_FRAME; ++i) {
        f32 t = (f32) i / (f32) OMM_PERRY_CHARGE_NUM_POINTS_PER_FRAME;
        vec3f_interpolate3(
            data->pos[i], t,
            data->pos[0], 0.f,
            data->pos[OMM_PERRY_CHARGE_NUM_POINTS_PER_FRAME], 1.f,
            data->pos[OMM_PERRY_CHARGE_NUM_POINTS_PER_FRAME * 2], 2.f
        );
    }

    // Update timers
    o->oPerryChargeHandTimer += sign_0_s(gOmmPerryCharge - o->oPerryChargeHandTimer);
    if (gOmmPerryCharge > 0) {
        o->oPerryChargeSwordTimer = gOmmPerryCharge;
    } else if (o->oPerryChargeSwordTimer >= OMM_PERRY_CHARGE_FULL) {
        o->oPerryChargeSwordTimer = (o->oPerryChargeSwordTimer + 1) % OMM_PERRY_CHARGE_END;
    } else {
        o->oPerryChargeSwordTimer = 0;
    }
    
    // Sword glow
    Vec3f posSwordGlow = {
        lerp_f(0.5f, pos0[0], pos1[0]),
        lerp_f(0.5f, pos0[1], pos1[1]),
        lerp_f(0.5f, pos0[2], pos1[2]),
    };
    gDPLoadTextureBlock(tri++, OMM_TEXTURE_EFFECT_PERRY_CHARGE_GLOW, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0);
    if (o->oPerryChargeSwordTimer >= OMM_PERRY_CHARGE_FULL) {
        f32 t = invlerp_0_1_f(o->oPerryChargeSwordTimer, OMM_PERRY_CHARGE_FULL, OMM_PERRY_CHARGE_END);
        omm_bhv_perry_charge_draw_billboard(&vtx, &tri, pos0, posSwordGlow, 0.f, 0, 4.f * t * scale[0], 1.f - t);
    } else if (o->oPerryChargeSwordTimer >= OMM_PERRY_CHARGE_START) {
        f32 t = invlerp_0_1_f(o->oPerryChargeSwordTimer, OMM_PERRY_CHARGE_FULL, OMM_PERRY_CHARGE_START);
        omm_bhv_perry_charge_draw_billboard(&vtx, &tri, pos0, posSwordGlow, 0.f, 0, 3.f * t * scale[0], 1.f - t);
    }
    
    // Sword sparkles
    gDPLoadTextureBlock(tri++, OMM_TEXTURE_EFFECT_PERRY_CHARGE_SPARKLE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0);
    for (s32 i = OMM_PERRY_CHARGE_NUM_FRAMES; i != 0; --i) {
        f32 k = invlerp_0_1_f(i, OMM_PERRY_CHARGE_NUM_FRAMES, 1);
        s32 j = o->oPerryChargeSwordTimer - ((i - 1) * 2);
        if (j >= OMM_PERRY_CHARGE_START && j < OMM_PERRY_CHARGE_FULL) {
            f32 t = invlerp_0_1_f(j, OMM_PERRY_CHARGE_START, OMM_PERRY_CHARGE_FULL);
            Vec3f posSwordSparkle = {
                lerp_f(lerp_f(t, 0.1f, 1.0f), pos0[0], pos1[0]),
                lerp_f(lerp_f(t, 0.1f, 1.0f), pos0[1], pos1[1]),
                lerp_f(lerp_f(t, 0.1f, 1.0f), pos0[2], pos1[2]),
            };
            omm_bhv_perry_charge_draw_billboard(&vtx, &tri, pos0, posSwordSparkle, scale[2] * 20.f, random_u16(), 1.f * k * scale[0], i == 1 ? 0.8f : 0.5f);
        }
    }

    // Hand sparkles
    gDPLoadTextureBlock(tri++, OMM_TEXTURE_EFFECT_PERRY_CHARGE_SPARKLE, G_IM_FMT_RGBA,G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0);
    s32 numSparkles = clamp_s(o->oPerryChargeHandTimer - OMM_PERRY_CHARGE_FULL, 0, OMM_PERRY_CHARGE_NUM_FRAMES) * 4;
    f32 t0 = invlerp_0_1_f(o->oPerryChargeHandTimer, OMM_PERRY_CHARGE_FULL, OMM_PERRY_CHARGE_END);
    for (s32 i = numSparkles - 1; i >= 0; --i) {
        f32 t = invlerp_0_1_f(i, numSparkles, 0);
        f32 s = t0 * t * (i == 0 ? 0.8f : 0.5f);
        f32 a = t0 * t * (i == 0 ? 0.8f : 0.5f);
        Vec3f posHandSparkle = {
            data->pos[i][0],
            data->pos[i][1],
            data->pos[i][2],
        };
        omm_bhv_perry_charge_draw_billboard(&vtx, &tri, pos0, posHandSparkle, scale[2] * 10.f, random_u16(), s, a);
    }
    gSPEndDisplayList(tri);
    
    // Update object's graphics
    obj_set_pos(o, pos0[0], pos0[1], pos0[2]);
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);
}

const BehaviorScript omm_bhv_perry_charge[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_perry_charge_update,
    0x09000000,
};

//
// Spawner (auto)
//

OMM_ROUTINE_UPDATE(omm_spawn_perry_charge) {
    if (gMarioObject && OMM_PLAYER_IS_PEACH) {
        struct Object *charge = obj_get_first_with_behavior(omm_bhv_perry_charge);
        if (!charge) {
            charge = obj_spawn_from_geo(gMarioObject, omm_geo_perry_charge, omm_bhv_perry_charge);
            obj_set_always_rendered(charge, true);
            obj_set_pos(charge, 0, 0, 0);
            obj_set_angle(charge, 0, 0, 0);
            obj_set_scale(charge, 0.f, 0.f, 0.f);
            charge->oPerryChargeSwordTimer = 0;
            charge->oPerryChargeHandTimer = 0;
        }
    }
}
