#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define OMM_MONEYBAG_COIN_NUM_COINS         (blueCoin ? 5 : 1)
#define OMM_MONEYBAG_COIN_NUM_PARTICLES     (blueCoin ? 15 : 5)
#define OMM_MONEYBAG_COIN_HITBOX_RADIUS     (blueCoin ? 100 : 60)
#define OMM_MONEYBAG_COIN_HITBOX_HEIGHT     (blueCoin ? 100 : 60)
#define OMM_MONEYBAG_COIN_HITBOX_WALL       (blueCoin ? 50 : 30)
#define OMM_MONEYBAG_COIN_FORWARD_VEL       (blueCoin ? 90 : 60)
#define OMM_MONEYBAG_COIN_SPARKLE_COLOR     (blueCoin ? 0x78 : 0xFF), (blueCoin ? 0x78 : 0xFF), (blueCoin ? 0xFF : 0x40)
#define OMM_MONEYBAG_COIN_SPARKLE_SCALE     (blueCoin ? 0.75f : 0.45f), (blueCoin ? 1.5f : 0.9f)

//
// Behavior
//

static void bhv_omm_moneybag_coin_delete(struct Object *o, bool spawnCoins) {
    if (spawnCoins) {
        bool blueCoin = o->oAction == 1;
        o->oNumLootCoins = OMM_MONEYBAG_COIN_NUM_COINS;
        obj_spawn_loot_yellow_coins(o, OMM_MONEYBAG_COIN_NUM_COINS, 0);
        obj_spawn_triangle_break_particles(o, OMM_MONEYBAG_COIN_NUM_PARTICLES, MODEL_YELLOW_COIN_NO_SHADOW, 0.75f, 0);
    } else {
        obj_spawn_white_puff(o, SOUND_OBJ_DEFAULT_DEATH);
    }
    obj_mark_for_deletion(o);
}

static void bhv_omm_moneybag_coin_loop() {
    struct Object *o = gCurrentObject;
    bool blueCoin = o->oAction == 1;
    obj_reset_hitbox(o, OMM_MONEYBAG_COIN_HITBOX_RADIUS, OMM_MONEYBAG_COIN_HITBOX_HEIGHT, 0, 0, OMM_MONEYBAG_COIN_HITBOX_WALL, 0);
    s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);

    // Out of time/bounds
    if (o->oTimer > 600 || check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS)) {
        bhv_omm_moneybag_coin_delete(o, false);
        return;
    }

    // Collided with a wall (or a slanted floor)
    if (check_object_step(o, step, OBJECT_STEP_CHECK_HIT_WALL)) {
        obj_bounce_on_wall(o, o->oWall ? o->oWall : o->oFloor, false, false);
        obj_play_sound(o, SOUND_GENERAL_COIN_DROP);
    }

    // Collided with a ceiling
    if (check_object_step(o, step, OBJECT_STEP_CHECK_HIT_CEILING) && o->oCeil && o->oCeil->normal.y > -0.9f) {
        obj_bounce_on_wall(o, o->oCeil, false, false);
        obj_play_sound(o, SOUND_GENERAL_COIN_DROP);
    }

    // Update
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, OMM_MONEYBAG_COIN_HITBOX_RADIUS, OMM_MONEYBAG_COIN_HITBOX_HEIGHT * 1.5f, 0, 0, OMM_MONEYBAG_COIN_HITBOX_WALL, OMM_MONEYBAG_COIN_HITBOX_HEIGHT / 4.f);
    struct Object *interacted = omm_obj_process_interactions(o, obj_get_interaction_flags_from_power(blueCoin, OBJ_INT_PRESET_MONEYBAG_COIN));
    if (interacted && !omm_obj_is_collectible(interacted)) {
        bhv_omm_moneybag_coin_delete(o, !(interacted->oFlags & OBJ_FLAG_MONEYBAG_COIN_INTERACTED));
        interacted->oFlags |= OBJ_FLAG_MONEYBAG_COIN_INTERACTED;
        return;
    }

    // Spawn sparkles
    omm_obj_spawn_sparkle(o, OMM_MONEYBAG_COIN_SPARKLE_COLOR, OMM_MONEYBAG_COIN_HITBOX_RADIUS / 3.f, OMM_MONEYBAG_COIN_SPARKLE_SCALE);
}

const BehaviorScript bhvOmmMoneybagCoin[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BILLBOARD(),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_moneybag_coin_loop),
        BHV_ADD_INT(oAnimState, 2), // Make the 2D coin spin faster
        BHV_ADD_INT(oTimer, 3), // Make the 3D coin rotate faster
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_moneybag_coin(struct Object *o, f32 power) {
    struct MarioState *m = gMarioState;
    if (m->numCoins > 0) {
        bool blueCoin = (power == 1.f && m->numCoins >= 5);
        struct Object *coin = obj_spawn_from_geo(o, blueCoin ? blue_coin_geo : yellow_coin_geo, bhvOmmMoneybagCoin);
        coin->oPosX += o->hitboxRadius * 0.25f * sins(o->oMoveAngleYaw);
        coin->oPosY += o->hitboxHeight * 0.75f - o->oScaleY * OMM_MONEYBAG_COIN_HITBOX_HEIGHT / 2.f;
        coin->oPosZ += o->hitboxRadius * 0.25f * coss(o->oMoveAngleYaw);
        obj_set_throw_vel(coin, o, OMM_MONEYBAG_COIN_FORWARD_VEL * o->oScaleX, 0.f);
        obj_scale(coin, o->oScaleY);
        coin->oAction = blueCoin;
        obj_play_sound(o, SOUND_GENERAL_COIN_SPURT_2);
        obj_set_interact_id(o, coin);
        gHudDisplay.coins = m->numCoins = m->numCoins - OMM_MONEYBAG_COIN_NUM_COINS;
        return coin;
    }
    return NULL;
}
