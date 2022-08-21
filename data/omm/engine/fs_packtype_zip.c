#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include <tinfl.h>

#define ZIP_BUFSIZE         0x00004000
#define ZIP_EOCD_BUFSIZE    0x0001002A
#define ZIP_LFH_SIG         0x04034B50
#define ZIP_CDH_SIG         0x02014B50
#define ZIP_EOCD_SIG        0x06054B50

typedef struct {
    fs_dirtree_t tree;          // this should always be first, so this could be used as a dirtree root
    const char *realpath;       // physical path to the zip file
    FILE *zipf;                 // open zip file handle, if any
} zip_pack_t;

typedef struct {
    fs_dirtree_entry_t tree;    // this should always be first, so this could be used as a dirtree entry
    u64 ofs;                    // offset to compressed data in zip
    u16 bits;                   // general purpose zip flags
    u16 comp_type;              // compression method
    u32 crc;                    // CRC-32
    u64 comp_size;              // size of compressed data in zip
    u64 uncomp_size;            // size of decompressed data
    u16 attr_int;               // internal attributes
    u32 attr_ext;               // external attributes
    bool ofs_fixed;             // if true, 'ofs' points to the file data, otherwise to LFH
} zip_entry_t;

typedef struct {
    zip_entry_t *entry;         // the dirtree entry of this file
    u32 comp_pos;               // read position in compressed data
    u32 uncomp_pos;             // read position in uncompressed data
    u8 *buffer;                 // decompression buffer (if compressed)
    z_stream zstream;           // tinfl zlib stream
    FILE *fstream;              // duplicate of zipf of the parent zip file
    bool inited;                // buffer is ready for decompression
} zip_file_t;

static s64 zip_find_eocd(FILE *f, s64 *outlen) {
    fseek(f, 0, SEEK_END);
    const s64 fsize = ftell(f);
    if (OMM_LIKELY(fsize > 16)) {
        u8 buf[ZIP_EOCD_BUFSIZE];
        const s64 rx = (fsize < ZIP_EOCD_BUFSIZE ? fsize : ZIP_EOCD_BUFSIZE);
        fseek(f, fsize - rx, SEEK_SET);
        if (OMM_LIKELY(fread(buf, rx, 1, f))) {
            for (s64 i = rx - 8; i >= 0; --i) {
                if (buf[i + 0] == 0x50 &&
                    buf[i + 1] == 0x4B &&
                    buf[i + 2] == 0x05 &&
                    buf[i + 3] == 0x06) {
                    if (outlen) *outlen = fsize;
                    return fsize - rx + i;
                }
            }
        }
    }
    return -1;
}

static bool zip_parse_eocd(FILE *f, u64 *cdir_ofs, u64 *data_ofs, u64 *count) {
    struct eocd_s {
        u32 sig;
        u16 this_disk;
        u16 cdir_disk;
        u16 disk_entry_count;
        u16 total_entry_count;
        u32 cdir_size;
        u32 cdir_ofs;
        u16 comment_len;
    } __attribute__((__packed__));
    struct eocd_s eocd;

    s64 fsize = 0;
    s64 pos = zip_find_eocd(f, &fsize);
    if (OMM_LIKELY(pos >= 0)) {
        fseek(f, pos, SEEK_SET);
        if (OMM_LIKELY(fread(&eocd, sizeof(eocd), 1, f) && LE_TO_HOST32(eocd.sig) == ZIP_EOCD_SIG && !eocd.this_disk && !eocd.cdir_disk && eocd.disk_entry_count == eocd.total_entry_count)) {
            *count = LE_TO_HOST16(eocd.total_entry_count);
            *cdir_ofs = LE_TO_HOST32(eocd.cdir_ofs);
            eocd.cdir_size = LE_TO_HOST32(eocd.cdir_size);
            if (OMM_LIKELY((u64) pos >= *cdir_ofs + eocd.cdir_size)) {
                *data_ofs = (u64) (pos - (*cdir_ofs + eocd.cdir_size));
                *cdir_ofs += *data_ofs;
                eocd.comment_len = LE_TO_HOST16(eocd.comment_len);
                return (pos + 22 + eocd.comment_len) == fsize;
            }
        }
    }
    return false;
}

static bool zip_fixup_offset(zip_file_t *zipfile) {
    struct lfh_s {
        u32 sig;
        u16 version_required;
        u16 bits;
        u16 comp_type;
        u16 mod_time;
        u16 mod_date;
        u32 crc;
        u32 comp_size;
        u32 uncomp_size;
        u16 fname_len;
        u16 extra_len;
    } __attribute__((__packed__));
    struct lfh_s lfh;

    zip_entry_t *ent = zipfile->entry;
    fseek(zipfile->fstream, ent->ofs, SEEK_SET);
    if (OMM_LIKELY(fread(&lfh, sizeof(lfh), 1, zipfile->fstream))) {
        lfh.fname_len = LE_TO_HOST16(lfh.fname_len);
        lfh.extra_len = LE_TO_HOST16(lfh.extra_len);
        ent->ofs += sizeof(lfh) + lfh.fname_len + lfh.extra_len;
        ent->ofs_fixed = true;
        return true;
    }
    return false;
}

static zip_entry_t *zip_load_entry(FILE *f, fs_dirtree_t *tree, const u64 data_ofs) {
    struct cdh_s {
        u32 sig;
        u16 version_used;
        u16 version_required;
        u16 bits;
        u16 comp_type;
        u16 mod_time;
        u16 mod_date;
        u32 crc;
        u32 comp_size;
        u32 uncomp_size;
        u16 fname_len;
        u16 extra_len;
        u16 comment_len;
        u16 start_disk;
        u16 attr_int;
        u32 attr_ext;
        u32 lfh_ofs;
    } __attribute__((__packed__));
    struct cdh_s cdh;

    zip_entry_t zipent;
    memset(&zipent, 0, sizeof(zipent));
    if (OMM_LIKELY(fread(&cdh, sizeof(cdh), 1, f) && LE_TO_HOST32(cdh.sig) == ZIP_CDH_SIG)) {
        zipent.bits = LE_TO_HOST16(cdh.bits);
        zipent.comp_type = LE_TO_HOST16(cdh.comp_type);
        zipent.crc = LE_TO_HOST32(cdh.crc);
        zipent.comp_size = LE_TO_HOST32(cdh.comp_size);
        zipent.uncomp_size = LE_TO_HOST32(cdh.uncomp_size);
        zipent.ofs = LE_TO_HOST32(cdh.lfh_ofs);
        zipent.attr_int = LE_TO_HOST16(cdh.attr_int);
        zipent.attr_ext = LE_TO_HOST32(cdh.attr_ext);
        cdh.fname_len = LE_TO_HOST16(cdh.fname_len);
        cdh.comment_len = LE_TO_HOST16(cdh.comment_len);
        cdh.extra_len = LE_TO_HOST16(cdh.extra_len);
        char name[SYS_MAX_PATH];
        if (OMM_LIKELY(fread(name, min(SYS_MAX_PATH - 1, cdh.fname_len), 1, f))) {
            bool is_dir = (name[cdh.fname_len - 1] == '/' || name[cdh.fname_len - 1] == '\\');
            name[cdh.fname_len - is_dir] = 0;
            zip_entry_t *retent = (zip_entry_t *) fs_dirtree_add(tree, name, is_dir);
            if (OMM_LIKELY(retent != NULL)) {
                zipent.tree = retent->tree;
                memcpy(retent, &zipent, sizeof(zipent));
                retent->ofs += data_ofs;
                fseek(f, cdh.extra_len + cdh.comment_len, SEEK_CUR);
                return retent;
            }
        }
    }
    return NULL;
}

static bool zip_load_entries(FILE *f, fs_dirtree_t *tree, const u64 cdir_ofs, const u64 data_ofs, const u64 count) {
    fseek(f, cdir_ofs, SEEK_SET);
    for (u64 i = 0; i != count; ++i) {
        if (!zip_load_entry(f, tree, data_ofs)) {
            return false;
        }
    }
    return true;
}

static bool is_zip(FILE *f) {
    u32 sig = 0;
    return fread(&sig, sizeof(sig), 1, f) && ((LE_TO_HOST32(sig) == ZIP_LFH_SIG) || zip_find_eocd(f, NULL) >= 0);
}

static void zip_close(zip_file_t *zipfile) {
    if (zipfile->buffer) {
        inflateEnd(&zipfile->zstream);
        omm_free(zipfile->buffer);
    }
    if (zipfile->fstream) fclose(zipfile->fstream);
    omm_free(zipfile);
}

//
// Packtype: ZIP
//

static void *fs_packtype_zip_mount(const char *realpath) {
    FILE *f = fopen(realpath, "rb");
    if (f) {
        if (is_zip(f)) {
            zip_pack_t *pack = omm_new(zip_pack_t, 1);
            if (OMM_LIKELY(pack != NULL)) {
                u64 cdir_ofs, data_ofs, count;
                if (zip_parse_eocd(f, &cdir_ofs, &data_ofs, &count) &&
                    fs_dirtree_init(&pack->tree, sizeof(zip_entry_t)) &&
                    zip_load_entries(f, &pack->tree, cdir_ofs, data_ofs, count)) {
                    pack->realpath = sys_strdup(realpath);
                    pack->zipf = f;
                    return pack;
                }
                omm_free(pack);
            }
        }
        fclose(f);
    }
    return NULL;
}

static void fs_packtype_zip_unmount(void *pack) {
    zip_pack_t *zip = (zip_pack_t *) pack;
    fs_dirtree_free(&zip->tree);
    if (zip->zipf) fclose(zip->zipf);
    omm_free(zip->realpath);
    omm_free(zip);
}

static bool fs_packtype_zip_is_file(void *pack, const char *fname) {
    zip_entry_t *ent = (zip_entry_t *) fs_dirtree_find((fs_dirtree_t *) pack, fname);
    return ent && !ent->tree.is_dir;
}

static bool fs_packtype_zip_is_dir(void *pack, const char *fname) {
    zip_entry_t *ent = (zip_entry_t *) fs_dirtree_find((fs_dirtree_t *) pack, fname);
    return ent && ent->tree.is_dir;
}

static fs_file_t *fs_packtype_zip_open(void *pack, const char *vpath) {
    zip_pack_t *zip = (zip_pack_t *) pack;
    zip_entry_t *ent = (zip_entry_t *) fs_dirtree_find((fs_dirtree_t *) zip, vpath);
    if (OMM_LIKELY(ent && !ent->tree.is_dir)) {
        zip_file_t *zipfile = omm_new(zip_file_t, 1);
        if (OMM_LIKELY(zipfile != NULL)) {
            zipfile->entry = ent;
            zipfile->fstream = fopen(zip->realpath, "rb");
            if (OMM_LIKELY(zipfile->fstream && (ent->ofs_fixed || zip_fixup_offset(zipfile)))) {
                if (ent->comp_type) {
                    zipfile->buffer = omm_new(u8, ZIP_BUFSIZE);
                    if (zipfile->buffer && inflateInit2(&zipfile->zstream, -MAX_WBITS) == Z_OK) {
                        zipfile->inited = true;
                    }
                } else {
                    zipfile->inited = true;
                }
                if (OMM_LIKELY(zipfile->inited)) {
                    fs_file_t *fsfile = omm_new(fs_file_t, 1);
                    if (OMM_LIKELY(fsfile != NULL)) {
                        fsfile->handle = zipfile;
                        fsfile->parent = NULL;
                        fseek(zipfile->fstream, ent->ofs, SEEK_SET);
                        return fsfile;
                    }
                }
            }
            zip_close(zipfile);
        }
    }
    return NULL;
}

static void fs_packtype_zip_close(UNUSED void *pack, fs_file_t *file) {
    if (!file) return;
    zip_file_t *zipfile = (zip_file_t *) file->handle;
    if (zipfile) zip_close(zipfile);
    omm_free(file);
}

static s64 fs_packtype_zip_read(UNUSED void *pack, fs_file_t *file, void *buf, const u64 size) {
    zip_file_t *zipfile = (zip_file_t *) file->handle;
    zip_entry_t *ent = zipfile->entry;
    s64 avail = ent->uncomp_size - zipfile->uncomp_pos;
    s64 max_read = ((s64) size > avail) ? avail : (s64) size;
    if (OMM_LIKELY(max_read)) {
        s64 rx = 0;
        if (!ent->comp_type) {
            rx = fread(buf, 1, size, zipfile->fstream);
        } else {
            zipfile->zstream.next_out = buf;
            zipfile->zstream.avail_out = (u32) max_read;
            while (rx < max_read) {
                const u32 before = (u32)zipfile->zstream.total_out;
                if (zipfile->zstream.avail_in == 0) {
                    s32 comp_rx = ent->comp_size - zipfile->comp_pos;
                    if (comp_rx > 0) {
                        if (comp_rx > ZIP_BUFSIZE) comp_rx = ZIP_BUFSIZE;
                        comp_rx = fread(zipfile->buffer, 1, comp_rx, zipfile->fstream);
                        if (comp_rx == 0) break;
                        zipfile->comp_pos += (u32)comp_rx;
                        zipfile->zstream.next_in = zipfile->buffer;
                        zipfile->zstream.avail_in = (u32)comp_rx;
                    }
                }
                s32 err = inflate(&zipfile->zstream, Z_SYNC_FLUSH);
                rx += zipfile->zstream.total_out - before;
                if (err != Z_OK) break;
            }
        }
        zipfile->uncomp_pos += rx;
        return rx;
    }
    return 0;
}

static bool fs_packtype_zip_seek(UNUSED void *pack, fs_file_t *file, const s64 ofs) {
    zip_file_t *zipfile = (zip_file_t *) file->handle;
    zip_entry_t *ent = zipfile->entry;
    if (ofs <= (s64) ent->uncomp_size) {
        if (!ent->comp_type) {
            if (fseek(zipfile->fstream, ofs + ent->ofs, SEEK_SET) == 0) {
                zipfile->uncomp_pos = ofs;
            }
        } else {
            if (ofs < zipfile->uncomp_pos) {
                z_stream zstream;
                memset(&zstream, 0, sizeof(zstream));
                if (inflateInit2(&zstream, -MAX_WBITS) != Z_OK) return false;
                if (fseek(zipfile->fstream, ent->ofs, SEEK_SET) != 0) return false;
                inflateEnd(&zipfile->zstream);
                memcpy(&zipfile->zstream, &zstream, sizeof(zstream));
                zipfile->uncomp_pos = zipfile->comp_pos = 0;
            }
            u8 buf[512];
            while (zipfile->uncomp_pos != ofs) {
                u32 max_read = (u32) (ofs - zipfile->uncomp_pos);
                if (max_read > sizeof(buf)) max_read = sizeof(buf);
                if (fs_packtype_zip_read(pack, file, buf, max_read) != max_read) return false;
            }
        }
        return true;
    }
    return false;    
}

static s64 fs_packtype_zip_tell(UNUSED void *pack, fs_file_t *file) {
    return ((zip_file_t *) file->handle)->uncomp_pos;
}

static s64 fs_packtype_zip_size(UNUSED void *pack, fs_file_t *file) {
    zip_file_t *zipfile = (zip_file_t *) file->handle;
    return zipfile->entry->uncomp_size;
}

static bool fs_packtype_zip_eof(UNUSED void *pack, fs_file_t *file) {
    zip_file_t *zipfile = (zip_file_t *) file->handle;
    return zipfile->uncomp_pos >= zipfile->entry->uncomp_size;
}

fs_packtype_t fs_packtype_zip = {
    "zip",
    fs_packtype_zip_mount,
    fs_packtype_zip_unmount,
    fs_dirtree_walk,
    fs_packtype_zip_is_file,
    fs_packtype_zip_is_dir,
    fs_packtype_zip_open,
    fs_packtype_zip_read,
    fs_packtype_zip_seek,
    fs_packtype_zip_tell,
    fs_packtype_zip_size,
    fs_packtype_zip_eof,
    fs_packtype_zip_close,
};
