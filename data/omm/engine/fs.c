#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <sys/stat.h>
#include <dirent.h>
#if WINDOWS_BUILD
#include <windows.h>
#include <direct.h>
#endif

static sys_path_t fs_gamedir;
static sys_path_t fs_gamepath;
static sys_path_t fs_savepath;

//
// Packers
//

extern fs_packtype_t fs_packtype_dir;
extern fs_packtype_t fs_packtype_zip;
static fs_packtype_t *fs_packers[] = {
    &fs_packtype_dir,
    &fs_packtype_zip,
};

//
// Directories
//

struct fs_dir_s {
    u8 flags;
    void *pack;
    const char *realpath;
    fs_packtype_t *packer;
    struct fs_dir_s *prev, *next;
};
static fs_dir_t *fs_searchpaths = NULL;
#define for_each_dir_in_search_paths for (fs_dir_t *dir = fs_searchpaths; dir; dir = dir->next)

static fs_dir_t *fs_dir_find(const char *realpath) {
    for_each_dir_in_search_paths {
        if (!sys_strcasecmp(realpath, dir->realpath)) {
            return dir;
        }
    }
    return NULL;
}

static void fs_dir_scan(const char *ropath, const char *dir, u8 flags) {
    sys_path_t dirpath;
    fs_cat_paths(dirpath, ropath, dir);
    if (fs_sys_dir_exists(dirpath)) {
        fs_pathlist_t plist = fs_sys_enumerate(dirpath, false);
        for (s32 i = 0; i != plist.numpaths; ++i) {
            fs_mount(plist.paths[i], flags);
        }
        fs_pathlist_free(&plist);
        fs_mount(dirpath, flags);
    }
}

static bool fs_dir_is_custom_zip(fs_dir_t *dir) {
    return dir->packer == &fs_packtype_zip && !(strstr(dir->realpath, FS_BASEPACK) || strstr(dir->realpath, FS_OMM_BASEPACK));
}

static bool fs_dir_is_base_zip(fs_dir_t *dir) {
    return dir->packer == &fs_packtype_zip && (strstr(dir->realpath, FS_BASEPACK) || strstr(dir->realpath, FS_OMM_BASEPACK));
}

static bool fs_dir_is_dir(fs_dir_t *dir) {
    return dir->packer == &fs_packtype_dir;
}

typedef bool (*fs_dir_f)(fs_dir_t *);
static fs_dir_f fs_dir_types[] = {
    fs_dir_is_dir,
    fs_dir_is_base_zip,
    fs_dir_is_custom_zip,
};

//
// Find
//

struct finddata_s {
    const char *pattern;
    char *dst;
};

static bool fs_find_walk(void *user, const char *path) {
    struct finddata_s *data = (struct finddata_s *) user;
    if (strstr(path, data->pattern)) {
        str_cpy(data->dst, sizeof(sys_path_t), path);
        return false;
    }
    return true;
}

//
// Match
//

struct matchdata_s {
    const char *prefix;
    u64 prefix_len;
    char *dst;
};

static bool fs_match_walk(void *user, const char *path) {
    struct matchdata_s *data = (struct matchdata_s *) user;
    if (!strncmp(path, data->prefix, data->prefix_len)) {
        str_cpy(data->dst, sizeof(sys_path_t), path);
        return false;
    }
    return true;
}

//
// Enumerate
//

static bool fs_enumerate_walk(void *user, const char *path) {
    fs_pathlist_t *data = (fs_pathlist_t *) user;
    if (data->listcap == data->numpaths) {
        data->listcap *= 2;
        char **newpaths = realloc(data->paths, data->listcap * sizeof(char *));
        if (!newpaths) return false;
        data->paths = newpaths;
    }
    data->paths[data->numpaths++] = str_dup(path);
    return true;
}

//
// File system
//

// __argc, __argv are Windows only
// The following only works on non-Windows platforms
#if !WINDOWS_BUILD

static int __argc;
static char **__argv;

OMM_AT_STARTUP static void fs_init_args(int argc, char *argv[]) {
    __argc = argc;
    __argv = argv;
}

#endif

static void fs_init_paths() {
    OMM_DO_ONCE {
        parse_cli_opts(__argc, __argv);
        fs_convert_path(fs_gamedir, *gCLIOpts.gamedir ? gCLIOpts.gamedir : FS_BASEDIR);
        fs_convert_path(fs_savepath, *gCLIOpts.savepath ? gCLIOpts.savepath : sys_user_path());
        fs_cat_paths(fs_gamepath, sys_exe_path(), fs_gamedir);
        omm_log("Game directory set to \"%s\"\n",, fs_gamedir);
        omm_log("Save path set to \"%s\"\n",, fs_savepath);
    }
}

#define fs_mount_dirs(path, flags, dir1, dir2) { \
    sys_path_t basepath, dirpath; \
    fs_convert_path(basepath, path); \
    fs_cat_paths(dirpath, dir1, dir2); \
    fs_dir_scan(basepath, dirpath, flags); \
}

bool fs_init(UNUSED const char **rodirs, UNUSED const char *gamedir, UNUSED const char *writepath) {
    OMM_DO_ONCE {
        fs_init_paths();
        omm_log("Initializing file system\n");

        // Mount fs_gamedir and packs dirs from exe path
        fs_mount_dirs("!", FS_DIR_READ, fs_gamedir, "");
        fs_mount_dirs("!", FS_DIR_PACKS, fs_gamedir, FS_PACKSDIR);
        fs_mount_dirs("!", FS_DIR_PACKS, FS_DYNOSDIR, FS_PACKSDIR);

        // Mount res and packs dirs from save path
        fs_mount_dirs(fs_savepath, FS_DIR_READ, FS_BASEDIR, "");
        fs_mount_dirs(fs_savepath, FS_DIR_PACKS, FS_BASEDIR, FS_PACKSDIR);
        fs_mount_dirs(fs_savepath, FS_DIR_PACKS, FS_DYNOSDIR, FS_PACKSDIR);

        // Mount save path
        fs_mount(fs_savepath, FS_DIR_WRITE);

        // Sort fs_searchpaths:
        // - custom zips are first
        // - base.*.zip and omm.zip are next
        // - directories are last
        fs_dir_t *paths = NULL;
        array_for_each_(fs_dir_f, fs_dir_is, fs_dir_types) {
            for_each_dir_in_search_paths {
                if ((*fs_dir_is)(dir)) {
                    fs_dir_t *path = mem_dup(dir, sizeof(fs_dir_t));
                    path->prev = NULL;
                    path->next = paths;
                    if (paths) paths->prev = path;
                    paths = path;
                    omm_printf("Mounting: \"%s\"\n",, path->realpath);
                }
            }
        }
        fs_searchpaths = paths;

        // Other inits
        omm_audio_init();
#if OMM_GAME_IS_R96X
        extern void omm_r96x_generate_json();
        omm_r96x_generate_json();
#endif
    }

    // Done
    return true;
}

static const char *FS_GAME_CODES[] = {
#define OMM_GAME(code, ...) "." code ".",
#include "data/omm/omm_games.inl"
#undef OMM_GAME
};

bool fs_mount(const char *realpath, u8 flags) {
    if (!fs_dir_find(realpath)) {
        const char *ext = sys_file_extension(realpath);
        fs_packtype_t *packer = NULL;
        void *pack = NULL;

        // Select packer
        for (u32 i = 0; i != array_length(fs_packers); ++i) {
            if (!ext || !sys_strcasecmp(ext, fs_packers[i]->extension)) {
                pack = fs_packers[i]->mount(realpath);
                if (pack) {
                    packer = fs_packers[i];
                    break;
                }
            }
        }

        // Add to search paths
        if (pack && packer) {

            // Ignore packs intended for other games
            if (packer == &fs_packtype_zip) {
                const char *filename = sys_file_name(realpath);
                for (u32 i = 0; i != array_length(FS_GAME_CODES); ++i) {
                    if (!strstr(FS_GAME_CODES[i], OMM_GAME_CODE) && strstr(filename, FS_GAME_CODES[i])) {
                        omm_printf_warning("Skipping: \"%s\"\n",, filename);
                        packer->unmount(pack);
                        return false;
                    }
                }
            }

            // Add path to search paths
            fs_dir_t *dir = mem_new(fs_dir_t, 1);
            if (OMM_LIKELY(dir)) {
                dir->flags = flags;
                dir->pack = pack;
                dir->realpath = str_dup(realpath);
                dir->packer = packer;
                dir->prev = NULL;
                dir->next = fs_searchpaths;
                if (fs_searchpaths) fs_searchpaths->prev = dir;
                fs_searchpaths = dir;
                return true;
            }
            packer->unmount(pack);
        }
    }
    return false;
}

bool fs_unmount(const char *realpath) {
    fs_dir_t *dir = fs_dir_find(realpath);
    if (dir) {
        dir->packer->unmount(dir->pack);
        mem_del(dir->realpath);
        if (dir->prev) dir->prev->next = dir->next;
        if (dir->next) dir->next->prev = dir->prev;
        if (dir == fs_searchpaths) fs_searchpaths = dir->next;
        mem_del(dir);
        return true;
    }
    return false;
}

s32 fs_walk(const char *base, walk_fn_t walkfn, void *user, const bool recur, u8 flags) {
    bool found = false;
    for_each_dir_in_search_paths {
        if (dir->flags & flags) {
            s32 res = dir->packer->walk(dir->pack, base, walkfn, user, recur);
            if (res == FS_WALK_INTERRUPTED) return res;
            if (res != FS_WALK_NOTFOUND) found = true;
        }
    }
    return found ? FS_WALK_SUCCESS : FS_WALK_NOTFOUND;
}

bool fs_is_file(const char *fname) {
    for_each_dir_in_search_paths {
        if (dir->packer->is_file(dir->pack, fname)) {
            return true;
        }
    }
    return false;
}

bool fs_is_dir(const char *fname) {
    for_each_dir_in_search_paths {
        if (dir->packer->is_dir(dir->pack, fname)) {
            return true;
        }
    }
    return false;
}

fs_file_t *fs_open(const char *vpath) {
    for_each_dir_in_search_paths {
        fs_file_t *f = dir->packer->open(dir->pack, vpath);
        if (f) {
            f->parent = dir;
            return f;
        }
    }
    return NULL;
}

void fs_close(fs_file_t *file) {
    if (file) {
        file->parent->packer->close(file->parent->pack, file);
    }
}

s64 fs_read(fs_file_t *file, void *buf, const u64 size) {
    if (file) {
        return file->parent->packer->read(file->parent->pack, file, buf, size);
    }
    return -1;
}

bool fs_seek(fs_file_t *file, const s64 ofs) {
    if (file) {
        return file->parent->packer->seek(file->parent->pack, file, ofs);
    }
    return false;
}

s64 fs_tell(fs_file_t *file) {
    if (file) {
        return file->parent->packer->tell(file->parent->pack, file);
    }
    return -1;
}

s64 fs_size(fs_file_t *file) {
    if (file) {
        return file->parent->packer->size(file->parent->pack, file);
    }
    return -1;
}

bool fs_eof(fs_file_t *file) {
    if (file) {
        return file->parent->packer->eof(file->parent->pack, file);
    }
    return true;
}

s64 fs_fsize(const char *vpath) {
    for_each_dir_in_search_paths {
        s64 fsize = dir->packer->fsize(dir->pack, vpath);
        if (fsize >= 0) {
            return fsize;
        }
    }
    return -1;
}

const char *fs_find(sys_path_t dst, const char *pattern, u8 flags) {
    struct finddata_s data = {
        .pattern = pattern,
        .dst = dst,
    };
    if (fs_walk("", fs_find_walk, &data, true, flags) == FS_WALK_INTERRUPTED) {
        return dst;
    }
    return NULL;
}

const char *fs_match(sys_path_t dst, const char *prefix, u8 flags) {
    struct matchdata_s data = {
        .prefix = prefix,
        .prefix_len = strlen(prefix),
        .dst = dst,
    };
    if (fs_walk("", fs_match_walk, &data, true, flags) == FS_WALK_INTERRUPTED) {
        return dst;
    }
    return NULL;
}

fs_pathlist_t fs_enumerate(const char *base, const bool recur, u8 flags) {
    char **paths = mem_new(char *, 32);
    fs_pathlist_t pathlist = { paths, 0, 32 };
    if (paths && fs_walk(base, fs_enumerate_walk, &pathlist, recur, flags) == FS_WALK_INTERRUPTED) {
        fs_pathlist_free(&pathlist);
    }
    return pathlist;
}

void fs_pathlist_free(fs_pathlist_t *pathlist) {
    if (pathlist && pathlist->paths) {
        for (s32 i = 0; i < pathlist->numpaths; ++i) {
            mem_del(pathlist->paths[i]);
        }
        mem_del(pathlist->paths);
        pathlist->paths = NULL;
        pathlist->numpaths = 0;
    }
}

const char *fs_readline(fs_file_t *file, char *dst, u64 size) {
    s64 rx = 0;
    char chr, *p;
    for (p = dst, size--; size; size--) {
        if ((rx = fs_read(file, &chr, 1)) <= 0) break;
        *p++ = chr;
        if (chr == '\n') break;
    }
    *p = 0;
    return (p == dst || rx <= 0) ? NULL : p;
}

void *fs_load_file(const char *vpath, u64 *outsize) {
    void *buf = NULL;
    fs_file_t *f = fs_open(vpath);
    if (f) {
        s64 size = fs_size(f);
        if (OMM_LIKELY(size > 0)) {
            buf = (void *) mem_new(u8, size);
            if (OMM_LIKELY(buf)) {
                s64 rx = fs_read(f, buf, size);
                if (rx == size) {
                    if (outsize) *outsize = size;
                } else {
                    mem_del(buf);
                }
            }
        }
        fs_close(f);
    }
    return buf;
}

u8 *fs_load_png(const char *vpath, s32 *w, s32 *h) {
    u64 size;
    u8 *file = fs_load_file(vpath, &size);
    if (file) {
        u8 *data = stbi_load_from_memory(file, size, w, h, NULL, 4);
        mem_del(file);
        return data;
    }
    return NULL;
}

const char *fs_convert_path(sys_path_t dst, const char *path)  {
    if (path[0] == '!') {
        str_cat(dst, sizeof(sys_path_t), sys_exe_path(), path + 1);
    } else {
        str_cpy(dst, sizeof(sys_path_t), path);
    }
    str_rep(dst, sizeof(sys_path_t), dst, '\\', '/');
    return dst;
}

const char *fs_cat_paths(sys_path_t dst, const char *path1, const char *path2) {

    // Sanitize path1, remove the trailing separators
    sys_path_t cpath1;
    fs_convert_path(cpath1, path1);
    for (char *ppath1 = cpath1 + strlen(cpath1) - 1; ppath1 >= cpath1 && *ppath1 == '/'; *(ppath1--) = 0);

    // Sanitize path2, remove the heading separators
    sys_path_t cpath2;
    fs_convert_path(cpath2, path2);
    char *ppath2 = cpath2; for (; *ppath2 == '/'; ++ppath2);

    // Concatenate the two, separate them by a slash
    if (*cpath1 && *ppath2) {
        str_cat(dst, sizeof(sys_path_t), cpath1, "/", ppath2);
    } else {
        str_cat(dst, sizeof(sys_path_t), cpath1, ppath2);
    }
    return dst;
}

const char *fs_get_game_path(sys_path_t dst, const char *vpath) {
    fs_init_paths();
    return fs_cat_paths(dst, fs_gamepath, vpath);
}

const char *fs_get_save_path(sys_path_t dst, const char *vpath) {
    fs_init_paths();
    return fs_cat_paths(dst, fs_savepath, vpath);
}

const char *fs_get_write_path(const char *vpath) {
    return vpath;
}

//
// OS File System
//

bool fs_sys_file_exists(const char *name) {
    struct stat st;
    return stat(name, &st) == 0 && S_ISREG(st.st_mode);
}

bool fs_sys_dir_exists(const char *name) {
    struct stat st;
    return stat(name, &st) == 0 && S_ISDIR(st.st_mode);
}

bool fs_sys_walk(const char *base, walk_fn_t walk, void *user, const bool recur) {
    DIR *dir = opendir(base);
    if (dir) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] != 0 && ent->d_name[0] != '.') {
                sys_path_t fullpath;
                fs_cat_paths(fullpath, base, ent->d_name);
                if (fs_sys_dir_exists(fullpath)) {
                    if (recur && !fs_sys_walk(fullpath, walk, user, recur)) {
                        closedir(dir);
                        return false;
                    }
                } else if (!walk(user, fullpath)) {
                    closedir(dir);
                    return false;
                }
            }
        }
        closedir(dir);
        return true;
    }
    return false;
}

fs_pathlist_t fs_sys_enumerate(const char *base, const bool recur) {
    char **paths = mem_new(char *, 32);
    fs_pathlist_t pathlist = { paths, 0, 32 };
    if (paths && !fs_sys_walk(base, fs_enumerate_walk, &pathlist, recur)) {
        fs_pathlist_free(&pathlist);
    }
    return pathlist;
}

bool fs_sys_mkdir(const char *name) {
#if WINDOWS_BUILD
    return _mkdir(name) == 0;
#else
    return mkdir(name, 0777) == 0;
#endif
}

bool fs_sys_copy_file(const char *from, const char *to) {
    bool success = false;
    FILE *f_from = fopen(from, "rb");
    if (f_from) {
        FILE *f_to = fopen(to, "wb");
        if (f_to) {
            u8 buf[BUFSIZ];
            u64 bytes;
            for (success = true; (bytes = fread(buf, 1, sizeof(buf), f_from)) > 0;) {
                if (fwrite(buf, 1, bytes, f_to) < bytes) {
                    success = false;
                    break;
                }
            }
            fclose(f_to);
        }
        fclose(f_from);
    }
    return success;
}

bool fs_sys_delete_file(const char *name) {
    return remove(name) == 0;
}
