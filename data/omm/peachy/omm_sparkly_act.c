#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#undef do_cutscene_handler

//
// Star dance
//

typedef struct {
    s32 animID;
    s32 handState;
    s16 yawOffset;
    f32 yOffset;
    f32 animSpeed;
} OmmSparklyStarDanceAnimFrame;

static const OmmSparklyStarDanceAnimFrame OMM_SPARKLY_STAR_DANCE_ANIM_FRAMES[] = {
    // Animation, Hands, Yaw, PosY, Speed
    { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS, 0x0000,  0, 1 }, // 00
    { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS, 0x0000,  0, 1 }, // 01
    { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS, 0x0000,  0, 1 }, // 02
    { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS, 0x0080,  0, 1 }, // 03
    { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS, 0x0100,  0, 1 }, // 04
    { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS, 0x0200,  0, 1 }, // 05
    { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS, 0x0400,  0, 1 }, // 06
    { MARIO_ANIM_TRIPLE_JUMP_LAND, MARIO_HAND_FISTS, 0x0800,  0, 1 }, // 07
    { MARIO_ANIM_TWIRL,            MARIO_HAND_FISTS, 0x1000,  0, 1 }, // 08
    { MARIO_ANIM_TWIRL,            MARIO_HAND_FISTS, 0x2000,  0, 1 }, // 09
    { MARIO_ANIM_TWIRL,            MARIO_HAND_FISTS, 0x4000,  0, 1 }, // 10
    { MARIO_ANIM_TWIRL,            MARIO_HAND_FISTS, 0x6000,  0, 1 }, // 11
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0x8000,  0, 1 }, // 12
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0xA000,  0, 1 }, // 13
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0xC000,  0, 1 }, // 14
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0xE000,  0, 1 }, // 15
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0x0000,  0, 1 }, // 16
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0x2000,  0, 1 }, // 17
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0x4000,  0, 1 }, // 18
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0x6000,  0, 1 }, // 19
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0x8000,  0, 1 }, // 20
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0xA000,  0, 1 }, // 21
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0xC000,  0, 1 }, // 22
    { MARIO_ANIM_TWIRL,            MARIO_HAND_OPEN,  0xE000,  0, 1 }, // 23
    { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS, 0xF000,  0, 2 }, // 24
    { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS, 0xF800,  0, 2 }, // 25
    { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS, 0xFC00,  0, 2 }, // 26
    { MARIO_ANIM_TWIRL_LAND,       MARIO_HAND_FISTS, 0x0000,  0, 2 }, // 27
    { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS, 0x0000, 16, 2 }, // 28
    { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS, 0x0000, 24, 2 }, // 29
    { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS, 0x0000, 28, 2 }, // 30
    { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS, 0x0000, 30, 2 }, // 31
    { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS, 0x0000, 31, 2 }, // 32
    { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS, 0x0000, 32, 2 }, // 33
    { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS, 0x0000, 32, 2 }, // 34
    { MARIO_ANIM_SINGLE_JUMP,      MARIO_HAND_FISTS, 0x0000, 32, 2 }, // 35
};

static bool omm_sparkly_act_star_dance_update(struct MarioState *m) {

    // Enable time stop and spawn the celebration star
    if (m->actionTimer == 0) {
        m->faceAngle[1] = m->area->camera->yaw;
        vec3s_set(m->marioObj->oGfxAngle, 0, m->area->camera->yaw, 0);
        disable_background_sound();
        music_pause();
        omm_sound_play(OMM_SOUND_EVENT_SPARKLY_STAR_GET, gGlobalSoundArgs);
        omm_obj_spawn_sparkly_star_celebration(m->marioObj, gOmmSparklyMode);
        set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        m->marioObj->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        m->marioObj->oAnimID = -1;
        m->marioObj->oCurrAnim = NULL;
    }

    // Display the text box "Pink-Gold/Crystal/Nebula star"
    else if (m->actionTimer == 28) {
        s32 starIndex = omm_sparkly_get_star_index(gOmmSparklyMode, gCurrLevelNum, gCurrAreaIndex);
        ustr_t levelName, starName;
        omm_render_effect_you_got_a_star_begin(
            OMM_SPARKLY_TEXT_STAR[gOmmSparklyMode],
            omm_sparkly_get_level_name(levelName, gOmmSparklyMode, starIndex),
            omm_sparkly_get_star_name(starName, gOmmSparklyMode, starIndex)
        );
    }

    // Wa-hoo!
    else if (m->actionTimer == 29) {
        SFX(SOUND_MARIO_HERE_WE_GO);
        set_camera_shake_from_hit(SHAKE_GROUND_POUND);
    }

    // Resume action
    else if (m->actionTimer == 90) {
        obj_deactivate_all_with_behavior(bhvOmmSparklyStarCelebration);
        m->marioObj->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        enable_background_sound();
        music_resume();
        omm_render_effect_you_got_a_star_end();
        omm_health_fully_heal_mario(m, true);
        m->healCounter = OMM_BREATH_REFILL;
        return true;
    }

    // Fix the camera during the animation
    omm_mario_lock_camera(m, true);

    // Animation
    const OmmSparklyStarDanceAnimFrame *frame = &OMM_SPARKLY_STAR_DANCE_ANIM_FRAMES[min_s(m->actionTimer, 35)];
    ANM(frame->animID, frame->animSpeed);
    if (m->actionTimer >= 33) obj_anim_clamp_frame(m->marioObj, 0, 9); // Luigi's freaking flutter jump
    m->marioBodyState->handState = frame->handState;
    vec3f_set(m->marioObj->oGfxPos, m->pos[0], m->pos[1] + frame->yOffset, m->pos[2]);
    vec3s_set(m->marioObj->oGfxAngle, 0, m->faceAngle[1] - frame->yawOffset, 0);
    m->actionTimer++;
    return false;
}

s32 omm_sparkly_act_star_dance(struct MarioState *m) {
    if (omm_sparkly_act_star_dance_update(m)) {
        mario_set_forward_vel(m, 0.f);
        omm_mario_set_action(m, m->actionArg ? ACT_WATER_IDLE : ACT_FREEFALL, 0, 0);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

//
// Ending
//

#if OMM_GAME_IS_SM64
typedef struct {
    struct Object *obj;
    s32 anim;
    s32 jump;
} EndToadStruct;

static struct {
    s32 timer;
    u8 fade;
    ustr_t msg;
} sOmmEndToadDialog[1];

static void omm_sparkly_act_ending_update_toad_anim(EndToadStruct *toad) {
    if (toad->obj) {
        geo_obj_init_animation(&toad->obj->header.gfx, (struct Animation **) &toad_seg6_anims_0600FB58[toad->anim]);
        if (obj_anim_is_near_end(toad->obj) && (toad->anim == 0 || toad->anim == 2)) {
            toad->anim++;
        }
    }
}

static void omm_sparkly_act_ending_update_toad_jump(EndToadStruct *toad) {
    if (toad->obj && toad->jump > 0) {
        toad->obj->oPosY += 60.f * sins(0x1000 * toad->jump);
        toad->jump--;
    }
}

static void omm_sparkly_act_ending_set_toad_message(const char *msg, s16 duration) {
    const u8 *converted = omm_text_get_string_for_selected_player(omm_text_convert(msg, false));
    omm_text_copy(sOmmEndToadDialog->msg, sizeof(ustr_t), converted);
    sOmmEndToadDialog->timer = duration + 10; // + Fade in/out frames
    sOmmEndToadDialog->fade = 0;
}

static void omm_sparkly_act_ending_set_visual_pos(struct Object *o, f32 *value) {
    Vec3s t; find_mario_anim_flags_and_translation(o, o->oGfxAngle[1], t);
    f32 x = o->oGfxPos[0] + t[0];
    f32 y = o->oGfxPos[1] + 10.f;
    f32 z = o->oGfxPos[2] + t[2];
    struct Surface *floor;
    *value = find_floor(x, y, z, &floor);
}

static struct Object *omm_sparkly_act_ending_get_grand_star() {
    for_each_object_with_behavior(grandStar, bhvStaticObject) {
        if (obj_has_model(grandStar, MODEL_STAR) || obj_has_geo_layout(grandStar, omm_geo_star_0_opaque)) {
            return grandStar;
        }
    }
    return NULL;
}

static void omm_sparkly_act_ending_update_music(struct MarioState *m) {
#if OMM_GAME_IS_R96X
    if (m->actionArg == 3) {
        static u32 sConfigMasterVolume = 0;
        if (sConfigMasterVolume != 0 && configMasterVolume > 0) {
            configMasterVolume -= MIN(configMasterVolume, 1 + (gGlobalTimer & 1));
        }
        if (m->actionTimer == 44) {
            sConfigMasterVolume = configMasterVolume;
        } else if (m->actionTimer == 276) {
            r96_stop_jingle();
            configMasterVolume = sConfigMasterVolume;
            sConfigMasterVolume = 0;
        }
    } else if (m->actionArg == 6 && m->actionTimer == 460) {
        r96_play_jingle(R96_EVENT_CREDITS, 0.1, 1.0, 1500);
    }
#else
    if (m->actionArg == 3 && m->actionTimer == 44) {
        music_fade_out(SEQ_PLAYER_LEVEL, 192);
    } else if (m->actionArg == 6 && m->actionTimer == 460) {
        play_cutscene_music(SEQUENCE_ARGS(15, SEQ_EVENT_CUTSCENE_CREDITS));
    }
#endif
}

void omm_sparkly_ending_dialog() {
    if (gMarioState->action == ACT_END_PEACH_CUTSCENE && gOmmSparklyEnding == OMM_SPARKLY_ENDING_BAD) {
        if (sOmmEndToadDialog->timer > 0) {

            // Render text
            omm_render_create_dl_ortho_matrix();
            gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
            gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, sOmmEndToadDialog->fade);
            print_generic_string(get_str_x_pos_from_center(SCREEN_WIDTH / 2, sOmmEndToadDialog->msg, 10), 13, sOmmEndToadDialog->msg);
            gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

            // Fade in/out
            if (sOmmEndToadDialog->timer-- < 6) {
                sOmmEndToadDialog->fade -= 0x33;
            } else if (sOmmEndToadDialog->fade < 0xFF) {
                sOmmEndToadDialog->fade += 0x33;
            }
        }
    } else {
        sOmmEndToadDialog->timer = 0;
        do_cutscene_handler();
    }
}

#endif

// Sparkly Stars bad ending (Normal only)
// Toads are waiting for Mario, but Peach isn't here...
s32 omm_sparkly_act_ending_1(struct MarioState *m) {
#if OMM_GAME_IS_SM64
    static Vp sEndCutsceneVp = { { { 640, 480, 511, 0 }, { 640, 480, 511, 0 } } };
    static EndToadStruct sEndToadL = { NULL, 0, 0 };
    static EndToadStruct sEndToadR = { NULL, 0, 0 };

    // Update the grand star model
    // Use OMM_SPARKLY_STARS_MODE instead of gOmmSparklyMode, as the latter returns 0 during the ending cutscene
    struct Object *grandStar = omm_sparkly_act_ending_get_grand_star();
    if (grandStar && gOmmSparkly->grandStar) {
        grandStar->oGraphNode = geo_layout_to_graph_node(NULL, OMM_SPARKLY_STAR_GEO_OPAQUE[OMM_SPARKLY_STARS_MODE]);
        if (grandStar->curBhvCommand < bhvOmmSparklyGrandStarEnding || grandStar->curBhvCommand >= bhvOmmSparklyGrandStarEnding + 6) {
            grandStar->curBhvCommand = bhvOmmSparklyGrandStarEnding;
            grandStar->bhvStackIndex = 0;
        }
    }

    // Play a different cutscene during the 'bad' ending
    if (gOmmSparklyEnding == OMM_SPARKLY_ENDING_BAD) {
        omm_sparkly_act_ending_update_music(m);
        switch (m->actionArg) {
            case 0: {
                return OMM_MARIO_ACTION_RESULT_CONTINUE;
            } break;

            case 1: {
                return OMM_MARIO_ACTION_RESULT_CONTINUE;
            } break;

            case 2: {
                return OMM_MARIO_ACTION_RESULT_CONTINUE;
            } break;

            case 3: {
                switch (m->actionTimer) {
                    case 1: {
                        play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 14, 0xFF, 0xFF, 0xFF);
                    } break;

                    case 2: {
                        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
                    } break;

                    case 40: {
                        if (grandStar) obj_mark_for_deletion(grandStar);
                        sEndToadL.obj = spawn_object_abs_with_rot(m->marioObj, 0, MODEL_TOAD, bhvStaticObject, -100, 906, -1400, 0, 0, 0);
                        sEndToadR.obj = spawn_object_abs_with_rot(m->marioObj, 0, MODEL_TOAD, bhvStaticObject, +100, 906, -1400, 0, 0, 0);
                        sEndToadL.obj->oOpacity = 255;
                        sEndToadR.obj->oOpacity = 255;
                        sEndToadL.anim = 4;
                        sEndToadR.anim = 5;
                        sEndToadL.jump = 0;
                        sEndToadR.jump = 0;
                    } break;

                    case 44: {
                        play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 192, 0xFF, 0xFF, 0xFF);
                    } break;

                    case 276: {
                        m->actionArg++;
                    } break;
                }
            } break;

            case 4: {
                ANM(MARIO_ANIM_CREDITS_RETURN_FROM_LOOK_UP, 1.f);
                m->actionState = 0;
                m->actionTimer = 0;
                m->actionArg++;
            } break;

            case 5: {
                return OMM_MARIO_ACTION_RESULT_CONTINUE;
            } break;

            case 6: {
                switch (m->actionTimer) {

                    // Mario scratches his head
                    case 1: {
                        ANM(MARIO_ANIM_MISSING_CAP, 1.f);
                    } break;

                    // Toad message 1
                    case 60: {
                        ANM(MARIO_ANIM_FIRST_PERSON, 1.f);
                        sEndToadL.jump = 8;
                        omm_sound_play(OMM_SOUND_TOAD_1, sEndToadL.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_1, 30);
                    } break;

                    // Toad message 2
                    case 120: {
                        sEndToadR.jump = 8;
                        omm_sound_play(OMM_SOUND_TOAD_2, sEndToadR.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_2, 60);
                    } break;

                    // Toad message 3
                    case 210: {
                        omm_sound_play(OMM_SOUND_TOAD_3, sEndToadL.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_3, 60);
                    } break;

                    // Toad message 4, Mario falls on his butt
                    case 300: {
                        omm_sound_play(OMM_SOUND_TOAD_4, sEndToadR.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_4, 60);
                        ANM(MARIO_ANIM_DYING_FALL_OVER, 1.f);
                    } break;

                    // Toad message 5, Mario stands up, credits sequence
                    case 460: {
                        sEndToadR.jump = 8;
                        omm_sound_play(OMM_SOUND_TOAD_5, sEndToadR.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_5, 60);
                        ANM(MARIO_ANIM_WAKE_FROM_SLEEP, 1.f);
                    } break;

                    // Mario looks at Toads
                    case 480: {
                        ANM(MARIO_ANIM_FIRST_PERSON, 1.f);
                    } break;

                    // Toad message 6
                    case 540: {
                        sEndToadL.jump = 8;
                        omm_sound_play(OMM_SOUND_TOAD_6, sEndToadL.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_6, 60);
                    } break;

                    // Toad message 7
                    case 620: {
                        sEndToadR.jump = 8;
                        omm_sound_play(OMM_SOUND_TOAD_7, sEndToadR.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_7, 60);
                    } break;

                    // Right Toad double hop
                    case 628: {
                        sEndToadR.jump = 8;
                    } break;

                    // Toad message 8
                    case 710: {
                        sEndToadL.jump = 8;
                        omm_sound_play(OMM_SOUND_TOAD_8, sEndToadL.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_8, 60);
                    } break;

                    // Left Toad double hop
                    case 718: {
                        sEndToadL.jump = 8;
                    } break;

                    // Toad message 9
                    case 790: {
                        omm_sound_play(OMM_SOUND_TOAD_9, sEndToadL.obj->oCameraToObject);
                        omm_sparkly_act_ending_set_toad_message(OMM_TEXT_BAD_ENDING_TOAD_9, 60);
                    } break;

                    // Toads move, Mario look at the sky
                    case 800: {
                        sEndToadL.anim = 0;
                        sEndToadR.anim = 2;
                        ANM(MARIO_ANIM_CREDITS_START_WALK_LOOK_UP, 1.f);
                    } break;

                    // Mario moves
                    case 900: {
                        ANM(MARIO_ANIM_CREDITS_LOOK_BACK_THEN_RUN, 1.f);
                    } break;

                    // Camera starts moving up, birds spawn
                    case 960: {
                    } break;

                    // Credits
                    case 1200: {
                        level_trigger_warp(m, WARP_OP_CREDITS_NEXT);
                        gPaintingMarioYEntry = 1500;
                        m->actionState = 1;
                        m->actionArg = 12;
                        return OMM_MARIO_ACTION_RESULT_BREAK;
                    } break;
                }

                omm_sparkly_act_ending_set_visual_pos(m->marioObj, &m->marioObj->oGfxPos[1]);
                omm_sparkly_act_ending_set_visual_pos(sEndToadL.obj, &sEndToadL.obj->oPosY);
                omm_sparkly_act_ending_set_visual_pos(sEndToadR.obj, &sEndToadR.obj->oPosY);
                omm_sparkly_act_ending_update_toad_jump(&sEndToadL);
                omm_sparkly_act_ending_update_toad_jump(&sEndToadR);
            } break;
        }

        m->actionTimer++;
        omm_sparkly_act_ending_update_toad_anim(&sEndToadL);
        omm_sparkly_act_ending_update_toad_anim(&sEndToadR);
        sEndCutsceneVp.vp.vscale[0] = 640;
        sEndCutsceneVp.vp.vscale[1] = 360;
        sEndCutsceneVp.vp.vtrans[0] = 640;
        sEndCutsceneVp.vp.vtrans[1] = 480;
        override_viewport_and_clip(NULL, &sEndCutsceneVp, 0, 0, 0);
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
#endif
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}

// Sparkly Stars bad ending (Normal only)
// No one is waving at the camera. They're all looking for Peach...
s32 omm_sparkly_act_ending_2(struct MarioState *m) {
#if OMM_GAME_IS_SM64
    if (gOmmSparklyEnding == OMM_SPARKLY_ENDING_BAD) {
        action_init(0, 0, 0, 0, m->statusForCamera->cameraEvent = CAM_EVENT_START_END_WAVING;);
        stop_and_set_height_to_floor(m);
        m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        if (m->actionTimer++ >= 300) {
            level_trigger_warp(m, WARP_OP_CREDITS_END);
        }
        return OMM_MARIO_ACTION_RESULT_BREAK;
    }
#endif
    return OMM_MARIO_ACTION_RESULT_CONTINUE;
}
