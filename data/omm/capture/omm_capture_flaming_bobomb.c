#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_cappy_flaming_bobomb_explode(struct Object *o, bool deleteObj) {
    omm_obj_spawn_flaming_bobomb_explosion(o);

    // Final boom
    if (deleteObj) {
        obj_drop_to_floor(o);
        omm_obj_spawn_bowser_shockwave_fire(o, 100, 80, 120, 40, 5000, OMM_TEXTURE_BOWSER_FIRE_BLUE_1, OMM_TEXTURE_BOWSER_FIRE_BLUE_2);
        obj_mark_for_deletion(o->oFlamingBobombAura);
        obj_mark_for_deletion(o);
    }

    // Bowser
    struct Object *bowser = obj_get_first_with_behavior(bhvOmmBowser);
    if (bowser->oHealth > 0 && obj_get_horizontal_distance(o, bowser) < (bowser->hitboxRadius + 600.f)) {
        bowser->oBowserHit = 1;
    }
}

//
// Init
//

bool omm_cappy_flaming_bobomb_init(struct Object* o) {
    if (o->oAction != 1 &&
        o->oAction != 3 &&
        o->oAction != 5) {
        return false;
    }

    gOmmObject->flaming_bobomb.interactedTimer = 0;
    gOmmObject->flaming_bobomb.interactedFire = false;
    gOmmObject->flaming_bobomb.captureDuringAscent = (o->oAction == 1);
    return true;
}

void omm_cappy_flaming_bobomb_end(struct Object *o) {
    omm_cappy_flaming_bobomb_explode(o, true);
}

u64 omm_cappy_flaming_bobomb_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_FLAMING_BOBOMB;
}

f32 omm_cappy_flaming_bobomb_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_flaming_bobomb_update(struct Object *o) {

    // Hitbox
    o->oScaleX = 1.2f;
    o->oScaleY = 1.2f;
    o->oScaleZ = 1.2f;
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        pobj_jump(o, 1);

        // Explosion
        if (POBJ_B_BUTTON_PRESSED && (gOmmObject->state.actionTimer == 0)) {
            gOmmObject->state.actionTimer = 30;
            gOmmObject->state.actionState++;

            // Five charges
            if (gOmmObject->state.actionState == 5) {
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 0);
            } else {
                omm_cappy_flaming_bobomb_explode(o, false);
            }
        } else {
            gOmmObject->state.actionTimer = max_s(0, gOmmObject->state.actionTimer - 1);
        }
    }
    pobj_stop_if_unpossessed();

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions(

    // Do not process interactions during recovery time
    if (gOmmObject->flaming_bobomb.interactedTimer != 0) {
        break;
    }

    // The flaming bob-omb can withstand one fire attack before exploding and releasing Mario
    if ((obj->oInteractType & INTERACT_FLAME) &&
        !gOmmObject->flaming_bobomb.interactedFire &&
        obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HURTBOX_HITBOX_IF_ZERO)) {

        // Mario gets damaged, but keeps his capture
        // Then, stays invincible for 2 seconds
        gMarioState->hurtCounter = 12;
        gOmmObject->flaming_bobomb.interactedFire = !OMM_CHEAT_GOD_MODE;
        gOmmObject->flaming_bobomb.interactedTimer = 60;
        obj->oInteractStatus |= INT_STATUS_INTERACTED;

        // Gfx/Sfx
        omm_obj_spawn_flaming_bobomb_explosion(o);
    }

    );
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    obj_anim_play(o, 0, max_f(1.f, (o->oVelY <= 0.f) * POBJ_ABS_FORWARD_VEL * 2.f / pobj_get_walk_speed(o)));

    // OK
    gOmmObject->flaming_bobomb.interactedTimer = max_s(0, gOmmObject->flaming_bobomb.interactedTimer - 1);
    pobj_return_ok;
}

void omm_cappy_flaming_bobomb_update_gfx(struct Object *o) {
    bool damaged = gOmmObject->flaming_bobomb.interactedTimer != 0;

    // Gfx
    o->oFaceAnglePitch = 0;
    o->oFaceAngleRoll = 0;
    o->oMoveAnglePitch = 0;
    o->oMoveAngleRoll = 0;
    o->oGraphYOffset = 0;
    obj_update_gfx(o);
    obj_random_blink(o, &o->oBobombBlinkTimer);
    if (damaged && (gGlobalTimer & 1)) {
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    } else {
        o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    }

    // Aura gfx
    vec3f_copy(&o->oFlamingBobombAura->oPosX, &o->oPosX);
    vec3f_copy(&o->oFlamingBobombAura->oScaleX, &o->oScaleX);
    vec3f_mul(&o->oFlamingBobombAura->oScaleX, 6.f);
    o->oFlamingBobombAura->oGraphYOffset = o->oScaleY * 60.f;
    obj_copy_visibility_and_transparency(o->oFlamingBobombAura, o);
    if (damaged && (gGlobalTimer & 3) >= 2) {
        o->oFlamingBobombAura->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    } else {
        o->oFlamingBobombAura->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    }

    // Smoke gfx
    if (gOmmObject->flaming_bobomb.interactedFire) {
        if (damaged) {
            obj_play_sound(o, !POBJ_IS_STAR_DANCING * POBJ_SOUND_LAVA_BURN);
            struct Object *smoke = spawn_object(o, MODEL_BURN_SMOKE, bhvBobombFuseSmoke);
            smoke->oPosY += 90.f * o->oScaleY;
            smoke->oVelY = 10.f;
        } else if ((gGlobalTimer % 3) == 0) {
            struct Object *smoke = spawn_object(o, MODEL_BURN_SMOKE, bhvBobombFuseSmoke);
            smoke->oPosY += 80.f * o->oScaleY;
            smoke->oVelY = 8.f;
        }
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}
