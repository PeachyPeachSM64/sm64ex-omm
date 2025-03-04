#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Baserom
//

static struct {
    sys_path_t filepath;
    bool appdata;
} sBaseromUsZ64[1] = {0};

static void rom_asset_remember_baserom(sys_path_t filepath, bool appdata) {
    str_cpy(sBaseromUsZ64->filepath, sizeof(sys_path_t), filepath);
    sBaseromUsZ64->appdata = appdata;
}

void rom_asset_copy_baserom_to_appdata() {
    if (!sBaseromUsZ64->appdata) {
        sys_path_t filepathAppdata;
        fs_cat_paths(filepathAppdata, sys_user_path(), BASEROM_FILENAME);
        fs_sys_copy_file(sBaseromUsZ64->filepath, filepathAppdata);
    }
}

static void rom_asset_delete_baserom_from_appdata() {
    if (sBaseromUsZ64->appdata && fs_sys_file_exists(sBaseromUsZ64->filepath)) {
        fs_sys_delete_file(sBaseromUsZ64->filepath);
    }
}

#define rom_asset_fatal(fmt, ...) { \
    rom_asset_delete_baserom_from_appdata(); \
    sys_fatal("%s: " fmt "%s%s%s", \
        __FUNCTION__ \
        __VA_ARGS__, \
        (*sBaseromUsZ64->filepath ? "\nROM file: \"" : ""), \
        (sBaseromUsZ64->filepath), \
        (*sBaseromUsZ64->filepath ? "\"" : "") \
    ); \
}

//
// ROM Assets
//

#define ROM_ASSET_TYPE_NONE          (0)
#define ROM_ASSET_TYPE_TEXTURE       (1)
#define ROM_ASSET_TYPE_TEXTURE_MIO0  (2)
#define ROM_ASSET_TYPE_SKYBOX        (3)

typedef struct {
    u32 type;
    u32 address;
    u32 offset;
    u32 size;
    s32 width;
    s32 height;
    sys_path_t name;
} RomAsset;

static OmmArray_(RomAsset *) sRomAssets = omm_array_zero;

static void rom_asset_register(u32 type, u32 address, u32 offset, u32 size, s32 width, s32 height, const char *name) {
    if (type) {
        RomAsset *asset = mem_new(RomAsset, 1);
        asset->type = type;
        asset->address = address;
        asset->offset = offset;
        asset->size = size;
        asset->width = width;
        asset->height = height;
        str_cpy(asset->name, sizeof(sys_path_t), name);
        omm_array_add(sRomAssets, ptr, asset);
    }
}

#define rom_asset_texture(address, size, width, height, name) \
    rom_asset_register(ROM_ASSET_TYPE_TEXTURE, address, 0, size, width, height, name)

#define rom_asset_texture_mio0(address, offset, size, width, height, name) \
    rom_asset_register(ROM_ASSET_TYPE_TEXTURE_MIO0, address, offset, size, width, height, name)

#define rom_asset_skybox(address, size, name) \
    rom_asset_register(ROM_ASSET_TYPE_SKYBOX, address, 0, size, 0, 0, name)

OMM_AT_STARTUP static void rom_asset_init() {
#include "rom_assets.inl"
}

#undef rom_asset_texture
#undef rom_asset_texture_mio0
#undef rom_asset_skybox

//
// MIO0 decompression
//

#define U32_BE(buf) (u32) (((buf)[0] << 24) | ((buf)[1] << 16) | ((buf)[2] << 8) | ((buf)[3] << 0))
#define U32_LE(buf) (u32) (((buf)[3] << 24) | ((buf)[2] << 16) | ((buf)[1] << 8) | ((buf)[0] << 0))

#define MIO0_MAGIC "MIO0"
#define MIO0_GET_BIT(buf, bit) ((buf)[(bit) / 8] & (1 << (7 - ((bit) % 8))))

typedef struct {
    u8 *data;
    u32 size;
} Mio0Buffer;

static bool rom_asset_is_mio0(const u8 *data) {
    return mem_eq(data, MIO0_MAGIC, 4);
}

static u8 *rom_asset_decompress_mio0(const u8 *data, u32 *size) {

    // Extract header
    if (!rom_asset_is_mio0(data)) {
        rom_asset_fatal(
            "Input data is not MIO0-compressed!"
        );
    }
    u32 destSize = U32_BE(data + 4);
    u32 compOffset = U32_BE(data + 8);
    u32 uncompOffset = U32_BE(data + 12);

    // Decompress data
    u8 *decompressed = mem_new(u8, destSize);
    for (u32 bitIdx = 0, bytesWritten = 0, compIdx = 0, uncompIdx = 0; bytesWritten < destSize;) {
        if (MIO0_GET_BIT(data + 16, bitIdx)) {
            decompressed[bytesWritten] = data[uncompOffset + uncompIdx];
            bytesWritten++;
            uncompIdx++;
        } else {
            const u8 *vals = &data[compOffset + compIdx];
            compIdx += 2;
            u32 length = ((vals[0] & 0xF0) >> 4) + 3;
            u32 idx = ((vals[0] & 0x0F) << 8) + vals[1] + 1;
            for (u32 i = 0; i != length; ++i) {
                decompressed[bytesWritten] = decompressed[bytesWritten - idx];
                bytesWritten++;
            }
        }
        bitIdx++;
    }

    // Return data
    *size = destSize;
    return decompressed;
}

static Mio0Buffer *rom_asset_load_mio0(const u8 *data) {
    static OmmHMap_(Mio0Buffer *) sMio0Buffers = omm_hmap_zero;

    // Check the cache
    s32 i = omm_hmap_find(sMio0Buffers, (uintptr_t) data);
    if (i != -1) {
        return omm_hmap_get(sMio0Buffers, Mio0Buffer *, i);
    }

    // Load and decompress buffer
    Mio0Buffer *buffer = mem_new(Mio0Buffer, 1);
    buffer->data = rom_asset_decompress_mio0(data, &buffer->size);
    omm_hmap_insert(sMio0Buffers, (uintptr_t) data, buffer);
    return buffer;
}

//
// ROM
//

static bool rom_asset_load_rom_data(u8 *data, u32 size, sys_path_t filepath, bool appdata) {

    // Open baserom
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        return false;
    }

    // Read ROM data
    u32 read = fread(data, 1, size, f);
    fclose(f);
    if (read != size) {
        return false;
    }

    // Check MIO0 sections
    omm_array_for_each(sRomAssets, p_asset) {
        RomAsset *asset = p_asset->as_ptr;
        if (asset->type == ROM_ASSET_TYPE_TEXTURE_MIO0 ||
            asset->type == ROM_ASSET_TYPE_SKYBOX) {
            if (!rom_asset_is_mio0(data + asset->address)) {
                return false;
            }
        }
    }

    // OK
    omm_log("Loading ROM data from file: \"%s\"\n",, filepath);
    rom_asset_remember_baserom(filepath, appdata);
    return true;
}

static void rom_asset_load_rom(u8 *data, u32 size) {
    OMM_DO_ONCE {
        sys_path_t filepath;

        // AppData
        fs_cat_paths(filepath, sys_user_path(), BASEROM_FILENAME);
        if (rom_asset_load_rom_data(data, size, filepath, true)) {
            return;
        }

        // Exe directory
        fs_cat_paths(filepath, sys_exe_path(), BASEROM_FILENAME);
        if (rom_asset_load_rom_data(data, size, filepath, false)) {
            return;
        }

        // Handle Windows stupid 'feature' that hides the file extensions by default
        fs_cat_paths(filepath, sys_exe_path(), BASEROM_FILENAME BASEROM_EXT);
        if (rom_asset_load_rom_data(data, size, filepath, false)) {
            return;
        }

        // Not found
        rom_asset_fatal(
            "Unable to find a Super Mario 64 US ROM file.\n"
            "Please put a valid copy of the Super Mario 64 US ROM in the executable directory and rename it \"" BASEROM_FILENAME "\"."
        );
    }
}

static u8 *rom_asset_get_rom() {
    static u8 sRomData[BASEROM_SIZE];
    rom_asset_load_rom(sRomData, sizeof(sRomData));
    return sRomData;
}

//
// Textures
//

#define SCALE_5_8(x)  (((x) * 0xFF) / 0x1F)
#define SCALE_4_8(x)  ((x) * 0x11)
#define SCALE_3_8(x)  ((x) * 0x24)

static u8 *rom_asset_texture_convert_rgba16(const u8 *data, u32 size) {
    u8 *img = mem_new(u8, size * 2);
    u8 *buf = img;
    for (u32 i = 0; i != size; i += 2) {
        u16 c = (data[i + 0] << 8) | data[i + 1];
        u8 r = (c >> 11) & 0x1F;
        u8 g = (c >>  6) & 0x1F;
        u8 b = (c >>  1) & 0x1F;
        u8 a = (c >>  0) & 0x01;
        *(buf++) = (SCALE_5_8(r));
        *(buf++) = (SCALE_5_8(g));
        *(buf++) = (SCALE_5_8(b));
        *(buf++) = (0xFF  *  (a));
    }
    return img;
}

static u8 *rom_asset_texture_convert_rgba32(const u8 *data, u32 size) {
    u8 *img = mem_new(u8, size * 1);
    mem_cpy(img, data, size);
    return img;
}

static u8 *rom_asset_texture_convert_ia4(const u8 *data, u32 size) {
    u8 *img = mem_new(u8, size * 8);
    u8 *buf = img;
    for (u32 i = 0; i != size; ++i) {
        u8 h0 = (data[i] >> 4) & 0xF;
        u8 h1 = (data[i] >> 0) & 0xF;
        *(buf++) = (SCALE_3_8(h0 >> 1));
        *(buf++) = (SCALE_3_8(h0 >> 1));
        *(buf++) = (SCALE_3_8(h0 >> 1));
        *(buf++) = (0xFF  *  (h0 &  1));
        *(buf++) = (SCALE_3_8(h1 >> 1));
        *(buf++) = (SCALE_3_8(h1 >> 1));
        *(buf++) = (SCALE_3_8(h1 >> 1));
        *(buf++) = (0xFF  *  (h1 &  1));
    }
    return img;
}

static u8 *rom_asset_texture_convert_ia8(const u8 *data, u32 size) {
    u8 *img = mem_new(u8, size * 4);
    u8 *buf = img;
    for (u32 i = 0; i != size; ++i) {
        u8 c = (data[i] >> 4) & 0xF;
        u8 a = (data[i] >> 0) & 0xF;
        *(buf++) = (SCALE_4_8(c));
        *(buf++) = (SCALE_4_8(c));
        *(buf++) = (SCALE_4_8(c));
        *(buf++) = (SCALE_4_8(a));
    }
    return img;
}

static u8 *rom_asset_texture_convert_ia16(const u8 *data, u32 size) {
    u8 *img = mem_new(u8, size * 2);
    u8 *buf = img;
    for (u32 i = 0; i != size; i += 2) {
        u8 c = data[i + 0];
        u8 a = data[i + 1];
        *(buf++) = (c);
        *(buf++) = (c);
        *(buf++) = (c);
        *(buf++) = (a);
    }
    return img;
}

static u8 *rom_asset_texture_convert_i4(const u8 *data, u32 size) {
    u8 *img = mem_new(u8, size * 8);
    u8 *buf = img;
    for (u32 i = 0; i != size; ++i) {
        u8 h0 = (data[i] >> 4) & 0xF;
        u8 h1 = (data[i] >> 0) & 0xF;
        *(buf++) = (SCALE_4_8(h0));
        *(buf++) = (SCALE_4_8(h0));
        *(buf++) = (SCALE_4_8(h0));
        *(buf++) = (0xFF);
        *(buf++) = (SCALE_4_8(h1));
        *(buf++) = (SCALE_4_8(h1));
        *(buf++) = (SCALE_4_8(h1));
        *(buf++) = (0xFF);
    }
    return img;
}

static u8 *rom_asset_texture_convert_i8(const u8 *data, u32 size) {
    u8 *img = mem_new(u8, size * 4);
    u8 *buf = img;
    for (u32 i = 0; i != size; ++i) {
        *(buf++) = (data[i]);
        *(buf++) = (data[i]);
        *(buf++) = (data[i]);
        *(buf++) = (0xFF);
    }
    return img;
}

static u8 *rom_asset_texture_convert_to_rgba32(const char *name, const u8 *data, u32 size) {
    if (strstr(name, ".rgba16")) return rom_asset_texture_convert_rgba16(data, size);
    if (strstr(name, ".rgba32")) return rom_asset_texture_convert_rgba32(data, size);
    if (strstr(name, ".ia4"))    return rom_asset_texture_convert_ia4   (data, size);
    if (strstr(name, ".ia8"))    return rom_asset_texture_convert_ia8   (data, size);
    if (strstr(name, ".ia16"))   return rom_asset_texture_convert_ia16  (data, size);
    if (strstr(name, ".i4"))     return rom_asset_texture_convert_i4    (data, size);
    if (strstr(name, ".i8"))     return rom_asset_texture_convert_i8    (data, size);
    rom_asset_fatal(
        "File \"%s\": Unknown texture format.",, name
    );
}

static u8 *rom_asset_texture_extract(const RomAsset *asset, bool mio0) {
    if (mio0) {
        Mio0Buffer *buffer = rom_asset_load_mio0(rom_asset_get_rom() + asset->address);
        return rom_asset_texture_convert_to_rgba32(asset->name, buffer->data + asset->offset, asset->size);
    }
    return rom_asset_texture_convert_to_rgba32(asset->name, rom_asset_get_rom() + asset->address, asset->size);
}

//
// Skyboxes
//

static void rom_asset_skybox_copy_tile(u8 *skybox, const u8 *tile, s32 x, s32 y, s32 skyboxWidth, s32 tileWidth, s32 tileHeight) {
    skybox += 4 * (tileWidth - 1) * (y * (tileHeight - 1) * skyboxWidth + x);
    for (s32 line = 0; line != (tileHeight - 1); ++line) {
        mem_cpy(skybox, tile, sizeof(u8) * 4 * (tileWidth - 1));
        tile += 4 * tileWidth;
        skybox += 4 * (tileWidth - 1) * skyboxWidth;
    }
}

static u8 *rom_asset_skybox_extract_and_assemble_tiles(
    const u8 *data, u32 size, s32 *width, s32 *height,
    s32 skyboxWidth, s32 skyboxHeight,
    s32 tileWidth, s32 tileHeight,
    s32 tableWidth, s32 tableHeight
) {

    // Extract tiles
    s32 tileSize = tileWidth * tileHeight * sizeof(u16);
    s32 tableSize = tableWidth * tableHeight * sizeof(u32);
    s32 numTiles = (size - tableSize) / tileSize;
    u8 **tiles = mem_new(u8 *, numTiles);
    for (s32 i = 0; i != numTiles; ++i) {
        tiles[i] = rom_asset_texture_convert_rgba16(data + i * tileSize, tileSize);
    }

    // Assemble tiles
    *width = skyboxWidth * (tileWidth - 1);
    *height = skyboxHeight * (tileHeight - 1);
    u8 *skybox = mem_new(u8, *width * *height * 4);
    if (!tableSize) {
        for (s32 i = 0; i != numTiles; ++i) {
            s32 x = (i % skyboxWidth);
            s32 y = (i / skyboxWidth);
            rom_asset_skybox_copy_tile(skybox, tiles[i], x, y, skyboxWidth, tileWidth, tileHeight);
        }
    } else {
        const u8 *table = data + numTiles * tileSize;
        u32 tableOffset = U32_BE(table);
        for (s32 y = 0; y != skyboxHeight; ++y)
        for (s32 x = 0; x != skyboxWidth; ++x) {
            const u8 *tableIndex = table + sizeof(u32) * ((y * tableWidth) + x);
            u32 tileIndex = (U32_BE(tableIndex) - tableOffset) / tileSize;
            rom_asset_skybox_copy_tile(skybox, tiles[tileIndex], x, y, skyboxWidth, tileWidth, tileHeight);
        }
    }

    // Free tiles
    for (s32 i = 0; i != numTiles; ++i) {
        mem_del(tiles[i]);
    }
    mem_del(tiles);
    return skybox;
}

static u8 *rom_asset_skybox_extract(const RomAsset *asset, s32 *width, s32 *height) {
    Mio0Buffer *buffer = rom_asset_load_mio0(rom_asset_get_rom() + asset->address);
    if (strcmp(asset->name, "levels/ending/cake") == 0) {
        return rom_asset_skybox_extract_and_assemble_tiles(buffer->data, asset->size, width, height, 4, 12, 80, 20, 0, 0);
    }
    return rom_asset_skybox_extract_and_assemble_tiles(buffer->data, asset->size, width, height, 8, 8, 32, 32, 10, 8);
}

//
// Sound data
//

#define SEQUENCES_FILE  "sound/sequences.bin"
#define SAMPLES_FILE    "sound/sound_data.tbl"
#define ROM_DATA_MAGIC  "ROM_DATA"

static void rom_asset_sound_load_data(u8 *data, u64 size) {
    for (u64 pos = 0; pos < size; ++pos) {
        if (mem_eq(data + pos, ROM_DATA_MAGIC, 8)) {
            u32 offset = U32_LE(data + pos + 8);
            u32 length = U32_LE(data + pos + 12);
            mem_cpy(data + pos, rom_asset_get_rom() + offset, length);
            pos += (length - 1);
        }
    }
}

//
// Precache
//

void rom_asset_precache_textures(bool (*func)(const char *, s32, s32, u8 *)) {
    omm_array_for_each(sRomAssets, p_asset) {
        RomAsset *asset = p_asset->as_ptr;

        // Texture (uncompressed)
        if (asset->type == ROM_ASSET_TYPE_TEXTURE) {
            u8 *data = rom_asset_texture_extract(asset, false);
            if (data && !func(asset->name, asset->width, asset->height, data)) {
                mem_del(data);
            }
        }

        // Texture (compressed)
        if (asset->type == ROM_ASSET_TYPE_TEXTURE_MIO0) {
            u8 *data = rom_asset_texture_extract(asset, true);
            if (data && !func(asset->name, asset->width, asset->height, data)) {
                mem_del(data);
            }
        }

        // Skybox
        if (asset->type == ROM_ASSET_TYPE_SKYBOX) {
            s32 width, height;
            u8 *data = rom_asset_skybox_extract(asset, &width, &height);
            if (data && !func(asset->name, width, height, data)) {
                mem_del(data);
            }
        }
    }
}

//
// Load
//

u8 *rom_asset_load_texture(const char *name, s32 *width, s32 *height) {
    omm_array_for_each(sRomAssets, p_asset) {
        RomAsset *asset = p_asset->as_ptr;
        if (strcmp(name, asset->name) == 0) {

            // Texture (uncompressed)
            if (asset->type == ROM_ASSET_TYPE_TEXTURE) {
                *width = asset->width;
                *height = asset->height;
                return rom_asset_texture_extract(asset, false);
            }

            // Texture (compressed)
            if (asset->type == ROM_ASSET_TYPE_TEXTURE_MIO0) {
                *width = asset->width;
                *height = asset->height;
                return rom_asset_texture_extract(asset, true);
            }

            // Skybox
            if (asset->type == ROM_ASSET_TYPE_SKYBOX) {
                return rom_asset_skybox_extract(asset, width, height);
            }
        }
    }
    return NULL;
}

u8 *rom_asset_load_sound_data(const char *name) {
    u64 size;
    u8 *data = fs_load_file(name, &size);
    if (data && (strcmp(name, SEQUENCES_FILE) == 0 || strcmp(name, SAMPLES_FILE) == 0)) {
        rom_asset_sound_load_data(data, size);
    }
    return data;
}
