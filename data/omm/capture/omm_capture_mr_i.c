#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_mr_i_init(struct Object *o) {
    if (o->oBehParams2ndByte != 0 || ( // Don't capture Big Mr. I
        o->oAction != 1 &&
        o->oAction != 2)) {
        return false;
    }

    struct Object *iris = obj_get_nearest_with_behavior(o, bhvMrIBody);
    if (iris) {
        iris->oRoom = -1;
        iris->oNodeFlags |= GRAPH_RENDER_ACTIVE;
        iris->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
        iris->activeFlags &= ~ACTIVE_FLAG_IN_DIFFERENT_ROOM;
    }
    return true;
}

void omm_cappy_mr_i_end(struct Object *o) {
    obj_drop_to_floor(o);
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->oMrIUnkFC = 0;
    o->oAction = 1;
}

u64 omm_cappy_mr_i_get_type(struct Object *o) {
    return o->oBehParams2ndByte == 0 ? OMM_CAPTURE_MR_I : 0;
}

f32 omm_cappy_mr_i_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_mr_i_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }

        // Shoot
        // Hold B to increase strength, speed and size of projectile
        pobj_charge_attack(15, o, 250, 100);
        pobj_release_attack(15,
            omm_obj_spawn_mr_i_beam(o, _power_);
        );
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // OK
    pobj_return_ok;
}

void omm_cappy_mr_i_update_gfx(struct Object *o) {

    // Gfx
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_update_gfx(o);

    // Cappy transform
    bool modelPack = omm_models_get_model_pack_index(o->oGraphNode->georef) != -1;
    gOmmObject->cappy.tra_y = modelPack ? 160.f : 180.f;
    gOmmObject->cappy.scale = 1.5f;
}
