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

    gOmmObject->bullet_bill.scale = o->oScaleY;
    return true;
}

void omm_cappy_bullet_bill_end(struct Object *o) {
    if (gOmmObject->state.actionState) {
        o->oAction = 0;
        o->oTimer = 0;
    }
    pobj_reset_home();
    obj_scale(o, gOmmObject->bullet_bill.scale);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
}

u64 omm_cappy_bullet_bill_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_BULLET_BILL;
}

f32 omm_cappy_bullet_bill_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_bullet_bill_update(struct Object *o) {
    obj_scale(o, gOmmObject->bullet_bill.scale);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;
    POBJ_SET_ATTACKING_BREAKABLE;

    // Inputs
    omm_capture_set_camera_behind_mario();
    if (pobj_process_inputs(o)) {
        pobj_move_3d(o, false, POBJ_B_BUTTON_DOWN, POBJ_A_BUTTON_DOWN, 0x3000, 0x300, 0x300);
        gOmmObject->state.actionTimer += (POBJ_B_BUTTON_DOWN ? 3 : 2);

        // The bullet bill explodes after a while
        if (gOmmObject->state.actionTimer >= 600) {
            omm_obj_spawn_explosion(o, 0, 1, 4, 10, omm_geo_explosion);
            obj_spawn_white_puff(o, SOUND_OBJ_DEFAULT_DEATH);
            gOmmObject->state.actionState = 1;
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
        }
    }
    pobj_stop_if_unpossessed();

    // Movement
    perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_UPDATE_PREV_POS);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Wall collision
    if (o->oWall) {
        omm_obj_spawn_explosion(o, 0, 1, 4, 10, omm_geo_explosion);
        obj_spawn_white_puff(o, SOUND_OBJ_DEFAULT_DEATH);
        gOmmObject->state.actionState = 1;
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
    }
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    struct Object *smoke = POBJ_B_BUTTON_DOWN ? omm_obj_spawn_fire_smoke(o, 0) : spawn_object(o, MODEL_SMOKE, bhvWhitePuffSmoke);
    Vec3f dir = { 0, 0, -1 };
    vec3f_rotate_zxy(dir, dir, o->oFaceAnglePitch, o->oFaceAngleYaw, 0);
    smoke->oPosX += o->hitboxRadius * dir[0];
    smoke->oPosY += o->hitboxRadius * dir[1] + omm_capture_get_hitbox_down_offset(o);
    smoke->oPosZ += o->hitboxRadius * dir[2];

    // OK
    pobj_return_ok;
}

void omm_cappy_bullet_bill_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Starts inflating after 8 seconds
    if (gOmmObject->state.actionTimer >= 480) {
        f32 t = invlerp_0_1_s(gOmmObject->state.actionTimer, 480, 600);
        f32 s = relerp_0_1_f(sins(0x10000 / (1.f - t)), -1.f, +1.f, 0.8f, 1.2f);
        f32 offset = omm_capture_get_hitbox_down_offset(o) * (1.f - s);
        obj_scale(o, gOmmObject->bullet_bill.scale * s);
        o->oGfxPos[1] += offset;
    }

    // Cappy transform
    gOmmObject->cappy.tra_y = 200.f;
    gOmmObject->cappy.tra_z = 100.f;
    gOmmObject->cappy.scale = 4.f;
    gOmmObject->cappy.o_gfx = true;
}
