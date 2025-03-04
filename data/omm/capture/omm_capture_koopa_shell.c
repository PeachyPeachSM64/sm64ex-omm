#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_koopa_shell_init(struct Object *o) {
    if (o->behavior == bhvKoopaShell) { // Changing the behavior, so unshelled Koopas don't try to target the captured shell
        o->behavior = bhvOmmPossessedKoopaShell;
    }
    return true;
}

void omm_cappy_koopa_shell_end(struct Object *o) {
    if (o->behavior == bhvOmmPossessedKoopaShell) {
        o->behavior = bhvKoopaShell;
    }
    audio_stop_shell_music();
}

u64 omm_cappy_koopa_shell_get_type(struct Object *o) {
    return o->behavior == bhvKoopaShellUnderwater ? OMM_CAPTURE_KOOPA_SHELL_WATER : OMM_CAPTURE_KOOPA_SHELL;
}

f32 omm_cappy_koopa_shell_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_koopa_shell_update(struct Object *o) {
    bool isKoopaShellUnderwater = (o->behavior == bhvKoopaShellUnderwater);

    // Init
    if (!gOmmObject->state.actionFlag) {
        audio_play_shell_music();
        gOmmObject->state.actionFlag = true;
    }

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, POBJ_B_BUTTON_DOWN, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }
    }

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER * isKoopaShellUnderwater;
    POBJ_SET_AFFECTED_BY_WATER * isKoopaShellUnderwater;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_QUICKSAND;
    POBJ_SET_ABLE_TO_MOVE_ON_WATER * !isKoopaShellUnderwater;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;
    POBJ_SET_ATTACKING_BREAKABLE;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
    if (o->oFloorType == OBJ_FLOOR_TYPE_GROUND) {
        obj_make_step_sound_and_particle(o, NULL, 0, 0, POBJ_SOUND_RIDING_SHELL_GROUND, OBJ_PARTICLE_MIST);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_koopa_shell_update_gfx(struct Object *o) {

    // Gfx
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_update_gfx(o);
    o->oGfxAngle[1] = gGlobalTimer * 0x2000;
    o->oFlags |= OBJ_FLAG_SHADOW_COPY_OBJ_POS;

    // Cappy transform
    gOmmObject->cappy.scale = 0.f;
}
