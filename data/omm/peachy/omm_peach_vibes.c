#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_peach_vibe_increase(s32 inc) {
    gOmmData->mario->peach.vibeGauge = !OMM_CHEAT_PEACH_ENDLESS_VIBE_GAUGE * clamp_s(gOmmData->mario->peach.vibeGauge - inc, 0, OMM_PEACH_VIBE_GAUGE_MAX);
}

static void omm_peach_vibe_decrease(s32 dec) {
    gOmmData->mario->peach.vibeGauge = !OMM_CHEAT_PEACH_ENDLESS_VIBE_GAUGE * clamp_s(gOmmData->mario->peach.vibeGauge + dec, 0, OMM_PEACH_VIBE_GAUGE_MAX);
}

static bool omm_peach_vibe_check_type(struct MarioState *m, s32 vibeType) {
    switch (vibeType) {

        // Joy: not in Joy action, or underwater
        case OMM_PEACH_VIBE_TYPE_JOY: {
            if (gOmmData->mario->peach.vibeType == OMM_PEACH_VIBE_TYPE_JOY) {
                if (m->action != ACT_OMM_PEACH_VIBE_JOY_MOVE &&
                    m->action != ACT_OMM_PEACH_VIBE_JOY_FLY &&
                    m->action != ACT_OMM_PEACH_VIBE_JOY_ATTACK) {
                    return false;
                }
            }
            if ((m->action & ACT_FLAG_SWIMMING) || (m->action & ACT_FLAG_METAL_WATER)) {
                return false;
            }
        } break;

        // Rage: underwater
        case OMM_PEACH_VIBE_TYPE_RAGE: {
            if ((m->action & ACT_FLAG_SWIMMING) || (m->action & ACT_FLAG_METAL_WATER)) {
                return false;
            }
        } break;

        // Gloom: underwater
        case OMM_PEACH_VIBE_TYPE_GLOOM: {
            if ((m->action & ACT_FLAG_SWIMMING) || (m->action & ACT_FLAG_METAL_WATER)) {
                return false;
            }
        } break;

        // Calm: at max health/O2 and not in freezing water
        case OMM_PEACH_VIBE_TYPE_CALM: {
            if (!omm_world_is_frozen() || (m->action & ACT_GROUP_MASK) != ACT_GROUP_SUBMERGED) {
                if (OMM_MOVESET_ODYSSEY) {
                    if ((g1HPMode || omm_health_is_at_max(m)) && gOmmData->mario->state.o2 == 0) {
                        return false;
                    }
                } else {
                    if (g1HPMode || m->health >= 0x880) {
                        return false;
                    }
                }
            }
        } break;
    }
    return true;
}

static bool omm_peach_vibe_check(struct MarioState *m) {

    // Sparkly Stars Assist mode
    // Prevent Peach from activating Vibes if Vibes are not allowed
    if (OMM_SPARKLY_STARS_ASSIST && omm_ssc_data_flags(OMM_SSD_NO_VIBE)) {
        return false;
    }

    // Per vibe
    if (!omm_peach_vibe_check_type(m, gOmmData->mario->peach.vibeType)) {
        return false;
    }

    // Vibe gauge empty
    if (gOmmData->mario->peach.vibeGauge >= OMM_PEACH_VIBE_GAUGE_MAX) {
        return false;
    }

    // Direct damage
    if (m->hurtCounter != 0) {
        return false;
    }

    // Holding or riding an object
    if (m->heldObj || m->riddenObj) {
        return false;
    }

    // Intangible or invulnerable state
    if (m->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)) {
        return false;
    }

    // Incompatible actions
    if (m->action == ACT_FIRST_PERSON ||
        m->action == ACT_RIDING_HOOT ||
        m->action == ACT_CRAZY_BOX_BOUNCE ||
        m->action == ACT_PICKING_UP ||
        m->action == ACT_DIVE_PICKING_UP ||
        m->action == ACT_PLACING_DOWN ||
        m->action == ACT_THROWING ||
        m->action == ACT_HEAVY_THROW ||
        m->action == ACT_PICKING_UP_BOWSER ||
        m->action == ACT_HOLDING_BOWSER ||
        m->action == ACT_RELEASING_BOWSER) {
        return false;
    }

    // Ok
    return true;
}

static void omm_peach_vibe_update_gauge(struct MarioState *m) {

    // Refill the vibe gauge every time a recovery heart spins
    if (!(gTimeStopState & TIME_STOP_ENABLED)) {
        for_each_object_with_behavior(obj, bhvRecoveryHeart) {
            s32 recovery = max_s(0, obj->oAngleVelYaw - 400) / 200;
            omm_peach_vibe_increase(recovery);
        }
    }

    // Update the vibe gauge and apply the Calm effects
    switch (gOmmData->mario->peach.vibeType) {

        // No active vibe: refill the vibe gauge
        case OMM_PEACH_VIBE_TYPE_NONE: {
            if (!omm_ssc_data_flags(OMM_SSD_NO_VIBE_AUTO_REFILL) || !OMM_SSC_IS_OK) {
                if (omm_mario_is_idling(m)) {
                    omm_peach_vibe_increase(OMM_PEACH_VIBE_GAUGE_IDLE_INC);
                } else {
                    omm_peach_vibe_increase(OMM_PEACH_VIBE_GAUGE_INC);
                }
            }
        } break;

        // Joy: drains the vibe gauge when flying
        case OMM_PEACH_VIBE_TYPE_JOY: {
            switch (m->action) {
                case ACT_OMM_PEACH_VIBE_JOY_MOVE:   omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_JOY_MOVE_DEC);   break;
                case ACT_OMM_PEACH_VIBE_JOY_FLY:    omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_JOY_FLY_DEC);    break;
                case ACT_OMM_PEACH_VIBE_JOY_ATTACK: omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_JOY_ATTACK_DEC); break;
            }
        } break;

        // Rage
        case OMM_PEACH_VIBE_TYPE_RAGE: {
            omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_RAGE_DEC);
        } break;

        // Gloom
        case OMM_PEACH_VIBE_TYPE_GLOOM: {
            omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_GLOOM_DEC);
        } break;

        // Calm: drains the vibe gauge to restore HP and O2
        // Also prevents the effect of frozen water
        case OMM_PEACH_VIBE_TYPE_CALM: {
            if (OMM_MOVESET_ODYSSEY) {

                // Health
                if (!g1HPMode && !omm_health_is_at_max(m)) {
                    if ((gOmmData->mario->peach.vibeTimer % 60) == 0) {
                        omm_health_heal_mario(m);
                    }
                    omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_CALM_HP_DEC);
                }

                // Air (if the value is near 0, set it to 0 without draining the Vibe gauge)
                if (gOmmData->mario->state.o2 != 0) {
                    if (gOmmData->mario->state.o2 < 8) {
                        gOmmData->mario->state.o2 = 0;
                    } else {
                        gOmmData->mario->state.o2 -= 6;
                        omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_CALM_O2_DEC);
                    }
                }
            } else {

                // Health (if the value is near 0x880, set it to 0x880 without draining the Vibe gauge)
                if (!g1HPMode && m->health < 0x880) {
                    if (m->health >= 0x878) {
                        m->health = 0x880;
                    } else {
                        m->health += 6;
                        omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_CALM_HP_DEC);
                    }

                    // Air (if the value is near 0x880, set it to 0x880 without draining the Vibe gauge)
                    if ((m->health < 0x880) && ((m->action & ACT_FLAG_SWIMMING) || (m->input & INPUT_IN_POISON_GAS))) {
                        if (m->health >= 0x878) {
                            m->health = 0x880;
                        } else {
                            m->health += 4;
                            omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_CALM_O2_DEC);
                        }
                    }
                }
            }

            // Freezing water
            if (omm_world_is_frozen() && (m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {
                omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_CALM_FREEZE_DEC);
            }
        } break;
    }
}

static s32 omm_peach_vibe_handle_inputs() {
    if (gOmmData->mario->peach.vibeTimer > OMM_PEACH_VIBE_COOLDOWN) {
        if (gPlayer1Controller->buttonPressed & Y_BUTTON) {
            switch (gPlayer1Controller->buttonDown & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
                case U_JPAD: return (gOmmData->mario->peach.vibeGauge < OMM_PEACH_VIBE_GAUGE_LIMIT) * OMM_PEACH_VIBE_TYPE_JOY;
                case D_JPAD: return (gOmmData->mario->peach.vibeGauge < OMM_PEACH_VIBE_GAUGE_LIMIT) * OMM_PEACH_VIBE_TYPE_CALM;
                case L_JPAD: return (gOmmData->mario->peach.vibeGauge < OMM_PEACH_VIBE_GAUGE_LIMIT) * OMM_PEACH_VIBE_TYPE_GLOOM;
                case R_JPAD: return (gOmmData->mario->peach.vibeGauge < OMM_PEACH_VIBE_GAUGE_LIMIT) * OMM_PEACH_VIBE_TYPE_RAGE;
                default:     return -1;
            }
        }
    }
    return 0;
}

static bool omm_peach_vibe_toggle(struct MarioState *m, s32 vibeAction) {

    // Activation or switch + end cap power-up
    if (vibeAction > 0 && gOmmData->mario->peach.vibeType != vibeAction && omm_peach_vibe_check_type(m, vibeAction)) {
        gOmmData->mario->peach.vibeType = vibeAction;
        gOmmData->mario->peach.vibeTimer = 0;
        omm_sound_play(OMM_SOUND_EFFECT_PEACH_VIBE, gGlobalSoundArgs);
        switch (vibeAction) {
            case OMM_PEACH_VIBE_TYPE_JOY: {
                omm_sound_play(OMM_SOUND_PEACH_VIBE_JOY, m->marioObj->oCameraToObject);
                omm_mario_set_action(m, ACT_OMM_PEACH_VIBE_JOY_MOVE, 0, 0);
                gOmmData->mario->spin.yaw = m->faceAngle[1];
            } break;

            case OMM_PEACH_VIBE_TYPE_RAGE: {
                omm_sound_play(OMM_SOUND_PEACH_VIBE_RAGE, m->marioObj->oCameraToObject);
            } break;

            case OMM_PEACH_VIBE_TYPE_GLOOM: {
                omm_sound_play(OMM_SOUND_PEACH_VIBE_GLOOM, m->marioObj->oCameraToObject);
            } break;

            case OMM_PEACH_VIBE_TYPE_CALM: {
                omm_sound_play(OMM_SOUND_PEACH_VIBE_CALM, m->marioObj->oCameraToObject);
            } break;
        }
        if (m->flags & (MARIO_METAL_CAP | MARIO_WING_CAP | MARIO_VANISH_CAP)) {
            m->capTimer = 1;
        }
        return true;
    }

    // Deactivation
    if (vibeAction < 0 && gOmmData->mario->peach.vibeType != OMM_PEACH_VIBE_TYPE_NONE) {
        gOmmData->mario->peach.vibeType = OMM_PEACH_VIBE_TYPE_NONE;
        gOmmData->mario->peach.vibeTimer = 0;
        return true;
    }
    return false;
}

static void omm_peach_vibe_update_effects(struct MarioState *m) {
    switch (gOmmData->mario->peach.vibeType) {

        // Joy
        case OMM_PEACH_VIBE_TYPE_JOY: {
            omm_spawn_peach_vibe_joy_tornado(m->marioObj);
        } break;

        // Rage
        case OMM_PEACH_VIBE_TYPE_RAGE: {
            omm_spawn_peach_vibe_rage_aura(m->marioObj);
            if (omm_mario_is_ground_pound_landing(m)) {

                // If Peach lands on an object...
                if (m->floor && m->floor->object) {

                    // It's a wooden post, sink it in one hit
                    if (m->floor->object->behavior == bhvWoodenPost) {
                        if (!m->floor->object->oWoodenPostMarioPounding) {
                            f32 prevOffsetY = m->floor->object->oWoodenPostOffsetY;
                            m->floor->object->oWoodenPostOffsetY = -190.f;
                            m->pos[1] += (m->floor->object->oWoodenPostOffsetY - prevOffsetY);
                            m->marioObj->oPosY = m->pos[1];
                            obj_load_collision_model(m->floor->object);
                        }
                    }

                    // It's a destructible object, destroy it
                    else {
                        omm_obj_process_one_surface_interaction(m->marioObj, m->floor->object, OBJ_INT_PRESET_PEACH_VIBE_RAGE_GROUND_POUND_LAND);
                    }
                }

                // Release a fire shockwave, but drains the Vibe gauge
                if (m->actionTimer == 0) {
                    omm_spawn_peach_vibe_rage_shockwave(m->marioObj);
                    set_environmental_camera_shake(SHAKE_ENV_BOWSER_THROW_BOUNCE);
                    omm_peach_vibe_decrease(OMM_PEACH_VIBE_GAUGE_RAGE_SHOCKWAVE_DEC);
                    m->actionTimer = 1;
                }
            }
        } break;

        // Gloom
        case OMM_PEACH_VIBE_TYPE_GLOOM: {
            omm_spawn_peach_vibe_gloom_aura(m->marioObj);
            omm_spawn_peach_vibe_gloom_tear(m->marioObj);
        } break;

        // Calm
        case OMM_PEACH_VIBE_TYPE_CALM: {
            omm_spawn_peach_vibe_calm_aura(m->marioObj);
        } break;
    }
}

//
// Public
//

bool omm_peach_vibe_is_active() {
    return gOmmData->mario->peach.vibeType != OMM_PEACH_VIBE_TYPE_NONE;
}

bool omm_peach_vibe_is_joy() {
    return gOmmData->mario->peach.vibeType == OMM_PEACH_VIBE_TYPE_JOY;
}

bool omm_peach_vibe_is_rage() {
    return gOmmData->mario->peach.vibeType == OMM_PEACH_VIBE_TYPE_RAGE;
}

bool omm_peach_vibe_is_gloom() {
    return gOmmData->mario->peach.vibeType == OMM_PEACH_VIBE_TYPE_GLOOM;
}

bool omm_peach_vibe_is_calm() {
    return gOmmData->mario->peach.vibeType == OMM_PEACH_VIBE_TYPE_CALM;
}

bool omm_peach_vibe_activate(struct MarioState *m, s32 vibe) {
    return omm_peach_vibe_check(m) && omm_peach_vibe_toggle(m, vibe);
}

bool omm_peach_vibe_deactivate(struct MarioState *m) {
    return omm_peach_vibe_toggle(m, -1);
}

void omm_peach_vibe_update(struct MarioState *m) {
    gOmmData->mario->peach.vibeTimer++;
    omm_peach_vibe_update_gauge(m);
    if (omm_peach_vibe_check(m)) {
        omm_peach_vibe_toggle(m, omm_peach_vibe_handle_inputs());
    } else {
        omm_peach_vibe_toggle(m, -1);
    }
    omm_peach_vibe_update_effects(m);
}

//
// Music variations caused by Vibes
//

#if !OMM_GAME_IS_R96A

static void update_sequence_channel(struct SequenceChannel *channel) {
    static f32 sSeqChannelInfoBuffer[0x80][OMM_PEACH_VIBE_TYPE_COUNT][2];
    static s32 sSeqChannelInfoIndex = 0;

    // Init sequence channel info
    if (!(channel->updatesPerFrameUnused & 0x80)) {
        f32 f = channel->freqScale;
        f32 v = channel->volumeScale;
        if (channel->bankId >= 0x26) { // Streamed audio
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_NONE ][0] = f;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_NONE ][1] = v;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_JOY  ][0] = f * OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_JOY;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_JOY  ][1] = v * OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_JOY;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_RAGE ][0] = f * OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_RAGE;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_RAGE ][1] = v * OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_RAGE;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_GLOOM][0] = f * OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_GLOOM;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_GLOOM][1] = v * OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_GLOOM;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_CALM ][0] = f * OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_CALM;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_CALM ][1] = v * OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_CALM;
        } else {
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_NONE ][0] = f;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_NONE ][1] = v;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_JOY  ][0] = f * OMM_PEACH_VIBE_MUSIC_FREQ_MOD_JOY;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_JOY  ][1] = v * OMM_PEACH_VIBE_MUSIC_VOLUME_MOD_JOY;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_RAGE ][0] = f * OMM_PEACH_VIBE_MUSIC_FREQ_MOD_RAGE;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_RAGE ][1] = v * OMM_PEACH_VIBE_MUSIC_VOLUME_MOD_RAGE;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_GLOOM][0] = f * OMM_PEACH_VIBE_MUSIC_FREQ_MOD_GLOOM;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_GLOOM][1] = v * OMM_PEACH_VIBE_MUSIC_VOLUME_MOD_GLOOM;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_CALM ][0] = f * OMM_PEACH_VIBE_MUSIC_FREQ_MOD_CALM;
            sSeqChannelInfoBuffer[sSeqChannelInfoIndex][OMM_PEACH_VIBE_TYPE_CALM ][1] = v * OMM_PEACH_VIBE_MUSIC_VOLUME_MOD_CALM;
        }
        channel->updatesPerFrameUnused = (u8) (sSeqChannelInfoIndex | 0x80);
        sSeqChannelInfoIndex = (sSeqChannelInfoIndex + 1) & 0x7F;
    }

    // Update sequence channel values
    channel->freqScale   = sSeqChannelInfoBuffer[(s32) (channel->updatesPerFrameUnused & 0x7F)][gOmmData->mario->peach.vibeType][0];
    channel->volumeScale = sSeqChannelInfoBuffer[(s32) (channel->updatesPerFrameUnused & 0x7F)][gOmmData->mario->peach.vibeType][1];
}

static bool is_sequence_in_list(struct SequenceChannel *channel, s32 *seqList) {
    if (seqList && channel->seqPlayer) {
        s32 seqId = channel->seqPlayer->seqId;
        for (s32 *head = seqList; *head != -1; ++head) {
            if (*head == seqId) {
                return true;
            }
        }
        return false;
    }
    return true;
}

static void omm_peach_vibe_update_sequences(s32 seqPlayer, s32 *seqList) {
    for (s32 i = 0; i != CHANNELS_MAX; ++i) {
        struct SequenceChannel *channel = gSequencePlayers[seqPlayer].channels[i];
        if (channel && channel != &gSequenceChannelNone && is_sequence_in_list(channel, seqList)) {
            update_sequence_channel(channel);
        }
    }
}

OMM_ROUTINE_PRE_RENDER(omm_peach_vibe_update_music) {
    if (OMM_PLAYER_IS_PEACH) {
        omm_peach_vibe_update_sequences(SEQ_PLAYER_LEVEL, NULL);
        omm_peach_vibe_update_sequences(SEQ_PLAYER_ENV, OMM_ARRAY_OF(s32) { SEQ_EVENT_PIRANHA_PLANT, SEQ_EVENT_MERRY_GO_ROUND, SEQ_EVENT_BOSS, SEQ_EVENT_ENDLESS_STAIRS, -1 });
    }
}

#endif
