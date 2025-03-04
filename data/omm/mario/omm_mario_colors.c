#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define _CUSTOM_ "_custom_"
#define MARIO_CUSTOM_ "mario" _CUSTOM_
#define PEACH_CUSTOM_ "peach" _CUSTOM_
#define MARIO_OFFSET (0x00)
#define PEACH_OFFSET (0x10)

OMM_INLINE Lights1 rgb24_to_lights1(u32 a, u32 l) {
    u8 ar = ((a >> 16) & 0xFF);
    u8 ag = ((a >>  8) & 0xFF);
    u8 ab = ((a >>  0) & 0xFF);
    u8 lr = ((l >> 16) & 0xFF);
    u8 lg = ((l >>  8) & 0xFF);
    u8 lb = ((l >>  0) & 0xFF);
    return (Lights1) gdSPDefLights1(
        ar, ag, ab,
        lr, lg, lb,
        40, 40, 40
    );
}

OMM_INLINE void lights1_to_rgb24(Lights1 lights1, u32 *a, u32 *l) {
    u32 ar = lights1.a.l.col[0];
    u32 ag = lights1.a.l.col[1];
    u32 ab = lights1.a.l.col[2];
    u32 lr = lights1.l->l.col[0];
    u32 lg = lights1.l->l.col[1];
    u32 lb = lights1.l->l.col[2];
    *a = ((ar << 16) | (ag << 8) | (ab << 0));
    *l = ((lr << 16) | (lg << 8) | (lb << 0));
}

//
// Mario colors
//

typedef struct {
    u32 cap[2];      // mario_cap_seg3_lights_0301CF38
    u32 hair[2];     // mario_brown2_lights_group
    u32 skin[2];     // mario_beige_lights_group
    u32 gloves[2];   // mario_white_lights_group
    u32 shirt[2];    // mario_red_lights_group
    u32 overalls[2]; // mario_blue_lights_group
    u32 shoes[2];    // mario_brown1_lights_group
    u32 cappy[2];    // omm_cappy_eye_top_light, omm_cappy_eye_bottom_light
    u32 emblem[2];   // mario_emblem_lights_group
} OmmMarioColors;

static OmmMarioColors sOmmMarioColorsPresets[] = {
      /*        Cap       */  /*       Hair       */  /*       Skin       */  /*      Gloves      */  /*       Shirt      */  /*     Overalls     */  /*       Shoes      */  /*       Cappy      */  /*      Emblem      */
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x7F0000, 0xFF0000 } }, // Default
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x7F0000, 0xFF0000 } }, // Vanilla
    { { 0x7C1824, 0xD82C2C }, { 0x390C00, 0x541C00 }, { 0xB4704C, 0xF0B88C }, { 0x7F7F7F, 0xFFFFFF }, { 0x7C1824, 0xD82C2C }, { 0x041C48, 0x2454B4 }, { 0x3C1800, 0x502814 }, { 0xFF0000, 0xFF4080 }, { 0x7C1824, 0xD82C2C } }, // Star Road
    { { 0x3F0000, 0xFF0000 }, { 0x1C0100, 0x730600 }, { 0x3F301E, 0xFEC179 }, { 0x3F3F3F, 0xFFFFFF }, { 0x3F0000, 0xFF0000 }, { 0x00003F, 0x0000FF }, { 0x1C0703, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x3F0000, 0xFF0000 } }, // Beta
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x7F0000, 0xFF0000 } }, // Mario
    { { 0x005500, 0x00AA00 }, { 0x3F0000, 0x7F0000 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x005500, 0x00AA00 }, { 0x000064, 0x0000C8 }, { 0x490000, 0x930000 }, { 0xFF0000, 0xFF4080 }, { 0x005500, 0x00AA00 } }, // Luigi
    { { 0x807000, 0xFFE000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x807000, 0xFFE000 }, { 0x501850, 0xA030A0 }, { 0x005500, 0x00AA00 }, { 0xFF0000, 0xFF4080 }, { 0x183074, 0x3060E8 } }, // Wario
    { { 0x401060, 0x8020C0 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x401060, 0x8020C0 }, { 0x1F1F1F, 0x3F3F3F }, { 0x7F3000, 0xFF6000 }, { 0xFF0000, 0xFF4080 }, { 0x7F7000, 0xFFE000 } }, // Waluigi
    { { 0x7F3C50, 0xFF78A0 }, { 0x7F7F0C, 0xFFFF18 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F0018, 0xFF0030 }, { 0xFF0000, 0xFF4080 }, { 0x7F3C50, 0xFF78A0 } }, // Peach
    { { 0x7F7818, 0xFFF030 }, { 0x603010, 0xC06020 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7818, 0xFFF030 }, { 0x7F3010, 0xFF6020 }, { 0x7F3010, 0xFF6020 }, { 0xFF0000, 0xFF4080 }, { 0x7F7818, 0xFFF030 } }, // Daisy
    { { 0x607F7F, 0xD0FFFF }, { 0x7F7850, 0xFFF0A0 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x607F7F, 0xD0FFFF }, { 0x407878, 0x80F0F0 }, { 0x407878, 0x80F0F0 }, { 0xFF0000, 0xFF4080 }, { 0x607F7F, 0xD0FFFF } }, // Rosalina
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x500000, 0xA00000 }, { 0x000000, 0x3F3F3F }, { 0xFF0000, 0xFF4080 }, { 0x7F0000, 0xFF0000 } }, // Pauline
    { { 0x786000, 0xF0C000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x786000, 0xF0C000 }, { 0x7F0000, 0xFF0000 }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x786000, 0xF0C000 } }, // Builder
    { { 0x7F7F7F, 0xFFFFFF }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x7F7F7F, 0xFFFFFF } }, // Fire Flower
    { { 0x00687F, 0x00D0FF }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00687F, 0x00D0FF }, { 0x7F0000, 0xFF0000 }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x00687F, 0x00D0FF } }, // Ice Flower
    { { 0x000000, 0x3F3F3F }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x000000, 0x3F3F3F }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x000000, 0x3F3F3F } }, // Red Star
    { { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFF0000, 0xFF4080 }, { 0xFFFFFF, 0x00A0C0 } }, // Galaxy Ice
    { { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0xFF0000, 0xFF4080 }, { 0x20FF20, 0x007000 } }, // Goo
    { { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFF0000, 0xFF4080 }, { 0xFFFF00, 0xFF0000 } }, // Lava
    { { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFF0000, 0xFF4080 }, { 0xFFE0FF, 0xB80040 } }, // Nebula
    { { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xFF0000, 0xFF4080 }, { 0xE0F0FF, 0x4080FF } }, // Crystal
    { { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xFF0000, 0xFF4080 }, { 0xA8402E, 0xFCBCAC } }, // Pink-Gold
    { { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0xFF0000, 0xFF4080 }, { 0x904818, 0xFFF040 } }, // Gold
    { { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0xFF0000, 0xFF4080 }, { 0x484848, 0xD0D0D0 } }, // Silver
    { { 0x7F7F7F, 0xFFFFFF }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0xFF0000, 0xFF4080 }, { 0x7F7F7F, 0xFFFFFF } }, // White
    { { 0x000000, 0x3F3F3F }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F }, { 0xFF0000, 0xFF4080 }, { 0x000000, 0x3F3F3F } }, // Black
    { { 0x100810, 0x201020 }, { 0x380840, 0x701080 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x100810, 0x201020 }, { 0x400820, 0x801040 }, { 0x7F0000, 0xFF0000 }, { 0xFF0000, 0xFF4080 }, { 0x100810, 0x201020 } }, // SonicDark
    { { 0xA244BD, 0xDA5EFF }, { 0x9A0025, 0xFF0046 }, { 0xF4AB64, 0xE3CB96 }, { 0xA0A0A0, 0xEEEEEE }, { 0xA244BD, 0xDA5EFF }, { 0x111111, 0x444444 }, { 0x592000, 0x8C4710 }, { 0xF000FF, 0x7A00FF }, { 0xA244BD, 0xDA5EFF } }, // JokerFactor
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x7F0000, 0xFF0000 } }, // Custom 1
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x7F0000, 0xFF0000 } }, // Custom 2
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x7F0000, 0xFF0000 } }, // Custom 3
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E }, { 0xFF0000, 0xFF4080 }, { 0x7F0000, 0xFF0000 } }, // Custom 4
};
static str_t sOmmMarioColorsCustoms[4] = {
    "Custom 1",
    "Custom 2",
    "Custom 3",
    "Custom 4",
};

#if OMM_MARIO_COLORS
extern Lights1 mario_brown2_lights_group;
extern Lights1 mario_beige_lights_group;
extern Lights1 mario_white_lights_group;
extern Lights1 mario_red_lights_group;
extern Lights1 mario_blue_lights_group;
extern Lights1 mario_brown1_lights_group;
#else
Lights1 mario_brown2_lights_group = gdSPDefLights1(0x39, 0x03, 0x00, 0x73, 0x06, 0x00, 0x28, 0x28, 0x28);
Lights1 mario_beige_lights_group  = gdSPDefLights1(0x7F, 0x60, 0x3C, 0xFE, 0xC1, 0x79, 0x28, 0x28, 0x28);
Lights1 mario_white_lights_group  = gdSPDefLights1(0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);
Lights1 mario_red_lights_group    = gdSPDefLights1(0x7F, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x28, 0x28, 0x28);
Lights1 mario_blue_lights_group   = gdSPDefLights1(0x00, 0x00, 0x7F, 0x00, 0x00, 0xFF, 0x28, 0x28, 0x28);
Lights1 mario_brown1_lights_group = gdSPDefLights1(0x39, 0x0E, 0x07, 0x72, 0x1C, 0x0E, 0x28, 0x28, 0x28);
#endif
#if OMM_CAP_COLORS
extern Lights1 mario_cap_seg3_lights_0301CF38;
extern Lights1 mario_cap_seg3_lights_0301CF08;
#else
Lights1 mario_cap_seg3_lights_0301CF38 = gdSPDefLights1(0x7F, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x28, 0x28, 0x28);
Lights1 mario_cap_seg3_lights_0301CF08 = gdSPDefLights1(0x39, 0x03, 0x00, 0x73, 0x06, 0x00, 0x28, 0x28, 0x28);
#endif
extern Lights1 omm_cappy_eye_top_light;
extern Lights1 omm_cappy_eye_bottom_light;

// Emblem (sm64coopdx)
Lights1 mario_emblem_lights_group = gdSPDefLights1(0x7F, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x28, 0x28, 0x28);

#if OMM_MARIO_COLORS

#define patch_offset(gfx, offset, value) ((Gfx *) gfx)[offset] = (Gfx) value

// Patch some Mario display lists
// - Differentiate shirt and cap colors
// - Use the proper color for the bottom of the cap
OMM_AT_STARTUP static void omm_mario_colors_patch_display_lists() {

    // mario_face_cap_on_dl
    patch_offset(mario_face_cap_on_dl, 0x1, gsSPLight(&mario_cap_seg3_lights_0301CF38.l, 1));
    patch_offset(mario_face_cap_on_dl, 0x2, gsSPLight(&mario_cap_seg3_lights_0301CF38.a, 2));

    // mario_right_hand_cap_dl
    static const Gfx mario_right_hand_cap_top_dl_fixed[] = {
        gsSPLight(&mario_cap_seg3_lights_0301CF38.l, 1),
        gsSPLight(&mario_cap_seg3_lights_0301CF38.a, 2),
        gsSPDisplayList(mario_right_hand_cap_top_dl),
        gsSPEndDisplayList(),
    };
    patch_offset(mario_right_hand_cap_dl, 0x0, gsSPDisplayList(mario_right_hand_cap_top_dl_fixed));
    patch_offset(mario_right_hand_cap_dl, 0x4, gsSPLight(&mario_cap_seg3_lights_0301CF08.l, 1));
    patch_offset(mario_right_hand_cap_dl, 0x5, gsSPLight(&mario_cap_seg3_lights_0301CF08.a, 2));

    // mario_m_logo_dl
    extern Vtx mario_m_logo_dl_vertex[];
    static const Gfx mario_m_logo_dl_fixed[] = {
        gsSPLight(&mario_cap_seg3_lights_0301CF38.l, 1),
        gsSPLight(&mario_cap_seg3_lights_0301CF38.a, 2),
        gsSPVertex(mario_m_logo_dl_vertex, 7, 0),
        gsSPEndDisplayList(),
    };
    patch_offset(mario_m_logo_dl, 0x0, gsSPDisplayList(mario_m_logo_dl_fixed));

    // mario_right_hand_cap_m_logo_dl
    extern Vtx mario_right_hand_cap_m_logo_dl_vertex[];
    static const Gfx mario_right_hand_cap_m_logo_dl_fixed[] = {
        gsSPLight(&mario_cap_seg3_lights_0301CF38.l, 1),
        gsSPLight(&mario_cap_seg3_lights_0301CF38.a, 2),
        gsSPVertex(mario_right_hand_cap_m_logo_dl_vertex, 5, 0),
        gsSPEndDisplayList(),
    };
    patch_offset(mario_right_hand_cap_m_logo_dl, 0x0, gsSPDisplayList(mario_right_hand_cap_m_logo_dl_fixed));
}

#endif

OMM_AT_STARTUP static void omm_mario_colors_init() {
    lights1_to_rgb24(mario_cap_seg3_lights_0301CF38, &sOmmMarioColorsPresets[0].cap[0],      &sOmmMarioColorsPresets[0].cap[1]);
    lights1_to_rgb24(mario_brown2_lights_group,      &sOmmMarioColorsPresets[0].hair[0],     &sOmmMarioColorsPresets[0].hair[1]);
    lights1_to_rgb24(mario_beige_lights_group,       &sOmmMarioColorsPresets[0].skin[0],     &sOmmMarioColorsPresets[0].skin[1]);
    lights1_to_rgb24(mario_white_lights_group,       &sOmmMarioColorsPresets[0].gloves[0],   &sOmmMarioColorsPresets[0].gloves[1]);
    lights1_to_rgb24(mario_red_lights_group,         &sOmmMarioColorsPresets[0].shirt[0],    &sOmmMarioColorsPresets[0].shirt[1]);
    lights1_to_rgb24(mario_blue_lights_group,        &sOmmMarioColorsPresets[0].overalls[0], &sOmmMarioColorsPresets[0].overalls[1]);
    lights1_to_rgb24(mario_brown1_lights_group,      &sOmmMarioColorsPresets[0].shoes[0],    &sOmmMarioColorsPresets[0].shoes[1]);
    lights1_to_rgb24(omm_cappy_eye_top_light,        &sOmmMarioColorsPresets[0].cappy[0],    &sOmmMarioColorsPresets[0].cappy[0]);
    lights1_to_rgb24(omm_cappy_eye_bottom_light,     &sOmmMarioColorsPresets[0].cappy[1],    &sOmmMarioColorsPresets[0].cappy[1]);
    lights1_to_rgb24(mario_cap_seg3_lights_0301CF38, &sOmmMarioColorsPresets[0].emblem[0],   &sOmmMarioColorsPresets[0].emblem[1]);
}

OMM_INLINE void omm_mario_colors_fill_color_from_palette_preset(u32 *color, const u32 *preset) {
    if (preset) {
        color[0] = (*preset >> 1) & 0x7F7F7F;
        color[1] = *preset;
    }
}

OMM_ROUTINE_PRE_RENDER(omm_mario_colors_update) {
    s32 paletteEditorCustom = omm_palette_editor_get_current_palette();
    gOmmExtrasMarioColors   = min_s(gOmmExtrasMarioColors, omm_mario_colors_count() - 1);
    OmmMarioColors colors   = sOmmMarioColorsPresets[paletteEditorCustom != -1 ? omm_mario_colors_count() - 4 + paletteEditorCustom : (s32) gOmmExtrasMarioColors];

    // Check CS palette preset
    if (gOmmCsPalettePreset && paletteEditorCustom == -1) {
        const u32 **palette = omm_models_cs_get_palette_preset(omm_models_cs_get_current_cs_index());
        if (palette) {
            omm_mario_colors_fill_color_from_palette_preset(colors.overalls, palette[0]);
            omm_mario_colors_fill_color_from_palette_preset(colors.shirt,    palette[1]);
            omm_mario_colors_fill_color_from_palette_preset(colors.gloves,   palette[2]);
            omm_mario_colors_fill_color_from_palette_preset(colors.shoes,    palette[3]);
            omm_mario_colors_fill_color_from_palette_preset(colors.hair,     palette[4]);
            omm_mario_colors_fill_color_from_palette_preset(colors.skin,     palette[5]);
            omm_mario_colors_fill_color_from_palette_preset(colors.cap,      palette[6]);
            omm_mario_colors_fill_color_from_palette_preset(colors.emblem,   palette[7]);
        }
    }

    mario_brown2_lights_group      = rgb24_to_lights1(colors.hair[0],     colors.hair[1]);
    mario_beige_lights_group       = rgb24_to_lights1(colors.skin[0],     colors.skin[1]);
    mario_white_lights_group       = rgb24_to_lights1(colors.gloves[0],   colors.gloves[1]);
    mario_red_lights_group         = rgb24_to_lights1(colors.shirt[0],    colors.shirt[1]);
    mario_blue_lights_group        = rgb24_to_lights1(colors.overalls[0], colors.overalls[1]);
    mario_brown1_lights_group      = rgb24_to_lights1(colors.shoes[0],    colors.shoes[1]);
    omm_cappy_eye_top_light        = rgb24_to_lights1(colors.cappy[0],    colors.cappy[0]);
    omm_cappy_eye_bottom_light     = rgb24_to_lights1(colors.cappy[1],    colors.cappy[1]);
    mario_emblem_lights_group      = rgb24_to_lights1(colors.emblem[0],   colors.emblem[1]);
    mario_cap_seg3_lights_0301CF38 = rgb24_to_lights1(colors.cap[0],      colors.cap[1]);
    mario_cap_seg3_lights_0301CF08 = rgb24_to_lights1(
        (colors.cap[0] >> 1) & 0x7F7F7F,
        (colors.cap[1] >> 1) & 0x7F7F7F
    );
}

//
// Peach colors
//

typedef struct {
    u32 skin[2];     // omm_peach_skin_light
    u32 hair[2];     // omm_peach_hair_light
    u32 dress1[2];   // omm_peach_dress_1_light
    u32 dress2[2];   // omm_peach_dress_2_light
    u32 gloves[2];   // omm_peach_glove_light
    u32 earrings[2]; // omm_peach_earring_light
    u32 shoes[2];    // omm_peach_shoe_light
    u32 tiara[2];    // omm_tiara_eye_top_light, omm_tiara_eye_bottom_light
} OmmPeachColors;

static OmmPeachColors sOmmPeachColorsPresets[] = {
      /*       Skin       */  /*       Hair       */  /*      Dress1      */  /*      Dress2      */  /*      Gloves      */  /*     Earrings     */  /*       Shoes      */  /*       Tiara      */
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Default
    { { 0x7F7352, 0xFFE6A5 }, { 0x7F5F0C, 0xFFBF18 }, { 0x7F635E, 0xFFC6BD }, { 0x773142, 0xEF6384 }, { 0x7F7F7F, 0xFFFFFF }, { 0x0B1A67, 0x1635CE }, { 0x703040, 0xE06080 }, { 0xFF0080, 0xFF40FF } }, // Vanilla
    { { 0x7F705B, 0xFFE1B6 }, { 0x7F6B0C, 0xFFD718 }, { 0x7F4F57, 0xFF9FAE }, { 0x6F2439, 0xDF4972 }, { 0x7F7F7F, 0xFFFFFF }, { 0x053D73, 0x0B7AE6 }, { 0x77182C, 0xEF3058 }, { 0xFF0080, 0xFF40FF } }, // Star Road
    { { 0x3F3929, 0xFFE6A5 }, { 0x3F2F06, 0xFFBF18 }, { 0x3F312F, 0xFFC6BD }, { 0x3B1821, 0xEF6384 }, { 0x3F3F3F, 0xFFFFFF }, { 0x050D33, 0x1635CE }, { 0x3B1821, 0xEF6384 }, { 0xFF0080, 0xFF40FF } }, // Beta
    { { 0x7F6E64, 0xFFDCC8 }, { 0x390300, 0x730600 }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F4040, 0xFF8080 }, { 0x390E07, 0x721C0E }, { 0xFF0080, 0xFF40FF } }, // Mario
    { { 0x7F6E64, 0xFFDCC8 }, { 0x3F0000, 0x7F0000 }, { 0x005500, 0x00AA00 }, { 0x000064, 0x0000C8 }, { 0x7F7F7F, 0xFFFFFF }, { 0x407040, 0x80E080 }, { 0x490000, 0x930000 }, { 0xFF0080, 0xFF40FF } }, // Luigi
    { { 0x7F6E64, 0xFFDCC8 }, { 0x390300, 0x730600 }, { 0x807000, 0xFFE000 }, { 0x501850, 0xA030A0 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F20, 0xFFFF40 }, { 0x005500, 0x00AA00 }, { 0xFF0080, 0xFF40FF } }, // Wario
    { { 0x7F6E64, 0xFFDCC8 }, { 0x390300, 0x730600 }, { 0x401060, 0x8020C0 }, { 0x1F1F1F, 0x3F3F3F }, { 0x7F7F7F, 0xFFFFFF }, { 0x60407F, 0xC080FF }, { 0x7F3000, 0xFF6000 }, { 0xFF0080, 0xFF40FF } }, // Waluigi
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Peach
    { { 0x7F6E64, 0xFFDCC8 }, { 0x603010, 0xC06020 }, { 0x7F7818, 0xFFF030 }, { 0x7F3010, 0xFF6020 }, { 0x7F7F7F, 0xFFFFFF }, { 0x007050, 0x00E0A0 }, { 0x7F3010, 0xFF6020 }, { 0xFF0080, 0xFF40FF } }, // Daisy
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F7850, 0xFFF0A0 }, { 0x407878, 0x80F0F0 }, { 0x607F7F, 0xD0FFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F40, 0xFFFF80 }, { 0x407878, 0x80F0F0 }, { 0xFF0080, 0xFF40FF } }, // Rosalina
    { { 0x7F6E64, 0xFFDCC8 }, { 0x390300, 0x730600 }, { 0x7F0000, 0xFF0000 }, { 0x500000, 0xA00000 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F6000, 0xFFC000 }, { 0x000000, 0x3F3F3F }, { 0xFF0080, 0xFF40FF } }, // Pauline
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x786000, 0xF0C000 }, { 0x7F0000, 0xFF0000 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Builder
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Fire Flower
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x00687F, 0x00D0FF }, { 0x004048, 0x008090 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Ice Flower
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x000000, 0x3F3F3F }, { 0x7F0000, 0xFF0000 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Red Star
    { { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFFFFFF, 0x00A0C0 }, { 0xFF0080, 0xFF40FF } }, // Galaxy Ice
    { { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0xFF0080, 0xFF40FF } }, // Goo
    { { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFF0080, 0xFF40FF } }, // Lava
    { { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFFE0FF, 0xB80040 }, { 0xFF0080, 0xFF40FF } }, // Nebula
    { { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xE0F0FF, 0x4080FF }, { 0xFF0080, 0xFF40FF } }, // Crystal
    { { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xA8402E, 0xFCBCAC }, { 0xFF0080, 0xFF40FF } }, // Pink-Gold
    { { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0x904818, 0xFFF040 }, { 0xFF0080, 0xFF40FF } }, // Gold
    { { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0x484848, 0xD0D0D0 }, { 0xFF0080, 0xFF40FF } }, // Silver
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F7F7F, 0xFFFFFF }, { 0xFF0080, 0xFF40FF } }, // White
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F }, { 0x00607F, 0x00C0FF }, { 0x000000, 0x3F3F3F }, { 0xFF0080, 0xFF40FF } }, // Black
    { { 0x7F6E64, 0xFFDCC8 }, { 0x000000, 0x3F3F3F }, { 0x400820, 0x801040 }, { 0x380840, 0x701080 }, { 0x404040, 0x808080 }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // SonicDark
    { { 0x7F6E64, 0xFFDCC8 }, { 0x9A0025, 0xFF0046 }, { 0x111111, 0x444444 }, { 0xA244BD, 0xDF89FF }, { 0x818181, 0xEEEEEE }, { 0x8D2D8D, 0xFF00E0 }, { 0x19007F, 0x4F4FFF }, { 0xD100FF, 0x5C00FF } }, // JokerFactor
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Custom 1
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Custom 2
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Custom 3
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 }, { 0xFF0080, 0xFF40FF } }, // Custom 4
};
static str_t sOmmPeachColorsCustoms[4] = {
    "Custom 1",
    "Custom 2",
    "Custom 3",
    "Custom 4",
};

extern Lights1 omm_peach_skin_light;
extern Lights1 omm_peach_hair_light;
extern Lights1 omm_peach_dress_1_light;
extern Lights1 omm_peach_dress_2_light;
extern Lights1 omm_peach_glove_light;
extern Lights1 omm_peach_earring_light;
extern Lights1 omm_peach_shoe_light;
extern Lights1 omm_tiara_eye_top_light;
extern Lights1 omm_tiara_eye_bottom_light;

OMM_AT_STARTUP static void omm_peach_colors_init() {
    lights1_to_rgb24(omm_peach_skin_light,       &sOmmPeachColorsPresets[0].skin[0],     &sOmmPeachColorsPresets[0].skin[1]);
    lights1_to_rgb24(omm_peach_hair_light,       &sOmmPeachColorsPresets[0].hair[0],     &sOmmPeachColorsPresets[0].hair[1]);
    lights1_to_rgb24(omm_peach_dress_1_light,    &sOmmPeachColorsPresets[0].dress1[0],   &sOmmPeachColorsPresets[0].dress1[1]);
    lights1_to_rgb24(omm_peach_dress_2_light,    &sOmmPeachColorsPresets[0].dress2[0],   &sOmmPeachColorsPresets[0].dress2[1]);
    lights1_to_rgb24(omm_peach_glove_light,      &sOmmPeachColorsPresets[0].gloves[0],   &sOmmPeachColorsPresets[0].gloves[1]);
    lights1_to_rgb24(omm_peach_earring_light,    &sOmmPeachColorsPresets[0].earrings[0], &sOmmPeachColorsPresets[0].earrings[1]);
    lights1_to_rgb24(omm_peach_shoe_light,       &sOmmPeachColorsPresets[0].shoes[0],    &sOmmPeachColorsPresets[0].shoes[1]);
    lights1_to_rgb24(omm_tiara_eye_top_light,    &sOmmPeachColorsPresets[0].tiara[0],    &sOmmPeachColorsPresets[0].tiara[0]);
    lights1_to_rgb24(omm_tiara_eye_bottom_light, &sOmmPeachColorsPresets[0].tiara[1],    &sOmmPeachColorsPresets[0].tiara[1]);
}

OMM_ROUTINE_PRE_RENDER(omm_peach_colors_update) {
    gOmmExtrasPeachColors        = min_s(gOmmExtrasPeachColors, omm_mario_colors_count() - 1);
    s32 paletteEditorCustom      = omm_palette_editor_get_current_palette();
    const OmmPeachColors *colors = &sOmmPeachColorsPresets[paletteEditorCustom != -1 ? omm_mario_colors_count() - 4 + paletteEditorCustom : (s32) gOmmExtrasPeachColors];
    omm_peach_skin_light         = rgb24_to_lights1(colors->skin[0],     colors->skin[1]);
    omm_peach_hair_light         = rgb24_to_lights1(colors->hair[0],     colors->hair[1]);
    omm_peach_dress_1_light      = rgb24_to_lights1(colors->dress1[0],   colors->dress1[1]);
    omm_peach_dress_2_light      = rgb24_to_lights1(colors->dress2[0],   colors->dress2[1]);
    omm_peach_glove_light        = rgb24_to_lights1(colors->gloves[0],   colors->gloves[1]);
    omm_peach_earring_light      = rgb24_to_lights1(colors->earrings[0], colors->earrings[1]);
    omm_peach_shoe_light         = rgb24_to_lights1(colors->shoes[0],    colors->shoes[1]);
    omm_tiara_eye_top_light      = rgb24_to_lights1(colors->tiara[0],    colors->tiara[0]);
    omm_tiara_eye_bottom_light   = rgb24_to_lights1(colors->tiara[1],    colors->tiara[1]);
}

//
// Read/Write
//

#define COLOR(_character_, _part_) sOmm##_character_##ColorsPresets[palette]._part_

static u32 *omm_mario_colors_get_color(s32 palette, s32 index, bool default_) {
    u32 *sOmmMarioColorsSaveFileDataMappings[][2] = {

        // Mario
        [MARIO_OFFSET + 0x00] = { COLOR(Mario, cap), NULL },
        [MARIO_OFFSET + 0x01] = { COLOR(Mario, hair), NULL },
        [MARIO_OFFSET + 0x02] = { COLOR(Mario, skin), NULL },
        [MARIO_OFFSET + 0x03] = { COLOR(Mario, gloves), NULL },
        [MARIO_OFFSET + 0x04] = { COLOR(Mario, overalls), NULL },
        [MARIO_OFFSET + 0x05] = { COLOR(Mario, shoes), NULL },
        [MARIO_OFFSET + 0x06] = { COLOR(Mario, cappy), NULL },
        [MARIO_OFFSET + 0x07] = { COLOR(Mario, shirt), COLOR(Mario, cap) },
        [MARIO_OFFSET + 0x08] = { COLOR(Mario, emblem), COLOR(Mario, cap) },
        [MARIO_OFFSET + 0x09] = { NULL, NULL },

        // Peach
        [PEACH_OFFSET + 0x00] = { COLOR(Peach, skin), NULL },
        [PEACH_OFFSET + 0x01] = { COLOR(Peach, hair), NULL },
        [PEACH_OFFSET + 0x02] = { COLOR(Peach, dress1), NULL },
        [PEACH_OFFSET + 0x03] = { COLOR(Peach, dress2), NULL },
        [PEACH_OFFSET + 0x04] = { COLOR(Peach, gloves), NULL },
        [PEACH_OFFSET + 0x05] = { COLOR(Peach, earrings), NULL },
        [PEACH_OFFSET + 0x06] = { COLOR(Peach, shoes), NULL },
        [PEACH_OFFSET + 0x07] = { COLOR(Peach, tiara), NULL },
        [PEACH_OFFSET + 0x08] = { NULL, NULL },
    };
    return sOmmMarioColorsSaveFileDataMappings[index][default_];
}

OMM_INLINE s32 char_to_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

bool omm_mario_colors_read(const char *name, const char *value1, const char *value2, bool *invalid) {
    if (strstr(name, _CUSTOM_)) {
        if (!*value1) {
            *invalid = true;
            return false;
        }

        // Check custom palette number
        s32 number = char_to_hex(name[13]);
        if (number < 1 || number > 4) {
            *invalid = true;
            return false;
        }

        // Get palette data
        s32 offset, palette = number + array_length(sOmmMarioColorsPresets) - 5;
        char *custom = NULL;
        if (strstr(name, MARIO_CUSTOM_) == name) {
            offset = MARIO_OFFSET;
            custom = (char *) sOmmMarioColorsCustoms[number - 1];
        } else if (strstr(name, PEACH_CUSTOM_) == name) {
            offset = PEACH_OFFSET;
            custom = (char *) sOmmPeachColorsCustoms[number - 1];
        } else {
            *invalid = true;
            return false;
        }

        // Read colors
        for (s32 index = 0;; ++index) {

            // Get color
            u32 *color = omm_mario_colors_get_color(palette, offset + index, false);
            if (!color) {
                break;
            }

            // Apply default color
            u32 *color0 = omm_mario_colors_get_color(palette, offset + index, true);
            if (color0) {
                color[0] = color0[0];
                color[1] = color0[1];
            }

            // Read color
            for (s32 i = 0; i != 12; ++i) {

                // Check char
                s32 h = char_to_hex(*value1);
                if (h == -1) {
                    break;
                }

                // Fill color
                u32 shift = (4 * (5 - (i % 6)));
                color[i / 6] &= ~(0xF << shift);
                color[i / 6] |= (h << shift);
                value1++;
            }
        }

        // Fill name
        s32 length = strlen(value2);
        if (length) {
            mem_cpy(custom, value2, min_s(length + 1, sizeof(str_t)));
        } else {
            str_fmt(custom, sizeof(str_t), "Custom %d", number);
        }
        return true;
    }
    return false;
}

static void omm_mario_colors_write_colors(const char *section, const char *key, const str_t *customs, s32 offset) {
    omm_save_file_write_buffer("%s\n", section);
    for (s32 i = 0; i != 4; ++i) {
        omm_save_file_write_buffer("%s%d = ", key, i + 1);
        s32 palette = i + array_length(sOmmMarioColorsPresets) - 4;
        for (s32 index = 0;; ++index) {
            u32 *color = omm_mario_colors_get_color(palette, offset + index, false);
            if (!color) break;
            omm_save_file_write_buffer("%06X%06X", color[0], color[1]);
        }
        const char *name = customs[i];
        if (*name) {
            omm_save_file_write_buffer(" \"%s\"\n", name);
        } else {
            omm_save_file_write_buffer(" \"Custom %d\"\n", i + 1);
        }
    }
    omm_save_file_write_buffer("\n");
}

void omm_mario_colors_write() {
    omm_mario_colors_write_colors(OMM_SAVE_FILE_SECTION_MARIO_COLORS, MARIO_CUSTOM_, sOmmMarioColorsCustoms, MARIO_OFFSET);
    omm_mario_colors_write_colors(OMM_SAVE_FILE_SECTION_PEACH_COLORS, PEACH_CUSTOM_, sOmmPeachColorsCustoms, PEACH_OFFSET);
}

void omm_mario_colors_reset(bool peach) {
    for (s32 i = 0; i != 4; ++i) {
        if (peach) {
            sOmmPeachColorsPresets[i + array_length(sOmmPeachColorsPresets) - 4] = sOmmPeachColorsPresets[0];
            str_fmt(sOmmPeachColorsCustoms[i], sizeof(str_t), "Custom %d", i + 1);
        } else {
            sOmmMarioColorsPresets[i + array_length(sOmmMarioColorsPresets) - 4] = sOmmMarioColorsPresets[0];
            str_fmt(sOmmMarioColorsCustoms[i], sizeof(str_t), "Custom %d", i + 1);
        }
    }
}

//
// Options menu
//

const char **omm_mario_colors_choices(bool peach) {
    static const char *sOmmMarioColorsChoices[][32] = { {
        OMM_TEXT_OPT_MARIO_COLORS_DEFAULT,
        OMM_TEXT_OPT_MARIO_COLORS_SM64,
        OMM_TEXT_OPT_MARIO_COLORS_STAR_ROAD,
        OMM_TEXT_OPT_MARIO_COLORS_BETA,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO,
        OMM_TEXT_OPT_MARIO_COLORS_LUIGI,
        OMM_TEXT_OPT_MARIO_COLORS_WARIO,
        OMM_TEXT_OPT_MARIO_COLORS_WALUIGI,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH,
        OMM_TEXT_OPT_MARIO_COLORS_DAISY,
        OMM_TEXT_OPT_MARIO_COLORS_ROSALINA,
        OMM_TEXT_OPT_MARIO_COLORS_PAULINE,
        OMM_TEXT_OPT_MARIO_COLORS_BUILDER,
        OMM_TEXT_OPT_MARIO_COLORS_FIRE_FLOWER,
        OMM_TEXT_OPT_MARIO_COLORS_ICE_FLOWER,
        OMM_TEXT_OPT_MARIO_COLORS_RED_STAR,
        OMM_TEXT_OPT_MARIO_COLORS_GALAXY_ICE,
        OMM_TEXT_OPT_MARIO_COLORS_GOO,
        OMM_TEXT_OPT_MARIO_COLORS_LAVA,
        OMM_TEXT_OPT_MARIO_COLORS_NEBULA,
        OMM_TEXT_OPT_MARIO_COLORS_CRYSTAL,
        OMM_TEXT_OPT_MARIO_COLORS_PINK_GOLD,
        OMM_TEXT_OPT_MARIO_COLORS_GOLD,
        OMM_TEXT_OPT_MARIO_COLORS_SILVER,
        OMM_TEXT_OPT_MARIO_COLORS_WHITE,
        OMM_TEXT_OPT_MARIO_COLORS_BLACK,
        OMM_TEXT_OPT_MARIO_COLORS_SONICDARK,
        OMM_TEXT_OPT_MARIO_COLORS_JOKERFACTOR,
        sOmmMarioColorsCustoms[0],
        sOmmMarioColorsCustoms[1],
        sOmmMarioColorsCustoms[2],
        sOmmMarioColorsCustoms[3],
    }, {
        OMM_TEXT_OPT_MARIO_COLORS_DEFAULT,
        OMM_TEXT_OPT_MARIO_COLORS_SM64,
        OMM_TEXT_OPT_MARIO_COLORS_STAR_ROAD,
        OMM_TEXT_OPT_MARIO_COLORS_BETA,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO,
        OMM_TEXT_OPT_MARIO_COLORS_LUIGI,
        OMM_TEXT_OPT_MARIO_COLORS_WARIO,
        OMM_TEXT_OPT_MARIO_COLORS_WALUIGI,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH,
        OMM_TEXT_OPT_MARIO_COLORS_DAISY,
        OMM_TEXT_OPT_MARIO_COLORS_ROSALINA,
        OMM_TEXT_OPT_MARIO_COLORS_PAULINE,
        OMM_TEXT_OPT_MARIO_COLORS_BUILDER,
        OMM_TEXT_OPT_MARIO_COLORS_FIRE_FLOWER,
        OMM_TEXT_OPT_MARIO_COLORS_ICE_FLOWER,
        OMM_TEXT_OPT_MARIO_COLORS_RED_STAR,
        OMM_TEXT_OPT_MARIO_COLORS_GALAXY_ICE,
        OMM_TEXT_OPT_MARIO_COLORS_GOO,
        OMM_TEXT_OPT_MARIO_COLORS_LAVA,
        OMM_TEXT_OPT_MARIO_COLORS_NEBULA,
        OMM_TEXT_OPT_MARIO_COLORS_CRYSTAL,
        OMM_TEXT_OPT_MARIO_COLORS_PINK_GOLD,
        OMM_TEXT_OPT_MARIO_COLORS_GOLD,
        OMM_TEXT_OPT_MARIO_COLORS_SILVER,
        OMM_TEXT_OPT_MARIO_COLORS_WHITE,
        OMM_TEXT_OPT_MARIO_COLORS_BLACK,
        OMM_TEXT_OPT_MARIO_COLORS_SONICDARK,
        OMM_TEXT_OPT_MARIO_COLORS_JOKERFACTOR,
        sOmmPeachColorsCustoms[0],
        sOmmPeachColorsCustoms[1],
        sOmmPeachColorsCustoms[2],
        sOmmPeachColorsCustoms[3],
    } };
    return sOmmMarioColorsChoices[peach];
}

s32 omm_mario_colors_count() {
    return array_length(sOmmMarioColorsPresets);
}

//
// Palette editor
//

s32 omm_mario_colors_lights_count(bool peach) {
    if (peach) return sizeof(OmmPeachColors) / sizeof(u32);
    return sizeof(OmmMarioColors) / sizeof(u32);
}

s32 omm_mario_colors_light_groups_count(bool peach) {
    return omm_mario_colors_lights_count(peach) / 2;
}

u32 *omm_mario_colors_get_light(bool peach, s32 palette, s32 index) {
    if (peach) {
        switch (index) {
            case 0x00: return &sOmmPeachColorsPresets[palette].skin[0];
            case 0x01: return &sOmmPeachColorsPresets[palette].skin[1];
            case 0x02: return &sOmmPeachColorsPresets[palette].hair[0];
            case 0x03: return &sOmmPeachColorsPresets[palette].hair[1];
            case 0x04: return &sOmmPeachColorsPresets[palette].dress1[0];
            case 0x05: return &sOmmPeachColorsPresets[palette].dress1[1];
            case 0x06: return &sOmmPeachColorsPresets[palette].dress2[0];
            case 0x07: return &sOmmPeachColorsPresets[palette].dress2[1];
            case 0x08: return &sOmmPeachColorsPresets[palette].gloves[0];
            case 0x09: return &sOmmPeachColorsPresets[palette].gloves[1];
            case 0x0A: return &sOmmPeachColorsPresets[palette].earrings[0];
            case 0x0B: return &sOmmPeachColorsPresets[palette].earrings[1];
            case 0x0C: return &sOmmPeachColorsPresets[palette].shoes[0];
            case 0x0D: return &sOmmPeachColorsPresets[palette].shoes[1];
            case 0x0E: return &sOmmPeachColorsPresets[palette].tiara[0];
            case 0x0F: return &sOmmPeachColorsPresets[palette].tiara[1];
        }
    } else {
        switch (index) {
            case 0x00: return &sOmmMarioColorsPresets[palette].cap[0];
            case 0x01: return &sOmmMarioColorsPresets[palette].cap[1];
            case 0x02: return &sOmmMarioColorsPresets[palette].hair[0];
            case 0x03: return &sOmmMarioColorsPresets[palette].hair[1];
            case 0x04: return &sOmmMarioColorsPresets[palette].skin[0];
            case 0x05: return &sOmmMarioColorsPresets[palette].skin[1];
            case 0x06: return &sOmmMarioColorsPresets[palette].gloves[0];
            case 0x07: return &sOmmMarioColorsPresets[palette].gloves[1];
            case 0x08: return &sOmmMarioColorsPresets[palette].shirt[0];
            case 0x09: return &sOmmMarioColorsPresets[palette].shirt[1];
            case 0x0A: return &sOmmMarioColorsPresets[palette].overalls[0];
            case 0x0B: return &sOmmMarioColorsPresets[palette].overalls[1];
            case 0x0C: return &sOmmMarioColorsPresets[palette].shoes[0];
            case 0x0D: return &sOmmMarioColorsPresets[palette].shoes[1];
            case 0x0E: return &sOmmMarioColorsPresets[palette].cappy[0];
            case 0x0F: return &sOmmMarioColorsPresets[palette].cappy[1];
            case 0x10: return &sOmmMarioColorsPresets[palette].emblem[0];
            case 0x11: return &sOmmMarioColorsPresets[palette].emblem[1];
        }
    }
    return NULL;
}

const char *omm_mario_colors_light_name(bool peach, s32 index) {
    static const char *sOmmMarioColorsLightNames[][MAX(sizeof(OmmMarioColors), sizeof(OmmPeachColors)) / sizeof(u32)] = { {
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_CAP_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_CAP_2,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_HAIR_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_HAIR_2,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SKIN_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SKIN_2,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_GLOVES_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_GLOVES_2,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SHIRT_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SHIRT_2,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_OVERALLS_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_OVERALLS_2,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SHOES_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SHOES_2,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_CAPPY_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_CAPPY_2,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_EMBLEM_1,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_EMBLEM_2,
    }, {
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_SKIN_1,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_SKIN_2,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_HAIR_1,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_HAIR_2,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_DRESS_1,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_DRESS_2,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_HIPS_1,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_HIPS_2,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_GLOVES_1,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_GLOVES_2,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_EARRINGS_1,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_EARRINGS_2,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_SHOES_1,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_SHOES_2,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_TIARA_1,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_TIARA_2,
    } };
    return sOmmMarioColorsLightNames[peach][index];
}

const char *omm_mario_colors_light_group_name(bool peach, s32 index) {
    static const char *sOmmMarioColorsLightGroupNames[][MAX(sizeof(OmmMarioColors), sizeof(OmmPeachColors)) / (2 * sizeof(u32))] = { {
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_CAP,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_HAIR,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SKIN,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_GLOVES,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SHIRT,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_OVERALLS,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_SHOES,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_CAPPY,
        OMM_TEXT_OPT_MARIO_COLORS_MARIO_LIGHT_EMBLEM,
    }, {
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_SKIN,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_HAIR,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_DRESS,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_HIPS,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_GLOVES,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_EARRINGS,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_SHOES,
        OMM_TEXT_OPT_MARIO_COLORS_PEACH_LIGHT_TIARA,
    } };
    return sOmmMarioColorsLightGroupNames[peach][index];
}

//
// Color palettes from sm64coopdx
//

const void *omm_mario_colors_part_to_light(s32 part) {
    static const Lights1 *sOmmMarioLights[] = {
        &mario_blue_lights_group, // PANTS
        &mario_red_lights_group, // SHIRT
        &mario_white_lights_group, // GLOVES
        &mario_brown1_lights_group, // SHOES
        &mario_brown2_lights_group, // HAIR
        &mario_beige_lights_group, // SKIN
        &mario_cap_seg3_lights_0301CF38, // CAP
        &mario_emblem_lights_group, // EMBLEM
    };
    if (part >= 0 && part < (s32) array_length(sOmmMarioLights)) {
        return (const void *) sOmmMarioLights[part];
    }
    return NULL;
}
