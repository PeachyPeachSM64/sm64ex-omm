#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

static void bhv_omm_wf_transition_update() {
    struct Object *o = gCurrentObject;
    struct MarioState *m = gMarioState;
    if (!omm_is_transition_active()) {

        // Unload Whomp King
        struct Object *whompKingBoss = obj_get_first_with_behavior(bhvWhompKingBoss);
        if (whompKingBoss) {
            stop_background_music(SEQ_EVENT_BOSS);
            obj_mark_for_deletion(whompKingBoss);
        }

        // Warp Mario back
        vec3f_set(m->pos, 840, 3584, 1230);
        m->intendedYaw = m->faceAngle[1] = m->marioObj->oFaceAngleYaw = m->marioObj->oGfxAngle[1] = 0xA000;
        if (omm_mario_is_capture(m)) {
            gOmmCapture->oFaceAngleYaw = m->intendedYaw;
            obj_set_forward_and_y_vel(gOmmCapture, 0, 0);
            omm_mario_unlock(m);
        } else {
            vec3f_zero(m->vel);
            mario_set_forward_vel(m, 0);
            omm_mario_set_action(m, ACT_FREEFALL, 0, 0xFFFF);
        }

        // Play transition
        gTimeStopState &= ~(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
        play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 30, 0xFF, 0xFF, 0xFF);
        omm_render_effect_you_got_a_star_stop();
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmWfTransition[] = {
    OBJ_TYPE_SPAWNER,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_wf_transition_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_wf_transition(struct Object *o) {
    struct Object *transition = spawn_object(o, MODEL_NONE, bhvOmmWfTransition);
    transition->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    gTimeStopState |= (TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
    play_transition(WARP_TRANSITION_FADE_INTO_CIRCLE, 30, 0xFF, 0xFF, 0xFF);
    return transition;
}
