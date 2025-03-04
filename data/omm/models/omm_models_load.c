#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <zlib.h>

#define FUNC_CODE ((u32) 0x434E5546) // FUNC
#define PNTR_CODE ((u32) 0x52544E50) // PNTR
#define LUAV_CODE ((u32) 0x5641554C) // LUAV
#define TEXR_CODE ((u32) 0x52584554) // TEXR

#define F32VTX_S0 ((u16) 0x3346) // F3
#define F32VTX_S1 ((u16) 0x5632) // 2V
#define F32VTX_S2 ((u16) 0x5854) // TX

//
// Bin file
//

#define DYNOS_BIN_COMPRESS_MAGIC (0x4E4942534F4E5944llu)
#define DYNOS_BIN_HEADER_LENGTH  (sizeof(u64) + sizeof(u64))

static OmmFileBuffer *omm_models_open(const char *filename, u64 *loaded_bytes) {
    static OmmFileBuffer fb[1] = {0};
    fs_file_t *file = fs_open(filename);
    if (file) {
        if (loaded_bytes) {
            *loaded_bytes += fs_size(file);
        }

        // Compressed
        u64 magic = 0;
        if (fs_read(file, &magic, sizeof(u64)) == sizeof(u64) && magic == DYNOS_BIN_COMPRESS_MAGIC) {
            u64 u_length = 0;
            if (fs_read(file, &u_length, sizeof(u64)) == sizeof(u64)) {
                u64 c_length = (u64) fs_size(file);
                if (c_length >= DYNOS_BIN_HEADER_LENGTH) {
                    u8 *c_buffer = mem_new(u8, c_length - DYNOS_BIN_HEADER_LENGTH);
                    if (c_buffer) {
                        fs_seek(file, DYNOS_BIN_HEADER_LENGTH);
                        if (fs_read(file, c_buffer, sizeof(u8) * (c_length - DYNOS_BIN_HEADER_LENGTH)) == (s64) (sizeof(u8) * (c_length - DYNOS_BIN_HEADER_LENGTH))) {
                            u8 *u_buffer = mem_new(u8, u_length);
                            if (u_buffer) {
                                uLongf u_len = (uLongf) u_length;
                                int uncompress_rc = uncompress((Bytef *) u_buffer, &u_len, (const Bytef *) c_buffer, c_length);
                                u_length = (u64) u_len;
                                if (uncompress_rc == Z_OK) {
                                    fs_close(file);
                                    mem_del(c_buffer);
                                    fb_init(fb, u_buffer, (s32) u_length);
                                    return fb;
                                }
                                mem_del(u_buffer);
                            }
                        }
                        mem_del(c_buffer);
                    }
                }
            }
        }

        // Uncompressed
        u64 length = (u64) fs_size(file);
        u8 *buffer = mem_new(u8, length);
        if (buffer) {
            fs_seek(file, 0);
            if (fs_read(file, buffer, sizeof(u8) * length) == (s64) (sizeof(u8) * length)) {
                fs_close(file);
                fb_init(fb, buffer, (s32) length);
                return fb;
            }
        }
        fs_close(file);
    }
    fb_init(fb, NULL, 0);
    return fb;
}

//
// Read data
//

static void *omm_models_get_pointer(OmmGfxData *gfx_data, const char *ptr_name, u32 ptr_data) {

    // Lights
    omm_data_nodes_for_each(gfx_data->lights, light_node, Lights1) {
        if (strcmp(light_node->name, ptr_name) == 0) {
            if (ptr_data == 1) {
                return (void *) &light_node->data->l[0];
            }
            if (ptr_data == 2) {
                return (void *) &light_node->data->a;
            }
            sys_fatal("omm_models_get_pointer: Unknown Light type: %u", ptr_data);
        }
    }

    // Textures
    omm_data_nodes_for_each(gfx_data->textures, texture_node, OmmTexData) {
        if (strcmp(texture_node->name, ptr_name) == 0) {
            return (void *) texture_node;
        }
    }

    // Display lists
    omm_data_nodes_for_each(gfx_data->display_lists, display_list_node, Gfx) {
        if (strcmp(display_list_node->name, ptr_name) == 0) {
            return (void *) display_list_node->data;
        }
    }

    // Geo layouts
    omm_data_nodes_for_each(gfx_data->geo_layouts, geo_layout_node, GeoLayout) {
        if (strcmp(geo_layout_node->name, ptr_name) == 0) {
            return (void *) geo_layout_node->data;
        }
    }

    // Vertices
    omm_data_nodes_for_each(gfx_data->vertices, vtx_node, Vtx) {
        if (strcmp(vtx_node->name, ptr_name) == 0) {
            return (void *) (vtx_node->data + ptr_data);
        }
    }

    // Error
    sys_fatal("omm_models_get_pointer: Pointer not found: %s", ptr_name);
    return NULL;
}

static void *omm_models_read_pointer(OmmFileBuffer *fb, OmmGfxData *gfx_data, u32 type) {

    // FUNC
    if (type == FUNC_CODE) {
        s32 index = fb_read(fb, s32);
        return omm_models_get_func_pointer(index);
    }

    // PNTR
    if (type == PNTR_CODE) {
        str_t ptr_name;
        fb_read_str(fb, ptr_name);
        u32 ptr_data = fb_read(fb, u32);
        return omm_models_get_pointer(gfx_data, ptr_name, ptr_data);
    }

    // LUAV
    if (type == LUAV_CODE) {
        str_t func_name;
        fb_read_str(fb, func_name);
        // return omm_models_get_func_pointer_from_lua_name(func_name); // NOT IMPLEMENTED
        return omm_models_get_func_pointer(-1); // geo_nop
    }

    // Not a pointer
    return NULL;
}

//
// Optimizations
//

#define gfx_op(gfx)             ((u8) ((gfx)->words.w0 >> 24))
#define gfx_w0(gfx)             (gfx)->words.w0
#define gfx_w1(gfx)             (gfx)->words.w1
#define gfx_w1p(gfx)            ((void *) (gfx)->words.w1)
#define gfx_c0(gfx, off, siz)   (((gfx)->words.w0 >> (off)) & ((1U << siz) - 1))
#define gfx_c1(gfx, off, siz)   (((gfx)->words.w1 >> (off)) & ((1U << siz) - 1))

#define realloc_buf(buf, typ, oldsiz, newsiz) { \
    typ *newbuf = mem_new(typ, (newsiz)); \
    if (oldsiz) mem_cpy((newbuf), (buf), (oldsiz) * sizeof(typ)); \
    mem_del(buf); \
    buf = newbuf; \
}

OMM_OPTIMIZE static void omm_models_optimize_triangle_data(OmmDataNode_Gfx *node) {
    Gfx *gfx = node->data;
    Vtx *verts = NULL; u32 nverts = 0; u32 lastcount = 0;
    u32 *tris = NULL; u32 ntris = 0;
    for (u32 k = 0, i0 = 0; k != (u32) node->size; ++k, gfx++) {

        // gsSPVertex
        if (gfx_op(gfx) == G_VTX) {
            i0 = nverts;
            u32 count = lastcount = gfx_c0(gfx, 12, 8);
            u32 first = gfx_c0(gfx, 1, 7) - count;

            // Skip skinned meshes
            if (first != 0) {
                mem_del(verts);
                mem_del(tris);
                return;
            }

            realloc_buf(verts, Vtx, nverts, nverts + count);
            mem_cpy(verts + nverts, gfx_w1p(gfx), count * sizeof(Vtx));
            nverts += count;
            continue;
        }

        // gsSP1Triangle
        if (gfx_op(gfx) == G_TRI1) {
            u32 i1 = gfx_c0(gfx, 16, 8) >> 1;
            u32 i2 = gfx_c0(gfx,  8, 8) >> 1;
            u32 i3 = gfx_c0(gfx,  0, 8) >> 1;
            realloc_buf(tris, u32, ntris, ntris + 3);
            tris[ntris + 0] = i0 + i1;
            tris[ntris + 1] = i0 + i2;
            tris[ntris + 2] = i0 + i3;
            ntris += 3;
            continue;
        }

        // gsSP2Triangles
        if (gfx_op(gfx) == G_TRI2) {
            u32 i1 = gfx_c0(gfx, 16, 8) >> 1;
            u32 i2 = gfx_c0(gfx,  8, 8) >> 1;
            u32 i3 = gfx_c0(gfx,  0, 8) >> 1;
            u32 i4 = gfx_c1(gfx, 16, 8) >> 1;
            u32 i5 = gfx_c1(gfx,  8, 8) >> 1;
            u32 i6 = gfx_c1(gfx,  0, 8) >> 1;
            realloc_buf(tris, u32, ntris, ntris + 6);
            tris[ntris + 0] = i0 + i1;
            tris[ntris + 1] = i0 + i2;
            tris[ntris + 2] = i0 + i3;
            tris[ntris + 3] = i0 + i4;
            tris[ntris + 4] = i0 + i5;
            tris[ntris + 5] = i0 + i6;
            ntris += 6;
            continue;
        }
    }

    // Replace the old display list by a 3-command display list with better performance
    mem_del(node->data);
    node->data = mem_new(Gfx, 3);
    gSPVertexExt(node->data + 0, verts, nverts, lastcount);
    gSPTrianglesExt(node->data + 1, tris, ntris / 3);
    gSPEndDisplayList(node->data + 2);
    node->size = 3;
}

static bool omm_models_check_emblem_color(OmmDataNode_Gfx *node) {
    static const Gfx sSPCopyLightsPlayerPartEMBLEM[] = { gsSPCopyLightsPlayerPart(7) };
    Gfx *gfx = node->data;
    for (u32 i = 0; i != (u32) node->size; ++i, gfx++) {
        if (gfx->words.w0 == sSPCopyLightsPlayerPartEMBLEM[0].words.w0 ||
            gfx->words.w0 == sSPCopyLightsPlayerPartEMBLEM[1].words.w0) {
            return true;
        }
    }
    return false;
}

//
// Read binary
//

static void omm_models_load_light_data(OmmFileBuffer *fb, OmmGfxData *gfx_data) {
    OmmDataNode_Lights1 *light_node = mem_new(OmmDataNode_Lights1, 1);

    // Name
    fb_read_str(fb, light_node->name);

    // Data
    light_node->data = mem_new(Lights1, 1);
    *light_node->data = fb_read(fb, Lights1);

    // Append
    omm_array_add(gfx_data->lights, ptr, light_node);
}

static void omm_models_load_texture_data(OmmFileBuffer *fb, OmmGfxData *gfx_data) {
    OmmDataNode_OmmTexData *texture_node = mem_new(OmmDataNode_OmmTexData, 1);

    // Name
    fb_read_str(fb, texture_node->name);

    // Data
    texture_node->data = mem_new(OmmTexData, 1);
    texture_node->data->uploaded = false;

    // Check for the texture ref magic
    s32 file_pos = fb_tell(fb);
    u32 texr_code = fb_read(fb, u32);
    if (texr_code == TEXR_CODE) {

        // That's a duplicate, find the original node and copy its content
        str_t node_name;
        fb_read_str(fb, node_name);
        omm_data_nodes_for_each(gfx_data->textures, loaded_node, OmmTexData) {
            if (strcmp(loaded_node->name, node_name) == 0) {
                texture_node->data->png_data   = loaded_node->data->png_data; // Data pointer copy, should be fine
                texture_node->data->raw_data   = loaded_node->data->raw_data; // Data pointer copy, should be fine
                texture_node->data->raw_width  = loaded_node->data->raw_width;
                texture_node->data->raw_height = loaded_node->data->raw_height;
                break;
            }
        }
    } else {
        fb_seek(fb, file_pos);
        s32 png_length = 0;
        texture_node->data->png_data = (const u8 *) fb_read_buffer(fb, sizeof(u8), &png_length);
        if (png_length > 0) {
            texture_node->data->raw_data = stbi_load_from_memory(
                texture_node->data->png_data,
                png_length,
                &texture_node->data->raw_width,
                &texture_node->data->raw_height,
                NULL, 4
            );
        } else { // Probably a palette
            texture_node->data->raw_data = NULL;
            texture_node->data->raw_width = 0;
            texture_node->data->raw_height = 0;
        }
    }

    // Append
    omm_array_add(gfx_data->textures, ptr, texture_node);
}

static void omm_models_load_vertex_data(OmmFileBuffer *fb, OmmGfxData *gfx_data) {
    OmmDataNode_Vtx *vtx_node = mem_new(OmmDataNode_Vtx, 1);

    // Name
    fb_read_str(fb, vtx_node->name);

    // Data
    bool is_f32vtx = false;
    vtx_node->size = fb_read(fb, s32);
    vtx_node->data = mem_new(Vtx, vtx_node->size);
    for (s32 i = 0; i != vtx_node->size; ++i) {
        if (is_f32vtx) {
            vtx_node->data[i].n.ob[0] = fb_read(fb, f32);
            vtx_node->data[i].n.ob[1] = fb_read(fb, f32);
            vtx_node->data[i].n.ob[2] = fb_read(fb, f32);
        } else {
            vtx_node->data[i].n.ob[0] = fb_read(fb, s16);
            vtx_node->data[i].n.ob[1] = fb_read(fb, s16);
            vtx_node->data[i].n.ob[2] = fb_read(fb, s16);
        }
        vtx_node->data[i].n.flag  = fb_read(fb, u16);
        vtx_node->data[i].n.tc[0] = fb_read(fb, s16);
        vtx_node->data[i].n.tc[1] = fb_read(fb, s16);
        vtx_node->data[i].n.n[0]  = fb_read(fb, s8);
        vtx_node->data[i].n.n[1]  = fb_read(fb, s8);
        vtx_node->data[i].n.n[2]  = fb_read(fb, s8);
        vtx_node->data[i].n.a     = fb_read(fb, u8);

        // Check sentinel on first vertex
        const f32 *ob = vtx_node->data[i].n.ob;
        if (!is_f32vtx && i == 0 && ob[0] == F32VTX_S0 && ob[1] == F32VTX_S1 && ob[2] == F32VTX_S2) {
            vtx_node->size--; i--;
            is_f32vtx = true;
        }
    }

    // Billboard check
    if (!gfx_data->disable_billboard) {
        Vec3f pn = { 0, 0, 0 };
        for (s32 i = 2; i < vtx_node->size; ++i) {
            Vec3f p0; vec3f_copy(p0, vtx_node->data[i - 2].v.ob);
            Vec3f p1; vec3f_copy(p1, vtx_node->data[i - 1].v.ob);
            Vec3f p2; vec3f_copy(p2, vtx_node->data[i - 0].v.ob);
            Vec3f v0; vec3f_dif(v0, p0, p1);
            Vec3f v1; vec3f_dif(v1, p1, p2);
            Vec3f vn; vec3f_cross(vn, v0, v1);
            if (!vec3f_eq(vn, gVec3fZero)) { // skip zero normals
                vec3f_normalize(vn);
                if (!vec3f_eq(pn, gVec3fZero)) { // don't compare to zero normal
                    f32 dot = vec3f_dot(vn, pn);
                    if (dot < 0.9f) { // Points don't form a plane -> no billboard
                        gfx_data->disable_billboard = true;
                        break;
                    }
                }
                vec3f_copy(pn, vn);
            }
        }
    }

    // Append
    omm_array_add(gfx_data->vertices, ptr, vtx_node);
}

static void omm_models_load_display_list_data(OmmFileBuffer *fb, OmmGfxData *gfx_data) {
    OmmDataNode_Gfx *display_list_node = mem_new(OmmDataNode_Gfx, 1);

    // Name
    fb_read_str(fb, display_list_node->name);

    // Data
    display_list_node->size = fb_read(fb, s32);
    display_list_node->data = mem_new(Gfx, display_list_node->size);
    for (s32 i = 0; i != display_list_node->size; ++i) {
        u32 words_w0 = fb_read(fb, u32);
        u32 words_w1 = fb_read(fb, u32);
        void *ptr = omm_models_read_pointer(fb, gfx_data, words_w1);
        if (ptr) {
            display_list_node->data[i].words.w0 = (uintptr_t) words_w0;
            display_list_node->data[i].words.w1 = (uintptr_t) ptr;
        } else {
            display_list_node->data[i].words.w0 = (uintptr_t) words_w0;
            display_list_node->data[i].words.w1 = (uintptr_t) words_w1;

            // (Un)fix TEXTURE_GEN on custom models
            if (_SHIFTR(words_w0, 24, 8) == G_GEOMETRYMODE) {
                u32 set = _SHIFTR(words_w1, 0, 24);
                u32 clr = _SHIFTR(~words_w0, 0, 24);
                if (set & G_TEXTURE_GEN) { set |= G_TEXTURE_GEN_INVERT; }
                if (clr & G_TEXTURE_GEN) { clr |= G_TEXTURE_GEN_INVERT; }
                gSPGeometryMode(&display_list_node->data[i], clr, set);
            }
        }
    }

    // Pack all vertex and triangle data into one contiguous array,
    // increasing performance when rendering the model
    if (gfx_op(display_list_node->data) == G_VTX) {
        omm_models_optimize_triangle_data(display_list_node);
    }

    // Check if the model uses the EMBLEM color (sm64coopdx)
    if (!gfx_data->use_emblem && omm_models_check_emblem_color(display_list_node)) {
        gfx_data->use_emblem = true;
    }

    // Append
    omm_array_add(gfx_data->display_lists, ptr, display_list_node);
}

#define FIX_NODE(func, command, ...) { \
    if (ptr == func) { \
        __VA_ARGS__; \
        GeoLayout fix[] = { command }; \
        u32 fix_size = array_length(fix); \
        mem_cpy(geo_layout_node->data + offset + 1 - fix_size, fix, fix_size); \
        return; \
    } \
}

#define FIX_NODE_WITH_PARAM(func, command, mod) { \
    FIX_NODE(func, __EXPAND(command), __EXPAND(u16 param = ((geo_layout_node->data[offset - 1] >> 16) & 0xFFFF) % mod)) \
}

static void omm_models_fix_geo_mario_node(OmmDataNode_GeoLayout *geo_layout_node, s32 offset, void *ptr) {
    FIX_NODE(geo_mirror_mario_set_alpha, GEO_ASM(0, geo_mirror_mario_set_alpha));
    FIX_NODE(geo_switch_mario_stand_run, GEO_SWITCH_CASE(0, geo_switch_mario_stand_run));
    FIX_NODE(geo_switch_mario_eyes, GEO_SWITCH_CASE(0, geo_switch_mario_eyes));
    FIX_NODE(geo_mario_tilt_torso, GEO_ASM(0, geo_mario_tilt_torso));
    FIX_NODE(geo_mario_head_rotation, GEO_ASM(0, geo_mario_head_rotation));
    FIX_NODE_WITH_PARAM(geo_switch_mario_hand, GEO_SWITCH_CASE(param, geo_switch_mario_hand), 0x10000);
    FIX_NODE_WITH_PARAM(geo_mario_hand_foot_scaler, GEO_ASM(param, geo_mario_hand_foot_scaler), 3);
    FIX_NODE(geo_switch_mario_cap_effect, GEO_SWITCH_CASE(0, geo_switch_mario_cap_effect));
    FIX_NODE(geo_switch_mario_cap_on_off, GEO_SWITCH_CASE(0, geo_switch_mario_cap_on_off));
    FIX_NODE_WITH_PARAM(geo_mario_rotate_wing_cap_wings, GEO_ASM(param, geo_mario_rotate_wing_cap_wings), 2);
}

static void omm_models_load_geo_layout_data(OmmFileBuffer *fb, OmmGfxData *gfx_data) {
    OmmDataNode_GeoLayout *geo_layout_node = mem_new(OmmDataNode_GeoLayout, 1);

    // Name
    fb_read_str(fb, geo_layout_node->name);

    // Data
    geo_layout_node->size = fb_read(fb, s32);
    geo_layout_node->data = mem_new(GeoLayout, geo_layout_node->size);
    for (s32 i = 0; i != geo_layout_node->size; ++i) {
        u32 value = fb_read(fb, u32);
        void *ptr = omm_models_read_pointer(fb, gfx_data, value);
        if (ptr) {
            geo_layout_node->data[i] = (uintptr_t) ptr;
            omm_models_fix_geo_mario_node(geo_layout_node, i, ptr);
        } else {
            geo_layout_node->data[i] = (uintptr_t) value;
        }
    }

    // Append
    omm_array_add(gfx_data->geo_layouts, ptr, geo_layout_node);
}

static void omm_models_load_animation_data(OmmFileBuffer *fb, OmmGfxData *gfx_data) {
    OmmDataNode_Animation *anim_node = mem_new(OmmDataNode_Animation, 1);
    s32 values_length, index_length;

    // Name
    fb_read_str(fb, anim_node->name);

    // Data
    anim_node->data              = mem_new(Animation, 1);
    anim_node->data->flags       = fb_read(fb, s16);
    UNUSED s16 anim_y_trans_div  = fb_read(fb, s16);
    anim_node->data->mStartFrame = fb_read(fb, s16);
    anim_node->data->mLoopStart  = fb_read(fb, s16);
    anim_node->data->mLoopEnd    = fb_read(fb, s16);
    UNUSED s16 bone_count        = fb_read(fb, s16);
    UNUSED u32 length            = fb_read(fb, u32);
    anim_node->data->values      = (const s16 *) fb_read_buffer(fb, sizeof(s16), &values_length);
    anim_node->data->index       = (const u16 *) fb_read_buffer(fb, sizeof(u16), &index_length);
    anim_node->data->length      = ANIM_LENGTH(index_length, values_length);

    // Append
    omm_array_add(gfx_data->animations, ptr, anim_node);
}

static void omm_models_load_animation_table(OmmFileBuffer *fb, OmmGfxData *gfx_data) {
    Animation *anim_ptr = NULL;

    // Data
    str_t anim_name;
    fb_read_str(fb, anim_name);
    if (strcmp(anim_name, "NULL") != 0) {
        omm_data_nodes_for_each(gfx_data->animations, anim_node, Animation) {
            if (strcmp(anim_node->name, anim_name) == 0) {
                anim_ptr = (void *) anim_node->data;
                break;
            }
        }
        if (!anim_ptr) { // Not a fatal error; the model can still use the vanilla anims
            return;
        }
    }

    // Append
    omm_array_add(gfx_data->animation_table, ptr, anim_ptr);
}

static void omm_models_load_priority(OmmFileBuffer *fb, OmmGfxData *gfx_data) {
    gfx_data->priority = fb_read(fb, u8);
}

//
// Load from binary
//

OmmGfxData *omm_models_load_from_binary(const char *pack_folder, const char **actor_names, u64 *loaded_bytes) {
    typedef struct {
        sys_path_t pack_folder;
        OmmArray_(const char *) gfx_names;
        OmmArray_(OmmGfxData *) gfx_data;
    } OmmGfxDataCache;
    static OmmArray_(OmmGfxDataCache *) sOmmGfxDataCache = omm_array_zero;
    const char **actor_name = NULL;

    // Look for pack in cache
    OmmGfxDataCache *cached_pack = NULL;
    omm_array_for_each(sOmmGfxDataCache, p_gfx_data_cache) {
        OmmGfxDataCache *gfx_data_cache = (OmmGfxDataCache *) p_gfx_data_cache->as_ptr;
        if (strcmp(gfx_data_cache->pack_folder, pack_folder) == 0) {
            cached_pack = gfx_data_cache;
            break;
        }
    }

    // Look for actor in pack
    if (cached_pack) {
        omm_array_for_each(cached_pack->gfx_names, p_gfx_name) {
            const char *gfx_name = (const char *) p_gfx_name->as_ptr;
            for (actor_name = actor_names; *actor_name; ++actor_name) {
                if (gfx_name == *actor_name) { // Perfectly valid, actor_names comes from static RO data, so its address never changes during execution
                    return (OmmGfxData *) omm_array_get(cached_pack->gfx_data, ptr, i_p_gfx_name);
                }
            }
        }
    }

    // Load data from binary file
    OmmGfxData *gfx_data = NULL;
    for (actor_name = actor_names; *actor_name; ++actor_name) {
        sys_path_t filename;
        str_cat(filename, sizeof(filename), pack_folder, "/", *actor_name, ".bin");
        OmmFileBuffer *fb = omm_models_open(filename, loaded_bytes);
        if (!fb_eof(fb)) {
            gfx_data = mem_new(OmmGfxData, 1);
            for (bool done = false; !done && !fb_eof(fb);) {
                u8 data_type = fb_read(fb, u8);
                switch (data_type) {
                    case OMM_MODELS_DATA_TYPE_LIGHT:           omm_models_load_light_data       (fb, gfx_data); break;
                    case OMM_MODELS_DATA_TYPE_TEXTURE:         omm_models_load_texture_data     (fb, gfx_data); break;
                    case OMM_MODELS_DATA_TYPE_VERTEX:          omm_models_load_vertex_data      (fb, gfx_data); break;
                    case OMM_MODELS_DATA_TYPE_DISPLAY_LIST:    omm_models_load_display_list_data(fb, gfx_data); break;
                    case OMM_MODELS_DATA_TYPE_GEO_LAYOUT:      omm_models_load_geo_layout_data  (fb, gfx_data); break;
                    case OMM_MODELS_DATA_TYPE_ANIMATION:       omm_models_load_animation_data   (fb, gfx_data); break;
                    case OMM_MODELS_DATA_TYPE_ANIMATION_TABLE: omm_models_load_animation_table  (fb, gfx_data); break;
                    case OMM_MODELS_DATA_TYPE_PRIORITY:        omm_models_load_priority         (fb, gfx_data); break;
                    default: {
                        omm_log_warning("In file \"%s\", at offset 0x%08X: Unknown data type (0x%02X)",, filename, (u32) fb_tell(fb), data_type);
                        done = true;
                    } break;
                }
            }
            fb_free(fb);
            break;
        }
    }

    // If not loaded, make sure to insert at least one valid actor name for the cache to work
    if (!gfx_data || !*actor_name) {
        actor_name = actor_names;
    }

    // Add data to cache, even if not loaded
    if (cached_pack) {
        omm_array_add(cached_pack->gfx_names, ptr, *actor_name);
        omm_array_add(cached_pack->gfx_data, ptr, gfx_data);
    } else {
        cached_pack = mem_new(OmmGfxDataCache, 1);
        str_cpy(cached_pack->pack_folder, sizeof(cached_pack->pack_folder), pack_folder);
        omm_array_add(cached_pack->gfx_names, ptr, *actor_name);
        omm_array_add(cached_pack->gfx_data, ptr, gfx_data);
        omm_array_add(sOmmGfxDataCache, ptr, cached_pack);
    }
    return gfx_data;
}

//
// Caching
//

void omm_models_load_all(u64 *loaded_bytes) {
    omm_array_for_each(gOmmPackList, p_pack) {
        OmmPackData *pack = p_pack->as_ptr;
        if (pack && pack->exists && pack->caching != OMM_MODELS_CACHING_CACHED) {
            if (!gOmmModelPackCaching) {
                pack->caching = OMM_MODELS_CACHING_IGNORE;
            } else if (!pack->enabled) {
                pack->caching = OMM_MODELS_CACHING_DISABLED;
            } else if (pack->caching == OMM_MODELS_CACHING_DISABLED) {
                pack->caching = OMM_MODELS_CACHING_ENABLED;
            } else if (pack->caching == OMM_MODELS_CACHING_ENABLED) {
                omm_log("Caching model pack: %s\n",, pack->name);
                if (pack->cs_index) {
                    omm_models_cs_load_all(pack->cs_index, loaded_bytes);
                } else {
                    for (s32 actor_index = 0; actor_index != omm_models_get_actor_count(); ++actor_index) {
                        OmmGfxData *gfx_data = omm_models_load_from_binary(pack->path, omm_models_get_actor_names(actor_index), loaded_bytes);
                        if (gfx_data) {
                            OmmDataNode_GeoLayout *geo_layout_node = (OmmDataNode_GeoLayout *) omm_array_get(gfx_data->geo_layouts, ptr, omm_array_count(gfx_data->geo_layouts) - 1);
                            geo_layout_to_graph_node(NULL, geo_layout_node->data);
                        }
                    }
                }
                pack->caching = OMM_MODELS_CACHING_CACHED;
            }
        }
    }
}

s32 *omm_models_get_caching_state(bool *toggle) {
    omm_array_for_each(gOmmPackList, p_pack) {
        OmmPackData *pack = p_pack->as_ptr;
        if (pack && &pack->enabled == toggle) {
            return &pack->caching;
        }
    }
    return NULL;
}

u64 omm_models_precache_get_size() {
    u64 size = 0;
    if (gOmmModelPackCaching) {
        omm_array_for_each(gOmmPackList, p_pack) {
            OmmPackData *pack = p_pack->as_ptr;
            if (pack && pack->exists && pack->enabled) {
                pack->caching = OMM_MODELS_CACHING_ENABLED;
                if (pack->cs_index) {
                    size += omm_models_cs_get_size(pack->cs_index);
                } else {
                    for (s32 actor_index = 0; actor_index != omm_models_get_actor_count(); ++actor_index) {
                        const char *pack_folder = pack->path;
                        const char **actor_names = omm_models_get_actor_names(actor_index);
                        for (const char **actor_name = actor_names; *actor_name; ++actor_name) {
                            sys_path_t filename;
                            str_cat(filename, sizeof(filename), pack_folder, "/", *actor_name, ".bin");
                            s64 fsize = fs_fsize(filename);
                            if (fsize != -1) {
                                size += fsize;
                            }
                        }
                    }
                }
            }
        }
    }
    return size;
}
