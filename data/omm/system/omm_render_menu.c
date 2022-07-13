#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Sparkly Stars timer
//

static void omm_render_menu_ssd_timer(s16 x, s16 y, s16 w, u8 alpha, s32 timer) {
    static const char *sTimerGlyphs[] = {
        OMM_TEXTURE_HUD_0,
        OMM_TEXTURE_HUD_1,
        OMM_TEXTURE_HUD_2,
        OMM_TEXTURE_HUD_3,
        OMM_TEXTURE_HUD_4,
        OMM_TEXTURE_HUD_5,
        OMM_TEXTURE_HUD_6,
        OMM_TEXTURE_HUD_7,
        OMM_TEXTURE_HUD_8,
        OMM_TEXTURE_HUD_9,
        OMM_TEXTURE_HUD_DOTS,
    };

    s32 glyphs[] = {
        ((timer / 1080000) % 10),
        ((timer /  108000) % 10),
        10,
        ((timer /   18000) %  6),
        ((timer /    1800) % 10),
        10,
        ((timer /     300) %  6),
        ((timer /      30) % 10),
    };
    for (s32 i = 0; i != 8; ++i) {
        omm_render_glyph(
            x + i * OMM_RENDER_SPARKLY_TIMER_OFFSET_X(w), y, w, w,
            0xFF, 0xFF, 0xFF, alpha, sTimerGlyphs[glyphs[i]], false
        );
    }
}

//
// Update
//

static void omm_render_menu_update_score(struct Object *sparklyStarsScore) {
    static s32 sDeleteTimer = 0;
    sparklyStarsScore->oOpacity = 0xFF * invlerp_0_1_f(sparklyStarsScore->oTimer, 0, 10) * sqrtf(invlerp_0_1_f(sDeleteTimer, 90, 30));
    bool closeScoreScreen = false;

    // After holding B for 3 seconds, delete the Sparkly Stars save data
    if (sDeleteTimer >= 90) {
        omm_ssd_clear_mode(sparklyStarsScore->oAction);
        play_sound(SOUND_MARIO_WAAAOOOW, gGlobalSoundArgs);
        closeScoreScreen = true;
    }
    
    // Press A to close the score screen
    else if (gPlayer1Controller->buttonPressed & A_BUTTON) {
        closeScoreScreen = true;
    }
    
    // Hold B to delete the Sparkly Stars save data
    else if (gPlayer1Controller->buttonDown & B_BUTTON) {
        sDeleteTimer++;
    }
    
    // Press B to close the score screen
    else if (sDeleteTimer > 0) {
        closeScoreScreen = true;
    }
    
    // Reset the delete timer if B is released
    else {
        sDeleteTimer = 0;
    }
    
    // Close the score screen
    if (closeScoreScreen) {
        obj_mark_for_deletion(sparklyStarsScore);
        gPlayer1Controller->buttonPressed = 0;
        gPlayer2Controller->buttonPressed = 0;
        gPlayer3Controller->buttonPressed = 0;
        sDeleteTimer = 0;
    }
}

static void omm_render_menu_update_character(struct Object *characterSelectButton, f32 *cursorPos) {

    // Animation
    if (characterSelectButton->oMenuButtonState == MENU_BUTTON_STATE_ZOOM_IN_OUT) {
        characterSelectButton->oMenuButtonScale = lerp_f(sins(invlerp_0_1_f(characterSelectButton->oMenuButtonTimer, 0, 7) * 0x8000), 1.f, 0.8f);
    }
    
    // Select a button by pressing A, B or Start
    else if (gPlayer1Controller->buttonPressed & (A_BUTTON | B_BUTTON | START_BUTTON)) {

        // Change the current character by pressing the character select button
        if (abs_f(cursorPos[0] - (characterSelectButton->oPosX / 67.5f)) < 20.f &&
            abs_f(cursorPos[1] - (characterSelectButton->oPosY / 67.5f)) < 16.f) {

            // Advance the character index by 1 until an unlocked character is found and select it
            do { gOmmCharacter = (gOmmCharacter + 1) % OMM_NUM_PLAYABLE_CHARACTERS;
            } while (!omm_player_is_unlocked(gOmmCharacter));
            omm_player_select(gOmmCharacter);

            // Play the character's 'Here-we-go' sound and change the button appearance
            play_sound(SOUND_MARIO_HERE_WE_GO, gGlobalSoundArgs);
            play_sound(SOUND_MENU_CLICK_FILE_SELECT | 0xFF00, gGlobalSoundArgs);
            characterSelectButton->oMenuButtonState = MENU_BUTTON_STATE_ZOOM_IN_OUT;
            switch (omm_player_get_selected_index()) {
                case OMM_PLAYER_MARIO: characterSelectButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_mario); break;
                case OMM_PLAYER_PEACH: characterSelectButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_peach); break;
                case OMM_PLAYER_LUIGI: characterSelectButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_luigi); break;
                case OMM_PLAYER_WARIO: characterSelectButton->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_menu_button_wario); break;
            }
        }

        // Select a star icon by pressing A to display the Sparkly Stars score screen
        else if (gPlayer1Controller->buttonPressed & A_BUTTON) {
            for (s32 mode = OMM_SSM_NORMAL; mode != OMM_SSM_COUNT; ++mode) {
                if (omm_ssd_is_selectible(mode)) {
                    if (cursorPos[0] >= 56 + mode * 18 && cursorPos[1] >= 72 &&
                        cursorPos[0] <= 73 + mode * 18 && cursorPos[1] <= 90) {
                        spawn_object(characterSelectButton, MODEL_NONE, omm_bhv_act_select_star)->oAction = mode;
                        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundArgs);
                        break;
                    }
                }
            }
        }
    }
}

static void omm_render_menu_update_mode() {
#if !OMM_GAME_IS_SMSR

    // Press L to enable/disable 1 HP mode
    if (gPlayer1Controller->buttonPressed & L_TRIG) {
        play_sound(SOUND_MENU_MARIO_CASTLE_WARP, gGlobalSoundArgs);
        g1HPMode = !g1HPMode;
    }
#endif
}

void omm_render_menu_update_inputs(struct Object *characterSelectButton, struct Object *sparklyStarsScore, f32 *cursorPos) {
    if (sparklyStarsScore) {
        omm_render_menu_update_score(sparklyStarsScore);
    } else if (characterSelectButton) {
        omm_render_menu_update_character(characterSelectButton, cursorPos);
        omm_render_menu_update_mode();
    }
}

void omm_render_menu_update(f32 *cursorPos, s8 selectedButtonId, u8 alpha) {
    if (selectedButtonId >= MENU_BUTTON_PLAY_FILE_A && selectedButtonId <= MENU_BUTTON_PLAY_FILE_D) {
        omm_select_save_file(1 + selectedButtonId - MENU_BUTTON_PLAY_FILE_A);
        omm_player_select(gOmmCharacter);
    } else if (selectedButtonId == MENU_BUTTON_NONE) {
        struct Object *characterSelectButton = obj_get_first_with_behavior(omm_bhv_menu_character_select_button);
        struct Object *sparklyStarsScore = obj_get_first_with_behavior(omm_bhv_act_select_star);
        omm_render_menu_update_inputs(characterSelectButton, sparklyStarsScore, cursorPos);
        omm_render_menu(characterSelectButton, sparklyStarsScore, alpha);
    }
}

//
// Render
//

static void omm_render_menu_character_button(struct Object *characterSelectButton, u8 alpha) {
    if (characterSelectButton) {
        const u8 *str64 = omm_text_convert(omm_player_get_name(omm_player_get_selected_index()), false);
        s16 dx = omm_render_get_string_width(str64) / 2;
        gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
        gDPSetEnvColor(gDisplayListHead++, 0xFF, 0xFF, 0xFF, alpha);
        print_generic_string(253 - dx, 39, str64);
        gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
    }
}

static void omm_render_menu_mode_display(u8 alpha) {
#if !OMM_GAME_IS_SMSR
#if defined(WIDESCREEN)
    s16 hardModeX = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(3);
#else
    s16 hardModeX = -6;
#endif
    if (g1HPMode) {
        omm_render_string(hardModeX, 3, 0xFF, 0xFF, 0xFF, alpha, omm_text_convert(OMM_TEXT_MENU_HARD_MODE_ON, false), false);
    } else {
        omm_render_string(hardModeX, 3, 0x40, 0x40, 0x40, alpha / 2, omm_text_convert(OMM_TEXT_MENU_HARD_MODE_OFF, false), false);
    }
#else
    OMM_UNUSED(alpha);
#endif
}

static void omm_render_menu_stars_icons(u8 alpha) {
    for (s32 mode = OMM_SSM_NORMAL; mode != OMM_SSM_COUNT; ++mode) {
        if (omm_ssd_is_selectible(mode)) {
            if (omm_ssd_is_completed(mode)) {
                omm_render_glyph(216 + 18 * mode, SCREEN_HEIGHT - 51, 16, 16, 0xFF, 0xFF, 0xFF, alpha, OMM_SSX_HUD_GLYPH[mode], false);
            } else {
                omm_render_glyph(216 + 18 * mode, SCREEN_HEIGHT - 51, 16, 16, 0x00, 0x00, 0x00, alpha / 2, OMM_SSX_HUD_GLYPH[mode], false);
            }
        }
    }
}

static void omm_render_menu_score_screen(struct Object *sparklyStarsScore) {
    if (sparklyStarsScore) {
        s32 mode = sparklyStarsScore->oAction;
        omm_render_shade_screen(0.9f * sparklyStarsScore->oOpacity);
        omm_render_glyph(10, SCREEN_HEIGHT - 36, 16, 16, 0xFF, 0xFF, 0xFF, sparklyStarsScore->oOpacity, OMM_SSX_HUD_GLYPH[mode], false);
        omm_render_number(30, SCREEN_HEIGHT - 36, 16, 16, 12, sparklyStarsScore->oOpacity, omm_ssd_get_star_count(mode), 2, true, false);
        omm_render_menu_ssd_timer(220, SCREEN_HEIGHT - 36, 14, sparklyStarsScore->oOpacity, omm_ssd_get_timer(mode));
        u8 r = OMM_SSX_HUD_COLOR[mode][0];
        u8 g = OMM_SSX_HUD_COLOR[mode][1];
        u8 b = OMM_SSX_HUD_COLOR[mode][2];
        for (s32 i = 0; i != 30; ++i) {
            s16 x = 12 + 140 * (i / 15);
            s16 y = SCREEN_HEIGHT - 8 - (44 + 12 * (i % 15));
            u8 *name = omm_ssd_get_star_level_name(mode, i);
            if ((i == 29 && omm_ssd_is_grand_star_collected(mode)) || omm_ssd_is_star_collected(mode, i)) {
                omm_render_string(x, y, r, g, b, sparklyStarsScore->oOpacity, name, false);
            } else {
                omm_render_string(x, y, r / 3, g / 3, b / 3, sparklyStarsScore->oOpacity, name, false);
            }
        }
    }
}

void omm_render_menu(struct Object *characterSelectButton, struct Object *sparklyStarsScore, u8 alpha) {
    omm_render_create_dl_ortho_matrix();
    omm_render_menu_character_button(characterSelectButton, alpha);
    omm_render_menu_mode_display(alpha);
    omm_render_menu_stars_icons(alpha);
    omm_render_menu_score_screen(sparklyStarsScore);
}
