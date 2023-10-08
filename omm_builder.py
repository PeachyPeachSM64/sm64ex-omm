#!/usr/bin/env python3

import os
import sys
import subprocess

#
# Format
#

FMT_RESET    = "\033[0m"
FMT_BOLD     = "\033[1m"
FMT_ULINE    = "\033[4m"
FMT_INVERT   = "\033[7m"
COL_DEFAULT  = "\033[39m"
COL_BLACK    = "\033[30m"
COL_RED      = "\033[31m"
COL_GREEN    = "\033[32m"
COL_YELLOW   = "\033[33m"
COL_BLUE     = "\033[34m"
COL_MAGENTA  = "\033[35m"
COL_CYAN     = "\033[36m"
COL_LGRAY    = "\033[37m"
COL_GRAY     = "\033[90m"
COL_LRED     = "\033[91m"
COL_LGREEN   = "\033[92m"
COL_LYELLOW  = "\033[93m"
COL_LBLUE    = "\033[94m"
COL_LMAGENTA = "\033[95m"
COL_LCYAN    = "\033[96m"
COL_WHITE    = "\033[97m"
BGC_DEFAULT  = "\033[49m"
BGC_BLACK    = "\033[40m"
BGC_RED      = "\033[41m"
BGC_GREEN    = "\033[42m"
BGC_YELLOW   = "\033[43m"
BGC_BLUE     = "\033[44m"
BGC_MAGENTA  = "\033[45m"
BGC_CYAN     = "\033[46m"
BGC_LGRAY    = "\033[47m"
BGC_GRAY     = "\033[100m"
BGC_LRED     = "\033[101m"
BGC_LGREEN   = "\033[102m"
BGC_LYELLOW  = "\033[103m"
BGC_LBLUE    = "\033[104m"
BGC_LMAGENTA = "\033[105m"
BGC_LCYAN    = "\033[106m"
BGC_WHITE    = "\033[107m"

#
# Repositories
#

OMM_REPOSITORIES = {
    "omm":  "https://github.com/PeachyPeachSM64/sm64ex-omm.git -b nightly",
    "smex": "https://github.com/sm64pc/sm64ex.git -b nightly",
    "smms": "https://github.com/sm64pc/sm64ex.git -b nightly",
    "sm74": "https://github.com/PeachyPeachSM64/sm64ex-omm-romhacks.git -b sm74",
    "smsr": "https://github.com/PeachyPeachSM64/sm64ex-omm-romhacks.git -b smsr",
    "smgs": "https://github.com/PeachyPeachSM64/sm64ex-omm-romhacks.git -b smgs",
    "r96x": "https://github.com/Render96/Render96ex.git -b tester",
}

OMM_BUILDER_REMOTE_VERSION = "https://raw.githubusercontent.com/PeachyPeachSM64/sm64ex-omm/builder/omm_builder.py"
OMM_SOURCE_REMOTE_VERSION  = "https://raw.githubusercontent.com/PeachyPeachSM64/sm64ex-omm/nightly/omm.mk"

#
# Builder args
#

OMM_BUILDER_ARG_HELP  = { "arg": "-h", "info": "Display the help."                                 }
OMM_BUILDER_ARG_BUILD = { "arg": "-b", "info": "Only build the game, don't run it."                }
OMM_BUILDER_ARG_RESET = { "arg": "-r", "info": "Force a reset before building the game."           }
OMM_BUILDER_ARG_LOCAL = { "arg": "-l", "info": "Use local OMM source, don't check remote version." }
OMM_BUILDER_ARG_DEBUG = { "arg": "-d", "info": "Build the game with debug features."               }
OMM_BUILDER_ARG_INPUT = { "arg": "-i", "info": "Run the input GUI keys one after another."         }
OMM_BUILDER_ARGS      = [ OMM_BUILDER_ARG_HELP,
                          OMM_BUILDER_ARG_BUILD,
                          OMM_BUILDER_ARG_RESET,
                          OMM_BUILDER_ARG_LOCAL,
                          OMM_BUILDER_ARG_DEBUG,
                          OMM_BUILDER_ARG_INPUT ]

#
# Builder GUI
#

OMM_BUILDER_GUI_BUFFER_0      = []
OMM_BUILDER_GUI_BUFFER_1      = []

OMM_BUILDER_GUI_LOGO_0        = "                                    _                    "
OMM_BUILDER_GUI_LOGO_1        = "  ____ __    ___    __  _____      |_|_    _             "
OMM_BUILDER_GUI_LOGO_2        = " / __ \| \  / | \  / |  | __ \ _  _ _| | _| | ___  _ __  "
OMM_BUILDER_GUI_LOGO_3        = "/ /  \ |  \/  |  \/  |  | |/ /| || | | |/   |/ _ \| '_/  "
OMM_BUILDER_GUI_LOGO_4        = "\ \__/ | |  | | |  | |  | |) \| \/ | | | (] |  __/| |    "
OMM_BUILDER_GUI_LOGO_5        = " \____/|_|\/|_|_|\/|_|  |____/ \__/|_|_|\___|\___\|_|    "

OMM_BUILDER_GUI_MENU_CLOSE    = 0
OMM_BUILDER_GUI_MENU_GAMES    = 1
OMM_BUILDER_GUI_MENU_COMMANDS = 2
OMM_BUILDER_GUI_MENU_BUILD    = 3
OMM_BUILDER_GUI_MENU_PATCHES  = 4
OMM_BUILDER_GUI_MENU_TEXTURES = 5
OMM_BUILDER_GUI_MENU_SOUNDS   = 6
OMM_BUILDER_GUI_MENU_MODELS   = 7
OMM_BUILDER_GUI_MENU_AUDIOS   = 8

OMM_BUILDER_GUI_ACTION_NONE   = 0
OMM_BUILDER_GUI_ACTION_BUILD  = 1
OMM_BUILDER_GUI_ACTION_RUN    = 2
OMM_BUILDER_GUI_ACTION_CLEAR  = 3
OMM_BUILDER_GUI_ACTION_RESET  = 4
OMM_BUILDER_GUI_ACTION_DELETE = 5

OMM_BUILDER_GUI_KEY_UP        = "eE"
OMM_BUILDER_GUI_KEY_DOWN      = "dD"
OMM_BUILDER_GUI_KEY_LEFT      = "sS"
OMM_BUILDER_GUI_KEY_RIGHT     = "fF"
OMM_BUILDER_GUI_KEY_ENTER     = "cC"
OMM_BUILDER_GUI_KEY_BACK      = "xX0"
OMM_BUILDER_GUI_KEY_DIGIT     = "123456789"

#
# Builder data
#

OMM_BUILDER_DATA = {
    "menus": [ {},
        { "name": "Games",         "list": "game",     "prev": OMM_BUILDER_GUI_MENU_CLOSE    },
        { "name": "Commands",      "list": "commands", "prev": OMM_BUILDER_GUI_MENU_GAMES    },
        { "name": "Build",         "list": "build",    "prev": OMM_BUILDER_GUI_MENU_COMMANDS },
        { "name": "Patches",       "list": "patches",  "prev": OMM_BUILDER_GUI_MENU_BUILD    },
        { "name": "Texture Packs", "list": "textures", "prev": OMM_BUILDER_GUI_MENU_BUILD    },
        { "name": "Sound Packs",   "list": "sounds",   "prev": OMM_BUILDER_GUI_MENU_BUILD    },
        { "name": "Model Packs",   "list": "models",   "prev": OMM_BUILDER_GUI_MENU_BUILD    },
        { "name": "Audio Packs",   "list": "audios",   "prev": OMM_BUILDER_GUI_MENU_BUILD    },
    ],
    "game": [
        { "name": "Super Mario 64 ex-nightly",       "path": "smex", "dep": "",          "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario 64 Moonshine",        "path": "smms", "dep": "moonshine", "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario 74",                  "path": "sm74", "dep": "",          "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario Star Road",           "path": "smsr", "dep": "",          "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario 64: The Green Stars", "path": "smgs", "dep": "",          "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Render96",                        "path": "r96x", "dep": "",          "audio": True,  "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
    ],
    "commands": [
        { "name": "Build",  "type": "menu",   "val": OMM_BUILDER_GUI_MENU_BUILD,    "checks": "dep;rom" },
        { "name": "Run",    "type": "action", "val": OMM_BUILDER_GUI_ACTION_RUN,    "checks": "exe"     },
        { "name": "Clear",  "type": "action", "val": OMM_BUILDER_GUI_ACTION_CLEAR,  "checks": "dir"     },
        { "name": "Reset",  "type": "action", "val": OMM_BUILDER_GUI_ACTION_RESET,  "checks": "dir"     },
        { "name": "Delete", "type": "action", "val": OMM_BUILDER_GUI_ACTION_DELETE, "checks": "dir"     },
    ],
    "build": [
        { "name": "Build Speed",   "type": "choice", "val": "speed", "set": "speed"                               },
        { "name": "Render API",    "type": "choice", "val": "rapi",  "set": "rapi"                                },
        { "name": "Patches",       "type": "list",   "val": OMM_BUILDER_GUI_MENU_PATCHES,  "checks": "size"       },
        { "name": "Texture Packs", "type": "list",   "val": OMM_BUILDER_GUI_MENU_TEXTURES, "checks": "size"       },
        { "name": "Sound Packs",   "type": "list",   "val": OMM_BUILDER_GUI_MENU_SOUNDS,   "checks": "size"       },
        { "name": "Model Packs",   "type": "list",   "val": OMM_BUILDER_GUI_MENU_MODELS,   "checks": "size"       },
        { "name": "Audio Packs",   "type": "list",   "val": OMM_BUILDER_GUI_MENU_AUDIOS,   "checks": "size;audio" },
        { "name": "Build and Run", "type": "action", "val": OMM_BUILDER_GUI_ACTION_BUILD                          },
    ],
    "speed": [
        { "name": "Slow",    "jobs": [                               ] },
        { "name": "Fast",    "jobs": ["-j" + str(os.cpu_count() // 2)] },
        { "name": "Faster",  "jobs": ["-j" + str(os.cpu_count()     )] },
        { "name": "Fastest", "jobs": ["-j"                           ] },
    ],
    "rapi": [
        { "name": "OpenGL 2.1", "flags": ["RENDER_API=GL",    "WINDOW_API=SDL2", "AUDIO_API=SDL2", "CONTROLLER_API=SDL2", "GRUCODE=f3dex2e"] },
        { "name": "DirectX 11", "flags": ["RENDER_API=D3D11", "WINDOW_API=DXGI", "AUDIO_API=SDL2", "CONTROLLER_API=SDL2", "GRUCODE=f3dex2e"] },
    ],
    "builder": [],
    "version": [],
    "revision": [],
    "patches": [],
    "textures": [],
    "sounds": [],
    "models": [],
    "audios": [],
}

#
# Builder info
#

OMM_BUILDER_INFO = {
    "game": [
        { "info": "PC Port of Super Mario 64 with additional features."                                                               },
        { "info": "PC Port mod developed by TurnFlashed, S4ys and Fito. Features 10 new worlds and a total of 50 Moons."              },
        { "info": "PC Port of Lugmillord's rom-hack, Super Mario 74. Features both Normal and Extreme Editions."                      },
        { "info": "PC Port of Skelux's rom-hack, Super Mario Star Road. Features the 120 main stars and 10 extra stars."              },
        { "info": "PC Port of Kampel125's rom-hack, Super Mario 64: The Green Stars. Features 131 unique stars."                      },
        { "info": "Super Mario 64 with the look of '96 renders. Has DynOS built-in, a new audio system and playable Luigi and Wario." },
    ],
    "commands": [
        { "info": "Compile the game with some customization."                                                                         },
        { "info": "Launch the compiled game."                                                                                         },
        { "info": "Clear the build/us_pc directory."                                                                                  },
        { "info": "Reset the game directory to its initial state, i.e. without patches applied."                                      },
        { "info": "Delete the game directory and all its content."                                                                    },
    ],
    "build": [
        { "info": "Building process duration. The faster, the more power-consuming."                                                  },
        { "info": "Backend used to render the game."                                                                                  },
        { "info": "Game-changing modifications. Not all patches are compatible, so expect errors with some of them."                  },
        { "info": "Replace the game's original textures by custom ones."                                                              },
        { "info": "Replace the game's original sounds by custom ones."                                                                },
        { "info": "Replace the game's original models by custom ones."                                                                },
        { "info": "Replace the game's original audio (musics, jingles, voice clips) by custom ones."                                  },
        { "info": "Compile the game."                                                                                                 },
    ],
}

#
# Builder downloads
#

OMM_BUILDER_DOWNLOADS = {
    "@dynos_patch": {
        "name": "DynOS",
        "type": "patches",
        "dest": "DynOS.patch",
        "path": "DynOS.patch",
        "url" : "https://raw.githubusercontent.com/PeachyPeachSM64/sm64ex-omm-resources/master/dynos.patch",
    },
    "@time_trials_patch": {
        "name": "Time Trials",
        "type": "patches",
        "dest": "time_trials.patch",
        "path": "time_trials.patch",
        "url" : "https://raw.githubusercontent.com/PeachyPeachSM64/sm64ex-omm-resources/master/time_trials.patch",
    },
    "@omm_hd_texture_pack": {
        "name": "OMM HD",
        "type": "textures",
        "dest": "OMM_HD",
        "path": "OMM_HD",
        "url" : "https://github.com/PeachyPeachSM64/sm64ex-omm-resources.git -b omm-hd",
    },
    "@render96_model_pack": {
        "name": "Render96 Model Pack",
        "type": "models",
        "dest": "Render96_Model_Pack.7z",
        "path": "Render96_Model_Pack/Render96_DynOs_v3.2",
        "url" : "https://github.com/Render96/ModelPack/releases/download/3.2/Render96_DynOs_v3.2.7z",
    },
    "@render96_hd_texture_pack": {
        "name": "Render96 HD",
        "type": "textures",
        "dest": "Render96_HD",
        "path": "Render96_HD",
        "url" : "https://github.com/pokeheadroom/RENDER96-HD-TEXTURE-PACK.git -b master",
    },
}

#
# Specs
#

OMM_BUILDER_VERSION = "2.2.1"
OMM_BUILDER_AUTHOR  = "PeachyPeach"
OMM_BUILDER_DATE    = "2022-2023"
OMM_SOURCE_DIR      = "omm"
OMM_SOURCE_DIRS     = [ "assets", "data", "omm" ]
OMM_CUSTOM_EXTS     = [ ".7z", ".zip", ".gz", ".tar", ".rar" ]
OMM_DEPENDENCY_EXTS = [ ".7z", ".zip", ".gz", ".tar", ".rar" ]

#
# Shell commands
#

if os.name == "nt":
    def __bash__(cmd, *args):
        return "bash -c \"" + " ".join([cmd, *args]).replace("\"", "\\\"") + "\""
else:
    def __bash__(cmd, *args):
        return " ".join([cmd, *args])

def run_cmd(cmd, *args, check=False, capture_output=False):
    return subprocess.run(__bash__(cmd, *args), shell=True, check=check, capture_output=capture_output).stdout

def quote(s: str):
    return "\"" + s.strip("\"") + "\""

def copy_file(src: str, dest: str):
    run_cmd("cp", "-f", quote(src), quote(dest))

def copy_dir(src: str, dest: str):
    run_cmd("cp", "-rf", quote(src), quote(dest))

def move_file(src: str, dest: str):
    run_cmd("mv", "-f", quote(src), quote(dest))

def move_dir(src: str, dest: str):
    run_cmd("rm", "-rf", quote(dest))
    run_cmd("mv", "-f", quote(src), quote(dest))

def remove_file(path: str):
    run_cmd("rm", "-f", quote(path))

def remove_dir(path: str):
    run_cmd("rm", "-rf", quote(path))

def download_file(url: str, dest: str = None):
    return run_cmd("wget", "--no-check-certificate", "--no-cache", "--no-cookies", "-q", url, "-O", quote(dest) if dest is not None else "-", capture_output=True)

def git_clone(url: str, dest: str):
    run_cmd("git", "clone", "--single-branch", "--depth", "1", url, quote(dest))

def git_apply(path: str):
    run_cmd("git", "apply", "--reject", "--whitespace=nowarn", quote(path))

def git_reset():
    run_cmd("git", "reset", "-q", "--hard")

def git_clean():
    run_cmd("git", "clean", "-fdx")

def git_pull(repo: str):
    run_cmd("git", "config", "remote.origin.url", repo.split()[0])
    run_cmd("git", "config", "pull.rebase", "true")
    run_cmd("git", "pull")

def zip_files(path: str, dest: str):
    import shutil
    shutil.make_archive(dest, "zip", path, verbose=False)

def unzip_files(path: str, dest: str):
    run_cmd("7z", "x", quote(path), "-o" + quote(dest))

#
# Custom resources
#

def to_title(s: str) -> str:
    s = s.replace("_", " ")
    cap = True
    for i in range(len(s)):
        c = s[i]
        is_letter = ((ord(c) >= ord("A") and ord(c) <= ord("Z")) or (ord(c) >= ord("a") and ord(c) <= ord("z")))
        if cap and is_letter:
            s = s[:i] + c.upper() + s[i + 1:]
            cap = False
        elif not is_letter:
            cap = True
    for i in range(1, len(s) - 1):
        cp = s[i - 1]
        cc = s[i]
        cn = s[i + 1]
        if cc == "." and (not (ord(cp) >= ord("0") and ord(cp) <= ord("9")) or not (ord(cn) >= ord("0") and ord(cn) <= ord("9"))):
            s = s[:i] + " " + s[i + 1:]
    return s

def is_patch_file(path: str) -> bool:
    return os.path.isfile(path) and ".patch" in path and all([exclude not in path.lower() for exclude in ["omm"]])

def get_patch_file(path: str) -> tuple[str, str]:
    name = path[path.rfind("/") + 1:path.find(".patch")]
    return to_title(name), path

def is_texture_pack(path: str) -> bool:
    return os.path.isdir(path) and "/gfx" in path

def get_texture_pack(path: str) -> tuple[str, str]:
    path = path[:path.rfind("/gfx")]
    name = path[path.rfind("/") + 1:]
    return to_title(name), path

def is_sound_pack(path: str) -> bool:
    return os.path.isfile(path) and "sound/sound_data.tbl" in path

def get_sound_pack(path: str) -> tuple[str, str]:
    path = path[:path.rfind("/sound/")]
    name = path[path.rfind("/") + 1:]
    return to_title(name), path

def is_model_pack(path: str) -> bool:
    return os.path.isfile(path) and ("_geo.bin" in path or "model.inc.c" in path)

def get_model_pack(path: str) -> tuple[str, str]:
    if "model.inc.c" in path: path = path[:path.rfind("/model.inc.c")]
    path = path[:path.rfind("/")]
    name = path
    for pattern in ["/actors"]:
        if pattern in name.lower():
            name = name[:name.lower().find(pattern)]
    name = name[name.rfind("/") + 1:]
    return to_title(name), path

def is_audio_pack(path: str) -> bool:
    return os.path.isfile(path) and (".wav" in path or ".ogg" in path)

def get_audio_pack(path: str) -> tuple[str, str]:
    path = path[:path.rfind("/")]
    path = path[:path.rfind("/")]
    name = path
    for pattern in ["/ogg", "/wav", "/dynos", "/audio"]:
        if pattern in name.lower():
            name = name[:name.lower().find(pattern)]
    name = name[name.rfind("/") + 1:]
    return to_title(name), path

OMM_BUILDER_CUSTOM_RESOURCES = {
    "patches": { "is": is_patch_file, "get": get_patch_file },
    "textures": { "is": is_texture_pack, "get": get_texture_pack },
    "sounds": { "is": is_sound_pack, "get": get_sound_pack },
    "models": { "is": is_model_pack, "get": get_model_pack },
    "audios": { "is": is_audio_pack, "get": get_audio_pack },
}

#
# Utils
#

def info(*args, **kwargs):
    print(*args, **kwargs)

def done(*args, **kwargs):
    print(COL_GREEN, *args, FMT_RESET, sep="", **kwargs)

def warning(*args, **kwargs):
    print(COL_YELLOW + "(!) Warning: ", *args, FMT_RESET, sep="", **kwargs)

def error(*args, **kwargs):
    print(COL_RED + "[X] ERROR: ", *args, FMT_RESET, sep="", **kwargs)
    input(COL_YELLOW + "Press ENTER to quit." + FMT_RESET)
    exit(0)

def check_arg(arg: str) -> bool:
    return arg["arg"] in sys.argv

def check_availability(state: dict, menu_item: dict) -> tuple[bool, str]:
    if "checks" in menu_item:
        game = OMM_BUILDER_DATA["game"][state["game"]]

        # Check SM64 rom
        if "rom" in menu_item["checks"] and state["baserom"] is None:
            return False, "Cannot build `" + game["name"] + "` without the original Super Mario 64 US rom."

        # Check executable
        if "exe" in menu_item["checks"] and get_exe("repos/" + game["path"]) is None:
            return False, "Cannot run `" + game["name"] + "`. The game must be built first."

        # Check dependencies
        if "dep" in menu_item["checks"] and game["dep"] and not os.path.isdir(game["dep"]):
            return False, "Cannot build `" + game["name"] + "` due to a missing dependency: `" + game["dep"] + "`."

        # Check directory
        if "dir" in menu_item["checks"] and not os.path.isdir("repos/" + game["path"]):
            return False, "The directory `" + game["path"] + "` does not exist. Build " + game["name"] + " to create it."

        # Check size of list
        if "size" in menu_item["checks"] and len(OMM_BUILDER_DATA[OMM_BUILDER_DATA["menus"][menu_item["val"]]["list"]]) == 0:
            return False, "There is no available element in " + OMM_BUILDER_DATA["menus"][menu_item["val"]]["name"] + "."

        # Check Audio Packs compatibility
        if "audio" in menu_item["checks"] and not game["audio"]:
            state["audios"] = [False for _ in state["audios"]]
            return False, "Only Render96 can use Audio Packs."

    # Available
    return True, None

def is_available(state: dict, menu_item: dict) -> bool:
    res, _ = check_availability(state, menu_item)
    return res

def slen(s: str) -> int:
    length = 0
    escape = False
    for c in s:
        if c == "\033": escape = True
        if not escape: length += 1
        if c == "m": escape = False
    return length

def sanitize_name(name):
    sanitized_name = name
    for c in [" ", "(", ")", "[", "]", "{", "}"]:
        sanitized_name = sanitized_name.replace(c, "_")
    return sanitized_name

def get_version_number(data: str, name: str) -> str:
    tokens = data.replace(":", " ").replace("=", " ").replace("'", " ").replace("\"", " ").split()
    try: i = tokens.index(name)
    except: return None
    return tokens[i+1]

#
# File utils
#

def get_exe(dir: str) -> str:
    for dirpath, _, filenames in os.walk(dir + "/build/us_pc"):
        for filename in filenames:
            if ".exe" in filename or ".f3dex2e" in filename:
                return dirpath + "/" + filename
    return None

def start_game(dir: str):
    exe = get_exe(dir)
    if not exe: error("Game executable not found.")
    run_cmd(f"./{exe}", "&")

def get_omm_version_tag(data: str) -> str:
    version = get_version_number(data, "OMM_VERSION_NUMBER")
    revision = get_version_number(data, "OMM_VERSION_REVISION")
    if version is not None and revision is not None:
        return "omm." + version + "." + revision
    return None

def is_patched(name: str) -> bool:
    return os.path.isfile("enhancements/" + name + ".patched")

def set_patched(name: str):
    os.makedirs("enhancements", exist_ok=True)
    with open("enhancements/" + name + ".patched", "w") as f: f.write("")

def delete_files(root: str, condition):
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            filepath = dirpath + "/" + filename
            if os.path.isfile(filepath) and condition(root, filepath):
                remove_file(filepath)

def fix_typo(filepath: str, str_from: str, str_to: str):
    if os.path.isfile(filepath):
        file = open(filepath, "r", newline="\n", encoding="utf-8", errors="ignore")
        data = file.read()
        file.close()
        edit = False
        done = False
        iloc = 0
        while not done:
            iloc = data.find(str_from, iloc)
            if iloc != -1:
                data = data[:iloc] + str_to + data[iloc + len(str_from):]
                iloc += len(str_to)
                edit = True
            else:
                done = True
        if edit:
            file = open(filepath, "w", newline="\n", encoding="utf-8", errors="ignore")
            file.write(data)
            file.close()

#
# Get a single char from stdin: basically wait for key press
#

get_char = None
try:
    import msvcrt
    def __get_char():
        return run_cmd("read", "-srN1", "x", "&&", "echo", "-n", "$x", check=True, capture_output=True).decode()
    get_char = __get_char
except ImportError:
    import tty, termios
    def __get_char():
        fd = sys.stdin.fileno()
        attr = termios.tcgetattr(fd)
        try:
            tty.setcbreak(fd)
            key = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, attr)
        return key
    get_char = __get_char

#
# GUI
#

def omm_builder_gui_clear():
    global OMM_BUILDER_GUI_BUFFER_0
    OMM_BUILDER_GUI_BUFFER_0.clear()
    OMM_BUILDER_GUI_BUFFER_0.append("")

def omm_builder_gui_print(string: str):
    global OMM_BUILDER_GUI_BUFFER_0
    OMM_BUILDER_GUI_BUFFER_0[-1] += string

def omm_builder_gui_new_line():
    global OMM_BUILDER_GUI_BUFFER_0
    OMM_BUILDER_GUI_BUFFER_0.append("")

def omm_builder_gui_print_buffer():
    global OMM_BUILDER_GUI_BUFFER_0
    global OMM_BUILDER_GUI_BUFFER_1
    if len(OMM_BUILDER_GUI_BUFFER_0) == len(OMM_BUILDER_GUI_BUFFER_1):
        for i in range(len(OMM_BUILDER_GUI_BUFFER_0)):
            if i == 0 or OMM_BUILDER_GUI_BUFFER_0[i] != OMM_BUILDER_GUI_BUFFER_1[i]:
                print("\033[" + str(i + 1) + ";1H" + OMM_BUILDER_GUI_BUFFER_0[i])
    else:
        run_cmd("clear")
        for line in OMM_BUILDER_GUI_BUFFER_0: print(line)
    OMM_BUILDER_GUI_BUFFER_1 = OMM_BUILDER_GUI_BUFFER_0[:]

def omm_builder_gui_get_header_info(state: dict, label: str, menu: int, var: str) -> str:
    if state["menu"] >= menu: return "  " + COL_LYELLOW + label + "  " + COL_WHITE + OMM_BUILDER_DATA[var][state[var]]["name"]
    return ""

def omm_builder_gui_print_header(state: dict):
    header = [
        [omm_builder_gui_get_header_info(state, "Builder Ver ", OMM_BUILDER_GUI_MENU_CLOSE,    "builder" ), COL_LMAGENTA + "<E>" + COL_LGRAY + "  Up    ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_0],
        [omm_builder_gui_get_header_info(state, "OMM Version ", OMM_BUILDER_GUI_MENU_CLOSE,    "version" ), COL_LMAGENTA + "<D>" + COL_LGRAY + "  Down  ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_1],
        [omm_builder_gui_get_header_info(state, "OMM Revision", OMM_BUILDER_GUI_MENU_CLOSE,    "revision"), COL_LMAGENTA + "<S>" + COL_LGRAY + "  Left  ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_2],
        [omm_builder_gui_get_header_info(state, "Game Name   ", OMM_BUILDER_GUI_MENU_COMMANDS, "game"    ), COL_LMAGENTA + "<F>" + COL_LGRAY + "  Right ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_3],
        [omm_builder_gui_get_header_info(state, "Build Speed ", OMM_BUILDER_GUI_MENU_BUILD,    "speed"   ), COL_LMAGENTA + "<C>" + COL_LGRAY + "  Select", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_4],
        [omm_builder_gui_get_header_info(state, "Render API  ", OMM_BUILDER_GUI_MENU_BUILD,    "rapi"    ), COL_LMAGENTA + "<X>" + COL_LGRAY + "  Back  ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_5],
    ]
    len_left   = max([slen(game["name"]) for game in OMM_BUILDER_DATA["game"]]) + 16
    len_middle = max([slen(line[1]) for line in header])
    len_right  = max([slen(line[2]) for line in header])
    pad_left   = (state["width"] - (len_left + len_middle + len_right)) // 2
    pad_right  = (state["width"] - (len_left + len_middle + len_right + pad_left))
    omm_builder_gui_new_line()
    for line in header:
        omm_builder_gui_print(line[0] + FMT_RESET + (" " * (pad_left  + len_left   - slen(line[0]))))
        omm_builder_gui_print(line[1] + FMT_RESET + (" " * (pad_right + len_middle - slen(line[1]))))
        omm_builder_gui_print(line[2] + FMT_RESET + (" " * (            len_right  - slen(line[2]))))
        omm_builder_gui_new_line()
    omm_builder_gui_new_line()

def omm_builder_gui_print_title(state: dict):
    menu = OMM_BUILDER_DATA["menus"][state["menu"]]
    menu_list_count = len(OMM_BUILDER_DATA[menu["list"]])
    menu_title_text = " " + menu["name"] + " (" + COL_WHITE + str(menu_list_count) + COL_LCYAN + ") "
    menu_title_size = slen(menu_title_text)
    menu_title_padl = (state["width"] - 4 - menu_title_size) // 2
    menu_title_padr = (state["width"] - 4 - menu_title_size - menu_title_padl)
    omm_builder_gui_print(" " + FMT_BOLD + COL_LCYAN + "+" + "-" * menu_title_padl + FMT_RESET)
    omm_builder_gui_print(                 COL_LCYAN +       menu_title_text       + FMT_RESET)
    omm_builder_gui_print(      FMT_BOLD + COL_LCYAN + menu_title_padr * "-" + "+" + FMT_RESET)
    omm_builder_gui_new_line()

def omm_builder_gui_print_table_line(state: dict, line: str, len_cols: list, selected: bool = False):
    pad = state["width"] - 6
    omm_builder_gui_print(" " + FMT_BOLD + COL_LCYAN + "| " + FMT_RESET)
    for j in range(len(line)):
        text = line[j]
        if selected: omm_builder_gui_print(FMT_INVERT + FMT_BOLD)
        omm_builder_gui_print(text)
        omm_builder_gui_print(" " * (len_cols[j] - slen(text)))
        pad -= len_cols[j]
    omm_builder_gui_print(" " * pad)
    omm_builder_gui_print(" " + FMT_RESET + FMT_BOLD + COL_LCYAN + "| " + FMT_RESET)
    omm_builder_gui_new_line()

def omm_builder_gui_print_table(state: dict):
    menu = OMM_BUILDER_DATA["menus"][state["menu"]]
    menu_list = OMM_BUILDER_DATA[menu["list"]]
    menu_table = []
    for i, menu_item in enumerate(menu_list):
        item_type   = menu_item["type"]
        item_list   = OMM_BUILDER_DATA["menus"][menu_item["val"]]["list"] if item_type == "list" else None
        item_color  = COL_CYAN if is_available(state, menu_item) else COL_GRAY
        item_values = state[menu_item["set"]] if "set" in menu_item else None
        item_value  = item_values[i] if type(item_values) == list else item_values
        item_row    = [item_color + (("<" + str(i + 1) + ">  ") if len(menu_list) <= 9 else "") + menu_item["name"]]
        if  item_color == COL_GRAY: item_row.append("")
        elif item_type == "toggle": item_row.append(((COL_GREEN + "Enabled") if item_value else (COL_RED + "Disabled")) + item_color)
        elif item_type == "choice": item_row.append(COL_LGRAY + OMM_BUILDER_DATA[menu_item["val"]][item_value]["name"] + item_color)
        elif item_type == "list"  : item_row.append(item_color + str(sum(state[item_list])) + "/" + str(len(state[item_list])) + item_color)
        else:                       item_row.append("")
        menu_table.append(item_row)
    num_rows = len(menu_table)
    num_cols = len(menu_table[0])
    len_cols = [(8 + max([ slen(line[i]) for line in menu_table ])) for i in range(num_cols)]
    for i in range(state["lines"]):
        index = state["start"] + i
        if index < num_rows: omm_builder_gui_print_table_line(state, menu_table[index], len_cols, index == state["index"])
        else: omm_builder_gui_print_table_line(state, [], len_cols)
    omm_builder_gui_print(" " + FMT_BOLD + COL_LCYAN + "+" + "-" * (state["width"] - 4) + "+" + FMT_RESET)
    omm_builder_gui_new_line()

def omm_builder_gui_print_footer(state: dict):
    menu = OMM_BUILDER_DATA["menus"][state["menu"]]
    menu_list = OMM_BUILDER_DATA[menu["list"]]
    menu_item = menu_list[state["index"]]
    res, error = check_availability(state, menu_item)
    text = (OMM_BUILDER_INFO[menu["list"]][state["index"]]["info"] if menu["list"] in OMM_BUILDER_INFO else "") if res else error
    color = COL_LGRAY if res else COL_RED
    if len(text) > state["width"] - 4:
        omm_builder_gui_print("  " + color + text[:state["width"] - 7] + "..." + FMT_RESET)
    else:
        omm_builder_gui_print("  " + color + text + " " * (state["width"] - 4 - len(text)) + FMT_RESET)

def omm_builder_gui_process_input(state: dict):

    # Wait for a key press
    if state["input"]:
        key = state["input"][0]
        state["input"] = state["input"][1:]
    else:
        try:
            key = get_char()
        except:
            print("\033[?25h") # Show cursor
            run_cmd("stty", "echo") # Re-enable echo
            run_cmd("clear")
            exit(0)
    if not key: return

    # Use digits as shortcuts
    if key in OMM_BUILDER_GUI_KEY_DIGIT:
        index = int(key) - 1
        index_max = (min(len(OMM_BUILDER_DATA[OMM_BUILDER_DATA["menus"][state["menu"]]["list"]]), 10) % 10)
        if index < index_max:
            state["index"] = index
            key = OMM_BUILDER_GUI_KEY_ENTER[0]

    # Menu data
    menu = OMM_BUILDER_DATA["menus"][state["menu"]]
    menu_list = OMM_BUILDER_DATA[menu["list"]]
    menu_item = menu_list[state["index"]]
    item_type = menu_item["type"]
    state_idx = state["index"]
    state_set = menu_item["set"] if "set" in menu_item else None
    state_val = state[state_set] if state_set in state else None
    state_val = state_val[state_idx] if type(state_val) == list else state_val
    state_max = len(OMM_BUILDER_DATA[state_set]) if item_type == "choice" else None

    # Key presses
    if key in OMM_BUILDER_GUI_KEY_ENTER:
        if is_available(state, menu_item):
            if   item_type == "toggle": state_val = not state_val
            elif item_type == "choice": state_val = (state_val + 1) % state_max
            elif item_type == "action": state["action"] = menu_item["val"]
            elif item_type == "menu" or item_type == "list":
                state["menu"] = menu_item["val"]
                state["index"] = 0
                state["start"] = 0
                state_val = state_idx
    elif key in OMM_BUILDER_GUI_KEY_BACK:
        state["menu"] = menu["prev"]
        state["index"] = 0
        state["start"] = 0
    elif key in OMM_BUILDER_GUI_KEY_UP:
        state["index"] = max(state["index"] - 1, 0)
        state["start"] = min(state["start"], state["index"])
    elif key in OMM_BUILDER_GUI_KEY_DOWN:
        state["index"] = min(state["index"] + 1, len(menu_list) - 1)
        state["start"] = max(state["start"], state["index"] - state["lines"] + 1)
    elif key in OMM_BUILDER_GUI_KEY_LEFT:
        if   item_type == "toggle": state_val = False
        elif item_type == "choice": state_val = (state_val + state_max - 1) % state_max
    elif key in OMM_BUILDER_GUI_KEY_RIGHT:
        if   item_type == "toggle": state_val = True
        elif item_type == "choice": state_val = (state_val + 1) % state_max

    # Update state value
    if state_set in state and state_val is not None:
        if type(state[state_set]) == list: state[state_set][state_idx] = state_val
        else: state[state_set] = state_val

def omm_builder_gui_init(baserom: str, version: str, resources: dict):

    # Init versions
    OMM_BUILDER_DATA["builder"] = [{"name": OMM_BUILDER_VERSION}]
    OMM_BUILDER_DATA["version"] = [{"name": ".".join(version.split(".")[1:-1])}]
    OMM_BUILDER_DATA["revision"] = [{"name": version.split(".")[-1]}]

    # Init resources
    for resource in OMM_BUILDER_CUSTOM_RESOURCES:
        items = {name.lower(): {"name": name, "path": path} for name, path in resources[resource].items()}
        items = dict(sorted(items.items()))
        OMM_BUILDER_DATA[resource] = [{
            "name": item["name"],
            "path": item["path"],
            "type": "toggle",
            "set": resource
        } for _, item in items.items()]

    # Init builder state
    terminal_width = int(run_cmd("tput", "cols", check=True, capture_output=True))
    terminal_height = int(run_cmd("tput", "lines", check=True, capture_output=True))
    state = {
        "width": terminal_width,
        "height": terminal_height,
        "baserom": baserom,
        "menu": OMM_BUILDER_GUI_MENU_GAMES,
        "action": OMM_BUILDER_GUI_ACTION_NONE,
        "input": "",
        "index": 0,
        "start": 0,
        "lines": terminal_height - 12,
        "game": 0,
        "speed": 2,
        "rapi": 0,
        "builder": 0,
        "version": 0,
        "revision": 0,
        **{resource: [False for _ in resources[resource]] for resource in OMM_BUILDER_CUSTOM_RESOURCES},
    }

    # Init automatic inputs
    for i, arg in enumerate(sys.argv):
        if arg == OMM_BUILDER_ARG_INPUT["arg"] and i + 1 < len(sys.argv):
            state["input"] = sys.argv[i + 1]
            break

    # Resize window if needed
    if state["width"] < 120 or state["height"] < 22:
        state["width"]  = max(state["width"], 120)
        state["height"] = max(state["height"], 22)
        state["lines"]  = state["height"] - 12
        print("\033[8;" + str(state["height"]) + ";" + str(state["width"]) + "t")
    return state

def omm_builder_gui_update(state: dict):
    run_cmd("stty", "-echo") # Disable echo
    print("\033[?25l") # Hide cursor
    while state["menu"] != OMM_BUILDER_GUI_MENU_CLOSE and state["action"] == OMM_BUILDER_GUI_ACTION_NONE:
        omm_builder_gui_clear()
        omm_builder_gui_print_header(state)
        omm_builder_gui_print_title(state)
        omm_builder_gui_print_table(state)
        omm_builder_gui_print_footer(state)
        omm_builder_gui_print_buffer()
        omm_builder_gui_process_input(state)
    print("\033[?25h") # Show cursor
    run_cmd("stty", "echo") # Re-enable echo
    run_cmd("clear")
    print("")
    return state

#
# OMM Builder
#

def omm_builder_init() -> str:

    # Help command
    if check_arg(OMM_BUILDER_ARG_HELP):
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_0 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_1 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_2 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_3 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_4 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_5 + FMT_RESET)
        print("")
        print("A terminal based (G)UI to build Super Mario 64, rom-hacks and more with OMM.")
        print("Easy to use, it allows some levels of customization to enhance your builds.")
        print("")
        print(COL_LYELLOW + "Version: " + COL_WHITE + OMM_BUILDER_VERSION + FMT_RESET)
        print(COL_LYELLOW + "(c): " + COL_WHITE + OMM_BUILDER_AUTHOR + " ~ " + OMM_BUILDER_DATE + FMT_RESET)
        print("")
        print("Command-line arguments:")
        for arg in OMM_BUILDER_ARGS:
            print("  " + COL_LCYAN + arg["arg"] + FMT_RESET + "  " + arg["info"])
        exit(0)

    # Get the baserom
    for filename in os.listdir("."):
        if ".z64" in filename and os.path.isfile(filename):
            return filename
    return None

def omm_builder_check_dependencies():

    # Check Python version
    py_version = sum([float(x) / pow(100, i) for i, x in enumerate(sys.version[:sys.version.find(" ")].split("."))])
    info("Python version: " + COL_CYAN + sys.version + FMT_RESET)
    if py_version < 3.09:
        error("Python v3.9 or later is required.")

    # Check dependencies
    for dependency in ["make", "git", "zip", "unzip", "7z", "cc", "gcc", "g++", "cpp"]:
        try:
            info("Checking dependency: `" + dependency + "`... ", end="")
            run_cmd("command", "-v", dependency, check=True, capture_output=True)
            info(COL_GREEN + "OK" + FMT_RESET)
        except:
            error("Missing dependency. To fix it:\n[Windows] Download and run the `OMM Builder Setup Script`.\n[Linux] Run the command `sudo apt install build-essential git python3 libglew-dev libsdl2-dev zip p7zip*`.")

def omm_builder_check_for_updates():
    if not check_arg(OMM_BUILDER_ARG_INPUT): # Skip builder update if input arg
        omm_builder_version = get_version_number(download_file(OMM_BUILDER_REMOTE_VERSION).decode(), "OMM_BUILDER_VERSION")
        if omm_builder_version is not None and OMM_BUILDER_VERSION != omm_builder_version:
            warning("Your OMM builder is not up-to-date.\nLocal version: ", OMM_BUILDER_VERSION, " - Remote version: ", omm_builder_version)
            info("Do you want to download and install the latest version? [y/n] ", end="")
            answer = ""
            while not answer in ["y", "n"]:
                answer = input().lower()
            if answer == "y":
                info("Updating OMM builder...")
                git_reset()
                git_pull(OMM_REPOSITORIES["omm"])
                done("Done.")
                exit(0)

def omm_builder_get_omm_version() -> tuple[str, bool]:
    info("--- Checking OMM version...")
    remote = get_omm_version_tag(download_file(OMM_SOURCE_REMOTE_VERSION).decode()) if not check_arg(OMM_BUILDER_ARG_LOCAL) else None
    local = get_omm_version_tag(open("omm/omm.mk").read()) if os.path.isfile("omm/omm.mk") else None
    
    # No source available
    if remote is None and local is None:
        error("Cannot retrieve OMM version.")

    # No remote, use local
    if remote is None:
        return local, False

    # No local, use remote and update
    if local is None:
        return remote, True

    # If remote and local don't match, use remote and update
    if remote != local:
        return remote, True

    # If both versions match, don't update
    return remote, False

def omm_builder_extract_zips(path):
    info("--- Extracting zip archives...")
    for dirpath, _, filenames in os.walk(path):
        for filename in filenames:
            fullname = dirpath + "/" + filename
            ext = fullname[fullname.rfind("."):].lower()
            if os.path.isfile(fullname) and ext in OMM_CUSTOM_EXTS:
                dir = sanitize_name(fullname[:fullname.rfind(".")])
                if not os.path.isdir(dir):
                    os.makedirs(dir, exist_ok=True)
                    info("Extracting " + fullname + "...")
                    unzip_files(fullname, dir)

def omm_builder_check_game_dependencies(path):
    info("--- Checking for game dependencies...")
    for filename in os.listdir(path):
        fullname = path + "/" + filename
        ext = fullname[fullname.rfind("."):].lower()
        if os.path.isfile(fullname) and ext in OMM_DEPENDENCY_EXTS:
            name = filename[:filename.rfind(".")].lower()
            for game in OMM_BUILDER_DATA["game"]:
                dep = game["dep"]
                depdir = sanitize_name(path + "/" + dep)
                if dep and dep in name and not os.path.isdir(depdir):
                    info("Found archive for game dependency `" + dep + "`: " + fullname + " - Extracting...")
                    os.makedirs(depdir, exist_ok=True)
                    unzip_files(fullname, depdir)

def omm_builder_sanitize_names(path):
    for dirpath, dirnames, filenames in os.walk(path):
        for dirname in dirnames:
            fullname = dirpath + "/" + dirname
            sanitized = sanitize_name(fullname)
            if fullname != sanitized:
                move_dir(fullname, sanitized)
                return omm_builder_sanitize_names(path)
        for filename in filenames:
            fullname = dirpath + "/" + filename
            sanitized = sanitize_name(fullname)
            if fullname != sanitized:
                move_file(fullname, sanitized)

def omm_builder_scan_custom():
    resources = {
        "patches": {},
        "textures": {},
        "sounds": {},
        "models": {},
        "audios": {},
    }

    # Add scanned resources
    for dirpath, dirnames, filenames in os.walk("custom"):
        for xname in filenames + dirnames:
            for resource, functions in OMM_BUILDER_CUSTOM_RESOURCES.items():
                path = dirpath + "/" + xname
                if functions["is"](path):
                    name, path = functions["get"](path)
                    if name not in resources[resource]:
                        resources[resource][name] = path

    # Add downloadable resources
    for id, res in OMM_BUILDER_DOWNLOADS.items():
        res_path = "custom/" + res["dest"]
        if not os.path.exists(res_path):
            resources[res["type"]][res["name"]] = id

    return resources

def omm_builder_process_command(state: dict, version: str, should_update: bool):
    if state["menu"] != OMM_BUILDER_GUI_MENU_CLOSE and state["action"] != OMM_BUILDER_GUI_ACTION_NONE:

        # State values
        state_action   = state["action"]
        state_game     = state["game"]
        state_speed    = state["speed"]
        state_rapi     = state["rapi"]
        state_patches  = state["patches"]
        state_textures = state["textures"]
        state_sounds   = state["sounds"]
        state_models   = state["models"]
        state_audios   = state["audios"]

        # Game values
        game           = OMM_BUILDER_DATA["game"][state_game]["name"]
        game_path      = OMM_BUILDER_DATA["game"][state_game]["path"]
        game_dir       = "repos/" + game_path
        game_repo      = OMM_REPOSITORIES[game_path]
        game_patches   = OMM_BUILDER_DATA["patches"]
        game_textures  = OMM_BUILDER_DATA["textures"]
        game_sounds    = OMM_BUILDER_DATA["sounds"]
        game_models    = OMM_BUILDER_DATA["models"]
        game_audios    = OMM_BUILDER_DATA["audios"]

        # Make values
        make_builder   = ["OMM_BUILDER=1", "VERSION=us"]
        make_debug     = ["DEBUG=1", "OMM_DEBUG=1"] if check_arg(OMM_BUILDER_ARG_DEBUG) else []
        make_speed     = OMM_BUILDER_DATA["speed"][state_speed]["jobs"]
        make_rapi      = OMM_BUILDER_DATA["rapi"][state_rapi]["flags"]

        # Run the game
        if state_action == OMM_BUILDER_GUI_ACTION_RUN:
            info("--- Starting `" + game + "`...")
            start_game(game_dir)
            exit(0)

        # Clear build directory
        if state_action == OMM_BUILDER_GUI_ACTION_CLEAR:
            info("--- Clearing `" + game + "` build directory...")
            remove_dir(game_dir + "/build")
            done("Done.")
            exit(0)

        # Reset target directory
        if state_action == OMM_BUILDER_GUI_ACTION_RESET:
            info("--- Resetting `" + game + "`...")
            if os.path.isdir(game_dir):
                os.chdir(game_dir)
                git_reset()
                git_clean()
                git_pull(game_repo)
            done("Done.")
            exit(0)

        # Delete target directory
        if state_action == OMM_BUILDER_GUI_ACTION_DELETE:
            info("--- Deleting `" + game + "`...")
            remove_dir(game_dir)
            done("Done.")
            exit(0)

        # Build the game
        if state_action == OMM_BUILDER_GUI_ACTION_BUILD:
            info("--- Building `" + game + "`...")

            # Check OMM source 1: create it or update it to the latest version
            info("--- Checking OMM source (1/3)...")
            if should_update:

                # Delete the omm directory
                info("Removing old OMM source...")
                remove_dir(OMM_SOURCE_DIR)

                # Clone the OMM repository
                info("Downloading latest version...")
                git_clone(OMM_REPOSITORIES["omm"], OMM_SOURCE_DIR)
                if not os.path.isdir(OMM_SOURCE_DIR):
                    error("Cannot clone the OMM source git repository: " + OMM_REPOSITORIES["omm"])

            # Check OMM source 2: delete the non-OMM directories and keep only the files that contain "omm/", "omm_" or "omm."
            info("--- Checking OMM source (2/3)...")
            for dirname in os.listdir(OMM_SOURCE_DIR):
                dirpath = OMM_SOURCE_DIR + "/" + dirname
                if os.path.isdir(dirpath) and dirname not in OMM_SOURCE_DIRS:
                    remove_dir(dirpath)
            is_not_omm_file = lambda root, filepath : not any([pattern in filepath[len(root):] for pattern in ["omm/", "omm_", "omm."]])
            delete_files(OMM_SOURCE_DIR, is_not_omm_file)

            # Clone the game repository
            fresh_clone = False
            if not os.path.isdir(game_dir):
                info("--- Cloning `" + game + "` repository...")
                os.makedirs(game_dir)
                git_clone(game_repo, game_dir)
                if not os.path.isdir(game_dir):
                    error("Cannot clone the git repository: " + game_repo)
                fresh_clone = True
            os.chdir(game_dir)

            # Check OMM source 3: if the latest version is not patched, reset the directory
            info("--- Checking OMM source (3/3)...")
            if not fresh_clone and (should_update or not is_patched(version) or check_arg(OMM_BUILDER_ARG_RESET)):
                info("--- Resetting `" + game + "`...")
                git_reset()
                git_clean()
                git_pull(game_repo)

            # Copy the dependency
            dependency = OMM_BUILDER_DATA["game"][state_game]["dep"]
            if dependency and not is_patched(dependency):
                info("--- Copying dependency contents...")
                copy_dir("../../" + dependency + "/.", ".")
                set_patched(dependency)

            # Delete shitty files
            info("--- Eliminating bad files...")
            is_bad_file = lambda _, filepath : ".png.png" in filepath
            delete_files("textures", is_bad_file)

            # Download missing content
            downloaded = False
            for resource_type in OMM_BUILDER_CUSTOM_RESOURCES.keys():
                for i, resource in enumerate(OMM_BUILDER_DATA[resource_type]):
                    if state[resource_type][i]:
                        res = OMM_BUILDER_DOWNLOADS.get(resource["path"])
                        if res:
                            res_path = "../../custom/" + res["dest"]
                            if not os.path.exists(res_path):
                                if not downloaded:
                                    info("--- Downloading resources...")
                                    downloaded = True
                                info("Downloading `" + res["name"] + "`...")
                                if ".git" in res["url"]:
                                    git_clone(res["url"], res_path)
                                    for name in os.listdir(res_path):
                                        path = res_path + "/" + name
                                        if os.path.isfile(path):
                                            remove_file(path)
                                        elif os.path.isdir(path) and name not in ["gfx", "sound"]:
                                            remove_dir(path)
                                else:
                                    download_file(res["url"], res_path)
                                resource["path"] = "custom/" + res["path"]
            if downloaded:
                omm_builder_extract_zips("../../custom")
                omm_builder_sanitize_names("../../custom")

            # Copy OMM contents
            if not is_patched(version):
                info("--- Copying OMM source...")
                copy_dir("../../" + OMM_SOURCE_DIR + "/.", ".")

            # Apply the selected patches
            if any(state_patches):
                info("--- Applying patches...")
                for i, patch in enumerate(game_patches):
                    if state_patches[i]:
                        patch_name = patch["name"]
                        patch_file = patch["path"]
                        patch_file = patch_file[patch_file.rfind("/") + 1:]
                        patch_path = "../../" + patch["path"]
                        if not is_patched(patch_file) and os.path.isfile(patch_path):
                            info("Applying " + patch_name + "...")
                            git_apply(patch_path)
                            set_patched(patch_file)

            # Apply the Makefile patch and run the patcher
            if not is_patched(version):
                info("--- Patching OMM...")

                # Patch Makefile
                if not os.path.isfile("Makefile"):
                    error("Missing Makefile.")
                file = open("Makefile", "r", newline="\n", encoding="utf-8", errors="ignore")
                data = file.read()
                file.close()
                what = "include omm.mk"
                if data.find(what) == -1:
                    where = data.find("BIN_DIRS := bin bin/$(VERSION)")
                    if where == -1:
                        error("Malformed Makefile.")
                    data = data[:where] + what + "\n" + data[where:]
                    file = open("Makefile", "w", newline="\n", encoding="utf-8", errors="ignore")
                    file.write(data)
                    file.close()

                # Run patcher
                info("python3 omm_patcher.py -p")
                run_cmd("python3", "omm_patcher.py", "-p")
                set_patched(version)

            # Copy the baserom and build the game
            info("--- Building game...")
            copy_file("../../" + state["baserom"], "baserom.us.z64")

            # Make
            make_args = make_speed + make_builder + make_rapi + make_debug

            # OMM make flags
            for arg in sys.argv:
                if "OMM_" in arg and ("=0" in arg or "=1" in arg):
                    make_args.append(arg)

            # Let's-a-go
            info("make", *make_args)
            make_tries = 10
            make_logs = "../../" + game_path + ".logs.txt"
            for make_try in range(make_tries):
                run_cmd("make", *make_args, "2>&1", "|", "tee", quote(make_logs))

                # If the executable is built, everything went right
                if get_exe(".") is not None:
                    break

                # Try to fix itself...
                info("\nCompilation failed. Trying to understand the cause...")
                if not os.path.isfile(make_logs):
                    error("Cannot read logs: Missing file `" + game_path + ".logs.txt`.")

                # ...by reading logs
                retry = False
                with open(make_logs, "r", newline="\n", encoding="utf-8", errors="ignore") as f:
                    data = f.readlines()
                    for line in data:

                        # Target dependency miss
                        if (line[line.find("fatal error"):line.find("No such file or directory")].find("rgba16.inc.c") != -1 or
                            line[line.find("ld.exe"):line.find("No such file or directory")].find("-laudiofile") != -1):
                            info("Some dependencies were missing when building the current target.")
                            info("Running `make` again... Remaining tries: " + str(make_tries - 1 - make_try) + "\n")
                            retry = True
                            break

                # Do make again?
                if not retry:
                    break

            # Last check
            exe = get_exe(".")
            if exe is None:
                error("Something went wrong. Aborting building process...")

            # Install texture packs
            if any(state_textures):
                info("--- Installing Texture Packs...")
                for i, pack in enumerate(game_textures):
                    if state_textures[i]:
                        pack_name = pack["name"]
                        pack_path = pack["path"]
                        pack_dest = sanitize_name(pack_name).lower()
                        info("Installing `" + pack_name + "`...")
                        zip_files("../../" + pack_path, "build/us_pc/res/" + pack_dest)

            # Install sound packs
            if any(state_sounds):
                info("--- Installing Sound Packs...")
                for i, pack in enumerate(game_sounds):
                    if state_sounds[i]:
                        pack_name = pack["name"]
                        pack_path = pack["path"]
                        pack_dest = sanitize_name(pack_name).lower()
                        info("Installing `" + pack_name + "`...")
                        zip_files("../../" + pack_path, "build/us_pc/res/" + pack_dest)

            # Install model packs
            if any(state_models):
                info("--- Installing Model Packs...")
                os.makedirs("build/us_pc/dynos/packs", exist_ok=True)
                for i, pack in enumerate(game_models):
                    if state_models[i]:
                        pack_name = pack["name"]
                        pack_path = pack["path"]
                        pack_dest = sanitize_name(pack_name)
                        info("Installing `" + pack_name + "`...")
                        copy_dir("../../" + pack_path, "build/us_pc/dynos/packs/" + pack_dest)

            # Install audio packs
            if any(state_audios):
                info("--- Installing Audio Packs...")
                os.makedirs("build/us_pc/dynos/audio", exist_ok=True)
                for i, pack in enumerate(game_audios):
                    if state_audios[i]:
                        pack_name = pack["name"]
                        pack_path = pack["path"]
                        info("Installing `" + pack_name + "`...")
                        copy_dir("../../" + pack_path + "/.", "build/us_pc/dynos/audio/")

            # Fix permissions
            run_cmd("chmod", "644", "-f", "-R", "build/us_pc/res")
            run_cmd("chmod", "644", "-f", "-R", "build/us_pc/dynos")
            run_cmd("chmod", "755", "-f", exe)

            # Run the game
            done("Game `" + game + "` built successfully!")
            if not check_arg(OMM_BUILDER_ARG_BUILD):
                info("--- Starting `" + game + "`...")
                start_game(".")
            exit(0)

#
# Main
#

if __name__ == "__main__":
    baserom = omm_builder_init()
    omm_builder_check_dependencies()
    omm_builder_check_for_updates()
    version, should_update = omm_builder_get_omm_version()
    omm_builder_check_game_dependencies(".")
    omm_builder_extract_zips("custom")
    omm_builder_sanitize_names("custom")
    resources = omm_builder_scan_custom()
    state = omm_builder_gui_init(baserom, version, resources)
    state = omm_builder_gui_update(state)
    omm_builder_process_command(state, version, should_update)
    exit(0)