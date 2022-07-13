#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define LIFE_UP_CUTSCENE_HEAL_END   (OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT * 3)
#define LIFE_UP_CUTSCENE_POS_END    (LIFE_UP_CUTSCENE_HEAL_END + 15)
#define LIFE_UP_CUTSCENE_END        (LIFE_UP_CUTSCENE_POS_END + 15)

f32 omm_get_life_up_gauge_position(s32 hp) {
    struct Object *o = obj_get_first_with_behavior(omm_bhv_life_up);
    if (o == NULL) return (hp <= omm_health_get_max_hp(0) ? 1.f : 0.f);
    f32 t = (f32) (o->oTimer - LIFE_UP_CUTSCENE_HEAL_END) / (f32) (LIFE_UP_CUTSCENE_POS_END - LIFE_UP_CUTSCENE_HEAL_END);
    return sqrtf(clamp_0_1_f(1.f - t));
}

f32 omm_get_life_gauge_rumble_strength(struct MarioState *m) {
    struct Object *o = obj_get_first_with_behavior(omm_bhv_life_up);
    if (o == NULL) return 15.f * omm_health_is_mario_losing_hp(m);
    f32 t = (f32) (o->oTimer - LIFE_UP_CUTSCENE_POS_END) / (f32) (LIFE_UP_CUTSCENE_END - LIFE_UP_CUTSCENE_POS_END);
    return (t < 0.f ? 0.f : 20.f * (1.f - t));
}

//
// Behavior
//

static void omm_bhv_life_up_update() {
    gPlayer1Controller->buttonPressed &= ~START_BUTTON;
    struct Object *o = gCurrentObject;
    if (o->oAction == 0) {
        omm_health_update(gMarioState);
        if (o->oTimer >= LIFE_UP_CUTSCENE_POS_END) {
            o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
            clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
            omm_sound_stop(OMM_SOUND_EVENT_LIFE_UP);
            omm_sound_play(OMM_SOUND_EFFECT_HEAL, NULL);
            music_unlower_volume(SEQ_PLAYER_LEVEL, 60);
            o->oAction = o->oPrevAction = 1;
        }
    } else if (o->oTimer >= LIFE_UP_CUTSCENE_END) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript omm_bhv_life_up[] = {
    OBJ_TYPE_DEFAULT,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_life_up_update,
    0x09000000
};

//
// Spawner
//

struct Object *omm_spawn_life_up(struct Object *o) {
    struct Object *lifeup = spawn_object(o, MODEL_NONE, omm_bhv_life_up);
    lifeup->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
    omm_sound_play(OMM_SOUND_EVENT_LIFE_UP, NULL);
    music_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
    return lifeup;
}
