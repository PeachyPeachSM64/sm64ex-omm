#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

static void bhv_omm_spiny_ball_delete(struct Object *o) {
    obj_spawn_white_puff(o, SOUND_OBJ_DEFAULT_DEATH);
    obj_mark_for_deletion(o);
}

static void bhv_omm_spiny_ball_update() {
    struct Object *o = gCurrentObject;

    // In Lakitu's hand
    if (o->parentObj) {

        // Release if no longer in Lakitu's hand
        if (!o->parentObj->oHeldProjectile) {
            obj_set_throw_vel(o, o->parentObj, 30.f, 30.f);
            obj_scale(o, o->parentObj->oScaleY);
            o->parentObj = NULL;
        }

        // Follow parent's hand
        else {
            s16 da = o->parentObj->oFaceAngleYaw - 0x6000;
            f32 dh =  50.f * o->parentObj->oScaleX;
            f32 dv = 110.f * o->parentObj->oScaleY;
            o->oPosX = o->parentObj->oPosX + dh * sins(da);
            o->oPosY = o->parentObj->oPosY + dv;
            o->oPosZ = o->parentObj->oPosZ + dh * coss(da);
            o->oFaceAngleYaw = o->parentObj->oFaceAngleYaw;
        }
    }

    // Released
    else {
        s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);
        o->oVelY -= 4.f;

        // Out of bounds, or collided with a wall/ceiling/floor
        if (check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS | OBJECT_STEP_CHECK_HIT_WALL | OBJECT_STEP_CHECK_HIT_FLOOR_WITH_NEGATIVE_VEL | OBJECT_STEP_CHECK_HIT_SLANTED_FLOOR | OBJECT_STEP_CHECK_HIT_SLANTED_CEILING)) {
            bhv_omm_spiny_ball_delete(o);
            return;
        }
    }

    // Update
    obj_set_params(o, 0, 0, 0, 0, !o->parentObj);
    obj_reset_hitbox(o, 60, 100, 0, 0, 30, 40);
    if (!o->parentObj) {
        struct Object *interacted = omm_obj_process_interactions(o, obj_get_interaction_flags_from_power(o->oObjectPower, OBJ_INT_PRESET_SPINY_BALL));
        if (interacted && !omm_obj_is_collectible(interacted)) {
            bhv_omm_spiny_ball_delete(o);
        }
    }

    // Gfx
    vec3f_copy(o->oGfxPos, &o->oPosX);
    o->oGfxAngle[0] -= 0x2000 * !o->parentObj;
    o->oGfxAngle[1] = o->oFaceAngleYaw;
    o->oGfxAngle[2] = 0;
}

const BehaviorScript bhvOmmSpinyBall[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_spiny_ball_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_spiny_ball(struct Object *o) {
    struct Object *ball = obj_spawn_from_geo(o, spiny_ball_geo, bhvOmmSpinyBall);
    obj_init_animation_with_sound(ball, spiny_egg_seg5_anims_050157E4, 0);
    ball->parentObj = o;
    obj_set_interact_id(o, ball);
    return ball;
}
