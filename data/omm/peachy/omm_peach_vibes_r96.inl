#if OMM_GAME_IS_R96A

extern "C" {
#include "data/omm/omm_defines.h"
#include "data/omm/peachy/omm_peach_actions.h"
#include "data/r96/r96_defines.h"
extern s32 *gOmmPeachVibeTypeRef;
}

//
// Macros
// The same code is used for both Musics and Jingles, and this file is included twice
//

#undef sVibeAudioFreqModifiers
#undef sVibeAudioVolumeModifiers
#undef sPlayingAudio
#undef softenAudioVolume
#undef configAudioMute
#undef DynOS_Audio_Resample
#undef DynOS_Music_Callback
#undef DynOS_Jingle_Callback
#undef DynOS_Audio_Callback

#if defined(sLoadedJingles)
#define sVibeAudioFreqModifiers     sVibeJingleFreqModifiers
#define sVibeAudioVolumeModifiers   sVibeJingleVolumeModifiers
#define sPlayingAudio               sPlayingJingle
#define softenAudioVolume           softenJingleVolume
#define configAudioMute             0
#define DynOS_Audio_Resample        DynOS_Jingle_Resample
#define DynOS_Jingle_Callback       DynOS_Jingle_Callback_
#define DynOS_Audio_Callback        DynOS_Jingle_Callback
#else
#define sVibeAudioFreqModifiers     sVibeMusicFreqModifiers
#define sVibeAudioVolumeModifiers   sVibeMusicVolumeModifiers
#define sPlayingAudio               sPlayingMusic
#define softenAudioVolume           softenVolume
#define configAudioMute             configMusicMute
#define DynOS_Audio_Resample        DynOS_Music_Resample
#define DynOS_Music_Callback        DynOS_Music_Callback_
#define DynOS_Audio_Callback        DynOS_Music_Callback
#endif

#define SAMPLE_ALIGNMENT            (sizeof(s16) * MUSIC_CHANNELS)
#define ALIGN_TO_SAMPLE(x)          ((((s32) (x)) / SAMPLE_ALIGNMENT) * SAMPLE_ALIGNMENT)

//
// Constants
//

static const f32 sVibeAudioFreqModifiers[] = {
    1.f,
    OMM_PEACH_VIBE_MUSIC_R96A_FREQ_MOD_JOY,
    OMM_PEACH_VIBE_MUSIC_R96A_FREQ_MOD_RAGE,
    OMM_PEACH_VIBE_MUSIC_R96A_FREQ_MOD_GLOOM,
    OMM_PEACH_VIBE_MUSIC_R96A_FREQ_MOD_CALM,
};

static const f32 sVibeAudioVolumeModifiers[] = {
    1.f,
    OMM_PEACH_VIBE_MUSIC_R96A_VOLUME_MOD_JOY,
    OMM_PEACH_VIBE_MUSIC_R96A_VOLUME_MOD_RAGE,
    OMM_PEACH_VIBE_MUSIC_R96A_VOLUME_MOD_GLOOM,
    OMM_PEACH_VIBE_MUSIC_R96A_VOLUME_MOD_CALM,
};

//
// Update
//

static void DynOS_Audio_Resample(u8 *aOutput, s32 aLength, const u8 *aInput, f32 aFreqModifier) {
    if (aFreqModifier != 1.f) {
        s32 outputSamples = aLength / (sizeof(s16) * MUSIC_CHANNELS);
        for (s32 sample = 0; sample != outputSamples; ++sample) {
            s32 sample0 = sample * aFreqModifier;
            s32 sample1 = sample0 + 1;
            f32 sampleF = ((f32) sample * aFreqModifier) - (f32) sample0;
            for (s32 channel = 0; channel != MUSIC_CHANNELS; ++channel) {
                ((s16 *) aOutput)[MUSIC_CHANNELS * sample + channel] =
                (((s16 *) aInput)[MUSIC_CHANNELS * sample0 + channel] * (1.f - sampleF)) +
                (((s16 *) aInput)[MUSIC_CHANNELS * sample1 + channel] * sampleF);
            }
        }
    } else {
        OMM_MEMCPY(aOutput, aInput, aLength);
    }
}

static void DynOS_Audio_Callback(UNUSED void *, u8 *aStream, s32 aLength) {
    bzero(aStream, aLength);
    if (sPlayingAudio) {
        f32 _Volume = clamp_f(sVibeAudioVolumeModifiers[*gOmmPeachVibeTypeRef] * sPlayingAudio->mVolume * (configMasterVolume / 127.f) * (configMusicVolume / 127.f) * softenAudioVolume * !configAudioMute, 0.f, 1.f);
        f32 _FreqMod = sVibeAudioFreqModifiers[*gOmmPeachVibeTypeRef];
        s32 _AudioLength = ALIGN_TO_SAMPLE(aLength * _FreqMod);
        s32 _AudioLenTilEnd = sPlayingAudio->mLength - sPlayingAudio->mCurrent;

        // Resample audio
        if (_AudioLenTilEnd < _AudioLength) {
            s32 _LenTilEnd = ALIGN_TO_SAMPLE(_AudioLenTilEnd / _FreqMod);
            DynOS_Audio_Resample(aStream, _LenTilEnd, sPlayingAudio->mData + sPlayingAudio->mCurrent, _FreqMod);
            if (sPlayingAudio->mLoop != 0) {
                DynOS_Audio_Resample(aStream + _LenTilEnd, aLength - _LenTilEnd, sPlayingAudio->mData + sPlayingAudio->mLoop, _FreqMod);
                sPlayingAudio->mCurrent = sPlayingAudio->mLoop + ALIGN_TO_SAMPLE((aLength - _LenTilEnd) * _FreqMod);
            } else {
                sPlayingAudio = NULL;
            }
        } else {
            DynOS_Audio_Resample(aStream, aLength, sPlayingAudio->mData + sPlayingAudio->mCurrent, _FreqMod);
            sPlayingAudio->mCurrent += _AudioLength;
        }

        // Mix audio
        DynOS_Audio_Mix(aStream, aStream, aLength, _Volume, 0);

        // Update current
        if (sPlayingAudio) {
#if !defined(sLoadedJingles)
            mCurrentMultiTrack = sPlayingAudio->mCurrent;
#endif
        }
    }
}

#endif
