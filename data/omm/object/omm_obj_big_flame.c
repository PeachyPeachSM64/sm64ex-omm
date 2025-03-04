#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layouts
//

const GeoLayout omm_geo_big_flame[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(red_flame_geo),
};

const GeoLayout omm_geo_big_flame_2[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(blue_flame_geo),
};

//
// Behaviors
//

static void bhv_omm_big_flame_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    obj_set_scale(o, 20, 20, 20);
    obj_set_params(o, INTERACT_FLAME, 0, 0, 0, true);
    obj_reset_hitbox(o, 8, 16, 0, 0, 0, 8);
    if (omm_mario_is_burning(m) && m->interactObj == o) {
        if (OMM_SPARKLY_MODE_IS_ENABLED) play_sound(SOUND_GENERAL2_RIGHT_ANSWER, gGlobalSoundArgs);
        obj_spawn_white_puff(o, SOUND_GENERAL_FLAME_OUT);
        obj_mark_for_deletion(o);
    } else {
        o->oAnimState++;
    }
}

const BehaviorScript bhvOmmBigFlame[] = {
    OBJ_TYPE_LEVEL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_big_flame_update),
    BHV_END_LOOP(),
};

static void bhv_omm_big_flame_2_update() {
    struct Object *o = gCurrentObject;
    if (o->parentObj && o->parentObj->activeFlags && o->parentObj->behavior == bhvOmmBigFlame) {
        obj_set_scale(o, 0, 0, 0);
    } else {
        obj_set_scale(o, 5, 5, 5);
        o->parentObj = NULL;
    }
    obj_set_params(o, 0, 0, 0, 0, false);
    obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    o->oAnimState++;
}

const BehaviorScript bhvOmmBigFlame2[] = {
    OBJ_TYPE_DEFAULT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_big_flame_2_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_big_flame(struct Object *o, f32 x, f32 y, f32 z) {
    struct Object *flame = obj_spawn_from_geo(o, omm_geo_big_flame, bhvOmmBigFlame);
    obj_set_xyz(flame, x, y, z);
    obj_set_scale(flame, 0, 0, 0);
    obj_spawn_from_geo(flame, omm_geo_big_flame_2, bhvOmmBigFlame2);
    return flame;
}
