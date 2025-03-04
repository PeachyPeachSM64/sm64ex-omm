#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CONFIGFILE_NAME "omm_config.txt"

static const struct ConfigOption OMM_CONFIG_OPTIONS[] = {
    CONFIG_TOGGLE("cs_palette_preset", gOmmCsPalettePreset),
    CONFIG_TOGGLE("cs_animations", gOmmCsAnimations),
    CONFIG_TOGGLE("cs_voices", gOmmCsVoices),
    CONFIG_SCROLL("cs_voice_volume_factor", gOmmCsVoiceVolumeFactor),
    CONFIG_TOGGLE("time_trials_enabled", gOmmTimeTrialsEnabled),
    CONFIG_TOGGLE("time_trials_show_star_ghosts", gOmmTimeTrialsShowStarGhosts),
    CONFIG_TOGGLE("time_trials_show_bowser_ghosts", gOmmTimeTrialsShowBowserGhosts),
    CONFIG_TOGGLE("time_trials_show_slide_ghosts", gOmmTimeTrialsShowSlideGhosts),
    CONFIG_TOGGLE("time_trials_show_coins_ghosts", gOmmTimeTrialsShowCoinsGhosts),
    CONFIG_TOGGLE("time_trials_show_all_stars_ghosts", gOmmTimeTrialsShowAllStarsGhosts),
    CONFIG_KBINDS("omm_key_a", gOmmControlsButtonA),
    CONFIG_KBINDS("omm_key_b", gOmmControlsButtonB),
    CONFIG_KBINDS("omm_key_x", gOmmControlsButtonX),
    CONFIG_KBINDS("omm_key_y", gOmmControlsButtonY),
    CONFIG_KBINDS("omm_key_start", gOmmControlsButtonStart),
    CONFIG_KBINDS("omm_key_spin", gOmmControlsButtonSpin),
    CONFIG_KBINDS("omm_key_l", gOmmControlsTriggerL),
    CONFIG_KBINDS("omm_key_r", gOmmControlsTriggerR),
    CONFIG_KBINDS("omm_key_z", gOmmControlsTriggerZ),
    CONFIG_KBINDS("omm_key_cup", gOmmControlsCUp),
    CONFIG_KBINDS("omm_key_cdown", gOmmControlsCDown),
    CONFIG_KBINDS("omm_key_cleft", gOmmControlsCLeft),
    CONFIG_KBINDS("omm_key_cright", gOmmControlsCRight),
    CONFIG_KBINDS("omm_key_dup", gOmmControlsDUp),
    CONFIG_KBINDS("omm_key_ddown", gOmmControlsDDown),
    CONFIG_KBINDS("omm_key_dleft", gOmmControlsDLeft),
    CONFIG_KBINDS("omm_key_dright", gOmmControlsDRight),
    CONFIG_KBINDS("omm_key_stickup", gOmmControlsStickUp),
    CONFIG_KBINDS("omm_key_stickdown", gOmmControlsStickDown),
    CONFIG_KBINDS("omm_key_stickleft", gOmmControlsStickLeft),
    CONFIG_KBINDS("omm_key_stickright", gOmmControlsStickRight),
    CONFIG_CHOICE("omm_frame_rate", gOmmFrameRate),
#if OMM_CODE_DEBUG
    CONFIG_CHOICE("omm_show_fps", gOmmShowFPS),
#else
    CONFIG_TOGGLE("omm_show_fps", gOmmShowFPS),
#endif
    CONFIG_CHOICE("omm_texture_caching", gOmmTextureCaching),
    CONFIG_TOGGLE("omm_model_pack_caching", gOmmModelPackCaching),
    CONFIG_CHOICE("omm_hud_mode", gOmmHudMode),
    CONFIG_SCROLL("omm_hud_size", gOmmHudSize),
    CONFIG_CHOICE("omm_camera_invert_1st_person", gOmmCameraInvert1stPerson),
    CONFIG_CHOICE("omm_camera_invert_3rd_person", gOmmCameraInvert3rdPerson),
    CONFIG_CHOICE_SC("omm_character", gOmmCharacter),
    CONFIG_CHOICE_SC("omm_moveset_type", gOmmMovesetType),
    CONFIG_CHOICE_SC("omm_cap_type", gOmmCapType),
    CONFIG_CHOICE_SC("omm_stars_mode", gOmmStarsMode),
    CONFIG_CHOICE_SC("omm_power_ups_type", gOmmPowerUpsType),
    CONFIG_CHOICE_SC("omm_camera_mode", gOmmCameraMode),
    CONFIG_CHOICE_SC("omm_sparkly_stars_mode", gOmmSparklyStarsMode),
    CONFIG_CHOICE_SC("omm_sparkly_stars_hint_at_level_entry", gOmmSparklyStarsHintAtLevelEntry),
    CONFIG_CHOICE_SC("omm_sparkly_stars_completion_reward", gOmmSparklyStarsCompletionReward),
    CONFIG_TOGGLE_SC("omm_sparkly_stars_perry_charge", gOmmSparklyStarsPerryCharge),
    CONFIG_CHOICE_SC("omm_extras_mario_colors", gOmmExtrasMarioColors),
    CONFIG_CHOICE_SC("omm_extras_peach_colors", gOmmExtrasPeachColors),
    CONFIG_CHOICE_SC("omm_extras_objects_radar", gOmmExtrasObjectsRadar),
    CONFIG_TOGGLE_SC("omm_extras_smo_animations", gOmmExtrasSMOAnimations),
    CONFIG_TOGGLE_SC("omm_extras_cappy_and_tiara", gOmmExtrasCappyAndTiara),
    CONFIG_TOGGLE_SC("omm_extras_colored_stars", gOmmExtrasColoredStars),
    CONFIG_TOGGLE_SC("omm_extras_reveal_secrets", gOmmExtrasRevealSecrets),
    CONFIG_TOGGLE_SC("omm_extras_show_star_number", gOmmExtrasShowStarNumber),
    CONFIG_TOGGLE_SC("omm_extras_invisible_mode", gOmmExtrasInvisibleMode),
    CONFIG_TOGGLE_SC("omm_extras_mario_mode", gOmmExtrasMarioMode),
#if OMM_CODE_DEBUG
    CONFIG_TOGGLE_SC("omm_debug_hitbox", gOmmDebugHitbox),
    CONFIG_TOGGLE_SC("omm_debug_hurtbox", gOmmDebugHurtbox),
    CONFIG_TOGGLE_SC("omm_debug_wallbox", gOmmDebugWallbox),
    CONFIG_TOGGLE_SC("omm_debug_surface", gOmmDebugSurface),
    CONFIG_TOGGLE_SC("omm_debug_mario", gOmmDebugMario),
    CONFIG_TOGGLE_SC("omm_debug_cappy", gOmmDebugCappy),
#endif
#if OMM_CODE_DEV
#include "data/omm/dev/omm_dev_opt_config.inl"
#endif
};

static OmmArray sOmmUnknownConfigOptions = omm_array_zero;

const char *configfile_name() {
    return OMM_CONFIGFILE_NAME;
}

void configfile_load(const char *filename) {
    omm_log("Loading configuration from \"%s\"\n",, filename);
    omm_array_for_each(sOmmUnknownConfigOptions, p_line) { free(p_line->as_ptr); }
    omm_array_delete(sOmmUnknownConfigOptions);

    // Open file, create one if it doesn't exist
    fs_file_t *file = fs_open(filename);
    if (!file) {
        file = fs_open(OMM_CONFIGFILE_NAME);
        if (!file) {
            file = fs_open(CONFIGFILE_DEFAULT);
            if (!file) {
                omm_printf("Config file \"%s\" not found. Creating it.\n",, filename);
                configfile_save(filename);
                return;
            }
        }
    }

    // Go through each line in the file
    char *line = NULL;
    while ((line = read_file_line(file)) != NULL) {
        s32 length = strlen(line);

        // Strip
        char *p = line;
        while (isspace(*p)) p++;
        if (!*p || *p == '#') continue;

        // Tokenize
        char *tokens[1 + MAX_BINDS];
        s32 numTokens = tokenize_string(p, sizeof(tokens) / sizeof(tokens[0]), tokens);
        if (numTokens >= 2) {
            const struct ConfigOption *option = NULL;

            // Model pack
            if (omm_models_read_config(tokens[0], tokens[1])) {
                free(line);
                continue;
            }

            // SM64 config
            array_for_each_(const struct ConfigOption, option_sm64, options) {
                if (strcmp(tokens[0], option_sm64->name) == 0) {
                    option = option_sm64;
                    break;
                }
            }

            // OMM config
            if (!option) {
                array_for_each_(const struct ConfigOption, option_omm, OMM_CONFIG_OPTIONS) {
                    if (strcmp(tokens[0], option_omm->name) == 0) {
                        option = option_omm;
                        break;
                    }
                }
            }

            // Process option
            if (option) {
                switch (option->type) {
                    case CONFIG_TYPE_BOOL:  { *option->boolValue = (strcmp(tokens[1], "true") == 0); } break;
                    case CONFIG_TYPE_UINT:  { sscanf(tokens[1], "%u", option->uintValue); } break;
                    case CONFIG_TYPE_FLOAT: { sscanf(tokens[1], "%f", option->floatValue); } break;
                    case CONFIG_TYPE_BIND:  { for (s32 i = 0; i != MAX_BINDS && i != numTokens - 1; ++i) sscanf(tokens[i + 1], "%x", option->uintValue + i); } break;
                }
                omm_printf("Option `%s` set to",, tokens[0]);
                for (s32 i = 1; i != numTokens; ++i) omm_printf(" \"%s\"",, tokens[i]);
                omm_printf("\n");
                free(line);
            } else {
                for (s32 i = 0; i != length; ++i) {
                    switch (line[i]) {
                        case '\0': line[i] = ' '; break;
                        case '\r': line[i] = '\0'; break;
                        case '\n': line[i] = '\0'; break;
                    }
                }
                omm_array_add(sOmmUnknownConfigOptions, ptr, line);
            }
        }
    }
    fs_close(file);
}

static void configfile_save_option(FILE *file, const struct ConfigOption *option) {
    if (strstr(option->name, "key_") == option->name) return; // Don't save old key binds
    switch (option->type) {
        case CONFIG_TYPE_BOOL:  { fprintf(file, "%s %s\n", option->name, *option->boolValue ? "true" : "false"); } break;
        case CONFIG_TYPE_UINT:  { fprintf(file, "%s %u\n", option->name, *option->uintValue); } break;
        case CONFIG_TYPE_FLOAT: { fprintf(file, "%s %f\n", option->name, *option->floatValue); } break;
        case CONFIG_TYPE_BIND:  { fprintf(file, "%s", option->name); for (s32 i = 0; i != MAX_BINDS; ++i) fprintf(file, " %04x", option->uintValue[i]); fprintf(file, "\n"); } break;
    }
}

void configfile_save(const char *filename) {
    if (!gOmmGlobals->configNoSave) {
        sys_path_t filepath;
        FILE *file = fopen(fs_get_save_path(filepath, filename), "w");
        if (file) {
            omm_log("Saving configuration to \"%s\"\n",, filename);
            array_for_each_(const struct ConfigOption, option_sm64, options) {
                configfile_save_option(file, option_sm64);
            }
            omm_array_for_each(sOmmUnknownConfigOptions, p_line) {
                const char *line = (const char *) p_line->as_ptr;
                if (strstr(line, "key_") == line) continue; // Don't save old key binds
                fprintf(file, "%s\n", line);
            }
            array_for_each_(const struct ConfigOption, option_omm, OMM_CONFIG_OPTIONS) {
                configfile_save_option(file, option_omm);
            }
            omm_models_write_config(file);
            fclose(file);
        }
    }
}

// Make sure to not use old ones
#define configfile_name configfile_name_unused
#define configfile_load configfile_load_unused
#define configfile_save configfile_save_unused
extern void configfile_load(const char *filename);
extern void configfile_save(const char *filename);
extern const char *configfile_name(void);
