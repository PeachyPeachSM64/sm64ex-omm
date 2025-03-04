#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/engine/gfx_texture.h"
extern bool gfx_texture_has_palette(const char *texname);

void *omm_models_find_texture(OmmHMap *phmap, struct GfxRenderingAPI *rapi, const void *p) {
    omm_array_for_each(gOmmActorList, p_actor) {
        OmmActorGfx *actor = (OmmActorGfx *) p_actor->as_ptr;
        if (actor->gfx_data) {
            omm_data_nodes_for_each(actor->gfx_data->textures, texture_node, OmmTexData) {
                if ((const void *) texture_node == p) {
                    const OmmPackData *pack = (const OmmPackData *) omm_array_get(gOmmPackList, ptr, actor->pack_index);

                    // Node found, computing name hash
                    sys_path_t name;
                    fs_cat_paths(name, pack->name, texture_node->name);
                    u32 hash = str_hash(name);
                    GfxTexture *tex = NULL;

                    // Look for texture in cache
                    s32 i = omm_hmap_find(*phmap, hash);
                    if (i != -1) {
                        tex = omm_hmap_get(*phmap, GfxTexture *, i);
                        if (!texture_node->data->uploaded) {
                            rapi->select_texture(0, tex->id);
                            rapi->upload_texture(texture_node->data->raw_data, texture_node->data->raw_width, texture_node->data->raw_height);
                            texture_node->data->uploaded = true;
                        }
                        return (void *) tex;
                    }

                    // Create new texture
                    tex = mem_new(GfxTexture, 1);
                    tex->hash = hash;
                    tex->id = rapi->new_texture();
                    tex->w = texture_node->data->raw_width;
                    tex->h = texture_node->data->raw_height;
                    tex->pal = 0;
                    tex->cms = 0;
                    tex->cmt = 0;
                    tex->lin = 0;
                    tex->data = (gfx_texture_has_palette(texture_node->name) ? mem_dup(texture_node->data->raw_data, tex->w * tex->h * 4) : NULL);
                    rapi->select_texture(0, tex->id);
                    rapi->set_sampler_parameters(0, false, 0, 0);

                    // Cache texture
                    omm_hmap_insert(*phmap, tex->hash, tex);

                    // Upload texture
                    rapi->select_texture(0, tex->id);
                    rapi->upload_texture(texture_node->data->raw_data, texture_node->data->raw_width, texture_node->data->raw_height);
                    texture_node->data->uploaded = true;
                    return (void *) tex;
                }
            }
        }
    }
    return NULL;
}
