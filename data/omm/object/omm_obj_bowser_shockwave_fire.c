#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define OMM_SHOCKWAVE_FIRE_MAX_NUM_POINTS 256

//
// Gfx data
//

static const Gfx omm_bowser_shockwave_fire_wave_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_RED_1, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(NULL),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static const Gfx omm_bowser_shockwave_fire_fire_gfx[] = {
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_BOWSER_FIRE_RED_2, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1024, 1024, 0, 0, 0, 0, 0, 0, 0),
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
    Gfx *displayLists[2];
    struct {
        Gfx gfx[array_length(omm_bowser_shockwave_fire_wave_gfx)];
        Gfx tri[OMM_SHOCKWAVE_FIRE_MAX_NUM_POINTS * 2 + 1];
        Vtx vtx[(OMM_SHOCKWAVE_FIRE_MAX_NUM_POINTS + 1) * 2];
    } wave, fire;
} OmmBowserShockwaveFireGeoData;

static const u32 sOmmBowserShockwaveFireGeoDataDisplayListsOffsets[] = {
    offsetof(OmmBowserShockwaveFireGeoData, wave.gfx),
    offsetof(OmmBowserShockwaveFireGeoData, fire.gfx),
};

//
// Geo layout
//

const GeoLayout omm_geo_bowser_shockwave_fire[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_ASM(0, geo_link_geo_data),
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_shockwave_fire_wave_gfx),
        GEO_ASM(1, geo_link_geo_data),
        GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_bowser_shockwave_fire_fire_gfx),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static bool is_mario_in_range(struct Object *o, struct MarioState *m) {

    // Lateral distance
    f32 marioRadius = (omm_mario_is_capture(m) ? omm_capture_get_wall_hitbox_radius(gOmmCapture) : (50.f * m->marioObj->oScaleX));
    f32 waveRadius = (o->oBowserShockwaveFireRadius * o->oScaleX);
    f32 waveWidth = (o->oBowserShockwaveFireWidth / 2.5f) + marioRadius;
    f32 distMin = waveRadius - waveWidth;
    f32 distMax = waveRadius + waveWidth;
    f32 distToMario = sqrtf(sqr_f(m->pos[0] - o->oPosX) + sqr_f(m->pos[2] - o->oPosZ));
    if (distMin > distToMario ||
        distMax < distToMario) {
        return false;
    }

    // Height check
    f32 yMin = o->oPosY;
    f32 yMax = o->oPosY + o->oBowserShockwaveFireHeight;
    f32 yMarioMin = m->pos[1];
    f32 yMarioMax = m->pos[1] + (m->marioObj->hitboxHeight * m->marioObj->oScaleY);
    f32 sumOfHeights = o->oBowserShockwaveFireHeight + m->marioObj->hitboxHeight;
    if ((yMarioMax - yMin) > sumOfHeights ||
        (yMax - yMarioMin) > sumOfHeights) {
        return false;
    }

    // Mario's butt is about to burn
    return true;
}

static void bhv_omm_bowser_shockwave_fire_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer > o->oBowserShockwaveFireDuration) {
        obj_mark_for_deletion(o);
        return;
    }

    // Update scale
    f32 scale = (o->oBowserShockwaveFireSpeed / o->oBowserShockwaveFireRadius) * o->oTimer;
    o->oScaleX = scale;
    o->oScaleY = 1.f;
    o->oScaleZ = scale;

    // Update gfx
    OmmBowserShockwaveFireGeoData *data = geo_get_geo_data(o,
        sizeof(OmmBowserShockwaveFireGeoData),
        sOmmBowserShockwaveFireGeoDataDisplayListsOffsets,
        array_length(sOmmBowserShockwaveFireGeoDataDisplayListsOffsets)
    );
    f32 radius    = o->oBowserShockwaveFireRadius;
    f32 width     = o->oBowserShockwaveFireWidth * 1.60f / max_f(o->oScaleX, 0.01f);
    f32 height    = o->oBowserShockwaveFireHeight * 1.25f;
    f32 radiusIn  = max_f(0, (radius - width / 2));
    f32 radiusOut = max_f(0, (radius + width / 2));
    s32 frame     = (o->oTimer % 20);
    f32 opacity   = clamp_0_1_f((o->oBowserShockwaveFireDuration - o->oTimer) / 15.f);
    s16 texU      = (frame % 4) * 240;
    s16 texV      = (frame / 4) * 160;
    s32 numPoints = ((clamp_s((s32) ((2.f * M_PI * radius * o->oScaleX) / 200.f), 8, OMM_SHOCKWAVE_FIRE_MAX_NUM_POINTS) / 2) * 2); // Must be even
    Vtx *waveVtx  = data->wave.vtx;
    Gfx *waveTri  = data->wave.tri;
    Vtx *fireVtx  = data->fire.vtx;
    Gfx *fireTri  = data->fire.tri;
    for (s32 i = 0; i <= numPoints; ++i) {
        u16 angle = (i * 0x10000) / numPoints;

        // Triangles
        if (i < numPoints) {
            gSPVertex(waveTri++, waveVtx, 4, 0);
            gSP2Triangles(waveTri++, 0, 2, 1, 0, 2, 3, 1, 0);
            gSPVertex(fireTri++, fireVtx, 4, 0);
            gSP2Triangles(fireTri++, 0, 2, 1, 0, 2, 3, 1, 0);
        }

        // Wave vertices
        *(waveVtx++) = (Vtx) { { { radiusIn  * sins(angle), 0, radiusIn  * coss(angle) }, 0, { 0x2000 * (i % 2), 0x0800 }, { 0xFF, 0xFF, 0xFF, 0xFF * opacity } } };
        *(waveVtx++) = (Vtx) { { { radiusOut * sins(angle), 0, radiusOut * coss(angle) }, 0, { 0x2000 * (i % 2), 0x1800 }, { 0xFF, 0xFF, 0xFF, 0xFF * opacity } } };

        // Fire vertices
        s32 du = (i % 4 != 0) * (i % 4 == 2 ? 239 : 120);
        *(fireVtx++) = (Vtx) { { { radius * sins(angle), 0,      radius * coss(angle) }, 0, { (texU + du) << 5, (texV + 160) << 5 }, { 0xFF, 0xFF, 0xFF, 0xFF * opacity } } };
        *(fireVtx++) = (Vtx) { { { radius * sins(angle), height, radius * coss(angle) }, 0, { (texU + du) << 5, (texV +   0) << 5 }, { 0xFF, 0xFF, 0xFF, 0xFF * opacity } } };
    }
    gSPEndDisplayList(waveTri++);
    gSPEndDisplayList(fireTri++);
    gfx_copy_and_fill_null(data->wave.gfx, omm_bowser_shockwave_fire_wave_gfx, sizeof(omm_bowser_shockwave_fire_wave_gfx), data->wave.tri);
    gfx_copy_and_fill_null(data->fire.gfx, omm_bowser_shockwave_fire_fire_gfx, sizeof(omm_bowser_shockwave_fire_fire_gfx), data->fire.tri);
    gDPSetTextureImage(data->wave.gfx + 4, G_IM_FMT_RGBA, G_IM_SIZ_32b_LOAD_BLOCK, 1, o->oBowserShockwaveFireTextureWave);
    gDPSetTextureImage(data->fire.gfx + 4, G_IM_FMT_RGBA, G_IM_SIZ_32b_LOAD_BLOCK, 1, o->oBowserShockwaveFireTextureFire);

    // Hitbox
    if (is_mario_in_range(o, gMarioState)) {
        f32 width = o->oBowserShockwaveFireWidth / max_f(o->oScaleX, 0.01f);
        obj_set_params(o, INTERACT_FLAME, 1, 0, 0, true);
        obj_reset_hitbox(o, o->oBowserShockwaveFireRadius + width, o->oBowserShockwaveFireHeight, 0, 0, 0, 0);
    } else {
        obj_set_params(o, 0, 0, 0, 0, false);
        obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    }
}

const BehaviorScript bhvOmmBowserShockwaveFire[] = {
    OBJ_TYPE_GENACTOR,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_bowser_shockwave_fire_update),
        BHV_ADD_INT(oAnimState, 1),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_bowser_shockwave_fire(struct Object *o, f32 radius, f32 width, f32 height, f32 speed, f32 distMax, const void *textureWave, const void *textureFire) {
    struct Object *fire = obj_spawn_from_geo(o, omm_geo_bowser_shockwave_fire, bhvOmmBowserShockwaveFire);
    obj_set_always_rendered(fire, true);
    obj_set_xyz(fire, o->oPosX, o->oPosY + 10, o->oPosZ);
    obj_set_angle(fire, 0, 0, 0);
    obj_scale(fire, 0);
    fire->oOpacity = 0xFF;
    fire->oBowserShockwaveFireRadius = abs_f(radius);
    fire->oBowserShockwaveFireWidth = abs_f(width);
    fire->oBowserShockwaveFireHeight = abs_f(height);
    fire->oBowserShockwaveFireSpeed = abs_f(speed);
    fire->oBowserShockwaveFireDuration = abs_s(distMax / max_f(speed, 1.f));
    fire->oBowserShockwaveFireTextureWave = textureWave;
    fire->oBowserShockwaveFireTextureFire = textureFire;
    return fire;
}
