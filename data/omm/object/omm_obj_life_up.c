#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Behavior
//

static void bhv_omm_life_up_update() {
    gPlayer1Controller->buttonPressed &= ~START_BUTTON;
    struct Object *o = gCurrentObject;
    if (o->oAction == 0) {
        omm_health_update(gMarioState);
        gHudDisplay.timer -= sTimerRunning;
        if (o->oTimer >= OMM_HEALTH_ODYSSEY_LIFE_UP_POS_END) {
            o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
            clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
            omm_sound_stop(OMM_SOUND_EVENT_LIFE_UP);
            omm_sound_play(OMM_SOUND_EFFECT_HEAL, NULL);
            music_unlower_volume(SEQ_PLAYER_LEVEL, 60);
            o->oAction = o->oPrevAction = 1;
        }
    } else if (o->oTimer >= OMM_HEALTH_ODYSSEY_LIFE_UP_CUTSCENE_END) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmLifeUp[] = {
    OBJ_TYPE_DEFAULT,
    0x08000000,
    0x0C000000, (uintptr_t) bhv_omm_life_up_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_life_up(struct Object *o) {
    struct Object *lifeup = spawn_object(o, MODEL_NONE, bhvOmmLifeUp);
    lifeup->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
    omm_sound_play(OMM_SOUND_EVENT_LIFE_UP, NULL);
    music_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
    return lifeup;
}
