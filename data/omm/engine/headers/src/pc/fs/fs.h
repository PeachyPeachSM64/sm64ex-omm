#ifndef _SM64_FS_H_
#define _SM64_FS_H_

#include "types.h"
#include "pc/platform.h" // sys_exe_path, sys_user_path

// Save filename (not used, kept because compiling issues)
#define SAVE_FILENAME       "sm64_save_file.bin"

// Base ROM
#define BASEROM_SIZE        0x800000 // 8MB
#define BASEROM_EXT         ".z64"
#define BASEROM_FILENAME    "baserom.us" BASEROM_EXT

// Resources
#define FS_BASEPACK         "base." OMM_GAME_CODE ".zip"
#define FS_TEXTUREDIR       "gfx"
#define FS_SOUNDDIR         "sound"
#define FS_PACKSDIR         "packs"
#define FS_DYNOSDIR         "dynos"
#define FS_OMM_BASEPACK     "omm.zip"

// Dir flags
#define FS_DIR_READ         (1 << 0)
#define FS_DIR_WRITE        (1 << 1)
#define FS_DIR_PACKS        (1 << 2)

// Walk return codes
#define FS_WALK_SUCCESS     (0)
#define FS_WALK_INTERRUPTED (1)
#define FS_WALK_NOTFOUND    (2)
#define FS_WALK_ERROR       (4)

//
// Structs
//

// Filepath string
typedef char sys_path_t[SYS_MAX_PATH];

// Walk function signature
typedef bool (*walk_fn_t)(void *, const char *);

// Virtual directory handle (defined in fs.c)
typedef struct fs_dir_s fs_dir_t;

// Virtual file handle
typedef struct fs_file_s {
    void *handle;
    fs_dir_t *parent;
} fs_file_t;

// List of paths, returned by fs_enumerate()
typedef struct fs_pathlist_s {
    char **paths;
    s32 numpaths;
    s32 listcap;
} fs_pathlist_t;

// Packtype
typedef struct fs_packtype_s {
    const char  *extension;
    void      *(*mount)   (const char *rpath);
    void       (*unmount) (void *pack);
    s32        (*walk)    (void *pack, const char *base, walk_fn_t walkfn, void *user, const bool recur);
    bool       (*is_file) (void *pack, const char *path);
    bool       (*is_dir)  (void *pack, const char *path);
    fs_file_t *(*open)    (void *pack, const char *path);
    s64        (*read)    (void *pack, fs_file_t *file, void *buf, const u64 size);
    bool       (*seek)    (void *pack, fs_file_t *file, const s64 ofs);
    s64        (*tell)    (void *pack, fs_file_t *file);
    s64        (*size)    (void *pack, fs_file_t *file);
    bool       (*eof)     (void *pack, fs_file_t *file);
    void       (*close)   (void *pack, fs_file_t *file);
    s64        (*fsize)   (void *pack, const char *path);
} fs_packtype_t;

//
// Virtual file system
//

bool          fs_init            (const char **rodirs, const char *gamedir, const char *userdir);
bool          fs_mount           (const char *realpath, u8 flags);
bool          fs_unmount         (const char *realpath);
s32           fs_walk            (const char *base, walk_fn_t walkfn, void *user, const bool recur, u8 flags);
fs_pathlist_t fs_enumerate       (const char *base, const bool recur, u8 flags);
void          fs_pathlist_free   (fs_pathlist_t *pathlist);
bool          fs_is_file         (const char *fname);
bool          fs_is_dir          (const char *fname);
fs_file_t    *fs_open            (const char *vpath);
void          fs_close           (fs_file_t *file);
s64           fs_read            (fs_file_t *file, void *buf, const u64 size);
bool          fs_seek            (fs_file_t *file, const s64 ofs);
s64           fs_tell            (fs_file_t *file);
s64           fs_size            (fs_file_t *file);
bool          fs_eof             (fs_file_t *file);
s64           fs_fsize           (const char *vpath);
void         *fs_load_file       (const char *vpath, u64 *outsize);
u8           *fs_load_png        (const char *vpath, s32 *w, s32 *h);
const char   *fs_readline        (fs_file_t *file, char *dst, u64 size);
const char   *fs_find            (sys_path_t dst, const char *pattern, u8 flags);
const char   *fs_match           (sys_path_t dst, const char *prefix, u8 flags);
const char   *fs_convert_path    (sys_path_t dst, const char *path);
const char   *fs_cat_paths       (sys_path_t dst, const char *path1, const char *path2);
const char   *fs_get_game_path   (sys_path_t dst, const char *vpath);
const char   *fs_get_save_path   (sys_path_t dst, const char *vpath);
const char   *fs_get_write_path  (const char *vpath); // For compatibility, do not use

#if defined(AUDIO_LOAD_H) || (OMM_GAME_IS_R96X && defined(AUDIO_DATA_H))
extern u8 *rom_asset_load_sound_data(const char *name);
#define fs_load_file(name, ...) rom_asset_load_sound_data(name)
#endif

//
// Real file system
//

bool          fs_sys_walk        (const char *base, walk_fn_t walk, void *user, const bool recur);
fs_pathlist_t fs_sys_enumerate   (const char *base, const bool recur);
bool          fs_sys_file_exists (const char *name);
bool          fs_sys_dir_exists  (const char *name);
bool          fs_sys_mkdir       (const char *name);
bool          fs_sys_copy_file   (const char *from, const char *to);
bool          fs_sys_delete_file (const char *name);

#endif // _SM64_FS_H_
