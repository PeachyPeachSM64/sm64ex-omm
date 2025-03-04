#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static u32 dirtree_hash(const char *s, u64 len) {
    u32 hash = 0x1505;
    while (len--) hash = ((hash << 5) + hash) ^ *(s++);
    return hash & (FS_NUMBUCKETS - 1);
}

static fs_dirtree_entry_t *dirtree_add_ancestors(fs_dirtree_t *tree, char *name) {
    fs_dirtree_entry_t *ent = tree->root;
    char *last_sep = strrchr(name, '/');
    if (last_sep) {
        *last_sep = 0;
        ent = fs_dirtree_find(tree, name);
        if (!ent) {
            ent = fs_dirtree_add(tree, name, true);
        }
        *last_sep = '/';
    }
    return ent;
}

// Due to how linked lists work, files are walked through in reverse order
static fs_dirtree_entry_t *get_last_entry(fs_dirtree_entry_t *ent) {
    if (ent) {
        ent->prev_sibling = NULL;
        for (;; ent = ent->next_sibling) {
            if (!ent->next_sibling) {
                return ent;
            }
            ent->next_sibling->prev_sibling = ent;
        }
    }
    return NULL;
}

static s32 dirtree_walk_impl(fs_dirtree_entry_t *ent, walk_fn_t walkfn, void *user, const bool recur) {
    s32 res = FS_WALK_SUCCESS;
    for (ent = get_last_entry(ent->next_child); ent && res == FS_WALK_SUCCESS; ent = ent->prev_sibling) {
    // for (ent = ent->next_child; ent && res == FS_WALK_SUCCESS; ent = ent->next_sibling) {
        if (ent->is_dir && recur && ent->next_child) {
            res = dirtree_walk_impl(ent, walkfn, user, recur);
        } else if (!ent->is_dir && !walkfn(user, ent->name)) {
            return FS_WALK_INTERRUPTED;
        }
    }
    return res;
}

//
// Dir Tree
//

bool fs_dirtree_init(fs_dirtree_t *tree, const u64 entry_len) {
    mem_zero(tree, sizeof(fs_dirtree_t));
    tree->root = (fs_dirtree_entry_t *) mem_new(u8, entry_len);
    if (OMM_LIKELY(tree->root)) {
        tree->root->name = "";
        tree->root->is_dir = true;
        tree->entry_len = entry_len;
        return true;
    }
    return false;
}

void fs_dirtree_free(fs_dirtree_t *tree) {
    if (tree) {
        mem_del(tree->root);
        for (s32 i = 0; i != FS_NUMBUCKETS; ++i) {
            for (fs_dirtree_entry_t *next, *ent = tree->buckets[i]; ent; ent = next) {
                next = ent->next_hash;
                mem_del(ent);
            }
        }
    }
}

static char *strrstr(char *str, const char *substr) {
    for (char *res = NULL;; str++) {
        str = strstr(str, substr);
        if (!str) {
            return res;
        }
        res = str;
    }
}

static char *get_shortened_name(char *name) {
    static const char *sResourceDirs[] = {
        "/" FS_TEXTUREDIR "/",
        "/" FS_SOUNDDIR "/",
    };
    array_for_each_(const char *, dir, sResourceDirs) {
        char *res = strrstr(name, *dir);
        if (res) {
            return res + 1;
        }
    }
    return name;
}

static bool should_shorten_name(const char *name) {
    return (
        strstr(name, ".png") ||
        strstr(name, ".wav") ||
        strstr(name, "/bank_sets") ||
        strstr(name, "/sequences.bin") ||
        strstr(name, "/sound_data.ctl") ||
        strstr(name, "/sound_data.tbl")
    );
}

fs_dirtree_entry_t *fs_dirtree_add(fs_dirtree_t *tree, char *name, const bool is_dir) {
    if (should_shorten_name(name)) {
        name = get_shortened_name(name);
    }
    fs_dirtree_entry_t *ent = fs_dirtree_find(tree, name);
    if (!ent) {
        fs_dirtree_entry_t *parent = dirtree_add_ancestors(tree, name);
        if (parent) {
            const u64 name_len = strlen(name);
            const u64 allocsize = tree->entry_len + name_len + 1;
            ent = (fs_dirtree_entry_t *) mem_new(u8, allocsize);
            if (OMM_LIKELY(ent)) {
                ent->name = (const char *) ent + tree->entry_len;
                strcpy((char *) ent->name, name);
                const u32 hash = dirtree_hash(name, name_len);
                ent->next_hash = tree->buckets[hash];
                tree->buckets[hash] = ent;
                ent->next_sibling = parent->next_child;
                ent->is_dir = is_dir;
                parent->next_child = ent;
            }
        }
    }
    return ent;
}

fs_dirtree_entry_t *fs_dirtree_find(fs_dirtree_t *tree, const char *name) {
    if (name) {
        if (*name) {
            const u32 hash = dirtree_hash(name, strlen(name));
            for (fs_dirtree_entry_t *prev = NULL, *ent = tree->buckets[hash]; ent; ent = ent->next_hash) {
                if (strcmp(ent->name, name) == 0) {
                    if (prev) {
                        prev->next_hash = ent->next_hash;
                        ent->next_hash = tree->buckets[hash];
                        tree->buckets[hash] = ent;
                    }
                    return ent;
                }
                prev = ent;
            }
            return NULL;
        }
        return tree->root;
    }
    return NULL;
}

s32 fs_dirtree_walk(void *pack, const char *base, walk_fn_t walkfn, void *user, const bool recur) {
    fs_dirtree_t *tree = (fs_dirtree_t *) pack;
    fs_dirtree_entry_t *ent = fs_dirtree_find(tree, base);
    if (ent) {
        return dirtree_walk_impl(ent, walkfn, user, recur);
    }
    return FS_WALK_NOTFOUND;
}
