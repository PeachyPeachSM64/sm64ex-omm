#ifndef OMM_THREAD_H
#define OMM_THREAD_H

#include "types.h"
#include <pthread.h>

u64  gfx_texture_precache_get_size();
void gfx_texture_precache_start(u64 *loaded_bytes);
void gfx_texture_precache_end();

void rom_asset_precache_textures(bool (*func)(const char *, s32, s32, u8 *));
u8  *rom_asset_load_texture(const char *name, s32 *width, s32 *height);
u8  *rom_asset_load_sound_data(const char *name);
void rom_asset_copy_baserom_to_appdata();

u64  omm_models_precache_get_size();

#endif
