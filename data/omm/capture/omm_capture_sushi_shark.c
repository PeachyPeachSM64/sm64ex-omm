#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_sushi_shark_init(struct Object *o) {
    if (!obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ))) {
        return false;
    }

    return true;
}

void omm_cappy_sushi_shark_end(struct Object *o) {
    obj_anim_play(o, 0, 1.f);
    o->oHomeY -= find_water_level(o->oPosX, o->oPosZ);
    o->oAction = 1;
    o->oPrevAction = 1;
    o->oTimer = 1;
    o->oSushiSharkUnkF4 = random_u16() << 7;
}

u64 omm_cappy_sushi_shark_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SUSHI_SHARK;
}

f32 omm_cappy_sushi_shark_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_sushi_shark_update(struct Object *o) {
    bool isDashing = vec3f_length(&o->oVelX) > pobj_get_run_speed(o);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_UNDER_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;
    POBJ_SET_ATTACKING;
    POBJ_SET_ATTACKING_BREAKABLE * isDashing;

    // Inputs
    omm_capture_set_camera_behind_mario();
    if (pobj_process_inputs(o)) {
        pobj_move_3d(o, false, POBJ_B_BUTTON_DOWN, POBJ_A_BUTTON_DOWN, 0x3F00, 0x300, 0x300);
    }
    pobj_stop_if_unpossessed();

    // Movement
    perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_UPDATE_PREV_POS | OBJ_STEP_KEEP_UNDERWATER);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    f32 distInc = vec3f_length(&o->oVelX);
    f32 animSpeed = relerp_0_1_f(distInc, pobj_get_walk_speed(o), pobj_get_dash_speed(o), 1.f, 3.f);
    obj_anim_play(o, 0, animSpeed);
    f32 distMin = pobj_get_walk_speed(o) * (o->oCurrAnim->mLoopEnd / 2.f) * relerp_0_1_f(animSpeed, 1.f, 3.f, 1.f, 2.f / 3.f);
    f32 bubbleDistance = fmodf(gOmmObject->state.walkDistance, distMin / 4.f);
    obj_make_step_sound_and_particle(o, &bubbleDistance, distMin / 4.f, distInc, 0, OBJ_PARTICLE_BUBBLE);
    obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, distMin, distInc, POBJ_SOUND_SWIMMING, 0);
    if (animSpeed >= 1.5f) {
        spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvPlungeBubble);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_sushi_shark_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    f32 frame = o->oAnimFrame;
    f32 offset = 100.f * o->oScaleX * coss(0x10000 * frame / o->oCurrAnim->mLoopEnd);
    o->oGfxPos[0] = o->oPosX - offset * sins(o->oFaceAngleYaw + 0x4000);
    o->oGfxPos[2] = o->oPosZ - offset * coss(o->oFaceAngleYaw + 0x4000);

    // Cappy transform
    gOmmObject->cappy.object = o;
}
