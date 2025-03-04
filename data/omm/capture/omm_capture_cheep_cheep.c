#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_cheep_cheep_init(struct Object *o) {
    if (o->oAction == 0 || !obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ))) {
        return false;
    }

    return true;
}

void omm_cappy_cheep_cheep_end(struct Object *o) {
    struct Object *whirlpool = obj_get_first_with_behavior_and_parent(o, bhvOmmWhirlpool);
    if (whirlpool) {
        obj_mark_for_deletion(whirlpool);
    }
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    o->oAction = 0;
    o->oTimer = 0;
}

u64 omm_cappy_cheep_cheep_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_CHEEP_CHEEP;
}

f32 omm_cappy_cheep_cheep_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_cheep_cheep_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_UNDER_WATER;
    POBJ_SET_FLOATING;
    POBJ_SET_ABLE_TO_OPEN_DOORS;
    POBJ_SET_ATTACKING * (--gOmmObject->state.actionTimer > 0);

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        f32 vel = pobj_get_walk_speed(o);
        f32 inc = vel / 8.f;

        // Spin attack
        if (gOmmObject->state.actionTimer <= 0 && ((POBJ_A_BUTTON_PRESSED && POBJ_B_BUTTON_PRESSED) || (POBJ_A_BUTTON_PRESSED && POBJ_B_BUTTON_DOWN) || (POBJ_B_BUTTON_PRESSED && POBJ_A_BUTTON_DOWN))) {
            if (vec3f_length(&o->oVelX) < 1.f) {
                obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, vel);
            }
            obj_play_sound(o, POBJ_SOUND_FLYING_FAST);
            gOmmObject->state.actionTimer = 16;
        }

        // Go up
        else if (POBJ_A_BUTTON_DOWN && !POBJ_B_BUTTON_DOWN) {
            o->oVelY = approach_f32(o->oVelY, +vel, inc, inc);
        }

        // Go down
        else if (POBJ_B_BUTTON_DOWN && !POBJ_A_BUTTON_DOWN) {
            o->oVelY = approach_f32(o->oVelY, -vel, inc, inc);
        }

        // Reset velocity
        else {
            o->oVelY = approach_f32(o->oVelY, 0.f, inc, inc);
        }
    }

    // Friction
    bool spinAttack = (gOmmObject->state.actionTimer > 0);
    if (spinAttack) {
        vec3f_set(gOmmObject->state.friction, 1.f, 1.f, 1.f);
    }

    // Movement
    f32 velMag = (spinAttack ? pobj_get_dash_speed(o) : min_f(vec3f_length(&o->oVelX), pobj_get_walk_speed(o)));
    vec3f_set_mag(&o->oVelX, velMag);
    perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_UPDATE_PREV_POS | OBJ_STEP_STICKY_FEET | OBJ_STEP_KEEP_UNDERWATER);
    pobj_decelerate(o);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Attract coins during spin
    if (spinAttack) {
        omm_obj_process_interactions(o, POBJ_INT_PRESET_CHEEP_CHEEP_SPIN);
    }

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    f32 distInc = vec3f_length(&o->oVelX);
    f32 distMin = pobj_get_walk_speed(o) * 12.f;
    f32 bubbleDistance = fmodf(gOmmObject->state.walkDistance, distMin / 4.f);
    obj_make_step_sound_and_particle(o, &bubbleDistance, distMin / 4.f, distInc, 0, OBJ_PARTICLE_BUBBLE);
    obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, distMin, distInc, POBJ_SOUND_SWIMMING, 0);

    // Spin attack gfx
    struct Object *whirlpool = obj_get_first_with_behavior_and_parent(o, bhvOmmWhirlpool);
    if (spinAttack) {
        if (!whirlpool) {
            whirlpool = omm_obj_spawn_whirlpool(o);
        }
        Vec3f v = { 0, 0, o->hitboxRadius * 1.5f };
        Vec3f t = { o->oPosX, o->oPosY + o->hitboxHeight / 2.f, o->oPosZ };
        Vec3s r = { o->oFaceAnglePitch, o->oFaceAngleYaw, 0 };
        Vec3f s = { o->oScaleX, o->oScaleY, o->oScaleZ };
        vec3f_transform(&whirlpool->oPosX, v, t, r, s);
        whirlpool->oFaceAnglePitch = -o->oFaceAnglePitch;
        whirlpool->oFaceAngleYaw = o->oFaceAngleYaw + 0x8000;
        whirlpool->oAngleVelRoll = -0x2000;
        obj_scale(whirlpool, 0.2f * o->oScaleX);
        spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvPlungeBubble);
    } else if (whirlpool) {
        obj_mark_for_deletion(whirlpool);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_cheep_cheep_update_gfx(struct Object *o) {

    // Gfx
    f32 dist; s16 pitch, yaw, roll;
    roll = max_s(0, gOmmObject->state.actionTimer) * -0x2000;
    vec3f_get_dist_and_angle(gVec3fZero, &o->oVelX, &dist, &pitch, &yaw);
    if (vec3f_hdist(gVec3fZero, &o->oVelX) < 1.f) {
        yaw = o->oFaceAngleYaw;
    }
    obj_set_angle(o, approach_s16_symmetric(o->oFaceAnglePitch, -pitch, 0x800), yaw, roll);
    obj_update_gfx(o);

    // Cappy transform
    gOmmObject->cappy.object = o;
}
