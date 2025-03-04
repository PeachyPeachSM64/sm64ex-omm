#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

typedef struct {
    const char *texture;
    u32 sound;
} OmmCakeEnding;

static const OmmCakeEnding OMM_CAKE_ENDINGS[][4] = {

  ////////////////
 // Yoshi mode //
////////////////
{
    { OMM_TEXTURE_ENDING_YOSHI, SOUND_GENERAL_YOSHI_TALK }, // Mario
    { OMM_TEXTURE_ENDING_YOSHI, SOUND_GENERAL_YOSHI_TALK }, // Peach
    { OMM_TEXTURE_ENDING_YOSHI, SOUND_GENERAL_YOSHI_TALK }, // Luigi
    { OMM_TEXTURE_ENDING_YOSHI, SOUND_GENERAL_YOSHI_TALK }, // Wario
},

#if OMM_GAME_IS_SM64

  ////////////////
 // Bad ending //
////////////////
{
    { OMM_TEXTURE_ENDING_0_MARIO, SOUND_OBJ_BOWSER_LAUGH }, // Mario
    { OMM_TEXTURE_ENDING_0_MARIO, SOUND_OBJ_BOWSER_LAUGH }, // Peach
    { OMM_TEXTURE_ENDING_0_LUIGI, SOUND_OBJ_BOWSER_LAUGH }, // Luigi
    { OMM_TEXTURE_ENDING_0_WARIO, SOUND_OBJ_BOWSER_LAUGH }, // Wario
},

  /////////////////
 // True ending //
/////////////////
{
    { OMM_TEXTURE_ENDING_1, SOUND_PEACH_THANK_YOU_MARIO }, // Mario
    { OMM_TEXTURE_ENDING_1, SOUND_PEACH_THANK_YOU_MARIO }, // Peach
    { OMM_TEXTURE_ENDING_1, SOUND_PEACH_THANK_YOU_MARIO }, // Luigi
    { OMM_TEXTURE_ENDING_1, SOUND_PEACH_THANK_YOU_MARIO }, // Wario
},

#endif

};

static const OmmCakeEnding *omm_get_cake_ending() {
    s32 playerIndex = omm_player_get_selected_index_model_and_sounds();
    if (gOmmGlobals->yoshiMode) {
        return &OMM_CAKE_ENDINGS[0][playerIndex];
    }
#if OMM_GAME_IS_SM64
    if (gOmmSparklyEnding) {
        return &OMM_CAKE_ENDINGS[gOmmSparklyEnding][playerIndex];
    }
#endif
    return NULL;
}

void omm_render_cake_ending_screen() {
    const OmmCakeEnding *ending = omm_get_cake_ending();
    if (ending) {
        const Gfx gfx[] = {
            gsSPDisplayList(dl_proj_mtx_fullscreen),
            gsSPDisplayList(dl_hud_img_begin),
            gsDPLoadTextureBlock(ending->texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 0, 0, 0, 0),
            gsSPTextureRectangle(0, 0, SCREEN_WIDTH << 2, SCREEN_HEIGHT << 2, G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10),
            gsSPDisplayList(dl_hud_img_end),
            gsSPEndDisplayList(),
        };
        gSPDisplayList(gDisplayListHead++, mem_cpy(omm_alloc_gfx(array_length(gfx)), gfx, sizeof(gfx)));
    }
}

bool omm_render_cake_ending_screen_play_sound() {
    const OmmCakeEnding *ending = omm_get_cake_ending();
    if (ending) {
        u32 bankIndex = (ending->sound & SOUNDARGS_MASK_BANK) >> SOUNDARGS_SHIFT_BANK;
        sound_banks_enable(SEQ_PLAYER_SFX, 1 << bankIndex);
        play_sound(ending->sound, gGlobalSoundArgs);
        return true;
    }
    return false;
}
