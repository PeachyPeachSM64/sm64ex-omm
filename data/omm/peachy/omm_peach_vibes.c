#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static bool omm_peach_vibe_check_type(struct MarioState *m, s32 vibeType) {
    switch (vibeType) {

        // Joy: deactivate if not in Joy action, or underwater
        case OMM_PEACH_VIBE_TYPE_JOY: {
            if (gOmmPeach->vibeType == OMM_PEACH_VIBE_TYPE_JOY) {
                if (m->action != ACT_OMM_PEACH_VIBE_JOY_MOVE &&
                    m->action != ACT_OMM_PEACH_VIBE_JOY_FLY &&
                    m->action != ACT_OMM_PEACH_VIBE_JOY_ATTACK) {
                    return false;
                }
            }
            if (m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)) {
                return false;
            }
        } break;

        // Rage: deactivate if underwater
        case OMM_PEACH_VIBE_TYPE_RAGE: {
            if (m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)) {
                return false;
            }
        } break;

        // Gloom: deactivate if underwater
        case OMM_PEACH_VIBE_TYPE_GLOOM: {
            if (m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)) {
                return false;
            }
        } break;

        // Calm: deactivate at max health/breath and not in (cold) water/poison gas
        case OMM_PEACH_VIBE_TYPE_CALM: {
            if (OMM_CHEAT_GOD_MODE) {
                return false;
            }
            if (m->input & INPUT_IN_POISON_GAS) {
                return true;
            }
            if ((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED && (omm_world_is_flooded() || omm_world_is_cold() || m->pos[1] < m->waterLevel - OMM_MARIO_WATER_BREATH_RECOVERY_OFFSET)) {
                return true;
            }
            if (omm_health_is_at_max(m) && (gOmmMario->state.breath == 0 || OMM_MOVESET_CLASSIC)) {
                return false;
            }
        } break;
    }
    return true;
}

static bool omm_peach_vibe_check(struct MarioState *m) {

    // Per vibe
    if (!omm_peach_vibe_check_type(m, gOmmPeach->vibeType)) {
        return false;
    }

    // Vibe gauge empty
    if (gOmmPeach->vibeGauge >= OMM_PEACH_VIBE_GAUGE_MAX) {
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
            omm_peach_vibe_increase(m, recovery);
        }
    }

    // Update the vibe gauge and apply the Calm effects
    switch (gOmmPeach->vibeType) {

        // No active vibe: refill the vibe gauge
        case OMM_PEACH_VIBE_TYPE_NONE: {
            if (!omm_sparkly_context_get_data(OMM_SPARKLY_DATA_NO_VIBE_AUTO_REFILL) || !OMM_SPARKLY_STATE_IS_OK) {
                if (omm_mario_is_idling(m)) {
                    omm_peach_vibe_increase(m, OMM_PEACH_VIBE_GAUGE_IDLE_INC);
                } else {
                    omm_peach_vibe_increase(m, OMM_PEACH_VIBE_GAUGE_INC);
                }
            }
        } break;

        // Joy: drains the vibe gauge when flying
        case OMM_PEACH_VIBE_TYPE_JOY: {
            switch (m->action) {
                case ACT_OMM_PEACH_VIBE_JOY_MOVE:   omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_JOY_MOVE_DEC);   break;
                case ACT_OMM_PEACH_VIBE_JOY_FLY:    omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_JOY_FLY_DEC);    break;
                case ACT_OMM_PEACH_VIBE_JOY_ATTACK: omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_JOY_ATTACK_DEC); break;
            }
        } break;

        // Rage
        case OMM_PEACH_VIBE_TYPE_RAGE: {
            omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_RAGE_DEC);
        } break;

        // Gloom
        case OMM_PEACH_VIBE_TYPE_GLOOM: {
            omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_GLOOM_DEC);
        } break;

        // Calm: drains the vibe gauge to restore health and breath
        // Also prevents the effect of frozen water
        case OMM_PEACH_VIBE_TYPE_CALM: {
            if (OMM_MOVESET_ODYSSEY) {

                // Health
                if (!omm_health_is_at_max(m)) {
                    if ((gOmmPeach->vibeTimer % 60) == 0) {
                        omm_health_heal_mario(m, true);
                    }
                    omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_CALM_HEALTH_DEC);
                }

                // Breath
                if (gOmmMario->state.breath != 0) {
                    s32 recovery = min_s(6, gOmmMario->state.breath);
                    gOmmMario->state.breath -= recovery;
                    omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_CALM_BREATH_DEC * relerp_0_1_f(recovery, 1, 6, 0.5f, 1.f));
                }

            } else if (!omm_health_is_at_max(m)) {

                // Health
                if (m->health < OMM_HEALTH_CLASSIC_MAX - 6) {
                    m->health += 6;
                    omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_CALM_HEALTH_DEC);
                }

                // Breath in water/poison gas
                else if ((m->action & ACT_FLAG_SWIMMING) || (m->input & INPUT_IN_POISON_GAS)) {
                    s32 recovery = OMM_HEALTH_CLASSIC_MAX - m->health;
                    m->health = OMM_HEALTH_CLASSIC_MAX;
                    omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_CALM_BREATH_DEC * relerp_0_1_f(recovery, 1, 6, 0.5f, 1.f));
                }

                // Max health
                else {
                    m->health = OMM_HEALTH_CLASSIC_MAX;
                }
            }

            // Cold water
            if (omm_world_is_cold() && (m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {
                omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_CALM_FREEZE_DEC);
            }
        } break;
    }
}

static s32 omm_peach_vibe_handle_inputs() {
    if (gOmmPeach->vibeTimer > OMM_PEACH_VIBE_COOLDOWN) {
        if (gPlayer1Controller->buttonPressed & Y_BUTTON) {
            switch (gPlayer1Controller->buttonDown & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
                case U_JPAD: return (gOmmPeach->vibeGauge < OMM_PEACH_VIBE_GAUGE_LIMIT) * OMM_PEACH_VIBE_TYPE_JOY;
                case D_JPAD: return (gOmmPeach->vibeGauge < OMM_PEACH_VIBE_GAUGE_LIMIT) * OMM_PEACH_VIBE_TYPE_CALM;
                case L_JPAD: return (gOmmPeach->vibeGauge < OMM_PEACH_VIBE_GAUGE_LIMIT) * OMM_PEACH_VIBE_TYPE_GLOOM;
                case R_JPAD: return (gOmmPeach->vibeGauge < OMM_PEACH_VIBE_GAUGE_LIMIT) * OMM_PEACH_VIBE_TYPE_RAGE;
                default:     return -1;
            }
        }
    }
    return 0;
}

static bool omm_peach_vibe_toggle(struct MarioState *m, s32 vibeAction) {

    // Activation or switch + end cap power-up
    if (vibeAction > 0 && gOmmPeach->vibeType != vibeAction && !omm_mario_is_milk(m)) {
        if (!gOmmAllow->vibes || (!gOmmAllow->joyVibe && vibeAction == OMM_PEACH_VIBE_TYPE_JOY)) {
            play_buzz_sound();
            return false;
        }
        if (omm_peach_vibe_check_type(m, vibeAction)) {
            gOmmPeach->vibeType = vibeAction;
            gOmmPeach->vibeTimer = 0;
            gOmmPeach->vibeGfxTimer = gGlobalTimer;
            omm_sound_play(OMM_SOUND_EFFECT_PEACH_VIBE, gGlobalSoundArgs);
            switch (vibeAction) {
                case OMM_PEACH_VIBE_TYPE_JOY: {
                    omm_sound_play(OMM_SOUND_PEACH_VIBE_JOY, m->marioObj->oCameraToObject);
                    omm_mario_set_action(m, ACT_OMM_PEACH_VIBE_JOY_MOVE, 0, 0);
                    gOmmMario->spin.yaw = m->faceAngle[1];
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
                omm_mario_unset_cap(m);
            }
            return true;
        }
    }

    // Deactivation
    if (vibeAction < 0 && gOmmPeach->vibeType != OMM_PEACH_VIBE_TYPE_NONE) {
        if (m->action == ACT_START_SLEEPING || m->action == ACT_SLEEPING) {
            omm_mario_set_action(m, ACT_WAKING_UP, 0, 0);
        }
        gOmmPeach->vibeType = OMM_PEACH_VIBE_TYPE_NONE;
        gOmmPeach->vibeTimer = 0;
        gOmmPeach->vibeGfxTimer = gGlobalTimer;
        return true;
    }
    return false;
}

static void omm_peach_vibe_update_effects(struct MarioState *m) {
    switch (gOmmPeach->vibeType) {

        // Joy
        case OMM_PEACH_VIBE_TYPE_JOY: {
            omm_obj_spawn_peach_vibe_joy_tornado(m->marioObj);
        } break;

        // Rage
        case OMM_PEACH_VIBE_TYPE_RAGE: {
            omm_obj_spawn_peach_vibe_rage_aura(m->marioObj);
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
                    omm_obj_spawn_peach_vibe_rage_shockwave(m->marioObj)->oPosY = m->pos[1];
                    set_environmental_camera_shake(SHAKE_ENV_BOWSER_THROW_BOUNCE);
                    omm_peach_vibe_decrease(m, OMM_PEACH_VIBE_GAUGE_RAGE_SHOCKWAVE_DEC);
                    m->actionTimer = 1;
                }
            }
        } break;

        // Gloom
        case OMM_PEACH_VIBE_TYPE_GLOOM: {
            omm_obj_spawn_peach_vibe_gloom_aura(m->marioObj);
            omm_obj_spawn_peach_vibe_gloom_tear(m->marioObj);
        } break;

        // Calm
        case OMM_PEACH_VIBE_TYPE_CALM: {
            omm_obj_spawn_peach_vibe_calm_aura(m->marioObj);
        } break;
    }
}

//
// Public
//

bool omm_peach_vibe_is_active() {
    return gOmmPeach->vibeType != OMM_PEACH_VIBE_TYPE_NONE;
}

bool omm_peach_vibe_is_joy() {
    return gOmmPeach->vibeType == OMM_PEACH_VIBE_TYPE_JOY;
}

bool omm_peach_vibe_is_rage() {
    return gOmmPeach->vibeType == OMM_PEACH_VIBE_TYPE_RAGE;
}

bool omm_peach_vibe_is_gloom() {
    return gOmmPeach->vibeType == OMM_PEACH_VIBE_TYPE_GLOOM;
}

bool omm_peach_vibe_is_calm() {
    return gOmmPeach->vibeType == OMM_PEACH_VIBE_TYPE_CALM;
}

bool omm_peach_vibe_activate(struct MarioState *m, s32 vibe) {
    return omm_peach_vibe_check(m) && omm_peach_vibe_toggle(m, vibe);
}

bool omm_peach_vibe_deactivate(struct MarioState *m) {
    return omm_peach_vibe_toggle(m, -1);
}

void omm_peach_vibe_increase(struct MarioState *m, u32 inc) {
    if (OMM_CHEAT_PEACH_ENDLESS_VIBE_GAUGE) {
        gOmmPeach->vibeGauge = 0;
    } else if (gOmmPeach->vibeGauge > 0) {
        gOmmPeach->vibeGauge = clamp_s(gOmmPeach->vibeGauge - (s32) inc, 0, OMM_PEACH_VIBE_GAUGE_MAX);
        if (!omm_peach_vibe_is_active() && gOmmPeach->vibeGauge == 0) {
            omm_obj_spawn_peach_vibe_aura(m->marioObj, true);
        }
    }
}

void omm_peach_vibe_decrease(UNUSED struct MarioState *m, u32 dec) {
    if (OMM_CHEAT_PEACH_ENDLESS_VIBE_GAUGE) {
        gOmmPeach->vibeGauge = 0;
    } else if (gOmmPeach->vibeGauge < OMM_PEACH_VIBE_GAUGE_MAX) {
        gOmmPeach->vibeGauge = clamp_s(gOmmPeach->vibeGauge + (s32) dec, 0, OMM_PEACH_VIBE_GAUGE_MAX);
    }
}

void omm_peach_vibe_update(struct MarioState *m) {
    gOmmPeach->vibeTimer++;
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

#if !OMM_GAME_IS_R96X

static const struct { f32 freq; f32 volume; } OMM_PEACH_VIBE_MUSIC_MODIFIERS[2][OMM_PEACH_VIBE_TYPE_COUNT] = {
    {   // Seq audio
        [OMM_PEACH_VIBE_TYPE_NONE]  = { .freq = OMM_PEACH_VIBE_MUSIC_SEQ_FREQ_MOD_NONE,   .volume = OMM_PEACH_VIBE_MUSIC_SEQ_VOLUME_MOD_NONE   },
        [OMM_PEACH_VIBE_TYPE_JOY]   = { .freq = OMM_PEACH_VIBE_MUSIC_SEQ_FREQ_MOD_JOY,    .volume = OMM_PEACH_VIBE_MUSIC_SEQ_VOLUME_MOD_JOY    },
        [OMM_PEACH_VIBE_TYPE_RAGE]  = { .freq = OMM_PEACH_VIBE_MUSIC_SEQ_FREQ_MOD_RAGE,   .volume = OMM_PEACH_VIBE_MUSIC_SEQ_VOLUME_MOD_RAGE   },
        [OMM_PEACH_VIBE_TYPE_GLOOM] = { .freq = OMM_PEACH_VIBE_MUSIC_SEQ_FREQ_MOD_GLOOM,  .volume = OMM_PEACH_VIBE_MUSIC_SEQ_VOLUME_MOD_GLOOM  },
        [OMM_PEACH_VIBE_TYPE_CALM]  = { .freq = OMM_PEACH_VIBE_MUSIC_SEQ_FREQ_MOD_CALM,   .volume = OMM_PEACH_VIBE_MUSIC_SEQ_VOLUME_MOD_CALM   },
    },
    {   // Streamed audio
        [OMM_PEACH_VIBE_TYPE_NONE]  = { .freq = OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_NONE,  .volume = OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_NONE  },
        [OMM_PEACH_VIBE_TYPE_JOY]   = { .freq = OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_JOY,   .volume = OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_JOY   },
        [OMM_PEACH_VIBE_TYPE_RAGE]  = { .freq = OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_RAGE,  .volume = OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_RAGE  },
        [OMM_PEACH_VIBE_TYPE_GLOOM] = { .freq = OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_GLOOM, .volume = OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_GLOOM },
        [OMM_PEACH_VIBE_TYPE_CALM]  = { .freq = OMM_PEACH_VIBE_MUSIC_STRM_FREQ_MOD_CALM,  .volume = OMM_PEACH_VIBE_MUSIC_STRM_VOLUME_MOD_CALM  },
    },
};

static void omm_peach_vibe_update_sequence_channel(struct SequenceChannel *channel) {
    static struct {
        struct {
            f32 freq;
            f32 volume;
            s32 vibeType;
        } buffer[0x80][1];
        s32 index;
    } sOmmPeachVibeSeqChannelInfo[1];

    s32 index = (channel->updatesPerFrameUnused & 0x7F);
    bool streamed = (channel->bankId >= 0x26);

    // Init sequence channel info
    if (!(channel->updatesPerFrameUnused & 0x80)) {
        index = sOmmPeachVibeSeqChannelInfo->index = (sOmmPeachVibeSeqChannelInfo->index + 1) & 0x7F;
        sOmmPeachVibeSeqChannelInfo->buffer[index]->freq = channel->freqScale;
        sOmmPeachVibeSeqChannelInfo->buffer[index]->volume = channel->volumeScale;
        sOmmPeachVibeSeqChannelInfo->buffer[index]->vibeType = OMM_PEACH_VIBE_TYPE_NONE;
        channel->updatesPerFrameUnused = (u8) (index | 0x80);
        // omm_debug_log("Sequence channel inited - INDEX: %02X | ADDR: %016llX\n",, (u8) index, (uintptr_t) channel);
    }

    // If freq changed, re-apply modifier
    if (channel->freqScale != sOmmPeachVibeSeqChannelInfo->buffer[index]->freq) {
        channel->freqScale *= OMM_PEACH_VIBE_MUSIC_MODIFIERS[streamed][gOmmPeach->vibeType].freq;
        // omm_debug_log("Freq scale changed - INDEX: %02X | OLD: %.4f | NEW: %.4f\n",, (u8) index, sOmmPeachVibeSeqChannelInfo->buffer[index]->freq, channel->freqScale);
        sOmmPeachVibeSeqChannelInfo->buffer[index]->freq = channel->freqScale;
    }

    // If volume changed, re-apply modifier
    if (channel->volumeScale != sOmmPeachVibeSeqChannelInfo->buffer[index]->volume) {
        channel->volumeScale *= OMM_PEACH_VIBE_MUSIC_MODIFIERS[streamed][gOmmPeach->vibeType].volume;
        // omm_debug_log("Volume scale changed - INDEX: %02X | OLD: %.4f | NEW: %.4f\n",, (u8) index, sOmmPeachVibeSeqChannelInfo->buffer[index]->volume, channel->volumeScale);
        sOmmPeachVibeSeqChannelInfo->buffer[index]->volume = channel->volumeScale;
    }

    // If vibe type changed, revert old modifiers, apply new modifiers
    s32 prevVibeType = sOmmPeachVibeSeqChannelInfo->buffer[index]->vibeType;
    if (gOmmPeach->vibeType != prevVibeType) {
        channel->freqScale /= OMM_PEACH_VIBE_MUSIC_MODIFIERS[streamed][prevVibeType].freq;
        channel->freqScale *= OMM_PEACH_VIBE_MUSIC_MODIFIERS[streamed][gOmmPeach->vibeType].freq;
        channel->volumeScale /= OMM_PEACH_VIBE_MUSIC_MODIFIERS[streamed][prevVibeType].volume;
        channel->volumeScale *= OMM_PEACH_VIBE_MUSIC_MODIFIERS[streamed][gOmmPeach->vibeType].volume;
        sOmmPeachVibeSeqChannelInfo->buffer[index]->freq = channel->freqScale;
        sOmmPeachVibeSeqChannelInfo->buffer[index]->volume = channel->volumeScale;
        sOmmPeachVibeSeqChannelInfo->buffer[index]->vibeType = gOmmPeach->vibeType;
        // omm_debug_log("Vibe type changed - INDEX: %02X | OLD: %d | NEW: %d\n",, (u8) index, prevVibeType, gOmmPeach->vibeType);
    }
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
            omm_peach_vibe_update_sequence_channel(channel);
        }
    }
}

OMM_ROUTINE_PRE_RENDER(omm_peach_vibe_update_music) {
    if (OMM_PLAYER_IS_PEACH) {
        omm_peach_vibe_update_sequences(SEQ_PLAYER_LEVEL, NULL);
        omm_peach_vibe_update_sequences(SEQ_PLAYER_ENV, array_of(s32) { SEQ_EVENT_PIRANHA_PLANT, SEQ_EVENT_MERRY_GO_ROUND, SEQ_EVENT_BOSS, SEQ_EVENT_ENDLESS_STAIRS, -1 });
    }
}

#endif
