#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"
extern void omm_mario_capture_update_cap_object(struct Object *o, struct Object *obj, struct Object *cap);

//
// Behaviors
//

static void bhv_omm_yoshi_mode_yoshi_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    if (OMM_EXTRAS_INVISIBLE_MODE) {
        obj_mark_for_deletion(o);
        return;
    }

    // Mario actions
    u16 soundBanksEnable = 0;
    u16 soundBanksDisable = 0;
    switch (m->action) {

        case ACT_FALL_AFTER_STAR_GRAB: {
            soundBanksEnable = (1 << 3);
            obj_anim_play(o, 2, 1.f);
            obj_anim_extend(o);
        } break;

        case ACT_STAR_DANCE_NO_EXIT: {
            soundBanksEnable = (1 << 3);
            if (m->actionState == 2) {
                disable_time_stop();
                enable_background_sound();
                gOmmWarp->state = POBJ_WARP_STATE_WARPING;
                gOmmWarp->georef = yoshi_geo;
                gOmmWarp->behavior = bhvOmmYoshi;
                gOmmWarp->object = NULL;
                gOmmWarp->behParams = 0;
                gOmmWarp->behParams2ndByte = 0;
                omm_mario_possess_object_after_warp(m);
                gOmmWarp->state = POBJ_WARP_STATE_HAS_WARPED;
                omm_mario_lock(m, 15);
                obj_mark_for_deletion(o);
                return;
            }
            obj_anim_play(o, 0, 1.f);
        } break;

        case ACT_STAR_DANCE_EXIT: {
            soundBanksEnable = (1 << 3);
            obj_anim_play(o, 0, 1.f);
        } break;

        case ACT_STAR_DANCE_WATER: {
            soundBanksEnable = (1 << 3);
            obj_anim_play(o, 1, 1.f);
        } break;

        case ACT_JUMBO_STAR_CUTSCENE: {
            soundBanksEnable = (1 << 3) | (1 << 5) | (1 << 9);
            switch ((m->actionArg << 4) | m->actionState) {

                // Falling
                case 0x00: {
                    obj_anim_play(o, 2, 1.f);
                    obj_anim_extend(o);
                } break;

                // Landing
                case 0x01: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Raise hand
                case 0x10: {
                    m->pos[1] -= 45.f;
                    if (m->pos[1] > 310.f) {
                        obj_anim_play(o, 2, 1.f);
                        obj_anim_extend(o);
                    } else {
                        obj_anim_play(o, 0, 1.f);
                    }
                } break;

                // Taking off
                case 0x11: {
                    m->pos[1] -= 45.f;
                    switch (m->marioObj->oAnimFrame) {
                        case 3:
                        case 28:
                        case 60: {
                            obj_anim_play_with_sound(o, 2, 1.f, NO_SOUND, true);
                            obj_anim_extend(o);
                        } break;
                    }
                } break;

                // Flying
                case 0x20:
                case 0x21:
                case 0x22: {
                    obj_anim_play(o, 1, 4.f);
                    struct Object *wings = omm_obj_spawn_yoshi_wings(o);
                    if (wings) {
                        wings->oAnimState = 1;
                    }
                } break;
            }
        } break;

        case ACT_END_PEACH_CUTSCENE: {
            soundBanksEnable = (1 << 3);
            obj_unload_all_with_behavior(bhvYoshi);
            obj_scale(m->marioObj, 0.75f);
            switch (m->actionArg) {

                // Falling
                case 0: {
                    obj_anim_play(o, 2, 1.f);
                    obj_anim_extend(o);
                } break;

                // Landing
                case 1: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Summon jumbo star
                case 2: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Spawn Peach
                case 3: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Peach descends
                case 4: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Run to Peach
                case 5: {
                    obj_anim_play(o, 1, 2.f);
                    if (m->pos[2] <= -1105.f) {
                        m->pos[2] = -1105.f;
                        m->actionState = 0;
                        m->actionTimer = 0;
                        m->actionArg++;
                    }
                } break;

                // Peach dialog 1
                case 6: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Peach dialog 2
                case 7: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Kiss
                case 8: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Star dance
                case 9: {
                    if (m->actionTimer == 85 || m->actionTimer == 95) {
                        m->vel[1] = 20.f;
                    }
                    m->pos[1] += m->vel[1];
                    m->vel[1] -= 5.f;
                    m->floorHeight = find_floor(m->pos[0], m->pos[1] + 100.f, m->pos[2], &m->floor);
                    if (m->pos[1] <= m->floorHeight) {
                        m->pos[1] = m->floorHeight;
                        m->vel[1] = 0.f;
                        obj_anim_play(o, 0, 1.f);
                    } else {
                        obj_anim_play(o, 2, 1.f);
                        obj_anim_extend(o);
                    }
                } break;

                // Peach dialog 3
                case 10: {
                    obj_anim_play(o, 0, 1.f);
                } break;

                // Run to castle
                // Skip the "Mario!" voice clip
                case 11: {
                    m->actionTimer = 0;
                    m->actionState++;
                    if (m->actionState == 10) {
                        obj_play_sound(o, SOUND_GENERAL_YOSHI_TALK);
                    }
                    if (m->actionState >= 389) {
                        m->actionTimer = 389;
                    }
                    m->pos[2] -= 10.f * min_f(1.f, sqr_f(m->actionState / 45.f));
                    m->pos[1] = m->floorHeight = find_floor(m->pos[0], m->pos[1] + 100.f, m->pos[2], &m->floor);
                    obj_anim_play(o, 1, 1.f);
                } break;

                // Fade out
                case 12: {
                    obj_anim_play(o, 1, 1.f);
                } break;
            }
        } break;

        case ACT_CREDITS_CUTSCENE: {
            soundBanksDisable = (1 << 3) | (1 << 5) | (1 << 9);
            obj_unload_all_with_behavior(bhvYoshi);
            obj_scale(m->marioObj, 0.8f);
            if (m->pos[1] < m->waterLevel - 100) {
                obj_anim_play(o, 1, 1.f);
            } else {
                obj_anim_play(o, 0, 1.f);
            }
        } break;

        case ACT_END_WAVING_CUTSCENE: {
            soundBanksDisable = (1 << 3) | (1 << 5) | (1 << 9);
            obj_unload_all_with_behavior(bhvYoshi);
            obj_scale(m->marioObj, 0.75f);
            obj_anim_play(o, 0, 1.f);
            if (m->actionState == 1) {
                m->pos[0] -= 60.f;
                m->faceAngle[1] += 0x8000;
                m->actionState = 2;
            }
        } break;

        default: {
            obj_mark_for_deletion(o);
        } return;
    }

    // Sync gfx with Mario
    obj_set_pos_vec3f(o, m->pos);
    obj_set_angle_vec3s(o, m->faceAngle);
    obj_set_scale_vec3f(o, m->marioObj->oGfxScale);
    obj_update_gfx(o);
    m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;

    // Wing cap
    if (omm_mario_has_wing_cap(m)) {
        omm_obj_spawn_yoshi_wings(o);
    }

    // Vanish cap
    if (omm_mario_has_vanish_cap(m) && !omm_mario_cap_is_flickering(m)) {
        o->oTransparency = 0x80;
        o->activeFlags |= ACTIVE_FLAG_DITHERED_ALPHA;
    } else {
        o->oTransparency = 0;
        o->activeFlags &= ~ACTIVE_FLAG_DITHERED_ALPHA;
    }

    // Sounds
    omm_sound_stop(OMM_SOUND_EVENT_UNCAPTURE);
    sound_banks_enable(SEQ_PLAYER_SFX, soundBanksEnable);
    sound_banks_disable(SEQ_PLAYER_SFX, soundBanksDisable);

    // Cap
    struct Object *cap = obj_get_first_with_behavior(bhvOmmYoshiModeCap);
    if (!cap) {
        cap = obj_spawn_from_geo(o, marios_cap_geo, bhvOmmYoshiModeCap);
        cap->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    }
}

static void bhv_omm_yoshi_mode_cap_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (!p || !p->activeFlags || p->behavior != bhvOmmYoshiModeYoshi) {
        obj_mark_for_deletion(o);
        return;
    }
    omm_mario_capture_update_cap_object(p, p, o);
}

const BehaviorScript bhvOmmYoshiModeYoshi[] = {
    OBJ_TYPE_SPECIAL,
    BHV_LOAD_ANIMATIONS(oAnimations, yoshi_seg5_anims_05024100),
    BHV_ANIMATE(0),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_yoshi_mode_yoshi_update),
    BHV_END_LOOP()
};

const BehaviorScript bhvOmmYoshiModeCap[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_yoshi_mode_cap_update),
    BHV_END_LOOP()
};

//
// Spawner
//

struct Object *omm_obj_spawn_yoshi_mode_yoshi(struct Object *o) {
    struct Object *yoshi = obj_get_first_with_behavior(bhvOmmYoshiModeYoshi);
    if (!yoshi) {
        yoshi = obj_spawn_from_geo(o, yoshi_geo, bhvOmmYoshiModeYoshi);
        yoshi->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        obj_unload_all_with_behavior(bhvOmmYoshi);
    }
    return yoshi;
}

OMM_ROUTINE_UPDATE(omm_yoshi_mode_ending_cutscene) {
    if (gOmmGlobals->yoshiMode && gMarioObject != NULL && omm_is_ending_cutscene() && !omm_is_ending_cake_screen()) {
        omm_obj_spawn_yoshi_mode_yoshi(gMarioObject);
    }
}
