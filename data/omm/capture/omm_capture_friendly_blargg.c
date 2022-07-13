#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_friendly_blargg_init(UNUSED struct Object *o) {
    gOmmData->object->state.actionFlag = false;
    return true;
}

void cappy_friendly_blargg_end(struct Object *o) {
    audio_stop_shell_music();
    o->oAction = 0;
    o->oInteractStatus = 0;
}

f32 cappy_friendly_blargg_get_top(struct Object *o) {
    return 50.f * o->oScaleY;
}

//
// Update
//

s32 cappy_friendly_blargg_update(struct Object *o) {

    // Init
    if (!gOmmData->object->state.actionFlag) {
        audio_play_shell_music();
        gOmmData->object->state.actionFlag = true;
    }

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        pobj_move(o, false, POBJ_B_BUTTON_DOWN, false);
    }

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
    obj_make_step_sound_and_particle(o, &gOmmData->object->state.walkDistance, 0.f, 0.f, SOUND_MOVING_RIDING_SHELL_LAVA, OBJ_PARTICLE_FLAME);
    
    // Cappy values
    gOmmData->object->cappy.offset[0] = -3.f;
    gOmmData->object->cappy.offset[1] = 48.f + 5.f * sins(((obj_anim_get_frame(o) - 6) * 0x10000) / 30);
    gOmmData->object->cappy.offset[2] = 44.f;
    gOmmData->object->cappy.angle[0]  = -0x1400;
    gOmmData->object->cappy.scale     = 0.5f;

    // OK
    POBJ_RETURN_OK;
}
