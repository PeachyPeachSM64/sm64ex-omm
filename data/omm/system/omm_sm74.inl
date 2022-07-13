#if OMM_GAME_IS_SM74
#if !defined(OMM_OPTIONS_MENU_INL)
// File included from omm_options.c

//
// Warp to level (init)
//

typedef struct { s32 level; s32 act; } ActValues;
static OmmArray sOmmWarpActValues[3] = { omm_array_zero, omm_array_zero, omm_array_zero };

static const u8 **sOmmWarpLevelChoices[3] = { NULL, NULL, NULL };
static const u8 **sOmmWarpActChoices[3] = { NULL, NULL, NULL };

struct Option *sOmmWarpLevelOpt = NULL;
struct Option *sOmmWarpActOpt = NULL;

static u32 sOmmWarpLevel = 0;
static u32 sOmmWarpArea = 0;
static u32 sOmmWarpAct = 0;

static void omm_opt_init_warp_to_level() {
    for (sm74_mode__omm_opt_init_warp_to_level = 1; sm74_mode__omm_opt_init_warp_to_level <= 2; ++sm74_mode__omm_opt_init_warp_to_level) {
        for (s32 i = 0; i != omm_level_get_count(); ++i) {
            s32 level = omm_level_get_list()[i];
            s32 stars = max_s(0x1, omm_stars_get_bits_total(level));
            for (s32 j = 0; j != 6; ++j) {
                if ((stars >> j) & 1) {
                    ActValues *actValues = OMM_MEMNEW(ActValues, 1);
                    actValues->level = level;
                    actValues->act = j + 1;
                    omm_array_add(sOmmWarpActValues[sm74_mode__omm_opt_init_warp_to_level], ptr, actValues);
                }
            }
        }
    }

    sOmmWarpLevelChoices[0] = OMM_MEMNEW(u8 *, omm_level_get_count() * 2);
    sOmmWarpLevelChoices[1] = sOmmWarpLevelChoices[0];
    sOmmWarpLevelChoices[2] = sOmmWarpLevelChoices[0] + omm_level_get_count();
    for (sm74_mode__omm_opt_init_warp_to_level = 1; sm74_mode__omm_opt_init_warp_to_level <= 2; ++sm74_mode__omm_opt_init_warp_to_level) {
        for (s32 i = 0; i != omm_level_get_count(); ++i) {
            const u8 *name = omm_level_get_name(omm_level_get_list()[i], true, true);
            sOmmWarpLevelChoices[sm74_mode__omm_opt_init_warp_to_level][i] = OMM_MEMDUP(name, omm_text_length(name) + 1);
        }
    }

    sOmmWarpActChoices[0] = OMM_MEMNEW(u8 *, omm_array_count(sOmmWarpActValues[1]) + omm_array_count(sOmmWarpActValues[2]));
    sOmmWarpActChoices[1] = sOmmWarpActChoices[0];
    sOmmWarpActChoices[2] = sOmmWarpActChoices[0] + omm_array_count(sOmmWarpActValues[1]);
    for (sm74_mode__omm_opt_init_warp_to_level = 1; sm74_mode__omm_opt_init_warp_to_level <= 2; ++sm74_mode__omm_opt_init_warp_to_level) {
        omm_array_for_each(sOmmWarpActValues[sm74_mode__omm_opt_init_warp_to_level], p) {
            ActValues *actValues = (ActValues *) p->as_ptr;
            const u8 *name = omm_level_get_act_name(actValues->level, actValues->act, true, true);
            sOmmWarpActChoices[sm74_mode__omm_opt_init_warp_to_level][i_p] = OMM_MEMDUP(name, omm_text_length(name) + 1);
        }
    }
}

static struct Option omm_opt_make_choice_level(const char *label, u32 *value) {
    struct Option opt = { 0 };
    opt.type = OPT_CHOICE;
    opt.label = omm_text_convert(label, true);
    opt.uval = value;
    opt.choices = (const u8 **) 1;
    opt.numChoices = 0;
    return opt;
}

static struct Option omm_opt_make_choice_area(UNUSED const char *label, u32 *value) {
    struct Option opt = { 0 };
    opt.type = OPT_CHOICE;
    opt.label = omm_text_convert(OMM_TEXT_SM74_OPT_WARP_EDITION, true);
    opt.uval = value;
    opt.choices = OMM_MEMNEW(u8 *, 2);
    opt.numChoices = 2;
    opt.choices[0] = omm_text_convert(OMM_TEXT_SM74_OPT_WARP_NORMAL, true);
    opt.choices[1] = omm_text_convert(OMM_TEXT_SM74_OPT_WARP_EXTREME, true);
    return opt;
}

static struct Option omm_opt_make_choice_act(const char *label, u32 *value) {
    struct Option opt = { 0 };
    opt.type = OPT_CHOICE;
    opt.label = omm_text_convert(label, true);
    opt.uval = value;
    opt.choices = (const u8 **) 2;
    opt.numChoices = 0;
    return opt;
}

//
// Warp to level (update)
//

static u32 omm_opt_get_level_index(s32 level) {
    for (s32 i = 0; i != omm_level_get_count(); ++i) {
        if (omm_level_get_list()[i] == level) {
            return i;
        }
    }
    return 0;
}

static u32 omm_opt_get_first_act_index(s32 level) {
    s32 mode = sOmmWarpArea + 1;
    omm_array_for_each(sOmmWarpActValues[mode], p) {
        ActValues *actValues = (ActValues *) p->as_ptr;
        if (actValues->level == level) {
            return i_p;
        }
    }
    return 0;
}

static void omm_opt_update_warp_to_level() {
    static u32 sOmmWarpLevelPrev = (u32) -1;
    static u32 sOmmWarpAreaPrev = (u32) -1;
    static u32 sOmmWarpActPrev = (u32) -1;
    if (OMM_UNLIKELY(!sOmmWarpLevelOpt || !sOmmWarpActOpt)) {
        return;
    }

    // Mode changed
    if (sOmmWarpAreaPrev != sOmmWarpArea) {
        s32 mode = sOmmWarpArea + 1;
        s32 level = omm_level_get_list()[sOmmWarpLevel];
        sOmmWarpLevelOpt->choices = sOmmWarpLevelChoices[mode];
        sOmmWarpLevelOpt->numChoices = omm_level_get_count();
        sOmmWarpActOpt->choices = sOmmWarpActChoices[mode];
        sOmmWarpActOpt->numChoices = omm_array_count(sOmmWarpActValues[mode]);
        sOmmWarpAct = omm_opt_get_first_act_index(level);
    }

    // Level changed
    else if (sOmmWarpLevelPrev != sOmmWarpLevel) {
        s32 level = omm_level_get_list()[sOmmWarpLevel];
        sOmmWarpAct = omm_opt_get_first_act_index(level);
    }

    // Act changed
    else if (sOmmWarpActPrev != sOmmWarpAct) {
        s32 mode = sOmmWarpArea + 1;
        s32 level = ((ActValues *) omm_array_get(sOmmWarpActValues[mode], ptr, sOmmWarpAct))->level;
        sOmmWarpLevel = omm_opt_get_level_index(level);
    }

    // Update values
    sOmmWarpLevelPrev = sOmmWarpLevel;
    sOmmWarpAreaPrev = sOmmWarpArea;
    sOmmWarpActPrev = sOmmWarpAct;
}

static void omm_opt_warp_to_level(UNUSED void *opt, s32 arg) {
    if (!arg) {
        s32 level = omm_level_get_list()[sOmmWarpLevel];
        s32 mode = sOmmWarpArea + 1;
        s32 act = ((ActValues *) omm_array_get(sOmmWarpActValues[mode], ptr, sOmmWarpAct))->act;
        if (!omm_warp_to_level(level, mode, act)) {
            play_sound(SOUND_MENU_CAMERA_BUZZ | 0xFF00, gGlobalSoundArgs);
        }
    }
}

void omm_opt_sm74_change_mode(UNUSED void *opt, s32 arg) {
    if (!arg) {
        if (optmenu_open) optmenu_toggle();
        initiate_warp(gCurrLevelNum, gCurrAreaIndex ^ 3, 0x0A, 0);
        fade_into_special_warp(0, 0);
        gSavedCourseNum = COURSE_NONE;
        gDialogBoxState = 0;
        gMenuMode = -1;
    }
}

#else
// File included from omm_options_menu.inl

extern struct Option *sOmmWarpLevelOpt;
extern struct Option *sOmmWarpActOpt;

static void omm_sm74_opt_locate_warp_to_level_options(struct SubMenu *subMenu) {
    if (subMenu) {
        for (s32 i = 0; i != subMenu->numOpts; ++i) {
            struct Option *opt = &subMenu->opts[i];
            if (opt->type == OPT_SUBMENU) {
                omm_sm74_opt_locate_warp_to_level_options(opt->nextMenu);
            } else if (opt->type == OPT_CHOICE) {
                switch ((uintptr_t) opt->choices) {
                    case 1: sOmmWarpLevelOpt = opt; break;
                    case 2: sOmmWarpActOpt = opt; break;
                }
            }
        }
    }
}

OMM_ROUTINE_UPDATE(omm_sm74_opt_update) {
    if (OMM_UNLIKELY(!sOmmWarpLevelOpt || !sOmmWarpActOpt)) {
        omm_sm74_opt_locate_warp_to_level_options(&menuMain);
    }
}

#endif
#endif
