#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

// + Odyssey Mario's Moveset
// + Warp To Level
// + Warp To Castle
// + Model Packs
// + Character Select
// + Time Trials
//   Game
//   Camera
//   Controls
//   Display
//   Sound
//   Cheats
// + Data Management
// + SM74 Swap Version
// + Return To Main Menu
// + Change Game
// + Exit Game

#define omm_opt_skip_option(opt_type, opt_var, opt_val) { \
    if (opt->type == opt_type && opt->opt_var == &opt_val) { \
        subMenu->numOpts--; \
        continue; \
    } \
}

#define omm_opt_remove_option(opt_type, opt_var, opt_val) { \
    if (opt->type == opt_type && opt->opt_var == &opt_val) { \
        mem_mov(opt, opt + 1, sizeof(struct Option) * (subMenu->numOpts - j - 1)); \
        subMenu->numOpts--; j--; \
        continue; \
    } \
}

#define omm_opt_rename_option(opt_type, opt_var, opt_val, opt_name) { \
    if (opt->type == opt_type && opt->opt_var == &opt_val) { \
        opt->label = omm_opt_text(opt_name); \
        continue; \
    } \
}

static void omm_opt_add_option(struct Option **head, s32 type, const u8 *label, void *value) {
    (*head)->type = type;
    (*head)->label = label;
    switch (type) {
        case OPT_SUBMENU: (*head)->nextMenu = (struct SubMenu *) value; break;
        case OPT_BUTTON: (*head)->actionFn = (void (*) (struct Option *, s32)) value; break;
    }
    (*head)++;
}

void omm_opt_init_main_menu() {
    s32 numOpts = menuMain.numOpts + 10 + (1 * OMM_GAME_IS_SM74);
    struct Option *opts = mem_new(struct Option, numOpts);
    struct Option *head = opts;

    // OMM sub-menu
    omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptMenu.label, gOmmOptMenu.subMenu);

    // Warp to level sub-menu
    omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptWarpToLevel.label, gOmmOptWarpToLevel.subMenu);

    // Warp to castle sub-menu
    omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptWarpToCastle.label, gOmmOptWarpToCastle.subMenu);

    // Model packs sub-menu
    if (gOmmOptModels.subMenu) {
        omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptModels.label, gOmmOptModels.subMenu);
    } else {
        numOpts--;
    }

    // Character Select sub-menu
    if (gOmmOptCs.subMenu) {
        omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptCs.label, gOmmOptCs.subMenu);
    } else {
        numOpts--;
    }

    // Time Trials sub-menu
    omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptTimeTrials.label, gOmmOptTimeTrials.subMenu);

    // Other sub-menus
    for (s32 i = 0; i != menuMain.numOpts; ++i) {

#if OMM_GAME_IS_R96X
        // Game sub-menu (Render96)
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuGame) {
            struct SubMenu *subMenu = menuMain.opts[i].nextMenu;
            struct Option *newOpts = mem_new(struct Option, subMenu->numOpts);
            for (s32 j = 0, k = 0, n = subMenu->numOpts; j != n; ++j) {
                struct Option *opt = subMenu->opts + j;

                // Skip Precache textures toggle
                omm_opt_skip_option(OPT_TOGGLE, bval, configPrecacheRes);

                // Skip Disable billboard toggle
                omm_opt_skip_option(OPT_TOGGLE, bval, configBillboard);

                // Copy option
                mem_cpy(newOpts + k++, opt, sizeof(struct Option));
            }
            subMenu->opts = newOpts;
        }
#endif

        // Display sub-menu
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuVideo) {
            s32 numNewOpts = 5 + OMM_GAME_IS_RF14;
            struct SubMenu *subMenu = menuMain.opts[i].nextMenu;
            struct Option *newOpts = mem_new(struct Option, subMenu->numOpts + numNewOpts);

            // Frame rate
            newOpts[0].type = OPT_CHOICE;
            newOpts[0].choices = mem_new(const u8 *, gOmmFrameRateCount);
            newOpts[0].numChoices = gOmmFrameRateCount;
            newOpts[0].uval = &gOmmFrameRate;
            newOpts[0].label = omm_opt_text(OMM_TEXT_OPT_FRAME_RATE);
            newOpts[0].choices[OMM_FPS_30] = omm_opt_text(OMM_TEXT_OPT_FRAME_RATE_30);
            newOpts[0].choices[OMM_FPS_60] = omm_opt_text(OMM_TEXT_OPT_FRAME_RATE_60);
            newOpts[0].choices[OMM_FPS_AUTO] = omm_opt_text(OMM_TEXT_OPT_FRAME_RATE_AUTO);
            newOpts[0].choices[OMM_FPS_INF] = omm_opt_text(OMM_TEXT_OPT_FRAME_RATE_UNLIMITED);

            // Show FPS
#if OMM_CODE_DEBUG
            newOpts[1].type = OPT_CHOICE;
            newOpts[1].choices = mem_new(const u8 *, gOmmShowFPSCount);
            newOpts[1].numChoices = gOmmShowFPSCount;
            newOpts[1].uval = &gOmmShowFPS;
            newOpts[1].label = omm_opt_text(OMM_TEXT_OPT_SHOW_FPS);
            newOpts[1].choices[0] = omm_opt_text(OMM_TEXT_OPT_SHOW_FPS_DISABLED);
            newOpts[1].choices[1] = omm_opt_text(OMM_TEXT_OPT_SHOW_FPS_ENABLED);
            newOpts[1].choices[2] = omm_opt_text(OMM_TEXT_OPT_SHOW_FPS_PROFILER);
            newOpts[1].choices[3] = omm_opt_text(OMM_TEXT_OPT_SHOW_FPS_GFX);
#else
            newOpts[1].type = OPT_TOGGLE;
            newOpts[1].bval = &gOmmShowFPS;
            newOpts[1].label = omm_opt_text(OMM_TEXT_OPT_SHOW_FPS);
#endif

            // Other options
            // Remove Apply button and some Render96 options
            for (s32 j = 0, k = 2, n = subMenu->numOpts; j != n; ++j) {
                struct Option *opt = subMenu->opts + j;

                // Texture caching
                if (opt->type == OPT_CHOICE && opt->uval == &configFiltering) {
                    newOpts[k].type = OPT_CHOICE;
                    newOpts[k].choices = mem_new(const u8 *, gOmmTextureCachingCount);
                    newOpts[k].numChoices = gOmmTextureCachingCount;
                    newOpts[k].uval = &gOmmTextureCaching;
                    newOpts[k].label = omm_opt_text(OMM_TEXT_OPT_TEXTURE_CACHING);
                    newOpts[k].choices[0] = omm_opt_text(OMM_TEXT_OPT_TEXTURE_CACHING_DISABLED);
                    newOpts[k].choices[1] = omm_opt_text(OMM_TEXT_OPT_TEXTURE_CACHING_AT_START_UP);
                    newOpts[k].choices[2] = omm_opt_text(OMM_TEXT_OPT_TEXTURE_CACHING_PERMANENT);
                    k++;
#if !OMM_GAME_IS_RF14
                }

                if (opt->type == OPT_TOGGLE && opt->bval == &configHUD) {
#endif
                    // Model pack caching
                    newOpts[k].type = OPT_TOGGLE;
                    newOpts[k].bval = &gOmmModelPackCaching;
                    newOpts[k].label = omm_opt_text(OMM_TEXT_OPT_MODEL_PACK_CACHING);
                    k++;

                    // HUD type
                    newOpts[k].type = OPT_CHOICE;
                    newOpts[k].choices = mem_new(const u8 *, gOmmHudModeCount);
                    newOpts[k].numChoices = gOmmHudModeCount;
                    newOpts[k].uval = &gOmmHudMode;
                    newOpts[k].label = omm_opt_text(OMM_TEXT_OPT_HUD_MODE);
                    newOpts[k].choices[0] = omm_opt_text(OMM_TEXT_OPT_HUD_MODE_ALWAYS);
                    newOpts[k].choices[1] = omm_opt_text(OMM_TEXT_OPT_HUD_MODE_VANISHING);
                    newOpts[k].choices[2] = omm_opt_text(OMM_TEXT_OPT_HUD_MODE_PRO);
                    newOpts[k].choices[3] = omm_opt_text(OMM_TEXT_OPT_HUD_MODE_NONE);
                    k++;

                    // HUD size
                    newOpts[k].type = OPT_SCROLL;
                    newOpts[k].scrMin = 8;
                    newOpts[k].scrMax = 20;
                    newOpts[k].scrStep = 1;
                    newOpts[k].uval = &gOmmHudSize;
                    newOpts[k].label = omm_opt_text(OMM_TEXT_OPT_HUD_SIZE);
                    k++;
#if !OMM_GAME_IS_RF14
                    continue;
#endif
                }

#if OMM_GFX_API_DX
                // Skip VSync toggle on DirectX (always enabled)
                omm_opt_skip_option(OPT_TOGGLE, bval, configWindow.vsync);
#endif

                // Skip Apply button
                omm_opt_skip_option(OPT_BUTTON, actionFn, optvideo_apply);

#if OMM_GAME_IS_R96X
                // Skip 60 FPS toggle (Render96)
                omm_opt_skip_option(OPT_TOGGLE, bval, config60FPS);

                // Skip internal resolution settings (Render96)
                omm_opt_skip_option(OPT_TOGGLE, bval, configInternalResolutionBool);
                omm_opt_skip_option(OPT_CHOICE, uval, configCustomInternalResolution);

                // Skip draw distance scroll (Render96)
                omm_opt_skip_option(OPT_SCROLL, uval, configDrawDistance);
#endif

                // Texture filtering: remove three-point
                if (opt->type == OPT_CHOICE && opt->uval == &configFiltering) {
                    opt->numChoices = 2;
                    configFiltering %= 2;
                }

                // Copy option
                mem_cpy(newOpts + k++, opt, sizeof(struct Option));
            }
            subMenu->opts = newOpts;
            subMenu->numOpts += numNewOpts;
        }

#if OMM_GAME_IS_RF14
        // Remove the Settings sub-menu
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuSettings) {
            numOpts--;
            continue;
        }
#endif

        // Camera sub-menu
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuCamera) {
            struct SubMenu *subMenu = menuMain.opts[i].nextMenu;
            struct Option *newOpts = mem_new(struct Option, subMenu->numOpts);
            mem_cpy(newOpts, subMenu->opts, sizeof(struct Option) * subMenu->numOpts);

            // Options
            for (s32 j = 0; j != subMenu->numOpts; ++j) {
                struct Option *opt = newOpts + j;

                // Mouse control
                omm_opt_rename_option(OPT_TOGGLE, bval, configCameraMouse, OMM_TEXT_OPT_CAMERA_MOUSE_CONTROL);

                // X sensitivity
                omm_opt_rename_option(OPT_SCROLL, uval, configCameraXSens, OMM_TEXT_OPT_CAMERA_X_SENSITIVITY);

                // Y sensitivity
                omm_opt_rename_option(OPT_SCROLL, uval, configCameraYSens, OMM_TEXT_OPT_CAMERA_Y_SENSITIVITY);

                // Centre aggression
                omm_opt_rename_option(OPT_SCROLL, uval, configCameraAggr, OMM_TEXT_OPT_CAMERA_CENTRE_AGGRESSION);

                // Remove Analog camera
                omm_opt_remove_option(OPT_TOGGLE, bval, configCameraAnalog);

#if BETTER_CAM_IS_PUPPY_CAM
                // Remove Camera opacity
                omm_opt_remove_option(OPT_CHOICE, uval, configCameraOpacity);

                // Remove Debug camera
                omm_opt_remove_option(OPT_TOGGLE, bval, configDebugCamera);
#else
                // Remove Angle deceleration
                omm_opt_remove_option(OPT_SCROLL, uval, configCameraDegrade);

                // Remove Pan level
                omm_opt_remove_option(OPT_SCROLL, uval, configCameraPan);
#endif

                // Replace "Invert X Axis" by "Invert 1st Person Camera"
                if (opt->type == OPT_TOGGLE && opt->bval == &configCameraInvertX) {
                    opt->type = OPT_CHOICE;
                    opt->choices = mem_new(const u8 *, gOmmCameraInvert1stPersonCount);
                    opt->numChoices = gOmmCameraInvert1stPersonCount;
                    opt->uval = &gOmmCameraInvert1stPerson;
                    opt->label = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_1ST_PERSON);
                    opt->choices[0] = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_NONE);
                    opt->choices[1] = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_X);
                    opt->choices[2] = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_Y);
                    opt->choices[3] = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_BOTH);
                    continue;
                }

                // Replace "Invert Y Axis" by "Invert 3rd Person Camera"
                if (opt->type == OPT_TOGGLE && opt->bval == &configCameraInvertY) {
                    opt->type = OPT_CHOICE;
                    opt->choices = mem_new(const u8 *, gOmmCameraInvert3rdPersonCount);
                    opt->numChoices = gOmmCameraInvert3rdPersonCount;
                    opt->uval = &gOmmCameraInvert3rdPerson;
                    opt->label = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_3RD_PERSON);
                    opt->choices[0] = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_NONE);
                    opt->choices[1] = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_X);
                    opt->choices[2] = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_Y);
                    opt->choices[3] = omm_opt_text(OMM_TEXT_OPT_CAMERA_INVERT_BOTH);
                    continue;
                }
            }

            // Snapshot mode
            struct Option *newOpt = newOpts + subMenu->numOpts;
            newOpt->type = OPT_BUTTON;
            newOpt->label = omm_opt_text(OMM_TEXT_OPT_CAMERA_SNAPSHOT_MODE);
            newOpt->actionFn = omm_opt_camera_enter_snapshot_mode;

            // Update sub-menu
            subMenu->opts = newOpts;
            subMenu->numOpts += 1;
        }

        // Custom Controls sub-menu
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuControls) {
            omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptControls.label, gOmmOptControls.subMenu);
            continue;
        }

#if !OMM_GAME_IS_R96X
        // Custom Cheats sub-menu
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuCheats) {
            omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptCheats.label, gOmmOptCheats.subMenu);
            continue;
        }
#endif

        // Move the exit button to the end
        if (menuMain.opts[i].type == OPT_BUTTON && menuMain.opts[i].actionFn == optmenu_act_exit) {
            numOpts--;
            continue;
        }

        // Add options
        mem_cpy(head++, &menuMain.opts[i], sizeof(struct Option));
    }

    // Data management sub-menu
    omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptDataManagement.label, gOmmOptDataManagement.subMenu);

#if OMM_GAME_IS_SM74
    // Change SM74 mode button
    omm_opt_add_option(&head, OPT_BUTTON, omm_opt_text(OMM_TEXT_SM74_OPT_SWAP_VERSION), omm_opt_sm74_change_mode);
#endif

    // Return to main menu button
    omm_opt_add_option(&head, OPT_BUTTON, omm_opt_text(OMM_TEXT_OPT_RETURN_TO_MAIN_MENU), omm_opt_return_to_main_menu);

    // Change game sub-menu
    if (gOmmOptChangeGame.subMenu) {
        omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptChangeGame.label, gOmmOptChangeGame.subMenu);
    } else {
        numOpts--;
    }

    // Exit game sub-menu
    omm_opt_add_option(&head, OPT_SUBMENU, gOmmOptExitGame.label, gOmmOptExitGame.subMenu);

    // Update main menu
    menuMain.numOpts = numOpts;
    menuMain.opts = opts;
}

//
// State
//

static bool omm_opt_is_available(struct Option *opt) {
    return (
        opt->type == OPT_SUBMENU && opt->nextMenu == gOmmOptDataManagement.subMenu ?
        omm_is_main_menu() :
        OPT_STATE(opt->type) <= omm_opt_get_state()
    );
}

static struct SubMenu *omm_opt_get_current_menu(struct SubMenu *subMenu) {
    static struct SubMenu sSubMenu[1] = {0};
    mem_del(sSubMenu->opts);
    sSubMenu->prev = subMenu->prev;
    sSubMenu->label = subMenu->label;
    sSubMenu->opts = mem_new(struct Option, subMenu->numOpts);
    sSubMenu->numOpts = 0;
    sSubMenu->select = 0;
    for (s32 i = 0; i != subMenu->numOpts; ++i) {
        struct Option *opt = &subMenu->opts[i];
        if (omm_opt_is_available(opt)) {
            sSubMenu->opts[sSubMenu->numOpts++] = *opt;
        }
        if (i == subMenu->select) {
            sSubMenu->select = sSubMenu->numOpts - 1;
        }
    }
    return sSubMenu;
}

static void omm_opt_update_select(struct SubMenu *subMenu, s32 inc) {
    s32 dir = sign_s(inc);
    subMenu->select = (subMenu->select + subMenu->numOpts + inc) % subMenu->numOpts;
    while (!omm_opt_is_available(&subMenu->opts[subMenu->select])) {
        subMenu->select = (subMenu->select + subMenu->numOpts + dir) % subMenu->numOpts;
    }
}

//
// Draw
//

#include "gfx_dimensions.h"

static s32 sScrollBarOffset = 0;

#define OMM_OPT_Y                       ((SCREEN_HEIGHT * 2) / 3)
#define OMM_OPT_TITLE_Y                 (OMM_OPT_Y + 32)
#define OMM_OPT_OFFSET_Y                (16)
#define OMM_OPT_COUNT                   (9)
#define OMM_OPT_COLOR_WHITE             (0xFFFFFFFF)
#define OMM_OPT_COLOR_GRAY              (0x808080FF)
#define OMM_OPT_COLOR_BACKGROUND        (0x00000080)
#define OMM_OPT_COLOR_SELECT            (0x40D0FFFF)
#define OMM_OPT_COLOR_SELECT_BOX        (0x00FFFF20)
#define OMM_OPT_COLOR_ENABLED           (0x20E020FF)
#define OMM_OPT_COLOR_DISABLED          (0xFF2020FF)
#define OMM_OPT_COLOR_LOADING           (0xA0A0A0FF)
#define OMM_OPT_OFFSET_FROM_LEFT_EDGE   (20.f * sqr_f(GFX_DIMENSIONS_ASPECT_RATIO))
#define OMM_OPT_OFFSET_FROM_RIGHT_EDGE  (20.f * sqr_f(GFX_DIMENSIONS_ASPECT_RATIO) + sScrollBarOffset)
#define OMM_OPT_SELECT_BOX_MARGIN       (4)
#define OMM_OPT_BACKGROUND_BOX_MARGIN   (OMM_OPT_SELECT_BOX_MARGIN + 8)
#define OMM_OPT_SCROLL_BAR_SIZE         ((s32) (40.f * GFX_DIMENSIONS_ASPECT_RATIO))
#if OMM_GAME_IS_R96X
#define OMM_OPT_TEXT_CONVERT(convert)   true
#else
#define OMM_OPT_TEXT_CONVERT(convert)   convert
#endif

static const char *omm_opt_int_to_string(str_t dst, const char *fmt, s32 x) {
    str_fmt(dst, sizeof(str_t), fmt, x);
    return dst;
}

static void omm_opt_print_string(const void *p, bool convert, s32 x, s32 y, u32 color, bool alignLeft) {
    const u8 *str64 = (convert ? (const u8 *) omm_text_convert((const char *) p, false) : (const u8 *) p);
    if (color & 0xFF) {
        x = (alignLeft ? GFX_DIMENSIONS_FROM_LEFT_EDGE(x) : GFX_DIMENSIONS_FROM_RIGHT_EDGE(x - 0.99f + omm_render_get_string_width(str64)));
        omm_render_string(x, y, ((color >> 24) & 0xFF), ((color >> 16) & 0xFF), ((color >> 8) & 0xFF), ((color >> 0) & 0xFF), str64, true);
    }
}

static void omm_opt_print_box(s32 x, s32 y, s32 w, s32 h, u32 color, bool alignLeft) {
    if (color & 0xFF) {
        omm_render_texrect((alignLeft ? GFX_DIMENSIONS_FROM_LEFT_EDGE(x) : GFX_DIMENSIONS_FROM_RIGHT_EDGE(x + w)), y, w, h, 32, 32, ((color >> 24) & 0xFF), ((color >> 16) & 0xFF), ((color >> 8) & 0xFF), ((color >> 0) & 0xFF), OMM_TEXTURE_MISC_WHITE, false);
    }
}

static const u8 *omm_opt_get_choice(ustr_t dst, struct Option *opt) {
#if OMM_GAME_IS_R96X

    // Language
    if (!opt->choices || !opt->numChoices) {
        return omm_text_copy(dst, sizeof(ustr_t), omm_text_convert(languages[*opt->uval]->name, false));
    }

    // Choice
    const char *choice = (const char *) opt->choices[*opt->uval];

    // Level
    if (strstr(choice, OMM_TEXT_OPT_WARP_TO_LEVEL_COURSE_NAME) == choice) {
        s32 levelNum; sscanf(choice + sizeof("OMM_TEXT_OPT_WARP_TO_LEVEL_COURSE_NAME"), "%02d", &levelNum);
        return omm_level_get_course_name(dst, levelNum, OMM_GAME_MODE, true, true);
    }

    // Area
    if (strstr(choice, OMM_TEXT_OPT_WARP_TO_LEVEL_AREA_NAME) == choice) {
        s32 areaIndex; sscanf(choice + sizeof("OMM_TEXT_OPT_WARP_TO_LEVEL_AREA_NAME"), "%01d", &areaIndex);
        u8 *textArea = omm_text_convert(OMM_TEXT_OPT_WARP_TO_LEVEL_AREA, false);
        s32 textAreaLength = min_s(61, omm_text_length(textArea));
        omm_text_copy(dst, sizeof(ustr_t), textArea);
        dst[textAreaLength + 0] = DIALOG_CHAR_SPACE;
        dst[textAreaLength + 1] = areaIndex;
        dst[textAreaLength + 2] = DIALOG_CHAR_TERMINATOR;
        return dst;
    }

    // Act
    if (strstr(choice, OMM_TEXT_OPT_WARP_TO_LEVEL_ACT_NAME) == choice) {
        s32 levelNum; sscanf(choice + sizeof("OMM_TEXT_OPT_WARP_TO_LEVEL_ACT_NAME"), "%02d", &levelNum);
        s32 actNum; sscanf(choice + sizeof("OMM_TEXT_OPT_WARP_TO_LEVEL_ACT_NAME") + 3, "%02d", &actNum);
        return omm_level_get_act_name(dst, levelNum, actNum, OMM_GAME_MODE, true, true);
    }

    // Default
    return omm_text_convert(choice, false);
#else
    return omm_text_copy(dst, sizeof(ustr_t), opt->choices[*opt->uval]);
#endif
}

static void omm_opt_draw_option(struct Option *opt, bool selected, s32 y) {
    if (!opt) {
        return;
    }

    // Selected box
    if (selected) {
        u8 a = (u8) ((coss(gGlobalTimer * 0x800) + 1.f) * 0x20);
        omm_opt_print_box(
            OMM_OPT_OFFSET_FROM_LEFT_EDGE - OMM_OPT_SELECT_BOX_MARGIN,
            y - OMM_OPT_SELECT_BOX_MARGIN,
            0.99f + GFX_DIMENSIONS_FROM_RIGHT_EDGE(OMM_OPT_OFFSET_FROM_RIGHT_EDGE) - GFX_DIMENSIONS_FROM_LEFT_EDGE(OMM_OPT_OFFSET_FROM_LEFT_EDGE) + 2 * OMM_OPT_SELECT_BOX_MARGIN,
            8 + 2 * OMM_OPT_SELECT_BOX_MARGIN,
            OMM_OPT_COLOR_SELECT_BOX + a, 1
        );
    }

    // Label
    if ((void *) opt->label == (void *) OMM_TEXT_OPT_MODELS_LABEL) {
        str_t numPacks; str_fmt(numPacks, sizeof(numPacks), " (%d)", omm_models_get_model_pack_count());
        const u8 *label = omm_text_convert(OMM_TEXT_OPT_MODELS_LABEL, false);
        const u8 *packs = omm_text_convert(numPacks, false);
        s32 labelLength = omm_text_length(label);
        s32 packsLength = omm_text_length(packs);
        ustr_t labelWithNumPacks;
        mem_set(labelWithNumPacks, 0xFF, sizeof(labelWithNumPacks));
        mem_cpy(labelWithNumPacks, label, labelLength);
        mem_cpy(labelWithNumPacks + labelLength, packs, packsLength);
        omm_opt_print_string(labelWithNumPacks, false, OMM_OPT_OFFSET_FROM_LEFT_EDGE, y, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 1);
    } else {
        omm_opt_print_string(opt->label, OMM_OPT_TEXT_CONVERT(false), OMM_OPT_OFFSET_FROM_LEFT_EDGE, y, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 1);
    }

    // Values
    switch (OPT_TYPE(opt->type)) {
        case OPT_TOGGLE: {
            if (currentMenu == gOmmOptModels.subMenu) {
                s32 *caching = omm_models_get_caching_state(opt->bval);
                if (caching && *caching == OMM_MODELS_CACHING_ENABLED) {
                    omm_opt_print_string(OMM_TEXT_OPT_LOADING, OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, OMM_OPT_COLOR_LOADING, 0);
                    break;
                }
            }
            if (*opt->bval) {
                omm_opt_print_string(OMM_TEXT_OPT_ENABLED, OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, OMM_OPT_COLOR_ENABLED, 0);
            } else {
                omm_opt_print_string(OMM_TEXT_OPT_DISABLED, OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, OMM_OPT_COLOR_DISABLED, 0);
            }
        } break;

        case OPT_CHOICE: {
            ustr_t choice;
            omm_opt_print_string(omm_opt_get_choice(choice, opt), false, OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 0);
        } break;

        case OPT_SCROLL: {
            str_t scrollStr;
            s32 w = (s32) (OMM_OPT_SCROLL_BAR_SIZE * (f32) (*opt->uval - opt->scrMin) / (f32) (opt->scrMax - opt->scrMin));
            omm_opt_print_string(omm_opt_int_to_string(scrollStr, "%d", *opt->uval), OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 0);
            omm_opt_print_box(OMM_OPT_OFFSET_FROM_RIGHT_EDGE + 28, y + 1, OMM_OPT_SCROLL_BAR_SIZE + 2, 6, OMM_OPT_COLOR_GRAY, 0);
            omm_opt_print_box(OMM_OPT_OFFSET_FROM_RIGHT_EDGE + 29 + OMM_OPT_SCROLL_BAR_SIZE - w, y + 2, w, 4, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 0);
        } break;

        case OPT_BIND: {
            for (s32 i = 0; i != 3; ++i) {
                u32 bind = opt->uval[i];
                if (selected && i == optmenu_bind_idx) {
                    if (optmenu_binding) {
                        omm_opt_print_string(OMM_TEXT_OPT_DOT_DOT_DOT, OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_SELECT, 0);
                    } else if (bind == VK_INVALID) {
                        omm_opt_print_string(OMM_TEXT_OPT_NONE, OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_SELECT, 0);
                    } else {
                        str_t bindStr;
                        omm_opt_print_string(omm_opt_int_to_string(bindStr, "%04X", bind), OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_SELECT, 0);
                    }
                } else {
                    if (bind == VK_INVALID) {
                        omm_opt_print_string(OMM_TEXT_OPT_NONE, OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_GRAY, 0);
                    } else {
                        str_t bindStr;
                        omm_opt_print_string(omm_opt_int_to_string(bindStr, "%04X", bind), OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_WHITE, 0);
                    }
                }
            }
        } break;

        case OPT_SUBMENU: {
            if (selected) {
                omm_opt_print_string(OMM_TEXT_OPT_PRESS_A, OMM_OPT_TEXT_CONVERT(true), OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, OMM_OPT_COLOR_SELECT, 0);
            }
        } break;

        default: {
        } break;
    }
}

static void omm_opt_draw_menu(struct SubMenu *subMenu) {
    if (subMenu) {
        sScrollBarOffset = 0;

        // Colorful title
        const u8 *title = OMM_OPT_TEXT_CONVERT(false) ? omm_text_convert((const char *) subMenu->label, false) : subMenu->label;
        omm_render_string_hud((SCREEN_WIDTH - omm_render_get_string_width_hud(title)) / 2 + 1, OMM_OPT_TITLE_Y - 1, 0x00, 0x00, 0x00, 0x80, title, false);
        omm_render_string_hud((SCREEN_WIDTH - omm_render_get_string_width_hud(title)) / 2, OMM_OPT_TITLE_Y, 0xFF, 0xFF, 0xFF, 0xFF, title, false);

        // Background box
        omm_opt_print_box(
            OMM_OPT_OFFSET_FROM_LEFT_EDGE - OMM_OPT_BACKGROUND_BOX_MARGIN,
            OMM_OPT_Y - (OMM_OPT_BACKGROUND_BOX_MARGIN + OMM_OPT_OFFSET_Y * (OMM_OPT_COUNT - 1)),
            GFX_DIMENSIONS_FROM_RIGHT_EDGE(OMM_OPT_OFFSET_FROM_RIGHT_EDGE) - GFX_DIMENSIONS_FROM_LEFT_EDGE(OMM_OPT_OFFSET_FROM_LEFT_EDGE) + 2 * OMM_OPT_BACKGROUND_BOX_MARGIN,
            2 * OMM_OPT_BACKGROUND_BOX_MARGIN + 8 + OMM_OPT_OFFSET_Y * (OMM_OPT_COUNT - 1),
            OMM_OPT_COLOR_BACKGROUND, 1
        );
        sScrollBarOffset = (subMenu->numOpts > OMM_OPT_COUNT ? 10 : 0);

        // Display options
        s32 maxIndex = max_s(0, subMenu->numOpts - OMM_OPT_COUNT);
        s32 topIndex = clamp_s(subMenu->select - OMM_OPT_COUNT / 2, 0, maxIndex);
        for (s32 i = 0, j = topIndex; i != min_s(subMenu->numOpts, OMM_OPT_COUNT); ++i, ++j) {
            omm_opt_draw_option(subMenu->opts + j, j == subMenu->select, OMM_OPT_Y - (OMM_OPT_OFFSET_Y * i));
        }

        // Scroll bar
        if (sScrollBarOffset) {
            sScrollBarOffset = 0;
            s32 h0 = (OMM_OPT_COUNT * OMM_OPT_OFFSET_Y) - 2;
            s32 y0 = OMM_OPT_Y + ((3 * OMM_OPT_OFFSET_Y) / 4) - (h0 + 2);
            s32 h = (s32) (h0 / sqrtf(maxIndex + 1));
            s32 y = y0 + (s32) ((h0 - h) * (1.f - (f32) topIndex / (f32) maxIndex));
            omm_opt_print_box(OMM_OPT_OFFSET_FROM_RIGHT_EDGE - 4, y0, 6, h0 + 2, OMM_OPT_COLOR_GRAY, 0);
            omm_opt_print_box(OMM_OPT_OFFSET_FROM_RIGHT_EDGE - 3, y + 1, 4, h, OMM_OPT_COLOR_WHITE, 0);
        }
    }
}

//
// Toggle
//

static void omm_opt_toggle() {
    if (!optmenu_open) {
        play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundArgs);
        currentMenu = &menuMain;
        optmenu_open = 1;
        omm_opt_update_select(currentMenu, 0);
    } else {
        play_sound(SOUND_MENU_MARIO_CASTLE_WARP2, gGlobalSoundArgs);
        optmenu_open = 0;
        omm_camera_update_settings();
        controller_reconfigure();
        configfile_save(configfile_name());
    }
}

//
// Inputs
//

#define OMM_OPT_INPUT_SOUND_NONE    (0x00)
#define OMM_OPT_INPUT_SOUND_CHANGED (0x01)
#define OMM_OPT_INPUT_SOUND_ERROR   (0x02)
#define OMM_OPT_INPUT_SOUND         (0x0F)

#define OMM_OPT_INPUT_HOLD_CONTINUE (0x00)
#define OMM_OPT_INPUT_HOLD_STOP     (0x10)
#define OMM_OPT_INPUT_HOLD_RESET    (0x20)
#define OMM_OPT_INPUT_HOLD          (0xF0)

void omm_opt_return_to_previous_menu(UNUSED struct Option *opt, s32 arg) {
    if (!arg) {
        if (currentMenu->prev) {
            currentMenu = currentMenu->prev;
            omm_opt_update_select(currentMenu, 0);
        } else {
            optmenu_toggle();
        }
    }
}

static s32 omm_opt_update_opt_toggle(struct Option *opt, s32 val) {
    bool before = *opt->bval;
    if (val < 0) *opt->bval = false;
    if (val > 0) *opt->bval = true;
    if (val == 0) *opt->bval = !*opt->bval;
    return *opt->bval != before;
}

static s32 omm_opt_update_opt_choice(struct Option *opt, s32 val) {
    u32 before = *opt->uval;
#if OMM_GAME_IS_R96X
    if (!opt->choices || !opt->numChoices) {
        *opt->uval = (*opt->uval + get_num_languages() + sign_s(val)) % get_num_languages();
        if (*opt->uval != before) {
            set_language(languages[*opt->uval]);
            return OMM_OPT_INPUT_SOUND_CHANGED;
        }
        return OMM_OPT_INPUT_SOUND_NONE;
    }
#endif
    *opt->uval = (*opt->uval + opt->numChoices + sign_s(val)) % opt->numChoices;
    return *opt->uval != before;
}

static s32 omm_opt_update_opt_scroll(struct Option *opt, s32 val) {
    if (val < 0) {
        if (*opt->uval == opt->scrMin) {
            *opt->uval = opt->scrMax;
            return OMM_OPT_INPUT_SOUND_CHANGED;
        }
        *opt->uval = (u32) max_s((s32) *opt->uval + val * max_s(1, opt->scrStep), opt->scrMin);
        if (*opt->uval == opt->scrMin) {
            return OMM_OPT_INPUT_SOUND_CHANGED | OMM_OPT_INPUT_HOLD_STOP;
        }
        return OMM_OPT_INPUT_SOUND_CHANGED;
    }
    if (val > 0) {
        if (*opt->uval == opt->scrMax) {
            *opt->uval = opt->scrMin;
            return OMM_OPT_INPUT_SOUND_CHANGED;
        }
        *opt->uval = (u32) min_s((s32) *opt->uval + val * max_s(1, opt->scrStep), opt->scrMax);
        if (*opt->uval == opt->scrMax) {
            return OMM_OPT_INPUT_SOUND_CHANGED | OMM_OPT_INPUT_HOLD_STOP;
        }
        return OMM_OPT_INPUT_SOUND_CHANGED;
    }
    return OMM_OPT_INPUT_SOUND_NONE;
}

static s32 omm_opt_update_opt_submenu(struct Option *opt, s32 val) {
    if (val == 0) {
        if (opt->nextMenu) {
            opt->nextMenu->prev = currentMenu;
            currentMenu = opt->nextMenu;
            omm_opt_update_select(currentMenu, 0);
            return OMM_OPT_INPUT_SOUND_CHANGED | OMM_OPT_INPUT_HOLD_RESET;
        }
        return OMM_OPT_INPUT_SOUND_ERROR | OMM_OPT_INPUT_HOLD_RESET;
    }
    return OMM_OPT_INPUT_SOUND_NONE;
}

static s32 omm_opt_update_opt_button(struct Option *opt, s32 val) {
    if (val == 0) {
        if (opt->actionFn) {
            opt->actionFn(opt, 0);
            if (opt->uval == gOmmOptYesFunc) {
                currentMenu->select = 0;
                play_sound(SOUND_MENU_MARIO_CASTLE_WARP2 | 0xFF00, gGlobalSoundArgs);
                omm_opt_return_to_previous_menu(opt, 0);
                return OMM_OPT_INPUT_SOUND_NONE;
            }
            return OMM_OPT_INPUT_SOUND_CHANGED | OMM_OPT_INPUT_HOLD_RESET;
        }
        return OMM_OPT_INPUT_SOUND_ERROR | OMM_OPT_INPUT_HOLD_RESET;
    }
    return OMM_OPT_INPUT_SOUND_NONE;
}

static s32 omm_opt_update_opt_bind(struct Option *opt, s32 val) {

    // Unbind button (Z press)
    if (val == 0xFF) {
        opt->uval[optmenu_bind_idx] = VK_INVALID;
        return OMM_OPT_INPUT_SOUND_NONE;
    }

    // Start binding (A press)
    if (val == 0) {
        opt->uval[optmenu_bind_idx] = VK_INVALID;
        optmenu_binding = TRUE;
        controller_get_raw_key(); // clear the last key, which is probably A
        return OMM_OPT_INPUT_SOUND_CHANGED | OMM_OPT_INPUT_HOLD_STOP;
    }

    // Previous/next bind slot
    optmenu_bind_idx = (optmenu_bind_idx + MAX_BINDS + val) % MAX_BINDS;
    return OMM_OPT_INPUT_SOUND_CHANGED;
}

static s32 omm_opt_update_opt(struct Option *opt, s32 val, bool aDown) {
    switch (OPT_TYPE(opt->type)) {
        case OPT_TOGGLE:  return omm_opt_update_opt_toggle(opt, val);
        case OPT_CHOICE:  return omm_opt_update_opt_choice(opt, val);
        case OPT_SCROLL:  return omm_opt_update_opt_scroll(opt, val * (aDown ? 5 : 1));
        case OPT_SUBMENU: return omm_opt_update_opt_submenu(opt, val);
        case OPT_BUTTON:  return omm_opt_update_opt_button(opt, val);
        case OPT_BIND:    return omm_opt_update_opt_bind(opt, val);
        default:          return OMM_OPT_INPUT_SOUND_NONE;
    }
}

static s32 omm_opt_process_inputs(u16 abzs, u16 udlr) {
    omm_opt_update_select(currentMenu, 0);

    // Start button: close the menu
    if (abzs & START_BUTTON) {
        optmenu_toggle();
        return OMM_OPT_INPUT_SOUND_NONE;
    }

    // A press: toggle, next choice, open submenu, press button, bind key
    if (abzs & A_BUTTON) {
        s32 res = omm_opt_update_opt(&currentMenu->opts[currentMenu->select], 0, false);
        if (res) return res;
    }

    // B press: return to previous menu
    if (abzs & B_BUTTON) {
        if (currentMenu->prev) {
            currentMenu = currentMenu->prev;
            omm_opt_update_select(currentMenu, 0);
            return OMM_OPT_INPUT_SOUND_CHANGED | OMM_OPT_INPUT_HOLD_RESET;
        }
        optmenu_toggle();
        return OMM_OPT_INPUT_SOUND_NONE;
    }

    // Z press: unbind key
    if (abzs & Z_TRIG) {
        if (currentMenu->opts[currentMenu->select].type == OPT_BIND) {
            return omm_opt_update_opt(&currentMenu->opts[currentMenu->select], 0xFF, false);
        }
    }

    // Left/Right
    if ((udlr & (L_JPAD | R_JPAD)) != (L_JPAD | R_JPAD)) {

        // Left: untoggle, previous choice, decrease scroll
        if (udlr & L_JPAD) {
            s32 res = omm_opt_update_opt(&currentMenu->opts[currentMenu->select], -1, gPlayer1Controller->buttonDown & A_BUTTON);
            if (res) return res;
        }

        // Right: toggle, next choice, increase scroll
        if (udlr & R_JPAD) {
            s32 res = omm_opt_update_opt(&currentMenu->opts[currentMenu->select], +1, gPlayer1Controller->buttonDown & A_BUTTON);
            if (res) return res;
        }
    }

    // Up/Down
    if ((udlr & (U_JPAD | D_JPAD)) != (U_JPAD | D_JPAD)) {

        // Up: previous option
        if (udlr & U_JPAD) {
            omm_opt_update_select(currentMenu, -1);
            return OMM_OPT_INPUT_SOUND_CHANGED;
        }

        // Down: next option
        if (udlr & D_JPAD) {
            omm_opt_update_select(currentMenu, +1);
            return OMM_OPT_INPUT_SOUND_CHANGED;
        }
    }

    // No input
    return OMM_OPT_INPUT_SOUND_NONE;
}

static void omm_opt_check_buttons() {
    static u16 optmenu_hold_button = 0;

    // Open/close options menu
    if ((gPlayer1Controller->buttonPressed & R_TRIG) && !optmenu_binding) {
        optmenu_toggle();
        return;
    }

    // Process inputs
    if (optmenu_open) {

        // Binding buttons
        if (optmenu_binding) {
            u32 key = controller_get_raw_key();
            if (key != VK_INVALID) {
                play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundArgs);
                currentMenu->opts[currentMenu->select].uval[optmenu_bind_idx] = key;
                optmenu_binding = FALSE;
                optmenu_hold_count = 0;
                optmenu_hold_button = 0;
                optmenu_option_timer = 0;
            }
            return;
        }

        // Inputs
        u16 abzs = gPlayer1Controller->buttonPressed & (A_BUTTON | B_BUTTON | Z_TRIG | START_BUTTON);
        u16 udlr = (
            (gPlayer1Controller->buttonDown & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) |
            ((gPlayer1Controller->stickY > +56) * U_JPAD) |
            ((gPlayer1Controller->stickY < -56) * D_JPAD) |
            ((gPlayer1Controller->stickX < -56) * L_JPAD) |
            ((gPlayer1Controller->stickX > +56) * R_JPAD)
        );
        if (!udlr || udlr != optmenu_hold_button) {
            optmenu_hold_count = 0;
            optmenu_hold_button = udlr;
            optmenu_option_timer = 0;
            if (!abzs) return;
        }

        // Update hold timer
        if (--optmenu_option_timer <= 0) {
            if (optmenu_hold_count == 0) {
                optmenu_hold_count++;
                optmenu_option_timer = 10;
            } else {
                optmenu_option_timer = 3;
            }
        } else {
            udlr = 0;
        }

        // Update options
        s32 res = omm_opt_process_inputs(abzs, udlr);
        switch (res & OMM_OPT_INPUT_SOUND) {
            case OMM_OPT_INPUT_SOUND_NONE: break;
            case OMM_OPT_INPUT_SOUND_CHANGED: play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundArgs); break;
            case OMM_OPT_INPUT_SOUND_ERROR: play_sound(SOUND_MENU_CAMERA_BUZZ, gGlobalSoundArgs); break;
        }
        switch (res & OMM_OPT_INPUT_HOLD) {
            case OMM_OPT_INPUT_HOLD_CONTINUE: break;
            case OMM_OPT_INPUT_HOLD_STOP: optmenu_option_timer = 0xFFFF; break;
            case OMM_OPT_INPUT_HOLD_RESET: optmenu_hold_count = 1; optmenu_option_timer = 10; break;
        }
    }

    // Make sure some variables are reset properly
    else {
        optmenu_binding = FALSE;
        optmenu_option_timer = 0;
        optmenu_hold_count = 0;
        currentMenu = NULL;
    }
}

//
// Options menu overrides
//

void optmenu_draw(void) {
    omm_opt_draw_menu(omm_opt_get_current_menu(currentMenu));
}

void optmenu_draw_prompt(void) {
}

void optmenu_toggle(void) {
    omm_opt_toggle();
}

void optmenu_check_buttons(void) {
    omm_opt_check_buttons();
    if (!optmenu_binding) {
        omm_opt_update_binds();
    }
}

#define optmenu_draw optmenu_draw_unused
#define optmenu_draw_prompt optmenu_draw_prompt_unused
#define optmenu_toggle optmenu_toggle_unused
#define optmenu_check_buttons optmenu_check_buttons_unused
