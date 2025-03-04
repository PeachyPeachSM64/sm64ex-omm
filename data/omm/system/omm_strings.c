#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Texts
//

#define OMM_TEXT_(id, str) const char OMM_TEXT_##id[] = OMM_TEXT_FORMAT(#id, str);
#include "data/omm/omm_defines_texts.inl"
#undef OMM_TEXT_

//
// Textures
//

#define OMM_TEXTURE_(id, str) const char OMM_TEXTURE_##id[] = str;
#include "data/omm/omm_defines_textures.inl"
#undef OMM_TEXTURE_
const char *OMM_TEXTURE_STAR_BODY_[20] = {
    OMM_TEXTURE_STAR_BODY_0,
    OMM_TEXTURE_STAR_BODY_1,
    OMM_TEXTURE_STAR_BODY_2,
    OMM_TEXTURE_STAR_BODY_3,
    OMM_TEXTURE_STAR_BODY_4,
    OMM_TEXTURE_STAR_BODY_5,
    OMM_TEXTURE_STAR_BODY_6,
    OMM_TEXTURE_STAR_BODY_7,
    OMM_TEXTURE_STAR_BODY_8,
    OMM_TEXTURE_STAR_BODY_9,
    OMM_TEXTURE_STAR_BODY_10,
    OMM_TEXTURE_STAR_BODY_11,
    OMM_TEXTURE_STAR_BODY_12,
    OMM_TEXTURE_STAR_BODY_13,
    OMM_TEXTURE_STAR_BODY_14,
    OMM_TEXTURE_STAR_BODY_15,
    OMM_TEXTURE_STAR_BODY_16,
    OMM_TEXTURE_STAR_BODY_17,
    OMM_TEXTURE_STAR_BODY_18,
    OMM_TEXTURE_STAR_BODY_19,
};
const char *OMM_TEXTURE_STAR_FULL_[20] = {
    OMM_TEXTURE_STAR_FULL_0,
    OMM_TEXTURE_STAR_FULL_1,
    OMM_TEXTURE_STAR_FULL_2,
    OMM_TEXTURE_STAR_FULL_3,
    OMM_TEXTURE_STAR_FULL_4,
    OMM_TEXTURE_STAR_FULL_5,
    OMM_TEXTURE_STAR_FULL_6,
    OMM_TEXTURE_STAR_FULL_7,
    OMM_TEXTURE_STAR_FULL_8,
    OMM_TEXTURE_STAR_FULL_9,
    OMM_TEXTURE_STAR_FULL_10,
    OMM_TEXTURE_STAR_FULL_11,
    OMM_TEXTURE_STAR_FULL_12,
    OMM_TEXTURE_STAR_FULL_13,
    OMM_TEXTURE_STAR_FULL_14,
    OMM_TEXTURE_STAR_FULL_15,
    OMM_TEXTURE_STAR_FULL_16,
    OMM_TEXTURE_STAR_FULL_17,
    OMM_TEXTURE_STAR_FULL_18,
    OMM_TEXTURE_STAR_FULL_19,
};
int OMM_STAR_COLOR_[OMM_STAR_COLOR_COUNT] = {
    OMM_STAR_COLORS
};

//
// Assets
//

#define OMM_ASSET_(id, str) const char OMM_ASSET_##id[] = str;
#include "data/omm/omm_defines_assets.inl"
#undef OMM_ASSET_

//
// Palettes
//

#define OMM_PALETTE_(id, str) const char OMM_PALETTE_##id[] = str;
#define OMM_PALETTE_LEVEL_(id, str) const char OMM_PALETTE_##id[] = str;
#include "data/omm/omm_defines_palettes.inl"
#undef OMM_PALETTE_
#undef OMM_PALETTE_LEVEL_
