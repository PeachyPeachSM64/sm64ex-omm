#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Context struct
//

static struct {
    const OmmSSD *data;
    struct Object *star;
    s32 state;
    s32 flags;
    s32 counters[2][OMM_SSC_C_COUNT];
} sOmmSSC[1];

//
// Context functions
//

bool omm_ssc_get(s32 flag) {
    return (sOmmSSC->flags & flag) != 0;
}

void omm_ssc_set(s32 flag, bool cond) {
    if (cond) {
        sOmmSSC->flags |= flag;
    }
}

void omm_ssc_clr(s32 flag) {
    sOmmSSC->flags &= ~(flag);
}

s32 omm_ssc_cnt(s32 counter) {
    return sOmmSSC->counters[0][counter];
}

void omm_ssc_mov(s32 counter, s32 value) {
    sOmmSSC->counters[0][counter] = value;
}

void omm_ssc_inc(s32 counter, s32 inc) {
    sOmmSSC->counters[0][counter] += inc;
}

s32 omm_ssc_dif(s32 counter) {
    return sOmmSSC->counters[0][counter] - sOmmSSC->counters[1][counter];
}

bool omm_ssc_state_ok() {
    return sOmmSSC->state == OMM_SSC_OK;
}

void omm_ssc_state_set(s32 state, bool sound) {
    if (sound && state != OMM_SSC_OK && OMM_SSC_IS_OK) {
        play_sound(SOUND_MENU_CAMERA_BUZZ | 0xFF00, gGlobalSoundArgs);
    }
    sOmmSSC->state = max_s(sOmmSSC->state, state);
}

s32 omm_ssc_data_flags(u64 flags) {
    if (sOmmSSC->data) {
        switch (flags) {
            case OMM_SSD_COINS:     return OMM_SSD_GET_COINS(sOmmSSC->data->flags);
            case OMM_SSD_RED_COINS: return OMM_SSD_GET_RED_COINS(sOmmSSC->data->flags);
            case OMM_SSD_BUTTONS:   return OMM_SSD_GET_BUTTONS(sOmmSSC->data->flags);
            case OMM_SSD_MUSHROOMS: return OMM_SSD_GET_MUSHROOMS(sOmmSSC->data->flags);
            default:                return OMM_GET_FLAG(sOmmSSC->data->flags, flags);
        }
    }
    return 0;
}

s32 omm_ssc_data_flames() {
    return obj_get_count_with_behavior(bhvFlame);
}

s32 omm_ssc_data_boxes() {
    s32 remainingBoxes = obj_get_count_with_behavior(bhvExclamationBox);
    for_each_object_with_behavior(obj, bhvExclamationBox) {
        remainingBoxes -= (!obj_is_dormant(obj) && obj->oBehParams2ndByte <= 2);
    }
    return remainingBoxes;
}

s32 omm_ssc_data_rings() {
    return obj_get_count_with_behavior_and_field_s32(omm_bhv_star_ring, 0x31, 1);
}

void omm_ssc_data_reset() {
    if (sOmmSSC->star) {
        obj_mark_for_deletion(sOmmSSC->star);
    }
    sOmmSSC->data = NULL;
    sOmmSSC->star = NULL;
}

bool omm_ssc_check_star(struct Object *star) {
    if (!sOmmSSC->data || !sOmmSSC->star || sOmmSSC->star != star) {
        if (star) obj_mark_for_deletion(star);
        sOmmSSC->star = NULL;
        return false;
    }
    return true;
}

void omm_ssc_spawn_star(struct MarioState *m) {
    if (OMM_SSC_IS_OK && sOmmSSC->data && !sOmmSSC->star) {

        // Spawn the star only if not condition star or condition is fulfilled
        bool isConditionStar = (sOmmSSC->data->flags & OMM_SSD_CONDITION) != 0;
        if (!isConditionStar || OMM_SSC_IS_SUCCESSFUL) {
            sOmmSSC->star = omm_spawn_sparkly_star(m->marioObj,
                gOmmSSM,
                sOmmSSC->data->starX,
                sOmmSSC->data->starY,
                sOmmSSC->data->starZ,
                isConditionStar
            );
            omm_ssc_set(OMM_SSC_F_STAR_SPAWNED, isConditionStar);
        }
    }
}

void omm_ssc_spawn_sign(struct MarioState *m) {
    if (OMM_SSC_IS_OK && sOmmSSC->data) {

        // Spawn sign
        s16 dialogId = OMM_DIALOG_SPARKLY_STAR + sOmmSSC->data->index;
        if (!obj_get_first_with_behavior(omm_bhv_sparkly_star_hint) && omm_get_dialog_entry(NULL, dialogId)) {
            omm_spawn_sparkly_star_hint(m->marioObj,
                gOmmSSM,
                sOmmSSC->data->signX,
                sOmmSSC->data->signY,
                sOmmSSC->data->signZ,
                sOmmSSC->data->signA,
                dialogId
            );
        }

        // Make objects awake after Mario finishes reading the hint sign
        if (m->prevAction == ACT_READING_SIGN && m->interactObj && m->interactObj->behavior == omm_bhv_sparkly_star_hint) {
            for_each_object_with_behavior(obj, omm_bhv_rising_lava) { obj_set_dormant(obj, false); }
            for_each_object_with_behavior(obj, bhvRedCoin) { obj_set_dormant(obj, false); }
            for_each_object_with_behavior(obj, bhvGoomba) { obj_set_dormant(obj, false); }
        }
    }
}

//
// Init
//

OMM_ROUTINE_LEVEL_ENTRY(omm_ssc_init) {
    gOmmSSM = OMM_SPARKLY_STARS_MODE;
    if (OMM_SSM_IS_ENABLED) {
        omm_ssc_reset();
        omm_ssd_turn_off_cheats();
    }
}

//
// Update
//

static s32 omm_ssc_display_hint_at_level_entry(struct MarioState *m, const OmmSSD *data, s32 lastEntryDialogStarIndex) {
    if (data && data->showOnEntry) {
        s16 dialogId = OMM_DIALOG_SPARKLY_STAR + data->index;
        if (omm_get_dialog_entry(NULL, dialogId)) {
            if (data->showOnEntry == 2) {
                omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, dialogId, 0xFFFF);
            } else if (OMM_SPARKLY_STARS_HINT) {
                s32 entryDialogStarIndex = data->index + 30 * gOmmSSM;
                if (entryDialogStarIndex != lastEntryDialogStarIndex) {
                    level_set_transition(-1, NULL);
                    create_dialog_box(dialogId);
                }
                return entryDialogStarIndex;
            }
        }
    }
    return -1;
}

void omm_ssc_update(struct MarioState *m) {
    s32 index = omm_ssd_get_star_index(gOmmSSM, gCurrLevelNum, gCurrAreaIndex);
    const OmmSSD *data = (index != -1 ? &gOmmSSD[gOmmSSM][index] : NULL);

    // Area
    // Reset some values when entering a new area, unless the data->area is 0
    // Invalidate the context if a timer was started
    // Restart the entire context inside the Castle
    static s32 sPrevAreaIndex = -1;
    if (sPrevAreaIndex != gCurrAreaIndex) {
        sPrevAreaIndex = gCurrAreaIndex;
        if (!data || data->area != 0) {
            if (OMM_SSC_IS_TIMER_STARTED) {
                omm_ssc_state_set(OMM_SSC_INVALID, 1);
                gHudDisplay.flags &= ~HUD_DISPLAY_FLAG_TIMER;
            }
            if (gCurrCourseNum == COURSE_NONE) {
                omm_ssc_reset();
            } else {
                omm_ssc_clr(OMM_SSC_F_INITED);
                omm_ssc_clr(OMM_SSC_F_SUCCESSFUL);
                omm_ssc_clr(OMM_SSC_F_STAR_SPAWNED);
                omm_ssc_clr(OMM_SSC_F_OBJECTS_SPAWNED);
                omm_ssc_clr(OMM_SSC_F_ENTRY_DIALOG);
                omm_ssc_clr(OMM_SSC_F_TIMER_STARTED);
                omm_ssc_clr(OMM_SSC_F_EVENT_STARTED);
                omm_ssc_mov(OMM_SSC_C_COIN_Y, 0);
                omm_ssc_mov(OMM_SSC_C_COIN_L, 0);
                omm_ssc_mov(OMM_SSC_C_COIN_B, 0);
                omm_ssc_mov(OMM_SSC_C_COIN_R, 0);
                omm_ssc_mov(OMM_SSC_C_MUSHROOM, 0);
                omm_ssc_mov(OMM_SSC_C_SECRET, 0);
            }
        }
    }

    // Data
    // If the data changes, unload the star and update the context data
    if (sOmmSSC->data != data || !data) {
        sOmmSSC->data = data;
        sOmmSSC->star = NULL;
    }

    // Per level
    u64 dataFlags = 0llu;
    if (data) {
        omm_ssc_mov(OMM_SSC_C_SECRET, obj_get_count_with_behavior(bhvHiddenStarTrigger));
        omm_ssc_update_level(m, data);
        dataFlags = data->flags;

        // Spawn coin arrow
        if (data->spawnCoinArrow && !OMM_SSC_IS_COIN_ARROW_SPAWNED) {
            f32 x = data->coinX;
            f32 y = data->coinY;
            f32 z = data->coinZ;
            f32 dx = coss(data->coinA);
            f32 dz = sins(data->coinA);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x,            y +  80, z,            0, 0, 0);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x,            y + 160, z,            0, 0, 0);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x,            y + 240, z,            0, 0, 0);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x,            y + 320, z,            0, 0, 0);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x,            y + 400, z,            0, 0, 0);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x - dx *  50, y + 350, z - dz *  50, 0, 0, 0);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x - dx * 100, y + 300, z - dz * 100, 0, 0, 0);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x + dx *  50, y + 350, z + dz *  50, 0, 0, 0);
            spawn_object_abs_with_rot(m->marioObj, 0, MODEL_YELLOW_COIN, bhvYellowCoin, x + dx * 100, y + 300, z + dz * 100, 0, 0, 0);
            omm_ssc_set(OMM_SSC_F_COIN_ARROW_SPAWNED, 1);
        }

        // Coin requirement
        s32 requiredCoins = OMM_SSD_GET_COINS(dataFlags);
        if (requiredCoins) {
            s32 collectedCoins = (
                omm_ssc_cnt(OMM_SSC_C_COIN_Y) * 1 +
                omm_ssc_cnt(OMM_SSC_C_COIN_R) * 2 +
                omm_ssc_cnt(OMM_SSC_C_COIN_B) * 5
            );
            omm_ssc_set(OMM_SSC_F_SUCCESSFUL, collectedCoins >= requiredCoins);
        }

        // All red coins
        s32 requiredRedCoins = OMM_SSD_GET_RED_COINS(dataFlags);
        if (requiredRedCoins) {
            s32 collectedRedCoins = omm_ssc_cnt(OMM_SSC_C_COIN_R);
            omm_ssc_set(OMM_SSC_F_SUCCESSFUL, collectedRedCoins >= requiredRedCoins);
        }
    }

    // Show the Sparkly Star hint on level entry
    // The context state must be ok, the dialog not already triggered, the game not paused,
    // the game not in transition, Mario not in a airborne or cutscene action
    static s32 sLastEntryDialogStarIndex = -1;
    if (OMM_SSC_IS_OK && !OMM_SSC_IS_ENTRY_DIALOG && !OMM_SSF_IS_GAME_PAUSED && !OMM_SSF_IS_TRANSITION &&
        (((m->action & ACT_GROUP_MASK) != ACT_GROUP_CUTSCENE) || !(m->action & ACT_FLAG_AIR))) {
        sLastEntryDialogStarIndex = omm_ssc_display_hint_at_level_entry(m, data, sLastEntryDialogStarIndex);
        omm_ssc_set(OMM_SSC_F_ENTRY_DIALOG, 1);
    }

    // Anti-cheats
    // Kills Mario if a cheat is detected
    // Update only when the game is not paused
    // If detected, doesn't check for cheats until the Bowser dialog is done
    if (!OMM_SSF_IS_GAME_PAUSED && !OMM_SSF_IS_CHEAT_DETECTED) {
        omm_ssc_set(OMM_SSC_F_CHEAT_DETECTED, omm_ssd_check_cheats(m));

        // Doesn't trigger the anti-cheat during transitions or if Mario isn't updated (time stop)
        if (!OMM_SSF_IS_TRANSITION && OMM_SSF_IS_MARIO_UPDATED && OMM_SSC_IS_CHEAT_DETECTED) {
            omm_ssf_set(OMM_SSF_CHEAT_DETECTED, 1);
            omm_ssc_clr(OMM_SSC_F_CHEAT_DETECTED);
            omm_ssc_state_set(OMM_SSC_INVALID, 0);
            omm_ssd_turn_off_cheats();

            // Wanna have a bad time?
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, false, 0);
            omm_spawn_problem(m->marioObj);
            if (m->action & ACT_FLAG_AIR) {
                omm_mario_set_action(m, (random_u16() & 1) ? ACT_HARD_FORWARD_AIR_KB : ACT_HARD_BACKWARD_AIR_KB, 1, 0);
            } else if (m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)) {
                omm_mario_set_action(m, (random_u16() & 1) ? ACT_FORWARD_WATER_KB : ACT_BACKWARD_WATER_KB, 1, 0);
            } else {
                omm_mario_set_action(m, (random_u16() & 1) ? ACT_HARD_FORWARD_GROUND_KB : ACT_HARD_BACKWARD_GROUND_KB, 1, 0);
            }
        }
    }

    // Assist mode
    if (OMM_SPARKLY_STARS_ASSIST && OMM_SSC_IS_OK) {

        // Inhibit forbidden inputs
        if (!OMM_SSF_IS_GAME_PAUSED && OMM_SSF_IS_MARIO_UPDATED && !omm_mario_is_reading(m)) {
            u16 noButtons = OMM_SSD_GET_BUTTONS(dataFlags) & ~START_BUTTON;
            gPlayer1Controller->buttonPressed &= ~noButtons;
            gPlayer1Controller->buttonDown &= ~noButtons;
            m->controller->buttonPressed &= ~noButtons;
            m->controller->buttonDown &= ~noButtons;
        }
        
        // Prevent Cappy from capturing objects if Captures are not allowed
        if ((dataFlags & OMM_SSD_NO_CAPTURE) && (m->action == ACT_OMM_POSSESSION || gOmmCapture)) {
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6);
        }

        // Prevent Peach from activating Vibes if Vibes are not allowed
        if (dataFlags & OMM_SSD_NO_VIBE) {
            if (omm_peach_vibe_is_active()) {
                gOmmData->mario->peach.vibeType = OMM_PEACH_VIBE_TYPE_NONE;
                gOmmData->mario->peach.vibeTimer = 0;
            }
            gPlayer1Controller->buttonPressed &= ~Y_BUTTON;
            gPlayer1Controller->buttonDown &= ~Y_BUTTON;
            m->controller->buttonPressed &= ~Y_BUTTON;
            m->controller->buttonDown &= ~Y_BUTTON;
        }
    }

    // Rules
    // Collecting all 30 Sparkly Stars of the current difficulty or spawning
    // a star in Normal mode allows the player to bypass the rules
    if (!omm_ssd_is_completed(gOmmSSM) && (!OMM_SSM_IS_NORMAL || !OMM_SSC_IS_STAR_SPAWNED)) {

        // No caps
        if (((m->flags & MARIO_WING_CAP) && !(dataFlags & OMM_SSD_WING_CAP)) ||
            ((m->flags & MARIO_METAL_CAP) && !(dataFlags & OMM_SSD_METAL_CAP)) ||
            ((m->flags & MARIO_VANISH_CAP) && !(dataFlags & OMM_SSD_VANISH_CAP))) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // No cannon
        if (m->action == ACT_IN_CANNON && !OMM_SSM_IS_NORMAL) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // No Vibe outside Lunatic mode
        if (!OMM_SSM_IS_LUNATIC && omm_peach_vibe_is_active()) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // Peach, Non-Stop mode and 1 HP mode are required in Lunatic mode
        if (OMM_SSM_IS_LUNATIC && (!OMM_PLAYER_IS_PEACH || !OMM_STARS_NON_STOP || !g1HPMode)) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }
    }

    // Requirements
    // In Normal mode, no longer checks requirements after the star spawn
    if (OMM_SSC_IS_INITED && (!OMM_SSM_IS_NORMAL || !OMM_SSC_IS_STAR_SPAWNED)) {

        // Only yellow coins
        if ((dataFlags & OMM_SSD_ONLY_COIN_Y) && (omm_ssc_cnt(OMM_SSC_C_COIN_R) || omm_ssc_cnt(OMM_SSC_C_COIN_B))) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // Only red coins
        if ((dataFlags & OMM_SSD_ONLY_COIN_R) && (omm_ssc_cnt(OMM_SSC_C_COIN_Y) || omm_ssc_cnt(OMM_SSC_C_COIN_L) || omm_ssc_cnt(OMM_SSC_C_COIN_B))) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // Only blue coins
        if ((dataFlags & OMM_SSD_ONLY_COIN_B) && (omm_ssc_cnt(OMM_SSC_C_COIN_Y) || omm_ssc_cnt(OMM_SSC_C_COIN_L) || omm_ssc_cnt(OMM_SSC_C_COIN_R))) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // No mushroom 1-up
        if ((dataFlags & OMM_SSD_NO_MUSHROOM) && omm_ssc_cnt(OMM_SSC_C_MUSHROOM)) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // No capture
        if ((dataFlags & OMM_SSD_NO_CAPTURE) && (m->action == ACT_OMM_POSSESSION || gOmmCapture)) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // No Vibe
        if ((dataFlags & OMM_SSD_NO_VIBE) && omm_peach_vibe_is_active()) {
            omm_ssc_state_set(OMM_SSC_INVALID, 1);
        }

        // No button press
        if (OMM_SSF_IS_MARIO_UPDATED) {
            u16 buttons = (m->controller->buttonPressed | m->controller->buttonDown);
            u16 noButtons = OMM_SSD_GET_BUTTONS(dataFlags);
            if (!omm_mario_is_reading(m) && (buttons & noButtons) != 0) {
                omm_ssc_state_set(OMM_SSC_INVALID, 1);
            }
        }
    }

    // Star
    // Unload if data is NULL or state == OMM_SSC_INVALID
    // Make it intangible if already spawned and state == OMM_SSC_FAIL
    if (!data || sOmmSSC->state == OMM_SSC_INVALID) {
        sOmmSSC->star = NULL;
    } else if (sOmmSSC->star && sOmmSSC->state == OMM_SSC_FAIL) {
        sOmmSSC->star->oAction = 3;
    }

    // Update counters
    OMM_MEMCPY(sOmmSSC->counters[1], sOmmSSC->counters[0], sizeof(sOmmSSC->counters[0]));
}

void omm_ssc_reset() {
    gPlayer1Controller->buttonPressed *= (gCurrCourseNum == COURSE_NONE);
    gPlayer1Controller->buttonDown *= (gCurrCourseNum == COURSE_NONE);
    gOmmSSM = OMM_SPARKLY_STARS_MODE;
    OMM_MEMSET(sOmmSSC, 0, sizeof(sOmmSSC));
}
