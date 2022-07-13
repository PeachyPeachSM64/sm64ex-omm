#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Health conversion
//

static s32 omm_fix_health(s32 health) {
    s32 fixed = clamp_s((((health - OMM_HEALTH_ODYSSEY_DEAD + OMM_HEALTH_ODYSSEY_PER_SEGMENT / 2) / OMM_HEALTH_ODYSSEY_PER_SEGMENT) * OMM_HEALTH_ODYSSEY_PER_SEGMENT) + OMM_HEALTH_ODYSSEY_DEAD, OMM_HEALTH_ODYSSEY_DEAD, OMM_HEALTH_ODYSSEY_MAX);
    return max_s(fixed, ((g1HPMode && health > OMM_HEALTH_ODYSSEY_DEAD) ? OMM_HEALTH_ODYSSEY_1_HP : OMM_HEALTH_ODYSSEY_DEAD));
}

static s32 omm_hp_to_health(s32 hp) {
    return (hp * OMM_HEALTH_ODYSSEY_PER_TICK) + OMM_HEALTH_ODYSSEY_DEAD;
}

static s32 omm_health_to_hp(s32 health) {
    return (health - OMM_HEALTH_ODYSSEY_DEAD + OMM_HEALTH_ODYSSEY_PER_TICK / 2) / OMM_HEALTH_ODYSSEY_PER_TICK;
}

s32 omm_health_get_max(s32 health) {
    if (OMM_MOVESET_CLASSIC || OMM_MOVESET_ODYSSEY_6H || (health > OMM_HEALTH_ODYSSEY_3_HP)) {
        return OMM_HEALTH_MAX;
    }
    return OMM_HEALTH_ODYSSEY_3_HP;
}

s32 omm_health_get_max_hp(s32 hp) {
    if (OMM_MOVESET_CLASSIC) return OMM_HEALTH_CLASSIC_NUM_SEGMENTS;
    return omm_health_to_hp(omm_health_get_max(omm_hp_to_health(hp)));
}

bool omm_health_is_mario_losing_hp(struct MarioState *m) {
    return gOmmData->mario->state.hp > omm_health_to_hp(m->health);
}

//
// Odyssey freeze effect
//

static bool omm_update_freeze(struct MarioState *m) {
    bool damage = false;
    bool isCold = ((m->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW);
    bool isNoO2 = (OMM_MOVESET_ODYSSEY && gOmmData->mario->state.o2 >= OMM_O2_MAX_DURATION);

    // Cold course, or Mario is out of breath
    if (isCold || isNoO2) {

        // Already dead
        if (m->action == ACT_DROWNING ||
            m->action == ACT_WATER_DEATH ||
            m->action == ACT_OMM_DEATH_FROZEN) {
            gOmmData->mario->state.freeze.gfx = OMM_FREEZING_WATER_TIMER_MAX;
        }
        
        // Freezing to death
        else {
        
            // Submerged
            // God Mode prevents Mario from freezing to death
            if (!omm_peach_vibe_is_calm() && !OMM_CHEAT_GOD_MODE && (m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {

                // Not invincible
                // Also stops briefly the freezing effect when Mario gets damaged or knockbacked
                if (!(m->flags & MARIO_METAL_CAP) && !(m->action & ACT_FLAG_INVULNERABLE) && !(m->action & ACT_FLAG_INTANGIBLE)) {
                    gOmmData->mario->state.freeze.dmg = min_s(gOmmData->mario->state.freeze.dmg + OMM_FREEZING_WATER_INC, OMM_FREEZING_WATER_TIMER_MAX);
                    gOmmData->mario->state.freeze.gfx = min_s(gOmmData->mario->state.freeze.gfx + OMM_FREEZING_WATER_INC, OMM_FREEZING_WATER_TIMER_MAX) * isCold;

                    // Drains HP faster if Mario is out of breath
                    if (gOmmData->mario->state.freeze.dmg >= ((OMM_FREEZING_WATER_TIMER_MAX * 2) / (2 + isNoO2))) {
                        gOmmData->mario->state.freeze.dmg = 0;
                        damage = true;
                    }

                    // Update freezing sound effect
                    if (isCold && (m->health > OMM_HEALTH_DEAD)) {
                        gOmmData->mario->state.freeze.sfx++;
                        if ((gOmmData->mario->state.freeze.dmg >= OMM_FREEZING_WATER_TIMER_CRITICAL) &&
                            (gOmmData->mario->state.freeze.sfx >= 3 || !omm_sound_is_bank_playing(3))) {
                            omm_sound_play(OMM_SOUND_EFFECT_FREEZE_2, NULL);
                            gOmmData->mario->state.freeze.sfx = 0;
                        } else if (gOmmData->mario->state.freeze.sfx >= 5 || !omm_sound_is_bank_playing(3)) {
                            omm_sound_play(OMM_SOUND_EFFECT_FREEZE_1, NULL);
                            gOmmData->mario->state.freeze.sfx = 0;
                        }
                    }
                }
            }

            // Airborne
            else if (m->action & ACT_FLAG_AIR) {
                gOmmData->mario->state.freeze.dmg = max_s(0, gOmmData->mario->state.freeze.dmg - OMM_FREEZING_WATER_DEC_AIR);
                gOmmData->mario->state.freeze.gfx = max_s(0, gOmmData->mario->state.freeze.gfx - OMM_FREEZING_WATER_DEC_AIR) * isCold;
            }

            // Ground or other states
            else {
                gOmmData->mario->state.freeze.dmg = max_s(0, gOmmData->mario->state.freeze.dmg - OMM_FREEZING_WATER_DEC_GROUND);
                gOmmData->mario->state.freeze.gfx = max_s(0, gOmmData->mario->state.freeze.gfx - OMM_FREEZING_WATER_DEC_GROUND) * isCold;
            }
        }
    }
    
    // Nothing to report
    else {
        gOmmData->mario->state.freeze.dmg = 0;
        gOmmData->mario->state.freeze.gfx = 0;
    }

    return damage;
}

//
// Odyssey oxygen level
//

static void increase_O2_level(s32 inc) {
    if (gOmmData->mario->state.o2 >= OMM_O2_MAX_DURATION / 100) { // <= 99%
        play_sound(SOUND_MENU_POWER_METER, gMarioObject->oCameraToObject);
    }
    gOmmData->mario->state.o2 = max_s(0, gOmmData->mario->state.o2 - inc);
}

static void decrease_O2_level(s32 dec) {
    gOmmData->mario->state.o2 = min_s(OMM_O2_MAX_DURATION, gOmmData->mario->state.o2 + dec);
}

static void omm_update_O2_level(struct MarioState *m) {

    // Mario is healed -> gives 1 extra second of O2 per heal tick
    if (m->healCounter > 0) {
        increase_O2_level(OMM_O2_HEAL_INC);
        if (gOmmData->mario->state.o2 == 0) {
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
            if (gOmmData->mario->state.o2 >= OMM_O2_MAX_DURATION) {
                m->health = OMM_HEALTH_ODYSSEY_DEAD;
                m->healCounter = 0;
                m->hurtCounter = 0;
            }
        }

        // Water
        else if ((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) {

            // Above water
            // God Mode turns Mario into a fish
            if (OMM_CHEAT_GOD_MODE || (m->pos[1] >= (m->waterLevel - 140))) {
                increase_O2_level(OMM_O2_HEAL_INC * 2);
            }

            // Under water
            else {

                // Drains more O2 if Mario is plunging or dashing
                if (m->action == ACT_OMM_WATER_GROUND_POUND) {
                    decrease_O2_level(OMM_O2_WATER_POUND_DEC);
                } else if (m->action == ACT_OMM_WATER_DASH) {
                    decrease_O2_level(OMM_O2_WATER_DASH_DEC);
                } else {
                    decrease_O2_level(OMM_O2_WATER_DEC);
                }

                // Insta-kill if out of breath in 1 HP mode
                if (g1HPMode && gOmmData->mario->state.o2 >= OMM_O2_MAX_DURATION) {
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
        if ((gOmmData->mario->state.o2 >= (OMM_O2_MAX_DURATION * 3) / 4) && !omm_peach_vibe_is_calm()) {
            play_sound(SOUND_MOVING_ALMOST_DROWNING, gGlobalSoundArgs);
        }
    }
}

//
// Odyssey health system
//

static void omm_health_update_classic(struct MarioState *m) {

    // Hard mode: Prevent the game from draining health in water or poison
    // by setting both heal counter and hurt counter to non-zero values.
    // If the hurt counter is non-zero, prevent damage cancelation by
    // setting the heal counter to 0.
    if (g1HPMode) {
        if (m->hurtCounter == 0) {
            m->healCounter = 1;
            m->hurtCounter = 1;
        } else {
            m->healCounter = 0;
        }
    }

    // Drain even more health in cold water
    // 8 HP per frame ~= 1 segment per second, meaning that Mario cannot
    // stay any longer than 8 seconds in cold water without healing
    if (!omm_peach_vibe_is_calm() &&
        (m->action & ACT_FLAG_SWIMMING) &&
       !(m->action & ACT_FLAG_INTANGIBLE) &&
        (m->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW) {

        // 1 HP mode prevents health drain underwater, so the freeze
        // effect must be used instead to damage Mario in cold water
        if (g1HPMode) {
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
    m->action &= ~(ACT_GROUP_MASK * ((m->flags & MARIO_METAL_CAP) || g1HPMode));
    update_mario_health(m);
    m->action = action;
    m->health = clamp_s(m->health, 0xFF, g1HPMode ? OMM_HEALTH_CLASSIC_1_HP : OMM_HEALTH_CLASSIC_MAX);
}

void omm_health_update(struct MarioState *m) {
    if (OMM_MOVESET_CLASSIC) {
        omm_health_update_classic(m);
        return;
    }

    // Hard mode: only 1 health point
    if (g1HPMode && m->health > OMM_HEALTH_ODYSSEY_1_HP) {
        m->health = OMM_HEALTH_ODYSSEY_1_HP;
        gOmmData->mario->state.hp = omm_health_to_hp(m->health);
    }

    // Entered a new area
    if (gOmmData->mario->state.hp == 0) {
        gOmmData->mario->state.hp = omm_health_to_hp(m->health);
        gOmmData->mario->state.coins = (u16) m->numCoins;
    }

    // Update health system
    // Skip this part during the Life-up cutscene
    bool isLifeUpCutscene = (obj_get_first_with_behavior(omm_bhv_life_up) != NULL);
    if (!isLifeUpCutscene) {

        // O2 level
        omm_update_O2_level(m);

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
        if (gOmmData->mario->state.coins < (u16) m->numCoins) {
            gOmmData->mario->state.coins++;
            if ((gOmmData->mario->state.coins % OMM_HEALTH_ODYSSEY_NUM_COINS_PER_HEAL) == 0) {
                omm_health_heal_mario(m);
            }
        } else {
            gOmmData->mario->state.coins = (u16) m->numCoins;
        }
    }

    // Update health counter
    m->health = omm_fix_health(m->health);
    s32 counter = omm_health_to_hp(m->health);
    if (gOmmData->mario->state.hp < counter) {
        gOmmData->mario->state.hp++;
        if (!g1HPMode && !isLifeUpCutscene && (gOmmData->mario->state.hp % OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT) == (OMM_HEALTH_ODYSSEY_NUM_TICKS_PER_SEGMENT / 2)) {
            omm_sound_play(OMM_SOUND_EFFECT_HEAL, NULL);
        }
    } else if (gOmmData->mario->state.hp > counter) {
        gOmmData->mario->state.hp--;
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
        gOmmData->mario->state.hp = max_s(gOmmData->mario->state.hp, 1);
        m->health = omm_health_get_max(m->health);
    } else {
        m->healCounter = 31;
        m->hurtCounter = 0;
    }
}

void omm_health_life_up(struct MarioState *m) {
    omm_ssc_inc(OMM_SSC_C_MUSHROOM, 1);
    if (OMM_MOVESET_ODYSSEY) {
        if (!g1HPMode) {
            if (OMM_MOVESET_ODYSSEY_3H && (m->health <= omm_health_get_max(0))) {
                m->hurtCounter = 0;
                gOmmData->mario->state.hp = omm_health_to_hp(omm_health_get_max(0));
                omm_spawn_life_up(m->marioObj);
            }
            m->health = OMM_HEALTH_ODYSSEY_MAX;
        }
        m->healCounter = OMM_O2_REFILL;
    } else if (!g1HPMode) {
        play_sound(SOUND_GENERAL_COLLECT_1UP, gGlobalSoundArgs);
        m->healCounter = 31;
    }
    m->hurtCounter = 0;
}

void omm_health_set(struct MarioState *m, s16 hp) {
    m->health = hp;
    if (OMM_MOVESET_ODYSSEY) {
        m->health = omm_fix_health(m->health);
        gOmmData->mario->state.hp = omm_health_to_hp(m->health);
    }
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

        // Health system was changed
        // Set m->health, init gOmmData->mario->state.hp and gOmmData->mario->state.coins
        if (gOmmMovesetType != sOmmHealthPreviousState) {
            m->health = min_s(m->health, omm_health_get_max(0));
            gOmmData->mario->state.hp = omm_health_to_hp(m->health);
            gOmmData->mario->state.coins = (u16) m->numCoins;
            sOmmHealthPreviousState = gOmmMovesetType;
        }
    }
}
