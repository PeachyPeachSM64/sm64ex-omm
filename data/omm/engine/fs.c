#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <sys/stat.h>
#include <dirent.h>
#if WINDOWS_BUILD
#include <direct.h>
#endif

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

char fs_gamedir[SYS_MAX_PATH] = "";
char fs_writepath[SYS_MAX_PATH] = "";

struct fs_dir_s {
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

static void fs_dir_scan(const char *ropath, const char *dir) {
    omm_cat_paths(dirpath, SYS_MAX_PATH, ropath, dir);
    if (fs_sys_dir_exists(dirpath)) {
        fs_pathlist_t plist = fs_sys_enumerate(dirpath, false);
        for (s32 i = 0; i != plist.numpaths; ++i) {
            fs_mount(plist.paths[i]);
        }
        fs_pathlist_free(&plist);
        fs_mount(dirpath);
    }
}

static bool fs_dir_is_custom_zip(fs_dir_t *dir) {
    return dir->packer == &fs_packtype_zip && !strstr(dir->realpath, FS_BASEPACK_PREFIX);
}

static bool fs_dir_is_base_zip(fs_dir_t *dir) {
    return dir->packer == &fs_packtype_zip && strstr(dir->realpath, FS_BASEPACK_PREFIX);
}

static bool fs_dir_is_dir(fs_dir_t *dir) {
    return dir->packer == &fs_packtype_dir;
}

typedef bool (*fs_dir_f)(fs_dir_t *);
static fs_dir_f fs_dir_types[] = {
    fs_dir_is_dir,
    fs_dir_is_base_zip,
    fs_dir_is_custom_zip,
    NULL,
};

//
// Find
//

struct finddata_s {
    const char *pattern;
    char *dst;
    u64 dst_len;
};

static bool fs_find_walk(void *user, const char *path) {
    struct finddata_s *data = (struct finddata_s *) user;
    if (strstr(path, data->pattern)) {
        str_cpy(data->dst, data->dst_len, path);
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
    u64 dst_len;
};

static bool fs_match_walk(void *user, const char *path) {
    struct matchdata_s *data = (struct matchdata_s *) user;
    if (!strncmp(path, data->prefix, data->prefix_len)) {
        str_cpy(data->dst, data->dst_len, path);
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
    data->paths[data->numpaths++] = sys_strdup(path);
    return true;
}

//
// File system
//

bool fs_init(const char **rodirs, const char *gamedir, const char *writepath) {
    char buf[SYS_MAX_PATH];

    // Game dir
    fs_convert_path(fs_gamedir, SYS_MAX_PATH, gamedir);
    omm_log("Gamedir set to '%s'\n",, fs_gamedir);

    // Write path
    fs_convert_path(fs_writepath, SYS_MAX_PATH, writepath);
    omm_log("Writepath set to '%s'\n",, fs_writepath);

    // Mount all dirs from rodirs
    for (const char **p = rodirs; p && *p; ++p) {
        fs_dir_scan(fs_convert_path(buf, sizeof(buf), *p), FS_BASEDIR);
    }
    fs_dir_scan(fs_writepath, FS_BASEDIR);

    // Do the same if the game dir isn't the 'res' directory
    if (sys_strcasecmp(FS_BASEDIR, fs_gamedir)) {
        for (const char **p = rodirs; p && *p; ++p) {
            fs_dir_scan(fs_convert_path(buf, sizeof(buf), *p), fs_gamedir);
        }
        fs_dir_scan(fs_writepath, fs_gamedir);
    }

    // Finally, mount writepath
    fs_mount(fs_writepath);
    
    // Sort fs_searchpaths:
    // - custom zips are first
    // - base.zip are next
    // - directories are last
    fs_dir_t *paths = NULL;
    for_each_until_null(fs_dir_f, fs_dir_is, fs_dir_types) {
        for_each_dir_in_search_paths {
            if ((*fs_dir_is)(dir)) {
                fs_dir_t *path = omm_dup(dir, sizeof(fs_dir_t));
                path->prev = NULL;
                path->next = paths;
                if (paths) paths->prev = path;
                paths = path;
                omm_log("Mounting '%s'\n",, path->realpath);
            }
        }
    }
    fs_searchpaths = paths;

    // Other inits
    omm_audio_init();

    // Done
    return true;
}

bool fs_mount(const char *realpath) {
    if (!fs_dir_find(realpath)) {
        const char *ext = sys_file_extension(realpath);
        fs_packtype_t *packer = NULL;
        void *pack = NULL;

        // Select packer
        for (u32 i = 0; i != omm_static_array_length(fs_packers); ++i) {
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
            fs_dir_t *dir = omm_new(fs_dir_t, 1);
            if (OMM_LIKELY(dir)) {
                dir->pack = pack;
                dir->realpath = sys_strdup(realpath);
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
        omm_free(dir->realpath);
        if (dir->prev) dir->prev->next = dir->next;
        if (dir->next) dir->next->prev = dir->prev;
        if (dir == fs_searchpaths) fs_searchpaths = dir->next;
        omm_free(dir);
        return true;
    }
    return false;
}

s32 fs_walk(const char *base, walk_fn_t walkfn, void *user, const bool recur) {
    bool found = false;
    for_each_dir_in_search_paths {
        s32 res = dir->packer->walk(dir->pack, base, walkfn, user, recur);
        if (res == FS_WALK_INTERRUPTED) return res;
        if (res != FS_WALK_NOTFOUND) found = true;
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
    return -1;
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

const char *fs_find(char *outname, const u64 outlen, const char *pattern) {
    struct finddata_s data = {
        .pattern = pattern,
        .dst = outname,
        .dst_len = outlen,
    };
    if (fs_walk("", fs_find_walk, &data, true) == FS_WALK_INTERRUPTED) {
        return outname;
    }
    return NULL;
}

const char *fs_match(char *outname, const u64 outlen, const char *prefix) {
    struct matchdata_s data = {
        .prefix = prefix,
        .prefix_len = strlen(prefix),
        .dst = outname,
        .dst_len = outlen,
    };
    if (fs_walk("", fs_match_walk, &data, true) == FS_WALK_INTERRUPTED) {
        return outname;
    }
    return NULL;
}

fs_pathlist_t fs_enumerate(const char *base, const bool recur) {
    char **paths = omm_new(char *, 32);
    fs_pathlist_t pathlist = { paths, 0, 32 };
    if (paths && fs_walk(base, fs_enumerate_walk, &pathlist, recur) == FS_WALK_INTERRUPTED) {
        fs_pathlist_free(&pathlist);
    }
    return pathlist;
}

void fs_pathlist_free(fs_pathlist_t *pathlist) {
    if (pathlist && pathlist->paths) {
        for (s32 i = 0; i < pathlist->numpaths; ++i) {
            omm_free(pathlist->paths[i]);
        }
        omm_free(pathlist->paths);
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
            buf = (void *) omm_new(u8, size);
            if (OMM_LIKELY(buf)) {
                s64 rx = fs_read(f, buf, size);
                if (rx == size) {
                    if (outsize) *outsize = size;
                } else {
                    omm_free(buf);
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
        omm_free(file);
        return data;
    }
    return NULL;
}

const char *fs_get_write_path(const char *vpath) {
    static char path[SYS_MAX_PATH];
    fs_cat_paths(path, sizeof(path), fs_writepath, vpath);
    return path;
}

const char *fs_convert_path(char *buf, const u64 bufsiz, const char *path)  {
    if (path[0] == '!') {
        str_cat(buf, bufsiz, sys_exe_path(), path + 1);
    } else {
        str_cpy(buf, bufsiz, path);
    }
    str_rep(buf, '\\', '/');
    return buf;
}

const char *fs_cat_paths(char *buf, const u64 bufsiz, const char *path1, const char *path2) {

    // Sanitize path1, remove the trailing separators
    char cpath1[SYS_MAX_PATH];
    fs_convert_path(cpath1, SYS_MAX_PATH, path1);
    for (char *ppath1 = cpath1 + strlen(cpath1) - 1; ppath1 >= cpath1 && *ppath1 == '/'; *(ppath1--) = 0);

    // Sanitize path2, remove the heading separators
    char cpath2[SYS_MAX_PATH];
    fs_convert_path(cpath2, SYS_MAX_PATH, path2);
    char *ppath2 = cpath2; for (; *ppath2 == '/'; ++ppath2);

    // Concatenate the two, separate them by a slash
    if (*cpath1 && *ppath2) {
        str_cat(buf, bufsiz, cpath1, "/", ppath2);
    } else {
        str_cat(buf, bufsiz, cpath1, ppath2);
    }
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
                omm_cat_paths(fullpath, SYS_MAX_PATH, base, ent->d_name);
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
    char **paths = omm_new(char *, 32);
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

bool fs_sys_copy_file(const char *oldname, const char *newname) {
    bool ret = false;
    FILE *fin = fopen(oldname, "rb");
    if (fin) {
        FILE *fout = fopen(newname, "wb");
        if (fout) {
            u8 buf[0x1000];
            u64 rx;
            for (ret = true; (rx = fread(buf, 1, sizeof(buf), fin)) > 0;) {
                if (!fwrite(buf, rx, 1, fout)) {
                    ret = false;
                    break;
                }
            }
            fclose(fout);
        }
        fclose(fin);
    }
    return ret;
}
