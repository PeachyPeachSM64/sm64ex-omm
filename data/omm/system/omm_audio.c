#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define SOUND_PRIO(sound)   (((sound) & SOUNDARGS_MASK_PRIORITY) >> SOUNDARGS_SHIFT_PRIORITY)
#define SOUND_FREQ          32000

//
// Audio
//

#define OMM_AUDIO_SEQUENCE_SIZE     (audioFreq / 10)
#define OMM_AUDIO_OVERLAP_SIZE      (audioFreq / 50)
#define OMM_AUDIO_WINDOW_SIZE       (audioFreq / 60)
#define OMM_AUDIO_FLAT_DURATION     (OMM_AUDIO_SEQUENCE_SIZE - 2 * (OMM_AUDIO_OVERLAP_SIZE))
#define OMM_AUDIO_SEQUENCE_SKIP     ((s32) ((OMM_AUDIO_SEQUENCE_SIZE - OMM_AUDIO_OVERLAP_SIZE) * (timeScale)))

// Resampling is done in a temporary buffer, so output and input can be the same buffer
// outputScale > 1 = time up, pitch down, uses linear interpolation
s32 omm_audio_resample(u8 **output, const u8 *input, s32 inputLength, f32 outputScale) {
    const s16 *inputBuffer = (const s16 *) input;
    s32 inputSamples = inputLength / sizeof(s16);
    s32 outputSamples = inputSamples * outputScale;
    s16 *temp = mem_new(s16, outputSamples);
    f32 invOutputScale = 1.f / outputScale;

    // Resampling
    for (s32 i = 0; i != outputSamples; ++i) {
        s32 i0 = clamp_s(i * invOutputScale, 0, inputSamples - 1);
        s32 i1 = clamp_s(i0 + 1, 0, inputSamples - 1);
        f32 fi = (i * invOutputScale) - (f32) i0;
        temp[i] = (inputBuffer[i0] * (1.f - fi)) + (inputBuffer[i1] * fi);
    }

    // Copy temp to output
    mem_del(*output);
    *output = (u8 *) temp;
    return outputSamples * sizeof(s16);
}

// Resampling is done in a temporary buffer, so output and input can be the same buffer
// outputScale > 1 = time up, pitch down, uses nearest neighbor
s32 omm_audio_resample_fast(u8 **output, const u8 *input, s32 inputLength, f32 outputScale) {
    const s16 *inputBuffer = (const s16 *) input;
    s32 inputSamples = inputLength / sizeof(s16);
    s32 outputSamples = inputSamples * outputScale;
    s16 *temp = mem_new(s16, outputSamples);
    f32 invOutputScale = 1.f / outputScale;

    // Resampling
    for (s32 i = 0; i != outputSamples; ++i) {
        temp[i] = inputBuffer[clamp_s(i * invOutputScale, 0, inputSamples - 1)];
    }

    // Copy temp to output
    mem_del(*output);
    *output = (u8 *) temp;
    return outputSamples * sizeof(s16);
}

// Time stretching is done in a temporary buffer, so output and input can be the same buffer
// timeStretch > 1 = time up, pitch stays the same
s32 omm_audio_time_stretch(u8 **output, const u8 *input, s32 inputLength, s32 audioFreq, f32 timeStretch) {
    s32 inputSamples = inputLength / sizeof(s16);
    s32 outputSamples = inputSamples * timeStretch;
    s16 *temp = mem_new(s16, outputSamples);

    // Synchronized OverLap-Add (SOLA) algorithm
    const s16 *inputBuffer = (const s16 *) input;
    const s16 *currOffset = inputBuffer;
    s16 *outputBuffer = temp;
    f32 *overlap = mem_new(f32, OMM_AUDIO_OVERLAP_SIZE);
    f32 timeScale = 1.f / timeStretch;
    for (s32 remaining = inputSamples; remaining > OMM_AUDIO_SEQUENCE_SKIP + OMM_AUDIO_WINDOW_SIZE; remaining -= OMM_AUDIO_SEQUENCE_SKIP) {

        // Buffer overflow failsafe
        if ((uintptr_t) (currOffset + OMM_AUDIO_FLAT_DURATION + OMM_AUDIO_OVERLAP_SIZE) > (uintptr_t) (input + inputLength)) {
            currOffset = ((const s16 *) (input + inputLength)) - (OMM_AUDIO_FLAT_DURATION + OMM_AUDIO_OVERLAP_SIZE);
        }

        // Prepare output
        mem_cpy(outputBuffer, currOffset, OMM_AUDIO_FLAT_DURATION * sizeof(s16));
        const s16 *prevOffset = currOffset + OMM_AUDIO_FLAT_DURATION;
        inputBuffer += OMM_AUDIO_SEQUENCE_SKIP - OMM_AUDIO_OVERLAP_SIZE;
        s32 bestOffset = 0;
        f32 bestCorr = -1e30f;

        // Precalculate overlapping slopes with prevOffset
        for (s32 i = 0; i < OMM_AUDIO_OVERLAP_SIZE; ++i) {
            overlap[i] = (f32) (prevOffset[i] * i * (OMM_AUDIO_OVERLAP_SIZE - i));
        }

        // Find best overlap offset within [0..OMM_AUDIO_WINDOW_SIZE]
        for (s32 i = 0; i < OMM_AUDIO_WINDOW_SIZE; ++i) {
            f32 crossCorr = 0;
            for (s32 j = 0; j < OMM_AUDIO_OVERLAP_SIZE; ++j) {
                crossCorr += (f32) inputBuffer[i + j] * overlap[j];
            }
            if (crossCorr > bestCorr) {
                bestCorr = crossCorr;
                bestOffset = i;
            }
        }
        currOffset = inputBuffer + bestOffset;

        // Do overlapping between previous and current, copy result to outputBuffer
        for (s32 i = 0; i < OMM_AUDIO_OVERLAP_SIZE; ++i) {
            outputBuffer[i + OMM_AUDIO_FLAT_DURATION] = (prevOffset[i] * (OMM_AUDIO_OVERLAP_SIZE - i) + currOffset[i] * i) / OMM_AUDIO_OVERLAP_SIZE;
        }

        // Update offsets
        currOffset += OMM_AUDIO_OVERLAP_SIZE;
        inputBuffer += OMM_AUDIO_OVERLAP_SIZE;
        outputBuffer += OMM_AUDIO_SEQUENCE_SIZE - OMM_AUDIO_OVERLAP_SIZE;
    }

    // Copy temp to output
    mem_del(*output);
    mem_del(overlap);
    *output = (u8 *) temp;
    return outputSamples * sizeof(s16);
}

// Resizing is done in a temporary buffer, so output and input can be the same buffer
s32 omm_audio_resize(u8 **output, const u8 *input, s32 inputLength, s32 desiredLength) {
    u8 *temp = mem_new(u8, desiredLength);
    mem_cpy(temp, input, min_s(inputLength, desiredLength));
    mem_del(*output);
    *output = temp;
    return desiredLength;
}

// Pitch shifting is done in a temporary buffer, so output and input can be the same buffer
// pitchScale > 1 = pitch up, time stays the same
s32 omm_audio_pitch_shift(u8 **output, const u8 *input, s32 inputLength, s32 audioFreq, f32 pitchScale) {
    if (pitchScale > 1.f) {
        s32 tempLength = omm_audio_time_stretch(output, input, inputLength, audioFreq, pitchScale);
        s32 finalLength = omm_audio_resample(output, *output, tempLength, 1.f / pitchScale);
        omm_audio_resize(output, *output, finalLength, inputLength);
    } else if (pitchScale > 0.f && pitchScale < 1.f) {
        s32 tempLength = omm_audio_resample(output, input, inputLength, 1.f / pitchScale);
        s32 finalLength = omm_audio_time_stretch(output, *output, tempLength, audioFreq, pitchScale);
        omm_audio_resize(output, *output, finalLength, inputLength);
    }
    return inputLength;
}

u8 *omm_audio_mix(u8 *output, const u8 *input, s32 length, s32 volume, s32 distance) {
    s32 acrVolume = clamp_s((1 << 7) - ((distance << 7) / gAcousticReachPerLevel[gCurrLevelNum]), 0, 1 << 7);
    s32 mixVolume = clamp_s((volume * acrVolume) >> 7, 0, 1 << 7);
    for (s32 i = 0; i < length; i += sizeof(s16)) {
        *((s16 *) (output + i)) = (s16) ((((s32) *((s16 *) (input + i))) * mixVolume) >> 7);
    }
    return output;
}

//
// Data
//

// input[0] = raw data
// input[1] = joy data
// input[2] = rage data
// input[3] = gloom data
// input[4] = calm data
// output   = computed data
typedef struct {
    u8 *input[5];
    u8 *output;
    s32 length;
    s32 volume;
    u8 priority;
    u8 bank;
} OmmSoundData;

typedef struct {
    OmmSoundData *sound;
    u8 priority;
} OmmSoundPlaying;

static OmmSoundData sOmmSoundData[0x100] = {0};
static OmmSoundPlaying sOmmSoundPlaying[4] = {0};

static SDL_AudioDeviceID omm_sound_get_device(u8 bank) {
    static SDL_AudioDeviceID sSoundDeviceId[4] = {0};
    if (!sSoundDeviceId[bank]) {

        // Init SDL2 Audio
        if (!SDL_WasInit(SDL_INIT_AUDIO)) {
            if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
                sys_fatal("omm_sound_get_device: Could not initialize SDL Audio: %s", SDL_GetError());
            }
        }

        // Open sound device
        SDL_AudioSpec want, have;
        want.freq     = SOUND_FREQ;
        want.format   = AUDIO_S16SYS;
        want.channels = 1;
        want.samples  = 0x200;
        want.callback = NULL;
        want.userdata = NULL;
        sSoundDeviceId[bank] = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
        if (sSoundDeviceId[bank] == 0) {
            sys_fatal("omm_sound_get_device: Could not open sound device: %s", SDL_GetError());
        }
        SDL_PauseAudioDevice(sSoundDeviceId[bank], 0);
    }
    return sSoundDeviceId[bank];
}

//
// Play
//

static void omm_sound_play_internal(OmmSoundData *sound, f32 *pos, u8 priority) {
    if (sound->input[0]) {
        OmmSoundPlaying *playing = sOmmSoundPlaying + sound->bank;

        // Update playing sound
        if (SDL_GetQueuedAudioSize(omm_sound_get_device(sound->bank)) == 0) {
            playing->sound = NULL;
        }

        // Don't overwrite playing sounds with higher priority
        // Sounds with equal priority cancel out each other
        if (playing->sound && playing->priority > priority) {
            return;
        }

        // Set volume, then add sound to queue
        s32 volume = (sound->volume * configMasterVolume * configSfxVolume * !gSfxMute) / (MAX_VOLUME * MAX_VOLUME);
        s32 distance = (s32) (!pos ? 0 : vec3f_length(pos));
        omm_audio_mix(sound->output, sound->input[gOmmPeach->vibeType], sound->length, volume, distance);
        SDL_ClearQueuedAudio(omm_sound_get_device(sound->bank));
        SDL_QueueAudio(omm_sound_get_device(sound->bank), sound->output, sound->length);
        playing->sound = sound;
        playing->priority = priority;
    }
}

void omm_sound_play(s32 id, f32 *pos) {
    if (!omm_sound_play_character_sound_omm(id, pos)) {
        OmmSoundData *sound = sOmmSoundData + id;
        omm_sound_play_internal(sound, pos, sound->priority);
    }
}

void omm_sound_play_with_priority(s32 id, f32 *pos, u8 priority) {
    if (!omm_sound_play_character_sound_omm_with_priority(id, pos, priority)) {
        OmmSoundData *sound = sOmmSoundData + id;
        omm_sound_play_internal(sound, pos, priority);
    }
}

void omm_sound_stop(s32 id) {
    if (omm_sound_is_playing(id)) {
        OmmSoundData *sound = sOmmSoundData + id;
        OmmSoundPlaying *playing = sOmmSoundPlaying + sound->bank;
        SDL_ClearQueuedAudio(omm_sound_get_device(sound->bank));
        playing->sound = NULL;
    }
}

bool omm_sound_is_playing(s32 id) {
    OmmSoundData *sound = sOmmSoundData + id;
    if (sound->input[0]) {
        OmmSoundPlaying *playing = sOmmSoundPlaying + sound->bank;

        // Update playing sound
        if (SDL_GetQueuedAudioSize(omm_sound_get_device(sound->bank)) == 0) {
            playing->sound = NULL;
            return false;
        }

        // Playing
        return playing->sound == sound;
    }
    return false;
}

bool omm_sound_is_bank_playing(u8 bank) {
    return SDL_GetQueuedAudioSize(omm_sound_get_device(bank)) != 0;
}

//
// Load
//

static void omm_sound_load_wav(s32 id, const char *name, u8 bank, s32 vibesPitchShift, s32 volume100, u8 priority) {

    // Open file
    sys_path_t filename;
    str_cat(filename, sizeof(filename), FS_SOUNDDIR "/", name, ".wav");
    fs_file_t *f = fs_open(filename);
    if (!f) {
        sys_fatal("omm_sound_load_wav: Unable to open file \"%s\".", filename);
    }

    // Read file
    s64 size = fs_size(f);
    void *buf = mem_new(u8, size);
    if (fs_read(f, buf, size) < size) {
        fs_close(f);
        sys_fatal("omm_sound_load_wav: Unable to read file \"%s\".", filename);
    }
    fs_close(f);

    // Load wav
    SDL_AudioSpec wav; u8 *data; s32 length;
    if (!SDL_LoadWAV_RW(SDL_RWFromConstMem(buf, size), true, &wav, &data, (u32 *) &length)) {
        sys_fatal("omm_sound_load_wav: Unable to load file \"%s\": %s", filename, SDL_GetError());
    }
    mem_del(buf);

    // Check frequency
    if (wav.freq != SOUND_FREQ) {
        sys_fatal("omm_sound_load_wav: From file \"%s\", audio frequency should be " STRINGIFY(SOUND_FREQ) " Hz, is %d.", filename, wav.freq);
    }

    // Check format
    if (wav.format != AUDIO_S16SYS) {
        sys_fatal("omm_sound_load_wav: From file \"%s\", audio format is not Signed 16-bit PCM.", filename);
    }

    // Check channels
    if (wav.channels != 1) {
        sys_fatal("omm_sound_load_wav: From file \"%s\", audio channel count should be 1, is %d.", filename, wav.channels);
    }

    // Check volume
    if (volume100 < 0 || volume100 > 100) {
        sys_fatal("omm_sound_load_wav: From file \"%s\", audio volume should be between 0 and 100, is %d.", filename, volume100);
    }

    // Set sound data
    OmmSoundData *sound = sOmmSoundData + id;
    sound->input[0]     = data;
    sound->input[1]     = mem_dup(data, length * sizeof(u8));
    sound->input[2]     = mem_dup(data, length * sizeof(u8));
    sound->input[3]     = mem_dup(data, length * sizeof(u8));
    sound->input[4]     = mem_dup(data, length * sizeof(u8));
    sound->output       = mem_dup(data, length * sizeof(u8));
    sound->length       = length;
    sound->volume       = (volume100 * 0x7F) / 100;
    sound->priority     = priority;
    sound->bank         = bank;

    // Voice effects
    if (vibesPitchShift) {

        // Joy: pitch up
        omm_audio_pitch_shift(&sound->input[OMM_PEACH_VIBE_TYPE_JOY], data, length, SOUND_FREQ, OMM_PEACH_VIBE_SOUND_PITCH_MOD_JOY);

        // Rage: demonic voice
        u8 *rageL = mem_dup(data, length * sizeof(u8)); omm_audio_pitch_shift(&rageL, data, length, SOUND_FREQ, OMM_PEACH_VIBE_SOUND_PITCH_MOD_RAGE * 0.98f);
        u8 *rageR = mem_dup(data, length * sizeof(u8)); omm_audio_pitch_shift(&rageR, data, length, SOUND_FREQ, OMM_PEACH_VIBE_SOUND_PITCH_MOD_RAGE * 1.02f);
        for (s32 i = 0; i < length; i += sizeof(s16)) {
            *((s16 *) (sound->input[OMM_PEACH_VIBE_TYPE_RAGE] + i)) = (s16) clamp_s((((s32) *((s16 *) (rageL + i))) + ((s32) *((s16 *) (rageR + i)))) * 0.8f, -0x8000, +0x7FFF);
        }

        // Gloom: slow down
        omm_audio_resample(&sound->input[OMM_PEACH_VIBE_TYPE_GLOOM], data, length, 1.f / OMM_PEACH_VIBE_SOUND_PITCH_MOD_GLOOM);

        // Calm: pitch slightly up
        omm_audio_pitch_shift(&sound->input[OMM_PEACH_VIBE_TYPE_CALM], data, length, SOUND_FREQ, OMM_PEACH_VIBE_SOUND_PITCH_MOD_CALM);
    }
}

void omm_audio_init() {

    // "peach" bank
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_YAH,            "peach/omm_sound_peach_jump_yah",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_YAH_WAH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_WAH,            "peach/omm_sound_peach_jump_wah",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_YAH_WAH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_HOO,            "peach/omm_sound_peach_jump_hoo",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_YAH_WAH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_PUNCH_YAH,           "peach/omm_sound_peach_punch_yah",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_PUNCH_YAH));
    omm_sound_load_wav(OMM_SOUND_PEACH_PUNCH_WAH,           "peach/omm_sound_peach_punch_wah",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_PUNCH_WAH));
    omm_sound_load_wav(OMM_SOUND_PEACH_PUNCH_HOO,           "peach/omm_sound_peach_punch_hoo",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_PUNCH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_DIVE_HOOHOO,         "peach/omm_sound_peach_dive_hoohoo",            0, 1,  65, SOUND_PRIO(SOUND_MARIO_HOOHOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_YAHOO,          "peach/omm_sound_peach_jump_yahoo",             0, 1,  65, SOUND_PRIO(SOUND_MARIO_YAHOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_YAHOO_2,        "peach/omm_sound_peach_jump_yahoo",             0, 1,  65, SOUND_PRIO(SOUND_MARIO_YAHOO_WAHA_YIPPEE));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_WAHA,           "peach/omm_sound_peach_jump_waha",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_YAHOO_WAHA_YIPPEE));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_YIPPEE,         "peach/omm_sound_peach_jump_yippee",            0, 1,  65, SOUND_PRIO(SOUND_MARIO_YAHOO_WAHA_YIPPEE));
    omm_sound_load_wav(OMM_SOUND_PEACH_LAND_HAHA,           "peach/omm_sound_peach_land_haha",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_HAHA));
    omm_sound_load_wav(OMM_SOUND_PEACH_LAND_HAHA_2,         "peach/omm_sound_peach_land_haha",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_HAHA_2));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_MISS_UH,       "peach/omm_sound_peach_ledge_miss_uh",          0, 1,  65, SOUND_PRIO(SOUND_MARIO_UH));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_CLIMB_UH2,     "peach/omm_sound_peach_ledge_climb_uh2",        0, 1,  65, SOUND_PRIO(SOUND_MARIO_UH2));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_CLIMB_UH2_2,   "peach/omm_sound_peach_ledge_climb_uh2",        0, 1,  65, SOUND_PRIO(SOUND_MARIO_UH2_2));
    omm_sound_load_wav(OMM_SOUND_PEACH_GRAB_HRMM,           "peach/omm_sound_peach_grab_hrmm",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_HRMM));
    omm_sound_load_wav(OMM_SOUND_PEACH_THROW_WAH2,          "peach/omm_sound_peach_throw_wah2",             0, 1,  65, SOUND_PRIO(SOUND_MARIO_WAH2));
    omm_sound_load_wav(OMM_SOUND_PEACH_GROUND_POUND_WAH,    "peach/omm_sound_peach_ground_pound_wah",       0, 1,  65, SOUND_PRIO(SOUND_MARIO_GROUND_POUND_WAH));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_GRAB_WHOA,     "peach/omm_sound_peach_ledge_grab_whoa",        0, 1,  65, SOUND_PRIO(SOUND_MARIO_WHOA));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_CLIMB_EEUH,    "peach/omm_sound_peach_ledge_climb_eeuh",       0, 1,  65, SOUND_PRIO(SOUND_MARIO_EEUH));
    omm_sound_load_wav(OMM_SOUND_PEACH_ATTACKED,            "peach/omm_sound_peach_attacked",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_ATTACKED));
    omm_sound_load_wav(OMM_SOUND_PEACH_BONK_OOOF,           "peach/omm_sound_peach_bonk_ooof",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_OOOF));
    omm_sound_load_wav(OMM_SOUND_PEACH_BONK_OOOF_2,         "peach/omm_sound_peach_bonk_ooof",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_OOOF2));
    omm_sound_load_wav(OMM_SOUND_PEACH_BONK_DOH,            "peach/omm_sound_peach_bonk_doh",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_DOH));
    omm_sound_load_wav(OMM_SOUND_PEACH_HERE_WE_GO,          "peach/omm_sound_peach_here_we_go",             0, 1,  65, SOUND_PRIO(SOUND_MARIO_HERE_WE_GO));
    omm_sound_load_wav(OMM_SOUND_PEACH_YAWNING,             "peach/omm_sound_peach_yawning",                0, 1,  65, SOUND_PRIO(SOUND_MARIO_YAWNING));
    omm_sound_load_wav(OMM_SOUND_PEACH_PANTING1,            "peach/omm_sound_peach_panting1",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_PANTING));
    omm_sound_load_wav(OMM_SOUND_PEACH_PANTING2,            "peach/omm_sound_peach_panting2",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_PANTING));
    omm_sound_load_wav(OMM_SOUND_PEACH_PANTING3,            "peach/omm_sound_peach_panting3",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_PANTING));
    omm_sound_load_wav(OMM_SOUND_PEACH_COUGHING1,           "peach/omm_sound_peach_coughing1",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_COUGHING1));
    omm_sound_load_wav(OMM_SOUND_PEACH_COUGHING2,           "peach/omm_sound_peach_coughing2",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_COUGHING2));
    omm_sound_load_wav(OMM_SOUND_PEACH_COUGHING3,           "peach/omm_sound_peach_coughing3",              0, 1,  65, SOUND_PRIO(SOUND_MARIO_COUGHING3));
    omm_sound_load_wav(OMM_SOUND_PEACH_FALLING_WAAAOOOW,    "peach/omm_sound_peach_falling_waaaooow",       0, 1,  65, SOUND_PRIO(SOUND_MARIO_WAAAOOOW));
    omm_sound_load_wav(OMM_SOUND_PEACH_ON_FIRE,             "peach/omm_sound_peach_on_fire",                0, 1,  65, SOUND_PRIO(SOUND_MARIO_ON_FIRE));
    omm_sound_load_wav(OMM_SOUND_PEACH_DYING,               "peach/omm_sound_peach_dying",                  0, 1,  65, SOUND_PRIO(SOUND_MARIO_DYING));
    omm_sound_load_wav(OMM_SOUND_PEACH_DROWNING,            "peach/omm_sound_peach_drowning",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_DROWNING));
    omm_sound_load_wav(OMM_SOUND_PEACH_MAMA_MIA,            "peach/omm_sound_peach_mama_mia",               0, 1,  65, SOUND_PRIO(SOUND_MARIO_MAMA_MIA));
    omm_sound_load_wav(OMM_SOUND_PEACH_TWIRL_BOUNCE,        "peach/omm_sound_peach_twirl_bounce",           0, 1,  65, SOUND_PRIO(SOUND_MARIO_TWIRL_BOUNCE));
    omm_sound_load_wav(OMM_SOUND_PEACH_SO_LONGA_BOWSER,     "peach/omm_sound_peach_so_longa_bowser",        0, 1,  65, SOUND_PRIO(SOUND_MARIO_SO_LONGA_BOWSER));
    omm_sound_load_wav(OMM_SOUND_PEACH_LETS_A_GO,           "peach/omm_sound_peach_lets_a_go",              0, 1,  65, SOUND_PRIO(SOUND_MENU_STAR_SOUND_LETS_A_GO));
    omm_sound_load_wav(OMM_SOUND_PEACH_VIBE_JOY,            "peach/omm_sound_peach_vibe_joy",               0, 0,  65, 0xFF);
    omm_sound_load_wav(OMM_SOUND_PEACH_VIBE_RAGE,           "peach/omm_sound_peach_vibe_rage",              0, 0,  65, 0xFF);
    omm_sound_load_wav(OMM_SOUND_PEACH_VIBE_GLOOM,          "peach/omm_sound_peach_vibe_gloom",             0, 0,  65, 0xFF);
    omm_sound_load_wav(OMM_SOUND_PEACH_VIBE_CALM,           "peach/omm_sound_peach_vibe_calm",              0, 0,  65, 0xFF);

    // "toad" bank
    omm_sound_load_wav(OMM_SOUND_TOAD_1,                    "toad/omm_sound_toad_1",                        1, 0,  75, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_2,                    "toad/omm_sound_toad_2",                        1, 0,  75, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_3,                    "toad/omm_sound_toad_3",                        1, 0,  75, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_4,                    "toad/omm_sound_toad_4",                        1, 0,  75, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_5,                    "toad/omm_sound_toad_5",                        1, 0,  75, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_6,                    "toad/omm_sound_toad_6",                        1, 0,  75, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_7,                    "toad/omm_sound_toad_7",                        1, 0,  75, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_8,                    "toad/omm_sound_toad_8",                        1, 0,  75, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_9,                    "toad/omm_sound_toad_9",                        1, 0,  75, 0xFF);

    // "event" bank
    omm_sound_load_wav(OMM_SOUND_EVENT_CAPTURE,             "event/omm_sound_event_capture",                2, 0,  60, 0x80);
    omm_sound_load_wav(OMM_SOUND_EVENT_UNCAPTURE,           "event/omm_sound_event_uncapture",              2, 0,  60, 0x80);
    omm_sound_load_wav(OMM_SOUND_EVENT_LIFE_UP,             "event/omm_sound_event_life_up",                2, 0,  70, 0xF8);
    omm_sound_load_wav(OMM_SOUND_EVENT_SPARKLY_STAR_GET,    "event/omm_sound_event_sparkly_star_get",       2, 0,  90, 0xFE);
    omm_sound_load_wav(OMM_SOUND_EVENT_SECRET,              "event/omm_sound_event_secret",                 2, 0,  50, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH,               "event/omm_sound_event_death_1",                2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_WATER,         "event/omm_sound_event_death_2",                2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_FALL,          "event/omm_sound_event_death_3",                2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_MARIO,         "event/omm_sound_event_death_mario_1",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_MARIO_WATER,   "event/omm_sound_event_death_mario_2",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_MARIO_FALL,    "event/omm_sound_event_death_mario_3",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_PEACH,         "event/omm_sound_event_death_peach_1",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_PEACH_WATER,   "event/omm_sound_event_death_peach_2",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_PEACH_FALL,    "event/omm_sound_event_death_peach_3",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_LUIGI,         "event/omm_sound_event_death_luigi_1",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_LUIGI_WATER,   "event/omm_sound_event_death_luigi_2",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_LUIGI_FALL,    "event/omm_sound_event_death_luigi_3",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_WARIO,         "event/omm_sound_event_death_wario_1",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_WARIO_WATER,   "event/omm_sound_event_death_wario_2",          2, 0,  60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_WARIO_FALL,    "event/omm_sound_event_death_wario_3",          2, 0,  60, 0xFF);

    // "effect" bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_DAMAGE,             "effect/omm_sound_effect_damage",               3, 0,  50, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EFFECT_HEAL,               "effect/omm_sound_effect_heal",                 3, 0,  40, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EFFECT_FREEZE_1,           "effect/omm_sound_effect_freeze_1",             3, 0, 100, 0x01);
    omm_sound_load_wav(OMM_SOUND_EFFECT_FREEZE_2,           "effect/omm_sound_effect_freeze_2",             3, 0, 100, 0x01);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PROPELLER_1,        "effect/omm_sound_effect_propeller_1",          3, 0,  75, 0x02);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PROPELLER_2,        "effect/omm_sound_effect_propeller_2",          3, 0,  65, 0x02);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PROPELLER_3,        "effect/omm_sound_effect_propeller_3",          3, 0,  70, 0x02);
    omm_sound_load_wav(OMM_SOUND_EFFECT_MIDAIR_SPIN,        "effect/omm_sound_effect_midair_spin",          3, 0,  50, 0x03);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PEACH_GLIDE,        "effect/omm_sound_effect_peach_glide",          3, 0,  35, 0x40);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PEACH_VIBE,         "effect/omm_sound_effect_peach_vibe",           3, 0,  35, 0xF0);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PERRY_CHARGE,       "effect/omm_sound_effect_perry_charge",         1, 0,  50, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_PERRY_CHARGED,      "effect/omm_sound_effect_perry_charged",        1, 0,  50, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_PERRY_BEAM,         "effect/omm_sound_effect_perry_beam",           1, 0,  50, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_SONIC_JUMP_1,       "effect/omm_sound_effect_sonic_jump_1",         1, 0,  65, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_SONIC_JUMP_2,       "effect/omm_sound_effect_sonic_jump_2",         1, 0,  65, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_SONIC_CHARGE,       "effect/omm_sound_effect_sonic_charge",         1, 0,  65, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_SONIC_RELEASE,      "effect/omm_sound_effect_sonic_release",        1, 0,  65, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_SONIC_SPIN,         "effect/omm_sound_effect_sonic_spin",           1, 0,  65, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_YOSHI_TONGUE,       "effect/omm_sound_effect_yoshi_tongue",         1, 0,  35, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_YOSHI_FLUTTER,      "effect/omm_sound_effect_yoshi_flutter",        1, 0,  35, 0xFD); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_YOSHI_FLUTTER_END,  "effect/omm_sound_effect_yoshi_flutter_end",    1, 0,  35, 0xFE); // Use Toad sound bank
}
