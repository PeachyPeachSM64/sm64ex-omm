#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_amp_init(struct Object *o) {
    if (o->oAction == HOMING_AMP_ACT_INACTIVE) {
        return false;
    }

    obj_scale(o, 1.f);
    obj_anim_play(o, 0, 1.f);
    o->oAnimState = 1;
    return true;
}

void omm_cappy_amp_end(struct Object *o) {
    if (o->behavior == bhvHomingAmp) {
        pobj_reset_home();
        o->oAction = o->oPrevAction = HOMING_AMP_ACT_GIVE_UP;
        o->oAmpYPhase = 0;
    } else {
        o->oAction = AMP_ACT_IDLE;
        o->oMoveAngleYaw = o->oFaceAngleYaw;
        o->oAmpRadiusOfRotation = 0;
        o->oAmpYPhase = 0;
    }
    obj_scale(o, 1.f);
    obj_anim_play(o, 0, 1.f);
    o->oAnimState = 1;
}

u64 omm_cappy_amp_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_AMP;
}

f32 omm_cappy_amp_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_amp_update(struct Object *o) {

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
    POBJ_SET_FLOATING;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        s32 res = pobj_jump(o, 1);
        if (res == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        } else if (res != POBJ_RESULT_JUMP_HOLD) {
            if (gOmmObject->state.actionFlag) {
                gOmmObject->state.actionState += (POBJ_A_BUTTON_DOWN ? 1 : 60);
                if (gOmmObject->state.actionState < 60) {
                    o->oVelY = 0.f;
                    if (o->oTimer % 3 == 0) {
                        omm_obj_spawn_shockwave(o, 5.f, 20.f, 0x00, 0xFF, 12);
                    }
                }
            } else if (POBJ_A_BUTTON_PRESSED) {
                gOmmObject->state.actionFlag = true;
                gOmmObject->state.actionState = 0;
                o->oVelY = 0.f;
            }
        }

        // Electric discharge
        // Hold B to charge, release to attack
        if (!obj_get_first_with_behavior(bhvOmmAmpDischarge)) {
            pobj_charge_attack(15, o, 150, 60);
            pobj_release_attack(15,
                omm_obj_spawn_amp_discharge(o, _power_);
            );
        }
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();
    if (obj_is_on_ground(o)) {
        gOmmObject->state.actionFlag = false;
    }

    // Attract coins
    omm_obj_process_interactions(o, POBJ_INT_PRESET_AMP);

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();
    if (POBJ_IS_BOUNCING || POBJ_IS_STAR_DANCING) {
        gOmmObject->state.actionFlag = false;
    }

    // Animation, sound and particles
    obj_play_sound(o, POBJ_SOUND_AMP_BUZZ);

    // OK
    pobj_return_ok;
}

void omm_cappy_amp_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    o->oGfxPos[1] += 20.f * o->oScaleY + 5.f * coss(o->oTimer * 0x8B0);

    // Cappy transform
    bool modelPack = omm_models_get_model_pack_index(o->oGraphNode->georef) != -1;
    gOmmObject->cappy.tra_x = modelPack ? -3.f : 0.f;
    gOmmObject->cappy.tra_y = modelPack ? 22.f : 30.f;
    gOmmObject->cappy.scale = 0.8f;
    gOmmObject->cappy.o_gfx = true;
}
