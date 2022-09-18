// File included from options_menu.c

#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

static s32 sNumOpts = 0;

void omm_opt_init_main_menu() {
    sNumOpts = menuMain.numOpts + 1 + (2 * !OMM_CODE_DYNOS) + (1 * OMM_GAME_IS_SM74);
    struct Option *opts = (struct Option *) omm_new(struct Option, sNumOpts);
    struct Option *head = opts;

    // OMM sub-menu
    head->type = OPT_SUBMENU;
    head->label = gOmmOptMenu.label;
    head->nextMenu = (struct SubMenu *) gOmmOptMenu.subMenu;
    head++;

#if !OMM_CODE_DYNOS
    // Warp to level sub-menu
    head->type = OPT_SUBMENU;
    head->label = gOmmOptWarpToLevel.label;
    head->nextMenu = (struct SubMenu *) gOmmOptWarpToLevel.subMenu;
    head++;
#endif

    // Other options
    for (s32 i = 0; i != menuMain.numOpts; ++i) {

#if OMM_GAME_IS_R96X
        // Game sub-menu (Render96)
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuGame) {
            struct SubMenu *subMenu = menuMain.opts[i].nextMenu;
            struct Option *newOpts = omm_new(struct Option, subMenu->numOpts);
            for (s32 j = 0, k = 0, n = subMenu->numOpts; j != n; ++j) {
                struct Option *opt = subMenu->opts + j;

                // Skip Precache textures toggle
                if (opt->type == OPT_TOGGLE && opt->bval == &configPrecacheRes) {
                    subMenu->numOpts--;
                    continue;
                }

                // Copy option
                omm_copy(newOpts + k++, opt, sizeof(struct Option));
            }
            subMenu->opts = newOpts;
        }
#endif

        // Display sub-menu
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuVideo) {
            struct SubMenu *subMenu = menuMain.opts[i].nextMenu;
            struct Option *newOpts = omm_new(struct Option, subMenu->numOpts + 3);

            // Frame rate
            newOpts[0].type = OPT_CHOICE;
            newOpts[0].choices = omm_new(u8 *, 4);
            newOpts[0].numChoices = 4;
            newOpts[0].uval = &gOmmFrameRate;
#if OMM_GAME_IS_R96X
            newOpts[0].label = (const u8 *) OMM_TEXT_OPT_FRAME_RATE;
            newOpts[0].choices[OMM_FPS_30] = (const u8 *) OMM_TEXT_OPT_FRAME_RATE_30;
            newOpts[0].choices[OMM_FPS_60] = (const u8 *) OMM_TEXT_OPT_FRAME_RATE_60;
            newOpts[0].choices[OMM_FPS_AUTO] = (const u8 *) OMM_TEXT_OPT_FRAME_RATE_AUTO;
            newOpts[0].choices[OMM_FPS_INF] = (const u8 *) OMM_TEXT_OPT_FRAME_RATE_UNLIMITED;
#else
            newOpts[0].label = omm_text_convert(OMM_TEXT_OPT_FRAME_RATE, true);
            newOpts[0].choices[OMM_FPS_30] = omm_text_convert(OMM_TEXT_OPT_FRAME_RATE_30, true);
            newOpts[0].choices[OMM_FPS_60] = omm_text_convert(OMM_TEXT_OPT_FRAME_RATE_60, true);
            newOpts[0].choices[OMM_FPS_AUTO] = omm_text_convert(OMM_TEXT_OPT_FRAME_RATE_AUTO, true);
            newOpts[0].choices[OMM_FPS_INF] = omm_text_convert(OMM_TEXT_OPT_FRAME_RATE_UNLIMITED, true);
#endif

            // Show FPS
            newOpts[1].type = OPT_TOGGLE;
            newOpts[1].bval = &gOmmShowFPS;
#if OMM_GAME_IS_R96X
            newOpts[1].label = (const u8 *) OMM_TEXT_OPT_SHOW_FPS;
#else
            newOpts[1].label = omm_text_convert(OMM_TEXT_OPT_SHOW_FPS, true);
#endif

            // Other options
            // Remove Apply button and 60 FPS option (Render96)
            for (s32 j = 0, k = 2, n = subMenu->numOpts; j != n; ++j) {
                struct Option *opt = subMenu->opts + j;

                // Preload Textures
                if (opt->type == OPT_CHOICE && opt->uval == &configFiltering) {
                    newOpts[k].type = OPT_CHOICE;
                    newOpts[k].choices = omm_new(u8 *, 3);
                    newOpts[k].numChoices = 3;
                    newOpts[k].uval = &gOmmPreloadTextures;
#if OMM_GAME_IS_R96X
                    newOpts[k].label = (const u8 *) OMM_TEXT_OPT_PRELOAD_TEXTURES;
                    newOpts[k].choices[0] = (const u8 *) OMM_TEXT_OPT_PRELOAD_TEXTURES_NEVER;
                    newOpts[k].choices[1] = (const u8 *) OMM_TEXT_OPT_PRELOAD_TEXTURES_FROM_RAM;
                    newOpts[k].choices[2] = (const u8 *) OMM_TEXT_OPT_PRELOAD_TEXTURES_FROM_DISK;
#else
                    newOpts[k].label = omm_text_convert(OMM_TEXT_OPT_PRELOAD_TEXTURES, true);
                    newOpts[k].choices[0] = omm_text_convert(OMM_TEXT_OPT_PRELOAD_TEXTURES_NEVER, true);
                    newOpts[k].choices[1] = omm_text_convert(OMM_TEXT_OPT_PRELOAD_TEXTURES_FROM_RAM, true);
                    newOpts[k].choices[2] = omm_text_convert(OMM_TEXT_OPT_PRELOAD_TEXTURES_FROM_DISK, true);
#endif
                    k++;
                }

                // Skip Apply button
                if (opt->type == OPT_BUTTON && opt->actionFn == optvideo_apply) {
                    subMenu->numOpts--;
                    continue;
                }

#if OMM_GAME_IS_R96X
                // Skip 60 FPS toggle (Render96)
                if (opt->type == OPT_TOGGLE && opt->bval == &config60FPS) {
                    subMenu->numOpts--;
                    continue;
                }
#endif

                // Texture filtering: remove three-point
                if (opt->type == OPT_CHOICE && opt->uval == &configFiltering) {
                    opt->numChoices = 2;
                    configFiltering &= 1;
                }

                // Copy option
                omm_copy(newOpts + k++, opt, sizeof(struct Option));
            }
            subMenu->opts = newOpts;
            subMenu->numOpts += 3;
        }

        // Custom Controls sub-menu
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuControls) {
            head->type = OPT_SUBMENU;
            head->label = gOmmOptControls.label;
            head->nextMenu = (struct SubMenu *) gOmmOptControls.subMenu;
            head++;
            continue;
        }

#if !OMM_GAME_IS_R96X
        // Custom Cheats sub-menu
        if (menuMain.opts[i].type == OPT_SUBMENU && menuMain.opts[i].nextMenu == &menuCheats) {
            head->type = OPT_SUBMENU;
            head->label = gOmmOptCheats.label;
            head->nextMenu = (struct SubMenu *) gOmmOptCheats.subMenu;
            head++;
            continue;
        }
#endif

#if OMM_GAME_IS_SM74
        // Change SM74 mode button
        if (menuMain.opts[i].type == OPT_BUTTON && menuMain.opts[i].actionFn == optmenu_act_exit) {
            head->type = OPT_BUTTON;
            head->label = omm_text_convert(OMM_TEXT_SM74_SWAP_VERSION, true);
            head->actionFn = (void (*)(struct Option *, s32)) omm_opt_sm74_change_mode;
            head++;
        }
#endif

#if !OMM_CODE_DYNOS
        // Return to main menu button
        if (menuMain.opts[i].type == OPT_BUTTON && menuMain.opts[i].actionFn == optmenu_act_exit) {
            head->type = OPT_BUTTON;
            head->label = gOmmOptReturnToMainMenu.label;
            head->actionFn = (void (*)(struct Option *, s32)) omm_opt_return_to_main_menu;
            head++;
        }
#endif

        // Add options
        omm_copy(head++, &menuMain.opts[i], sizeof(struct Option));
    }

    // Update main menu
    menuMain.numOpts = sNumOpts;
    menuMain.opts = opts;
}

#if !OMM_CODE_DYNOS
OMM_ROUTINE_UPDATE(omm_opt_update_num_options) {
    menuMain.numOpts = sNumOpts;
}
#endif

#if !OMM_CODE_DYNOS

#include "gfx_dimensions.h"

#define OMM_OPT_Y                           ((SCREEN_HEIGHT * 2) / 3)
#define OMM_OPT_TITLE_Y                     (OMM_OPT_Y + 24)
#define OMM_OPT_OFFSET_Y                    (16)
#define OMM_OPT_COUNT                       (9)
#define OMM_OPT_LAST                        (OMM_OPT_COUNT - 1)
#define OMM_OPT_HALF_UP                     ((OMM_OPT_COUNT + 1) / 2)
#define OMM_OPT_HALF_DOWN                   ((OMM_OPT_COUNT - 1) / 2)
#define OMM_OPT_LIST_SIZE                   ((OMM_OPT_COUNT * 2) - 1)
#define OMM_OPT_REL(relIndex)               (((subMenu->select + relIndex) < 0) || ((subMenu->select + relIndex) >= subMenu->numOpts) ? NULL : &subMenu->opts[subMenu->select + relIndex])
#define OMM_OPT_COLOR_WHITE                 0xFFFFFFFF
#define OMM_OPT_COLOR_BLACK                 0x000000FF
#define OMM_OPT_COLOR_GRAY                  0xA0A0A0FF
#define OMM_OPT_COLOR_DARK_GRAY             0x808080FF
#define OMM_OPT_COLOR_SELECT                0x80E0FFFF
#define OMM_OPT_COLOR_SELECT_BOX            0x00FFFF20
#define OMM_OPT_COLOR_ENABLED               0x20E020FF
#define OMM_OPT_COLOR_DISABLED              0xFF2020FF
#define OMM_OPT_OFFSET_FROM_LEFT_EDGE       (20.f * sqr_f(GFX_DIMENSIONS_ASPECT_RATIO))
#define OMM_OPT_OFFSET_FROM_RIGHT_EDGE      (20.f * sqr_f(GFX_DIMENSIONS_ASPECT_RATIO))
#define OMM_OPT_SCROLL_BAR_SIZE             ((s32) (40.f * GFX_DIMENSIONS_ASPECT_RATIO))

static const char *omm_opt_int_to_string(const char *fmt, s32 x) {
    static char sBuffer[16];
    snprintf(sBuffer, 16, fmt, x);
    return sBuffer;
}

static void omm_opt_print_string(const void *p, bool convert, s32 x, s32 y, u32 color, bool alignLeft) {
    u8 *str64 = (convert ? omm_text_convert((const char *) p, false) : omm_text_copy((const u8 *) p, false));
    if (color & 0xFF) {
        x = (alignLeft ? GFX_DIMENSIONS_FROM_LEFT_EDGE(x) : GFX_DIMENSIONS_FROM_RIGHT_EDGE(x + omm_render_get_string_width(str64)));
        omm_render_string(x, y, ((color >> 24) & 0xFF), ((color >> 16) & 0xFF), ((color >> 8) & 0xFF), ((color >> 0) & 0xFF), str64, true);
    }
}

static void omm_opt_print_box(s32 x, s32 y, s32 w, s32 h, u32 color, bool alignLeft) {
    if (color & 0xFF) {
        Mtx *mtx = (Mtx *) alloc_display_list(sizeof(Mtx));
        if (!mtx) return;
        create_dl_translation_matrix(MENU_MTX_PUSH, (alignLeft ? GFX_DIMENSIONS_FROM_LEFT_EDGE(x) : GFX_DIMENSIONS_FROM_RIGHT_EDGE(x + w)), y + h, 0);
        guScale(mtx, (f32) w / 130.f, (f32) h / 80.f, 1.f);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        gDPSetEnvColor(gDisplayListHead++, ((color >> 24) & 0xFF), ((color >> 16) & 0xFF), ((color >> 8) & 0xFF), ((color >> 0) & 0xFF));
        gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    }
}

static struct Option **omm_opt_get_current_options(struct SubMenu *subMenu) {
    static struct Option *sOptionList[OMM_OPT_LIST_SIZE];
    s32 start = (OMM_OPT_LIST_SIZE - 1), end = 0;
    for (s32 i = 0; i != OMM_OPT_LIST_SIZE; ++i) {
        s32 j = (i + 1 - OMM_OPT_COUNT);
        sOptionList[i] = OMM_OPT_REL(j);
        if (sOptionList[i]) {
            start = min_s(start, i);
            end = max_s(end, i);
        }
    }
    if ((end - start) < OMM_OPT_COUNT) {
        return &sOptionList[start];
    }
    if (end <= OMM_OPT_LIST_SIZE - OMM_OPT_HALF_UP) {
        return &sOptionList[end - OMM_OPT_LAST];
    }
    if (start >= OMM_OPT_HALF_DOWN) {
        return &sOptionList[start];
    }
    return &sOptionList[OMM_OPT_HALF_DOWN];
}

static void omm_opt_draw_option(struct Option *opt, bool selected, s32 y) {
    if (!opt) {
        return;
    }

    // Selected box
    if (selected) {
        u8 a = (u8) ((coss(gGlobalTimer * 0x800) + 1.f) * 0x20);
        omm_opt_print_box(OMM_OPT_OFFSET_FROM_LEFT_EDGE - 4, y - 4, GFX_DIMENSIONS_FROM_RIGHT_EDGE(OMM_OPT_OFFSET_FROM_RIGHT_EDGE) - GFX_DIMENSIONS_FROM_LEFT_EDGE(OMM_OPT_OFFSET_FROM_LEFT_EDGE) + 8, 16, OMM_OPT_COLOR_SELECT_BOX + a, 1);
    }

    // Label
    omm_opt_print_string(opt->label, false, OMM_OPT_OFFSET_FROM_LEFT_EDGE, y, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 1);

    // Values
    switch (opt->type) {
        case OPT_TOGGLE: {
            if (*opt->bval) {
                omm_opt_print_string(OMM_TEXT_OPT_ENABLED, true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, OMM_OPT_COLOR_ENABLED, 0);
            } else {
                omm_opt_print_string(OMM_TEXT_OPT_DISABLED, true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, OMM_OPT_COLOR_DISABLED, 0);
            }
        } break;

        case OPT_CHOICE: {
            omm_opt_print_string(opt->choices[*opt->uval], false, OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 0);
        } break;

        case OPT_SCROLL: {
            s32 w = (s32) (OMM_OPT_SCROLL_BAR_SIZE * (f32) (*opt->uval - opt->scrMin) / (f32) (opt->scrMax - opt->scrMin));
            omm_opt_print_string(omm_opt_int_to_string("%d", *opt->uval), true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 0);
            omm_opt_print_box(OMM_OPT_OFFSET_FROM_RIGHT_EDGE + 28, y + 1, OMM_OPT_SCROLL_BAR_SIZE + 2, 6, OMM_OPT_COLOR_DARK_GRAY, 0);
            omm_opt_print_box(OMM_OPT_OFFSET_FROM_RIGHT_EDGE + 29 + OMM_OPT_SCROLL_BAR_SIZE - w, y + 2, w, 4, selected ? OMM_OPT_COLOR_SELECT : OMM_OPT_COLOR_WHITE, 0);
        } break;

        case OPT_BIND: {
            for (s32 i = 0; i != 3; ++i) {
                u32 bind = opt->uval[i];
                if (selected && i == optmenu_bind_idx) {
                    if (optmenu_binding) {
                        omm_opt_print_string(OMM_TEXT_OPT_DOT_DOT_DOT, true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_SELECT, 0);
                    } else if (bind == VK_INVALID) {
                        omm_opt_print_string(OMM_TEXT_OPT_NONE, true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_SELECT, 0);
                    } else {
                        omm_opt_print_string(omm_opt_int_to_string("%04X", bind), true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_SELECT, 0);
                    }
                } else {
                    if (bind == VK_INVALID) {
                        omm_opt_print_string(OMM_TEXT_OPT_NONE, true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_GRAY, 0);
                    } else {
                        omm_opt_print_string(omm_opt_int_to_string("%04X", bind), true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE + (2 - i) * 36, y, OMM_OPT_COLOR_WHITE, 0);
                    }
                }
            }
        } break;

        case OPT_SUBMENU: {
            if (selected) {
                omm_opt_print_string(OMM_TEXT_OPT_PRESS_A, true, OMM_OPT_OFFSET_FROM_RIGHT_EDGE, y, OMM_OPT_COLOR_SELECT, 0);
            }
        } break;

        default: {
        } break;
    }
}

static void omm_opt_draw_menu(struct SubMenu *subMenu) {
    if (subMenu) {
        
        // Colorful title
        omm_render_string_hud((SCREEN_WIDTH - omm_render_get_string_width_hud(subMenu->label)) / 2, OMM_OPT_TITLE_Y, 0xFF, 0xFF, 0xFF, 0xFF, subMenu->label, false);

        // Display options
        struct Option *currOpt = &subMenu->opts[subMenu->select];
        struct Option **options = omm_opt_get_current_options(subMenu);
        for (s32 i = 0; i != OMM_OPT_COUNT; ++i) {
            omm_opt_draw_option(options[i], options[i] == currOpt, OMM_OPT_Y - (OMM_OPT_OFFSET_Y * i));
        }

        // Scroll bar
        if (subMenu->numOpts > OMM_OPT_COUNT) {
            s32 h0 = (OMM_OPT_COUNT * OMM_OPT_OFFSET_Y);
            s32 y0 = OMM_OPT_Y + ((3 * OMM_OPT_OFFSET_Y) / 4) - h0;
            s32 h = (s32) (h0 * sqrtf(1.f / (subMenu->numOpts - OMM_OPT_LAST)));
            s32 y = y0 + ((h0 - h) * (1.f - clamp_0_1_f((f32) (subMenu->select - OMM_OPT_HALF_DOWN) / (f32) (subMenu->numOpts - OMM_OPT_LAST))));
            omm_opt_print_box(OMM_OPT_OFFSET_FROM_RIGHT_EDGE - 16, y0 - 1, 6, h0 + 2, OMM_OPT_COLOR_DARK_GRAY, 0);
            omm_opt_print_box(OMM_OPT_OFFSET_FROM_RIGHT_EDGE - 15, y, 4, h, OMM_OPT_COLOR_WHITE, 0);
        }
    }
}

void optmenu_draw(void) {
    omm_opt_draw_menu(currentMenu);

    // Update controls
    if (!optmenu_binding) {

        // Check if the A, R and Start buttons have binds
        // If not, reset binds to default
        for_each_(u32 *, binds, 3, omm_static_array_of(u32 *) {
            gOmmControlsButtonA,
            gOmmControlsTriggerR,
            gOmmControlsButtonStart }) {
            if ((*binds)[0] == VK_INVALID &&
                (*binds)[1] == VK_INVALID &&
                (*binds)[2] == VK_INVALID) {
                omm_opt_reset_binds(*binds);
            }
        }

        // Prevent the game from binding keys and buttons
        // to other binds if they're already bound to A
        // It's not a problem for R and Start, we can
        // still access to the 'Reset Controls' button
        for (s32 i = 0; i != MAX_BINDS; ++i) {
            if (gOmmControlsButtonA[i] != VK_INVALID) {
                for_each_(u32*, binds, 19, omm_static_array_of(u32*) {
                    gOmmControlsButtonB,
                    gOmmControlsButtonX,
                    gOmmControlsButtonY,
                    gOmmControlsButtonStart,
                    gOmmControlsTriggerL,
                    gOmmControlsTriggerR,
                    gOmmControlsTriggerZ,
                    gOmmControlsCUp,
                    gOmmControlsCDown,
                    gOmmControlsCLeft,
                    gOmmControlsCRight,
                    gOmmControlsDUp,
                    gOmmControlsDDown,
                    gOmmControlsDLeft,
                    gOmmControlsDRight,
                    gOmmControlsStickUp,
                    gOmmControlsStickDown,
                    gOmmControlsStickLeft,
                    gOmmControlsStickRight }) {
                    for (s32 j = 0; j != MAX_BINDS; ++j) {
                        if ((*binds)[i] == gOmmControlsButtonA[j]) {
                            (*binds)[i] = VK_INVALID;
                        }
                    }
                }
            }
        }
    }
}

#define optmenu_draw optmenu_draw_unused

#endif

#if OMM_GAME_IS_SM74
#define OMM_OPTIONS_MENU_INL
#include "data/omm/system/omm_sm74.inl"
#undef OMM_OPTIONS_MENU_INL
#endif
