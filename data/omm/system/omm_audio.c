#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define SOUND_ID(sound)     (((sound) >> 16) & 0xFFFF)
#define SOUND_PRIO(sound)   (((sound) >> 8) & 0xFF)
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
    s16 *temp = OMM_MEMNEW(s16, outputSamples);
    f32 invOutputScale = 1.f / outputScale;

    // Resampling
    for (s32 i = 0; i != outputSamples; ++i) {
        s32 i0 = clamp_s(i * invOutputScale, 0, inputSamples - 1);
        s32 i1 = clamp_s(i0 + 1, 0, inputSamples - 1);
        f32 fi = (i * invOutputScale) - (f32) i0;
        temp[i] = (inputBuffer[i0] * (1.f - fi)) + (inputBuffer[i1] * fi);
    }

    // Copy temp to output
    OMM_MEMDEL(*output);
    *output = (u8 *) temp;
    return outputSamples * sizeof(s16);
}

// Resampling is done in a temporary buffer, so output and input can be the same buffer
// outputScale > 1 = time up, pitch down, uses nearest neighbor
s32 omm_audio_resample_fast(u8 **output, const u8 *input, s32 inputLength, f32 outputScale) {
    const s16 *inputBuffer = (const s16 *) input;
    s32 inputSamples = inputLength / sizeof(s16);
    s32 outputSamples = inputSamples * outputScale;
    s16 *temp = OMM_MEMNEW(s16, outputSamples);
    f32 invOutputScale = 1.f / outputScale;

    // Resampling
    for (s32 i = 0; i != outputSamples; ++i) {
        temp[i] = inputBuffer[clamp_s(i * invOutputScale, 0, inputSamples - 1)];
    }

    // Copy temp to output
    OMM_MEMDEL(*output);
    *output = (u8 *) temp;
    return outputSamples * sizeof(s16);
}

// Time stretching is done in a temporary buffer, so output and input can be the same buffer
// timeStretch > 1 = time up, pitch stays the same
s32 omm_audio_time_stretch(u8 **output, const u8 *input, s32 inputLength, s32 audioFreq, f32 timeStretch) {
    s32 inputSamples = inputLength / sizeof(s16);
    s32 outputSamples = inputSamples * timeStretch;
    s16 *temp = OMM_MEMNEW(s16, outputSamples);

    // Synchronized OverLap-Add (SOLA) algorithm
    const s16 *inputBuffer = (const s16 *) input;
    const s16 *currOffset = inputBuffer;
    s16 *outputBuffer = temp;
    f32 *overlap = OMM_MEMNEW(f32, OMM_AUDIO_OVERLAP_SIZE);
    f32 timeScale = 1.f / timeStretch;
    for (s32 remaining = inputSamples; remaining > OMM_AUDIO_SEQUENCE_SKIP + OMM_AUDIO_WINDOW_SIZE; remaining -= OMM_AUDIO_SEQUENCE_SKIP) {

        // Buffer overflow failsafe
        if ((uintptr_t) (currOffset + OMM_AUDIO_FLAT_DURATION + OMM_AUDIO_OVERLAP_SIZE) > (uintptr_t) (input + inputLength)) {
            currOffset = ((const s16 *) (input + inputLength)) - (OMM_AUDIO_FLAT_DURATION + OMM_AUDIO_OVERLAP_SIZE);
        }
        
        // Prepare output
        OMM_MEMCPY(outputBuffer, currOffset, OMM_AUDIO_FLAT_DURATION * sizeof(s16));
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
    OMM_MEMDEL(*output);
    OMM_MEMDEL(overlap);
    *output = (u8 *) temp;
    return outputSamples * sizeof(s16);
}

// Resizing is done in a temporary buffer, so output and input can be the same buffer
s32 omm_audio_resize(u8 **output, const u8 *input, s32 inputLength, s32 desiredLength) {
    u8 *temp = OMM_MEMNEW(u8, desiredLength);
    OMM_MEMCPY(temp, input, min_s(inputLength, desiredLength));
    OMM_MEMDEL(*output);
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
    s32 acrVolume = clamp_s((1 << 7) - ((distance << 7) / sAcousticReachPerLevel[gCurrLevelNum]), 0, 1 << 7);
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

static OmmSoundData  sOmmSoundData[0x100] = { 0 };
static OmmSoundData *sOmmSoundPlaying[4] = { NULL };

static SDL_AudioDeviceID omm_sound_get_device(u8 bank) {
    static SDL_AudioDeviceID sSoundDeviceId[4] = { 0 };
    if (!sSoundDeviceId[bank]) {

        // Init SDL2 Audio
        if (!SDL_WasInit(SDL_INIT_AUDIO)) {
            if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
                sys_fatal("omm_sound_get_device: Could not init SDL Audio.");
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
        if (sSoundDeviceId == 0) {
            sys_fatal("omm_sound_get_device: Could not open sound device.");
        }
        SDL_PauseAudioDevice(sSoundDeviceId[bank], 0);
    }
    return sSoundDeviceId[bank];
}

//
// Play
//

void omm_sound_play(s32 id, f32 *pos) {
    if (!omm_sound_play_character_sound_omm(id, pos)) {
        OmmSoundData *sound = &sOmmSoundData[id];
        if (sound->input[0]) {

            // Update playing sound
            if (SDL_GetQueuedAudioSize(omm_sound_get_device(sound->bank)) == 0) {
                sOmmSoundPlaying[sound->bank] = NULL;
            }

            // Don't overwrite playing sounds with higher priority
            // Sounds with equal priority cancel out each other
            if (sOmmSoundPlaying[sound->bank] != NULL
                && sOmmSoundPlaying[sound->bank]->priority > sound->priority) {
                return;
            }

            s32 volume = (sound->volume * configMasterVolume * configSfxVolume) / (0x7F * 0x7F);
            s32 distance = (s32)(pos == NULL ? 0 : vec3f_length(pos));
            omm_audio_mix(sound->output, sound->input[gOmmData->mario->peach.vibeType], sound->length, volume, distance);
            SDL_ClearQueuedAudio(omm_sound_get_device(sound->bank));
            SDL_QueueAudio(omm_sound_get_device(sound->bank), sound->output, sound->length);
            sOmmSoundPlaying[sound->bank] = sound;
        }
    }
}

void omm_sound_stop(s32 id) {
    if (omm_sound_is_playing(id)) {
        OmmSoundData *sound = &sOmmSoundData[id];
        SDL_ClearQueuedAudio(omm_sound_get_device(sound->bank));
        sOmmSoundPlaying[sound->bank] = NULL;
    }
}

bool omm_sound_is_playing(s32 id) {
    OmmSoundData *sound = &sOmmSoundData[id];
    if (sound->input[0]) {

        // Update playing sound
        if (SDL_GetQueuedAudioSize(omm_sound_get_device(sound->bank)) == 0) {
            sOmmSoundPlaying[sound->bank] = NULL;
            return false;
        }

        // Playing
        return (sOmmSoundPlaying[sound->bank] == sound);
    }
    return false;
}

bool omm_sound_is_bank_playing(u8 bank) {
    return SDL_GetQueuedAudioSize(omm_sound_get_device(bank)) != 0;
}

//
// Load
//

static void omm_sound_load_wav(s32 id, const char *name, u8 bank, s32 vibesPitchShift, s32 volume, u8 priority) {

    // Load wav
    SDL_AudioSpec wav; u8 *data; s32 length;
    OMM_STRING(filename, 256, "%s/%s/%s.wav", OMM_EXE_FOLDER, OMM_SOUND_FOLDER, name);
    if (!SDL_LoadWAV(filename, &wav, &data, (u32 *) &length)) {
        sys_fatal("omm_sound_load_wav: Unable to load file %s.", filename);
    }
    if (wav.freq != SOUND_FREQ) {
        sys_fatal("omm_sound_load_wav: From file %s, audio frequency should be " STRINGIFY(SOUND_FREQ) " Hz, is %d.", filename, wav.freq);
    }
    if (wav.format != AUDIO_S16SYS) {
        sys_fatal("omm_sound_load_wav: From file %s, audio format is not Signed 16-bit PCM.", filename);
    }
    if (wav.channels != 1) {
        sys_fatal("omm_sound_load_wav: From file %s, audio channel count should be 1, is %d.", filename, wav.channels);
    }
    if (volume < 0 || volume > 127) {
        sys_fatal("omm_sound_load_wav: From file %s, audio volume should be between 0 and 127, is %d.", filename, volume);
    }

    // Set sound data
    OmmSoundData *sound = &sOmmSoundData[id];
    sound->input[0]     = data;
    sound->input[1]     = OMM_MEMDUP(data, length * sizeof(u8));
    sound->input[2]     = OMM_MEMDUP(data, length * sizeof(u8));
    sound->input[3]     = OMM_MEMDUP(data, length * sizeof(u8));
    sound->input[4]     = OMM_MEMDUP(data, length * sizeof(u8));
    sound->output       = OMM_MEMDUP(data, length * sizeof(u8));
    sound->length       = length;
    sound->volume       = volume;
    sound->priority     = priority;
    sound->bank         = bank;

    // Voice effects
    if (vibesPitchShift) {

        // Joy: pitch up
        omm_audio_pitch_shift(&sound->input[OMM_PEACH_VIBE_TYPE_JOY], data, length, SOUND_FREQ, OMM_PEACH_VIBE_SOUND_PITCH_MOD_JOY);

        // Rage: demonic voice
        u8 *rageL = OMM_MEMDUP(data, length * sizeof(u8)); omm_audio_pitch_shift(&rageL, data, length, SOUND_FREQ, OMM_PEACH_VIBE_SOUND_PITCH_MOD_RAGE * 0.98f);
        u8 *rageR = OMM_MEMDUP(data, length * sizeof(u8)); omm_audio_pitch_shift(&rageR, data, length, SOUND_FREQ, OMM_PEACH_VIBE_SOUND_PITCH_MOD_RAGE * 1.02f);
        for (s32 i = 0; i < length; i += sizeof(s16)) {
            *((s16 *) (sound->input[OMM_PEACH_VIBE_TYPE_RAGE] + i)) = (s16) clamp_s((((s32) *((s16 *) (rageL + i))) + ((s32) *((s16 *) (rageR + i)))) * 0.8f, -0x8000, +0x7FFF);
        }

        // Gloom: speed down
        omm_audio_resample(&sound->input[OMM_PEACH_VIBE_TYPE_GLOOM], data, length, 1.f / OMM_PEACH_VIBE_SOUND_PITCH_MOD_GLOOM);

        // Calm: pitch slightly up
        omm_audio_pitch_shift(&sound->input[OMM_PEACH_VIBE_TYPE_CALM], data, length, SOUND_FREQ, OMM_PEACH_VIBE_SOUND_PITCH_MOD_CALM);
    }
}

OMM_AT_STARTUP static void omm_audio_init() {

    // "peach" bank
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_YAH,            "peach/omm_sound_peach_jump_yah",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_YAH_WAH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_WAH,            "peach/omm_sound_peach_jump_wah",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_YAH_WAH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_HOO,            "peach/omm_sound_peach_jump_hoo",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_YAH_WAH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_YAHOO,          "peach/omm_sound_peach_jump_yahoo",         0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_YAHOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_WAHA,           "peach/omm_sound_peach_jump_waha",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_YAHOO_WAHA_YIPPEE));
    omm_sound_load_wav(OMM_SOUND_PEACH_JUMP_YIPPEE,         "peach/omm_sound_peach_jump_yippee",        0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_YAHOO_WAHA_YIPPEE));
    omm_sound_load_wav(OMM_SOUND_PEACH_LAND_HAHA,           "peach/omm_sound_peach_land_haha",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_HAHA));
    omm_sound_load_wav(OMM_SOUND_PEACH_PUNCH_YAH,           "peach/omm_sound_peach_punch_yah",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_PUNCH_YAH));
    omm_sound_load_wav(OMM_SOUND_PEACH_PUNCH_WAH,           "peach/omm_sound_peach_punch_wah",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_PUNCH_WAH));
    omm_sound_load_wav(OMM_SOUND_PEACH_PUNCH_HOO,           "peach/omm_sound_peach_punch_hoo",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_PUNCH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_GRAB_HRMM,           "peach/omm_sound_peach_grab_hrmm",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_HRMM));
    omm_sound_load_wav(OMM_SOUND_PEACH_THROW_WAH2,          "peach/omm_sound_peach_throw_wah2",         0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_WAH2));
    omm_sound_load_wav(OMM_SOUND_PEACH_GROUND_POUND_WAH,    "peach/omm_sound_peach_ground_pound_wah",   0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_GROUND_POUND_WAH));
    omm_sound_load_wav(OMM_SOUND_PEACH_DIVE_HOOHOO,         "peach/omm_sound_peach_dive_hoohoo",        0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_HOOHOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_TWIRL_BOUNCE,        "peach/omm_sound_peach_twirl_bounce",       0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_TWIRL_BOUNCE));
    omm_sound_load_wav(OMM_SOUND_PEACH_SO_LONGA_BOWSER,     "peach/omm_sound_peach_so_longa_bowser",    0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_SO_LONGA_BOWSER));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_GRAB_WHOA,     "peach/omm_sound_peach_ledge_grab_whoa",    0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_WHOA));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_CLIMB_EEUH,    "peach/omm_sound_peach_ledge_climb_eeuh",   0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_EEUH));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_CLIMB_UH2,     "peach/omm_sound_peach_ledge_climb_uh2",    0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_UH2));
    omm_sound_load_wav(OMM_SOUND_PEACH_LEDGE_MISS_UH,       "peach/omm_sound_peach_ledge_miss_uh",      0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_UH));
    omm_sound_load_wav(OMM_SOUND_PEACH_BONK_DOH,            "peach/omm_sound_peach_bonk_doh",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_DOH));
    omm_sound_load_wav(OMM_SOUND_PEACH_BONK_OOOF,           "peach/omm_sound_peach_bonk_ooof",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_OOOF));
    omm_sound_load_wav(OMM_SOUND_PEACH_ATTACKED,            "peach/omm_sound_peach_attacked",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_ATTACKED));
    omm_sound_load_wav(OMM_SOUND_PEACH_ON_FIRE,             "peach/omm_sound_peach_on_fire",            0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_ON_FIRE));
    omm_sound_load_wav(OMM_SOUND_PEACH_DYING,               "peach/omm_sound_peach_dying",              0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_DYING));
    omm_sound_load_wav(OMM_SOUND_PEACH_DROWNING,            "peach/omm_sound_peach_drowning",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_DROWNING));
    omm_sound_load_wav(OMM_SOUND_PEACH_FALLING_WAAAOOOW,    "peach/omm_sound_peach_falling_waaaooow",   0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_YAH_WAH_HOO));
    omm_sound_load_wav(OMM_SOUND_PEACH_MAMA_MIA,            "peach/omm_sound_peach_mama_mia",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_MAMA_MIA));
    omm_sound_load_wav(OMM_SOUND_PEACH_LETS_A_GO,           "peach/omm_sound_peach_lets_a_go",          0, 1, 0x54, SOUND_PRIO(SOUND_MENU_STAR_SOUND_LETS_A_GO));
    omm_sound_load_wav(OMM_SOUND_PEACH_HERE_WE_GO,          "peach/omm_sound_peach_here_we_go",         0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_HERE_WE_GO));
    omm_sound_load_wav(OMM_SOUND_PEACH_PANTING1,            "peach/omm_sound_peach_panting1",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_PANTING));
    omm_sound_load_wav(OMM_SOUND_PEACH_PANTING2,            "peach/omm_sound_peach_panting2",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_PANTING));
    omm_sound_load_wav(OMM_SOUND_PEACH_PANTING3,            "peach/omm_sound_peach_panting3",           0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_PANTING));
    omm_sound_load_wav(OMM_SOUND_PEACH_COUGHING1,           "peach/omm_sound_peach_coughing1",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_COUGHING1));
    omm_sound_load_wav(OMM_SOUND_PEACH_COUGHING2,           "peach/omm_sound_peach_coughing2",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_COUGHING2));
    omm_sound_load_wav(OMM_SOUND_PEACH_COUGHING3,           "peach/omm_sound_peach_coughing3",          0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_COUGHING3));
    omm_sound_load_wav(OMM_SOUND_PEACH_YAWNING,             "peach/omm_sound_peach_yawning",            0, 1, 0x54, SOUND_PRIO(SOUND_MARIO_YAWNING));
    omm_sound_load_wav(OMM_SOUND_PEACH_VIBE_JOY,            "peach/omm_sound_peach_vibe_joy",           0, 0, 0x54, 0xFF);
    omm_sound_load_wav(OMM_SOUND_PEACH_VIBE_RAGE,           "peach/omm_sound_peach_vibe_rage",          0, 0, 0x54, 0xFF);
    omm_sound_load_wav(OMM_SOUND_PEACH_VIBE_GLOOM,          "peach/omm_sound_peach_vibe_gloom",         0, 0, 0x54, 0xFF);
    omm_sound_load_wav(OMM_SOUND_PEACH_VIBE_CALM,           "peach/omm_sound_peach_vibe_calm",          0, 0, 0x54, 0xFF);

    // "toad" bank
    omm_sound_load_wav(OMM_SOUND_TOAD_1,                    "toad/omm_sound_toad_1",                    1, 0, 0x60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_2,                    "toad/omm_sound_toad_2",                    1, 0, 0x60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_3,                    "toad/omm_sound_toad_3",                    1, 0, 0x60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_4,                    "toad/omm_sound_toad_4",                    1, 0, 0x60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_5,                    "toad/omm_sound_toad_5",                    1, 0, 0x60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_6,                    "toad/omm_sound_toad_6",                    1, 0, 0x60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_7,                    "toad/omm_sound_toad_7",                    1, 0, 0x60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_8,                    "toad/omm_sound_toad_8",                    1, 0, 0x60, 0xFF);
    omm_sound_load_wav(OMM_SOUND_TOAD_9,                    "toad/omm_sound_toad_9",                    1, 0, 0x60, 0xFF);

    // "event" bank
    omm_sound_load_wav(OMM_SOUND_EVENT_CAPTURE,             "event/omm_sound_event_capture",            2, 0, 0x60, 0x80);
    omm_sound_load_wav(OMM_SOUND_EVENT_UNCAPTURE,           "event/omm_sound_event_uncapture",          2, 0, 0x60, 0x80);
    omm_sound_load_wav(OMM_SOUND_EVENT_LIFE_UP,             "event/omm_sound_event_life_up",            2, 0, 0x58, 0xF8);
    omm_sound_load_wav(OMM_SOUND_EVENT_SPARKLY_STAR_GET,    "event/omm_sound_event_sparkly_star_get",   2, 0, 0x74, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_MARIO,         "event/omm_sound_event_death_mario_1",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_MARIO_WATER,   "event/omm_sound_event_death_mario_2",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_MARIO_FALL,    "event/omm_sound_event_death_mario_3",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_PEACH,         "event/omm_sound_event_death_peach_1",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_PEACH_WATER,   "event/omm_sound_event_death_peach_2",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_PEACH_FALL,    "event/omm_sound_event_death_peach_3",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_LUIGI,         "event/omm_sound_event_death_luigi_1",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_LUIGI_WATER,   "event/omm_sound_event_death_luigi_2",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_LUIGI_FALL,    "event/omm_sound_event_death_luigi_3",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_WARIO,         "event/omm_sound_event_death_wario_1",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_WARIO_WATER,   "event/omm_sound_event_death_wario_2",      2, 0, 0x58, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EVENT_DEATH_WARIO_FALL,    "event/omm_sound_event_death_wario_3",      2, 0, 0x58, 0xFF);

    // "effect" bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_DAMAGE,             "effect/omm_sound_effect_damage",           3, 0, 0x40, 0xF0);
    omm_sound_load_wav(OMM_SOUND_EFFECT_HEAL,               "effect/omm_sound_effect_heal",             3, 0, 0x34, 0xF0);
    omm_sound_load_wav(OMM_SOUND_EFFECT_FREEZE_1,           "effect/omm_sound_effect_freeze_1",         3, 0, 0x7F, 0x00);
    omm_sound_load_wav(OMM_SOUND_EFFECT_FREEZE_2,           "effect/omm_sound_effect_freeze_2",         3, 0, 0x7F, 0x00);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PROPELLER_1,        "effect/omm_sound_effect_propeller_1",      3, 0, 0x60, 0x01);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PROPELLER_2,        "effect/omm_sound_effect_propeller_2",      3, 0, 0x54, 0x01);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PROPELLER_3,        "effect/omm_sound_effect_propeller_3",      3, 0, 0x58, 0x01);
    omm_sound_load_wav(OMM_SOUND_EFFECT_MIDAIR_SPIN,        "effect/omm_sound_effect_midair_spin",      3, 0, 0x40, 0x02);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PEACH_GLIDE,        "effect/omm_sound_effect_peach_glide",      3, 0, 0x30, 0x40);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PEACH_VIBE,         "effect/omm_sound_effect_peach_vibe",       3, 0, 0x30, 0xFF);
    omm_sound_load_wav(OMM_SOUND_EFFECT_PERRY_CHARGE,       "effect/omm_sound_effect_perry_charge",     1, 0, 0x40, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_PERRY_CHARGED,      "effect/omm_sound_effect_perry_charged",    1, 0, 0x40, 0xFF); // Use Toad sound bank
    omm_sound_load_wav(OMM_SOUND_EFFECT_PERRY_BEAM,         "effect/omm_sound_effect_perry_beam",       1, 0, 0x40, 0xFF); // Use Toad sound bank
}

//
// Character sounds
//

typedef struct { s32 type; union { s32 idNum; const char **idStr; }; } OmmCharacterSound;

#define SOUND_TYPE_NUL      (0 << 1)
#define SOUND_TYPE_N64      (1 << 1)
#define SOUND_TYPE_OMM      (2 << 1)
#define SOUND_TYPE_R96      (3 << 1)

#define SOUND_NUL           { .type = SOUND_TYPE_NUL, .idNum = 0 }
#define SOUND_N64(sound)    { .type = SOUND_TYPE_N64, .idNum = sound }
#define SOUND_OMM(sound)    { .type = SOUND_TYPE_OMM, .idNum = sound }
#define SOUND_R96(sound)    { .type = SOUND_TYPE_R96, .idStr = &sound }

static const OmmCharacterSound sOmmCharacterSoundsMarioN64[] = {
    SOUND_N64(SOUND_MARIO_YAH_WAH_HOO + 0x00000),       // Jump Yah
    SOUND_N64(SOUND_MARIO_YAH_WAH_HOO + 0x10000),       // Jump Wah
    SOUND_N64(SOUND_MARIO_YAH_WAH_HOO + 0x20000),       // Jump Hoo
    SOUND_N64(SOUND_MARIO_PUNCH_YAH),                   // Punch Yah
    SOUND_N64(SOUND_MARIO_PUNCH_WAH),                   // Punch Wah
    SOUND_N64(SOUND_MARIO_PUNCH_HOO),                   // Punch Hoo
    SOUND_N64(SOUND_MARIO_HOOHOO),                      // Hoo-hoo
    SOUND_N64(SOUND_MARIO_YAHOO),                       // Yahoo
    SOUND_N64(SOUND_MARIO_YAHOO_WAHA_YIPPEE + 0x00000), // Yahoo
    SOUND_N64(SOUND_MARIO_YAHOO_WAHA_YIPPEE + 0x10000), // Yahoo
    SOUND_N64(SOUND_MARIO_YAHOO_WAHA_YIPPEE + 0x20000), // Yahoo
    SOUND_N64(SOUND_MARIO_YAHOO_WAHA_YIPPEE + 0x30000), // Waha
    SOUND_N64(SOUND_MARIO_YAHOO_WAHA_YIPPEE + 0x40000), // Yippee
    SOUND_N64(SOUND_MARIO_HAHA),                        // Ha-ha
    SOUND_N64(SOUND_MARIO_HAHA_2),                      // Ha-ha
    SOUND_N64(SOUND_MARIO_UH),                          // Grunt 1
    SOUND_N64(SOUND_MARIO_UH2),                         // Grunt 2
    SOUND_N64(SOUND_MARIO_UH2_2),                       // Grunt 2
    SOUND_N64(SOUND_MARIO_HRMM),                        // Lift
    SOUND_N64(SOUND_MARIO_WAH2),                        // Throw
    SOUND_N64(SOUND_MARIO_GROUND_POUND_WAH),            // Ground pound Wah
    SOUND_N64(SOUND_MARIO_WHOA),                        // Whoa
    SOUND_N64(SOUND_MARIO_EEUH),                        // Pull-up
    SOUND_N64(SOUND_MARIO_ATTACKED),                    // Attacked
    SOUND_N64(SOUND_MARIO_OOOF),                        // Ooof
    SOUND_N64(SOUND_MARIO_OOOF2),                       // Ooof
    SOUND_N64(SOUND_MARIO_DOH),                         // Doh
    SOUND_N64(SOUND_MARIO_HERE_WE_GO),                  // Here we go
    SOUND_N64(SOUND_MARIO_YAWNING),                     // Yawning
    SOUND_N64(SOUND_MARIO_SNORING1),                    // Snoring 1
    SOUND_N64(SOUND_MARIO_SNORING2),                    // Snoring 2
    SOUND_N64(SOUND_MARIO_SNORING3),                    // Snoring 3
    SOUND_N64(SOUND_MARIO_PANTING + 0x00000),           // Panting 1
    SOUND_N64(SOUND_MARIO_PANTING + 0x10000),           // Panting 2
    SOUND_N64(SOUND_MARIO_PANTING + 0x20000),           // Panting 3
    SOUND_N64(SOUND_MARIO_PANTING_COLD),                // Panting (cold)
    SOUND_N64(SOUND_MARIO_COUGHING1),                   // Coughing 1
    SOUND_N64(SOUND_MARIO_COUGHING2),                   // Coughing 2
    SOUND_N64(SOUND_MARIO_COUGHING3),                   // Coughing 3
    SOUND_N64(SOUND_MARIO_WAAAOOOW),                    // Falling
    SOUND_N64(SOUND_MARIO_ON_FIRE),                     // Burned
    SOUND_N64(SOUND_MARIO_DYING),                       // Dying
    SOUND_N64(SOUND_MARIO_DROWNING),                    // Drowning
    SOUND_N64(SOUND_MARIO_MAMA_MIA),                    // Mama-mia
    SOUND_N64(SOUND_MARIO_TWIRL_BOUNCE),                // Boing
    SOUND_N64(SOUND_MARIO_SO_LONGA_BOWSER),             // So long-a Bowser
    SOUND_N64(SOUND_MARIO_IMA_TIRED),                   // I'm-a tired
    SOUND_N64(SOUND_MENU_STAR_SOUND_LETS_A_GO),         // Let's-a go (+ star sound)
    SOUND_N64(SOUND_MARIO_OKEY_DOKEY),                  // Okey dokey
    SOUND_N64(SOUND_MARIO_GAME_OVER),                   // Game Over
    SOUND_N64(SOUND_MARIO_HELLO),                       // Hello
    SOUND_N64(SOUND_MARIO_PRESS_START_TO_PLAY),         // Press Start to play
    SOUND_N64(SOUND_MENU_THANK_YOU_PLAYING_MY_GAME),    // Thank you so much for playing my game
    SOUND_N64(SOUND_MENU_STAR_SOUND_OKEY_DOKEY),        // Okey dokey (+ star sound)
    SOUND_N64(SOUND_PEACH_MARIO),                       // Peach: Mario
    SOUND_N64(SOUND_PEACH_POWER_OF_THE_STARS),          // Peach: The power of the Stars...
    SOUND_N64(SOUND_PEACH_THANKS_TO_YOU),               // Peach: Thanks to you
    SOUND_N64(SOUND_PEACH_THANK_YOU_MARIO),             // Peach: Thank you Mario
    SOUND_N64(SOUND_PEACH_SOMETHING_SPECIAL),           // Peach: We have to do something special...
    SOUND_N64(SOUND_PEACH_BAKE_A_CAKE),                 // Peach: Let's bake a delicious cake
    SOUND_N64(SOUND_PEACH_FOR_MARIO),                   // Peach: For Mario
    SOUND_N64(SOUND_PEACH_MARIO2),                      // Peach: Mario!
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_MARIO),             // OMM sound: Dying
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_MARIO_WATER),       // OMM sound: Drowning
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_MARIO_FALL),        // OMM sound: Falling
};

static const OmmCharacterSound sOmmCharacterSoundsPeachOMM[] = {
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_YAH),                // Jump Yah
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_WAH),                // Jump Wah
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_HOO),                // Jump Hoo
    SOUND_OMM(OMM_SOUND_PEACH_PUNCH_YAH),               // Punch Yah
    SOUND_OMM(OMM_SOUND_PEACH_PUNCH_WAH),               // Punch Wah
    SOUND_OMM(OMM_SOUND_PEACH_PUNCH_HOO),               // Punch Hoo
    SOUND_OMM(OMM_SOUND_PEACH_DIVE_HOOHOO),             // Hoo-hoo
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_YAHOO),              // Yahoo
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_YAHOO),              // Yahoo
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_YAHOO),              // Yahoo
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_YAHOO),              // Yahoo
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_WAHA),               // Waha
    SOUND_OMM(OMM_SOUND_PEACH_JUMP_YIPPEE),             // Yippee
    SOUND_OMM(OMM_SOUND_PEACH_LAND_HAHA),               // Ha-ha
    SOUND_OMM(OMM_SOUND_PEACH_LAND_HAHA),               // Ha-ha
    SOUND_OMM(OMM_SOUND_PEACH_LEDGE_MISS_UH),           // Grunt 1
    SOUND_OMM(OMM_SOUND_PEACH_LEDGE_CLIMB_UH2),         // Grunt 2
    SOUND_OMM(OMM_SOUND_PEACH_LEDGE_CLIMB_UH2),         // Grunt 2
    SOUND_OMM(OMM_SOUND_PEACH_GRAB_HRMM),               // Lift
    SOUND_OMM(OMM_SOUND_PEACH_THROW_WAH2),              // Throw
    SOUND_OMM(OMM_SOUND_PEACH_GROUND_POUND_WAH),        // Ground pound Wah
    SOUND_OMM(OMM_SOUND_PEACH_LEDGE_GRAB_WHOA),         // Whoa
    SOUND_OMM(OMM_SOUND_PEACH_LEDGE_CLIMB_EEUH),        // Pull-up
    SOUND_OMM(OMM_SOUND_PEACH_ATTACKED),                // Attacked
    SOUND_OMM(OMM_SOUND_PEACH_BONK_OOOF),               // Ooof
    SOUND_OMM(OMM_SOUND_PEACH_BONK_OOOF),               // Ooof
    SOUND_OMM(OMM_SOUND_PEACH_BONK_DOH),                // Doh
    SOUND_OMM(OMM_SOUND_PEACH_HERE_WE_GO),              // Here we go
    SOUND_OMM(OMM_SOUND_PEACH_YAWNING),                 // Yawning
    SOUND_NUL,                                          // Snoring 1
    SOUND_NUL,                                          // Snoring 2
    SOUND_NUL,                                          // Snoring 3
    SOUND_OMM(OMM_SOUND_PEACH_PANTING1),                // Panting 1
    SOUND_OMM(OMM_SOUND_PEACH_PANTING2),                // Panting 2
    SOUND_OMM(OMM_SOUND_PEACH_PANTING3),                // Panting 3
    SOUND_NUL,                                          // Panting (cold)
    SOUND_OMM(OMM_SOUND_PEACH_COUGHING1),               // Coughing 1
    SOUND_OMM(OMM_SOUND_PEACH_COUGHING2),               // Coughing 2
    SOUND_OMM(OMM_SOUND_PEACH_COUGHING3),               // Coughing 3
    SOUND_OMM(OMM_SOUND_PEACH_FALLING_WAAAOOOW),        // Falling
    SOUND_OMM(OMM_SOUND_PEACH_ON_FIRE),                 // Burned
    SOUND_OMM(OMM_SOUND_PEACH_DYING),                   // Dying
    SOUND_OMM(OMM_SOUND_PEACH_DROWNING),                // Drowning
    SOUND_OMM(OMM_SOUND_PEACH_MAMA_MIA),                // Mama-mia
    SOUND_OMM(OMM_SOUND_PEACH_TWIRL_BOUNCE),            // Boing
    SOUND_OMM(OMM_SOUND_PEACH_SO_LONGA_BOWSER),         // So long-a Bowser
    SOUND_NUL,                                          // I'm-a tired
    SOUND_OMM(OMM_SOUND_PEACH_LETS_A_GO),               // Let's-a go (+ star sound)
#if OMM_GAME_IS_R96A
    SOUND_R96(R96_MARIO_OKEY_DOKEY),                    // Okey dokey
    SOUND_R96(R96_MARIO_GAME_OVER),                     // Game Over
    SOUND_R96(R96_MARIO_HELLO),                         // Hello
    SOUND_R96(R96_MARIO_PRESS_START_TO_PLAY),           // Press Start to play
    SOUND_R96(R96_MARIO_THANK_YOU_PLAYING_MY_GAME),     // Thank you so much for playing my game
    SOUND_R96(R96_MARIO_OKEY_DOKEY),                    // Okey dokey (+ star sound)
    SOUND_R96(R96_WARIO_PEACH_WARIO),                   // Peach: Mario
    SOUND_R96(R96_WARIO_PEACH_POWER_OF_THE_STARS),      // Peach: The power of the Stars...
    SOUND_R96(R96_WARIO_PEACH_THANKS_TO_YOU),           // Peach: Thanks to you
    SOUND_R96(R96_WARIO_PEACH_THANK_YOU_WARIO),         // Peach: Thank you Mario
    SOUND_R96(R96_WARIO_PEACH_SOMETHING_SPECIAL),       // Peach: We have to do something special...
    SOUND_R96(R96_WARIO_PEACH_BAKE_A_CAKE),             // Peach: Let's bake a delicious cake
    SOUND_R96(R96_WARIO_PEACH_FOR_WARIO),               // Peach: For Mario
    SOUND_R96(R96_WARIO_SOUND_PEACH_WARIO2),            // Peach: Mario!
#else
    SOUND_N64(SOUND_MENU_STAR_SOUND_OKEY_DOKEY),        // Okey dokey
    SOUND_N64(SOUND_MARIO_GAME_OVER),                   // Game Over
    SOUND_N64(SOUND_MARIO_HELLO),                       // Hello
    SOUND_N64(SOUND_MARIO_PRESS_START_TO_PLAY),         // Press Start to play
    SOUND_N64(SOUND_MENU_THANK_YOU_PLAYING_MY_GAME),    // Thank you so much for playing my game
    SOUND_N64(SOUND_MENU_STAR_SOUND_OKEY_DOKEY),        // Okey dokey (+ star sound)
    SOUND_N64(SOUND_PEACH_MARIO),                       // Peach: Mario
    SOUND_N64(SOUND_PEACH_POWER_OF_THE_STARS),          // Peach: The power of the Stars...
    SOUND_N64(SOUND_PEACH_THANKS_TO_YOU),               // Peach: Thanks to you
    SOUND_N64(SOUND_PEACH_THANK_YOU_MARIO),             // Peach: Thank you Mario
    SOUND_N64(SOUND_PEACH_SOMETHING_SPECIAL),           // Peach: We have to do something special...
    SOUND_N64(SOUND_PEACH_BAKE_A_CAKE),                 // Peach: Let's bake a delicious cake
    SOUND_N64(SOUND_PEACH_FOR_MARIO),                   // Peach: For Mario
    SOUND_N64(SOUND_PEACH_MARIO2),                      // Peach: Mario!
#endif
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_PEACH),             // OMM sound: Dying
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_PEACH_WATER),       // OMM sound: Drowning
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_PEACH_FALL),        // OMM sound: Falling
};

#if OMM_GAME_IS_R96A
static const OmmCharacterSound sOmmCharacterSoundsMarioR96[] = {
    SOUND_R96(R96_MARIO_YAH),                           // Jump Yah
    SOUND_R96(R96_MARIO_WAH),                           // Jump Wah
    SOUND_R96(R96_MARIO_HOO),                           // Jump Hoo
    SOUND_R96(R96_MARIO_PUNCH_YAH),                     // Punch Yah
    SOUND_R96(R96_MARIO_WAH),                           // Punch Wah
    SOUND_R96(R96_MARIO_PUNCH_HOO),                     // Punch Hoo
    SOUND_R96(R96_MARIO_HOOHOO),                        // Hoo-hoo
    SOUND_R96(R96_MARIO_YAHOO),                         // Yahoo
    SOUND_R96(R96_MARIO_YAHOO),                         // Yahoo
    SOUND_R96(R96_MARIO_YAHOO),                         // Yahoo
    SOUND_R96(R96_MARIO_YAHOO),                         // Yahoo
    SOUND_R96(R96_MARIO_WAHA),                          // Waha
    SOUND_R96(R96_MARIO_YIPPEE),                        // Yippee
    SOUND_R96(R96_MARIO_HAHA),                          // Ha-ha
    SOUND_R96(R96_MARIO_HAHA),                          // Ha-ha
    SOUND_R96(R96_MARIO_GRUNT),                         // Grunt 1
    SOUND_R96(R96_MARIO_GRUNT_2),                       // Grunt 2
    SOUND_R96(R96_MARIO_GRUNT_2),                       // Grunt 2
    SOUND_R96(R96_MARIO_LIFT),                          // Lift
    SOUND_R96(R96_MARIO_THROW),                         // Throw
    SOUND_R96(R96_MARIO_THROW),                         // Ground pound Wah
    SOUND_R96(R96_MARIO_WHOA),                          // Whoa
    SOUND_R96(R96_MARIO_PULL_UP),                       // Pull-up
    SOUND_R96(R96_MARIO_ATTACKED),                      // Attacked
    SOUND_R96(R96_MARIO_OOOF),                          // Ooof
    SOUND_R96(R96_MARIO_OOOF),                          // Ooof
    SOUND_R96(R96_MARIO_DOH),                           // Doh
    SOUND_R96(R96_MARIO_HERE_WE_GO),                    // Here we go
    SOUND_R96(R96_MARIO_YAWNING),                       // Yawning
    SOUND_R96(R96_MARIO_SNORING_1),                     // Snoring 1
    SOUND_R96(R96_MARIO_SNORING_2),                     // Snoring 2
    SOUND_R96(R96_MARIO_SNORING_3),                     // Snoring 3
    SOUND_R96(R96_MARIO_PANTING),                       // Panting 1
    SOUND_R96(R96_MARIO_PANTING),                       // Panting 2
    SOUND_R96(R96_MARIO_PANTING),                       // Panting 3
    SOUND_R96(R96_MARIO_PANTING_COLD),                  // Panting (cold)
    SOUND_R96(R96_MARIO_COUGHING_1),                    // Coughing 1
    SOUND_R96(R96_MARIO_COUGHING_1),                    // Coughing 2
    SOUND_R96(R96_MARIO_COUGHING_1),                    // Coughing 3
    SOUND_R96(R96_MARIO_FALLING),                       // Falling
    SOUND_R96(R96_MARIO_ON_FIRE),                       // Burned
    SOUND_R96(R96_MARIO_DYING),                         // Dying
    SOUND_R96(R96_MARIO_DROWNING),                      // Drowning
    SOUND_R96(R96_MARIO_MAMA_MIA),                      // Mama-mia
    SOUND_R96(R96_MARIO_TWIRL_BOUNCE),                  // Boing
    SOUND_R96(R96_MARIO_SO_LONGA_BOWSER),               // So long-a Bowser
    SOUND_R96(R96_MARIO_IMA_TIRED),                     // I'm-a tired
    SOUND_R96(R96_MARIO_SOUND_LETS_A_GO),               // Let's-a go (+ star sound)
    SOUND_R96(R96_MARIO_OKEY_DOKEY),                    // Okey dokey
    SOUND_R96(R96_MARIO_GAME_OVER),                     // Game Over
    SOUND_R96(R96_MARIO_HELLO),                         // Hello
    SOUND_R96(R96_MARIO_PRESS_START_TO_PLAY),           // Press Start to play
    SOUND_R96(R96_MARIO_THANK_YOU_PLAYING_MY_GAME),     // Thank you so much for playing my game
    SOUND_R96(R96_MARIO_OKEY_DOKEY),                    // Okey dokey (+ star sound)
    SOUND_R96(R96_MARIO_PEACH_MARIO),                   // Peach: Mario
    SOUND_R96(R96_MARIO_PEACH_POWER_OF_THE_STARS),      // Peach: The power of the Stars...
    SOUND_R96(R96_MARIO_PEACH_THANKS_TO_YOU),           // Peach: Thanks to you
    SOUND_R96(R96_MARIO_PEACH_THANK_YOU_MARIO),         // Peach: Thank you Mario
    SOUND_R96(R96_MARIO_PEACH_SOMETHING_SPECIAL),       // Peach: We have to do something special...
    SOUND_R96(R96_MARIO_PEACH_BAKE_A_CAKE),             // Peach: Let's bake a delicious cake
    SOUND_R96(R96_MARIO_PEACH_FOR_MARIO),               // Peach: For Mario
    SOUND_R96(R96_MARIO_SOUND_PEACH_MARIO2),            // Peach: Mario!
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_MARIO),             // OMM sound: Dying
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_MARIO_WATER),       // OMM sound: Drowning
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_MARIO_FALL),        // OMM sound: Falling
};

static const OmmCharacterSound sOmmCharacterSoundsLuigiR96[] = {
    SOUND_R96(R96_LUIGI_YAH),                           // Jump Yah
    SOUND_R96(R96_LUIGI_WAH),                           // Jump Wah
    SOUND_R96(R96_LUIGI_HOO),                           // Jump Hoo
    SOUND_R96(R96_LUIGI_PUNCH_YAH),                     // Punch Yah
    SOUND_R96(R96_LUIGI_WAH),                           // Punch Wah
    SOUND_R96(R96_LUIGI_PUNCH_HOO),                     // Punch Hoo
    SOUND_R96(R96_LUIGI_HOOHOO),                        // Hoo-hoo
    SOUND_R96(R96_LUIGI_YAHOO),                         // Yahoo
    SOUND_R96(R96_LUIGI_YAHOO),                         // Yahoo
    SOUND_R96(R96_LUIGI_YAHOO),                         // Yahoo
    SOUND_R96(R96_LUIGI_YAHOO),                         // Yahoo
    SOUND_R96(R96_LUIGI_WAHA),                          // Waha
    SOUND_R96(R96_LUIGI_YIPPEE),                        // Yippee
    SOUND_R96(R96_LUIGI_HAHA),                          // Ha-ha
    SOUND_R96(R96_LUIGI_HAHA),                          // Ha-ha
    SOUND_R96(R96_LUIGI_GRUNT),                         // Grunt 1
    SOUND_R96(R96_LUIGI_GRUNT_2),                       // Grunt 2
    SOUND_R96(R96_LUIGI_GRUNT_2),                       // Grunt 2
    SOUND_R96(R96_LUIGI_LIFT),                          // Lift
    SOUND_R96(R96_LUIGI_THROW),                         // Throw
    SOUND_R96(R96_LUIGI_THROW),                         // Ground pound Wah
    SOUND_R96(R96_LUIGI_WHOA),                          // Whoa
    SOUND_R96(R96_LUIGI_PULL_UP),                       // Pull-up
    SOUND_R96(R96_LUIGI_ATTACKED),                      // Attacked
    SOUND_R96(R96_LUIGI_OOOF),                          // Ooof
    SOUND_R96(R96_LUIGI_OOOF),                          // Ooof
    SOUND_R96(R96_LUIGI_DOH),                           // Doh
    SOUND_R96(R96_LUIGI_HERE_WE_GO),                    // Here we go
    SOUND_R96(R96_LUIGI_YAWNING),                       // Yawning
    SOUND_R96(R96_LUIGI_SNORING_1),                     // Snoring 1
    SOUND_R96(R96_LUIGI_SNORING_2),                     // Snoring 2
    SOUND_R96(R96_LUIGI_SNORING_3),                     // Snoring 3
    SOUND_R96(R96_LUIGI_PANTING),                       // Panting 1
    SOUND_R96(R96_LUIGI_PANTING),                       // Panting 2
    SOUND_R96(R96_LUIGI_PANTING),                       // Panting 3
    SOUND_R96(R96_LUIGI_PANTING_COLD),                  // Panting (cold)
    SOUND_R96(R96_LUIGI_COUGHING_1),                    // Coughing 1
    SOUND_R96(R96_LUIGI_COUGHING_1),                    // Coughing 2
    SOUND_R96(R96_LUIGI_COUGHING_1),                    // Coughing 3
    SOUND_R96(R96_LUIGI_FALLING),                       // Falling
    SOUND_R96(R96_LUIGI_ON_FIRE),                       // Burned
    SOUND_R96(R96_LUIGI_DYING),                         // Dying
    SOUND_R96(R96_LUIGI_DROWNING),                      // Drowning
    SOUND_R96(R96_LUIGI_MAMA_MIA),                      // Mama-mia
    SOUND_R96(R96_LUIGI_TWIRL_BOUNCE),                  // Boing
    SOUND_R96(R96_LUIGI_SO_LONGA_BOWSER),               // So long-a Bowser
    SOUND_R96(R96_LUIGI_IMA_TIRED),                     // I'm-a tired
    SOUND_R96(R96_LUIGI_SOUND_LETS_A_GO),               // Let's-a go (+ star sound)
    SOUND_R96(R96_LUIGI_OKEY_DOKEY),                    // Okey dokey
    SOUND_R96(R96_LUIGI_GAME_OVER),                     // Game Over
    SOUND_R96(R96_LUIGI_HELLO),                         // Hello
    SOUND_R96(R96_LUIGI_PRESS_START_TO_PLAY),           // Press Start to play
    SOUND_R96(R96_LUIGI_THANK_YOU_PLAYING_MY_GAME),     // Thank you so much for playing my game
    SOUND_R96(R96_LUIGI_OKEY_DOKEY),                    // Okey dokey (+ star sound)
    SOUND_R96(R96_LUIGI_PEACH_LUIGI),                   // Peach: Mario
    SOUND_R96(R96_LUIGI_PEACH_POWER_OF_THE_STARS),      // Peach: The power of the Stars...
    SOUND_R96(R96_LUIGI_PEACH_THANKS_TO_YOU),           // Peach: Thanks to you
    SOUND_R96(R96_LUIGI_PEACH_THANK_YOU_LUIGI),         // Peach: Thank you Mario
    SOUND_R96(R96_LUIGI_PEACH_SOMETHING_SPECIAL),       // Peach: We have to do something special...
    SOUND_R96(R96_LUIGI_PEACH_BAKE_A_CAKE),             // Peach: Let's bake a delicious cake
    SOUND_R96(R96_LUIGI_PEACH_FOR_LUIGI),               // Peach: For Mario
    SOUND_R96(R96_LUIGI_SOUND_PEACH_LUIGI2),            // Peach: Mario!
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_LUIGI),             // OMM sound: Dying
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_LUIGI_WATER),       // OMM sound: Drowning
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_LUIGI_FALL),        // OMM sound: Falling
};

static const OmmCharacterSound sOmmCharacterSoundsWarioR96[] = {
    SOUND_R96(R96_WARIO_YAH),                           // Jump Yah
    SOUND_R96(R96_WARIO_WAH),                           // Jump Wah
    SOUND_R96(R96_WARIO_HOO),                           // Jump Hoo
    SOUND_R96(R96_WARIO_PUNCH_YAH),                     // Punch Yah
    SOUND_R96(R96_WARIO_WAH),                           // Punch Wah
    SOUND_R96(R96_WARIO_PUNCH_HOO),                     // Punch Hoo
    SOUND_R96(R96_WARIO_HOOHOO),                        // Hoo-hoo
    SOUND_R96(R96_WARIO_YAHOO),                         // Yahoo
    SOUND_R96(R96_WARIO_YAHOO),                         // Yahoo
    SOUND_R96(R96_WARIO_YAHOO),                         // Yahoo
    SOUND_R96(R96_WARIO_YAHOO),                         // Yahoo
    SOUND_R96(R96_WARIO_WAHA),                          // Waha
    SOUND_R96(R96_WARIO_YIPPEE),                        // Yippee
    SOUND_R96(R96_WARIO_HAHA),                          // Ha-ha
    SOUND_R96(R96_WARIO_HAHA),                          // Ha-ha
    SOUND_R96(R96_WARIO_GRUNT),                         // Grunt 1
    SOUND_R96(R96_WARIO_GRUNT_2),                       // Grunt 2
    SOUND_R96(R96_WARIO_GRUNT_2),                       // Grunt 2
    SOUND_R96(R96_WARIO_LIFT),                          // Lift
    SOUND_R96(R96_WARIO_THROW),                         // Throw
    SOUND_R96(R96_WARIO_THROW),                         // Ground pound Wah
    SOUND_R96(R96_WARIO_WHOA),                          // Whoa
    SOUND_R96(R96_WARIO_PULL_UP),                       // Pull-up
    SOUND_R96(R96_WARIO_ATTACKED),                      // Attacked
    SOUND_R96(R96_WARIO_OOOF),                          // Ooof
    SOUND_R96(R96_WARIO_OOOF),                          // Ooof
    SOUND_R96(R96_WARIO_DOH),                           // Doh
    SOUND_R96(R96_WARIO_HERE_WE_GO),                    // Here we go
    SOUND_R96(R96_WARIO_YAWNING),                       // Yawning
    SOUND_R96(R96_WARIO_SNORING_1),                     // Snoring 1
    SOUND_R96(R96_WARIO_SNORING_2),                     // Snoring 2
    SOUND_R96(R96_WARIO_SNORING_3),                     // Snoring 3
    SOUND_R96(R96_WARIO_PANTING),                       // Panting 1
    SOUND_R96(R96_WARIO_PANTING),                       // Panting 2
    SOUND_R96(R96_WARIO_PANTING),                       // Panting 3
    SOUND_R96(R96_WARIO_PANTING_COLD),                  // Panting (cold)
    SOUND_R96(R96_WARIO_COUGHING_1),                    // Coughing 1
    SOUND_R96(R96_WARIO_COUGHING_1),                    // Coughing 2
    SOUND_R96(R96_WARIO_COUGHING_1),                    // Coughing 3
    SOUND_R96(R96_WARIO_FALLING),                       // Falling
    SOUND_R96(R96_WARIO_ON_FIRE),                       // Burned
    SOUND_R96(R96_WARIO_DYING),                         // Dying
    SOUND_R96(R96_WARIO_DROWNING),                      // Drowning
    SOUND_R96(R96_WARIO_MAMA_MIA),                      // Mama-mia
    SOUND_R96(R96_WARIO_TWIRL_BOUNCE),                  // Boing
    SOUND_R96(R96_WARIO_SO_LONGA_BOWSER),               // So long-a Bowser
    SOUND_R96(R96_WARIO_IMA_TIRED),                     // I'm-a tired
    SOUND_R96(R96_WARIO_SOUND_LETS_A_GO),               // Let's-a go (+ star sound)
    SOUND_R96(R96_WARIO_OKEY_DOKEY),                    // Okey dokey
    SOUND_R96(R96_WARIO_GAME_OVER),                     // Game Over
    SOUND_R96(R96_WARIO_HELLO),                         // Hello
    SOUND_R96(R96_WARIO_PRESS_START_TO_PLAY),           // Press Start to play
    SOUND_R96(R96_WARIO_THANK_YOU_PLAYING_MY_GAME),     // Thank you so much for playing my game
    SOUND_R96(R96_WARIO_OKEY_DOKEY),                    // Okey dokey (+ star sound)
    SOUND_R96(R96_WARIO_PEACH_WARIO),                   // Peach: Mario
    SOUND_R96(R96_WARIO_PEACH_POWER_OF_THE_STARS),      // Peach: The power of the Stars...
    SOUND_R96(R96_WARIO_PEACH_THANKS_TO_YOU),           // Peach: Thanks to you
    SOUND_R96(R96_WARIO_PEACH_THANK_YOU_WARIO),         // Peach: Thank you Mario
    SOUND_R96(R96_WARIO_PEACH_SOMETHING_SPECIAL),       // Peach: We have to do something special...
    SOUND_R96(R96_WARIO_PEACH_BAKE_A_CAKE),             // Peach: Let's bake a delicious cake
    SOUND_R96(R96_WARIO_PEACH_FOR_WARIO),               // Peach: For Mario
    SOUND_R96(R96_WARIO_SOUND_PEACH_WARIO2),            // Peach: Mario!
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_WARIO),             // OMM sound: Dying
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_WARIO_WATER),       // OMM sound: Drowning
    SOUND_OMM(OMM_SOUND_EVENT_DEATH_WARIO_FALL),        // OMM sound: Falling
};
#endif

static const OmmCharacterSound *sOmmCharacterSounds[] = {
    sOmmCharacterSoundsMarioN64,
    sOmmCharacterSoundsPeachOMM,
#if OMM_GAME_IS_R96A
    sOmmCharacterSoundsMarioR96,
    sOmmCharacterSoundsLuigiR96,
    sOmmCharacterSoundsWarioR96,
#endif
};

static const OmmCharacterSound *sOmmCharacterSoundBanks[] = {
#if OMM_GAME_IS_R96A
    sOmmCharacterSoundsMarioR96,
    sOmmCharacterSoundsPeachOMM,
    sOmmCharacterSoundsLuigiR96,
    sOmmCharacterSoundsWarioR96,
#else
    sOmmCharacterSoundsMarioN64,
    sOmmCharacterSoundsPeachOMM,
    sOmmCharacterSoundsMarioN64,
    sOmmCharacterSoundsMarioN64,
#endif
};

static void omm_sound_process_character_sound_from_index(s32 index, f32 *pos, bool play) {
    const OmmCharacterSound *sound = &sOmmCharacterSoundBanks[omm_player_get_selected_index()][index];
    switch ((sound ? sound->type : 0) | play) {
        case SOUND_TYPE_NUL | 0: break;
        case SOUND_TYPE_NUL | 1: break;
        case SOUND_TYPE_N64 | 0: sound_stop(sound->idNum, pos ? pos : gGlobalSoundArgs); break;
        case SOUND_TYPE_N64 | 1: play_sound(sound->idNum, pos ? pos : gGlobalSoundArgs); break;
        case SOUND_TYPE_OMM | 0: omm_sound_stop(sound->idNum); break;
        case SOUND_TYPE_OMM | 1: omm_sound_play(sound->idNum, pos ? pos : gGlobalSoundArgs); break;
#if OMM_GAME_IS_R96A
        case SOUND_TYPE_R96 | 0: if (dynos_sound_is_playing(*sound->idStr)) { dynos_sound_stop(1); } break;
        case SOUND_TYPE_R96 | 1: dynos_sound_play(*sound->idStr, pos ? pos : gGlobalSoundArgs); break;
#endif
    }
}

static bool omm_sound_compare(const OmmCharacterSound *left, const OmmCharacterSound *right) {
    if (left->type == right->type) {
        switch (left->type) {
            case SOUND_TYPE_NUL: return false;
            case SOUND_TYPE_N64: return SOUND_ID(left->idNum) == SOUND_ID(right->idNum);
            case SOUND_TYPE_OMM: return left->idNum == right->idNum;
            case SOUND_TYPE_R96: return strcmp(*left->idStr, *right->idStr) == 0;
        }
    }
    return false;
}

static bool omm_sound_process_character_sound(const OmmCharacterSound sound, f32 *pos, bool play) {
    static bool exec = false;
    if (exec) return false;
    exec = true;
    for_each_(const OmmCharacterSound *, sounds, OMM_ARRAY_SIZE(sOmmCharacterSounds), sOmmCharacterSounds) {
        for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmCharacterSoundsMarioN64); ++i) {
            if (omm_sound_compare(*sounds + i, &sound)) {
                omm_sound_process_character_sound_from_index(i, pos, play);
                exec = false;
                return true;
            }
        }
    }
    exec = false;
    return false;
}

bool omm_sound_play_character_sound_n64(s32 id, f32 *pos) {
    return omm_sound_process_character_sound((const OmmCharacterSound) SOUND_N64(id), pos, true);
}

bool omm_sound_play_character_sound_omm(s32 id, f32 *pos) {
    return omm_sound_process_character_sound((const OmmCharacterSound) SOUND_OMM(id), pos, true);
}

bool omm_sound_play_character_sound_r96(const char *id, f32 *pos) {
    return omm_sound_process_character_sound((const OmmCharacterSound) SOUND_R96(id), pos, true);
}

bool omm_sound_stop_character_sound_n64(s32 id, f32 *pos) {
    return omm_sound_process_character_sound((const OmmCharacterSound) SOUND_N64(id), pos, false);
}

bool omm_sound_stop_character_sound_omm(s32 id, f32 *pos) {
    return omm_sound_process_character_sound((const OmmCharacterSound) SOUND_OMM(id), pos, false);
}

bool omm_sound_stop_character_sound_r96(const char *id, f32 *pos) {
    return omm_sound_process_character_sound((const OmmCharacterSound) SOUND_R96(id), pos, false);
}
