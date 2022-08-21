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
    if (gOmmOneHealthMode) return OMM_HEALTH_1_SEGMENT;
    if (OMM_SPARKLY_LUNATIC_HEALTH) return OMM_HEALTH_1_SEGMENT;
    if (OMM_SPARKLY_STATE_IS_OK && omm_sparkly_context_get_data(OMM_SPARKLY_DATA_3_HEALTH)) return OMM_HEALTH_ODYSSEY_3_SEGMENTS;
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

f32 omm_health_get_life_up_gauge_position(s32 ticks) {
    struct Object *o = obj_get_first_with_behavior(bhvOmmLifeUp);
    if (!o) return (ticks <= omm_health_get_max_ticks_hud(0) ? 1.f : 0.f);
    f32 t = (f32) (o->oTimer - OMM_HEALTH_ODYSSEY_LIFE_UP_HEAL_END) / (f32) (OMM_HEALTH_ODYSSEY_LIFE_UP_POS_END - OMM_HEALTH_ODYSSEY_LIFE_UP_HEAL_END);
    return sqrtf(clamp_0_1_f(1.f - t));
}

f32 omm_health_get_life_gauge_rumble_strength(struct MarioState *m) {
    struct Object *o = obj_get_first_with_behavior(bhvOmmLifeUp);
    if (!o) return 15.f * (gOmmMario->state.ticks > omm_health_to_ticks(m->health));
    f32 t = (f32) (o->oTimer - OMM_HEALTH_ODYSSEY_LIFE_UP_POS_END) / (f32) (OMM_HEALTH_ODYSSEY_LIFE_UP_CUTSCENE_END - OMM_HEALTH_ODYSSEY_LIFE_UP_POS_END);
    return (t < 0.f ? 0.f : 20.f * (1.f - t));
}

//
// Odyssey freeze effect
//

static bool omm_update_freeze(struct MarioState *m) {
    bool damage = false;
    bool isCold = omm_world_is_cold();
    bool isNoO2 = (OMM_MOVESET_ODYSSEY && gOmmMario->state.o2 >= OMM_O2_MAX_DURATION);

    // Cold course, or Mario is out of breath
    if (isCold || isNoO2) {

        // Already dead
        if (m->action == ACT_DROWNING ||
            m->action == ACT_WATER_DEATH ||
            m->action == ACT_OMM_DEATH_FROZEN) {
            gOmmMario->state.freeze.gfx = isCold * OMM_FREEZING_WATER_TIMER_MAX;
        }
        
        // Freezing to death
        else {
        
            // Submerged
            // God Mode prevents Mario from freezing to death
            if (!omm_peach_vibe_is_calm() && !OMM_CHEAT_GOD_MODE && (m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {

                // Not invincible
                // Also stops briefly the freezing effect when Mario gets damaged or knockbacked
                if (!(m->flags & MARIO_METAL_CAP) && !(m->action & ACT_FLAG_INVULNERABLE) && !(m->action & ACT_FLAG_INTANGIBLE)) {
                    gOmmMario->state.freeze.dmg = min_s(gOmmMario->state.freeze.dmg + OMM_FREEZING_WATER_INC, OMM_FREEZING_WATER_TIMER_MAX);
                    gOmmMario->state.freeze.gfx = min_s(gOmmMario->state.freeze.gfx + OMM_FREEZING_WATER_INC, OMM_FREEZING_WATER_TIMER_MAX) * isCold;

                    // Drains health faster if Mario is out of breath
                    if (gOmmMario->state.freeze.dmg >= ((OMM_FREEZING_WATER_TIMER_MAX * 2) / (2 + isNoO2))) {
                        gOmmMario->state.freeze.dmg = 0;
                        damage = true;
                    }

                    // Update freezing sound effect
                    if (isCold && !omm_mario_is_dead(m)) {
                        gOmmMario->state.freeze.sfx++;
                        if ((gOmmMario->state.freeze.dmg >= OMM_FREEZING_WATER_TIMER_CRITICAL) &&
                            (gOmmMario->state.freeze.sfx >= 3 || !omm_sound_is_bank_playing(3))) {
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
                gOmmMario->state.freeze.dmg = max_s(0, gOmmMario->state.freeze.dmg - OMM_FREEZING_WATER_DEC_AIR);
                gOmmMario->state.freeze.gfx = max_s(0, gOmmMario->state.freeze.gfx - OMM_FREEZING_WATER_DEC_AIR) * isCold;
            }

            // Ground or other states
            else {
                gOmmMario->state.freeze.dmg = max_s(0, gOmmMario->state.freeze.dmg - OMM_FREEZING_WATER_DEC_GROUND);
                gOmmMario->state.freeze.gfx = max_s(0, gOmmMario->state.freeze.gfx - OMM_FREEZING_WATER_DEC_GROUND) * isCold;
            }
        }
    }
    
    // Nothing to report
    else {
        gOmmMario->state.freeze.dmg = 0;
        gOmmMario->state.freeze.gfx = 0;
    }

    return damage;
}

//
// Odyssey oxygen level
//

static void increase_O2_level(s32 inc) {
    if (gOmmMario->state.o2 >= OMM_O2_MAX_DURATION / 100) { // <= 99%
        play_sound(SOUND_MENU_POWER_METER, gMarioObject->oCameraToObject);
    }
    gOmmMario->state.o2 = max_s(0, gOmmMario->state.o2 - inc);
}

static void decrease_O2_level(s32 dec) {
    gOmmMario->state.o2 = min_s(OMM_O2_MAX_DURATION, gOmmMario->state.o2 + dec);
}

static void omm_health_update_O2_level(struct MarioState *m) {
    if (OMM_CHEAT_GOD_MODE) {
        increase_O2_level(OMM_O2_HEAL_INC);
        return;
    }

    // Mario is healed -> gives 1 extra second of O2 per heal tick
    if (m->healCounter > 0) {
        increase_O2_level(OMM_O2_HEAL_INC);
        if (gOmmMario->state.o2 == 0) {
            m->healCounter = 0;
        } else {
            m->healCounter--;
        }
    }

    // "Metal Mario doesn't even have to breathe"
    if (!(m->flags & MARIO_METAL_CAP) &&
        !(m->action & ACT_FLAG_INTANGIBLE) &&
        !(m->action == ACT_OMM_POSSESSION)) {

        // Poison gas
        if (m->input & INPUT_IN_POISON_GAS) {
            decrease_O2_level(OMM_O2_POISON_DEC);

            // Insta-kill if out of breath in the poison gas
            if (gOmmMario->state.o2 >= OMM_O2_MAX_DURATION) {
                m->health = OMM_HEALTH_ODYSSEY_DEAD;
                m->healCounter = 0;
                m->hurtCounter = 0;
            }
        }

        // Water
        else if ((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {

            // Above water
            if (m->pos[1] >= m->waterLevel - 140) {
                increase_O2_level(OMM_O2_HEAL_INC * 2);
            }

            // Under water
            else {

                // Drains more O2 if Mario is plunging or dashing
                if (m->action == ACT_OMM_WATER_GROUND_POUND) {
                    decrease_O2_level(OMM_O2_WATER_POUND_DEC * (m->actionState == 1 ? 3 : 1));
                } else if (m->action == ACT_OMM_WATER_DASH) {
                    decrease_O2_level(OMM_O2_WATER_DASH_DEC);
                } else {
                    decrease_O2_level(OMM_O2_WATER_DEC);
                }

                // Insta-kill if out of breath in One-health mode
                if (gOmmOneHealthMode && gOmmMario->state.o2 >= OMM_O2_MAX_DURATION) {
                    m->health = OMM_HEALTH_ODYSSEY_DEAD;
                    m->healCounter = 0;
                    m->hurtCounter = 0;
                }
            }
        }

        // Fresh air
        else {
            increase_O2_level(OMM_O2_HEAL_INC);
        }

        // Play a sound effect when low on O2
        if ((gOmmMario->state.o2 >= (OMM_O2_MAX_DURATION * 3) / 4) && !omm_peach_vibe_is_calm()) {
            play_sound(SOUND_MOVING_ALMOST_DROWNING, gGlobalSoundArgs);
        }
    }
}

//
// Odyssey health system
//

static void omm_health_update_classic(struct MarioState *m) {

    // One-health mode: Prevent the game from draining health in water or poison
    // by setting both heal counter and hurt counter to non-zero values.
    // If the hurt counter is non-zero, prevent damage cancelation by
    // setting the heal counter to 0.
    if (gOmmOneHealthMode) {
        if (m->hurtCounter == 0) {
            m->healCounter = 1;
            m->hurtCounter = 1;
        } else {
            m->healCounter = 0;
        }
    }

    // Drain even more health in cold water
    // 8 health per frame ~= 1 segment per second, meaning that Mario cannot
    // stay any longer than 8 seconds in cold water without healing
    if (omm_world_is_cold() &&
       !omm_peach_vibe_is_calm() &&
        (m->action & ACT_FLAG_SWIMMING) &&
       !(m->action & ACT_FLAG_INTANGIBLE)) {

        // One-health mode prevents health drain underwater, so the freeze
        // effect must be used instead to damage Mario in cold water
        if (gOmmOneHealthMode) {
            if (omm_update_freeze(m)) {
                m->health = OMM_HEALTH_CLASSIC_DEAD;
                set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
                omm_mario_set_action(m, ACT_WATER_DEATH, 0, 0);
                mario_set_forward_vel(m, 0);
            }
        } else if (m->healCounter == 0 && m->hurtCounter == 0) {
            m->health = max_s(0xFF, m->health - 5);
        }
    }

    // Don't play the beeping sound underwater if Metal or Hard mode
    u32 action = m->action;
    m->action &= ~(ACT_GROUP_MASK * ((m->flags & MARIO_METAL_CAP) || gOmmOneHealthMode || OMM_SPARKLY_LUNATIC_HEALTH));
    update_mario_health(m);
    m->action = action;
    m->health = clamp_s(m->health, OMM_HEALTH_CLASSIC_DEAD, omm_health_get_max(m->health));
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
    bool isLifeUpCutscene = (obj_get_first_with_behavior(bhvOmmLifeUp) != NULL);
    if (!isLifeUpCutscene) {

        // O2 level
        omm_health_update_O2_level(m);

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
        }

        // Update coin counter and heal Mario every N coins
        if (gOmmMario->state.coins < m->numCoins) {
            gOmmMario->state.coins++;
            if ((gOmmMario->state.coins % OMM_HEALTH_ODYSSEY_NUM_COINS_PER_HEAL) == 0) {
                omm_health_heal_mario(m);
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
        if (!isLifeUpCutscene && (gOmmMario->state.ticks % OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT) == (OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT / 2)) {
            omm_sound_play(OMM_SOUND_EFFECT_HEAL, NULL);
        }
    } else if (gOmmMario->state.ticks > ticks) {
        gOmmMario->state.ticks--;
    }

    // Updated
    // Set this value to 0xFF to prevent the 'heal up' sound effect from playing
    gHudDisplay.wedges = 0xFF;
}

void omm_health_heal_mario(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        m->health = min_s(m->health + OMM_HEALTH_ODYSSEY_PER_SEGMENT, omm_health_get_max(m->health));
    }
}

void omm_health_fully_heal_mario(struct MarioState *m) {
    if (OMM_MOVESET_ODYSSEY) {
        gOmmMario->state.ticks = max_s(gOmmMario->state.ticks, 1);
        m->health = omm_health_get_max(m->health);
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
            omm_spawn_life_up(m->marioObj);
        }
        m->health = lifeUpHealth;
    } else {
        play_sound(SOUND_GENERAL_COLLECT_1UP, gGlobalSoundArgs);
    }
    m->healCounter = OMM_O2_REFILL;
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

//
// State
//

OMM_ROUTINE_UPDATE(omm_health_state_update) {
    static u32 sOmmHealthPreviousState = 0;

    // Set state to 0 on the main menu
    if (omm_is_main_menu()) {
        sOmmHealthPreviousState = 0;
        return;
    }
    
    // Update state if Mario is loaded
    if (gMarioObject) {    
        struct MarioState *m = gMarioState;

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
            sOmmHealthPreviousState = gOmmMovesetType;
        }
    }
}
