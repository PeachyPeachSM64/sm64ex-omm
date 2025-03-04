#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <sys/stat.h>

struct walkdata_s {
    u64 baselen;
    walk_fn_t userwalk;
    void *userdata;
};

static bool packdir_walkfn(void *userdata, const char *path) {
    struct walkdata_s *walk = (struct walkdata_s *) userdata;
    return walk->userwalk(walk->userdata, path + walk->baselen);
}

//
// Packtype: DIR
//

static void *fs_packtype_dir_mount(const char *realpath) {
    if (fs_sys_dir_exists(realpath)) {
        void *pack = (void *) str_dup(realpath);
        return pack;
    }
    return NULL;
}

static void fs_packtype_dir_unmount(void *pack) {
    mem_del(pack);
}

static s32 fs_packtype_dir_walk(void *pack, const char *base, walk_fn_t walkfn, void *user, const bool recur) {
    sys_path_t path;
    fs_cat_paths(path, (const char *) pack, base);
    if (fs_sys_dir_exists(path)) {
        struct walkdata_s walkdata = { strlen((const char *) pack) + 1, walkfn, user };
        if (fs_sys_walk(path, packdir_walkfn, &walkdata, recur)) {
            return FS_WALK_SUCCESS;
        }
        return FS_WALK_INTERRUPTED;
    }
    return FS_WALK_NOTFOUND;
}

static bool fs_packtype_dir_is_file(void *pack, const char *fname) {
    sys_path_t path;
    fs_cat_paths(path, (const char *) pack, fname);
    return fs_sys_file_exists(path);
}

static bool fs_packtype_dir_is_dir(void *pack, const char *fname) {
    sys_path_t path;
    fs_cat_paths(path, (const char *) pack, fname);
    return fs_sys_dir_exists(path);
}

static fs_file_t *fs_packtype_dir_open(void *pack, const char *vpath) {
    sys_path_t path;
    fs_cat_paths(path, (const char *) pack, vpath);
    FILE *f = fopen(path, "rb");
    if (f) {
        fs_file_t *fsfile = mem_new(fs_file_t, 1);
        if (OMM_LIKELY(fsfile)) {
            fsfile->parent = NULL;
            fsfile->handle = f;
            return fsfile;
        }
        fclose(f);
    }
    return NULL;
}

static void fs_packtype_dir_close(UNUSED void *pack, fs_file_t *file) {
    fclose((FILE *) file->handle);
    mem_del(file);
}

static s64 fs_packtype_dir_read(UNUSED void *pack, fs_file_t *file, void *buf, const u64 size) {
    return fread(buf, 1, size, (FILE *) file->handle);
}

static bool fs_packtype_dir_seek(UNUSED void *pack, fs_file_t *file, const s64 ofs) {
    return fseeko((FILE *) file->handle, ofs, SEEK_SET) == 0;
}

static s64 fs_packtype_dir_tell(UNUSED void *pack, fs_file_t *file) {
    return ftello((FILE *) file->handle);
}

static s64 fs_packtype_dir_size(UNUSED void *pack, fs_file_t *file) {
    s64 oldofs = ftello((FILE *) file->handle);
    fseeko((FILE *) file->handle, 0, SEEK_END);
    s64 size = ftello((FILE *) file->handle);
    fseeko((FILE *) file->handle, oldofs, SEEK_SET);
    return size;
}

static bool fs_packtype_dir_eof(UNUSED void *pack, fs_file_t *file) {
    return feof((FILE *) file->handle);
}

static s64 fs_packtype_dir_fsize(void *pack, const char *vpath) {
    sys_path_t path;
    fs_cat_paths(path, (const char *) pack, vpath);
    struct stat st;
    if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
        return (s64) st.st_size;
    }
    return -1;
}

fs_packtype_t fs_packtype_dir = {
    .extension = "",
    .mount     = fs_packtype_dir_mount,
    .unmount   = fs_packtype_dir_unmount,
    .walk      = fs_packtype_dir_walk,
    .is_file   = fs_packtype_dir_is_file,
    .is_dir    = fs_packtype_dir_is_dir,
    .open      = fs_packtype_dir_open,
    .read      = fs_packtype_dir_read,
    .seek      = fs_packtype_dir_seek,
    .tell      = fs_packtype_dir_tell,
    .size      = fs_packtype_dir_size,
    .eof       = fs_packtype_dir_eof,
    .close     = fs_packtype_dir_close,
    .fsize     = fs_packtype_dir_fsize,
};
