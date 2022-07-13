#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_bullet_bill_init(struct Object *o) {
    if (o->oAction != 2 || o->oTimer < 50) {
        return false;
    }

    gOmmData->object->state.initialPos[0] = o->oHomeX;
    gOmmData->object->state.initialPos[1] = o->oHomeY;
    gOmmData->object->state.initialPos[2] = o->oHomeZ;
    gOmmData->object->bullet_bill.scale   = o->oScaleY;
    gOmmData->object->state.actionState   = 0;
    gOmmData->object->state.actionTimer   = 0;
    return true;
}

void cappy_bullet_bill_end(struct Object *o) {
    if (gOmmData->object->state.actionState) {
        o->oAction = 0;
        o->oTimer = 0;
    }
    o->oHomeX = gOmmData->object->state.initialPos[0];
    o->oHomeY = gOmmData->object->state.initialPos[1];
    o->oHomeZ = gOmmData->object->state.initialPos[2];
    o->oScaleX = gOmmData->object->bullet_bill.scale;
    o->oScaleY = gOmmData->object->bullet_bill.scale;
    o->oScaleZ = gOmmData->object->bullet_bill.scale;
    o->oFaceAnglePitch = 0;
    o->oMoveAnglePitch = 0;
}

f32 cappy_bullet_bill_get_top(struct Object *o) {
    return 200.f * o->oScaleY;
}

//
// Update
//

s32 cappy_bullet_bill_update(struct Object *o) {

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
        gOmmData->object->state.actionTimer += (POBJ_B_BUTTON_DOWN ? 2 : 1);

        // The bullet bill explodes after a while
        if (gOmmData->object->state.actionTimer >= 300) {
            omm_spawn_explosion(o);
            obj_spawn_white_puff(o, 0);
            gOmmData->object->state.actionState = 1;
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
        }
        
        // Starts inflating after 8 seconds
        else if (gOmmData->object->state.actionTimer >= 240) {
            f32 t = invlerp_0_1_s(gOmmData->object->state.actionTimer, 240, 300);
            f32 s = relerp_0_1_f(sins(0x10000 / (1.f - t)), -1.f, +1.f, 0.75f, 1.25f);
            obj_scale(o, gOmmData->object->bullet_bill.scale * s);
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
        gOmmData->object->state.actionState = 1;
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
    }
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    obj_make_step_sound_and_particle(o, &gOmmData->object->state.walkDistance, 0.f, 0.f, -1, (POBJ_B_BUTTON_DOWN ? OBJ_PARTICLE_FIRE : OBJ_PARTICLE_SMOKE));

    // Cappy values
    gOmmData->object->cappy.offset[1] = 200.f;
    gOmmData->object->cappy.offset[2] = 50.f;
    gOmmData->object->cappy.scale     = 4.f;

    // OK
    POBJ_RETURN_OK;
}
