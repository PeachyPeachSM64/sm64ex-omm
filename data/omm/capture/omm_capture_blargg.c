#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_blargg_init(UNUSED struct Object *o) {
    gOmmObject->state.actionState = 0;
    return true;
}

void omm_cappy_blargg_end(struct Object *o) {
    o->oAction = 0;
    o->oInteractStatus = 0;
}

f32 omm_cappy_blargg_get_top(struct Object *o) {
    return 100.f * o->oScaleY;
}

//
// Update
//

s32 omm_cappy_blargg_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_SAND;
    POBJ_SET_IMMUNE_TO_WIND;
    POBJ_SET_ATTACKING;

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        gOmmObject->state.actionState = 0;
        pobj_move(o, false, false, false);
        if (POBJ_B_BUTTON_PRESSED) {
            gOmmObject->state.actionState = 1;
            omm_mario_lock(gMarioState, -1);
        }
    }

    // Movement
    Vec3f previousPos = { o->oPosX, o->oPosY, o->oPosZ };
    perform_object_step(o, POBJ_STEP_FLAGS);
    if (!obj_is_on_ground(o) || o->oFloor->type != SURFACE_BURNING) {
        vec3f_copy(&o->oPosX, previousPos);
        obj_drop_to_floor(o);
    }
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, 0.f, 0.f, -1, OBJ_PARTICLE_FLAME);

    // Animation
    switch (gOmmObject->state.actionState) {

        // Idle/Moving
        case 0: {
            obj_anim_play(o, 0, 1.f);
        } break;

        // Throw fire ball
        case 1: {
            obj_anim_play(o, 1, 1.f);
            if (obj_anim_is_past_frame(o, 12)) {
                obj_play_sound(o, SOUND_OBJ_FLAME_BLOWN | 0xFF00);
                omm_spawn_blargg_fire_ball(o);
            } else if (obj_anim_is_at_end(o)) {
                gOmmObject->state.actionState = 0;
                omm_mario_unlock(gMarioState);
            }
        } break;
    }
    
    // Cappy values
    gOmmObject->cappy.offset[0] = -6.f;
    gOmmObject->cappy.offset[1] = 96.f + 10.f * sins(((obj_anim_get_frame(o) - 6) * 0x10000) / 30);
    gOmmObject->cappy.offset[2] = 88.f;
    gOmmObject->cappy.angle[0]  = -0x1400;
    gOmmObject->cappy.scale     = 1.f * (gOmmObject->state.actionState != 1);

    // OK
    POBJ_RETURN_OK;
}
