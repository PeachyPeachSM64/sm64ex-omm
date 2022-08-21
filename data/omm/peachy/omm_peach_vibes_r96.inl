#if OMM_GAME_IS_R96X
extern "C" {

#include "data/omm/omm_includes.h"

static void omm_peach_vibe_audio_set_freq_and_volume(void *p, ma_sound *audio, f32 *volume, f32 freqMod, f32 volumeMod) {
    static OmmMap sAudioMap = omm_map_zero;
    s32 i = omm_map_find_key(sAudioMap, ptr, p);
    if (i == -1) {
        omm_map_add(sAudioMap, ptr, p, f32, *volume);
    } else {
        *volume = omm_map_get_val(sAudioMap, f32, i);
    }
    ma_sound_set_pitch(audio, freqMod);
    *volume *= volumeMod;
}

OMM_ROUTINE_PRE_RENDER(omm_peach_vibe_update_audio) {
    if (OMM_PLAYER_IS_PEACH) {
        f32 freqMod, volumeMod;
        switch (gOmmPeach->vibeType) {
            case OMM_PEACH_VIBE_TYPE_JOY:   freqMod = OMM_PEACH_VIBE_MUSIC_R96X_FREQ_MOD_JOY;   volumeMod = OMM_PEACH_VIBE_MUSIC_R96X_VOLUME_MOD_JOY;   break;
            case OMM_PEACH_VIBE_TYPE_RAGE:  freqMod = OMM_PEACH_VIBE_MUSIC_R96X_FREQ_MOD_RAGE;  volumeMod = OMM_PEACH_VIBE_MUSIC_R96X_VOLUME_MOD_RAGE;  break;
            case OMM_PEACH_VIBE_TYPE_GLOOM: freqMod = OMM_PEACH_VIBE_MUSIC_R96X_FREQ_MOD_GLOOM; volumeMod = OMM_PEACH_VIBE_MUSIC_R96X_VOLUME_MOD_GLOOM; break;
            case OMM_PEACH_VIBE_TYPE_CALM:  freqMod = OMM_PEACH_VIBE_MUSIC_R96X_FREQ_MOD_CALM;  volumeMod = OMM_PEACH_VIBE_MUSIC_R96X_VOLUME_MOD_CALM;  break;
            case OMM_PEACH_VIBE_TYPE_NONE:  freqMod = 1.f;                                      volumeMod = 1.f;                                        break;
        }
        if (sPlayingMusic) {
            omm_peach_vibe_audio_set_freq_and_volume((void *) sPlayingMusic, &sPlayingMusic->mAudio, &sPlayingMusic->mVolume, freqMod, volumeMod);
        }
        if (sPlayingJingle) {
            omm_peach_vibe_audio_set_freq_and_volume((void *) sPlayingJingle, &sPlayingJingle->mAudio, &sPlayingJingle->mVolume, freqMod, volumeMod);
        }
    }
}

}
#endif
