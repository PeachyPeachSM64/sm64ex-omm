#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_mr_blizzard_init(struct Object *o) {
    if (o->oAction != MR_BLIZZARD_ACT_SPAWN_SNOWBALL &&
        o->oAction != MR_BLIZZARD_ACT_THROW_SNOWBALL &&
        o->oAction != MR_BLIZZARD_ACT_ROTATE &&
        o->oAction != MR_BLIZZARD_ACT_JUMP) {
        return false;
    }

    // Remove the snowball from its hand
    if (o->oMrBlizzardHeldObj) {
        obj_mark_for_deletion(o->oMrBlizzardHeldObj);
        o->oMrBlizzardHeldObj = NULL;
        o->prevObj = NULL;
    }
    o->oMrBlizzardGraphYOffset = 24.f;
    o->oFaceAngleRoll = 0;
    o->oGraphYOffset = o->oMrBlizzardGraphYOffset - 40.f * (1.f - o->oMrBlizzardScale);
    obj_scale(o, o->oMrBlizzardScale);
    gOmmData->object->state.actionState = 0;
    gOmmData->object->state.actionTimer = 0;
    return true;
}

void cappy_mr_blizzard_end(struct Object *o) {

    // Turns the Mr. Blizzard into a snowball thrower one
    o->oBehParams2ndByte = MR_BLIZZARD_STYPE_NO_CAP;
    o->oAction = 0;
    obj_anim_play(o, 0, 1.f);

    // Remove the snowball from its hand and create a new fresh one
    if (o->oMrBlizzardHeldObj) {
        obj_mark_for_deletion(o->oMrBlizzardHeldObj);
        o->oMrBlizzardHeldObj = NULL;
        o->prevObj = NULL;
    }
    o->oMrBlizzardHeldObj = spawn_object_relative(0, -70, (s16) (o->oMrBlizzardGraphYOffset + 153.f), 0, o, MODEL_WHITE_PARTICLE, bhvMrBlizzardSnowball);
    o->prevObj = o->oMrBlizzardHeldObj;
}

f32 cappy_mr_blizzard_get_top(struct Object *o) {
    return 200.f * o->oScaleY;
}

//
// Update
//

s32 cappy_mr_blizzard_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        gOmmData->object->state.actionState = 0;
        pobj_move(o, false, false, gOmmData->object->state.actionTimer != 0);
        switch (pobj_jump(o, 2.f, 1)) {
            case POBJ_RESULT_HOP_SMALL: {
                obj_play_sound(o, SOUND_OBJ_SNOW_SAND1);
                obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, 0, 8, 4, 15, 8, -3, 0.4f, 0.3f);
            } break;
            case POBJ_RESULT_HOP_LARGE: {
                o->oVelY = omm_capture_get_jump_velocity(o) * POBJ_JUMP_MULTIPLIER * (OMM_SSM_IS_NORMAL && gCurrLevelNum == LEVEL_SL ? 1.25f : 1.f);
                obj_play_sound(o, SOUND_OBJ_SNOW_SAND1);
                obj_play_sound(o, SOUND_OBJ_MR_BLIZZARD_ALERT);
                obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, 0, 8, 4, 15, 8, -3, 0.4f, 0.3f);
            } break;
        }

        // Prepare snowball
        if (POBJ_B_BUTTON_DOWN) {
            if (!o->oMrBlizzardHeldObj) {
                o->oMrBlizzardHeldObj = omm_spawn_snowball(o);
            }
            f32 scale = 1.f + min_f(gOmmData->object->state.actionTimer, 30) * 0.05f;
            o->oMrBlizzardHeldObj->oScaleX = scale;
            o->oMrBlizzardHeldObj->oScaleY = scale;
            o->oMrBlizzardHeldObj->oScaleZ = scale;
            gOmmData->object->state.actionTimer++;
        }

        // Throw snowball
        else if (gOmmData->object->state.actionTimer != 0) {
            cur_obj_play_sound_2(SOUND_OBJ2_SCUTTLEBUG_ALERT);
            o->oMrBlizzardHeldObj = NULL;
            gOmmData->object->state.actionState = 1;
            gOmmData->object->state.actionTimer = 0;
            omm_mario_lock(gMarioState, 15);
        }
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_anim_play(o, gOmmData->object->state.actionState, 1.f);
    obj_anim_clamp_frame(o, 5 + 10 * (1 - gOmmData->object->state.actionState), 127);

    // Cappy values
    gOmmData->object->cappy.offset[1] = 200.f;
    gOmmData->object->cappy.scale     = 1.2f;

    // OK
    POBJ_RETURN_OK;
}
