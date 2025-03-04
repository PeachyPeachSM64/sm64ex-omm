#ifndef _SM64_DIRTREE_H_
#define _SM64_DIRTREE_H_

#include "types.h"

#define FS_NUMBUCKETS 64

// Virtual directory entry
typedef struct fs_dirtree_entry_s {
    const char *name;
    bool is_dir;
    struct fs_dirtree_entry_s *next_hash, *next_child, *next_sibling, *prev_sibling;
} fs_dirtree_entry_t;

// Virtual directory tree
typedef struct fs_dirtree_s {
    fs_dirtree_entry_t *root;
    fs_dirtree_entry_t *buckets[FS_NUMBUCKETS];
    u64 entry_len;
} fs_dirtree_t;

bool                fs_dirtree_init (fs_dirtree_t *tree, const u64 entry_len);
void                fs_dirtree_free (fs_dirtree_t *tree);
fs_dirtree_entry_t *fs_dirtree_add  (fs_dirtree_t *tree, char *name, const bool is_dir);
fs_dirtree_entry_t *fs_dirtree_find (fs_dirtree_t *tree, const char *name);
s32                 fs_dirtree_walk (void *tree, const char *base, walk_fn_t walkfn, void *user, const bool recur);

#endif // _SM64_DIRTREE_H_
