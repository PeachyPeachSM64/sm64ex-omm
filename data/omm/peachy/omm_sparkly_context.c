#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

OmmSparklyContext gOmmSparklyContext[1];

void omm_sparkly_state_set(s32 state, bool sound) {
    if (sound && state != OMM_SPARKLY_STATE_OK && OMM_SPARKLY_STATE_IS_OK) {
        play_buzz_sound();
    }
    gOmmSparklyContext->state = max_s(gOmmSparklyContext->state, state);
}

s32 omm_sparkly_context_get_data(u64 flags) {
    if (gOmmSparklyContext->data) {
        switch (flags) {
            case OMM_SPARKLY_DATA_COINS:       return OMM_SPARKLY_DATA_GET_COINS(gOmmSparklyContext->data->flags);
            case OMM_SPARKLY_DATA_RED_COINS:   return OMM_SPARKLY_DATA_GET_RED_COINS(gOmmSparklyContext->data->flags);
            case OMM_SPARKLY_DATA_BUTTONS:     return OMM_SPARKLY_DATA_GET_BUTTONS(gOmmSparklyContext->data->flags);
            case OMM_SPARKLY_DATA_MUSHROOMS:   return OMM_SPARKLY_DATA_GET_MUSHROOMS(gOmmSparklyContext->data->flags);
            case OMM_SPARKLY_DATA_OBJECT_TYPE: return OMM_SPARKLY_DATA_GET_OBJECT_TYPE(gOmmSparklyContext->data->flags);
            default:                           return (gOmmSparklyContext->data->flags & flags) != 0;
        }
    }
    return 0;
}

s32 omm_sparkly_context_get_remaining_flames() {
    return obj_get_count_with_behavior(bhvOmmBigFlame);
}

s32 omm_sparkly_context_get_remaining_boxes() {
    s32 remainingBoxes = obj_get_count_with_behavior(bhvExclamationBox);
    for_each_object_with_behavior(obj, bhvExclamationBox) {
        remainingBoxes -= (!obj_is_dormant(obj) && obj->oBehParams2ndByte <= 2);
    }
    return remainingBoxes;
}

s32 omm_sparkly_context_get_remaining_star_rings() {
    return obj_get_count_with_behavior_and_field_s32(bhvOmmStarRing, _FIELD(oAction), 1);
}

s32 omm_sparkly_context_get_remaining_enemies() {
    s32 remainingEnemies = 0;
    omm_array_for_each(omm_obj_get_sparkly_enemy_behaviors(), p_bhv) {
        const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            remainingEnemies += (!obj_is_dormant(obj) && !(obj->oFlags & OBJ_FLAG_SPARKLY_NOT_ENEMY));
        }
    }
    return remainingEnemies;
}

void omm_sparkly_context_reset_data() {
    if (gOmmSparklyContext->star) {
        obj_mark_for_deletion(gOmmSparklyContext->star);
    }
    gOmmSparklyContext->data = NULL;
    gOmmSparklyContext->spawner = NULL;
    gOmmSparklyContext->star = NULL;
}

bool omm_sparkly_context_check_star(struct Object *star) {
    if (!gOmmSparklyContext->data || !gOmmSparklyContext->star || gOmmSparklyContext->star != star) {
        if (star) obj_mark_for_deletion(star);
        gOmmSparklyContext->star = NULL;
        return false;
    }
    return true;
}

void omm_sparkly_context_spawn_star(struct MarioState *m) {
    if (OMM_SPARKLY_STATE_IS_OK && gOmmSparklyContext->data && !gOmmSparklyContext->star) {

        // Spawn the star only if not condition star or condition is fulfilled
        bool isConditionStar = (gOmmSparklyContext->data->flags & OMM_SPARKLY_DATA_CONDITION) != 0;
        if (!isConditionStar || gOmmSparklyContext->successful) {
            gOmmSparklyContext->star = omm_obj_spawn_sparkly_star(
                gOmmSparklyContext->spawner ? gOmmSparklyContext->spawner : m->marioObj,
                gOmmSparklyMode,
                gOmmSparklyContext->data->starX,
                gOmmSparklyContext->data->starY,
                gOmmSparklyContext->data->starZ,
                isConditionStar
            );
            gOmmSparklyContext->starSpawned |= isConditionStar;
        }
    }
}

static s16 omm_sparkly_context_get_star_dialog_id(s32 starIndex) {
    s16 dialogId;
    if (OMM_SPARKLY_IS_MODE_COMPLETED) {
        dialogId = OMM_DIALOG_SPARKLY_STAR_COMPLETED(gOmmSparklyMode, starIndex);
        if (omm_dialog_get_entry(NULL, dialogId)) {
            return dialogId;
        }
    }
    dialogId = OMM_DIALOG_SPARKLY_STAR(gOmmSparklyMode, starIndex);
    if (omm_dialog_get_entry(NULL, dialogId)) {
        return dialogId;
    }
    return 0;
}

void omm_sparkly_context_spawn_sign(struct MarioState *m) {
    if (OMM_SPARKLY_STATE_IS_OK && gOmmSparklyContext->data) {

        // Spawn sign
        if (!obj_get_first_with_behavior(bhvOmmSparklyStarHint)) {
            s16 dialogId = omm_sparkly_context_get_star_dialog_id(gOmmSparklyContext->data->starIndex);
            if (dialogId) {
                omm_obj_spawn_sparkly_star_hint(m->marioObj,
                    gOmmSparklyMode,
                    gOmmSparklyContext->data->signX,
                    gOmmSparklyContext->data->signY,
                    gOmmSparklyContext->data->signZ,
                    gOmmSparklyContext->data->signA,
                    dialogId
                );
            }
        }

        // Make objects awake after Mario finishes reading the hint sign
        if (m->prevAction == ACT_READING_SIGN && m->interactObj && m->interactObj->behavior == bhvOmmSparklyStarHint) {
            for_each_object_with_behavior(obj, bhvOmmRisingLava) { obj_set_dormant(obj, false); }
            for_each_object_with_behavior(obj, bhvRedCoin) { obj_set_dormant(obj, false); }
            for_each_object_with_behavior(obj, bhvGoomba) { obj_set_dormant(obj, false); }
            for_each_object_with_behavior(obj, bhvBobomb) { obj_set_dormant(obj, false); }
        }
    }
}

//
// Init
//

OMM_ROUTINE_LEVEL_ENTRY(omm_sparkly_context_init) {
    gOmmSparklyMode = OMM_SPARKLY_STARS_MODE;
    if (OMM_SPARKLY_MODE_IS_ENABLED) {
        omm_sparkly_context_reset();
        omm_sparkly_turn_off_cheats();
    }
}

//
// Update
//

static s32 omm_sparkly_context_display_hint_at_level_entry(struct MarioState *m, const OmmSparklyData *data, s32 lastEntryDialogStarIndex) {
    if (!data) {
        data = gOmmSparklyData[gOmmSparklyMode];
        for (s32 i = 0, n = omm_sparkly_get_bowser_4_index(gOmmSparklyMode); i != n; ++i, data++) {
            if (data->gameType == OMM_GAME_TYPE && data->levelNum == gCurrLevelNum) {
                break;
            }
        }
    }
    s16 dialogId = omm_sparkly_context_get_star_dialog_id(data->starIndex);
    if (dialogId) {
        switch (data->hint) {
            case OMM_SPARKLY_HINT_NEVER: {
            } break;

            case OMM_SPARKLY_HINT_ALWAYS: {
                omm_mario_set_action(m, ACT_READING_AUTOMATIC_DIALOG, dialogId, 0xFFFF);
            } break;

            case OMM_SPARKLY_HINT_AREA_1:
            case OMM_SPARKLY_HINT_AREA_2:
            case OMM_SPARKLY_HINT_AREA_3:
            case OMM_SPARKLY_HINT_AREA_4: {
                s32 entryDialogStarIndex = data->starIndex + 128 * gOmmSparklyMode;
                if (entryDialogStarIndex != lastEntryDialogStarIndex) {
                    if (gCurrAreaIndex - 1 == data->hint - OMM_SPARKLY_HINT_AREA_1 && (
                        OMM_SPARKLY_STARS_HINT_ALWAYS || (
                        OMM_SPARKLY_STARS_HINT_NOT_COLLECTED &&
                        !omm_sparkly_is_star_collected(gOmmSparklyMode, data->starIndex)))
                    ) {
                        gPlayer1Controller->buttonPressed = 0;
                        gPlayer2Controller->buttonPressed = 0;
                        gPlayer3Controller->buttonPressed = 0;
                        level_set_transition(-1, NULL);
                        create_dialog_box(dialogId);
                        return entryDialogStarIndex;
                    }
                    return -1;
                }
                return lastEntryDialogStarIndex;
            }
        }
    }
    return -1;
}

static void omm_sparkly_context_skip_regular_star_cutscene() {
    if (OMM_SPARKLY_STATE_IS_OK && (gCamera->cutscene == CUTSCENE_STAR_SPAWN || gCamera->cutscene == CUTSCENE_RED_COIN_STAR_SPAWN) && gCutsceneFocus && gCutsceneFocus->behavior != bhvOmmSparklyStar) {
        stop_cutscene(gCamera);
        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        gCutsceneFocus->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        gCutsceneFocus = NULL;
    }
}

void omm_sparkly_context_update(struct MarioState *m) {
    s32 starIndex = omm_sparkly_get_star_index(gOmmSparklyMode, gCurrLevelNum, gCurrAreaIndex);
    const OmmSparklyData *data = (starIndex != -1 ? &gOmmSparklyData[gOmmSparklyMode][starIndex] : NULL);

    // Area
    // Reset some values when entering a new area, unless the data->areaIndex is 0
    // Invalidate the context if a timer was started
    static s32 sPrevAreaIndex = -1;
    if (sPrevAreaIndex != gCurrAreaIndex) {
        sPrevAreaIndex = gCurrAreaIndex;
        if (!data || data->areaIndex != 0) {
            if (gOmmSparklyContext->timerStarted) {
                omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
                gHudDisplay.flags &= ~HUD_DISPLAY_FLAG_TIMER;
            }
            gOmmSparklyContext->inited = false;
            gOmmSparklyContext->successful = false;
            gOmmSparklyContext->starSpawned = false;
            gOmmSparklyContext->objectsSpawned = false;
            gOmmSparklyContext->entryDialog = false;
            gOmmSparklyContext->timerStarted = false;
            gOmmSparklyContext->eventStarted = false;
            gOmmSparklyContext->eventFlags = 0;
            gOmmSparklyContext->coinsYellow = 0;
            gOmmSparklyContext->coinsLost = 0;
            gOmmSparklyContext->coinsRed = 0;
            gOmmSparklyContext->coinsBlue = 0;
            gOmmSparklyContext->mushrooms = 0;
            gOmmSparklyContext->secrets = 0;
        }
    }

    // Data
    // If the data changes, unload the star and update the context data
    // Leave the current capture if the specific flag is set
    if (gOmmSparklyContext->data != data || !data) {
        gOmmSparklyContext->data = data;
        gOmmSparklyContext->spawner = m->marioObj;
        gOmmSparklyContext->star = NULL;
        if (data && (data->flags & OMM_SPARKLY_DATA_UNCAPTURE) && omm_mario_is_capture(m)) {
            struct Object *capture = gOmmCapture;
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
            obj_mark_for_deletion(capture);
        }
    }

    // Per level
    u64 dataFlags = 0llu;
    if (data) {
        dataFlags = data->flags;
        gOmmSparklyContext->secrets = obj_get_count_with_behavior(bhvHiddenStarTrigger);
        gOmmSparklyContext->coinsYellowDiff = gOmmSparklyContext->coinsYellow - gOmmSparklyContext->coinsYellowPrev;
        gOmmSparklyContext->coinsLostDiff = gOmmSparklyContext->coinsLost - gOmmSparklyContext->coinsLostPrev;
        gOmmSparklyContext->coinsRedDiff = gOmmSparklyContext->coinsRed - gOmmSparklyContext->coinsRedPrev;
        gOmmSparklyContext->coinsBlueDiff = gOmmSparklyContext->coinsBlue - gOmmSparklyContext->coinsBluePrev;
        gOmmSparklyContext->mushroomsDiff = gOmmSparklyContext->mushrooms - gOmmSparklyContext->mushroomsPrev;
        gOmmSparklyContext->secretsDiff = gOmmSparklyContext->secrets - gOmmSparklyContext->secretsPrev;

        // Coin requirement
        s32 requiredCoins = OMM_SPARKLY_DATA_GET_COINS(dataFlags);
        if (requiredCoins) {
            s32 collectedCoins = (
                gOmmSparklyContext->coinsYellow * 1 +
                gOmmSparklyContext->coinsRed * 2 +
                gOmmSparklyContext->coinsBlue * 5
            );
            gOmmSparklyContext->successful |= (collectedCoins >= requiredCoins);
            omm_sparkly_context_skip_regular_star_cutscene();
        }

        // All red coins
        s32 requiredRedCoins = OMM_SPARKLY_DATA_GET_RED_COINS(dataFlags);
        if (requiredRedCoins) {
            s32 collectedRedCoins = gOmmSparklyContext->coinsRed;
            gOmmSparklyContext->successful |= (collectedRedCoins >= requiredRedCoins);
            omm_sparkly_context_skip_regular_star_cutscene();
        }

        // Level update
        omm_sparkly_context_update_level(m, data);
    }

    // Show the Sparkly Star hint on level entry
    // The context state must be ok, the dialog not already triggered, the game not paused,
    // the game not in transition, Mario not in a airborne or cutscene action
    static s32 sLastEntryDialogStarIndex = -1;
    if (OMM_SPARKLY_STATE_IS_OK && !gOmmSparklyContext->entryDialog && !gOmmSparkly->gamePaused && !gOmmSparkly->transition &&
        (((m->action & ACT_GROUP_MASK) != ACT_GROUP_CUTSCENE) || !(m->action & ACT_FLAG_AIR))) {
        sLastEntryDialogStarIndex = omm_sparkly_context_display_hint_at_level_entry(m, data, sLastEntryDialogStarIndex);
        gPlayer1Controller->buttonPressed &= ~START_BUTTON;
        gPlayer2Controller->buttonPressed &= ~START_BUTTON;
        gPlayer3Controller->buttonPressed &= ~START_BUTTON;
        gOmmSparklyContext->entryDialog = true;
    }

    // Anti-cheats
    // Kills Mario if a cheat is detected
    // Update only when the game is not paused
    // If detected, doesn't check for cheats until the Bowser dialog is done
    if (!gOmmSparkly->gamePaused && !gOmmSparkly->cheatDetected) {
        gOmmSparklyContext->cheatDetected |= omm_sparkly_check_cheats(m);

        // Doesn't trigger the anti-cheat during transitions or if Mario isn't updated (time stop)
        if (!gOmmSparkly->transition && gOmmSparkly->marioUpdated && gOmmSparklyContext->cheatDetected) {
            gOmmSparkly->cheatDetected = true;
            gOmmSparklyCheats->counter++;
            gOmmSparklyContext->cheatDetected = false;
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 0);
            omm_sparkly_turn_off_cheats();
            omm_obj_spawn_problem(m->marioObj);
        }
    }

    // Rules
    // Collecting all Sparkly Stars of the current difficulty
    // allows the player to bypass the rules
    if (!OMM_SPARKLY_BYPASS_RULES) {

        // No caps
        if (((m->flags & MARIO_WING_CAP) && !(dataFlags & OMM_SPARKLY_DATA_WING_CAP)) ||
            ((m->flags & MARIO_METAL_CAP) && !(dataFlags & OMM_SPARKLY_DATA_METAL_CAP)) ||
            ((m->flags & MARIO_VANISH_CAP) && !(dataFlags & OMM_SPARKLY_DATA_VANISH_CAP))) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // No Yoshi summon
        if (obj_get_first_with_behavior(bhvOmmYoshi) ||
            obj_get_first_with_behavior(bhvOmmYoshiBubble) ||
            obj_get_first_with_behavior(bhvOmmYoshiFire) ||
            obj_get_first_with_behavior(bhvOmmYoshiFireball) ||
            obj_get_first_with_behavior(bhvOmmYoshiEgg) ||
            obj_get_first_with_behavior(bhvOmmYoshiTongue) ||
            obj_get_first_with_behavior(bhvOmmYoshiWings)) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // No cannon outside Normal mode
        if (!OMM_SPARKLY_MODE_IS_NORMAL && (m->action == ACT_IN_CANNON || (omm_mario_is_capture(m) && gOmmObject->state._cannonTimer > 0))) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // No Vibe outside Lunatic mode
        if (!OMM_SPARKLY_MODE_IS_LUNATIC && omm_peach_vibe_is_active()) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }
    }

    // Restrictions
    // In Normal mode, no longer checks restrictions after the star spawn
    if (gOmmSparklyContext->inited && !OMM_SPARKLY_BYPASS_RESTRICTIONS) {

        // Cap power required during cap courses
        if (((dataFlags & OMM_SPARKLY_DATA_WING_CAP) && !(m->flags & MARIO_WING_CAP)) ||
            ((dataFlags & OMM_SPARKLY_DATA_METAL_CAP) && !(m->flags & MARIO_METAL_CAP)) ||
            ((dataFlags & OMM_SPARKLY_DATA_VANISH_CAP) && !(m->flags & MARIO_VANISH_CAP))) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // Only yellow coins
        if ((dataFlags & OMM_SPARKLY_DATA_ONLY_COIN_Y) && (gOmmSparklyContext->coinsRed || gOmmSparklyContext->coinsBlue)) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // Only red coins
        if ((dataFlags & OMM_SPARKLY_DATA_ONLY_COIN_R) && (gOmmSparklyContext->coinsYellow || gOmmSparklyContext->coinsLost || gOmmSparklyContext->coinsBlue)) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // Only blue coins
        if ((dataFlags & OMM_SPARKLY_DATA_ONLY_COIN_B) && (gOmmSparklyContext->coinsYellow || gOmmSparklyContext->coinsLost || gOmmSparklyContext->coinsRed)) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // No mushroom 1-up
        if ((dataFlags & OMM_SPARKLY_DATA_NO_MUSHROOM) && gOmmSparklyContext->mushrooms) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // No capture
        if ((dataFlags & OMM_SPARKLY_DATA_NO_CAPTURE) && (omm_mario_is_capture(m) || gOmmCapture)) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // No Vibe
        if ((dataFlags & OMM_SPARKLY_DATA_NO_VIBE) && omm_peach_vibe_is_active()) {
            omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
        }

        // No button press
        if (gOmmSparkly->marioUpdated && !gOmmSparkly->transition) {
            u16 buttons = (m->controller->buttonPressed | m->controller->buttonDown);
            u16 noButtons = OMM_SPARKLY_DATA_GET_BUTTONS(dataFlags);
            if (!omm_mario_is_reading(m) && (buttons & noButtons) != 0) {
                omm_sparkly_state_set(OMM_SPARKLY_STATE_INVALID, 1);
            }
        }
    }

    // Star
    // Unload if data is NULL or state == OMM_SPARKLY_STATE_INVALID
    // Make it intangible if already spawned and state == OMM_SPARKLY_STATE_FAIL
    if (!data || gOmmSparklyContext->state == OMM_SPARKLY_STATE_INVALID) {
        gOmmSparklyContext->star = NULL;
    } else if (gOmmSparklyContext->star && gOmmSparklyContext->state == OMM_SPARKLY_STATE_FAIL) {
        gOmmSparklyContext->star->oAction = 3;
    }

    // Update counters
    gOmmSparklyContext->coinsYellowPrev = gOmmSparklyContext->coinsYellow;
    gOmmSparklyContext->coinsLostPrev = gOmmSparklyContext->coinsLost;
    gOmmSparklyContext->coinsRedPrev = gOmmSparklyContext->coinsRed;
    gOmmSparklyContext->coinsBluePrev = gOmmSparklyContext->coinsBlue;
    gOmmSparklyContext->mushroomsPrev = gOmmSparklyContext->mushrooms;
    gOmmSparklyContext->secretsPrev = gOmmSparklyContext->secrets;
}

void omm_sparkly_context_reset() {
    gPlayer1Controller->buttonPressed *= (gCurrCourseNum == COURSE_NONE);
    gPlayer1Controller->buttonDown *= (gCurrCourseNum == COURSE_NONE);
    gOmmSparklyMode = OMM_SPARKLY_STARS_MODE;
    mem_zero(gOmmSparklyContext, sizeof(gOmmSparklyContext));
    if (!OMM_SPARKLY_IS_MODE_COMPLETED) {
        gOmmWarp->state = POBJ_WARP_STATE_NOT_WARPING;
    }
}
