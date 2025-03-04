#if OMM_GAME_IS_SM74

//
// Warp to level (init)
//

#define OMM_SM74_NUM_WARPABLE_LEVELS (omm_level_get_count() - 4)
#define OMM_SM74_NUM_WARPABLE_ACTS   (15 * OMM_NUM_ACTS_MAX_PER_COURSE + (OMM_SM74_NUM_WARPABLE_LEVELS - 15))

static struct {
    u32 prevLevel, currLevel;
    u32 prevMode, currMode;
    u32 prevAct, currAct;
} sOmmWarp[1];

#define currArea currMode

static void omm_opt_init_warp_to_level() {
}

static struct Option omm_opt_make_choice_level(const char *label, u32 *value) {
    struct Option opt = {0};
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label);
    opt.uval = value;
    opt.numChoices = OMM_SM74_NUM_WARPABLE_LEVELS * OMM_NUM_SAVE_MODES;
    const u8 **choices = opt.choices = mem_new(const u8 *, opt.numChoices);
    for (s32 modeIndex = OMM_SM74_MODE_NORMAL; modeIndex <= OMM_SM74_MODE_EXTREME; ++modeIndex) {
        for (s32 levelValue = 0; levelValue != OMM_SM74_NUM_WARPABLE_LEVELS; ++levelValue) {
            s32 levelNum = omm_level_get_list()[levelValue];
            ustr_t levelName;
            omm_level_get_course_name(levelName, levelNum, modeIndex, true, true);
            *(choices++) = mem_dup(levelName, omm_opt_text_length(levelName) + 1);
        }
    }
    return opt;
}

static struct Option omm_opt_make_choice_area(UNUSED const char *label, u32 *value) {
    struct Option opt = {0};
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(OMM_TEXT_SM74_OPT_WARP_EDITION);
    opt.uval = value;
    opt.numChoices = OMM_NUM_SAVE_MODES;
    opt.choices = mem_new(const u8 *, opt.numChoices);
    opt.choices[OMM_SM74_MODE_NORMAL] = omm_opt_text(OMM_TEXT_SM74_OPT_WARP_NORMAL);
    opt.choices[OMM_SM74_MODE_EXTREME] = omm_opt_text(OMM_TEXT_SM74_OPT_WARP_EXTREME);
    return opt;
}

static struct Option omm_opt_make_choice_act(const char *label, u32 *value) {
    struct Option opt = {0};
    opt.type = OPT_CHOICE;
    opt.label = omm_opt_text(label);
    opt.uval = value;
    opt.numChoices = OMM_SM74_NUM_WARPABLE_ACTS * OMM_NUM_SAVE_MODES;
    const u8 **choices = opt.choices = mem_new(const u8 *, opt.numChoices);
    for (s32 modeIndex = OMM_SM74_MODE_NORMAL; modeIndex <= OMM_SM74_MODE_EXTREME; ++modeIndex) {
        for (s32 levelValue = 0; levelValue != OMM_SM74_NUM_WARPABLE_LEVELS; ++levelValue) {
            bool isMainCourse = (levelValue < 15);
            s32 levelNum = omm_level_get_list()[levelValue];
            for (s32 actIndex = 0; actIndex != (isMainCourse ? OMM_NUM_ACTS_MAX_PER_COURSE : 1); ++actIndex) {
                ustr_t actName;
                omm_level_get_act_name(actName, levelNum, (isMainCourse ? actIndex + 1 : -1), modeIndex, true, true);
                *(choices++) = mem_dup(actName, omm_opt_text_length(actName) + 1);
            }
        }
    }
    return opt;
}

//
// Warp to level (update)
//

static u32 omm_opt_get_level_value(u32 levelValue, u32 modeValue) {
    return (levelValue % OMM_SM74_NUM_WARPABLE_LEVELS) + modeValue * OMM_SM74_NUM_WARPABLE_LEVELS;
}

static u32 omm_opt_get_act_value(u32 actValue, u32 modeValue) {
    return (actValue % OMM_SM74_NUM_WARPABLE_ACTS) + modeValue * OMM_SM74_NUM_WARPABLE_ACTS;
}

static u32 omm_opt_get_first_act_value(u32 levelValue, u32 modeValue) {
    levelValue %= OMM_SM74_NUM_WARPABLE_LEVELS;
    if (levelValue < 15) return levelValue * OMM_NUM_ACTS_MAX_PER_COURSE + modeValue * OMM_SM74_NUM_WARPABLE_ACTS;
    return 15 * OMM_NUM_ACTS_MAX_PER_COURSE + (levelValue - 15) + modeValue * OMM_SM74_NUM_WARPABLE_ACTS;
}

static u32 omm_opt_get_level_value_from_act_value(u32 actValue, u32 modeValue) {
    actValue %= OMM_SM74_NUM_WARPABLE_ACTS;
    if (actValue >= 15 * OMM_NUM_ACTS_MAX_PER_COURSE) return omm_opt_get_level_value(15 + actValue - 15 * OMM_NUM_ACTS_MAX_PER_COURSE, modeValue);
    return omm_opt_get_level_value(actValue / OMM_NUM_ACTS_MAX_PER_COURSE, modeValue);
}

static s32 omm_opt_get_level_num(u32 levelValue) {
    return omm_level_get_list()[levelValue % OMM_SM74_NUM_WARPABLE_LEVELS];
}

static s32 omm_opt_get_act_num(u32 actValue) {
    actValue %= OMM_SM74_NUM_WARPABLE_ACTS;
    if (actValue >= 15 * OMM_NUM_ACTS_MAX_PER_COURSE) return 1;
    return (actValue % OMM_NUM_ACTS_MAX_PER_COURSE) + 1;
}

static void omm_opt_update_warp_to_level() {

    // Mode changed
    if (sOmmWarp->prevMode != sOmmWarp->currMode) {
        sOmmWarp->currLevel = omm_opt_get_level_value(sOmmWarp->currLevel, sOmmWarp->currMode);
        sOmmWarp->currAct = omm_opt_get_act_value(sOmmWarp->currAct, sOmmWarp->currMode);
    }

    // Level changed
    else if (sOmmWarp->prevLevel != sOmmWarp->currLevel) {
        sOmmWarp->currLevel = omm_opt_get_level_value(sOmmWarp->currLevel, sOmmWarp->currMode);
        sOmmWarp->currAct = omm_opt_get_first_act_value(sOmmWarp->currLevel, sOmmWarp->currMode);
    }

    // Act changed
    else if (sOmmWarp->prevAct != sOmmWarp->currAct) {
        sOmmWarp->currAct = omm_opt_get_act_value(sOmmWarp->currAct, sOmmWarp->currMode);
        sOmmWarp->currLevel = omm_opt_get_level_value_from_act_value(sOmmWarp->currAct, sOmmWarp->currMode);
    }

    // Update values
    sOmmWarp->prevLevel = sOmmWarp->currLevel;
    sOmmWarp->prevMode = sOmmWarp->currMode;
    sOmmWarp->prevAct = sOmmWarp->currAct;
}

static void omm_opt_warp_to_level(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        s32 levelNum = omm_opt_get_level_num(sOmmWarp->currLevel);
        s32 actNum = omm_opt_get_act_num(sOmmWarp->currAct);
        if (omm_is_main_menu() || !omm_warp_to_level(levelNum, sOmmWarp->currMode + 1, actNum)) {
            play_buzz_sound();
        }
    }
}

static void omm_opt_warp_to_castle(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        s32 levelNum = omm_opt_get_level_num(sOmmWarp->currLevel);
        if (omm_is_main_menu() || !omm_exit_level(levelNum, sOmmWarp->currMode + 1, true)) {
            play_buzz_sound();
        }
    }
}

void omm_opt_sm74_change_mode(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        if (!omm_is_main_menu()) {
            if (optmenu_open) optmenu_toggle();
            gCurrAreaIndex = sWarpDest.areaIdx = 1 + ((OMM_GAME_MODE + 1) % OMM_NUM_SAVE_MODES);
            initiate_warp(gCurrLevelNum, gCurrAreaIndex, 0x0A, 0);
            fade_into_special_warp(0, 0);
            gSavedCourseNum = COURSE_NONE;
            gDialogBoxState = 0;
            gMenuMode = -1;
        } else {
            play_buzz_sound();
        }
    }
}

#endif
