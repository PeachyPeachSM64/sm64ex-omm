#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "menu/intro_geo.h"
#include "level_commands.h"
extern bool omm_save_file_are_all_captures_registered_sm64();

void beh_yellow_background_menu_init(void) {}
void beh_yellow_background_menu_loop(void) {}
void bhv_menu_button_init(void) {}
void bhv_menu_button_loop(void) {}
void bhv_menu_button_manager_init(void) {}
void bhv_menu_button_manager_loop(void) {}
Gfx *geo_file_select_strings_and_menu_cursor(UNUSED s32 callContext, UNUSED struct GraphNode *node, UNUSED Mat4 mtx) { return NULL; }

static struct {
    f32 x, y, scale, alpha;
    f32 vx, vy, vscale, valpha;
} sOmmMainMenuSmoke[20];

static struct {
    s32 timer;
    s32 index;
    s32 yoshi;
} sOmmMainMenu[1];

static struct {
    bool open;
    s32 timer;
    s32 index;
    s32 mode;
} sOmmFileSelect[1];

static struct {
    bool open;
    s32 index;
} sOmmFileCopy[1];

static struct {
    bool open;
    s32 timer;
    bool coins;
} sOmmFileScore[1];

//
// Utils
//

static u32 omm_get_inputs() {
    if (omm_is_transition_active()) {
        return 0;
    }
    static bool inputHold = false;
    static s32 inputTimer = 0;
    u32  inputs = (gPlayer1Controller->buttonPressed);
    bool inputU = (gPlayer1Controller->stickY > +60 || (gPlayer1Controller->buttonDown & U_JPAD));
    bool inputD = (gPlayer1Controller->stickY < -60 || (gPlayer1Controller->buttonDown & D_JPAD));
    bool inputL = (gPlayer1Controller->stickX < -60 || (gPlayer1Controller->buttonDown & L_JPAD));
    bool inputR = (gPlayer1Controller->stickX > +60 || (gPlayer1Controller->buttonDown & R_JPAD));
    if (!inputU && !inputD && !inputL && !inputR) {
        inputHold = 0;
        inputTimer = 0;
    } else if (--inputTimer <= 0) {
        inputs |= inputU * STICK_UP;
        inputs |= inputD * STICK_DOWN;
        inputs |= inputL * STICK_LEFT;
        inputs |= inputR * STICK_RIGHT;
        if (!inputHold) {
            inputTimer = 10;
            inputHold = true;
        } else {
            inputTimer = 3;
        }
    }
    return inputs;
}

//
// Complete save cheat
// Press C-Up, C-Up, C-Down, C-Down, C-Left, C-Right, C-Left, C-Right, Z, R and select a file with A or Start
// Collects all stars, sets all flags, opens all cannons, register all captures and unlocks Peach
//

static const u16 sOmmCompleteSaveButtons[] = { U_CBUTTONS, U_CBUTTONS, D_CBUTTONS, D_CBUTTONS, L_CBUTTONS, R_CBUTTONS, L_CBUTTONS, R_CBUTTONS, Z_TRIG, R_TRIG, A_BUTTON | START_BUTTON, 0xFFFF };
static s32 sOmmCompleteSaveSequenceIndex = 0;
extern void omm_set_complete_save_file(s32 fileIndex, s32 modeIndex);

static s32 omm_update_complete_save_sequence_index() {
    if (gPlayer1Controller->buttonPressed != 0) {
        u16 buttonPressed = gPlayer1Controller->buttonPressed;
        u16 buttonRequired = sOmmCompleteSaveButtons[sOmmCompleteSaveSequenceIndex];
        if ((buttonPressed & buttonRequired) != 0 && (buttonPressed & ~buttonRequired) == 0) {
            sOmmCompleteSaveSequenceIndex++;
            if (sOmmCompleteSaveButtons[sOmmCompleteSaveSequenceIndex] == (A_BUTTON | START_BUTTON)) {
                sound_banks_enable(SEQ_PLAYER_SFX, 1 << 8);
                play_sound(SOUND_GENERAL2_RIGHT_ANSWER | 0xFF00, gGlobalSoundArgs);
            }
        } else {
            sOmmCompleteSaveSequenceIndex = 0;
        }
    }
    return sOmmCompleteSaveSequenceIndex;
}

static void omm_check_complete_save(s32 fileIndex, s32 modeIndex) {
    if (sOmmCompleteSaveSequenceIndex == array_length(sOmmCompleteSaveButtons) - 1) {
        omm_set_complete_save_file(fileIndex, modeIndex);
        omm_secrets_unlock(OMM_SECRET_TOAD_CHEAT_CODE);
    }
    sOmmCompleteSaveSequenceIndex = 0;
}

//
// Main Menu constants
//

#define GFX_DIMENSIONS_SCREEN_WIDTH     ((s32) (GFX_DIMENSIONS_FROM_RIGHT_EDGE(0) - GFX_DIMENSIONS_FROM_LEFT_EDGE(0) + 0.5f))

#define OMM_MM_PLAY                     (0)
#define OMM_MM_PLAY_NO_SAVE             (1)
#define OMM_MM_COPY                     (2)
#define OMM_MM_ERASE                    (3)
#define OMM_MM_SCORE                    (4)
#define OMM_MM_OPTIONS                  (5)
#define OMM_MM_PALETTE_EDITOR           (6)

#if OMM_GAME_IS_R96X
#define OMM_MM_LOGO_TEXTURE             "menu/" OMM_GAME_MENU "/logo.r96x.rgba32"
#else
#define OMM_MM_LOGO_TEXTURE             "menu/" OMM_GAME_MENU "/logo.rgba32"
#endif
#define OMM_MM_LOGO_W                   (OMM_MM_LOGO_H * 1.6f)
#define OMM_MM_LOGO_H                   (SCREEN_HEIGHT / 3)
#define OMM_MM_LOGO_X                   GFX_DIMENSIONS_FROM_LEFT_EDGE(0)
#define OMM_MM_LOGO_Y                   (SCREEN_HEIGHT - OMM_MM_LOGO_H)

#define OMM_MM_STRINGS_COUNT            ((s32) array_length(sOmmMainMenuStrings))
#define OMM_MM_STRINGS_X                GFX_DIMENSIONS_FROM_LEFT_EDGE(48)
#define OMM_MM_STRINGS_Y                ((11 * SCREEN_HEIGHT) / 20)
#define OMM_MM_STRINGS_H                (16)

#define OMM_MM_BOX_X                    (OMM_MM_STRINGS_X - 20)
#define OMM_MM_BOX_Y                    (OMM_MM_STRINGS_Y - 4 - OMM_MM_STRINGS_H * sOmmMainMenu->index)
#define OMM_MM_BOX_W                    (135 * GFX_DIMENSIONS_ASPECT_RATIO)
#define OMM_MM_BOX_H                    (OMM_MM_STRINGS_H)
#define OMM_MM_BOX_A                    (0x60 + 0x40 * sins(sOmmMainMenu->timer * 0x800))

#define OMM_MM_CURSOR_X                 (OMM_MM_STRINGS_X - 16)
#define OMM_MM_CURSOR_Y                 (OMM_MM_STRINGS_Y - 2 - OMM_MM_STRINGS_H * sOmmMainMenu->index)
#define OMM_MM_CURSOR_W                 (12)
#define OMM_MM_CURSOR_H                 (OMM_MM_CURSOR_W + (OMM_MM_CURSOR_W / 3) * sins(clamp_s(sOmmMainMenu->timer % 90, 0, 16) * 0x1000))

#define OMM_MM_INFO_STRING_X            GFX_DIMENSIONS_FROM_LEFT_EDGE(4)
#define OMM_MM_INFO_STRING_Y            (4)
#define OMM_MM_INFO_STRING_W            (5)
#define OMM_MM_INFO_STRING_H            (5)
#define OMM_MM_INFO_STRING_C            (0x80)

#define OMM_MM_INFO_BOX_X               (OMM_MM_INFO_STRING_X - 2)
#define OMM_MM_INFO_BOX_Y               (OMM_MM_INFO_STRING_Y - 2)
#define OMM_MM_INFO_BOX_W               (170 * GFX_DIMENSIONS_ASPECT_RATIO)
#define OMM_MM_INFO_BOX_H               (OMM_MM_INFO_STRING_H + 4)
#define OMM_MM_INFO_BOX_A               (0xC0)

#define OMM_MM_SOUND_SCROLL             (SOUND_MENU_CHANGE_SELECT | 0xFE00)
#define OMM_MM_SOUND_FILE_SELECT        (SOUND_MENU_STAR_SOUND | 0xFF00)
#define OMM_MM_SOUND_RESUME_GAME        omm_main_menu_get_mario_sound(SOUND_MENU_STAR_SOUND_OKEY_DOKEY)
#define OMM_MM_SOUND_PLAY_NO_SAVE       omm_main_menu_get_mario_sound(SOUND_MENU_STAR_SOUND_OKEY_DOKEY)

static struct { const char *label; const char *info[2]; } sOmmMainMenuStrings[] = {
    [OMM_MM_PLAY]           = { OMM_TEXT_MM_PLAY,           { OMM_TEXT_MM_INFO_PLAY,           OMM_TEXT_MM_INFO_PLAY             } },
    [OMM_MM_PLAY_NO_SAVE]   = { OMM_TEXT_MM_PLAY_NO_SAVE,   { OMM_TEXT_MM_INFO_PLAY_NO_SAVE,   OMM_TEXT_MM_INFO_PLAY_NO_SAVE     } },
    [OMM_MM_COPY]           = { OMM_TEXT_MM_COPY,           { OMM_TEXT_MM_INFO_COPY,           OMM_TEXT_MM_INFO_COPY             } },
    [OMM_MM_ERASE]          = { OMM_TEXT_MM_ERASE,          { OMM_TEXT_MM_INFO_ERASE,          OMM_TEXT_MM_INFO_ERASE            } },
    [OMM_MM_SCORE]          = { OMM_TEXT_MM_SCORE,          { OMM_TEXT_MM_INFO_SCORE,          OMM_TEXT_MM_INFO_SCORE            } },
    [OMM_MM_OPTIONS]        = { OMM_TEXT_MM_OPTIONS,        { OMM_TEXT_MM_INFO_OPTIONS,        OMM_TEXT_MM_INFO_OPTIONS          } },
    [OMM_MM_PALETTE_EDITOR] = { OMM_TEXT_MM_PALETTE_EDITOR, { OMM_TEXT_MM_INFO_PALETTE_EDITOR, OMM_TEXT_MM_INFO_PALETTE_EDITOR_2 } },
};

//
// Main Menu Mario animation
//

#pragma GCC push_options
#pragma GCC diagnostic ignored "-Wmissing-braces"

static const u16 anim_C2_indices[];
static const s16 anim_C2_values[];
#undef ANIMINDEX_NUMPARTS
#define ANIMINDEX_NUMPARTS(...) 0
#include "assets/anims/anim_C2.inc.c"

#pragma GCC pop_options

static const struct Animation sOmmMainMenuMarioAnim = {
    0, 0, 0, 0, 90, 0,
    anim_C2_values,
    anim_C2_indices,
    ANIM_LENGTH(array_length(anim_C2_indices), array_length(anim_C2_values)),
};

static const struct Animation *sOmmMainMenuMarioAnims[] = { &sOmmMainMenuMarioAnim };

//
// Main Menu update
//

static s32 omm_main_menu_get_mario_sound(s32 soundBits) {
    switch (soundBits) {
        case SOUND_MENU_STAR_SOUND_OKEY_DOKEY: {
            if (gOmmGlobals->yoshiMode) {
                play_sound(SOUND_MENU_STAR_SOUND | 0xFF00, gGlobalSoundArgs);
                return SOUND_GENERAL_YOSHI_TALK | 0xFF00;
            }
#if OMM_GAME_IS_SMSR
            if (gOmmGlobals->booZeroLife) {
                play_sound(SOUND_MENU_STAR_SOUND | 0xFF00, gGlobalSoundArgs);
                return SOUND_OBJ_BOO_LAUGH_LONG | 0xFF00;
            }
#endif
            return SOUND_MENU_STAR_SOUND_OKEY_DOKEY | 0xFF00;
        } break;

        case SOUND_MARIO_WAAAOOOW: {
            if (gOmmGlobals->yoshiMode) {
                return SOUND_GENERAL_SPLATTERING | 0xFF00;
            }
#if OMM_GAME_IS_SMSR
            if (gOmmGlobals->booZeroLife) {
                return SOUND_OBJ_BOO_LAUGH_SHORT | 0xFF00;
            }
#endif
            return SOUND_MARIO_WAAAOOOW | 0xFF00;
        } break;
    }
}

static f32 omm_main_menu_get_mario_cappy_pos_y(f32 t, f32 value_at_0, f32 peak, f32 t_peak) {
    f32 a = (value_at_0 - peak) / sqr_f(t_peak);
    f32 b = -2 * (value_at_0 - peak) / t_peak;
    f32 c = value_at_0;
    return a * t * t + b * t + c;
}

static f32 omm_main_menu_get_yoshi_head_height(struct Object *o) {
    static OmmMap_(struct GraphNode *, f32) sOmmMainMenuYoshiHeadHeights = omm_map_zero;

    // Find in cache
    s32 i = omm_map_find_key(sOmmMainMenuYoshiHeadHeights, ptr, o->oGraphNode);
    if (i != -1) {
        return omm_map_get_val(sOmmMainMenuYoshiHeadHeights, f32, i);
    }

    // Compute value for graph node
    Mat4 transform;
    obj_set_pos_vec3f(o, gVec3fZero);
    obj_set_angle_vec3s(o, gVec3sZero);
    obj_set_scale_vec3f(o, gVec3fOne);
    obj_update_gfx(o);
    obj_anim_play_with_sound(o, 0, 1.f, NO_SOUND, true);
    if (geo_compute_capture_cappy_obj_transform(o, 4, transform)) {
        f32 height = transform[3][1];
        omm_map_add(sOmmMainMenuYoshiHeadHeights, ptr, o->oGraphNode, f32, height);
        return height;
    }
    return 0;
}

static void omm_main_menu_update_mario_cappy(const BehaviorScript *bhv, f32 tmax) {
    struct Object *o = obj_get_first_with_behavior(bhv);
    if (!o) {
        return;
    }

    // Default values
    Vec3f pos = { 175 * GFX_DIMENSIONS_ASPECT_RATIO, 0, 0 };
    Vec3s angle = { 0, -0x41A * GFX_DIMENSIONS_ASPECT_RATIO, 0 };
    Vec3f scale = { 1, 1, 1 };
    s32 modelId = MODEL_MARIO;
    const GeoLayout *geoLayout = NULL;
    const struct Animation *const *animations = NULL;
    f32 t = relerp_0_1_f(o->oTimer, 0, tmax, 1, 0);

    // Cappy
    if (bhv == bhvOmmMainMenuCappy) {
        pos[1] = omm_main_menu_get_mario_cappy_pos_y(t, 0, 100, 0.225f) + relerp_0_1_f(sins(o->oTimer * 0x400), -1, 1, 20, 60);
        angle[0] = -0x400;
        angle[2] = relerp_0_1_f(coss(o->oTimer * 0x6B7), -1, 1, 0, -0x400);
        vec3f_set(scale, 6, 6, 6);
        modelId = MODEL_MARIOS_CAP;
        geoLayout = marios_cap_geo;
    }

    // Yoshi mode
    else if (gOmmGlobals->yoshiMode) {
        if (obj_has_geo_layout(o, yoshi_geo)) {
            f32 s = 450.f / omm_main_menu_get_yoshi_head_height(o);
            pos[1] = omm_main_menu_get_mario_cappy_pos_y(t, -950, -850, 0.2f) + 125;
            angle[0] = -0x800;
            vec3f_set(scale, s, s, s * 0.5f);
        } else {
            vec3f_set(scale, 0, 0, 0);
        }
        modelId = MODEL_YOSHI;
        geoLayout = yoshi_geo;
        animations = yoshi_seg5_anims_05024100;
    }

#if OMM_GAME_IS_SMSR
    // Zero-life Boo
    else if (gOmmGlobals->booZeroLife) {
        f32 sin01 = relerp_0_1_f(sins(o->oTimer * 0x800), -1, 1, 0, 1);
        pos[1] = omm_main_menu_get_mario_cappy_pos_y(t, -950, -850, 0.2f) + 725.f - 25.f * sin01;
        angle[0] = -0x800 - 0x200 * sin01;
        vec3f_set(scale,
            3.5f * (1.f + sins(o->oTimer * 0x400) * 0.03f),
            3.5f * (1.f - sins(o->oTimer * 0x400) * 0.03f),
            3.5f * (1.f + sins(o->oTimer * 0x400) * 0.03f)
        );
        modelId = MODEL_BOO;
        geoLayout = boo_geo;
    }
#endif

    // Mario
    else {
        f32 headHeight = geo_get_marios_anim_part_height(o->oGraphNode, MARIO_ANIM_PART_HEAD);
        pos[1] = omm_main_menu_get_mario_cappy_pos_y(t, -950, -850, 0.2f) - (7.f * headHeight - (564.2f + (headHeight - 85.f) * sign_f(headHeight - 85.f))); // don't ask
        angle[0] = -0x800;
        vec3f_set(scale, 7, 7, 7);
        modelId = MODEL_MARIO;
        geoLayout = mario_geo;
        animations = sOmmMainMenuMarioAnims;
    }

    // Update gfx
    obj_set_pos_vec3f(o, pos);
    obj_set_angle_vec3s(o, angle);
    obj_set_scale_vec3f(o, scale);
    obj_update_gfx(o);
    o->oGraphNode = gLoadedGraphNodes[modelId] = geo_layout_to_graph_node(NULL, geoLayout);
    if (animations) {
        o->oAnimations = (struct Animation **) animations;
        obj_anim_play(o, 0, 1.f);
    }
    o->oOpacity = 0xFF;
    o->oNodeFlags |= GRAPH_RENDER_ALWAYS;
    o->oTimer += !optmenu_open;
    if (gOmmExtrasInvisibleMode) {
        o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    } else {
        o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    }
}

static s32 omm_main_menu_update() {
    sOmmMainMenu->timer += !optmenu_open;
    gAreaUpdateCounter += !optmenu_open;

    // Update Mario and Cappy
    omm_main_menu_update_mario_cappy(bhvOmmMainMenuMario, 20);
    omm_main_menu_update_mario_cappy(bhvOmmMainMenuCappy, 30);

    // Stop updating if the options menu is open
    if (optmenu_open) {
        return 0;
    }

    // Transition to File select screen
    if (sOmmFileSelect->timer) {
        sOmmFileSelect->open = (--sOmmFileSelect->timer == 0);
        return 0;
    }

    // Update inputs
    u32 inputs = omm_get_inputs();

    // Yoshi mode
    // TODO: YOSHIMODE
//     if (inputs == Y_BUTTON && omm_save_file_are_all_captures_registered_sm64()) {
//         if (++sOmmMainMenu->yoshi == 5) {
//             gOmmGlobals->yoshiMode = !gOmmGlobals->yoshiMode;
//             if (gOmmGlobals->yoshiMode) {
//                 play_sound(SOUND_GENERAL_YOSHI_TALK, gGlobalSoundArgs);
// #if OMM_GAME_IS_SMSR
//             } else if (gOmmGlobals->booZeroLife) {
//                 play_sound(SOUND_OBJ_BOO_LAUGH_LONG, gGlobalSoundArgs);
// #endif
//             } else {
//                 play_sound(SOUND_MARIO_HERE_WE_GO, gGlobalSoundArgs);
//             }
//             for (u32 i = 0; i != array_length(sOmmMainMenuSmoke); ++i) {
//                 sOmmMainMenuSmoke[i].x = lerp_f(0.72f, GFX_DIMENSIONS_FROM_LEFT_EDGE(0), GFX_DIMENSIONS_FROM_RIGHT_EDGE(0)) + 40 * random_float_n1_p1();
//                 sOmmMainMenuSmoke[i].y = (SCREEN_HEIGHT / 4) + (SCREEN_HEIGHT / 4) * random_float_n1_p1();
//                 sOmmMainMenuSmoke[i].scale = 60.f + 40.f * random_float();
//                 sOmmMainMenuSmoke[i].alpha = 0xC0 + 0x40 * random_float();
//                 sOmmMainMenuSmoke[i].vx = 6.f * random_float_n1_p1();
//                 sOmmMainMenuSmoke[i].vy = 3.f * random_float_n1_p1();
//                 sOmmMainMenuSmoke[i].vscale = 1.f + 2.f * random_float();
//                 sOmmMainMenuSmoke[i].valpha = sOmmMainMenuSmoke[i].alpha / 30;
//             }
//             sOmmMainMenu->yoshi = 0;
//         }
//     } else if (inputs != 0) {
//         sOmmMainMenu->yoshi = 0;
//     }

    // Option above
    if (inputs & STICK_UP) {
        play_sound(OMM_MM_SOUND_SCROLL, gGlobalSoundArgs);
        sOmmMainMenu->index = (sOmmMainMenu->index + OMM_MM_STRINGS_COUNT - 1) % OMM_MM_STRINGS_COUNT;
        sOmmMainMenu->timer = 8;
    }

    // Option below
    else if (inputs & STICK_DOWN) {
        play_sound(OMM_MM_SOUND_SCROLL, gGlobalSoundArgs);
        sOmmMainMenu->index = (sOmmMainMenu->index + 1) % OMM_MM_STRINGS_COUNT;
        sOmmMainMenu->timer = 8;
    }

    // Next menu (A)
    else if (inputs & A_BUTTON) {
        switch (sOmmMainMenu->index) {

            // Open the file select screen
            case OMM_MM_PLAY:
            case OMM_MM_COPY:
            case OMM_MM_ERASE:
            case OMM_MM_SCORE: {
                play_sound(OMM_MM_SOUND_FILE_SELECT, gGlobalSoundArgs);
                sOmmFileSelect->timer = 30;
                sOmmFileSelect->index = 0;
                sOmmFileSelect->mode = 0;
            } break;

            // Start a new no-save game
            case OMM_MM_PLAY_NO_SAVE: {
                play_sound(OMM_MM_SOUND_PLAY_NO_SAVE, gGlobalSoundArgs);
#if OMM_GAME_IS_SM74
                // Normal Edition
                omm_save_file_erase(NUM_SAVE_FILES, OMM_SM74_MODE_NORMAL);
                omm_select_save_file(NUM_SAVE_FILES, OMM_SM74_MODE_NORMAL, COURSE_NONE, false);
#else
                // Play intro cutscene
                omm_save_file_erase(NUM_SAVE_FILES, OMM_GAME_MODE);
                omm_select_save_file(NUM_SAVE_FILES, OMM_GAME_MODE, COURSE_NONE, false);
#endif
                return NUM_SAVE_FILES + 1;
            } break;

            // Open the options menu
            case OMM_MM_OPTIONS: {
                optmenu_toggle();
                gPlayer1Controller->buttonPressed = 0;
                gPlayer1Controller->rawStickX = 0;
                gPlayer1Controller->rawStickY = 0;
                gPlayer1Controller->stickX = 0;
                gPlayer1Controller->stickY = 0;
                gPlayer1Controller->stickMag = 0;
                gPlayer1Controller->extStickX = 0;
                gPlayer1Controller->extStickY = 0;
            } break;

            // Open the palette editor
            case OMM_MM_PALETTE_EDITOR: {
                omm_palette_editor_open();
            } break;
        }
    }

    // Next menu (Start)
    else if (inputs & START_BUTTON) {
        switch (sOmmMainMenu->index) {

            // Resume the game from the last file, mode and course
            case OMM_MM_PLAY: {
                s32 fileIndex = omm_save_file_get_last_file_index();
                s32 modeIndex = omm_save_file_get_last_mode_index();
                if (omm_save_file_exists(fileIndex, modeIndex)) {
                    s32 courseNum = omm_save_file_get_last_course_num(fileIndex, modeIndex);
                    play_sound(OMM_MM_SOUND_RESUME_GAME, gGlobalSoundArgs);
                    omm_select_save_file(fileIndex, modeIndex, courseNum, false);
#if OMM_GAME_IS_R96X
                    r96_jingle_fade(true, -1, 0, 2000);
#endif
                    return fileIndex + 1;
                }
            } break;

            // Start a new no-save game
            case OMM_MM_PLAY_NO_SAVE: {
                play_sound(OMM_MM_SOUND_PLAY_NO_SAVE, gGlobalSoundArgs);
#if OMM_GAME_IS_SM74
                // Extreme Edition
                omm_save_file_erase(NUM_SAVE_FILES, OMM_SM74_MODE_EXTREME);
                omm_select_save_file(NUM_SAVE_FILES, OMM_SM74_MODE_EXTREME, COURSE_NONE, false);
#else
                // Skip intro cutscene
                omm_save_file_erase(NUM_SAVE_FILES, OMM_GAME_MODE);
                omm_select_save_file(NUM_SAVE_FILES, OMM_GAME_MODE, COURSE_NONE, true);
#endif
                return NUM_SAVE_FILES + 1;
            } break;
        }
    }

    return 0;
}

//
// Main Menu render
//

static void omm_main_menu_render_box(s16 x, s16 y, s16 w, s16 h, u8 r, u8 g, u8 b, u8 a0, u8 a1) {
    Vtx *vtx = (Vtx *) alloc_display_list(sizeof(Vtx) * 4);
    vtx[0] = (Vtx) { { { x,     y,     0 }, 0, { 0, 0 }, { r, g, b, a0 } } };
    vtx[1] = (Vtx) { { { x,     y + h, 0 }, 0, { 0, 0 }, { r, g, b, a0 } } };
    vtx[2] = (Vtx) { { { x + w, y,     0 }, 0, { 0, 0 }, { r, g, b, a1 } } };
    vtx[3] = (Vtx) { { { x + w, y + h, 0 }, 0, { 0, 0 }, { r, g, b, a1 } } };
    omm_render_create_dl_ortho_matrix();
    OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 2, 1, 0, 0, 1, 2, 3, 0);
}

static void omm_main_menu_render() {

    // Update Mario eyes
    gMarioState->marioBodyState->eyeState = ((OMM_MARIO_COLORS && omm_models_get_mario_model_pack_index() == -1) ? MARIO_EYES_LOOK_UP : MARIO_EYES_OPEN);

    // Smoke
    for (u32 i = 0; i != array_length(sOmmMainMenuSmoke); ++i) {
        if (sOmmMainMenuSmoke[i].alpha > 0) {
            omm_render_texrect(
                sOmmMainMenuSmoke[i].x - sOmmMainMenuSmoke[i].scale / 2.f,
                sOmmMainMenuSmoke[i].y - sOmmMainMenuSmoke[i].scale / 2.f,
                sOmmMainMenuSmoke[i].scale, sOmmMainMenuSmoke[i].scale,
                32, 32, 0xFF, 0xFF, 0xFF,
                sOmmMainMenuSmoke[i].alpha,
                OMM_ASSET_SMOKE, false
            );
            sOmmMainMenuSmoke[i].x += sOmmMainMenuSmoke[i].vx;
            sOmmMainMenuSmoke[i].y += sOmmMainMenuSmoke[i].vy;
            sOmmMainMenuSmoke[i].scale += sOmmMainMenuSmoke[i].vscale;
            sOmmMainMenuSmoke[i].alpha -= sOmmMainMenuSmoke[i].valpha;
        }
    }

    // Game logo
    omm_render_texrect(OMM_MM_LOGO_X, OMM_MM_LOGO_Y, OMM_MM_LOGO_W, OMM_MM_LOGO_H, 1024, 640, 0xFF, 0xFF, 0xFF, 0xFF, OMM_MM_LOGO_TEXTURE, false);

    // Options menu
    if (optmenu_open) {
        omm_render_create_dl_ortho_matrix();
        omm_render_shade_screen(0x7F);
        optmenu_draw();
        optmenu_check_buttons();
        return;
    }

    // Main strings
    omm_main_menu_render_box(OMM_MM_BOX_X, OMM_MM_BOX_Y, OMM_MM_BOX_W, OMM_MM_BOX_H, 0x00, 0xFF, 0xFF, OMM_MM_BOX_A, 0x00);
    omm_render_texrect(OMM_MM_CURSOR_X, OMM_MM_CURSOR_Y, OMM_MM_CURSOR_W, OMM_MM_CURSOR_H, 256, 256, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MENU_CURSOR, false);
    for (s32 i = 0; i != OMM_MM_STRINGS_COUNT; ++i) {
        const u8 *str = omm_text_convert(sOmmMainMenuStrings[i].label, false);
        omm_render_string(OMM_MM_STRINGS_X, OMM_MM_STRINGS_Y - OMM_MM_STRINGS_H * i, 0xFF, 0xFF, 0xFF, 0xFF, str, true);
    }

    // Info string
    omm_main_menu_render_box(OMM_MM_INFO_BOX_X, OMM_MM_INFO_BOX_Y, OMM_MM_INFO_BOX_W, OMM_MM_INFO_BOX_H, 0x00, 0x00, 0x00, OMM_MM_INFO_BOX_A, 0x00);
    const u8 *info = omm_text_convert(sOmmMainMenuStrings[sOmmMainMenu->index].info[OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED], false);
    omm_render_string_sized(OMM_MM_INFO_STRING_X, OMM_MM_INFO_STRING_Y, OMM_MM_INFO_STRING_W, OMM_MM_INFO_STRING_H, OMM_MM_INFO_STRING_C, OMM_MM_INFO_STRING_C, OMM_MM_INFO_STRING_C, 0xFF, info, false);

    // Transition to file select
    if (sOmmFileSelect->timer) {
        omm_render_texrect(
            GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 0, GFX_DIMENSIONS_SCREEN_WIDTH, SCREEN_HEIGHT,
            32, 32, 0xFF, 0xFF, 0xFF, 0xFF * relerp_0_1_f(sOmmFileSelect->timer, 30, 15, 0, 1), OMM_TEXTURE_MISC_WHITE, false
        );
    }
}

//
// File Select constants
//

#define OMM_FS_BACKGROUND_MARGIN                (16)
#define OMM_FS_BACKGROUND_BORDER                (12)

#define OMM_FS_TITLE_H                          (24)
#define OMM_FS_TITLE_Y                          (SCREEN_HEIGHT - (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER + OMM_FS_TITLE_H))

#define OMM_FS_FILE_BUTTON_MARGIN_OUT           (16)
#define OMM_FS_FILE_BUTTON_MARGIN_IN            (8)
#define OMM_FS_FILE_BUTTON_WIDTH                (48)
#define OMM_FS_FILE_BUTTON_HEIGHT               (36)
#define OMM_FS_FILE_BUTTON_BORDER               (4)

#define OMM_FS_L_SIZE                           (8)
#define OMM_FS_L_MARGIN                         (OMM_FS_BACKGROUND_MARGIN - OMM_FS_L_SIZE) / 2
#define OMM_FS_L_X                              GFX_DIMENSIONS_FROM_LEFT_EDGE(OMM_FS_L_MARGIN)
#define OMM_FS_L_Y                              (SCREEN_HEIGHT - (OMM_FS_L_MARGIN + OMM_FS_L_SIZE))

#define OMM_FS_BOX_X                            (x - OMM_FS_FILE_BUTTON_WIDTH / 2 - 8)
#define OMM_FS_BOX_Y                            (y - OMM_FS_FILE_BUTTON_HEIGHT / 2 - 8 - 8 * OMM_FS_MODE_IS_SAVE_MODE)
#define OMM_FS_BOX_W                            (w - 8)
#define OMM_FS_BOX_H                            (OMM_FS_FILE_BUTTON_HEIGHT + 16 + 8 * OMM_FS_MODE_IS_SAVE_MODE)
#define OMM_FS_BOX_A                            (0x60 + 0x40 * sins(sOmmFileSelect->timer * 0x800))

#define OMM_FS_SOUND_SCROLL                     (SOUND_MENU_CHANGE_SELECT | 0xFE00)
#define OMM_FS_SOUND_RETURN                     (SOUND_MENU_HAND_DISAPPEAR | 0xFF00)
#define OMM_FS_SOUND_CHANGE_MODE                (SOUND_MENU_MARIO_CASTLE_WARP | 0xFF00)
#define OMM_FS_SOUND_PLAY                       omm_main_menu_get_mario_sound(SOUND_MENU_STAR_SOUND_OKEY_DOKEY)
#define OMM_FS_SOUND_COPY                       (SOUND_MENU_STAR_SOUND | 0xFF00)
#define OMM_FS_SOUND_ERASE                      omm_main_menu_get_mario_sound(SOUND_MARIO_WAAAOOOW)
#define OMM_FS_SOUND_SCORE_OPEN                 (SOUND_MENU_CAMERA_ZOOM_IN | 0xFF00)
#define OMM_FS_SOUND_SCORE_CLOSE                (SOUND_MENU_CAMERA_ZOOM_OUT | 0xFF00)
#define OMM_FS_SOUND_SCORE_COINS                (SOUND_MENU_CLICK_FILE_SELECT | 0xFE00)
#define OMM_FS_SOUND_INVALID                    (SOUND_MENU_CAMERA_BUZZ | 0xFF00)

#define OMM_FS_MODE_SPARKLY_STARS               (OMM_NUM_SAVE_MODES + 0)
#define OMM_FS_MODE_COUNT                       (OMM_NUM_SAVE_MODES + 1)
#define OMM_FS_MODE_IS_AVAILABLE(mode)          (sOmmFileSelectStrings[sOmmMainMenu->index][mode] != NULL)
#define OMM_FS_MODE_IS_SAVE_MODE                (sOmmFileSelect->mode < OMM_NUM_SAVE_MODES)
#define OMM_FS_MODE_IS_SPARKLY_STARS            (sOmmFileSelect->mode == OMM_FS_MODE_SPARKLY_STARS)

#define OMM_FS_SCORE_COURSE_FONT_SIZE           (5)
#define OMM_FS_SCORE_COURSE_FONT_SIZE_RATIO     (OMM_FS_SCORE_COURSE_FONT_SIZE / 8.f)
#define OMM_FS_SCORE_COURSE_DATA_GAP            (12 * (1.f - (sl / 24.f)))
#define OMM_FS_SCORE_COURSE_LINE_Y              (SCREEN_HEIGHT - 64 - 10 * lineIndex)
#define OMM_FS_SCORE_COURSE_LEVEL_X_LEFT        (sl + 10)
#define OMM_FS_SCORE_COURSE_LEVEL_X_RIGHT       (sr - 10 - OMM_FS_SCORE_COURSE_DATA_GAP - maxWidth * OMM_FS_SCORE_COURSE_FONT_SIZE_RATIO - (OMM_FS_SCORE_COURSE_FONT_SIZE + 1) * 7)
#define OMM_FS_SCORE_COURSE_COINS_X_LEFT        (sl + 12 + OMM_FS_SCORE_COURSE_DATA_GAP + maxWidth * OMM_FS_SCORE_COURSE_FONT_SIZE_RATIO)
#define OMM_FS_SCORE_COURSE_COINS_X_RIGHT       (sr -  8 - (OMM_FS_SCORE_COURSE_FONT_SIZE + 2) * 7)
#define OMM_FS_SCORE_COURSE_STARS_X_LEFT        (sl + 12 + OMM_FS_SCORE_COURSE_DATA_GAP + maxWidth * OMM_FS_SCORE_COURSE_FONT_SIZE_RATIO + (OMM_FS_SCORE_COURSE_FONT_SIZE + 2) * k)
#define OMM_FS_SCORE_COURSE_STARS_X_RIGHT       (sr -  8 - (OMM_FS_SCORE_COURSE_FONT_SIZE + 2) * (7 - k))

#define OMM_FS_SCORE_SPARKLY_LINE_Y             (SCREEN_HEIGHT - 52 - 12 * j)
#define OMM_FS_SCORE_SPARKLY_LEVEL_X_LEFT       (sl + 10)
#define OMM_FS_SCORE_SPARKLY_LEVEL_X_RIGHT      (sr - 10 - sparklyLevels[i].maxWidth)

#define OMM_FS_COURSE_TEXTURE(mode, course)     "menu/" OMM_GAME_MENU "/" STRINGIFY(mode) "/" STRINGIFY(course)  ".rgba32"
#define OMM_FS_COURSE_TEXTURES(course)          OMM_FS_COURSE_TEXTURE(0, course), OMM_FS_COURSE_TEXTURE(1, course), OMM_FS_COURSE_TEXTURE(2, course), OMM_FS_COURSE_TEXTURE(3, course)

static const char *sOmmFileSelectCourses[][4] = {
    [COURSE_NONE]  = { OMM_FS_COURSE_TEXTURES(COURSE_NONE)  },
    [COURSE_BOB]   = { OMM_FS_COURSE_TEXTURES(COURSE_BOB)   },
    [COURSE_WF]    = { OMM_FS_COURSE_TEXTURES(COURSE_WF)    },
    [COURSE_JRB]   = { OMM_FS_COURSE_TEXTURES(COURSE_JRB)   },
    [COURSE_CCM]   = { OMM_FS_COURSE_TEXTURES(COURSE_CCM)   },
    [COURSE_BBH]   = { OMM_FS_COURSE_TEXTURES(COURSE_BBH)   },
    [COURSE_HMC]   = { OMM_FS_COURSE_TEXTURES(COURSE_HMC)   },
    [COURSE_LLL]   = { OMM_FS_COURSE_TEXTURES(COURSE_LLL)   },
    [COURSE_SSL]   = { OMM_FS_COURSE_TEXTURES(COURSE_SSL)   },
    [COURSE_DDD]   = { OMM_FS_COURSE_TEXTURES(COURSE_DDD)   },
    [COURSE_SL]    = { OMM_FS_COURSE_TEXTURES(COURSE_SL)    },
    [COURSE_WDW]   = { OMM_FS_COURSE_TEXTURES(COURSE_WDW)   },
    [COURSE_TTM]   = { OMM_FS_COURSE_TEXTURES(COURSE_TTM)   },
    [COURSE_THI]   = { OMM_FS_COURSE_TEXTURES(COURSE_THI)   },
    [COURSE_TTC]   = { OMM_FS_COURSE_TEXTURES(COURSE_TTC)   },
    [COURSE_RR]    = { OMM_FS_COURSE_TEXTURES(COURSE_RR)    },
    [COURSE_BITDW] = { OMM_FS_COURSE_TEXTURES(COURSE_BITDW) },
    [COURSE_BITFS] = { OMM_FS_COURSE_TEXTURES(COURSE_BITFS) },
    [COURSE_BITS]  = { OMM_FS_COURSE_TEXTURES(COURSE_BITS)  },
    [COURSE_PSS]   = { OMM_FS_COURSE_TEXTURES(COURSE_PSS)   },
    [COURSE_COTMC] = { OMM_FS_COURSE_TEXTURES(COURSE_COTMC) },
    [COURSE_TOTWC] = { OMM_FS_COURSE_TEXTURES(COURSE_TOTWC) },
    [COURSE_VCUTM] = { OMM_FS_COURSE_TEXTURES(COURSE_VCUTM) },
    [COURSE_WMOTR] = { OMM_FS_COURSE_TEXTURES(COURSE_WMOTR) },
    [COURSE_SA]    = { OMM_FS_COURSE_TEXTURES(COURSE_SA)    },
};

static const char *sOmmFileSelectStrings[][4] = {
    [OMM_MM_PLAY]  = { OMM_TEXT_FS_PLAY  },
    [OMM_MM_COPY]  = { OMM_TEXT_FS_COPY  },
    [OMM_MM_ERASE] = { OMM_TEXT_FS_ERASE },
    [OMM_MM_SCORE] = { OMM_TEXT_FS_SCORE },
};

static const char *sOmmFileSelectBackgrounds[][2] = {
                     /* ------- Save file --------    ------------ Sparkly star ------------- */
    [OMM_MM_PLAY]  = { OMM_ASSET_MENU_SELECT_BUTTON, NULL                                      },
    [OMM_MM_COPY]  = { OMM_ASSET_MENU_COPY_BUTTON,   NULL                                      },
    [OMM_MM_ERASE] = { OMM_ASSET_MENU_ERASE_BUTTON,  OMM_ASSET_MENU_ERASE_BUTTON               },
    [OMM_MM_SCORE] = { OMM_ASSET_MENU_SCORE_BUTTON,  OMM_ASSET_MENU_SCORE_SPARKLY_STARS_BUTTON },
};

static const u8 sOmmFileSelectButtonColors[NUM_SAVE_FILES][3][3] = {
    /* ----- Locked -----    --- Save file ----    -- Sparkly star -- */
    { { 0x80, 0x80, 0x80 }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xE0, 0x80 } },
    { { 0x80, 0x80, 0x80 }, { 0xFF, 0xFF, 0xFF }, { 0xC0, 0xE0, 0xFF } },
    { { 0x80, 0x80, 0x80 }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x40, 0x80 } },
    { { 0x80, 0x80, 0x80 }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF } },
};

static const char *sOmmSparklyModes[] = {
    OMM_TEXT_UNKNOWN,
    OMM_TEXT_SPARKLY_1,
    OMM_TEXT_SPARKLY_2,
    OMM_TEXT_SPARKLY_3,
    OMM_TEXT_SPARKLY_ALL,
};

static const char *sOmmSparklyScoreMessages[][2] = {
    { OMM_TEXT_SPARKLY_1_SCORE_MESSAGE_1, OMM_TEXT_SPARKLY_1_SCORE_MESSAGE_2 },
    { OMM_TEXT_SPARKLY_2_SCORE_MESSAGE_1, OMM_TEXT_SPARKLY_2_SCORE_MESSAGE_2 },
    { OMM_TEXT_SPARKLY_3_SCORE_MESSAGE_1, OMM_TEXT_SPARKLY_3_SCORE_MESSAGE_2 },
};

typedef struct { u32 flag; const void *texFull; const void *texEmpty; } ScoreBoardFlag;
static const ScoreBoardFlag sOmmScoreBoardFlags[] = {
    { SAVE_FLAG_HAVE_WING_CAP, OMM_TEXTURE_HUD_ICON_CAP_W, OMM_TEXTURE_HUD_ICON_CAP_W_EMPTY },
    { SAVE_FLAG_HAVE_METAL_CAP, OMM_TEXTURE_HUD_ICON_CAP_M, OMM_TEXTURE_HUD_ICON_CAP_M_EMPTY },
    { SAVE_FLAG_HAVE_VANISH_CAP, OMM_TEXTURE_HUD_ICON_CAP_V, OMM_TEXTURE_HUD_ICON_CAP_V_EMPTY },
    { SAVE_FLAG_UNLOCKED_BASEMENT_DOOR | SAVE_FLAG_HAVE_KEY_1, OMM_TEXTURE_HUD_ICON_KEY, OMM_TEXTURE_HUD_ICON_KEY_EMPTY },
    { SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR | SAVE_FLAG_HAVE_KEY_2, OMM_TEXTURE_HUD_ICON_KEY, OMM_TEXTURE_HUD_ICON_KEY_EMPTY },
    { 0, NULL, NULL },
};

static const char *sOmmFileSelectScoreSparklyTimerGlyphs[] = {
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

//
// File Select update
//

static s32 omm_file_select_next_mode(s32 mode) {
    for (s32 nextMode = mode + 1;; ++nextMode) {

        // Reached max
        if (nextMode == OMM_FS_MODE_COUNT) {
            nextMode = -1;
            continue;
        }

        // Not available
        if (!OMM_FS_MODE_IS_AVAILABLE(nextMode)) {
            continue;
        }

        // Sparkly stars
        if (nextMode == OMM_FS_MODE_SPARKLY_STARS && !OMM_REWARD_IS_SPARKLY_STARS_UNLOCKED) {
            continue;
        }

        // OK
        return nextMode;
    }
}

static s32 omm_file_select_update() {
    if (sOmmFileSelect->timer++ < 15) return 0;

    // Update inputs
    u32 inputs = omm_get_inputs();

    // Score board
    if (sOmmFileScore->open) {
        sOmmFileScore->timer++;
        sOmmFileScore->open = (sOmmFileScore->timer != 0);
        if (sOmmFileScore->timer < 10) return 0;

        // Switch between stars/coins score
        if (OMM_FS_MODE_IS_SAVE_MODE && (inputs & A_BUTTON)) {
            play_sound(OMM_FS_SOUND_SCORE_COINS, gGlobalSoundArgs);
            sOmmFileScore->coins = !sOmmFileScore->coins;
        }

        // Close score board
        else if (inputs & (A_BUTTON | B_BUTTON)) {
            play_sound(OMM_FS_SOUND_SCORE_CLOSE, gGlobalSoundArgs);
            sOmmFileScore->timer = -10;
        }
    }

    // Next save file (vertically)
    else if (inputs & (STICK_UP | STICK_DOWN)) {
        play_sound(OMM_FS_SOUND_SCROLL, gGlobalSoundArgs);
        sOmmFileSelect->index = (sOmmFileSelect->index + 2) % 4;
        sOmmFileSelect->timer = 40;
    }

    // Next save file (horizontally)
    else if (inputs & (STICK_LEFT | STICK_RIGHT)) {
        play_sound(OMM_FS_SOUND_SCROLL, gGlobalSoundArgs);
        sOmmFileSelect->index = ((sOmmFileSelect->index + 1) % 2) + 2 * (sOmmFileSelect->index / 2);
        sOmmFileSelect->timer = 40;
    }

    // Change mode
    else if (inputs & L_TRIG) {
        s32 prevMode = sOmmFileSelect->mode;
        sOmmFileSelect->mode = omm_file_select_next_mode(sOmmFileSelect->mode);
        if (prevMode != sOmmFileSelect->mode) play_sound(OMM_FS_SOUND_CHANGE_MODE, gGlobalSoundArgs);
    }

    // Return to main menu or cancel copy
    else if (inputs & B_BUTTON) {
        play_sound(OMM_FS_SOUND_RETURN, gGlobalSoundArgs);
        if (sOmmFileCopy->open) {
            sOmmFileCopy->open = false;
        } else {
            return -1;
        }
    }

    // Advance (A)
    else if (inputs & A_BUTTON) {
        switch (sOmmMainMenu->index) {

            // Start a new save or resume
            case OMM_MM_PLAY: {
                play_sound(OMM_FS_SOUND_PLAY, gGlobalSoundArgs);
                omm_check_complete_save(sOmmFileSelect->index, sOmmFileSelect->mode);
                omm_select_save_file(sOmmFileSelect->index, sOmmFileSelect->mode, COURSE_NONE, false);
                return sOmmFileSelect->index + 1;
            } break;

            // Copy an existing file
            case OMM_MM_COPY: {

                // Enter copy mode
                if (!sOmmFileCopy->open && omm_save_file_exists(sOmmFileSelect->index, sOmmFileSelect->mode)) {
                    play_sound(OMM_FS_SOUND_SCROLL, gGlobalSoundArgs);
                    sOmmFileCopy->index = sOmmFileSelect->index;
                    sOmmFileCopy->open = true;
                }

                // Do the file copy
                else if (sOmmFileCopy->open && sOmmFileCopy->index != sOmmFileSelect->index && !omm_save_file_exists(sOmmFileSelect->index, sOmmFileSelect->mode)) {
                    play_sound(OMM_FS_SOUND_COPY, gGlobalSoundArgs);
                    omm_save_file_copy(sOmmFileCopy->index, sOmmFileSelect->mode, sOmmFileSelect->index);
                    sOmmFileCopy->open = false;
                }

                // Invalid
                else {
                    play_sound(OMM_FS_SOUND_INVALID, gGlobalSoundArgs);
                }
            } break;

            // Erase an existing file
            case OMM_MM_ERASE: {

                // Erase a save file
                if (OMM_FS_MODE_IS_SAVE_MODE && omm_save_file_exists(sOmmFileSelect->index, sOmmFileSelect->mode)) {
                    play_sound(OMM_FS_SOUND_ERASE, gGlobalSoundArgs);
                    omm_save_file_erase(sOmmFileSelect->index, sOmmFileSelect->mode);
                }

                // Clear Sparkly stars mode
                else if (OMM_FS_MODE_IS_SPARKLY_STARS && omm_sparkly_is_timer_started(sOmmFileSelect->index + 1)) {
                    play_sound(OMM_FS_SOUND_ERASE, gGlobalSoundArgs);
                    omm_sparkly_clear_mode(sOmmFileSelect->index + 1);
                }

                // Invalid
                else {
                    play_sound(OMM_FS_SOUND_INVALID, gGlobalSoundArgs);
                }
            } break;

            // Open score board
            case OMM_MM_SCORE: {

                // Save file or Sparkly stars
                if ((OMM_FS_MODE_IS_SAVE_MODE && omm_save_file_exists(sOmmFileSelect->index, sOmmFileSelect->mode)) ||
                    (OMM_FS_MODE_IS_SPARKLY_STARS && omm_sparkly_is_unlocked(sOmmFileSelect->index + 1))) {
                    play_sound(OMM_FS_SOUND_SCORE_OPEN, gGlobalSoundArgs);
                    sOmmFileScore->open = true;
                    sOmmFileScore->timer = 0;
                    sOmmFileScore->coins = false;
                }

                // Invalid
                else {
                    play_sound(OMM_FS_SOUND_INVALID, gGlobalSoundArgs);
                }
            } break;
        }
    }

    // Advance (Start)
    else if (inputs & START_BUTTON) {
        switch (sOmmMainMenu->index) {

            // Start a new save but skip intro or resume to last course
            case OMM_MM_PLAY: {
                play_sound(OMM_FS_SOUND_PLAY, gGlobalSoundArgs);
                omm_check_complete_save(sOmmFileSelect->index, sOmmFileSelect->mode);
                omm_select_save_file(sOmmFileSelect->index, sOmmFileSelect->mode, omm_save_file_get_last_course_num(sOmmFileSelect->index, sOmmFileSelect->mode), true);
                return sOmmFileSelect->index + 1;
            } break;
        }
    }
    return 0;
}

//
// File Select render
//

static void omm_file_select_render_background(const void *texture, u8 intensity) {
    Vtx *vtx = omm_alloc_vtx(4);
    vtx[0] = (Vtx) {{{ GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0),              0, 0 }, 0, { 0x000, 0x400 }, { intensity, intensity, intensity, 0xFF }}};
    vtx[1] = (Vtx) {{{ GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0),             0, 0 }, 0, { 0x400, 0x400 }, { intensity, intensity, intensity, 0xFF }}};
    vtx[2] = (Vtx) {{{ GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0), SCREEN_HEIGHT, 0 }, 0, { 0x400, 0x000 }, { intensity, intensity, intensity, 0xFF }}};
    vtx[3] = (Vtx) {{{ GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0),  SCREEN_HEIGHT, 0 }, 0, { 0x000, 0x000 }, { intensity, intensity, intensity, 0xFF }}};

    omm_render_create_dl_ortho_matrix();
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);

    s32 SW = (s32) (GFX_DIMENSIONS_SCREEN_WIDTH);
    s32 X0 = (s32) (GFX_DIMENSIONS_FROM_LEFT_EDGE(0));
    omm_render_texrect(X0 + OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER, SCREEN_HEIGHT - (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER), SW - (OMM_FS_BACKGROUND_MARGIN * 2 + OMM_FS_BACKGROUND_BORDER), OMM_FS_BACKGROUND_BORDER, 32, 32, 0x00, 0x00, 0x00, 0xA0, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_texrect(X0 + SW - (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER), OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER, OMM_FS_BACKGROUND_BORDER, SCREEN_HEIGHT - 2 * (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER), 32, 32, 0x00, 0x00, 0x00, 0xA0, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_texrect(X0 + OMM_FS_BACKGROUND_MARGIN, OMM_FS_BACKGROUND_MARGIN, SW - (OMM_FS_BACKGROUND_MARGIN * 2 + OMM_FS_BACKGROUND_BORDER), OMM_FS_BACKGROUND_BORDER, 32, 32, 0xFF, 0xFF, 0xFF, 0x60, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_texrect(X0 + OMM_FS_BACKGROUND_MARGIN, OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER, OMM_FS_BACKGROUND_BORDER, SCREEN_HEIGHT - 2 * (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER), 32, 32, 0xFF, 0xFF, 0xFF, 0x60, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_texrect(X0 + OMM_FS_BACKGROUND_MARGIN, SCREEN_HEIGHT - (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER), OMM_FS_BACKGROUND_BORDER, OMM_FS_BACKGROUND_BORDER, 32, 32, 0x00, 0x00, 0x00, 0xA0, OMM_TEXTURE_MISC_WHITE_UP_RIGHT, false);
    omm_render_texrect(X0 + OMM_FS_BACKGROUND_MARGIN, SCREEN_HEIGHT - (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER), OMM_FS_BACKGROUND_BORDER, OMM_FS_BACKGROUND_BORDER, 32, 32, 0xFF, 0xFF, 0xFF, 0x60, OMM_TEXTURE_MISC_WHITE_DOWN_LEFT, false);
    omm_render_texrect(X0 + SW - (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER), OMM_FS_BACKGROUND_MARGIN, OMM_FS_BACKGROUND_BORDER, OMM_FS_BACKGROUND_BORDER, 32, 32, 0x00, 0x00, 0x00, 0xA0, OMM_TEXTURE_MISC_WHITE_UP_RIGHT, false);
    omm_render_texrect(X0 + SW - (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER), OMM_FS_BACKGROUND_MARGIN, OMM_FS_BACKGROUND_BORDER, OMM_FS_BACKGROUND_BORDER, 32, 32, 0xFF, 0xFF, 0xFF, 0x60, OMM_TEXTURE_MISC_WHITE_DOWN_LEFT, false);
}

static void omm_file_select_render_button(s16 x, s16 y, u8 r, u8 g, u8 b) {
    s16 x0 = x - OMM_FS_FILE_BUTTON_WIDTH / 2;
    s16 y0 = y - OMM_FS_FILE_BUTTON_HEIGHT / 2;
    s16 x1 = x + OMM_FS_FILE_BUTTON_WIDTH / 2;
    s16 y1 = y + OMM_FS_FILE_BUTTON_HEIGHT / 2;

    Vtx *vtx = alloc_display_list(sizeof(Vtx) * 4);
    vtx[0] = (Vtx) { { { x0, y0, 0 }, 0, { 0x000, 0x400 }, { r, g, b, 0xFF } } };
    vtx[1] = (Vtx) { { { x1, y0, 0 }, 0, { 0x400, 0x400 }, { r, g, b, 0xFF } } };
    vtx[2] = (Vtx) { { { x1, y1, 0 }, 0, { 0x400, 0x000 }, { r, g, b, 0xFF } } };
    vtx[3] = (Vtx) { { { x0, y1, 0 }, 0, { 0x000, 0x000 }, { r, g, b, 0xFF } } };

    omm_render_create_dl_ortho_matrix();
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(gDisplayListHead++, OMM_ASSET_MENU_FILE, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);

    omm_render_texrect(x0 + OMM_FS_FILE_BUTTON_BORDER, y1 - OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_WIDTH - OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, 32, 32, 0xFF, 0xFF, 0xFF, 0x80, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_texrect(x1 - OMM_FS_FILE_BUTTON_BORDER, y0 + OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_HEIGHT - OMM_FS_FILE_BUTTON_BORDER * 2, 32, 32, 0xFF, 0xFF, 0xFF, 0x80, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_texrect(x0, y0, OMM_FS_FILE_BUTTON_WIDTH - OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, 32, 32, 0x00, 0x00, 0x00, 0x80, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_texrect(x0, y0 + OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_HEIGHT - OMM_FS_FILE_BUTTON_BORDER * 2, 32, 32, 0x00, 0x00, 0x00, 0x80, OMM_TEXTURE_MISC_WHITE, false);
    omm_render_texrect(x0, y1 - OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, 32, 32, 0xFF, 0xFF, 0xFF, 0x80, OMM_TEXTURE_MISC_WHITE_UP_RIGHT, false);
    omm_render_texrect(x0, y1 - OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, 32, 32, 0x00, 0x00, 0x00, 0x80, OMM_TEXTURE_MISC_WHITE_DOWN_LEFT, false);
    omm_render_texrect(x1 - OMM_FS_FILE_BUTTON_BORDER, y0, OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, 32, 32, 0xFF, 0xFF, 0xFF, 0x80, OMM_TEXTURE_MISC_WHITE_UP_RIGHT, false);
    omm_render_texrect(x1 - OMM_FS_FILE_BUTTON_BORDER, y0, OMM_FS_FILE_BUTTON_BORDER, OMM_FS_FILE_BUTTON_BORDER, 32, 32, 0x00, 0x00, 0x00, 0x80, OMM_TEXTURE_MISC_WHITE_DOWN_LEFT, false);
}

static void omm_file_select_render_icon(s16 x, s16 y, s16 w, s16 h, s16 texw, s16 texh, u8 r, u8 g, u8 b, u8 a, const void *texture) {
    Vtx *vtx = alloc_display_list(sizeof(Vtx) * 4);
    vtx[0] = (Vtx) { { { x - w / 2, y - h / 2, 0 }, 0, {               0, (texh - 1) << 5 }, { r, g, b, a } } };
    vtx[1] = (Vtx) { { { x + w / 2, y - h / 2, 0 }, 0, { (texw - 1) << 5, (texh - 1) << 5 }, { r, g, b, a } } };
    vtx[2] = (Vtx) { { { x + w / 2, y + h / 2, 0 }, 0, { (texw - 1) << 5,               0 }, { r, g, b, a } } };
    vtx[3] = (Vtx) { { { x - w / 2, y + h / 2, 0 }, 0, {               0,               0 }, { r, g, b, a } } };

    omm_render_create_dl_ortho_matrix();
    OMM_RENDER_ENABLE_ALPHA(gDisplayListHead++);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineLERP(gDisplayListHead++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, texw, texh, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
}

static s32 omm_render_score_board_course_data(s32 fileIndex, s32 modeIndex, f32 scale, s32 sl, s32 sr, s32 x0, s32 y0, u8 alpha, const u8 *courseName, s32 courseNum, s32 lineIndex, s32 maxWidth, bool leftAlign, bool isCoinScore, bool highlightCurrentCourse) {
    u8 starCourseFlags = omm_stars_get_course_flags(courseNum, modeIndex);
    if (!starCourseFlags) return 0;
    s32 levelKnown = (courseNum == COURSE_NONE || omm_save_file_get_star_flags(fileIndex, modeIndex, courseNum - 1) != 0);

    // Highlight
    if (highlightCurrentCourse && courseNum != COURSE_NONE && gCurrCourseNum == courseNum) {
        s32 k = OMM_NUM_STARS_MAX_PER_COURSE;
        s32 hlX = lerp_f(scale, x0, (leftAlign ? OMM_FS_SCORE_COURSE_LEVEL_X_LEFT : OMM_FS_SCORE_COURSE_LEVEL_X_RIGHT) - 2);
        s32 hlY = lerp_f(scale, y0, OMM_FS_SCORE_COURSE_LINE_Y - 2);
        s32 hlW = lerp_f(scale,  0, (leftAlign ? OMM_FS_SCORE_COURSE_STARS_X_LEFT - OMM_FS_SCORE_COURSE_LEVEL_X_LEFT : OMM_FS_SCORE_COURSE_STARS_X_RIGHT - OMM_FS_SCORE_COURSE_LEVEL_X_RIGHT) + 2);
        s32 hlH = lerp_f(scale,  0, OMM_FS_SCORE_COURSE_FONT_SIZE + 4);
        s32 hlA = 0x20 + ((coss(gGlobalTimer * 0x800) + 1) * 0x20);
        omm_render_texrect(hlX, hlY, hlW, hlH, 32, 32, 0xFF, 0xFF, 0x40, (hlA * alpha) / 0xFF, OMM_TEXTURE_MISC_WHITE, false);
    }

    // Course
    const u8 *levelStr = (isCoinScore && courseNum == COURSE_NONE ? omm_text_convert(OMM_TEXT_PAUSE_CAPTURES, false) : (levelKnown ? courseName : omm_text_convert(OMM_TEXT_UNKNOWN, false)));
    s32 levelX = lerp_f(scale, x0, (leftAlign ? OMM_FS_SCORE_COURSE_LEVEL_X_LEFT : OMM_FS_SCORE_COURSE_LEVEL_X_RIGHT));
    s32 levelY = lerp_f(scale, y0, OMM_FS_SCORE_COURSE_LINE_Y);
    s32 levelW = lerp_f(scale,  0, OMM_FS_SCORE_COURSE_FONT_SIZE);
    s32 levelC = (levelKnown ? 0xFF : 0x80);
    omm_render_string_sized(levelX, levelY, levelW, levelW, levelC, levelC, levelC, alpha, levelStr, false);

    // Stars
    s32 numStars = 0;
    u8 starSaveFlags = omm_save_file_get_star_flags(fileIndex, modeIndex, courseNum - 1);
    for (s32 starIndex = 0, k = 0; starIndex != OMM_NUM_STARS_MAX_PER_COURSE; ++starIndex) {
        if (starCourseFlags & (1 << starIndex)) {
            bool collected = (starSaveFlags & (1 << starIndex)) != 0;
            numStars += collected;
#if OMM_GAME_IS_SMSR
            const BehaviorScript *bhv = NULL;
            if (omm_stars_get_star_data(omm_level_from_course(courseNum), 1, starIndex, &bhv, NULL) && bhv == bhvCustomSMSRStarReplica &&
                omm_save_file_get_total_star_count(fileIndex, modeIndex) < 121) {
                continue;
            }
#endif
            if (!isCoinScore) {
                s32 starX = lerp_f(scale, x0, (leftAlign ? OMM_FS_SCORE_COURSE_STARS_X_LEFT : OMM_FS_SCORE_COURSE_STARS_X_RIGHT) - 1);
                s32 starY = lerp_f(scale, y0, OMM_FS_SCORE_COURSE_LINE_Y - 1);
                s32 starW = lerp_f(scale,  0, OMM_FS_SCORE_COURSE_FONT_SIZE + 2);
                f32 starC = (collected ? 1.f : 0.25f);
                const void *starT = omm_render_get_star_glyph(clamp_s(courseNum, 0, 16), modeIndex, OMM_EXTRAS_COLORED_STARS, collected);
                const u8 *starRGB = omm_render_get_star_rgb(modeIndex, OMM_EXTRAS_COLORED_STARS, collected);
                omm_render_texrect(starX, starY, starW, starW, 128, 128, starC * starRGB[0], starC * starRGB[1], starC * starRGB[2], alpha, starT, false);
                k++;
            }
        }
    }

    // Score
    if (isCoinScore) {
        s32 scoreX = lerp_f(scale, x0, (leftAlign ? OMM_FS_SCORE_COURSE_COINS_X_LEFT : OMM_FS_SCORE_COURSE_COINS_X_RIGHT));
        s32 scoreY = lerp_f(scale, y0, OMM_FS_SCORE_COURSE_LINE_Y);
        s32 scoreW = lerp_f(scale,  0, OMM_FS_SCORE_COURSE_FONT_SIZE);

        // Coins
        if (courseNum != COURSE_NONE) {
            s32 coins = omm_save_file_get_course_coin_score(fileIndex, modeIndex, courseNum - 1);
            u8 coinsStr[] = { 0xF9, 0xFB, DIALOG_CHAR_SPACE, (coins >= 100 ? (coins / 100) % 10 : 0xE0), (coins >= 10 ? (coins / 10) % 10 : 0xE0), coins % 10, 0xFF };
            omm_render_string_sized(scoreX, scoreY, scoreW, scoreW, 0xFF, 0xFF, 0xFF, alpha, coinsStr, false);
        }

        // Captures
        else {
            str_t countBuf;
            str_fmt(countBuf, sizeof(countBuf), "%d / %d", omm_save_file_get_capture_count(fileIndex, modeIndex), omm_level_get_all_available_captures_count(modeIndex));
            const u8 *countStr = omm_text_convert(countBuf, false);
            omm_render_string_sized(scoreX, scoreY, scoreW, scoreW, 0xFF, 0xFF, 0xFF, alpha, countStr, false);
        }
    }

    // Collected stars
    return numStars;
}

static void omm_render_score_board_flags_data(s32 fileIndex, s32 modeIndex, f32 scale, s32 sl, s32 sr, s32 x0, s32 y0, u8 alpha, const u8 *courseName, s32 lineIndex, s32 maxWidth, const ScoreBoardFlag *flags) {
    s32 levelX = lerp_f(scale, x0, OMM_FS_SCORE_COURSE_LEVEL_X_RIGHT);
    s32 levelY = lerp_f(scale, y0, OMM_FS_SCORE_COURSE_LINE_Y);
    s32 levelW = lerp_f(scale,  0, OMM_FS_SCORE_COURSE_FONT_SIZE);
    omm_render_string_sized(levelX, levelY, levelW, levelW, 0xFF, 0xFF, 0xFF, alpha, courseName, false);
    for (s32 k = 0; flags[k].flag; ++k) {
        s32 hasFlag = (omm_save_file_get_flags(fileIndex, modeIndex) & flags[k].flag) != 0;
        s32 flagX = lerp_f(scale, x0, OMM_FS_SCORE_COURSE_STARS_X_RIGHT - 1);
        s32 flagY = lerp_f(scale, y0, OMM_FS_SCORE_COURSE_LINE_Y - 1);
        s32 flagW = lerp_f(scale,  0, OMM_FS_SCORE_COURSE_FONT_SIZE + 2);
        s32 flagC = (hasFlag ? 0xFF : 0x40);
        const void *flagT = (hasFlag ? flags[k].texFull : flags[k].texEmpty);
        omm_render_texrect(flagX, flagY, flagW, flagW, 128, 128, flagC, flagC, flagC, alpha, flagT, false);
    }
}

OMM_INLINE bool is_level_excluded(s32 levelNum) {
    for_each_in_(s32, levelNumExcluded, { OMM_LEVEL_EXCLUDE_LIST }) {
        if (levelNum == *levelNumExcluded) {
            return true;
        }
    }
    return false;
}

#define score_board_row(panel_, row_, courseNum_, levelNum_, name_) { \
    s32 index_ = -1; \
    if (row_ != -1) { \
        index_ = row_; \
    } else if (!is_level_excluded(levelNum_)) { \
        index_ = levels[panel_].count; \
    } \
    if (index_ != -1) { \
        levels[panel_].count = index_ + 1; \
        levels[panel_].levels[index_].valid = true; \
        levels[panel_].levels[index_].num = courseNum_; \
        levels[panel_].levels[index_].width = 0; \
        if (name_) { \
            omm_text_copy(levels[panel_].levels[index_].name, sizeof(ustr_t), name_); \
        } else { \
            omm_level_get_course_name(levels[panel_].levels[index_].name, levelNum_, modeIndex, false, false); \
        } \
    } \
}

#define score_board_level(panel_, row_, courseNum_, levelNum_) \
    score_board_row(panel_, row_, courseNum_, levelNum_, NULL)

#define score_board_name(panel_, row_, courseNum_, name_) \
    score_board_row(panel_, row_, courseNum_, -1, name_)

void omm_render_score_board(s32 fileIndex, s32 modeIndex, f32 scale, u8 a, bool isCoinScore, bool highlightCurrentCourse) {
    s32 sl = min_s(0, GFX_DIMENSIONS_FROM_LEFT_EDGE(0) / 4);
    s32 sr = SCREEN_WIDTH + max_s(0, (GFX_DIMENSIONS_FROM_RIGHT_EDGE(0) - SCREEN_WIDTH) / 4);
    s32 x0 = SCREEN_WIDTH / 2;
    s32 y0 = SCREEN_HEIGHT / 2;
    s32 alpha = a * scale;

    // Sparkly stars
    if (modeIndex == OMM_FS_MODE_SPARKLY_STARS) {
        s32 sparklyMode = fileIndex + 1;

        // If timer is not started yet, display a message
        if (!omm_sparkly_is_timer_started(sparklyMode)) {
            u8 r = OMM_SPARKLY_HUD_COLOR_R[sparklyMode];
            u8 g = OMM_SPARKLY_HUD_COLOR_G[sparklyMode];
            u8 b = OMM_SPARKLY_HUD_COLOR_B[sparklyMode];
            s32 numLines = array_length(*sOmmSparklyScoreMessages);
            s32 messageY = y0 - 4 + 8 * (numLines - 1);
            for (s32 i = 0; i != numLines; ++i) {
                const u8 *line = omm_text_convert(sOmmSparklyScoreMessages[fileIndex][i], false);
                s32 lineX = x0 - omm_render_get_string_width_sized(line, 8 * scale) / 2;
                s32 lineY = lerp_f(scale, y0, messageY - 16 * i);
                s32 lineW = lerp_f(scale,  0, 8);
                omm_render_string_sized(lineX, lineY, lineW, lineW, r, g, b, alpha, line, false);
            }
            return;
        }

        s32 allStars = (omm_sparkly_get_collected_count(sparklyMode) == omm_sparkly_get_num_stars(sparklyMode));
        s32 timer = omm_sparkly_get_timer(sparklyMode);

        // Collected count
        s32 starIconX = lerp_f(scale, x0, sl + 10);
        s32 starIconY = lerp_f(scale, y0, SCREEN_HEIGHT - 36);
        s32 starIconW = lerp_f(scale,  0, 16);
        s32 numStarsX = lerp_f(scale, x0, sl + 30);
        s32 numStarsY = lerp_f(scale, y0, SCREEN_HEIGHT - 36);
        s32 numStarsW = lerp_f(scale,  0, 16);
        s32 numStarsS = lerp_f(scale,  0, 12);
        s32 sepGlyphX = lerp_f(scale, x0, sl + 56);
        s32 sepGlyphY = lerp_f(scale, y0, SCREEN_HEIGHT - 36);
        s32 sepGlyphW = lerp_f(scale,  0, 12);
        s32 maxStarsX = lerp_f(scale, x0, sl + 66);
        s32 maxStarsY = lerp_f(scale, y0, SCREEN_HEIGHT - 36);
        s32 maxStarsW = lerp_f(scale,  0, 12);
        s32 maxStarsS = lerp_f(scale,  0, 9);
        omm_render_glyph(starIconX, starIconY, starIconW, starIconW, 0xFF, 0xFF, 0xFF, alpha, OMM_SPARKLY_HUD_GLYPH[sparklyMode], false);
        omm_render_number(numStarsX, numStarsY, numStarsW, numStarsW, numStarsS, alpha, omm_sparkly_get_collected_count(sparklyMode), 2, true, false);
        omm_render_glyph(sepGlyphX, sepGlyphY, sepGlyphW, sepGlyphW, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_SLASH, false);
        omm_render_number(maxStarsX, maxStarsY, maxStarsW, maxStarsW, maxStarsS, alpha, omm_sparkly_get_num_stars(sparklyMode), 2, true, false);

        // Elapsed time
        s32 timerGlyphs[] = {
            ((timer / 1080000) % 10), ((timer / 108000) % 10), 10,
            ((timer /   18000) %  6), ((timer /   1800) % 10), 10,
            ((timer /     300) %  6), ((timer /     30) % 10),
        };
        for (s32 i = 0; i != 8; ++i) {
            s32 glyphW = lerp_f(scale,  1, 14);
            s32 glyphX = lerp_f(scale, x0, sr - (20 + (7 - i) * OMM_RENDER_SPARKLY_TIMER_OFFSET_X(14)));
            s32 glyphY = lerp_f(scale, y0, SCREEN_HEIGHT - 36);
            omm_render_glyph(glyphX, glyphY, glyphW, glyphW, 0xFF, 0xFF, 0xFF, alpha, sOmmFileSelectScoreSparklyTimerGlyphs[timerGlyphs[i]], false);
        }

        // List of stars
        // Needs some preprocessing: compute the length of each column to perfectly balance the display
        struct { struct { u8 name[0x100]; s32 width; s32 index; } levels[15]; s32 count; s32 maxWidth; } sparklyLevels[2] = {0};
        for (s32 i = 0, n = omm_sparkly_get_num_stars(sparklyMode); i < n; ++i) {
            ustr_t levelName;
            if (omm_sparkly_get_level_name(levelName, sparklyMode, i)) {
                s32 courseNum = omm_level_get_course(gOmmSparklyData[sparklyMode][i].levelNum);
                s32 mainCourse = COURSE_IS_MAIN_COURSE(courseNum);
                s32 nameLength = omm_text_length(levelName);
                s32 nameWidth = omm_render_get_string_width(levelName);
                mem_cpy(sparklyLevels[mainCourse].levels[sparklyLevels[mainCourse].count].name, levelName, nameLength + 1);
                sparklyLevels[mainCourse].levels[sparklyLevels[mainCourse].count].width = nameWidth;
                sparklyLevels[mainCourse].levels[sparklyLevels[mainCourse].count].index = i;
                sparklyLevels[mainCourse].maxWidth = max_s(sparklyLevels[mainCourse].maxWidth, nameWidth);
                sparklyLevels[mainCourse].count++;
            }
        }
        u8 r = OMM_SPARKLY_HUD_COLOR_R[sparklyMode];
        u8 g = OMM_SPARKLY_HUD_COLOR_G[sparklyMode];
        u8 b = OMM_SPARKLY_HUD_COLOR_B[sparklyMode];
        for (s32 i = 0; i != 2; ++i)
        for (s32 j = 0; j != sparklyLevels[i].count; ++j) {
            s32 levelIndex = sparklyLevels[i].levels[j].index;
            s32 levelLast = (levelIndex == omm_sparkly_get_bowser_4_index(sparklyMode));
            const u8 *levelStr = (levelLast && !omm_sparkly_is_bowser_4_unlocked(sparklyMode) ? omm_text_convert(OMM_TEXT_UNKNOWN, false) : sparklyLevels[i].levels[j].name);
            s32 levelX = lerp_f(scale, x0, (i ? OMM_FS_SCORE_SPARKLY_LEVEL_X_LEFT : OMM_FS_SCORE_SPARKLY_LEVEL_X_RIGHT));
            s32 levelY = lerp_f(scale, y0, OMM_FS_SCORE_SPARKLY_LINE_Y);
            s32 levelW = lerp_f(scale,  0, 8);
            s32 levelD = ((levelLast ? allStars : omm_sparkly_is_star_collected(sparklyMode, levelIndex)) ? 1 : 3);
            omm_render_string_sized(levelX, levelY, levelW, levelW, r / levelD, g / levelD, b / levelD, alpha, levelStr, false);
        }
    }

    // Save file
    else {
        mem_new1(const u8 *, OMM_TEXT_LEVEL_SECRET_STARS_STR, omm_text_convert(OMM_TEXT_LEVEL_SECRET_STARS, true));
        mem_new1(const u8 *, OMM_TEXT_FS_CAPS_AND_KEYS_STR, omm_text_convert(OMM_TEXT_FS_CAPS_AND_KEYS, true));
        struct { struct { ustr_t name; s32 width; s32 num; bool valid; } levels[15]; s32 count; s32 maxWidth; } levels[2] = {0};
        score_board_level(0, -1, COURSE_BOB, LEVEL_BOB);
        score_board_level(0, -1, COURSE_WF, LEVEL_WF);
        score_board_level(0, -1, COURSE_JRB, LEVEL_JRB);
        score_board_level(0, -1, COURSE_CCM, LEVEL_CCM);
        score_board_level(0, -1, COURSE_BBH, LEVEL_BBH);
        score_board_level(0, -1, COURSE_HMC, LEVEL_HMC);
        score_board_level(0, -1, COURSE_LLL, LEVEL_LLL);
        score_board_level(0, -1, COURSE_SSL, LEVEL_SSL);
        score_board_level(0, -1, COURSE_DDD, LEVEL_DDD);
        score_board_level(0, -1, COURSE_SL, LEVEL_SL);
        score_board_level(0, -1, COURSE_WDW, LEVEL_WDW);
        score_board_level(0, -1, COURSE_TTM, LEVEL_TTM);
        score_board_level(0, -1, COURSE_THI, LEVEL_THI);
        score_board_level(0, -1, COURSE_TTC, LEVEL_TTC);
        score_board_level(0, -1, COURSE_RR, LEVEL_RR);
        score_board_level(1, -1, COURSE_BITDW, LEVEL_BITDW);
        score_board_level(1, -1, COURSE_BITFS, LEVEL_BITFS);
        score_board_level(1, -1, COURSE_BITS, LEVEL_BITS);
        score_board_level(1, -1, COURSE_TOTWC, LEVEL_TOTWC);
        score_board_level(1, -1, COURSE_COTMC, LEVEL_COTMC);
        score_board_level(1, -1, COURSE_VCUTM, LEVEL_VCUTM);
        score_board_level(1, -1, COURSE_PSS, LEVEL_PSS);
        score_board_level(1, -1, COURSE_SA, LEVEL_SA);
        score_board_level(1, -1, COURSE_WMOTR, LEVEL_WMOTR);
#if OMM_GAME_IS_R96X
        score_board_name(1, 9, COURSE_CAKE_END, OMM_TEXT_LEVEL_SECRET_STARS_STR);
#else
        score_board_level(1, 9, COURSE_CAKE_END, LEVEL_ENDING);
#endif
        score_board_name(1, 10, COURSE_NONE, OMM_TEXT_LEVEL_SECRET_STARS_STR);
        score_board_name(1, 11, COURSE_NONE, OMM_TEXT_FS_CAPS_AND_KEYS_STR);

        // Compute width and maxWidth
        for (s32 i = 0; i != 2; ++i)
        for (s32 j = 0; j != levels[i].count; ++j) {
            if (levels[i].levels[j].valid) {
                levels[i].levels[j].width = omm_render_get_string_width(levels[i].levels[j].name);
                levels[i].maxWidth = max_s(levels[i].maxWidth, levels[i].levels[j].width);
            }
        }

        // Main courses
        s32 numStars = 0;
        for (s32 i = 0; i != 15; ++i) {
            if (levels[0].levels[i].valid) {
                numStars += omm_render_score_board_course_data(
                    fileIndex, modeIndex,
                    scale, sl, sr, x0, y0, alpha,
                    levels[0].levels[i].name,
                    levels[0].levels[i].num, i,
                    levels[0].maxWidth, true,
                    isCoinScore, highlightCurrentCourse
                );
            }
        }

        // Bowser, Cap, Bonus levels and Castle
        for (s32 i = 0; i != 11; ++i) {
            if (levels[1].levels[i].valid) {
                numStars += omm_render_score_board_course_data(
                    fileIndex, modeIndex,
                    scale, sl, sr, x0, y0, alpha,
                    levels[1].levels[i].name,
                    levels[1].levels[i].num, i + (i / 3),
                    levels[1].maxWidth, false,
                    isCoinScore, highlightCurrentCourse
                );
            }
        }

        // Flags (caps and Bowser keys)
        omm_render_score_board_flags_data(
            fileIndex, modeIndex,
            scale, sl, sr, x0, y0, alpha,
            levels[1].levels[11].name, 14,
            levels[1].maxWidth,
            sOmmScoreBoardFlags
        );

        // Collected stars
        s32 starIconX = lerp_f(scale, x0, sl + 10);
        s32 starIconY = lerp_f(scale, y0, SCREEN_HEIGHT - 48);
        s32 starIconW = lerp_f(scale,  0, 16);
        s32 numStarsX = lerp_f(scale, x0, sl + 30);
        s32 numStarsY = lerp_f(scale, y0, SCREEN_HEIGHT - 48);
        s32 numStarsW = lerp_f(scale,  0, 16);
        s32 numStarsS = lerp_f(scale,  0, 12);
        s32 sepGlyphX = lerp_f(scale, x0, sl + 68);
        s32 sepGlyphY = lerp_f(scale, y0, SCREEN_HEIGHT - 48);
        s32 sepGlyphW = lerp_f(scale,  0, 12);
        s32 maxStarsX = lerp_f(scale, x0, sl + 78);
        s32 maxStarsY = lerp_f(scale, y0, SCREEN_HEIGHT - 48);
        s32 maxStarsW = lerp_f(scale,  0, 12);
        s32 maxStarsS = lerp_f(scale,  0, 9);
        const void *starIconTex = omm_render_get_star_glyph(0, modeIndex, OMM_EXTRAS_COLORED_STARS, true);
        const u8 *starIconRGB = omm_render_get_star_rgb(modeIndex, OMM_EXTRAS_COLORED_STARS, true);
        omm_render_glyph(starIconX, starIconY, starIconW, starIconW, starIconRGB[0], starIconRGB[1], starIconRGB[2], alpha, starIconTex, false);
        omm_render_number(numStarsX, numStarsY, numStarsW, numStarsW, numStarsS, alpha, numStars, 3, true, false);
        omm_render_glyph(sepGlyphX, sepGlyphY, sepGlyphW, sepGlyphW, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_SLASH, false);
        omm_render_number(maxStarsX, maxStarsY, maxStarsW, maxStarsW, maxStarsS, alpha, omm_stars_get_total_star_count(modeIndex), 3, true, false);

        // Collected coins
        s32 numCoins  = omm_save_file_get_total_coin_score(fileIndex, modeIndex);
        s32 numCoinsS = lerp_f(scale,  0, 12);
        s32 coinIconX = lerp_f(scale, x0, sr - (42 + 4 * numCoinsS));
        s32 coinIconY = lerp_f(scale, y0, SCREEN_HEIGHT - 48);
        s32 coinIconW = lerp_f(scale,  0, 16);
        s32 numCoinsX = lerp_f(scale, x0, sr - (22 + 4 * numCoinsS));
        s32 numCoinsY = lerp_f(scale, y0, SCREEN_HEIGHT - 48);
        s32 numCoinsW = lerp_f(scale,  0, 16);
        omm_render_glyph(coinIconX, coinIconY, coinIconW, coinIconW, 0xFF, 0xFF, 0xFF, alpha, OMM_TEXTURE_HUD_COIN, false);
        omm_render_number(numCoinsX, numCoinsY, numCoinsW, numCoinsW, numCoinsS, alpha, numCoins, 5, true, false);
    }
}

static void omm_file_select_render_score_board(f32 scale) {

    // Background
    s32 bgw = GFX_DIMENSIONS_SCREEN_WIDTH * scale + 2;
    s32 bgh = SCREEN_HEIGHT * scale;
    s32 bgx = (SCREEN_WIDTH - bgw) / 2 - 1;
    s32 bgy = (SCREEN_HEIGHT - bgh) / 2;
    omm_render_texrect(bgx, bgy, bgw, bgh, 32, 32, 0x00, 0x00, 0x00, 0xF0, OMM_TEXTURE_MISC_WHITE, false);

    // Score board
    omm_render_score_board(sOmmFileSelect->index, sOmmFileSelect->mode, scale, 0xFF, sOmmFileScore->coins, false);
}

static void omm_file_select_render() {
    s32 SW = SCREEN_WIDTH + max_s(0, (GFX_DIMENSIONS_SCREEN_WIDTH - SCREEN_WIDTH) / 3);
    s32 type = (OMM_FS_MODE_IS_SAVE_MODE * 1 + OMM_FS_MODE_IS_SPARKLY_STARS * 2);

    // Background
    omm_file_select_render_background(sOmmFileSelectBackgrounds[sOmmMainMenu->index][type - 1], (sOmmMainMenu->index == OMM_MM_PLAY ? 0xD2 : 0xFF));

    // Title
    const u8 *titleStr = omm_text_convert(sOmmFileSelectStrings[sOmmMainMenu->index][sOmmFileSelect->mode], false);
    omm_render_string_hud_centered(OMM_FS_TITLE_Y, 0xFF, 0xFF, 0xFF, 0xFF, titleStr, false);

    // Press L to change the current window
    s32 nextMode = omm_file_select_next_mode(sOmmFileSelect->mode);
    if (nextMode != sOmmFileSelect->mode) {
        const u8 *lStr = omm_text_convert(OMM_TEXT_FS_L, false);
        const u8 *lArrowStr = omm_text_convert(OMM_TEXT_FS_ARROW, false);
        const u8 *lModeStr = omm_text_convert(sOmmFileSelectStrings[sOmmMainMenu->index][nextMode], false);
        omm_render_string(OMM_FS_L_X +  1, OMM_FS_L_Y, 0xFF, 0xFF, 0xFF, 0xFF, lStr, true);
        omm_render_string(OMM_FS_L_X +  2, OMM_FS_L_Y, 0xFF, 0xFF, 0xFF, 0xFF, lStr, true);
        omm_render_string(OMM_FS_L_X + 10, OMM_FS_L_Y, 0xFF, 0xFF, 0xFF, 0xFF, lArrowStr, true);
        omm_render_string(OMM_FS_L_X + 20, OMM_FS_L_Y, 0xFF, 0xFF, 0xFF, 0xFF, lModeStr, true);
    }

    // Files
    for (s32 i = 0; i != 4; ++i) {
        bool unlocked = (
            (OMM_FS_MODE_IS_SAVE_MODE) ||
            (OMM_FS_MODE_IS_SPARKLY_STARS && ((i == 3) || omm_sparkly_is_unlocked(i + 1)))
        );
        s32 w = (SW - 2 * (OMM_FS_BACKGROUND_MARGIN + OMM_FS_BACKGROUND_BORDER)) / 2;
        s32 x = (SCREEN_WIDTH / 2) + (w + OMM_FS_FILE_BUTTON_MARGIN_IN) * (i % 2) + OMM_FS_FILE_BUTTON_MARGIN_OUT * ((i + 1) % 2) + (OMM_FS_FILE_BUTTON_WIDTH / 2) - w;
        s32 y = ((SCREEN_HEIGHT - OMM_FS_TITLE_H) / 2) + ((i / 2) == 0 ? +1 : -1) * (OMM_FS_FILE_BUTTON_HEIGHT / 2 + OMM_FS_FILE_BUTTON_MARGIN_OUT);

        // Selection box (copy)
        if (sOmmFileCopy->open && sOmmFileCopy->index == i) {
            omm_render_texrect(OMM_FS_BOX_X, OMM_FS_BOX_Y, OMM_FS_BOX_W, OMM_FS_BOX_H, 32, 32, 0xFF, 0xFF, 0x00, 0x80, OMM_TEXTURE_MISC_WHITE, false);
        }

        // Selection box
        if (sOmmFileSelect->index == i) {
            omm_render_texrect(OMM_FS_BOX_X, OMM_FS_BOX_Y, OMM_FS_BOX_W, OMM_FS_BOX_H, 32, 32, 0x00, 0xFF, 0xFF, OMM_FS_BOX_A, OMM_TEXTURE_MISC_WHITE, false);
        }

        // Generic button
        const u8 *colors = sOmmFileSelectButtonColors[i][unlocked * type];
        omm_file_select_render_button(x, y, colors[0], colors[1], colors[2]);

        // Sparkly stars
        if (OMM_FS_MODE_IS_SPARKLY_STARS) {

            // Sparkly mode
            s32 sparklyMode = i + 1;
            u8 *fileStr = omm_text_convert(sOmmSparklyModes[unlocked * sparklyMode], false);
            omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 9, y + 10, 0x00, 0x00, 0x00, 0xFF, fileStr, false);
            omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 7, y + 10, 0x00, 0x00, 0x00, 0xFF, fileStr, false);
            omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 8, y + 11, 0x00, 0x00, 0x00, 0xFF, fileStr, false);
            omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 8, y +  9, 0x00, 0x00, 0x00, 0xFF, fileStr, false);
            omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 8, y + 10, 0xFF, 0xFF, 0xFF, 0xFF, fileStr, false);

            // Display num stars and timer if unlocked
            if (unlocked && (i == 3 || omm_sparkly_is_timer_started(sparklyMode))) {

                // Num stars
                s32 numStars = (i == 3 ?
                    omm_sparkly_get_collected_count(OMM_SPARKLY_MODE_NORMAL) +
                    omm_sparkly_get_collected_count(OMM_SPARKLY_MODE_HARD) +
                    omm_sparkly_get_collected_count(OMM_SPARKLY_MODE_LUNATIC) :
                    omm_sparkly_get_collected_count(sparklyMode)
                );
                str_t numStarsBuf;
                str_fmt(numStarsBuf, sizeof(numStarsBuf), "%d", numStars);
                u8 *numStarsStr = omm_text_convert(numStarsBuf, false);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 +  8, y - 4, 0xFF, 0xFF, 0xFF, 0xFF, array_of(u8) { 0xFA, 0xFF }, true);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 18, y - 4, 0xFF, 0xFF, 0xFF, 0xFF, array_of(u8) { 0xFB, 0xFF }, true);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 28, y - 4, 0xFF, 0xFF, 0xFF, 0xFF, numStarsStr, true);

                // Timer
                s32 timer = (i == 3 ?
                    omm_sparkly_get_timer(OMM_SPARKLY_MODE_NORMAL) +
                    omm_sparkly_get_timer(OMM_SPARKLY_MODE_HARD) +
                    omm_sparkly_get_timer(OMM_SPARKLY_MODE_LUNATIC) :
                    omm_sparkly_get_timer(sparklyMode)
                );
                str_t timerBuf;
                str_fmt(timerBuf, sizeof(timerBuf), "%02d:%02d:%02d", (timer / 108000), (timer / 1800) % 60, (timer / 30) % 60);
                u8 *timerStr = omm_text_convert(timerBuf, false);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 8, y - 18, 0xFF, 0xFF, 0xFF, 0xFF, timerStr, true);
            }

            // Star icon
            if (i == 3) {
                static const f32 SPARKLY_STARS_ALL_DELTA_X[3] = { -0.50f, +0.50f,  0.00f };
                static const f32 SPARKLY_STARS_ALL_DELTA_Y[3] = { -0.30f, -0.30f, +0.45f };
                s32 s = (OMM_FS_FILE_BUTTON_HEIGHT - 2 * OMM_FS_FILE_BUTTON_BORDER) / 2;
                for (s32 m = 0; m != 3; ++m) {
                    s32 k = omm_sparkly_is_completed(m + 1);
                    s32 c = (k ? 0xFF : 0x00);
                    s32 a = (k ? 0xFF : 0x80);
                    omm_file_select_render_icon(x + SPARKLY_STARS_ALL_DELTA_X[m] * s, y + SPARKLY_STARS_ALL_DELTA_Y[m] * s, s, s, 128, 128, c, c, c, a, OMM_SPARKLY_HUD_GLYPH[m + 1]);
                }
            } else {
                s32 k = omm_sparkly_is_completed(sparklyMode);
                s32 c = (unlocked && k ? 0xFF : 0x00);
                s32 a = (unlocked && k ? 0xFF : 0x80);
                s32 s = ((OMM_FS_FILE_BUTTON_HEIGHT - 2 * OMM_FS_FILE_BUTTON_BORDER) * 3) / 4;
                omm_file_select_render_icon(x, y, s, s, 128, 128, c, c, c, a, OMM_SPARKLY_HUD_GLYPH[sparklyMode]);
            }
        } else {

            // Save file
            if (omm_save_file_exists(i, sOmmFileSelect->mode)) {
                s32 numStars = omm_save_file_get_total_star_count(i, sOmmFileSelect->mode);
                s32 numCoins = omm_save_file_get_total_coin_score(i, sOmmFileSelect->mode);
                s32 numCaptures = omm_save_file_get_capture_count(i, sOmmFileSelect->mode);
                s32 maxCaptures = omm_level_get_all_available_captures_count(sOmmFileSelect->mode);
                s32 lastCourseNum = omm_save_file_get_last_course_num(i, sOmmFileSelect->mode);

                // Num stars
                str_t numStarsBuf;
                str_fmt(numStarsBuf, sizeof(numStarsBuf), "%d", numStars);
                u8 *numStarsStr = omm_text_convert(numStarsBuf, false);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 +  8, y + 10, 0xFF, 0xFF, 0xFF, 0xFF, array_of(u8) { 0xFA, 0xFF }, true);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 18, y + 10, 0xFF, 0xFF, 0xFF, 0xFF, array_of(u8) { 0xFB, 0xFF }, true);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 28, y + 10, 0xFF, 0xFF, 0xFF, 0xFF, numStarsStr, true);

                // Num coins
                str_t numCoinsBuf;
                str_fmt(numCoinsBuf, sizeof(numCoinsBuf), "%d", numCoins);
                u8 *numCoinsStr = omm_text_convert(numCoinsBuf, false);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 +  8, y - 4, 0xFF, 0xFF, 0xFF, 0xFF, array_of(u8) { 0xF9, 0xFF }, true);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 18, y - 4, 0xFF, 0xFF, 0xFF, 0xFF, array_of(u8) { 0xFB, 0xFF }, true);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 28, y - 4, 0xFF, 0xFF, 0xFF, 0xFF, numCoinsStr, true);

                // Num captures
                str_t numCapturesBuf;
                str_fmt(numCapturesBuf, sizeof(numCapturesBuf), "%d / %d", numCaptures, maxCaptures);
                u8 *numCapturesStr = omm_text_convert(numCapturesBuf, false);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 +  8, y - 18, 0xFF, 0xFF, 0xFF, 0xFF, array_of(u8) { 0xFC, 0xFF }, true);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 18, y - 18, 0xFF, 0xFF, 0xFF, 0xFF, array_of(u8) { 0xFB, 0xFF }, true);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 28, y - 18, 0xFF, 0xFF, 0xFF, 0xFF, numCapturesStr, true);

                // Last course icon
                s32 wi = (OMM_FS_FILE_BUTTON_WIDTH - 2 * OMM_FS_FILE_BUTTON_BORDER);
                s32 hi = (OMM_FS_FILE_BUTTON_HEIGHT - 2 * OMM_FS_FILE_BUTTON_BORDER);
                omm_file_select_render_icon(x, y, wi, hi, 320, 240, 0xFF, 0xFF, 0xFF, 0xFF, sOmmFileSelectCourses[lastCourseNum][sOmmFileSelect->mode]);

                // Last course
                ustr_t courseStr;
                omm_level_get_course_name(courseStr, omm_level_from_course(lastCourseNum), sOmmFileSelect->mode, false, false);
                omm_render_string_sized(x - OMM_FS_FILE_BUTTON_WIDTH / 2, y - (OMM_FS_FILE_BUTTON_HEIGHT / 2) - 9, 5, 5, 0xFF, 0xFF, 0xFF, 0xFF, courseStr, true);
            } else {

                // Empty icon
                s32 wi = (OMM_FS_FILE_BUTTON_WIDTH - 2 * OMM_FS_FILE_BUTTON_BORDER);
                s32 hi = (OMM_FS_FILE_BUTTON_HEIGHT - 2 * OMM_FS_FILE_BUTTON_BORDER);
                omm_file_select_render_icon(x, y, wi, hi, 64, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_ASSET_MENU_EMPTY_FILE);

                // Empty
                u8 *emptyStr = omm_text_convert(OMM_TEXT_FS_EMPTY, false);
                omm_render_string(x + OMM_FS_FILE_BUTTON_WIDTH / 2 + 8, y - 4, 0x00, 0x00, 0x00, 0x40, emptyStr, false);
            }
        }
    }

    // Score board
    if (sOmmFileScore->open && sOmmFileScore->timer) {
        omm_file_select_render_score_board(sqr_f(relerp_0_1_f(abs_f(sOmmFileScore->timer), 0, 10, 0, 1)));
    }

    // Transition
    if (sOmmFileSelect->timer < 15) {
        omm_render_texrect(
            GFX_DIMENSIONS_FROM_LEFT_EDGE(0), 0, GFX_DIMENSIONS_SCREEN_WIDTH, SCREEN_HEIGHT,
            32, 32, 0xFF, 0xFF, 0xFF, 0xFF * relerp_0_1_f(sOmmFileSelect->timer, 0, 15, 1, 0), OMM_TEXTURE_MISC_WHITE, false
        );
    }
}

//
// Level functions
//

static s32 omm_level_main_menu_init(UNUSED s32 arg, UNUSED s32 unused) {
    mem_zero(sOmmMainMenuSmoke, sizeof(sOmmMainMenuSmoke));
    mem_zero(sOmmMainMenu, sizeof(sOmmMainMenu));
    mem_zero(sOmmFileSelect, sizeof(sOmmFileSelect));
    mem_zero(sOmmFileCopy, sizeof(sOmmFileCopy));
    mem_zero(sOmmFileScore, sizeof(sOmmFileScore));
    mem_zero(gPlayerSpawnInfos, sizeof(*gPlayerSpawnInfos));
    mem_zero(gPlayerCameraState, sizeof(gPlayerCameraState));
    mem_zero(gBodyStates, sizeof(gBodyStates));
    mem_zero(gMarioState, sizeof(*gMarioState));
    gMarioState->controller = gControllers;
    gMarioState->spawnInfo = gPlayerSpawnInfos;
    gMarioState->statusForCamera = gPlayerCameraState;
    gMarioState->marioBodyState = gBodyStates;
    gMarioState->marioBodyState->capState = MARIO_HAS_DEFAULT_CAP_OFF;
    gMarioState->marioBodyState->handState = MARIO_HAND_FISTS;
    gCurrCreditsEntry = NULL;
    sOmmCompleteSaveSequenceIndex = 0;
    gMarioAnimations = &gMarioAnimsData;
    return 0;
}

static s32 omm_level_main_menu_update(UNUSED s32 arg, UNUSED s32 unused) {
    if (sOmmFileSelect->open) {
#if OMM_GAME_IS_R96X
        r96_play_menu_jingle(R96_MENU_FILE_SELECT, 1.0, 1.0, 1500);
#else
        set_background_music(0, OMM_SEQ_FILE_SELECT, 0);
#endif
        sOmmCompleteSaveSequenceIndex = (sOmmMainMenu->index == OMM_MM_PLAY ? omm_update_complete_save_sequence_index() : 0);
        return omm_file_select_update();
    } else {
#if OMM_GAME_IS_R96X
        r96_play_menu_jingle(R96_EVENT_TITLE_SCREEN, 1.0, 1.0, 1500);
#else
        set_background_music(0, OMM_SEQ_MAIN_MENU, 0);
#endif
        sOmmCompleteSaveSequenceIndex = 0;
        return omm_main_menu_update();
    }
    return 0;
}

static s32 omm_level_main_menu_end(UNUSED s32 arg, UNUSED s32 unused) {
    omm_player_select(gOmmCharacter);
    sOmmCompleteSaveSequenceIndex = 0;
    return 0;
}

static Gfx *omm_level_main_menu_render(s32 callContext, UNUSED struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) {
        if (sOmmFileSelect->open) {
            omm_file_select_render();
        } else {
            omm_main_menu_render();
        }
    }
    return NULL;
}

//
// Level geolayout
//

const GeoLayout omm_level_main_menu_geo[] = {
    GEO_NODE_SCREEN_AREA(0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
    GEO_OPEN_NODE(),
        GEO_ZBUFFER(0),
        GEO_OPEN_NODE(),
            GEO_NODE_ORTHO(100),
            GEO_OPEN_NODE(),
                GEO_ASM(0, geo_intro_backdrop),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
        GEO_ZBUFFER(1),
        GEO_OPEN_NODE(),
            GEO_CAMERA_FRUSTUM(45, 100, 25000),
            GEO_OPEN_NODE(),
                GEO_CAMERA(0, 0, 0, 1000, 0, 0, 0, 0),
                GEO_OPEN_NODE(),
                    GEO_RENDER_OBJ(),
                GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
        GEO_ZBUFFER(0),
        GEO_OPEN_NODE(),
            GEO_ASM(0, omm_level_main_menu_render),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Level scripts
//

static const LevelScript omm_level_main_menu_reset[] = {
    STOP_MUSIC(190),
    TRANSITION(WARP_TRANSITION_FADE_INTO_COLOR, 15, 0x00, 0x00, 0x00),
    SLEEP(15),
    CALL(0, omm_level_main_menu_end),
    SLEEP(15),
    CLEAR_LEVEL(),
    JUMP(level_script_file_select),
};

const LevelScript level_script_file_select[] = {
    CALL(0, omm_level_main_menu_init),
    INIT_LEVEL(),
    ALLOC_LEVEL_POOL(),
    AREA(1, omm_level_main_menu_geo),
        OBJECT(0, 0, 0, 0, 0, 0, 0, 0, bhvOmmMainMenuMario),
        OBJECT(0, 0, 0, 0, 0, 0, 0, 0, bhvOmmMainMenuCappy),
    END_AREA(),
    FREE_LEVEL_POOL(),
    LOAD_AREA(1),
    TRANSITION(WARP_TRANSITION_FADE_FROM_COLOR, 15, 0xFF, 0xFF, 0xFF),
    CALL_LOOP(0, omm_level_main_menu_update),
    JUMP_IF(OP_EQ, -1, omm_level_main_menu_reset),
    GET_OR_SET(OP_SET, VAR_CURR_SAVE_FILE_NUM),
    STOP_MUSIC(190),
    TRANSITION(WARP_TRANSITION_FADE_INTO_COLOR, 15, 0xFF, 0xFF, 0xFF),
    SLEEP(15),
    CALL(0, omm_level_main_menu_end),
    SLEEP(15),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(1),
    SET_REG(OMM_LEVEL_ENTRY_POINT),
    EXIT_AND_EXECUTE(0x15, 0, 0, level_main_scripts_entry),
};
