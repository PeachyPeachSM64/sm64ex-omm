#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Texts
//

#define OMM_TEXT_(id, str) const char OMM_TEXT_##id[] = DEF(str, str, "OMM_TEXT_" #id, str, str, str);
#include "data/omm/omm_defines_texts.inl"
#undef OMM_TEXT_

//
// Textures
//

#define OMM_TEXTURE_(id, str) const char OMM_TEXTURE_##id[] = str;
#include "data/omm/omm_defines_textures.inl"
#undef OMM_TEXTURE_

//
// Assets
//

#define OMM_ASSET_(id, str) const char OMM_ASSET_##id[] = str;
#include "data/omm/omm_defines_assets.inl"
#undef OMM_ASSET_
