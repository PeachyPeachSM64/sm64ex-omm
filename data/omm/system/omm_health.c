#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

s32 omm_health_odyssey(s32 health) {
    s32 healthMin = OMM_HEALTH_ODYSSEY_DEAD;
    s32 healthMax = omm_health_get_max(health);
    return clamp_s((((health - OMM_HEALTH_ODYSSEY_DEAD + OMM_HEALTH_ODYSSEY_PER_SEGMENT / 2) / OMM_HEALTH_ODYSSEY_PER_SEGMENT) * OMM_HEALTH_ODYSSEY_PER_SEGMENT) + OMM_HEALTH_ODYSSEY_DEAD, healthMin, healthMax);
}

s32 omm_health_from_ticks(s32 ticks) {
    return (ticks * OMM_HEALTH_ODYSSEY_PER_TICK) + OMM_HEALTH_ODYSSEY_DEAD;
}

s32 omm_health_to_ticks(s32 health) {
    return (health - OMM_HEALTH_ODYSSEY_DEAD + OMM_HEALTH_ODYSSEY_PER_TICK / 2) / OMM_HEALTH_ODYSSEY_PER_TICK;
}

s32 omm_health_get_max(s32 health) {
#if OMM_GAME_IS_SMSR
    if (gOmmGlobals->yoshiMode && gOmmGlobals->booZeroLife) return OMM_HEALTH_1_SEGMENT;
#endif
    if (OMM_MOVESET_ODYSSEY_1H) return OMM_HEALTH_1_SEGMENT;
    if (OMM_SPARKLY_STATE_IS_OK && omm_sparkly_context_get_data(OMM_SPARKLY_DATA_1_HEALTH)) return OMM_HEALTH_1_SEGMENT;
    if (OMM_SPARKLY_STATE_IS_OK && omm_sparkly_context_get_data(OMM_SPARKLY_DATA_3_HEALTH)) return OMM_HEALTH_ODYSSEY_3_SEGMENTS;
    if (OMM_SPARKLY_NEBULA_CURSE) return OMM_HEALTH_ODYSSEY_3_SEGMENTS;
    if (OMM_MOVESET_CLASSIC) return OMM_HEALTH_CLASSIC_MAX;
    if (OMM_MOVESET_ODYSSEY_6H) return OMM_HEALTH_ODYSSEY_MAX;
    if (health > OMM_HEALTH_ODYSSEY_3_SEGMENTS) return OMM_HEALTH_ODYSSEY_MAX;
    return OMM_HEALTH_ODYSSEY_3_SEGMENTS;
}

s32 omm_health_get_max_hud(s32 health) {
    if (OMM_MOVESET_CLASSIC) return OMM_HEALTH_CLASSIC_MAX;
    if (OMM_MOVESET_ODYSSEY_6H) return OMM_HEALTH_ODYSSEY_MAX;
    if (health > OMM_HEALTH_ODYSSEY_3_SEGMENTS) return OMM_HEALTH_ODYSSEY_MAX;
    return OMM_HEALTH_ODYSSEY_3_SEGMENTS;
}

s32 omm_health_get_max_ticks(s32 ticks) {
    if (OMM_MOVESET_CLASSIC) return OMM_HEALTH_CLASSIC_NUM_SEGMENTS;
    return omm_health_to_ticks(omm_health_get_max(omm_health_from_ticks(ticks)));
}

s32 omm_health_get_max_ticks_hud(s32 ticks) {
    if (OMM_MOVESET_CLASSIC) return OMM_HEALTH_CLASSIC_NUM_SEGMENTS;
    return omm_health_to_ticks(omm_health_get_max_hud(omm_health_from_ticks(ticks)));
}

//
// Odyssey freeze effect
//

static bool omm_update_freeze(struct MarioState *m) {
    bool shouldDamage = false;
    bool isColdWater = omm_world_is_cold();
    bool isOutOfBreath = (OMM_MOVESET_ODYSSEY && gOmmMario->state.breath >= OMM_BREATH_MAX_DURATION);

    // Cold course, or Mario is out of breath
    if (isColdWater || isOutOfBreath) {

        // Already dead
        if (m->action == ACT_DROWNING ||
            m->action == ACT_WATER_DEATH ||
            m->action == ACT_OMM_DEATH_FROZEN) {
            gOmmMario->state.freeze.gfx = isColdWater * OMM_FREEZING_WATER_TIMER_MAX;
        }

        // Freezing to death
        else {

            // Submerged
            // God Mode prevents Mario from freezing to death
            if (!omm_peach_vibe_is_calm() && !OMM_CHEAT_GOD_MODE && (m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {

                // Not invincible
                // Also stops briefly the freezing effect when Mario gets damaged or knockbacked
                if (!(m->flags & MARIO_METAL_CAP) && !(m->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE))) {
                    gOmmMario->state.freeze.gfx = min_s(OMM_FREEZING_WATER_TIMER_MAX, gOmmMario->state.freeze.gfx + OMM_FREEZING_WATER_INC) * isColdWater;

                    // Damage Mario every 5 seconds if freezing, 2.5 seconds if out of breath and 1.5 second if both
                    gOmmMario->state.freeze.dmg = min_s(OMM_FREEZING_WATER_TIMER_MAX, gOmmMario->state.freeze.dmg +
                        OMM_FREEZING_WATER_INC * isColdWater +
                        OMM_FREEZING_WATER_INC_OUT_OF_BREATH * isOutOfBreath
                    );
                    if (gOmmMario->state.freeze.dmg >= OMM_FREEZING_WATER_TIMER_MAX) {
                        gOmmMario->state.freeze.dmg = 0;
                        shouldDamage = true;
                    }

                    // Update freezing sound effect
                    if (isColdWater && !omm_mario_is_dead(m)) {
                        gOmmMario->state.freeze.sfx++;
                        if (gOmmMario->state.freeze.dmg >= OMM_FREEZING_WATER_TIMER_CRITICAL && (gOmmMario->state.freeze.sfx >= 3 || !omm_sound_is_bank_playing(3))) {
                            omm_sound_play(OMM_SOUND_EFFECT_FREEZE_2, NULL);
                            gOmmMario->state.freeze.sfx = 0;
                        } else if (gOmmMario->state.freeze.sfx >= 5 || !omm_sound_is_bank_playing(3)) {
                            omm_sound_play(OMM_SOUND_EFFECT_FREEZE_1, NULL);
                            gOmmMario->state.freeze.sfx = 0;
                        }
                    }
                }
            }

            // Airborne
            else if (m->action & ACT_FLAG_AIR) {
                gOmmMario->state.freeze.dmg = max_s(0, gOmmMario->state.freeze.dmg - OMM_FREEZING_WATER_DEC_AIR) * isColdWater;
                gOmmMario->state.freeze.gfx = max_s(0, gOmmMario->state.freeze.gfx - OMM_FREEZING_WATER_DEC_AIR) * isColdWater;
            }

            // Ground or other states
            else {
                gOmmMario->state.freeze.dmg = max_s(0, gOmmMario->state.freeze.dmg - OMM_FREEZING_WATER_DEC_GROUND) * isColdWater;
                gOmmMario->state.freeze.gfx = max_s(0, gOmmMario->state.freeze.gfx - OMM_FREEZING_WATER_DEC_GROUND) * isColdWater;
            }
        }
    }

    // Nothing to report
    else {
        gOmmMario->state.freeze.dmg = 0;
        gOmmMario->state.freeze.gfx = 0;
    }

    return shouldDamage;
}

//
// Odyssey breath
//

static void increase_breath(s32 inc) {
    if (gOmmMario->state.breath >= OMM_BREATH_MAX_DURATION / 100) { // <= 99%
        play_sound(SOUND_MENU_POWER_METER, gGlobalSoundArgs);
    }
    gOmmMario->state.breath = max_s(0, gOmmMario->state.breath - inc);
}

static void decrease_breath(s32 dec) {
    gOmmMario->state.breath = min_s(OMM_BREATH_MAX_DURATION, gOmmMario->state.breath + dec);
}

static void omm_health_update_breath(struct MarioState *m) {
    if (OMM_CHEAT_GOD_MODE) {
        increase_breath(OMM_BREATH_HEAL_INC);
        return;
    }

    // Mario is healed -> gives 1 extra second of breath per heal tick
    if (m->healCounter > 0) {
        increase_breath(OMM_BREATH_HEAL_INC);
        if (gOmmMario->state.breath == 0) {
            m->healCounter = 0;
        } else {
            m->healCounter--;
        }
    }

    // "Metal Mario doesn't even have to breathe"
    if (!(m->flags & MARIO_METAL_CAP) && !(m->action & ACT_FLAG_INTANGIBLE) && !omm_mario_is_capture(m)) {

        // Poison gas
        if (m->input & INPUT_IN_POISON_GAS) {
            decrease_breath(OMM_BREATH_POISON_DEC);

            // Insta-kill if out of breath in the poison gas
            if (gOmmMario->state.breath >= OMM_BREATH_MAX_DURATION) {
                omm_stats_increase(hitsTaken, m->health > OMM_HEALTH_ODYSSEY_DEAD);
                m->health = OMM_HEALTH_ODYSSEY_DEAD;
                m->healCounter = 0;
                m->hurtCounter = 0;
            }
        }

        // Water
        else if ((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {

            // Above water
            if (!omm_world_is_flooded() && m->pos[1] >= m->waterLevel - OMM_MARIO_WATER_BREATH_RECOVERY_OFFSET) {
                increase_breath(OMM_BREATH_HEAL_INC * 2);
            }

            // Under water
            else {

                // Drains more breath if Mario is plunging or dashing
                if (m->action == ACT_OMM_WATER_GROUND_POUND) {
                    decrease_breath(OMM_BREATH_WATER_POUND_DEC * (m->actionState == 1 ? 3 : 1));
                } else if (m->action == ACT_OMM_WATER_DASH) {
                    decrease_breath(OMM_BREATH_WATER_DASH_DEC);
                } else {
                    decrease_breath(OMM_BREATH_WATER_DEC);
                }

                // Insta-kill if out of breath in One-health mode
                if (OMM_MOVESET_ODYSSEY_1H && gOmmMario->state.breath >= OMM_BREATH_MAX_DURATION) {
                    omm_stats_increase(hitsTaken, m->health > OMM_HEALTH_ODYSSEY_DEAD);
                    m->health = OMM_HEALTH_ODYSSEY_DEAD;
                    m->healCounter = 0;
                    m->hurtCounter = 0;
                }
            }
        }

        // Fresh air
        else {
            increase_breath(OMM_BREATH_HEAL_INC);
        }

        // Play a sound effect when low on breath
        if ((gOmmMario->state.breath >= (OMM_BREATH_MAX_DURATION * 3) / 4) && !omm_peach_vibe_is_calm()) {
            play_sound(SOUND_MOVING_ALMOST_DROWNING, gGlobalSoundArgs);
        }
    }
}

//
// Odyssey health system
//

static void omm_health_update_classic(struct MarioState *m) {
    static u8 sPrevHurtCounter = 0;
    if (sPrevHurtCounter == 0 && m->hurtCounter != 0) {
        gOmmMario->state.health.state = OMM_HEALTH_STATE_DAMAGE;
        gOmmMario->state.health.timer = 0;
        omm_stats_increase(hitsTaken, 1);
    }

    // Drain even more health in cold water
    // 8 health per frame ~= 1 segment per second, meaning that Mario cannot
    // stay any longer than 8 seconds in cold water without healing
    if (omm_world_is_cold() &&
       !omm_peach_vibe_is_calm() &&
        (m->action & ACT_FLAG_SWIMMING) &&
       !(m->action & ACT_FLAG_INTANGIBLE) &&
        (m->healCounter == 0) &&
        (m->hurtCounter == 0)) {
        m->health = max_s(0xFF, m->health - 5);
    }

    // Don't play the beeping sound underwater if Metal or Hard mode
    u32 action = m->action;
    m->action &= ~(ACT_GROUP_MASK * ((m->flags & MARIO_METAL_CAP) || OMM_MOVESET_ODYSSEY_1H));
    update_mario_health(m);
    m->action = action;
    m->health = clamp_s(m->health, OMM_HEALTH_CLASSIC_DEAD, omm_health_get_max(m->health));
    sPrevHurtCounter = m->hurtCounter;
}

void omm_health_update(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) {
        omm_health_update_classic(m);
        return;
    }

    // Entered a new area
    if (gOmmMario->state.ticks == 0) {
        gOmmMario->state.ticks = omm_health_to_ticks(m->health);
        gOmmMario->state.coins = m->numCoins;
    }

    // Update health system
    // Skip this part during the Life-up cutscene
    bool isLifeUpCutscene = (gOmmMario->state.health.state == OMM_HEALTH_STATE_LIFE_UP && obj_get_first_with_behavior(bhvOmmLifeUp) != NULL);
    if (!isLifeUpCutscene) {

        // Breath
        omm_health_update_breath(m);

        // Freezing water
        if (omm_update_freeze(m)) {
            m->hurtCounter = 1;
            m->invincTimer = 30;
            set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
            mario_set_forward_vel(m, 0);
        }

        // Mario takes damage
        if (m->hurtCounter != 0) {
            m->health -= OMM_HEALTH_ODYSSEY_PER_SEGMENT;
            m->hurtCounter = 0;
            omm_sound_play(OMM_SOUND_EFFECT_DAMAGE, NULL);
            gOmmMario->state.health.state = OMM_HEALTH_STATE_DAMAGE;
            gOmmMario->state.health.timer = 0;
            omm_stats_increase(hitsTaken, 1);
        }

        // Update coin counter and heal Mario every N coins
        if (gOmmMario->state.coins < m->numCoins) {
            gOmmMario->state.coins++;
            if ((gOmmMario->state.coins % OMM_HEALTH_ODYSSEY_NUM_COINS_PER_HEAL) == 0) {
                omm_health_heal_mario(m, true);
            }
        } else {
            gOmmMario->state.coins = m->numCoins;
        }
    }

    // Update health ticks
    m->health = omm_health_odyssey(m->health);
    s32 ticks = omm_health_to_ticks(m->health);
    if (gOmmMario->state.ticks < ticks) {
        gOmmMario->state.ticks++;
        if (!isLifeUpCutscene && !OMM_MOVESET_ODYSSEY_1H && (gOmmMario->state.ticks % OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT) == (OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT / 2)) {
            omm_sound_play(OMM_SOUND_EFFECT_HEAL, NULL);
        }
    } else if (gOmmMario->state.ticks > ticks) {
        gOmmMario->state.ticks--;
    }

    // Updated
    // Set this value to 0xFF to prevent the 'heal up' sound effect from playing
    gHudDisplay.wedges = 0xFF;
}

void omm_health_heal_mario(struct MarioState *m, bool updateHealthState) {
    if (OMM_MOVESET_ODYSSEY) {
        s32 newHealth = min_s(m->health + OMM_HEALTH_ODYSSEY_PER_SEGMENT, omm_health_get_max(m->health));
        if (updateHealthState && m->health < newHealth) {
            gOmmMario->state.health.state = OMM_HEALTH_STATE_HEAL;
            gOmmMario->state.health.timer = 0;
        }
        m->health = newHealth;
    }
}

void omm_health_fully_heal_mario(struct MarioState *m, bool updateHealthState) {
    if (OMM_MOVESET_ODYSSEY) {
        s32 newHealth = omm_health_get_max(m->health);
        if (updateHealthState && m->health < newHealth) {
            gOmmMario->state.health.state = OMM_HEALTH_STATE_HEAL;
            gOmmMario->state.health.timer = 0;
        }
        m->health = newHealth;
        gOmmMario->state.ticks = max_s(gOmmMario->state.ticks, 1);
    } else {
        m->healCounter = 31;
        m->hurtCounter = 0;
    }
}

void omm_health_life_up(struct MarioState *m) {
    gOmmSparklyContext->mushrooms++;
    s32 maxHealth = omm_health_get_max(0);
    if (maxHealth > OMM_HEALTH_1_SEGMENT && OMM_MOVESET_ODYSSEY) {
        s32 lifeUpHealth = omm_health_get_max(OMM_HEALTH_ODYSSEY_MAX);
        if (lifeUpHealth > maxHealth && m->health <= maxHealth) {
            gOmmMario->state.ticks = omm_health_to_ticks(maxHealth);
            omm_obj_spawn_life_up(m->marioObj);
        } else if (m->health < lifeUpHealth) {
            gOmmMario->state.health.state = OMM_HEALTH_STATE_HEAL;
            gOmmMario->state.health.timer = 0;
        }
        m->health = lifeUpHealth;
    } else {
        play_sound(SOUND_GENERAL_COLLECT_1UP, gGlobalSoundArgs);
    }
    m->healCounter = OMM_BREATH_REFILL;
    m->hurtCounter = 0;
}

void omm_health_set_max(struct MarioState *m) {
    m->health = omm_health_get_max(m->health);
    gOmmMario->state.ticks = omm_health_to_ticks(m->health);
    m->healCounter = 0;
    m->hurtCounter = 0;
}

bool omm_health_is_at_max(struct MarioState *m) {
    return m->health >= omm_health_get_max(m->health);
}

bool omm_health_is_healing(struct MarioState *m) {
    return (OMM_MOVESET_ODYSSEY && (gOmmMario->state.ticks < omm_health_to_ticks(m->health))) ||
           (OMM_MOVESET_CLASSIC && (m->healCounter != 0 && !omm_health_is_at_max(m)));
}

bool omm_health_is_taking_damage(struct MarioState *m) {
    return (OMM_MOVESET_ODYSSEY && (gOmmMario->state.ticks > omm_health_to_ticks(m->health))) ||
           (OMM_MOVESET_CLASSIC && (m->hurtCounter != 0 && !omm_mario_is_dead(m)));
}

//
// State
//

typedef struct {
    s32 duration;
    s32 moveStart;
    s32 moveEnd;
    s32 shakeXStart;
    s32 shakeXEnd;
    s32 shakeXFreq;
    s32 shakeXPhase;
    f32 shakeXOffset;
    f32 (*shakeXAmplitude)(s16);
    s32 shakeYStart;
    s32 shakeYEnd;
    s32 shakeYFreq;
    s32 shakeYPhase;
    f32 shakeYOffset;
    f32 (*shakeYAmplitude)(s16);
} OmmHealthState;

static f32 omm_health_state_shake_x_amplitude__zero(UNUSED s16 w) {
    return 0.f;
}

static f32 omm_health_state_shake_x_amplitude__life_up(s16 w) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(w);
    return OMM_HEALTH_ANIM_LIFE_UP_SHAKE_X;
}

static f32 omm_health_state_shake_y_amplitude__zero(UNUSED s16 w) {
    return 0.f;
}

static f32 omm_health_state_shake_y_amplitude__damage(s16 w) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(w);
    return OMM_HEALTH_ANIM_DAMAGE_SHAKE_Y;
}
static f32 omm_health_state_shake_y_amplitude__life_up(s16 w) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(w);
    return OMM_HEALTH_ANIM_LIFE_UP_SHAKE_Y;
}

static const OmmHealthState OMM_HEALTH_STATES[][2] = {
    [OMM_HEALTH_STATE_NONE] = {{
        .duration           = 0,
        .moveStart          = 0,
        .moveEnd            = 0,
        .shakeXStart        = 0,
        .shakeXEnd          = 0,
        .shakeXFreq         = 0,
        .shakeXPhase        = 0,
        .shakeXOffset       = 0,
        .shakeXAmplitude    = omm_health_state_shake_x_amplitude__zero,
        .shakeYStart        = 0,
        .shakeYEnd          = 0,
        .shakeYFreq         = 0,
        .shakeYPhase        = 0,
        .shakeYOffset       = 0,
        .shakeYAmplitude    = omm_health_state_shake_y_amplitude__zero,
    },{
        .duration           = 0,
        .moveStart          = 0,
        .moveEnd            = 0,
        .shakeXStart        = 0,
        .shakeXEnd          = 0,
        .shakeXFreq         = 0,
        .shakeXPhase        = 0,
        .shakeXOffset       = 0,
        .shakeXAmplitude    = omm_health_state_shake_x_amplitude__zero,
        .shakeYStart        = 0,
        .shakeYEnd          = 0,
        .shakeYFreq         = 0,
        .shakeYPhase        = 0,
        .shakeYOffset       = 0,
        .shakeYAmplitude    = omm_health_state_shake_y_amplitude__zero,
    }},
    [OMM_HEALTH_STATE_HEAL] = {{
        .duration           = OMM_HEALTH_ANIM_HEAL_MOVE_START,
        .moveStart          = 0,
        .moveEnd            = 0,
        .shakeXStart        = 0,
        .shakeXEnd          = 0,
        .shakeXFreq         = 0,
        .shakeXPhase        = 0,
        .shakeXOffset       = 0,
        .shakeXAmplitude    = omm_health_state_shake_x_amplitude__zero,
        .shakeYStart        = 0,
        .shakeYEnd          = 0,
        .shakeYFreq         = 0,
        .shakeYPhase        = 0,
        .shakeYOffset       = 0,
        .shakeYAmplitude    = omm_health_state_shake_y_amplitude__zero,
    },{
        .duration           = OMM_HEALTH_ANIM_HEAL_MOVE_END,
        .moveStart          = OMM_HEALTH_ANIM_HEAL_MOVE_START,
        .moveEnd            = OMM_HEALTH_ANIM_HEAL_MOVE_END,
        .shakeXStart        = 0,
        .shakeXEnd          = 0,
        .shakeXFreq         = 0,
        .shakeXPhase        = 0,
        .shakeXOffset       = 0,
        .shakeXAmplitude    = omm_health_state_shake_x_amplitude__zero,
        .shakeYStart        = 0,
        .shakeYEnd          = 0,
        .shakeYFreq         = 0,
        .shakeYPhase        = 0,
        .shakeYOffset       = 0,
        .shakeYAmplitude    = omm_health_state_shake_y_amplitude__zero,
    }},
    [OMM_HEALTH_STATE_DAMAGE] = {{
        .duration           = OMM_HEALTH_ANIM_DAMAGE_MOVE_START,
        .moveStart          = 0,
        .moveEnd            = 0,
        .shakeXStart        = 0,
        .shakeXEnd          = 0,
        .shakeXFreq         = 0,
        .shakeXPhase        = 0,
        .shakeXOffset       = 0,
        .shakeXAmplitude    = omm_health_state_shake_x_amplitude__zero,
        .shakeYStart        = 0,
        .shakeYEnd          = OMM_HEALTH_ANIM_DAMAGE_SHAKE_END,
        .shakeYFreq         = OMM_HEALTH_ANIM_DAMAGE_SHAKE_FREQ,
        .shakeYPhase        = 0x4000,
        .shakeYOffset       = 0,
        .shakeYAmplitude    = omm_health_state_shake_y_amplitude__damage,
    },{
        .duration           = OMM_HEALTH_ANIM_DAMAGE_MOVE_END,
        .moveStart          = OMM_HEALTH_ANIM_DAMAGE_MOVE_START,
        .moveEnd            = OMM_HEALTH_ANIM_DAMAGE_MOVE_END,
        .shakeXStart        = 0,
        .shakeXEnd          = 0,
        .shakeXFreq         = 0,
        .shakeXPhase        = 0,
        .shakeXOffset       = 0,
        .shakeXAmplitude    = omm_health_state_shake_x_amplitude__zero,
        .shakeYStart        = 0,
        .shakeYEnd          = OMM_HEALTH_ANIM_DAMAGE_SHAKE_END,
        .shakeYFreq         = OMM_HEALTH_ANIM_DAMAGE_SHAKE_FREQ,
        .shakeYPhase        = 0x4000,
        .shakeYOffset       = 0,
        .shakeYAmplitude    = omm_health_state_shake_y_amplitude__damage,
    }},
    [OMM_HEALTH_STATE_LIFE_UP] = {{
        .duration           = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_END,
        .moveStart          = OMM_HEALTH_ANIM_LIFE_UP_MOVE_START,
        .moveEnd            = OMM_HEALTH_ANIM_LIFE_UP_MOVE_END,
        .shakeXStart        = OMM_HEALTH_ANIM_LIFE_UP_MOVE_END,
        .shakeXEnd          = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_END,
        .shakeXFreq         = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_FREQ,
        .shakeXPhase        = 0x4000,
        .shakeXOffset       = -1.f,
        .shakeXAmplitude    = omm_health_state_shake_x_amplitude__life_up,
        .shakeYStart        = OMM_HEALTH_ANIM_LIFE_UP_MOVE_END,
        .shakeYEnd          = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_END,
        .shakeYFreq         = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_FREQ,
        .shakeYPhase        = 0,
        .shakeYOffset       = 0,
        .shakeYAmplitude    = omm_health_state_shake_y_amplitude__life_up,
    },{
        .duration           = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_END,
        .moveStart          = OMM_HEALTH_ANIM_LIFE_UP_MOVE_START,
        .moveEnd            = OMM_HEALTH_ANIM_LIFE_UP_MOVE_END,
        .shakeXStart        = OMM_HEALTH_ANIM_LIFE_UP_MOVE_END,
        .shakeXEnd          = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_END,
        .shakeXFreq         = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_FREQ,
        .shakeXPhase        = 0x4000,
        .shakeXOffset       = -1.f,
        .shakeXAmplitude    = omm_health_state_shake_x_amplitude__life_up,
        .shakeYStart        = OMM_HEALTH_ANIM_LIFE_UP_MOVE_END,
        .shakeYEnd          = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_END,
        .shakeYFreq         = OMM_HEALTH_ANIM_LIFE_UP_SHAKE_FREQ,
        .shakeYPhase        = 0,
        .shakeYOffset       = 0,
        .shakeYAmplitude    = omm_health_state_shake_y_amplitude__life_up,
    }},
};

static const OmmHealthState *omm_health_state_get(s32 state) {
    return &OMM_HEALTH_STATES[state][OMM_MOVESET_ODYSSEY && (OMM_HUD_ALWAYS || OMM_HUD_VANISHING)];
}

static f32 omm_health_state_get_relative_pos(s32 timer, s32 tmin, s32 tmax) {
    return tmax > tmin ? 1.f - sqr_f(invlerp_0_1_f(timer, tmin, tmax)) : 0.f;
}

static f32 omm_health_state_get_shake(s32 timer, s32 tmin, s32 tmax, s32 freq, s32 phase, f32 offset, f32 amplitude) {
    return tmax > tmin ? amplitude * relerp_0_1_f(timer, tmin, tmax, 1.f, 0.f) * (sins(freq * (clamp_s(timer, tmin, tmax) - tmin) + phase) + offset) : 0.f;
}

void omm_health_state_get_anim_params(s32 state, s32 timer, f32 *shakeX, f32 *shakeY, f32 *relPos) {
    const OmmHealthState *hstate = omm_health_state_get(state);
    *shakeX = omm_health_state_get_shake(timer, hstate->shakeXStart, hstate->shakeXEnd, hstate->shakeXFreq, hstate->shakeXPhase, hstate->shakeXOffset, hstate->shakeXAmplitude(gOmmHudSize));
    *shakeY = omm_health_state_get_shake(timer, hstate->shakeYStart, hstate->shakeYEnd, hstate->shakeYFreq, hstate->shakeYPhase, hstate->shakeYOffset, hstate->shakeYAmplitude(gOmmHudSize));
    *relPos = omm_health_state_get_relative_pos(timer, hstate->moveStart, hstate->moveEnd);
}

static bool omm_health_state_should_stay_active(struct MarioState *m) {

    // Heal
    if (gOmmMario->state.health.state == OMM_HEALTH_STATE_HEAL) {

        // Healing
        if (omm_health_is_healing(m)) {
            return true;
        }

        // Full health
        if (omm_health_is_at_max(m)) {
            return false;
        }

        // Calm Vibe is active
        if (omm_peach_vibe_is_calm()) {
            return true;
        }
    }

    // Damage
    else if (gOmmMario->state.health.state == OMM_HEALTH_STATE_DAMAGE) {

        // Taking damage
        if (omm_health_is_taking_damage(m)) {
            return true;
        }

        // Dead
        if (omm_mario_is_dead(m)) {
            return false;
        }

        // Underwater after depleting all breath, only if breath loss is possible
        if (gOmmMario->state.breath >= OMM_BREATH_MAX_DURATION && !(m->flags & MARIO_METAL_CAP) && !omm_mario_is_capture(m)) {
            return true;
        }
    }

    return false;
}

OMM_ROUTINE_UPDATE(omm_health_state_update) {
    static u32 sOmmHealthPreviousState = 0;
    static bool sOmmHealthBurnDamage = false;

    // Set state to 0 on the main menu
    if (omm_is_main_menu()) {
        sOmmHealthPreviousState = 0;
        sOmmHealthBurnDamage = false;
        return;
    }

    // Update state if Mario is loaded and game is not paused
    if (gMarioObject && !omm_is_game_paused()) {
        struct MarioState *m = gMarioState;

        // Health state
        if (omm_health_state_should_stay_active(m)) {
            gOmmMario->state.health.timer = min_s(gOmmMario->state.health.timer + 1, OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT);
        } else if (++gOmmMario->state.health.timer > omm_health_state_get(gOmmMario->state.health.state)->duration) {
            gOmmMario->state.health.state = OMM_HEALTH_STATE_NONE;
            gOmmMario->state.health.timer = 0;
        }

        // Burn damage
        if (!omm_mario_is_burning(m)) {
            sOmmHealthBurnDamage = false;
        } else if (OMM_MOVESET_ODYSSEY && !sOmmHealthBurnDamage) {
            sOmmHealthBurnDamage = true;
            m->hurtCounter = 1;
        }

        // Classic 8-segments health system
        if (OMM_MOVESET_CLASSIC) {
            if (sOmmHealthPreviousState != 0) {
                gPlayerCameraState->action = ACT_FLAG_SWIMMING;
                handle_power_meter_actions(0);
                gPlayerCameraState->action = m->action;
            }
            sOmmHealthPreviousState = 0;
            return;
        }

        // Health system was changed to Odyssey
        // Increase health to at least OMM_HEALTH_ODYSSEY_1_SEGMENT if Mario is not hurt/dead to not kill him after health conversion
        if (gOmmMovesetType != sOmmHealthPreviousState) {
            m->health = min_s(m->health, omm_health_get_max(0));
            if (m->health > OMM_HEALTH_ODYSSEY_DEAD && m->healCounter >= m->hurtCounter && !omm_mario_is_burning(m)) {
                m->health = max_s(m->health, OMM_HEALTH_ODYSSEY_1_SEGMENT);
            }
            m->health = omm_health_odyssey(m->health);
            gOmmMario->state.ticks = omm_health_to_ticks(m->health);
            gOmmMario->state.coins = m->numCoins;
            gOmmMario->state.health.state = OMM_HEALTH_STATE_NONE;
            gOmmMario->state.health.timer = 0;
            sOmmHealthPreviousState = gOmmMovesetType;
        }
    }
}
