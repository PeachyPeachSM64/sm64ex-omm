#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

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
    u32 cap[2];      // mario_red_lights_group + mario_cap_seg3_lights_0301CF38
    u32 hair[2];     // mario_brown2_lights_group
    u32 skin[2];     // mario_beige_lights_group
    u32 gloves[2];   // mario_white_lights_group
    u32 overalls[2]; // mario_blue_lights_group
    u32 shoes[2];    // mario_brown1_lights_group
} OmmMarioColors;

static OmmMarioColors sOmmMarioColorsPresets[] = {
      /*        Cap       */  /*       Hair       */  /*       Skin       */  /*      Gloves      */  /*     Overalls     */  /*       Shoes      */
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E } }, // Default
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E } }, // Vanilla
    { { 0x7C1824, 0xD82C2C }, { 0x390C00, 0x541C00 }, { 0xB4704C, 0xF0B88C }, { 0x7F7F7F, 0xFFFFFF }, { 0x041C48, 0x2454B4 }, { 0x3C1800, 0x502814 } }, // Star Road
    { { 0x3F0000, 0xFF0000 }, { 0x1C0100, 0x730600 }, { 0x3F301E, 0xFEC179 }, { 0x3F3F3F, 0xFFFFFF }, { 0x00003F, 0x0000FF }, { 0x1C0703, 0x721C0E } }, // Beta
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E } }, // Mario
    { { 0x005500, 0x00AA00 }, { 0x3F0000, 0x7F0000 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x000064, 0x0000C8 }, { 0x490000, 0x930000 } }, // Luigi
    { { 0x808000, 0xFFFF00 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x501850, 0xA030A0 }, { 0x005500, 0x00AA00 } }, // Wario
    { { 0x401060, 0x8020C0 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x1F1F1F, 0x3F3F3F }, { 0x7F3000, 0xFF6000 } }, // Waluigi
    { { 0x7F3C50, 0xFF78A0 }, { 0x7F7F0C, 0xFFFF18 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x681830, 0xD03060 }, { 0x7F0018, 0xFF0030 } }, // Peach
    { { 0x7F7818, 0xFFF030 }, { 0x603010, 0xC06020 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F3010, 0xFF6020 }, { 0x7F3010, 0xFF6020 } }, // Daisy
    { { 0x607F7F, 0xD0FFFF }, { 0x7F7850, 0xFFF0A0 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x407878, 0x80F0F0 }, { 0x407878, 0x80F0F0 } }, // Rosalina
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x500000, 0xA00000 }, { 0x000000, 0x3F3F3F } }, // Pauline
    { { 0x786000, 0xF0C000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x390E07, 0x721C0E } }, // Builder
    { { 0x7F7F7F, 0xFFFFFF }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x390E07, 0x721C0E } }, // Fire Flower
    { { 0x00687F, 0x00D0FF }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x390E07, 0x721C0E } }, // Ice Flower
    { { 0x000000, 0x3F3F3F }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x390E07, 0x721C0E } }, // Red Star
    { { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 } }, // Galaxy Ice
    { { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 } }, // Goo
    { { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 } }, // Lava
    { { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 } }, // Nebula
    { { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF } }, // Crystal
    { { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 } }, // Pink Gold
    { { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 } }, // Gold
    { { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 } }, // Silver
    { { 0x7F7F7F, 0xFFFFFF }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF } }, // White
    { { 0x000000, 0x3F3F3F }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F } }, // Black
    { { 0x00007F, 0x0000FF }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x390E07, 0x721C0E } }, // SMG4
    { { 0x100810, 0x201020 }, { 0x380840, 0x701080 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x400820, 0x801040 }, { 0x7F0000, 0xFF0000 } }, // SonicDark
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E } }, // Custom 1
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E } }, // Custom 2
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E } }, // Custom 3
    { { 0x7F0000, 0xFF0000 }, { 0x390300, 0x730600 }, { 0x7F603C, 0xFEC179 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00007F, 0x0000FF }, { 0x390E07, 0x721C0E } }, // Custom 4
};

#if OMM_CODE_MARIO_COLORS
extern Lights1 mario_red_lights_group;
extern Lights1 mario_brown2_lights_group;
extern Lights1 mario_beige_lights_group;
extern Lights1 mario_white_lights_group;
extern Lights1 mario_blue_lights_group;
extern Lights1 mario_brown1_lights_group;
extern Lights1 mario_cap_seg3_lights_0301CF38;
extern Lights1 mario_cap_seg3_lights_0301CF08;

OMM_AT_STARTUP static void omm_mario_colors_init() {
    lights1_to_rgb24(mario_red_lights_group,    &sOmmMarioColorsPresets[0].cap[0],      &sOmmMarioColorsPresets[0].cap[1]);
    lights1_to_rgb24(mario_brown2_lights_group, &sOmmMarioColorsPresets[0].hair[0],     &sOmmMarioColorsPresets[0].hair[1]);
    lights1_to_rgb24(mario_beige_lights_group,  &sOmmMarioColorsPresets[0].skin[0],     &sOmmMarioColorsPresets[0].skin[1]);
    lights1_to_rgb24(mario_white_lights_group,  &sOmmMarioColorsPresets[0].gloves[0],   &sOmmMarioColorsPresets[0].gloves[1]);
    lights1_to_rgb24(mario_blue_lights_group,   &sOmmMarioColorsPresets[0].overalls[0], &sOmmMarioColorsPresets[0].overalls[1]);
    lights1_to_rgb24(mario_brown1_lights_group, &sOmmMarioColorsPresets[0].shoes[0],    &sOmmMarioColorsPresets[0].shoes[1]);
}

OMM_ROUTINE_PRE_RENDER(omm_mario_colors_update) {
    gOmmExtrasMarioColors          = min_s(gOmmExtrasMarioColors, omm_mario_colors_count() - 1);
    mario_red_lights_group         = rgb24_to_lights1(sOmmMarioColorsPresets[gOmmExtrasMarioColors].cap[0],      sOmmMarioColorsPresets[gOmmExtrasMarioColors].cap[1]);
    mario_brown2_lights_group      = rgb24_to_lights1(sOmmMarioColorsPresets[gOmmExtrasMarioColors].hair[0],     sOmmMarioColorsPresets[gOmmExtrasMarioColors].hair[1]);
    mario_beige_lights_group       = rgb24_to_lights1(sOmmMarioColorsPresets[gOmmExtrasMarioColors].skin[0],     sOmmMarioColorsPresets[gOmmExtrasMarioColors].skin[1]);
    mario_white_lights_group       = rgb24_to_lights1(sOmmMarioColorsPresets[gOmmExtrasMarioColors].gloves[0],   sOmmMarioColorsPresets[gOmmExtrasMarioColors].gloves[1]);
    mario_blue_lights_group        = rgb24_to_lights1(sOmmMarioColorsPresets[gOmmExtrasMarioColors].overalls[0], sOmmMarioColorsPresets[gOmmExtrasMarioColors].overalls[1]);
    mario_brown1_lights_group      = rgb24_to_lights1(sOmmMarioColorsPresets[gOmmExtrasMarioColors].shoes[0],    sOmmMarioColorsPresets[gOmmExtrasMarioColors].shoes[1]);
    mario_cap_seg3_lights_0301CF38 = rgb24_to_lights1(sOmmMarioColorsPresets[gOmmExtrasMarioColors].cap[0],      sOmmMarioColorsPresets[gOmmExtrasMarioColors].cap[1]);
    mario_cap_seg3_lights_0301CF08 = rgb24_to_lights1(
        (sOmmMarioColorsPresets[gOmmExtrasMarioColors].cap[0] >> 1) & 0x7F7F7F,
        (sOmmMarioColorsPresets[gOmmExtrasMarioColors].cap[1] >> 1) & 0x7F7F7F
    );
}
#endif

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
} OmmPeachColors;

static OmmPeachColors sOmmPeachColorsPresets[] = {
      /*       Skin       */  /*       Hair       */  /*      Dress1      */  /*      Dress2      */  /*      Gloves      */  /*     Earrings     */  /*       Shoes      */
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Default
    { { 0x7F7352, 0xFFE6A5 }, { 0x7F5F0C, 0xFFBF18 }, { 0x7F635E, 0xFFC6BD }, { 0x773142, 0xEF6384 }, { 0x7F7F7F, 0xFFFFFF }, { 0x0B1A67, 0x1635CE }, { 0x703040, 0xE06080 } }, // Vanilla
    { { 0x7F705B, 0xFFE1B6 }, { 0x7F6B0C, 0xFFD718 }, { 0x7F4F57, 0xFF9FAE }, { 0x6F2439, 0xDF4972 }, { 0x7F7F7F, 0xFFFFFF }, { 0x053D73, 0x0B7AE6 }, { 0x77182C, 0xEF3058 } }, // Star Road
    { { 0x3F3929, 0xFFE6A5 }, { 0x3F2F06, 0xFFBF18 }, { 0x3F312F, 0xFFC6BD }, { 0x3B1821, 0xEF6384 }, { 0x3F3F3F, 0xFFFFFF }, { 0x050D33, 0x1635CE }, { 0x3B1821, 0xEF6384 } }, // Beta
    { { 0x7F6E64, 0xFFDCC8 }, { 0x390300, 0x730600 }, { 0x7F0000, 0xFF0000 }, { 0x00007F, 0x0000FF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F4040, 0xFF8080 }, { 0x390E07, 0x721C0E } }, // Mario
    { { 0x7F6E64, 0xFFDCC8 }, { 0x3F0000, 0x7F0000 }, { 0x005500, 0x00AA00 }, { 0x000064, 0x0000C8 }, { 0x7F7F7F, 0xFFFFFF }, { 0x407040, 0x80E080 }, { 0x490000, 0x930000 } }, // Luigi
    { { 0x7F6E64, 0xFFDCC8 }, { 0x390300, 0x730600 }, { 0x808000, 0xFFFF00 }, { 0x501850, 0xA030A0 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F20, 0xFFFF40 }, { 0x005500, 0x00AA00 } }, // Wario
    { { 0x7F6E64, 0xFFDCC8 }, { 0x390300, 0x730600 }, { 0x401060, 0x8020C0 }, { 0x1F1F1F, 0x3F3F3F }, { 0x7F7F7F, 0xFFFFFF }, { 0x60407F, 0xC080FF }, { 0x7F3000, 0xFF6000 } }, // Waluigi
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Peach
    { { 0x7F6E64, 0xFFDCC8 }, { 0x603010, 0xC06020 }, { 0x7F7818, 0xFFF030 }, { 0x7F3010, 0xFF6020 }, { 0x7F7F7F, 0xFFFFFF }, { 0x007050, 0x00E0A0 }, { 0x7F3010, 0xFF6020 } }, // Daisy
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F7850, 0xFFF0A0 }, { 0x407878, 0x80F0F0 }, { 0x607F7F, 0xD0FFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F40, 0xFFFF80 }, { 0x407878, 0x80F0F0 } }, // Rosalina
    { { 0x7F6E64, 0xFFDCC8 }, { 0x390300, 0x730600 }, { 0x7F0000, 0xFF0000 }, { 0x500000, 0xA00000 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F6000, 0xFFC000 }, { 0x000000, 0x3F3F3F } }, // Pauline
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x786000, 0xF0C000 }, { 0x7F0000, 0xFF0000 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Builder
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F0000, 0xFF0000 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Fire Flower
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x00687F, 0x00D0FF }, { 0x004048, 0x008090 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Ice Flower
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x000000, 0x3F3F3F }, { 0x7F0000, 0xFF0000 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Red Star
    { { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 }, { 0xFFFFFF, 0x00C0C0 } }, // Galaxy Ice
    { { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 }, { 0x20FF20, 0x007000 } }, // Goo
    { { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 }, { 0xFFFF00, 0xFF0000 } }, // Lava
    { { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 }, { 0xFFE0FF, 0xC00040 } }, // Nebula
    { { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF }, { 0xC0FFFF, 0x4080FF } }, // Crystal
    { { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 }, { 0xA06050, 0xF8D8B8 } }, // Pink Gold
    { { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 }, { 0x804010, 0xF0D020 } }, // Gold
    { { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 }, { 0x404040, 0xC0C0C0 } }, // Silver
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F7F7F, 0xFFFFFF } }, // White
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F }, { 0x000000, 0x3F3F3F }, { 0x00607F, 0x00C0FF }, { 0x000000, 0x3F3F3F } }, // Black
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00007F, 0x0000FF }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // SMG4
    { { 0x7F6E64, 0xFFDCC8 }, { 0x000000, 0x3F3F3F }, { 0x400820, 0x801040 }, { 0x380840, 0x701080 }, { 0x404040, 0x808080 }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // SonicDark
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Custom 1
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Custom 2
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Custom 3
    { { 0x7F6E64, 0xFFDCC8 }, { 0x7F780C, 0xFFF018 }, { 0x7F3C50, 0xFF78A0 }, { 0x681830, 0xD03060 }, { 0x7F7F7F, 0xFFFFFF }, { 0x00607F, 0x00C0FF }, { 0x7F0018, 0xFF0030 } }, // Custom 4
};

extern Lights1 omm_peach_skin_light;
extern Lights1 omm_peach_hair_light;
extern Lights1 omm_peach_dress_1_light;
extern Lights1 omm_peach_dress_2_light;
extern Lights1 omm_peach_glove_light;
extern Lights1 omm_peach_earring_light;
extern Lights1 omm_peach_shoe_light;

OMM_AT_STARTUP static void omm_peach_colors_init() {
    lights1_to_rgb24(omm_peach_skin_light,    &sOmmPeachColorsPresets[0].skin[0],     &sOmmPeachColorsPresets[0].skin[1]);
    lights1_to_rgb24(omm_peach_hair_light,    &sOmmPeachColorsPresets[0].hair[0],     &sOmmPeachColorsPresets[0].hair[1]);
    lights1_to_rgb24(omm_peach_dress_1_light, &sOmmPeachColorsPresets[0].dress1[0],   &sOmmPeachColorsPresets[0].dress1[1]);
    lights1_to_rgb24(omm_peach_dress_2_light, &sOmmPeachColorsPresets[0].dress2[0],   &sOmmPeachColorsPresets[0].dress2[1]);
    lights1_to_rgb24(omm_peach_glove_light,   &sOmmPeachColorsPresets[0].gloves[0],   &sOmmPeachColorsPresets[0].gloves[1]);
    lights1_to_rgb24(omm_peach_earring_light, &sOmmPeachColorsPresets[0].earrings[0], &sOmmPeachColorsPresets[0].earrings[1]);
    lights1_to_rgb24(omm_peach_shoe_light,    &sOmmPeachColorsPresets[0].shoes[0],    &sOmmPeachColorsPresets[0].shoes[1]);
}

OMM_ROUTINE_PRE_RENDER(omm_peach_colors_update) {
    gOmmExtrasPeachColors   = min_s(gOmmExtrasPeachColors, omm_mario_colors_count() - 1);
    omm_peach_skin_light    = rgb24_to_lights1(sOmmPeachColorsPresets[gOmmExtrasPeachColors].skin[0],     sOmmPeachColorsPresets[gOmmExtrasPeachColors].skin[1]);
    omm_peach_hair_light    = rgb24_to_lights1(sOmmPeachColorsPresets[gOmmExtrasPeachColors].hair[0],     sOmmPeachColorsPresets[gOmmExtrasPeachColors].hair[1]);
    omm_peach_dress_1_light = rgb24_to_lights1(sOmmPeachColorsPresets[gOmmExtrasPeachColors].dress1[0],   sOmmPeachColorsPresets[gOmmExtrasPeachColors].dress1[1]);
    omm_peach_dress_2_light = rgb24_to_lights1(sOmmPeachColorsPresets[gOmmExtrasPeachColors].dress2[0],   sOmmPeachColorsPresets[gOmmExtrasPeachColors].dress2[1]);
    omm_peach_glove_light   = rgb24_to_lights1(sOmmPeachColorsPresets[gOmmExtrasPeachColors].gloves[0],   sOmmPeachColorsPresets[gOmmExtrasPeachColors].gloves[1]);
    omm_peach_earring_light = rgb24_to_lights1(sOmmPeachColorsPresets[gOmmExtrasPeachColors].earrings[0], sOmmPeachColorsPresets[gOmmExtrasPeachColors].earrings[1]);
    omm_peach_shoe_light    = rgb24_to_lights1(sOmmPeachColorsPresets[gOmmExtrasPeachColors].shoes[0],    sOmmPeachColorsPresets[gOmmExtrasPeachColors].shoes[1]);
}

//
// Read/Write
//

OMM_INLINE s32 char_to_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

bool omm_mario_colors_read(const char **tokens) {
    static s32 gsCodeIndex = 0;

    // Mario GS color code
    if (strstr(tokens[0], "8107EC") == tokens[0]) {

        // Check custom index
        if (gsCodeIndex < 1 || gsCodeIndex > 4) {
            return false;
        }
        OmmMarioColors *preset = &sOmmMarioColorsPresets[OMM_ARRAY_SIZE(sOmmMarioColorsPresets) - 5 + gsCodeIndex];

        // Read GS code address
        s32 high = char_to_hex(tokens[0][6]);
        s32 low = char_to_hex(tokens[0][7]);
        if (high == -1 || low == -1) {
            return false;
        }
        s32 address = ((high << 4) | low);
        
        // Read GS code color values
        u32 *color;
        switch (address) {
            case 0x38: case 0x3A: case 0x40: case 0x42: color = &preset->cap[(address - 0x38) / 8]; break;
            case 0x98: case 0x9A: case 0xA0: case 0xA2: color = &preset->hair[(address - 0x98) / 8]; break;
            case 0x80: case 0x82: case 0x88: case 0x8A: color = &preset->skin[(address - 0x80) / 8]; break;
            case 0x50: case 0x52: case 0x58: case 0x5A: color = &preset->gloves[(address - 0x50) / 8]; break;
            case 0x20: case 0x22: case 0x28: case 0x2A: color = &preset->overalls[(address - 0x20) / 8]; break;
            case 0x68: case 0x6A: case 0x70: case 0x72: color = &preset->shoes[(address - 0x68) / 8]; break;
            default: return false;
        }
        if ((address & 3) == 0) {
            s32 rh = char_to_hex(tokens[1][0]);
            s32 rl = char_to_hex(tokens[1][1]);
            s32 gh = char_to_hex(tokens[1][2]);
            s32 gl = char_to_hex(tokens[1][3]);
            if (rh == -1 || rl == -1 || gh == -1 || gl == -1) {
                return false;
            }
            *color &= 0x0000FF;
            *color |= ((rh << 20) | (rl << 16) | (gh << 12) | (gl << 8));
        } else {
            s32 bh = char_to_hex(tokens[1][0]);
            s32 bl = char_to_hex(tokens[1][1]);
            if (bh == -1 || bl == -1) {
                return false;
            }
            *color &= 0xFFFF00;
            *color |= ((bh << 4) | (bl << 0));
        }
        return true;
    }

    // Mario custom colors
    if (strstr(tokens[0], "mario_custom_") == tokens[0]) {

        // Check custom index
        s32 index = char_to_hex(tokens[0][13]);
        if (index < 1 || index > 4) {
            return false;
        }

        // Check if it's a GS code
        if (*(tokens[1]) == 0) {
            gsCodeIndex = index;
            return true;
        }

        // Read preset
        gsCodeIndex = 0;
        OmmMarioColors preset = { 0 };
        const char *colorCodes = tokens[1];
        for (s32 i = 0; i != 72; ++i) {

            // Check char
            s32 h = char_to_hex(colorCodes[i]);
            if (h == -1) {
                return false;
            }

            // Get color
            u32 *color;
            switch (i / 12) {
                case 0: color = &preset.cap[(i / 6) & 1]; break;
                case 1: color = &preset.hair[(i / 6) & 1]; break;
                case 2: color = &preset.skin[(i / 6) & 1]; break;
                case 3: color = &preset.gloves[(i / 6) & 1]; break;
                case 4: color = &preset.overalls[(i / 6) & 1]; break;
                case 5: color = &preset.shoes[(i / 6) & 1]; break;
            }

            // Fill color
            *color |= (h << (4 * (5 - (i % 6))));
        }

        // Fill preset
        sOmmMarioColorsPresets[OMM_ARRAY_SIZE(sOmmMarioColorsPresets) - 5 + index] = preset;
        return true;
    }

    // Peach custom colors
    if (strstr(tokens[0], "peach_custom_") == tokens[0]) {

        // Check custom index
        s32 index = char_to_hex(tokens[0][13]);
        if (index < 1 || index > 4) {
            return false;
        }

        // Read preset
        gsCodeIndex = 0;
        OmmPeachColors preset = { 0 };
        const char *colorCodes = tokens[1];
        for (s32 i = 0; i != 84; ++i) {

            // Check char
            s32 h = char_to_hex(colorCodes[i]);
            if (h == -1) {
                return false;
            }

            // Get color
            u32 *color;
            switch (i / 12) {
                case 0: color = &preset.skin[(i / 6) & 1]; break;
                case 1: color = &preset.hair[(i / 6) & 1]; break;
                case 2: color = &preset.dress1[(i / 6) & 1]; break;
                case 3: color = &preset.dress2[(i / 6) & 1]; break;
                case 4: color = &preset.gloves[(i / 6) & 1]; break;
                case 5: color = &preset.earrings[(i / 6) & 1]; break;
                case 6: color = &preset.shoes[(i / 6) & 1]; break;
            }

            // Fill color
            *color |= (h << (4 * (5 - (i % 6))));
        }

        // Fill preset
        sOmmPeachColorsPresets[OMM_ARRAY_SIZE(sOmmPeachColorsPresets) - 5 + index] = preset;
        return true;
    }
    return false;
}

#define write(...) { *buffer += sprintf(*buffer, __VA_ARGS__); }
void omm_mario_colors_write(char **buffer) {
    write("[mario_colors]\n");
    for (s32 i = 0; i != 4; ++i) {
        const OmmMarioColors *preset = &sOmmMarioColorsPresets[OMM_ARRAY_SIZE(sOmmMarioColorsPresets) - 4 + i];
        write("mario_custom_%d = ", i + 1);
        write("%06X%06X%06X%06X%06X%06X%06X%06X%06X%06X%06X%06X\n",
            preset->cap[0], preset->cap[1],
            preset->hair[0], preset->hair[1],
            preset->skin[0], preset->skin[1],
            preset->gloves[0], preset->gloves[1],
            preset->overalls[0], preset->overalls[1],
            preset->shoes[0], preset->shoes[1]
        );
    }
    write("\n[peach_colors]\n");
    for (s32 i = 0; i != 4; ++i) {
        const OmmPeachColors *preset = &sOmmPeachColorsPresets[OMM_ARRAY_SIZE(sOmmPeachColorsPresets) - 4 + i];
        write("peach_custom_%d = ", i + 1);
        write("%06X%06X%06X%06X%06X%06X%06X%06X%06X%06X%06X%06X%06X%06X\n",
            preset->skin[0], preset->skin[1],
            preset->hair[0], preset->hair[1],
            preset->dress1[0], preset->dress1[1],
            preset->dress2[0], preset->dress2[1],
            preset->gloves[0], preset->gloves[1],
            preset->earrings[0], preset->earrings[1],
            preset->shoes[0], preset->shoes[1]
        );
    }
    write("\n");
}

//
// Options menu
//

const char **omm_mario_colors_choices() {
    static const char *sOmmMarioColorsChoices[] = {
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
        OMM_TEXT_OPT_MARIO_COLORS_SMG4,
        OMM_TEXT_OPT_MARIO_COLORS_SONICDARK,
        OMM_TEXT_OPT_MARIO_COLORS_CUSTOM_1,
        OMM_TEXT_OPT_MARIO_COLORS_CUSTOM_2,
        OMM_TEXT_OPT_MARIO_COLORS_CUSTOM_3,
        OMM_TEXT_OPT_MARIO_COLORS_CUSTOM_4,
    };
    return sOmmMarioColorsChoices;
}

s32 omm_mario_colors_count() {
    return OMM_ARRAY_SIZE(sOmmMarioColorsPresets);
}
