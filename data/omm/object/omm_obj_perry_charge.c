#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define OMM_PERRY_CHARGE_NUM_FRAMES             (8)
#define OMM_PERRY_CHARGE_BILLBOARD_SIZE         (128)
#define OMM_PERRY_CHARGE_BILLBOARD_HALF_SIZE    (OMM_PERRY_CHARGE_BILLBOARD_SIZE / 2)

static const char *sOmmPerryChargeGlowTextures[] = {
    OMM_TEXTURE_EFFECT_PERRY_CHARGE_GLOW_0,
    OMM_TEXTURE_EFFECT_PERRY_CHARGE_GLOW_1,
    OMM_TEXTURE_EFFECT_PERRY_CHARGE_GLOW_2,
};

//
// Gfx data
//

static const Gfx omm_perry_charge_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(NULL),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

//
// Geo data
//

typedef struct {
    Gfx *displayLists[1];
    Gfx gfx[array_length(omm_perry_charge_gfx)];
#if OMM_GAME_IS_SM64
    Gfx tri[(2 + OMM_PERRY_CHARGE_NUM_FRAMES) * 4 + 15];
    Vtx vtx[(2 + OMM_PERRY_CHARGE_NUM_FRAMES) * 8];
#else
    Gfx tri[(2 + OMM_PERRY_CHARGE_NUM_FRAMES) * 2 + 15];
    Vtx vtx[(2 + OMM_PERRY_CHARGE_NUM_FRAMES) * 4];
#endif
} OmmPerryChargeGeoData;

static const u32 sOmmPerryChargeGeoDataDisplayListsOffsets[] = {
    offsetof(OmmPerryChargeGeoData, gfx),
};

//
// Geo layouts
//

static void omm_geo_perry_charge_draw_billboard(Vtx **vtx, Gfx **tri, Vec3f billboardPos, Vec3f objPos, f32 camOffset, s16 angle, f32 scale, f32 opacity) {
    *tri = gfx_create_billboard(*tri, *vtx,
        -OMM_PERRY_CHARGE_BILLBOARD_HALF_SIZE * scale,
        -OMM_PERRY_CHARGE_BILLBOARD_HALF_SIZE * scale,
        OMM_PERRY_CHARGE_BILLBOARD_SIZE * scale,
        OMM_PERRY_CHARGE_BILLBOARD_SIZE * scale,
        0, 0, 128, 128,
        billboardPos, objPos,
        camOffset, angle,
        0xFF, 0xFF, 0xFF, 0xFF * opacity
    );
    *vtx += 4;
#if OMM_GAME_IS_SM64
    if (gOmmGlobals->isMirrorRoom) {
        billboardPos[0] = 2 * gOmmGlobals->mirrorX - billboardPos[0];
        *tri = gfx_create_billboard(*tri, *vtx,
            -OMM_PERRY_CHARGE_BILLBOARD_HALF_SIZE * scale,
            -OMM_PERRY_CHARGE_BILLBOARD_HALF_SIZE * scale,
            OMM_PERRY_CHARGE_BILLBOARD_SIZE * scale,
            OMM_PERRY_CHARGE_BILLBOARD_SIZE * scale,
            0, 0, 128, 128,
            billboardPos, objPos,
            camOffset, angle,
            0xFF, 0xFF, 0xFF, 0xFF * opacity
        );
        *vtx += 4;
    }
#endif
}

static Gfx *omm_geo_perry_charge_update_gfx(s32 callContext, UNUSED struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER && gCurrGraphNodeObject && gMarioObject) {
        struct Object *o = gCurrGraphNodeObject;
        struct Object *p = omm_perry_get_object();
        OmmPerryChargeGeoData *data = o->oGeoData;
        if (data && p) {
            Vtx *vtx = data->vtx;
            Gfx *tri = data->tri;
            u16 r = srandom_u16(o->oTimer);

            // Compute Perry values
            Vec3f posPerryHandle = { p->oPosX, p->oPosY, p->oPosZ };
            Vec3f posPerryTip;
            Vec3f scale;
            if (p->oScaleX > 0.f) {
                Vec3f delta = { 0.f, 110.f, 0.f };
                vec3f_rotate_zxy(delta, delta, p->oFaceAnglePitch, p->oFaceAngleYaw, p->oFaceAngleRoll);
                vec3f_mult(delta, delta, p->oGfxScale);
                vec3f_sum(posPerryTip, posPerryHandle, delta);
                vec3f_copy(scale, p->oGfxScale);
            } else {
                vec3f_copy(posPerryTip, posPerryHandle);
                vec3f_copy(scale, gMarioObject->oGfxScale);
            }

            // Sword glow
            Vec3f posSwordGlow = {
                lerp_f(0.5f, posPerryHandle[0], posPerryTip[0]),
                lerp_f(0.5f, posPerryHandle[1], posPerryTip[1]),
                lerp_f(0.5f, posPerryHandle[2], posPerryTip[2]),
            };
            const char *texture = sOmmPerryChargeGlowTextures[(r = srandom_u16(r)) % array_length(sOmmPerryChargeGlowTextures)];
            gDPLoadTextureBlock(tri++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0);
            if (o->oPerryChargeSwordTimer >= OMM_PERRY_CHARGE_FULL) {
                f32 t = invlerp_0_1_f(o->oPerryChargeSwordTimer, OMM_PERRY_CHARGE_FULL, OMM_PERRY_CHARGE_END);
                omm_geo_perry_charge_draw_billboard(&vtx, &tri, posSwordGlow, posPerryHandle, 0.f, 0, 4.f * t * scale[0], 1.f - t);
            } else if (o->oPerryChargeSwordTimer >= OMM_PERRY_CHARGE_START) {
                f32 t = invlerp_0_1_f(o->oPerryChargeSwordTimer, OMM_PERRY_CHARGE_FULL, OMM_PERRY_CHARGE_START);
                omm_geo_perry_charge_draw_billboard(&vtx, &tri, posSwordGlow, posPerryHandle, 0.f, 0, 3.f * t * scale[0], 1.f - t);
            }

            // Sword sparkles
            gDPLoadTextureBlock(tri++, OMM_TEXTURE_EFFECT_PERRY_CHARGE_SPARKLE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 128, 128, 0, 0, 0, 0, 0, 0, 0);
            for (s32 i = OMM_PERRY_CHARGE_NUM_FRAMES; i != 0; --i) {
                f32 k = invlerp_0_1_f(i, OMM_PERRY_CHARGE_NUM_FRAMES, 1);
                s32 j = o->oPerryChargeSwordTimer - ((i - 1) * 2);
                if (j >= OMM_PERRY_CHARGE_START && j < OMM_PERRY_CHARGE_FULL) {
                    f32 t = invlerp_0_1_f(j, OMM_PERRY_CHARGE_START, OMM_PERRY_CHARGE_FULL);
                    Vec3f posSwordSparkle = {
                        lerp_f(lerp_f(t, 0.1f, 1.0f), posPerryHandle[0], posPerryTip[0]),
                        lerp_f(lerp_f(t, 0.1f, 1.0f), posPerryHandle[1], posPerryTip[1]),
                        lerp_f(lerp_f(t, 0.1f, 1.0f), posPerryHandle[2], posPerryTip[2]),
                    };
                    omm_geo_perry_charge_draw_billboard(&vtx, &tri, posSwordSparkle, posPerryHandle, scale[2] * 20.f, (r = srandom_u16(r)), 1.f * k * scale[0], i == 1 ? 0.8f : 0.5f);
                }
            }

            // Hand sparkle
            f32 t0 = invlerp_0_1_f(o->oPerryChargeHandTimer, OMM_PERRY_CHARGE_FULL, OMM_PERRY_CHARGE_END);
            if (t0 > 0.f) {
                Vec3f posHandSparkle = {
                    posPerryHandle[0],
                    posPerryHandle[1],
                    posPerryHandle[2],
                };
                omm_geo_perry_charge_draw_billboard(&vtx, &tri, posHandSparkle, posPerryHandle, scale[2] * 10.f, (r = srandom_u16(r)), t0 * 0.9f, t0 * 0.8f);
            }
            gSPEndDisplayList(tri);
        }
    }
    return NULL;
}

const GeoLayout omm_geo_perry_charge[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(0, omm_geo_perry_charge_update_gfx),
#if OMM_GAME_IS_SM64
        GEO_ASM(0, geo_link_geo_data_skip_mirror_obj),
#else
        GEO_ASM(0, geo_link_geo_data),
#endif
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, NULL),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_perry_charge_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = omm_perry_get_object();
    if (!p || !OMM_PERRY_IS_AVAILABLE) {
        obj_mark_for_deletion(o);
        return;
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

    // Update gfx
    // omm_geo_perry_charge_update_gfx
    OmmPerryChargeGeoData *data = geo_get_geo_data(o,
        sizeof(OmmPerryChargeGeoData),
        sOmmPerryChargeGeoDataDisplayListsOffsets,
        array_length(sOmmPerryChargeGeoDataDisplayListsOffsets)
    );
    gfx_copy_and_fill_null(data->gfx, omm_perry_charge_gfx, sizeof(omm_perry_charge_gfx), data->tri);
    gSPEndDisplayList(data->tri);

    // Update object's graphics
    obj_copy_pos(o, p);
    obj_set_angle(o, 0, 0, 0);
    obj_set_scale(o, 1.f, 1.f, 1.f);
}

const BehaviorScript bhvOmmPerryCharge[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_perry_charge_update),
    BHV_END_LOOP(),
};

//
// Spawner (auto)
//

OMM_ROUTINE_UPDATE(omm_obj_spawn_perry_charge) {
    if (gMarioObject && OMM_PERRY_IS_AVAILABLE) {
        struct Object *charge = obj_get_first_with_behavior(bhvOmmPerryCharge);
        if (!charge) {
            charge = obj_spawn_from_geo(gMarioObject, omm_geo_perry_charge, bhvOmmPerryCharge);
            obj_set_always_rendered(charge, true);
            obj_set_xyz(charge, 0, 0, 0);
            obj_set_angle(charge, 0, 0, 0);
            obj_set_scale(charge, 0.f, 0.f, 0.f);
            charge->oPerryChargeSwordTimer = 0;
            charge->oPerryChargeHandTimer = 0;
        }
    }
}
