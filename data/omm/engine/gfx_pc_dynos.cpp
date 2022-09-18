#if defined(DYNOS)
#include "data/dynos.cpp.h"
extern "C" {
#include "data/omm/omm_defines.h"
#include "data/omm/omm_patches.h"
#include "data/omm/system/omm_system.h"
#include "data/omm/system/omm_memory.h"
#include "pc/gfx/gfx_rendering_api.h"
}

typedef struct { u32 hash, id; u8 cms, cmt, lin; } GfxTexture;

OMM_INLINE u32 string_hash(const char *str) {
    u32 hash = 0;
    for (; *str; str++) {
        hash = (hash * 31) + *str;
    }
    return hash;
}

static GfxTexture *__dynos_gfx_texture_find(OmmHMap *phmap, struct GfxRenderingAPI *rapi, const void *p) {
    Array<ActorGfx> &actorList = DynOS_Gfx_GetActorList();
    for (auto &actor : actorList) {
        if (actor.mGfxData) {
            for (auto &node : actor.mGfxData->mTextures) {
                if ((const void *) node == p) {
                    
                    // Node found, computing name hash
                    omm_cat_paths(name, SYS_MAX_PATH, "DYNOS", node->mName.begin());
                    u32 hash = string_hash(name);
                    GfxTexture *tex = NULL;

                    // Look for texture in cache
                    s32 i = omm_hmap_find(*phmap, hash);
                    if (i != -1) {
                        tex = omm_hmap_get(*phmap, GfxTexture *, i);
                        if (!node->mData->mUploaded) {
                            rapi->select_texture(0, tex->id);
                            rapi->upload_texture(node->mData->mRawData.begin(), node->mData->mRawWidth, node->mData->mRawHeight);
                            node->mData->mUploaded = true;
                        }
                        return tex;
                    }

                    // Create new texture
                    tex = (GfxTexture *) omm_new(GfxTexture, 1);
                    tex->hash = hash;
                    tex->id = rapi->new_texture();
                    tex->cms = 0;
                    tex->cmt = 0;
                    tex->lin = 0;
                    rapi->select_texture(0, tex->id);
                    rapi->set_sampler_parameters(0, false, 0, 0);

                    // Cache texture
                    omm_hmap_insert(*phmap, tex->hash, tex);

                    // Upload texture
                    rapi->select_texture(0, tex->id);
                    rapi->upload_texture(node->mData->mRawData.begin(), node->mData->mRawWidth, node->mData->mRawHeight);
                    node->mData->mUploaded = true;
                    return tex;
                }
            }
        }
    }
    return NULL;
}

extern "C" {
void *dynos_gfx_texture_find(OmmHMap *phmap, struct GfxRenderingAPI *rapi, const void *p) {
    return (void *) __dynos_gfx_texture_find(phmap, rapi, p);
}
}
#endif
