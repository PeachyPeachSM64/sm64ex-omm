#!/usr/bin/env python3

import sys, zipfile

BASEROM_US_Z64       = "build/us_pc/baserom.us.z64"
ROM_ASSETS_FILE      = "data/omm/engine/rom_assets.inl"
SOUND_DATA_ALIGNMENT = 0x10
SAMPLES_START        = 0x005936A0
SAMPLES_END          = 0x007B0860
SEQUENCES_START      = 0x007B0980
SEQUENCES_END        = 0x007CC620
ZERO_BYTES           = bytes(SOUND_DATA_ALIGNMENT)

def get_rom_assets():
    rom_assets = list()
    with open(ROM_ASSETS_FILE, "r") as f:
        for line in f:
            asset_name = line[line.find("\""):line.rfind("\"")]
            if asset_name:
                rom_assets.append(asset_name[1:] + ".png")
                rom_assets.append(asset_name[1:] + ".rgba16.png")
    return rom_assets

def generate_basepack(basepack_lst: str):
    basepack = dict()
    rom_assets = get_rom_assets()
    with open(basepack_lst, "r") as f:
        for line in f:
            tokens = line.strip().split()
            if len(tokens) >= 2:
                fname, aname = tokens[0], tokens[1]
                if fname not in rom_assets and ".ia1." not in fname:
                    basepack[aname] = fname
    return basepack

def find_offset(data: bytes, bytes_to_find: bytes, data_offsets: list):
    if bytes_to_find != ZERO_BYTES:
        data_offset = 0
        while True:
            data_offset = data.find(bytes_to_find, data_offset)
            for (start, end) in data_offsets:
                if data_offset >= start and data_offset < end:
                    data_offset += 1
                    break
            else:
                return data_offset
    return -1

def build_offsets(zipf: zipfile.ZipFile, aname: str, fname: str, rom_start: int, rom_end: int):
    data = open(fname, "rb").read()
    rom = open(BASEROM_US_Z64, "rb").read()
    rom_offset = rom_start
    data_offsets = list()
    while rom_offset < rom_end:
        bytes_to_find = rom[rom_offset : rom_offset + SOUND_DATA_ALIGNMENT]
        data_offset = find_offset(data, bytes_to_find, data_offsets)
        if data_offset != -1:
            length = SOUND_DATA_ALIGNMENT
            while data[data_offset + length : data_offset + length + SOUND_DATA_ALIGNMENT] == rom[rom_offset + length : rom_offset + length + SOUND_DATA_ALIGNMENT]:
                length += SOUND_DATA_ALIGNMENT
            data = data[:data_offset] + (
                "ROM_DATA".encode() +
                rom_offset.to_bytes(4, byteorder="little", signed=False) +
                length.to_bytes(4, byteorder="little", signed=False) +
                (b"\0" * (length - 16))
            ) + data[data_offset+length:]
            data_offsets.append((data_offset, data_offset + length))
            rom_offset += length
        else:
            rom_offset += SOUND_DATA_ALIGNMENT
    open(fname + ".tmp", "wb").write(data)
    zipf.write(fname + ".tmp", arcname=aname)

def build_basepack(basepack: dict, basepack_zip: str):
    print("Building basepack:", basepack_zip, "(%d files)" % (len(basepack)))
    with zipfile.ZipFile(basepack_zip, "w", compression=zipfile.ZIP_DEFLATED, allowZip64=False) as zipf:
        for aname, fname in basepack.items():
            if aname == "sound/sequences.bin":
                build_offsets(zipf, aname, fname, SEQUENCES_START, SEQUENCES_END)
            elif aname == "sound/sound_data.tbl":
                build_offsets(zipf, aname, fname, SAMPLES_START, SAMPLES_END)
            else:
                zipf.write(fname, arcname=aname)

if __name__ == "__main__":
    for i in range(2, len(sys.argv), 2):
        basepack = generate_basepack(sys.argv[i - 1])
        build_basepack(basepack, sys.argv[i])
