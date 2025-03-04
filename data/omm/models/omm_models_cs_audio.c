#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/system/omm_audio_character_sound.h"
#include "data/omm/models/omm_models_cs_char_sound.h"

#define STB_VORBIS_HEADER_ONLY
#include "data/omm/engine/miniaudio/stb_vorbis.c"

#if !OMM_GAME_IS_R96X
#define MINIAUDIO_IMPLEMENTATION
#endif
#include "data/omm/engine/miniaudio/miniaudio.h"

#define CHAR_SOUND_DEFAULT (CHAR_SOUND_MAX + 1)
#define SOUND_PRIO(x) (((x) & SOUNDARGS_MASK_PRIORITY) >> SOUNDARGS_SHIFT_PRIORITY)

typedef struct {
    s32 char_sound;
    s32 variant;
    u8 priority;
} CsCharSound;

#define CS_CHAR_SOUND(character_sound, char_sound, variant, priority) \
    [character_sound] = { char_sound, variant, SOUND_PRIO(priority) }

static const CsCharSound OMM_CHARACTER_SOUND_TO_CS_CHAR_SOUND[] = {
    CS_CHAR_SOUND(CHARACTER_SOUND_YAH, CHAR_SOUND_YAH_WAH_HOO, 0, SOUND_MARIO_YAH_WAH_HOO),
    CS_CHAR_SOUND(CHARACTER_SOUND_WAH, CHAR_SOUND_YAH_WAH_HOO, 1, SOUND_MARIO_YAH_WAH_HOO),
    CS_CHAR_SOUND(CHARACTER_SOUND_HOO, CHAR_SOUND_YAH_WAH_HOO, 2, SOUND_MARIO_YAH_WAH_HOO),
    CS_CHAR_SOUND(CHARACTER_SOUND_PUNCH_YAH, CHAR_SOUND_PUNCH_YAH, 0, SOUND_MARIO_PUNCH_YAH),
    CS_CHAR_SOUND(CHARACTER_SOUND_PUNCH_WAH, CHAR_SOUND_PUNCH_WAH, 0, SOUND_MARIO_PUNCH_WAH),
    CS_CHAR_SOUND(CHARACTER_SOUND_PUNCH_HOO, CHAR_SOUND_PUNCH_HOO, 0, SOUND_MARIO_PUNCH_HOO),
    CS_CHAR_SOUND(CHARACTER_SOUND_HOOHOO, CHAR_SOUND_HOOHOO, 0, SOUND_MARIO_HOOHOO),
    CS_CHAR_SOUND(CHARACTER_SOUND_YAHOO, CHAR_SOUND_YAHOO, 0, SOUND_MARIO_YAHOO),
    CS_CHAR_SOUND(CHARACTER_SOUND_YAHOO_2, CHAR_SOUND_YAHOO_WAHA_YIPPEE, 0, SOUND_MARIO_YAHOO_WAHA_YIPPEE),
    CS_CHAR_SOUND(CHARACTER_SOUND_YAHOO_3, CHAR_SOUND_YAHOO_WAHA_YIPPEE, 0, SOUND_MARIO_YAHOO_WAHA_YIPPEE),
    CS_CHAR_SOUND(CHARACTER_SOUND_YAHOO_4, CHAR_SOUND_YAHOO_WAHA_YIPPEE, 0, SOUND_MARIO_YAHOO_WAHA_YIPPEE),
    CS_CHAR_SOUND(CHARACTER_SOUND_WAHA, CHAR_SOUND_YAHOO_WAHA_YIPPEE, 1, SOUND_MARIO_YAHOO_WAHA_YIPPEE),
    CS_CHAR_SOUND(CHARACTER_SOUND_YIPPEE, CHAR_SOUND_YAHOO_WAHA_YIPPEE, 2, SOUND_MARIO_YAHOO_WAHA_YIPPEE),
    CS_CHAR_SOUND(CHARACTER_SOUND_HAHA, CHAR_SOUND_HAHA, 0, SOUND_MARIO_HAHA),
    CS_CHAR_SOUND(CHARACTER_SOUND_HAHA_2, CHAR_SOUND_HAHA_2, 0, SOUND_MARIO_HAHA_2),
    CS_CHAR_SOUND(CHARACTER_SOUND_UH, CHAR_SOUND_UH, 0, SOUND_MARIO_UH),
    CS_CHAR_SOUND(CHARACTER_SOUND_UH2, CHAR_SOUND_UH2, 0, SOUND_MARIO_UH2),
    CS_CHAR_SOUND(CHARACTER_SOUND_UH2_2, CHAR_SOUND_UH2_2, 0, SOUND_MARIO_UH2_2),
    CS_CHAR_SOUND(CHARACTER_SOUND_HRMM, CHAR_SOUND_HRMM, 0, SOUND_MARIO_HRMM),
    CS_CHAR_SOUND(CHARACTER_SOUND_THROW_WAH, CHAR_SOUND_WAH2, 0, SOUND_MARIO_WAH2),
    CS_CHAR_SOUND(CHARACTER_SOUND_GROUND_POUND_WAH, CHAR_SOUND_GROUND_POUND_WAH, 0, SOUND_MARIO_GROUND_POUND_WAH),
    CS_CHAR_SOUND(CHARACTER_SOUND_WHOA, CHAR_SOUND_WHOA, 0, SOUND_MARIO_WHOA),
    CS_CHAR_SOUND(CHARACTER_SOUND_EEUH, CHAR_SOUND_EEUH, 0, SOUND_MARIO_EEUH),
    CS_CHAR_SOUND(CHARACTER_SOUND_ATTACKED, CHAR_SOUND_ATTACKED, 0, SOUND_MARIO_ATTACKED),
    CS_CHAR_SOUND(CHARACTER_SOUND_OOOF, CHAR_SOUND_OOOF, 0, SOUND_MARIO_OOOF),
    CS_CHAR_SOUND(CHARACTER_SOUND_OOOF_2, CHAR_SOUND_OOOF2, 0, SOUND_MARIO_OOOF2),
    CS_CHAR_SOUND(CHARACTER_SOUND_DOH, CHAR_SOUND_DOH, 0, SOUND_MARIO_DOH),
    CS_CHAR_SOUND(CHARACTER_SOUND_HERE_WE_GO, CHAR_SOUND_HERE_WE_GO, 0, SOUND_MARIO_HERE_WE_GO),
    CS_CHAR_SOUND(CHARACTER_SOUND_YAWNING, CHAR_SOUND_YAWNING, 0, SOUND_MARIO_YAWNING),
    CS_CHAR_SOUND(CHARACTER_SOUND_SNORING_1, CHAR_SOUND_SNORING1, 0, SOUND_MARIO_SNORING1),
    CS_CHAR_SOUND(CHARACTER_SOUND_SNORING_2, CHAR_SOUND_SNORING2, 0, SOUND_MARIO_SNORING2),
    CS_CHAR_SOUND(CHARACTER_SOUND_SNORING_3, CHAR_SOUND_SNORING3, 0, SOUND_MARIO_SNORING3),
    CS_CHAR_SOUND(CHARACTER_SOUND_PANTING_1, CHAR_SOUND_PANTING, 0, SOUND_MARIO_PANTING),
    CS_CHAR_SOUND(CHARACTER_SOUND_PANTING_2, CHAR_SOUND_PANTING, 1, SOUND_MARIO_PANTING),
    CS_CHAR_SOUND(CHARACTER_SOUND_PANTING_3, CHAR_SOUND_PANTING, 2, SOUND_MARIO_PANTING),
    CS_CHAR_SOUND(CHARACTER_SOUND_PANTING_COLD, CHAR_SOUND_PANTING_COLD, 0, SOUND_MARIO_PANTING_COLD),
    CS_CHAR_SOUND(CHARACTER_SOUND_COUGHING_1, CHAR_SOUND_COUGHING1, 0, SOUND_MARIO_COUGHING1),
    CS_CHAR_SOUND(CHARACTER_SOUND_COUGHING_2, CHAR_SOUND_COUGHING2, 0, SOUND_MARIO_COUGHING2),
    CS_CHAR_SOUND(CHARACTER_SOUND_COUGHING_3, CHAR_SOUND_COUGHING3, 0, SOUND_MARIO_COUGHING3),
    CS_CHAR_SOUND(CHARACTER_SOUND_WAAAOOOW, CHAR_SOUND_WAAAOOOW, 0, SOUND_MARIO_WAAAOOOW),
    CS_CHAR_SOUND(CHARACTER_SOUND_ON_FIRE, CHAR_SOUND_ON_FIRE, 0, SOUND_MARIO_ON_FIRE),
    CS_CHAR_SOUND(CHARACTER_SOUND_DYING, CHAR_SOUND_DYING, 0, SOUND_MARIO_DYING),
    CS_CHAR_SOUND(CHARACTER_SOUND_DROWNING, CHAR_SOUND_DROWNING, 0, SOUND_MARIO_DROWNING),
    CS_CHAR_SOUND(CHARACTER_SOUND_MAMA_MIA, CHAR_SOUND_MAMA_MIA, 0, SOUND_MARIO_MAMA_MIA),
    CS_CHAR_SOUND(CHARACTER_SOUND_TWIRL_BOUNCE, CHAR_SOUND_TWIRL_BOUNCE, 0, SOUND_MARIO_TWIRL_BOUNCE),
    CS_CHAR_SOUND(CHARACTER_SOUND_SO_LONGA_BOWSER, CHAR_SOUND_SO_LONGA_BOWSER, 0, SOUND_MARIO_SO_LONGA_BOWSER),
    CS_CHAR_SOUND(CHARACTER_SOUND_IMA_TIRED, CHAR_SOUND_IMA_TIRED, 0, SOUND_MARIO_IMA_TIRED),
    CS_CHAR_SOUND(CHARACTER_SOUND_STAR_LETS_A_GO, CHAR_SOUND_LETS_A_GO, 0, SOUND_MENU_STAR_SOUND_LETS_A_GO),
    CS_CHAR_SOUND(CHARACTER_SOUND_OKEY_DOKEY, CHAR_SOUND_OKEY_DOKEY, 0, SOUND_MARIO_OKEY_DOKEY),
    CS_CHAR_SOUND(CHARACTER_SOUND_GAME_OVER, CHAR_SOUND_GAME_OVER, 0, SOUND_MARIO_GAME_OVER),
    CS_CHAR_SOUND(CHARACTER_SOUND_HELLO, CHAR_SOUND_HELLO, 0, SOUND_MARIO_HELLO),
    CS_CHAR_SOUND(CHARACTER_SOUND_PRESS_START_TO_PLAY, CHAR_SOUND_PRESS_START_TO_PLAY, 0, SOUND_MARIO_PRESS_START_TO_PLAY),
    CS_CHAR_SOUND(CHARACTER_SOUND_THANK_YOU_PLAYING_MY_GAME, CHAR_SOUND_DEFAULT, 0, SOUND_MENU_STAR_SOUND_OKEY_DOKEY),
    CS_CHAR_SOUND(CHARACTER_SOUND_STAR_OKEY_DOKEY, CHAR_SOUND_OKEY_DOKEY, 0, SOUND_MENU_STAR_SOUND_OKEY_DOKEY),
    CS_CHAR_SOUND(CHARACTER_SOUND_PEACH_MARIO, CHAR_SOUND_DEFAULT, 0, SOUND_PEACH_MARIO),
    CS_CHAR_SOUND(CHARACTER_SOUND_PEACH_POWER_OF_THE_STARS, CHAR_SOUND_DEFAULT, 0, SOUND_PEACH_POWER_OF_THE_STARS),
    CS_CHAR_SOUND(CHARACTER_SOUND_PEACH_THANKS_TO_YOU, CHAR_SOUND_DEFAULT, 0, SOUND_PEACH_THANKS_TO_YOU),
    CS_CHAR_SOUND(CHARACTER_SOUND_PEACH_THANK_YOU_MARIO, CHAR_SOUND_DEFAULT, 0, SOUND_PEACH_THANK_YOU_MARIO),
    CS_CHAR_SOUND(CHARACTER_SOUND_PEACH_SOMETHING_SPECIAL, CHAR_SOUND_DEFAULT, 0, SOUND_PEACH_SOMETHING_SPECIAL),
    CS_CHAR_SOUND(CHARACTER_SOUND_PEACH_BAKE_A_CAKE, CHAR_SOUND_DEFAULT, 0, SOUND_PEACH_BAKE_A_CAKE),
    CS_CHAR_SOUND(CHARACTER_SOUND_PEACH_FOR_MARIO, CHAR_SOUND_DEFAULT, 0, SOUND_PEACH_FOR_MARIO),
    CS_CHAR_SOUND(CHARACTER_SOUND_PEACH_MARIO_2, CHAR_SOUND_DEFAULT, 0, SOUND_PEACH_MARIO2),
    CS_CHAR_SOUND(CHARACTER_SOUND_OMM_DEATH, CHAR_SOUND_MAMA_MIA, OMM_SOUND_EVENT_DEATH << 8, 0xFF00),
    CS_CHAR_SOUND(CHARACTER_SOUND_OMM_DEATH_WATER, CHAR_SOUND_DROWNING, OMM_SOUND_EVENT_DEATH_WATER << 8, 0xFF00),
    CS_CHAR_SOUND(CHARACTER_SOUND_OMM_DEATH_FALL, CHAR_SOUND_WAAAOOOW, OMM_SOUND_EVENT_DEATH_FALL << 8, 0xFF00),
    CS_CHAR_SOUND(CHARACTER_SOUND_OMM_VIBE_JOY, CHAR_SOUND_YAHOO_WAHA_YIPPEE, 2, 0xFF00),
    CS_CHAR_SOUND(CHARACTER_SOUND_OMM_VIBE_RAGE, CHAR_SOUND_HERE_WE_GO, 0, 0xFF00),
    CS_CHAR_SOUND(CHARACTER_SOUND_OMM_VIBE_GLOOM, CHAR_SOUND_MAMA_MIA, 0, 0xFF00),
    CS_CHAR_SOUND(CHARACTER_SOUND_OMM_VIBE_CALM, CHAR_SOUND_OKEY_DOKEY, 0, 0xFF00),
};

typedef struct {
    ma_sound sound;
    ma_decoder decoder;
    void *buffer;
    u32 size;
    s32 character_sound;
    u8 priority;
} CsSound;

static ma_engine sCsAudioEngine;
static OmmHMap_(CsSound *) sCsSounds = omm_hmap_zero;
static CsSound *sCsSoundPlaying = NULL;

static void omm_models_cs_sound_init() {
    OMM_DO_ONCE {
        ma_result result = ma_engine_init(NULL, &sCsAudioEngine);
        if (result != MA_SUCCESS) {
            sys_fatal("omm_models_cs_sound_init: Could not initialize Miniaudio. (error code: %d)", result);
        }
    }
}

void *omm_models_cs_sound_load(const char *pack_folder, const char *sound_name, u64 *loaded_bytes) {
    omm_models_cs_sound_init();

    sys_path_t filename;
    str_fmt(filename, sizeof(filename), "%s/sound/%s", pack_folder, sound_name);

    // Check filename in cache
    u32 hash = str_hash(filename);
    s32 i = omm_hmap_find(sCsSounds, hash);
    if (i != -1) {
        return omm_hmap_get(sCsSounds, CsSound *, i);
    }

    // Open file
    fs_file_t *f = fs_open(filename);
    if (!f) {

        // Try to add known extensions
        u32 len = strlen(filename);
        for_each_in_(const char *, ext, { ".ogg", ".mp3", ".flac", ".aiff" }) {
            str_fmt(filename + len, sizeof(filename) - len, "%s", *ext);
            f = fs_open(filename);
            if (f) break;
        }

        // Don't fatal here, just log the error
        if (!f) {
            filename[len] = 0;
            omm_log_warning("File not found: \"%s\"\n",, filename);
            CsSound *sound = mem_new(CsSound, 1);
            omm_hmap_insert(sCsSounds, hash, sound);
            return sound;
        }
    }

    // Read file
    s64 size = fs_size(f);
    void *buf = mem_new(u8, size);
    if (fs_read(f, buf, size) < size) {
        fs_close(f);
        sys_fatal("omm_models_cs_sound_load: Unable to read file \"%s\".", filename);
    }
    fs_close(f);

    // Decode file
    CsSound *sound = mem_new(CsSound, 1);
    if (ma_decoder_init_memory(buf, size, NULL, &sound->decoder) != MA_SUCCESS) {
        sys_fatal("omm_models_cs_sound_load: Unable to decode file \"%s\".", filename);
    }

    // Load sound
    if (ma_sound_init_from_data_source(
        &sCsAudioEngine, &sound->decoder,
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION,
        NULL, &sound->sound
    ) != MA_SUCCESS) {
        sys_fatal("omm_models_cs_sound_load: Unable to load audio from file \"%s\".", filename);
    }

    // Add sound to cache
    sound->buffer = buf;
    sound->size = size;
    omm_hmap_insert(sCsSounds, hash, sound);
    if (loaded_bytes) *loaded_bytes += size;
    return sound;
}

static void omm_models_cs_sound_update_playing() {
    if (sCsSoundPlaying && (!ma_sound_is_playing(&sCsSoundPlaying->sound) || ma_sound_at_end(&sCsSoundPlaying->sound))) {
        sCsSoundPlaying = NULL;
    }
}

static void omm_models_cs_sound_play_internal(s32 character_sound, const char *pack_folder, const char *sound_name, f32 *pos, u8 priority) {
    if (sound_name && pack_folder) {
        omm_models_cs_sound_update_playing();

        // Don't overwrite playing sounds with higher priority
        // Sounds with equal priority cancel out each other
        if (sCsSoundPlaying && sCsSoundPlaying->priority > priority) {
            return;
        }

        // Stop playing sound
        if (sCsSoundPlaying) {
            ma_sound_stop(&sCsSoundPlaying->sound);
        }

        // Set volume, then play new sound
        CsSound *sound = (CsSound *) omm_models_cs_sound_load(pack_folder, sound_name, NULL);
        if (sound->size) {
            sound->character_sound = character_sound;
            sound->priority = priority;
            f32 volume = max_f(0,
                (configMasterVolume / 127.f) * (configSfxVolume / 127.f) * !gSfxMute * (gOmmCsVoiceVolumeFactor / 100.f) *
                (pos ? clamp_0_1_f(1.f - vec3f_length(pos) / gAcousticReachPerLevel[gCurrLevelNum]) : 1.f)
            );
            ma_sound_set_volume(&sound->sound, volume);
            ma_sound_seek_to_pcm_frame(&sound->sound, 0);
            ma_sound_start(&sound->sound);
            sCsSoundPlaying = sound;
        }
    }
}

static void omm_models_cs_sound_stop_internal(s32 character_sound) {
    omm_models_cs_sound_update_playing();
    if (sCsSoundPlaying && sCsSoundPlaying->character_sound == character_sound) {
        ma_sound_stop(&sCsSoundPlaying->sound);
        sCsSoundPlaying = NULL;
    }
}

static bool omm_models_cs_sound_process(u32 cs_index, s32 character_sound, f32 *pos, u8 priority, bool play) {
    if (cs_index) {
        const CsCharSound *char_sound = &OMM_CHARACTER_SOUND_TO_CS_CHAR_SOUND[character_sound];

        // Mario sound
        if (char_sound->char_sound == CHAR_SOUND_DEFAULT) {
            return false;
        }

        // OMM sound
        if (char_sound->variant >= 0x100) {
            s32 omm_sound_id = (char_sound->variant >> 8);
            if (!play) {
                omm_sound_stop(omm_sound_id);
            } else if (priority) {
                omm_sound_play_with_priority(omm_sound_id, pos, priority);
            } else {
                omm_sound_play(omm_sound_id, pos);
            }
        }

        // CS sound
        if (!play) {
            omm_models_cs_sound_stop_internal(character_sound);
        } else {
            const char *sound_name = omm_models_cs_get_sound_name(cs_index, char_sound->char_sound, char_sound->variant);
            const char *pack_folder = omm_models_cs_get_pack_folder(cs_index);
            if (priority) {
                omm_models_cs_sound_play_internal(character_sound, pack_folder, sound_name, pos, priority);
            } else {
                omm_models_cs_sound_play_internal(character_sound, pack_folder, sound_name, pos, char_sound->priority);
            }
        }
        return true;
    }
    return false;
}

bool omm_models_cs_sound_play(u32 cs_index, s32 character_sound, f32 *pos) {
    return omm_models_cs_sound_process(cs_index, character_sound, pos, 0, true);
}

bool omm_models_cs_sound_play_with_priority(u32 cs_index, s32 character_sound, f32 *pos, u8 priority) {
    return omm_models_cs_sound_process(cs_index, character_sound, pos, priority, true);
}

bool omm_models_cs_sound_stop(u32 cs_index, s32 character_sound) {
    return omm_models_cs_sound_process(cs_index, character_sound, NULL, 0, false);
}
