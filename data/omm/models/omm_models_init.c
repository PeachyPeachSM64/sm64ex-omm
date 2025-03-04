#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <dirent.h>
#if !WINDOWS_BUILD
#include <strings.h>
#define stricmp strcasecmp
#endif
#define MODEL_PACK "model_pack"

OmmArray_(OmmActorGfx *) gOmmActorList = omm_array_zero;
OmmArray_(OmmPackData *) gOmmPackList = omm_array_zero;

static const char *omm_models_get_pack_name(const char *pack_folder) {
    const char *sep_l = strrchr(pack_folder, '/');
    const char *sep_w = strrchr(pack_folder, '\\');
    if (sep_l > sep_w) return sep_l + 1;
    if (sep_w) return sep_w + 1;
    return pack_folder;
}

static s32 omm_models_pack_list_cmp(const void *l, const void *r) {
    const OmmPackData *pack_l = (const OmmPackData *) ((const __OmmNoP *) l)->as_ptr;
    const OmmPackData *pack_r = (const OmmPackData *) ((const __OmmNoP *) r)->as_ptr;
    return stricmp(pack_l->name, pack_r->name);
}

static bool omm_models_register_packs(UNUSED void *user, const char *path) {

    // Character Select
    if (strstr(path, ".lua")) {
        omm_models_cs_init(path);
    }

    // Check if it's a bin file
    if (strstr(path, ".bin")) {

        // Retrieve the pack name
        const char *sep = strrchr(path, '/');
        if (sep) {
            sys_path_t pack_folder = {0};
            mem_cpy(pack_folder, path, sep - path);
            const char *pack_name = omm_models_get_pack_name(pack_folder);

            // Not a Model pack
            if (strcmp(pack_name, "actors") == 0) {
                return true;
            }

            // Check if the pack is already registered
            OmmPackData *existing_pack = NULL;
            omm_array_for_each(gOmmPackList, p_pack) {
                OmmPackData *pack = (OmmPackData *) p_pack->as_ptr;
                if (pack && strcmp(pack->name, pack_name) == 0) {
                    existing_pack = pack;
                    break;
                }
            }

            // Create pack
            if (!existing_pack) {
                OmmPackData *new_pack = mem_new(OmmPackData, 1);
                str_cpy(new_pack->name, sizeof(new_pack->name), pack_name);
                str_cpy(new_pack->path, sizeof(new_pack->path), pack_folder);
                new_pack->exists = true;
                new_pack->enabled = false;
                new_pack->cs_index = 0;
                omm_array_add(gOmmPackList, ptr, new_pack);
                omm_printf("Model pack found: \"%s\"\n",, pack_name);
            }

            // Load pack
            else if (!existing_pack->exists) {
                str_cpy(existing_pack->path, sizeof(existing_pack->path), pack_folder);
                existing_pack->exists = true;
                omm_printf("Model pack found: \"%s\"\n",, pack_name);
            }
        }
    }
    return true;
}

static const char *omm_models_encode_pack_name(sys_path_t dst, const char *pack_name) {
    mem_zero(dst, sizeof(sys_path_t));
    for (s32 i = 0; *pack_name && i != sizeof(sys_path_t) - 1; pack_name++) {
        if (isspace(*pack_name)) {
            dst[i++] = '/';
            dst[i++] = (*pack_name / 10) + '0';
            dst[i++] = (*pack_name % 10) + '0';
            dst[i++] = '/';
        } else {
            dst[i++] = *pack_name;
        }
    }
    return dst;
}

static const char *omm_models_decode_pack_name(sys_path_t dst, const char *pack_name) {
    mem_zero(dst, sizeof(sys_path_t));
    for (s32 i = 0; *pack_name && i != sizeof(sys_path_t) - 1; i++) {
        if (*pack_name == '/') {
            dst[i] = (pack_name[1] - '0') * 10 + (pack_name[2] - '0');
            pack_name += 4;
        } else {
            dst[i] = *(pack_name++);
        }
    }
    return dst;
}

const void **omm_models_init() {
    omm_log("Registering models packs\n");

    // Alloc and init the actors gfx list
    s32 actor_count = omm_models_get_actor_count();
    omm_array_grow(gOmmActorList, ptr, mem_new(OmmActorGfx, 1), actor_count);
    omm_array_for_each(gOmmActorList, p_actor) {
        OmmActorGfx *actor = (OmmActorGfx *) p_actor->as_ptr;
        actor->pack_index = -1;
        actor->gfx_data = NULL;
        actor->graph_node = (struct GraphNode *) geo_layout_to_graph_node(NULL, (const GeoLayout *) omm_models_get_actor_layout(i_p_actor));
    }

    // Scan the packs folders
    fs_walk("", omm_models_register_packs, NULL, true, FS_DIR_PACKS);

    // Register model packs from CS packs
    omm_models_cs_register_packs();

    // Sort packs by alphabetical order
    s32 pack_count = omm_array_count(gOmmPackList);
    qsort(gOmmPackList.p, pack_count, sizeof(__OmmNoP), omm_models_pack_list_cmp);

    // Return a list of pack names and toggles
    if (pack_count) {
        const char **pack_names = mem_new(const char *, pack_count + 1);
        bool **pack_toggles = mem_new(bool *, pack_count + 1);
        for (s32 i = 0, j = 0; i != pack_count; ++i) {
            OmmPackData *pack = (OmmPackData *) omm_array_get(gOmmPackList, ptr, i);
            if (pack->exists) {
                pack_names[j] = pack->name;
                pack_toggles[j] = &pack->enabled;
                j++;
            }
        }
        if (!*pack_names) {
            mem_del(pack_names);
            mem_del(pack_toggles);
            return NULL;
        }
        const void **packs = mem_new(const void *, 2);
        packs[0] = (const void *) pack_names;
        packs[1] = (const void *) pack_toggles;
        return packs;
    }
    return NULL;
}

bool omm_models_read_config(const char *name, const char *value) {
    if (strcmp(name, MODEL_PACK) == 0) {
        if (*value) {
            sys_path_t pack_name;
            omm_models_decode_pack_name(pack_name, value);

            // Check if the pack is already registered
            omm_array_for_each(gOmmPackList, p_pack) {
                OmmPackData *pack = (OmmPackData *) p_pack->as_ptr;
                if (pack && strcmp(pack->name, pack_name) == 0) {
                    pack->enabled = true;
                    return true;
                }
            }

            // Add pack to load later
            OmmPackData *pack = mem_new(OmmPackData, 1);
            str_cpy(pack->name, sizeof(pack->name), pack_name);
            pack->exists = false;
            pack->enabled = true;
            pack->cs_index = 0; // Can't tell for now if it's a CS pack
            omm_array_add(gOmmPackList, ptr, pack);
        }
        return true;
    }
    return false;
}

void omm_models_write_config(FILE *file) {
    omm_array_for_each(gOmmPackList, p_pack) {
        const OmmPackData *pack = (const OmmPackData *) p_pack->as_ptr;
        if (pack && (!pack->exists || pack->enabled)) {
            sys_path_t pack_name;
            omm_models_encode_pack_name(pack_name, pack->name);
            fprintf(file, MODEL_PACK " %s\n", pack_name);
        }
    }
}

s32 omm_models_get_model_pack_count() {
    s32 count = 0;
    omm_array_for_each(gOmmPackList, p_pack) {
        const OmmPackData *pack = (const OmmPackData *) p_pack->as_ptr;
        count += (pack != NULL && pack->exists);
    }
    return count;
}

s32 omm_models_get_model_pack_index(const void *geo_layout) {
    s32 actor_index = omm_models_get_actor_index(geo_layout);
    if (actor_index >= 0 && actor_index < omm_array_count(gOmmActorList)) {
        return ((const OmmActorGfx *) omm_array_get(gOmmActorList, ptr, actor_index))->pack_index;
    }
    return -1;
}

s32 omm_models_get_mario_model_pack_index() {
    return omm_models_get_model_pack_index(mario_geo);
}

#if OMM_GAME_IS_R96X

OMM_ROUTINE_UPDATE(omm_r96x_update_model_packs_names) {
    struct LanguageEntry *lang = get_language();
    if (lang) {
        omm_array_for_each(gOmmPackList, p_pack) {
            const OmmPackData *pack = (const OmmPackData *) p_pack->as_ptr;
            if (pack && get_key_string(pack->name) == lang->none) {
                add_key_value_string(lang, pack->name, pack->name);
            }
        }
    }
}

#endif
