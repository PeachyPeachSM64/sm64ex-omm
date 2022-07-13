#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void cappy_flaming_bobomb_explode(struct Object *o, bool deleteObj) {
    omm_spawn_flaming_bobomb_explosion(o);

    // Final boom
    if (deleteObj) {
        obj_drop_to_floor(o);
        omm_spawn_shockwave_fire(o, 100, 80, 120, 40, 5000, OMM_TEXTURE_BOWSER_FIRE_BLUE_1, OMM_TEXTURE_BOWSER_FIRE_BLUE_2);
        obj_mark_for_deletion(o->oFlamingBobombAura);
        obj_mark_for_deletion(o);
    }

    // Bowser
    struct Object *bowser = obj_get_first_with_behavior(omm_bhv_bowser);
    if (bowser->oHealth > 0 && obj_get_horizontal_distance(o, bowser) < (bowser->hitboxRadius + 600.f)) {
        bowser->oBowserHit = 1;
    }
}

//
// Init
//

bool cappy_flaming_bobomb_init(struct Object* o) {
    if (o->oAction != 1 &&
        o->oAction != 3) {
        return false;
    }

    gOmmData->object->state.actionTimer = 0;
    gOmmData->object->state.actionState = 0;
    gOmmData->object->flaming_bobomb.interactedTimer = 0;
    gOmmData->object->flaming_bobomb.interactedFire = false;
    return true;
}

void cappy_flaming_bobomb_end(struct Object *o) {
    cappy_flaming_bobomb_explode(o, true);
}

f32 cappy_flaming_bobomb_get_top(struct Object *o) {
    return 94.f * o->oScaleY;
}

//
// Update
//

s32 cappy_flaming_bobomb_update(struct Object *o) {

    // Hitbox
    o->oScaleX = 1.2f;
    o->oScaleY = 1.2f;
    o->oScaleZ = 1.2f;
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        pobj_move(o, false, false, false);
        pobj_jump(o, 0, 1);

        // Explosion
        if (POBJ_B_BUTTON_PRESSED && (gOmmData->object->state.actionTimer == 0)) {
            gOmmData->object->state.actionTimer = 30;
            gOmmData->object->state.actionState++;

            // Five charges
            if (gOmmData->object->state.actionState == 5) {
                omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 0);
            } else {
                cappy_flaming_bobomb_explode(o, false);
            }
        } else {
            gOmmData->object->state.actionTimer = max_s(0, gOmmData->object->state.actionTimer - 1);
        }
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS(

    // Do not process interactions during recovery time
    if (gOmmData->object->flaming_bobomb.interactedTimer != 0) {
        break;
    }
    
    // The flaming bob-omb can withstand one fire attack before exploding and releasing Mario
    if ((obj->oInteractType & INTERACT_FLAME) &&
        !gOmmData->object->flaming_bobomb.interactedFire &&
        obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HURTBOX_HITBOX_IF_ZERO)) {

        // Mario gets damaged, but keeps his capture
        // Then, stays invincible for 2 seconds
        gMarioState->hurtCounter = 12;
        gOmmData->object->flaming_bobomb.interactedFire = !OMM_CHEAT_GOD_MODE;
        gOmmData->object->flaming_bobomb.interactedTimer = 60;
        obj->oInteractStatus |= INT_STATUS_INTERACTED;

        // Gfx/Sfx
        omm_spawn_flaming_bobomb_explosion(o);
    }

    );
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    o->oFaceAnglePitch = 0;
    o->oFaceAngleRoll = 0;
    o->oMoveAnglePitch = 0;
    o->oMoveAngleRoll = 0;
    o->oGraphYOffset = 0;
    obj_update_gfx(o);
    obj_anim_play(o, 0, (o->oVelY <= 0.f) * max_f(1.f, o->oForwardVel * (2.f / (omm_capture_get_walk_speed(o)))));
    obj_random_blink(o, &o->oBobombBlinkTimer);
    if (gOmmData->object->flaming_bobomb.interactedTimer & 1) {
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    } else {
        o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    }

    // Aura gfx
    vec3f_copy(&o->oFlamingBobombAura->oPosX, &o->oPosX);
    vec3f_copy(&o->oFlamingBobombAura->oScaleX, &o->oScaleX);
    vec3f_mul(&o->oFlamingBobombAura->oScaleX, 6.f);
    o->oFlamingBobombAura->oGraphYOffset = o->oScaleY * 60.f;
    if ((gOmmData->object->flaming_bobomb.interactedTimer % 4) >= 2) {
        o->oFlamingBobombAura->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    } else {
        o->oFlamingBobombAura->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    }

    // Smoke gfx
    if (gOmmData->object->flaming_bobomb.interactedFire) {
        if (gOmmData->object->flaming_bobomb.interactedTimer != 0) {
            play_sound(SOUND_MOVING_LAVA_BURN, o->oCameraToObject);
            struct Object *smoke = spawn_object(o, MODEL_BURN_SMOKE, bhvBobombFuseSmoke);
            smoke->oPosY += 90.f * o->oScaleY;
            smoke->oVelY = 10.f;
        } else if ((gGlobalTimer % 3) == 0) {
            struct Object *smoke = spawn_object(o, MODEL_BURN_SMOKE, bhvBobombFuseSmoke);
            smoke->oPosY += 80.f * o->oScaleY;
            smoke->oVelY = 8.f;
        }
    }

    // Cappy values
    gOmmData->object->cappy.offset[1] = 94.f;
    gOmmData->object->cappy.scale     = 1.2f;

    // OK
    gOmmData->object->flaming_bobomb.interactedTimer = max_s(0, gOmmData->object->flaming_bobomb.interactedTimer - 1);
    POBJ_RETURN_OK;
}
