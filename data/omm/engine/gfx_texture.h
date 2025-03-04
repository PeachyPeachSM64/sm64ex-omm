#ifndef GFX_TEXTURE_H
#define GFX_TEXTURE_H

#include "types.h"

typedef struct {
    u8 *data, *ptr;
    u32 hash, id;
    u32 w, h, pal;
    u8 cms, cmt, lin;
} GfxTexture;

#endif
