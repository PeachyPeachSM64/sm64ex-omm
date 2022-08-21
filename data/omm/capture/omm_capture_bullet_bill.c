#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_bullet_bill_init(struct Object *o) {
    if (o->oAction != 2 || o->oTimer < 50) {
        return false;
    }

    gOmmObject->state.initialPos[0] = o->oHomeX;
    gOmmObject->state.initialPos[1] = o->oHomeY;
    gOmmObject->state.initialPos[2] = o->oHomeZ;
    gOmmObject->bullet_bill.scale   = o->oScaleY;
    gOmmObject->state.actionState   = 0;
    gOmmObject->state.actionTimer   = 0;
    return true;
}

void omm_cappy_bullet_bill_end(struct Object *o) {
    if (gOmmObject->state.actionState) {
        o->oAction = 0;
        o->oTimer = 0;
    }
    o->oHomeX = gOmmObject->state.initialPos[0];
    o->oHomeY = gOmmObject->state.initialPos[1];
    o->oHomeZ = gOmmObject->state.initialPos[2];
    o->oScaleX = gOmmObject->bullet_bill.scale;
    o->oScaleY = gOmmObject->bullet_bill.scale;
    o->oScaleZ = gOmmObject->bullet_bill.scale;
    o->oFaceAnglePitch = 0;
    o->oMoveAnglePitch = 0;
}

f32 omm_cappy_bullet_bill_get_top(struct Object *o) {
    return 200.f * o->oScaleY;
}

//
// Update
//

s32 omm_cappy_bullet_bill_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_SAND;
    POBJ_SET_IMMUNE_TO_WIND;
    POBJ_SET_ATTACKING;

    // Inputs
    omm_capture_set_camera_behind_mario();
    if (!omm_mario_is_locked(gMarioState)) {
        pobj_move_3d(o, false, POBJ_B_BUTTON_DOWN);
        gOmmObject->state.actionTimer += (POBJ_B_BUTTON_DOWN ? 2 : 1);

        // The bullet bill explodes after a while
        if (gOmmObject->state.actionTimer >= 300) {
            omm_spawn_explosion(o);
            obj_spawn_white_puff(o, 0);
            gOmmObject->state.actionState = 1;
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
        }
        
        // Starts inflating after 8 seconds
        else if (gOmmObject->state.actionTimer >= 240) {
            f32 t = invlerp_0_1_s(gOmmObject->state.actionTimer, 240, 300);
            f32 s = relerp_0_1_f(sins(0x10000 / (1.f - t)), -1.f, +1.f, 0.75f, 1.25f);
            obj_scale(o, gOmmObject->bullet_bill.scale * s);
        }
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Wall collision
    if (o->oWall) {
        omm_spawn_explosion(o);
        obj_spawn_white_puff(o, 0);
        gOmmObject->state.actionState = 1;
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, 0.f, 0.f, -1, (POBJ_B_BUTTON_DOWN ? OBJ_PARTICLE_FIRE : OBJ_PARTICLE_SMOKE));

    // Cappy values
    gOmmObject->cappy.offset[1] = 200.f;
    gOmmObject->cappy.offset[2] = 50.f;
    gOmmObject->cappy.scale     = 4.f;

    // OK
    POBJ_RETURN_OK;
}
