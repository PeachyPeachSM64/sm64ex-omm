extern "C" {
#include "data/omm/omm_macros.h"
}
#if OMM_GAME_IS_R96X
#include "data/dynos.cpp.h"
extern "C" {
#include "sm64.h"

s32 gDDDBowsersSub = -1;
s32 gDDDPoles = -1;

void dynos_update_gfx() {}
void dynos_update_opt(UNUSED void *pad) {}

//
// Gfx
//

void *dynos_geo_get_graph_node(const void *geoLayout, bool keepInMemory) {
    return (void *) geo_layout_to_graph_node(NULL, (const GeoLayout *) geoLayout);
}

//
// Musics
//

void dynos_audio_init() {
    DynOS_Audio_Init();
}

bool dynos_music_load_presets_from_dynos_folder(const char *filename, const char *folder) {
    char filepath[256];
    char wavpath[256];
    snprintf(filepath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, filename);
    snprintf(wavpath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, folder);
    return DynOS_Music_LoadPresets(filepath, wavpath);
}

void dynos_music_play(const char *name, float volume_begin, float volume_end, s32 delay_time) {
    DynOS_Music_Play(name, volume_begin, volume_end, delay_time);
}

void dynos_music_multi_play(const char *name, float volume_begin, float volume_end, s32 delay_time) {
    DynOS_Music_Multi_Play(name, volume_begin, volume_end, delay_time);
}

void dynos_music_stop() {
    DynOS_Music_Stop();
}

void dynos_music_pause() {
    DynOS_Music_Pause();
}

void dynos_music_resume() {
    DynOS_Music_Resume();
}

void dynos_music_fade(bool end, float volume_begin, float volume_end, int delay_time) {
    DynOS_Music_Fade(end, volume_begin, volume_end, delay_time);
}

bool dynos_music_is_playing(const char *name) {
    return DynOS_Music_IsPlaying(name);
}

//
// Sounds
//

bool dynos_sound_load_presets_from_dynos_folder(const char *filename, const char *folder) {
    char filepath[256];
    char wavpath[256];
    snprintf(filepath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, filename);
    snprintf(wavpath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, folder);
    return DynOS_Sound_LoadPresets(filepath, wavpath);
}

void dynos_sound_play(const char *name, float *pos) {
    if (!omm_sound_play_character_sound_r96(name, pos)) {
        DynOS_Sound_Play(name, pos);
    }
}

void dynos_sound_stop(unsigned char bank) {
    DynOS_Sound_Stop(bank);
}

bool dynos_sound_is_playing(const char *name) {
    return DynOS_Sound_IsPlaying(name);
}

//
// Jingles
//

bool dynos_jingle_load_presets_from_dynos_folder(const char *filename, const char *folder) {
    char filepath[256];
    char wavpath[256];
    snprintf(filepath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, filename);
    snprintf(wavpath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, folder);
    return DynOS_Jingle_LoadPresets(filepath, wavpath);
}

void dynos_jingle_play(const char *name, float volume_begin, float volume_end, s32 delay_time) {
    DynOS_Jingle_Play(name, volume_begin, volume_end, delay_time);
}

void dynos_jingle_stop() {
    DynOS_Jingle_Stop();
}

void dynos_jingle_pause() {
    DynOS_Jingle_Pause();
}

void dynos_jingle_resume() {
    DynOS_Jingle_Resume();
}

void dynos_jingle_fade(bool end, float volume_begin, float volume_end, s32 delay_time) {
    DynOS_Jingle_Fade(end, volume_begin, volume_end, delay_time);
}

bool dynos_jingle_is_playing(const char *name) {
    return DynOS_Jingle_IsPlaying(name);
}

//
// Update
//

OMM_ROUTINE_UPDATE(dynos_audio_update) {

    // Fix the weird fade-in and the distorded music when collecting a Star
    if (dynos_jingle_is_playing(R96_EVENT_STAR_COLLECT) ||
        dynos_jingle_is_playing(R96_EVENT_KEY_COLLECT)) {
        dynos_music_stop();
    }
}

}
#endif
