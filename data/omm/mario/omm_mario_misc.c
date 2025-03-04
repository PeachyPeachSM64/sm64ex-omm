#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Cap modifier
// Unlocked if all stars of a level or all stars of a game
// Inputs: L + D-Pad
//

static bool omm_mario_try_to_interact_cap(struct MarioState *m, u32 capFlag, const BehaviorScript *capBhv) {
    if (!capFlag || (omm_save_file_get_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE) & capFlag)) {
        static struct Object capObj[1] = {0};
        capObj->behavior = capBhv;
        if (omm_mario_interact_cap(m, capObj)) {
            m->capTimer *= !OMM_CHEAT_CAP_MODIFIER || capBhv == bhvNormalCap;
            return true;
        }
    } else if (capFlag) {
        play_buzz_sound();
    }
    return false;
}

static bool omm_mario_check_cap_modifier(struct MarioState *m) {
    if (OMM_CHEAT_CAP_MODIFIER || (gOmmAllow->capModifier && (OMM_REWARD_IS_UNLIMITED_CAPS_UNLOCKED || OMM_REWARD_IS_INSTANT_CAPS_UNLOCKED))) {
        switch (m->controller->buttonPressed & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
            case U_JPAD: return omm_mario_try_to_interact_cap(m, !OMM_CHEAT_CAP_MODIFIER * SAVE_FLAG_HAVE_WING_CAP, bhvWingCap);
            case L_JPAD: return omm_mario_try_to_interact_cap(m, !OMM_CHEAT_CAP_MODIFIER * SAVE_FLAG_HAVE_VANISH_CAP, bhvVanishCap);
            case R_JPAD: return omm_mario_try_to_interact_cap(m, !OMM_CHEAT_CAP_MODIFIER * SAVE_FLAG_HAVE_METAL_CAP, bhvMetalCap);
            case D_JPAD: return omm_mario_try_to_interact_cap(m, 0, bhvNormalCap);
        }
    } else if (!gOmmAllow->capModifier && (m->controller->buttonPressed & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) != 0) {
        play_buzz_sound();
    }
    return false;
}

//
// Yoshi summon
// Unlocked if all 48 captures registered
// Inputs: L + Y
//

static bool omm_mario_check_yoshi_summon(struct MarioState *m) {
    if (!gOmmGlobals->yoshiMode && gOmmAllow->yoshiSummon && OMM_REWARD_IS_SUMMON_YOSHI_UNLOCKED && !omm_bowser_is_active()) { // Disable Yoshi summon during OMM Bowser fights
        switch (m->controller->buttonPressed & (Y_BUTTON)) {
            case Y_BUTTON: return omm_obj_spawn_yoshi(m->marioObj, m->faceAngle[1]) != NULL;
        }
    } else if (!gOmmAllow->yoshiSummon && (m->controller->buttonPressed & (Y_BUTTON)) != 0) {
        play_buzz_sound();
    }
    return false;
}

//
// Update
//

void omm_mario_update_l_actions(struct MarioState *m) {
    if (!omm_is_ending_cutscene() && (m->controller->buttonDown & L_TRIG) != 0) {
        if (omm_mario_check_cap_modifier(m)) {
            m->controller->buttonPressed &= ~(U_JPAD | D_JPAD | L_JPAD | R_JPAD);
        } else if (!omm_mario_is_capture(m) && omm_mario_check_yoshi_summon(m)) {
            m->controller->buttonPressed &= ~(Y_BUTTON);
        }
    }
}
