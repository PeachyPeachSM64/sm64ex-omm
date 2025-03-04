#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layout
//

const GeoLayout omm_geo_mr_i_beam[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(purple_marble_geo),
};

//
// Behavior
//

static void bhv_omm_mr_i_beam_delete(struct Object *o) {
    for (s32 i = 0; i != 10; ++i) {
        struct Object *p = spawn_object(o, MODEL_PURPLE_MARBLE, bhvPurpleParticle);
        obj_scale(p, 0.5f);
    }
    obj_create_sound_spawner(o, SOUND_OBJ_DEFAULT_DEATH);
    obj_mark_for_deletion(o);
}

static void bhv_omm_mr_i_beam_loop() {
    struct Object *o = gCurrentObject;
    s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);
    if (o->oTimer > 60 || check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS | OBJECT_STEP_CHECK_HIT_WALL | OBJECT_STEP_CHECK_HIT_SLANTED_FLOOR | OBJECT_STEP_CHECK_HIT_SLANTED_CEILING)) {
        bhv_omm_mr_i_beam_delete(o);
        return;
    }
    obj_reset_hitbox(o, 20, 30, 0, 0, 15, 0);
    obj_set_params(o, 0, 0, 0, 0, true);
    struct Object *interacted = omm_obj_process_interactions(o, obj_get_interaction_flags_from_power(o->oObjectPower, OBJ_INT_PRESET_MR_I_BEAM));
    if (interacted && !omm_obj_is_collectible(interacted)) {
        bhv_omm_mr_i_beam_delete(o);
    }
}

const BehaviorScript bhvOmmMrIBeam[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_mr_i_beam_loop),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_mr_i_beam(struct Object *o, f32 power) {
    struct Object *beam = obj_spawn_from_geo(o, omm_geo_mr_i_beam, bhvOmmMrIBeam);
    f32 scaleFactor = lerp_f(power, 1.f, 3.f);
    beam->oPosX += o->hitboxRadius * 1.1f * sins(o->oMoveAngleYaw);
    beam->oPosY += o->hitboxHeight * 0.5f - 30.f * scaleFactor;
    beam->oPosZ += o->hitboxRadius * 1.1f * coss(o->oMoveAngleYaw);
    obj_set_angle(beam, 0, o->oMoveAngleYaw, 0);
    obj_set_throw_vel(beam, o, 30.f * (0.5f + scaleFactor) * o->oScaleX, 0.f);
    obj_scale(beam, 2.f * scaleFactor * o->oScaleY);
    beam->oObjectPower = power;
    obj_play_sound(o, SOUND_OBJ_MRI_SHOOT);
    obj_set_interact_id(o, beam);
    return beam;
}
