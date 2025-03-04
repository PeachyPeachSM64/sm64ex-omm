#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_swoop_init(struct Object *o) {
    obj_scale(o, 1.f);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    clear_surface_data(gOmmObject->swoop.data);
    return true;
}

void omm_cappy_swoop_end(struct Object *o) {
    obj_scale(o, 1.f);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
}

u64 omm_cappy_swoop_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SWOOP;
}

f32 omm_cappy_swoop_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_swoop_update(struct Object *o) {
    struct Surface *ceil = get_surface_from_data(gOmmObject->swoop.data);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Ceiling check
    if (!POBJ_A_BUTTON_DOWN || POBJ_IS_OPENING_DOORS || omm_mario_is_locked(gMarioState) || !ceil) {
        ceil = NULL;
    }

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_FLOATING;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS * (ceil == NULL);

    // States
    if (ceil) {

        // Apply platform displacement
        if (ceil->object) {
            obj_apply_displacement(o,
                ceil->object,
                gOmmObject->swoop.data->pos,
                gOmmObject->swoop.data->angle,
                gOmmObject->swoop.data->scale,
                true
            );
        }

        // Movement
        o->oVelX = 0.f;
        o->oVelY = 5.f;
        o->oVelZ = 0.f;
        perform_object_step(o, POBJ_STEP_FLAGS);
        pobj_handle_special_floors(o);
        pobj_stop_if_unpossessed();

    } else {

        // Inputs
        if (pobj_process_inputs(o)) {
            pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
            if (pobj_jump(o, 6) == POBJ_RESULT_JUMP_START) {
                o->oCurrAnim = NULL;
                obj_play_sound(o, POBJ_SOUND_JUMP_SWOOP);
            }
            if (POBJ_B_BUTTON_DOWN && obj_anim_is_near_end(o)) {
                obj_play_sound(o, POBJ_SOUND_WING_FLAP_2);
            }
        }

        // Movement
        perform_object_step(o, POBJ_STEP_FLAGS);
        pobj_decelerate(o);
        pobj_apply_gravity(o, 1.f);
        pobj_handle_special_floors(o);
        pobj_stop_if_unpossessed();
    }

    // Batman
    if (!POBJ_IS_OPENING_DOORS && POBJ_A_BUTTON_DOWN && o->oCeil && o->oCeil->normal.y < -0.7f) {
        get_surface_data(gOmmObject->swoop.data, o->oCeil);
    } else {
        clear_surface_data(gOmmObject->swoop.data);
    }

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    if (get_surface_from_data(gOmmObject->swoop.data)) {
        obj_anim_play(o, 1, 1.f);
    } else {
        obj_anim_play(o, 0, (o->oVelY > 0.f || POBJ_B_BUTTON_DOWN) ? 2.f : 1.f);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_swoop_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    if (get_surface_from_data(gOmmObject->swoop.data)) {
        o->oGfxPos[1] += 60.f * o->oScaleY;
        o->oGfxAngle[2] = 0x8000;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}
