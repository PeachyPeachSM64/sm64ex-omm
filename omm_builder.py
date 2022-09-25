import os
import sys
import filecmp
import subprocess

OMM_IS_TEST             = os.path.isfile("sm64ex-omm-test")
OMM_REPOSITORY          = f"sm64ex-omm" + ("-test" if OMM_IS_TEST else "")
OMM_REPOSITORY_URL      = f"https://github.com/PeachyPeachSM64/{OMM_REPOSITORY}.git -b nightly"
OMM_BUILDER_VERSION_URL = f"https://raw.githubusercontent.com/PeachyPeachSM64/{OMM_REPOSITORY}/master/omm_builder.py"
OMM_VERSION_URL         = f"https://raw.githubusercontent.com/PeachyPeachSM64/{OMM_REPOSITORY}/nightly/omm.mk"
OMM_META_PATCH_URL      = f"https://raw.githubusercontent.com/PeachyPeachSM64/{OMM_REPOSITORY}/master/patch/omm.patch"
OMM_PATCH_URL           = f"https://raw.githubusercontent.com/PeachyPeachSM64/sm64ex-omm-resources/main/omm.patch.bin"
DYNOS_PATCH_URL         = f"https://raw.githubusercontent.com/PeachyPeachSM64/sm64ex-omm-resources/main/dynos.patch"

#
# Get a single char from stdin: basically wait for key press
#

get_char = None
try:
    import msvcrt
    def __get_char():
        return subprocess.check_output("bash -c \"read -srN1 x && echo -n $x\"", shell=True).decode("utf-8")
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
# Data
#

FMT_RESET                               = "\033[0m"
FMT_BOLD                                = "\033[1m"
FMT_ULINE                               = "\033[4m"
FMT_INVERT                              = "\033[7m"
COL_DEFAULT                             = "\033[39m"
COL_BLACK                               = "\033[30m"
COL_RED                                 = "\033[31m"
COL_GREEN                               = "\033[32m"
COL_YELLOW                              = "\033[33m"
COL_BLUE                                = "\033[34m"
COL_MAGENTA                             = "\033[35m"
COL_CYAN                                = "\033[36m"
COL_LGRAY                               = "\033[37m"
COL_GRAY                                = "\033[90m"
COL_LRED                                = "\033[91m"
COL_LGREEN                              = "\033[92m"
COL_LYELLOW                             = "\033[93m"
COL_LBLUE                               = "\033[94m"
COL_LMAGENTA                            = "\033[95m"
COL_LCYAN                               = "\033[96m"
COL_WHITE                               = "\033[97m"
BGC_DEFAULT                             = "\033[49m"
BGC_BLACK                               = "\033[40m"
BGC_RED                                 = "\033[41m"
BGC_GREEN                               = "\033[42m"
BGC_YELLOW                              = "\033[43m"
BGC_BLUE                                = "\033[44m"
BGC_MAGENTA                             = "\033[45m"
BGC_CYAN                                = "\033[46m"
BGC_LGRAY                               = "\033[47m"
BGC_GRAY                                = "\033[100m"
BGC_LRED                                = "\033[101m"
BGC_LGREEN                              = "\033[102m"
BGC_LYELLOW                             = "\033[103m"
BGC_LBLUE                               = "\033[104m"
BGC_LMAGENTA                            = "\033[105m"
BGC_LCYAN                               = "\033[106m"
BGC_WHITE                               = "\033[107m"

OMM_BUILDER_ARG_HELP                    = { "arg": "-h", "info": "Display the help."                                                   }
OMM_BUILDER_ARG_BUILD                   = { "arg": "-b", "info": "Only build the game, don't run it."                                  }
OMM_BUILDER_ARG_RESET                   = { "arg": "-r", "info": "Force a reset before building the game."                             }
OMM_BUILDER_ARG_LOCAL                   = { "arg": "-l", "info": "Use local OMM patch, don't try to check remote version."             }
OMM_BUILDER_ARG_FORCE                   = { "arg": "-f", "info": "Force a download of the latest patch (ignore -l)."                   }
OMM_BUILDER_ARG_DEBUG                   = { "arg": "-d", "info": "Build the game with the make flags DEBUG=1, OMM_DEBUG=1, OMM_DEV=1." }
OMM_BUILDER_ARG_INPUT                   = { "arg": "-i", "info": "Run the input GUI keys one after another."                           }
OMM_BUILDER_ARG_PATCH                   = { "arg": "-p", "info": "Create an OMM patch from the installed version."                     }
OMM_BUILDER_ARG_CLEAR                   = { "arg": "-x", "info": "Clear all OMM patches."                                              }
OMM_BUILDER_ARGS                        = [ OMM_BUILDER_ARG_HELP,
                                            OMM_BUILDER_ARG_BUILD,
                                            OMM_BUILDER_ARG_RESET,
                                            OMM_BUILDER_ARG_LOCAL,
                                            OMM_BUILDER_ARG_FORCE,
                                            OMM_BUILDER_ARG_DEBUG,
                                            OMM_BUILDER_ARG_INPUT,
                                            OMM_BUILDER_ARG_PATCH,
                                            OMM_BUILDER_ARG_CLEAR ]

OMM_BUILDER_GUI_BUFFER_0                = []
OMM_BUILDER_GUI_BUFFER_1                = []

OMM_BUILDER_GUI_LOGO_0                  = "                                    _                    "
OMM_BUILDER_GUI_LOGO_1                  = "  ____ __    ___    __  _____      |_|_    _             "
OMM_BUILDER_GUI_LOGO_2                  = " / __ \| \  / | \  / |  | __ \ _  _ _| | _| | ___  _ __  "
OMM_BUILDER_GUI_LOGO_3                  = "/ /  \ |  \/  |  \/  |  | |/ /| || | | |/   |/ _ \| '_/  "
OMM_BUILDER_GUI_LOGO_4                  = "\ \__/ | |  | | |  | |  | |) \| \/ | | | (] |  __/| |    "
OMM_BUILDER_GUI_LOGO_5                  = " \____/|_|\/|_|_|\/|_|  |____/ \__/|_|_|\___|\___\|_|    "

OMM_BUILDER_GUI_MENU_CLOSE              = 0
OMM_BUILDER_GUI_MENU_GAMES              = 1
OMM_BUILDER_GUI_MENU_COMMANDS           = 2
OMM_BUILDER_GUI_MENU_BUILD              = 3
OMM_BUILDER_GUI_MENU_PATCHES            = 4
OMM_BUILDER_GUI_MENU_TEXTURES           = 5
OMM_BUILDER_GUI_MENU_SOUNDS             = 6
OMM_BUILDER_GUI_MENU_MODELS             = 7
OMM_BUILDER_GUI_MENU_AUDIOS             = 8

OMM_BUILDER_GUI_ACTION_NONE             = 0
OMM_BUILDER_GUI_ACTION_BUILD            = 1
OMM_BUILDER_GUI_ACTION_RUN              = 2
OMM_BUILDER_GUI_ACTION_CLEAR            = 3
OMM_BUILDER_GUI_ACTION_RESET            = 4
OMM_BUILDER_GUI_ACTION_DELETE           = 5

OMM_BUILDER_GUI_KEY_UP                  = "eE"
OMM_BUILDER_GUI_KEY_DOWN                = "dD"
OMM_BUILDER_GUI_KEY_LEFT                = "sS"
OMM_BUILDER_GUI_KEY_RIGHT               = "fF"
OMM_BUILDER_GUI_KEY_ENTER               = "cC"
OMM_BUILDER_GUI_KEY_BACK                = "xX0"
OMM_BUILDER_GUI_KEY_DIGIT               = "123456789"

OMM_BUILDER_BASEROM                     = None
OMM_BUILDER_VERSION                     = "2.0.0"
OMM_PATCH_VERSION                       = ""
OMM_PATCH_REVISION                      = ""
OMM_PATCH_DIRNAME                       = ""
OMM_PATCH_TRUENAME                      = ""

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
        { "name": "Super Mario 64 ex-nightly",       "path": "smex", "repo": "https://github.com/sm64pc/sm64ex.git -b nightly",           "commit": "",                                         "dep": "",          "dynos": True,  "model": True, "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario 64 ex-alo",           "path": "xalo", "repo": "https://github.com/AloXado320/sm64ex-alo.git -b master",    "commit": "b9283d080d8f82befe3917a916843cbfb1399411", "dep": "",          "dynos": True,  "model": True, "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario 64 Moonshine",        "path": "smms", "repo": "https://github.com/sm64pc/sm64ex.git -b nightly",           "commit": "",                                         "dep": "moonshine", "dynos": True,  "model": True, "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario 74",                  "path": "sm74", "repo": "https://github.com/PeachyPeachSM64/sm64ex-omm.git -b sm74", "commit": "",                                         "dep": "",          "dynos": True,  "model": True, "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario Star Road",           "path": "smsr", "repo": "https://github.com/PeachyPeachSM64/sm64ex-omm.git -b smsr", "commit": "",                                         "dep": "",          "dynos": True,  "model": True, "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Super Mario 64: The Green Stars", "path": "smgs", "repo": "https://github.com/PeachyPeachSM64/sm64ex-omm.git -b smgs", "commit": "",                                         "dep": "",          "dynos": True,  "model": True, "audio": False, "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
        { "name": "Render96",                        "path": "r96x", "repo": "https://github.com/Render96/Render96ex.git -b tester",      "commit": "",                                         "dep": "",          "dynos": False, "model": True, "audio": True,  "type": "menu", "val": OMM_BUILDER_GUI_MENU_COMMANDS, "set": "game" },
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
        { "name": "DynOS",         "type": "toggle",                 "set": "dynos",       "checks": "dynos"      },
        { "name": "Patches",       "type": "list",   "val": OMM_BUILDER_GUI_MENU_PATCHES,  "checks": "size"       },
        { "name": "Texture Packs", "type": "list",   "val": OMM_BUILDER_GUI_MENU_TEXTURES, "checks": "size"       },
        { "name": "Sound Packs",   "type": "list",   "val": OMM_BUILDER_GUI_MENU_SOUNDS,   "checks": "size"       },
        { "name": "Model Packs",   "type": "list",   "val": OMM_BUILDER_GUI_MENU_MODELS,   "checks": "size;model" },
        { "name": "Audio Packs",   "type": "list",   "val": OMM_BUILDER_GUI_MENU_AUDIOS,   "checks": "size;audio" },
        { "name": "Build and Run", "type": "action", "val": OMM_BUILDER_GUI_ACTION_BUILD                          },
    ],
    "speed": [
        { "name": "Slow",    "jobs":  ""                         },
        { "name": "Fast",    "jobs": f" -j{os.cpu_count() // 2}" },
        { "name": "Faster",  "jobs": f" -j{os.cpu_count()}"      },
        { "name": "Fastest", "jobs":  " -j"                      },
    ],
    "rapi": [
        { "name": "OpenGL 2.1", "flags": " RENDER_API=GL WINDOW_API=SDL2 AUDIO_API=SDL2 CONTROLLER_API=SDL2 GRUCODE=f3dex2e"    },
        { "name": "DirectX 11", "flags": " RENDER_API=D3D11 WINDOW_API=DXGI AUDIO_API=SDL2 CONTROLLER_API=SDL2 GRUCODE=f3dex2e" },
    ],
    "patches": [],
    "textures": [],
    "sounds": [],
    "models": [],
    "audios": [],
}

OMM_BUILDER_INFO = {
    "game": [
        { "info": "PC Port of Super Mario 64 with additional features. DynOS is available as a patch."                                },
        { "info": "Up-to-date PC Port of Super Mario 64 featuring enhancements and optimizations from HackerSM64."                    },
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
        { "info": "Patch the latest version of DynOS to enable Model Packs support as well as an enhanced options menu."              },
        { "info": "Game-changing modifications. Not all patches are compatible, so expect errors with some of them."                  },
        { "info": "Replace the game's original textures by custom ones."                                                              },
        { "info": "Replace the game's original sounds by custom ones."                                                                },
        { "info": "Replace the game's original models by custom ones. DynOS must be patched or built-in for Model Packs to work."     },
        { "info": "Replace the game's original audio (musics, jingles, voice clips) by custom ones."                                  },
        { "info": "Compile the game."                                                                                                 },
    ],
}

#
# Custom resources
#

def to_title(s: str) -> str:
    s = s.replace('_', ' ')
    cap = True
    for i in range(len(s)):
        c = s[i]
        is_letter = ((ord(c) >= ord('A') and ord(c) <= ord('Z')) or (ord(c) >= ord('a') and ord(c) <= ord('z')))
        if cap and is_letter:
            s = s[:i] + c.upper() + s[i + 1:]
            cap = False
        elif not is_letter:
            cap = True
    for i in range(1, len(s) - 1):
        cp = s[i - 1]
        cc = s[i]
        cn = s[i + 1]
        if cc == '.' and (not (ord(cp) >= ord('0') and ord(cp) <= ord('9')) or not (ord(cn) >= ord('0') and ord(cn) <= ord('9'))):
            s = s[:i] + ' ' + s[i + 1:]
    return s

def is_patch_file(path: str) -> bool:
    return os.path.isfile(path) and ".patch" in path and all([ exclude not in path.lower() for exclude in [ "omm", "dynos" ]])

def get_patch_file(path: str) -> tuple[str, str]:
    name = path[path.rfind('/') + 1:path.find('.patch')]
    return to_title(name), path

def is_texture_pack(path: str) -> bool:
    return os.path.isdir(path) and "/gfx" in path

def get_texture_pack(path: str) -> tuple[str, str]:
    path = path[:path.rfind("/gfx")]
    name = path[path.rfind('/') + 1:]
    return to_title(name), path

def is_sound_pack(path: str) -> bool:
    return os.path.isfile(path) and "sound/sound_data.tbl" in path

def get_sound_pack(path: str) -> tuple[str, str]:
    path = path[:path.rfind("/sound/")]
    name = path[path.rfind('/') + 1:]
    return to_title(name), path

def is_model_pack(path: str) -> bool:
    return os.path.isfile(path) and ("_geo.bin" in path or "model.inc.c" in path)

def get_model_pack(path: str) -> tuple[str, str]:
    if "model.inc.c" in path: path = path[:path.rfind("/model.inc.c")]
    path = path[:path.rfind('/')]
    name = path
    for pattern in [ "/actors" ]:
        if pattern in name.lower():
            name = name[:name.lower().find(pattern)]
    name = name[name.rfind('/') + 1:]
    return to_title(name), path

def is_audio_pack(path: str) -> bool:
    return os.path.isfile(path) and (".wav" in path or ".ogg" in path)

def get_audio_pack(path: str) -> tuple[str, str]:
    path = path[:path.rfind('/')]
    path = path[:path.rfind('/')]
    name = path
    for pattern in [ "/ogg", "/wav", "/dynos", "/audio" ]:
        if pattern in name.lower():
            name = name[:name.lower().find(pattern)]
    name = name[name.rfind('/') + 1:]
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

def raise_error(error: str):
    print(COL_RED + "<!> ERROR: " + error + FMT_RESET)
    input(COL_YELLOW + "Press ENTER to quit." + FMT_RESET)
    exit(0)

def check_arg(arg: str) -> bool:
    return arg["arg"] in sys.argv

def check_availability(state: dict, menu_item: dict) -> tuple[bool, str]:
    if "checks" in menu_item:
        game = OMM_BUILDER_DATA["game"][state["game"]]

        # Check SM64 rom
        if "rom" in menu_item["checks"] and OMM_BUILDER_BASEROM is None:
            return False, "Cannot build " + game["name"] + " without the original Super Mario 64 US rom."

        # Check executable
        if "exe" in menu_item["checks"] and get_exe("repos/" + game["path"]) is None:
            return False, "Cannot run " + game["name"] + ". The game must be built first."

        # Check dependencies
        if "dep" in menu_item["checks"] and game["dep"] and not os.path.isdir(game["dep"]):
            return False, "Cannot build " + game["name"] + " due to a missing dependency: '" + game["dep"] + "'."

        # Check directory
        if "dir" in menu_item["checks"] and not os.path.isdir("repos/" + game["path"]):
            return False, "The directory '" + game["path"] + "' does not exist. Build " + game["name"] + " to create it."

        # Check size of list
        if "size" in menu_item["checks"] and len(OMM_BUILDER_DATA[OMM_BUILDER_DATA["menus"][menu_item["val"]]["list"]]) == 0:
            return False, "There is no available element in " + OMM_BUILDER_DATA["menus"][menu_item["val"]]["name"] + "."

        # Check DynOS patchable
        if "dynos" in menu_item["checks"] and not game["dynos"]:
            state["dynos"] = False
            return False, "DynOS cannot be patched to " + game["name"] + "."

        # Check Model Packs compatibility
        if "model" in menu_item["checks"] and not game["model"]:
            state["models"] = [ False for _ in state["models"] ]
            return False, "Model Packs cannot be used without DynOS."

        # Check Audio Packs compatibility
        if "audio" in menu_item["checks"] and not game["audio"]:
            state["audios"] = [ False for _ in state["audios"] ]
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
        if c == '\033': escape = True
        if not escape: length += 1
        if c == 'm': escape = False
    return length

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
    if not exe: raise_error("Game executable not found.")
    if ".exe" in exe: os.system("start " + exe)
    else: os.system("./" + exe + " &")

def get_omm_version(path: str) -> dict:
    version = {
        "OMM_VERSION_NUMBER": "",
        "OMM_VERSION_REVISION": "",
    }
    if os.path.isfile(path):
        with open(path, "r") as f:
            data = f.read()
            for k in version.keys():
                i0 = data.find(k + " :=")
                i1 = data.find("\n", i0)
                if i0 != -1 and i1 != -1:
                    version[k] = data[i0 + len(k + " :="):i1].lstrip(' \t\r\n').rstrip(' \t\r\n')
    return version

def get_omm_patch_dir() -> str:
    for dirname in os.listdir("."):
        if os.path.isdir(dirname) and "omm." in dirname:
            return dirname
    return ""

def is_patched(name: str) -> bool:
    return os.path.isfile("enhancements/" + name + ".patched")

def set_patched(name: str):
    os.makedirs("enhancements", exist_ok=True)
    with open("enhancements/" + name + ".patched", 'w') as f: f.write("")

def apply_patch(filepath: str):
    os.system("git apply --reject --whitespace=nowarn \"" + filepath + "\"")

def remove_files(root: str, condition):
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            filepath = dirpath + "/" + filename
            if os.path.isfile(filepath) and condition(root, filepath):
                os.remove(filepath)

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

def create_omm_patch():
    dir = get_omm_patch_dir()
    if len(dir) > 0:
        os.system("git clone --single-branch https://github.com/sm64pc/sm64ex.git -b nightly temp -q")
        if os.path.isdir("temp"):
            os.chdir("temp")
            os.system("cp -rf \"../" + dir + "/.\" .")
            os.system("git add .")
            os.system("git diff --diff-algorithm=minimal --unified=2 -p --staged --binary > ../omm.patch.bin")
            os.chdir("..")
            os.system("rm -rf temp")

def clear_omm_patches():
    for dirname in os.listdir("."):
        if os.path.isdir(dirname) and "omm." in dirname:
            os.system("rm -rf \"" + dirname + "\"")

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
        os.system("clear")
        for line in OMM_BUILDER_GUI_BUFFER_0: print(line)
    OMM_BUILDER_GUI_BUFFER_1 = OMM_BUILDER_GUI_BUFFER_0[:]

def omm_builder_gui_get_header_info(state: dict, label: str, menu: int, var: str) -> str:
    if var not in OMM_BUILDER_DATA: return "  " + COL_LYELLOW + label + "  " + COL_WHITE + var
    if state["menu"] >= menu:       return "  " + COL_LYELLOW + label + "  " + COL_WHITE + OMM_BUILDER_DATA[var][state[var]]["name"]
    return ""

def omm_builder_gui_print_header(state: dict):
    header = [
        [ omm_builder_gui_get_header_info(state, "Builder Ver ", OMM_BUILDER_GUI_MENU_CLOSE,    OMM_BUILDER_VERSION), COL_LMAGENTA + "<E>" + COL_LGRAY + "  Up    ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_0 ],
        [ omm_builder_gui_get_header_info(state, "OMM Version ", OMM_BUILDER_GUI_MENU_CLOSE,    OMM_PATCH_VERSION  ), COL_LMAGENTA + "<D>" + COL_LGRAY + "  Down  ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_1 ],
        [ omm_builder_gui_get_header_info(state, "OMM Revision", OMM_BUILDER_GUI_MENU_CLOSE,    OMM_PATCH_REVISION ), COL_LMAGENTA + "<S>" + COL_LGRAY + "  Left  ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_2 ],
        [ omm_builder_gui_get_header_info(state, "Game Name   ", OMM_BUILDER_GUI_MENU_COMMANDS, "game"             ), COL_LMAGENTA + "<F>" + COL_LGRAY + "  Right ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_3 ],
        [ omm_builder_gui_get_header_info(state, "Build Speed ", OMM_BUILDER_GUI_MENU_BUILD,    "speed"            ), COL_LMAGENTA + "<C>" + COL_LGRAY + "  Select", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_4 ],
        [ omm_builder_gui_get_header_info(state, "Render API  ", OMM_BUILDER_GUI_MENU_BUILD,    "rapi"             ), COL_LMAGENTA + "<X>" + COL_LGRAY + "  Back  ", FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_5 ],
    ]
    len_left   = max([ slen(game["name"]) for game in OMM_BUILDER_DATA["game"] ]) + 16
    len_middle = max([ slen(line[1]) for line in header ])
    len_right  = max([ slen(line[2]) for line in header ])
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
        item_row    = [ item_color + (("<" + str(i + 1) + ">  ") if len(menu_list) <= 9 else "") + menu_item["name"] ]
        if  item_color == COL_GRAY: item_row.append("")
        elif item_type == "toggle": item_row.append(((COL_GREEN + "Enabled") if item_value else (COL_RED + "Disabled")) + item_color)
        elif item_type == "choice": item_row.append(COL_LGRAY + OMM_BUILDER_DATA[menu_item["val"]][item_value]["name"] + item_color)
        elif item_type == "list"  : item_row.append(item_color + str(sum(state[item_list])) + "/" + str(len(state[item_list])) + item_color)
        else:                       item_row.append("")
        menu_table.append(item_row)
    num_rows = len(menu_table)
    num_cols = len(menu_table[0])
    len_cols = [ (8 + max([ slen(line[i]) for line in menu_table ])) for i in range(num_cols) ]
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
            os.system("stty echo") # Re-enable echo
            os.system("clear")
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

def omm_builder_gui_init(resources: dict):

    # Init resources
    for resource in OMM_BUILDER_CUSTOM_RESOURCES:
        items = { name.lower(): { "name": name, "path": path } for name, path in resources[resource].items() }
        items = dict(sorted(items.items()))
        OMM_BUILDER_DATA[resource] = [ {
            "name": item["name"],
            "path": item["path"],
            "type": "toggle",
            "set": resource
        } for _, item in items.items() ]

    # Init builder state
    state = {
        "width": int(subprocess.check_output("tput cols", shell=True)),
        "height": int(subprocess.check_output("tput lines", shell=True)),
        "menu": OMM_BUILDER_GUI_MENU_GAMES,
        "action": OMM_BUILDER_GUI_ACTION_NONE,
        "input": "",
        "index": 0,
        "start": 0,
        "lines": int(subprocess.check_output("tput lines", shell=True)) - 12,
        "game": 0,
        "speed": 2,
        "rapi": 0,
        "dynos": False,
        **{ resource: [ False for _ in resources[resource] ] for resource in OMM_BUILDER_CUSTOM_RESOURCES },
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
    os.system("stty -echo") # Disable echo
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
    os.system("stty echo") # Re-enable echo
    os.system("clear")
    print("")
    return state

#
# OMM Builder
#

def omm_builder_init():

    # Help command
    if check_arg(OMM_BUILDER_ARG_HELP):
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_0 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_1 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_2 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_3 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_4 + FMT_RESET)
        print(FMT_BOLD + COL_LYELLOW + OMM_BUILDER_GUI_LOGO_5 + FMT_RESET)
        print("")
        print(COL_LYELLOW + "Version: " + COL_WHITE + OMM_BUILDER_VERSION + FMT_RESET)
        print("")
        print("Command-line arguments:")
        for arg in OMM_BUILDER_ARGS:
            print("  " + COL_LCYAN + arg["arg"] + FMT_RESET + "  " + arg["info"])
        exit(0)

    # Test repo access git credentials
    if OMM_IS_TEST:
        print("+------------------------- OMM TEST REPOSITORY -------------------------+")
        print("| To make sure you have access to the repository, please authenticate.  |")
        print("| - If you're already authenticated, this step will be skipped.         |")
        print("| - If the authentication fails, the builder won't start.               |")
        print("+-----------------------------------------------------------------------+")
        os.system("git config --global credential.helper store")
        try: subprocess.check_output("git pull -q", shell=True)
        except:
            input("Press ENTER to quit.")
            exit(0)

    # Get the baserom
    global OMM_BUILDER_BASEROM
    for _, _, filenames in os.walk("."):
        for filename in filenames:
            if ".z64" in filename:
                OMM_BUILDER_BASEROM = filename
                return

def omm_builder_check_dependencies():

    # Check Python version
    py_version = sum([ float(x) / pow(100, i) for i, x in enumerate(sys.version[:sys.version.find(' ')].split('.'))])
    print("Python version: " + sys.version)
    if py_version < 3.09:
        raise_error("Python v3.9 or later is required.")

    # Check dependencies
    for dep, cmd in [
        ( "make",  "make -help" ),
        ( "git",   "git help"   ),
        ( "zip",   "zip -hh"    ),
        ( "unzip", "unzip -hh"  ),
        ( "7z",    "7z"         ),
        ( "cc",    "cc --help"  ),
        ( "gcc",   "gcc --help" ),
        ( "g++",   "g++ --help" ),
        ( "cpp",   "cpp --help" ),
    ]:
        try:
            print("Checking dependency: '" + dep + "'... ", end='')
            subprocess.check_output(cmd, shell=True)
            print(COL_GREEN + "OK" + FMT_RESET)
        except:
            raise_error("Missing dependency. To fix it:\n[Windows] Download and run the 'OMM Builder Setup Script'.\n[Linux] Run the command 'sudo apt install build-essential git python3 libglew-dev libsdl2-dev zip p7zip*'.")

def omm_builder_check_for_updates():
    if not check_arg(OMM_BUILDER_ARG_INPUT): # Skip builder update if input arg
        os.system(f"wget --no-check-certificate --no-cache --no-cookies {OMM_BUILDER_VERSION_URL} -O omm_builder.version -q || rm -f omm_builder.version")
        if os.path.isfile("omm_builder.version"):
            filecmp.clear_cache()
            up_to_date = filecmp.cmp("omm_builder.py", "omm_builder.version", shallow=False)
            os.remove("omm_builder.version")
            if not up_to_date:
                print("Your OMM builder is not up-to-date.")
                print("Do you want to download and install the latest version? [y/n] ", end='')
                answer = ""
                while not answer in ["y", "n"]:
                    answer = input().lower()
                if answer == "y":
                    print("Updating OMM builder...")
                    os.system("git config pull.rebase true")
                    os.system("git checkout master -q")
                    os.system("git pull -q")
                    os.system("rm -rf custom")
                    print("Done.")
                    exit(0)

def omm_builder_check_omm_commands():

    # Create .patch file
    if check_arg(OMM_BUILDER_ARG_PATCH):
        print("--- Creating OMM patch file...")
        create_omm_patch()
        print("Done.")
        exit(0)
    
    # Delete all omm* directories
    if check_arg(OMM_BUILDER_ARG_CLEAR):
        print("--- Clearing OMM patches...")
        clear_omm_patches()
        print("Done.")
        exit(0)

def omm_builder_check_omm_version():
    global OMM_PATCH_VERSION
    global OMM_PATCH_REVISION
    global OMM_PATCH_DIRNAME
    global OMM_PATCH_TRUENAME
    print("--- Checking OMM version...")
    if not check_arg(OMM_BUILDER_ARG_LOCAL): os.system(f"wget --no-check-certificate --no-cache --no-cookies {OMM_VERSION_URL} -O omm.version -q || rm -f omm.version")
    if os.path.isfile("omm.version"):
        version = get_omm_version("omm.version")
        OMM_PATCH_VERSION = version["OMM_VERSION_NUMBER"]
        OMM_PATCH_REVISION = version["OMM_VERSION_REVISION"]
        if not OMM_PATCH_VERSION: raise_error("Cannot retrieve remote OMM version number.")
        if not OMM_PATCH_REVISION: raise_error("Cannot retrieve remote OMM revision number.")
        OMM_PATCH_DIRNAME = "omm." + OMM_PATCH_VERSION + "." + OMM_PATCH_REVISION
        os.remove("omm.version")
    elif check_arg(OMM_BUILDER_ARG_FORCE):
        print("Forcing update of OMM...")
        OMM_PATCH_VERSION = "latest"
        OMM_PATCH_REVISION = "1"
        OMM_PATCH_DIRNAME = "omm." + OMM_PATCH_VERSION + "." + OMM_PATCH_REVISION
    else:
        print("Unable to retrieve remote OMM version number.")
        print("Using local OMM patch...")
        OMM_PATCH_DIRNAME = get_omm_patch_dir()
        if not OMM_PATCH_DIRNAME: raise_error("Cannot find any OMM patch.")
        version = get_omm_version(OMM_PATCH_DIRNAME + "/omm.mk")
        OMM_PATCH_VERSION = version["OMM_VERSION_NUMBER"]
        OMM_PATCH_REVISION = version["OMM_VERSION_REVISION"]
        if not OMM_PATCH_VERSION: raise_error("Cannot retrieve local OMM version number.")
        if not OMM_PATCH_REVISION: raise_error("Cannot retrieve local OMM revision number.")
    OMM_PATCH_TRUENAME = "omm." + OMM_PATCH_VERSION + "." + OMM_PATCH_REVISION
    print("Version: " + OMM_PATCH_VERSION)
    print("Revision: " + OMM_PATCH_REVISION)
    print("Directory: " + OMM_PATCH_DIRNAME)

def omm_builder_extract_zips():
    print("--- Extracting zip archives...")
    for dirpath, _, filenames in os.walk("custom"):
        for filename in filenames:
            fullname = dirpath + "/" + filename
            ext = fullname[fullname.rfind('.'):].lower()
            if os.path.isfile(fullname) and ext in [".7z", ".zip", ".gz", ".tar", ".rar"]:
                dir = fullname[:fullname.rfind('.')]
                if not os.path.isdir(dir):
                    os.makedirs(dir, exist_ok=True)
                    print("Extracting " + fullname + "...")
                    os.system("7z x \"" + fullname + "\" -o\"" + dir + "\" > /dev/null 2>&1")

def omm_builder_scan_custom():
    resources = {
        "patches": {},
        "textures": {},
        "sounds": {},
        "models": {},
        "audios": {},
    }
    for dirpath, dirnames, filenames in os.walk("custom"):
        for xname in filenames + dirnames:
            for resource, functions in OMM_BUILDER_CUSTOM_RESOURCES.items():
                path = dirpath + "/" + xname
                if functions["is"](path):
                    name, path = functions["get"](path)
                    if name not in resources[resource]:
                        resources[resource][name] = path
    return resources

def omm_builder_process_command(state: dict):
    if state["menu"] != OMM_BUILDER_GUI_MENU_CLOSE and state["action"] != OMM_BUILDER_GUI_ACTION_NONE:

        # State values
        state_action   = state["action"]
        state_game     = state["game"]
        state_speed    = state["speed"]
        state_rapi     = state["rapi"]
        state_dynos    = state["dynos"]
        state_patches  = state["patches"]
        state_textures = state["textures"]
        state_sounds   = state["sounds"]
        state_models   = state["models"]
        state_audios   = state["audios"]

        # Game values
        game           = OMM_BUILDER_DATA["game"][state_game]["name"]
        game_path      = OMM_BUILDER_DATA["game"][state_game]["path"]
        game_dir       = "repos/" + game_path
        game_repo      = OMM_BUILDER_DATA["game"][state_game]["repo"]
        game_commit    = OMM_BUILDER_DATA["game"][state_game]["commit"]
        game_patches   = OMM_BUILDER_DATA["patches"]
        game_textures  = OMM_BUILDER_DATA["textures"]
        game_sounds    = OMM_BUILDER_DATA["sounds"]
        game_models    = OMM_BUILDER_DATA["models"]
        game_audios    = OMM_BUILDER_DATA["audios"]

        # Make values
        make_debug     = " DEBUG=1 OMM_DEBUG=1 OMM_DEV=1" if check_arg(OMM_BUILDER_ARG_DEBUG) else ""
        make_speed     = OMM_BUILDER_DATA["speed"][state_speed]["jobs"]
        make_rapi      = OMM_BUILDER_DATA["rapi"][state_rapi]["flags"]

        # Run the game
        if state_action == OMM_BUILDER_GUI_ACTION_RUN:
            print("--- Starting " + game + "...")
            start_game(game_dir)
            exit(0)

        # Clear build directory
        if state_action == OMM_BUILDER_GUI_ACTION_CLEAR:
            print("--- Clearing " + game + " build directory...")
            os.system("rm -rf \"" + game_dir + "/build\"")
            print("Done.")
            exit(0)

        # Reset target directory
        if state_action == OMM_BUILDER_GUI_ACTION_RESET:
            print("--- Resetting " + game + "...")
            if os.path.isdir(game_dir):
                os.chdir(game_dir)
                os.system("git config pull.rebase true")
                os.system("git reset -q --hard")
                os.system("git clean -q -fdx")
                os.system("git pull  -q")
                os.system("git reset -q --hard " + game_commit)
            print("Done.")
            exit(0)

        # Delete target directory
        if state_action == OMM_BUILDER_GUI_ACTION_DELETE:
            print("--- Deleting " + game + "...")
            os.system("rm -rf \"" + game_dir + "\"")
            print("Done.")
            exit(0)

        # Build the game
        if state_action == OMM_BUILDER_GUI_ACTION_BUILD:
            print("--- Building " + game + "...")
            global OMM_BUILDER_BASEROM
            global OMM_PATCH_VERSION
            global OMM_PATCH_REVISION
            global OMM_PATCH_DIRNAME
            global OMM_PATCH_TRUENAME

            # Check OMM patch: Create it or update it to the latest version
            print("--- Checking OMM patch (1/3)...")
            local_patch_dir = get_omm_patch_dir()
            if local_patch_dir is not None and os.path.isdir(local_patch_dir):
                local_patch = get_omm_version(local_patch_dir + "/omm.mk")
                local_patch_ver = local_patch["OMM_VERSION_NUMBER"]
                local_patch_rev = local_patch["OMM_VERSION_REVISION"]
            else:
                local_patch_ver = ""
                local_patch_rev = ""
            up_to_date = not check_arg(OMM_BUILDER_ARG_FORCE) and (local_patch_ver == OMM_PATCH_VERSION) and (local_patch_rev == OMM_PATCH_REVISION)
            if not up_to_date or not os.path.isdir(OMM_PATCH_DIRNAME):

                # Delete all omm.* directories
                print("Removing old OMM patches...")
                clear_omm_patches()

                # Clone the OMM repository
                OMM_PATCH_DIRNAME = OMM_PATCH_TRUENAME
                print("Creating '" + OMM_PATCH_DIRNAME + "' from the latest version...")
                os.system("git clone --single-branch " + OMM_REPOSITORY_URL + " " + OMM_PATCH_DIRNAME)
                if not os.path.isdir(OMM_PATCH_DIRNAME):
                    raise_error("Cannot clone the git repository: " + OMM_REPOSITORY_URL)

            # Check OMM patch: Delete the .git, .github and .vscode directories and keep only the files that contain "omm/", "omm_" or "omm."
            print("--- Checking OMM patch (2/3)...")
            os.system("rm -rf \"" + OMM_PATCH_DIRNAME + "/.git\"")
            os.system("rm -rf \"" + OMM_PATCH_DIRNAME + "/.github\"")
            os.system("rm -rf \"" + OMM_PATCH_DIRNAME + "/.vscode\"")
            is_not_omm_file = lambda root, filepath : not any([ pattern in filepath[len(root):] for pattern in ["omm/", "omm_", "omm."] ])
            remove_files(OMM_PATCH_DIRNAME, is_not_omm_file)

            # Clone the game repository
            fresh_clone = False
            if not os.path.isdir(game_dir):
                print("--- Cloning " + game + " repository...")
                os.makedirs(game_dir)
                os.system("git clone --single-branch " + game_repo + " " + game_dir)
                if not os.path.isdir(game_dir):
                    raise_error("Cannot clone the git repository: " + game_repo)
                os.chdir(game_dir)
                os.system("git reset -q --hard " + game_commit)
                fresh_clone = True
            else:
                os.chdir(game_dir)

            # Check OMM patch: if a omm.*.patched file exists but is not the latest version, reset the directory
            print("--- Checking OMM patch (3/3)...")
            if not fresh_clone and (not up_to_date or not is_patched(OMM_PATCH_TRUENAME) or check_arg(OMM_BUILDER_ARG_RESET)):
                print("--- Resetting " + game + "...")
                os.system("git config pull.rebase true")
                os.system("git reset -q --hard")
                os.system("git clean -q -fdx")
                os.system("git pull  -q")
                os.system("git reset -q --hard " + game_commit)

            # Copy the dependency
            dependency = OMM_BUILDER_DATA["game"][state_game]["dep"]
            if dependency and not is_patched(dependency):
                print("--- Copying dependency contents...")
                os.system("cp -rf \"../../" + dependency + "/.\" .")
                set_patched(dependency)

            # Delete shitty files
            print("--- Eliminating bad files...")
            is_bad_file = lambda _, filepath : ".png.png" in filepath
            remove_files(".", is_bad_file)

            # Fix typos
            print("--- Fixing typos...")
            if os.path.isfile("sound/sound_data.s"):
                fix_typo("Makefile",
                    "$(SOUND_BIN_DIR)/sound_data.ctl $(SOUND_BIN_DIR)/sound_data.tbl $(SOUND_BIN_DIR)/sequences.bin $(SOUND_BIN_DIR)/bank_sets",
                    "$(SOUND_BIN_DIR)/sound_data.ctl.inc.c $(SOUND_BIN_DIR)/sound_data.tbl.inc.c $(SOUND_BIN_DIR)/sequences.bin.inc.c $(SOUND_BIN_DIR)/bank_sets.inc.c"
                )
                os.remove("sound/sound_data.s")
                with open("sound/sound_data.c", 'w') as f:
                    f.write('unsigned char gSoundDataADSR[] = {\n')
                    f.write('#include "sound/sound_data.ctl.inc.c"\n')
                    f.write('};\n')
                    f.write('unsigned char gSoundDataRaw[] = {\n')
                    f.write('#include "sound/sound_data.tbl.inc.c"\n')
                    f.write('};\n')
                    f.write('unsigned char gMusicData[] = {\n')
                    f.write('#include "sound/sequences.bin.inc.c"\n')
                    f.write('};\n')
                    f.write('unsigned char gBankSetsData[] = {\n')
                    f.write('#include "sound/bank_sets.inc.c"\n')
                    f.write('};\n')
            if os.path.isfile("Makefile"):
                fix_typo("Makefile",
                    "$(GODDARD_O_FILES) $(LDFLAGS)",
                    "$(GODDARD_O_FILES) -lstdc++ $(LDFLAGS)"
                )

            # Download and apply the DynOS patch
            if state_dynos:
                print("--- Applying DynOS patch...")
                if not is_patched("dynos.patch"):
                    os.system(f"wget --no-check-certificate --no-cache --no-cookies {DYNOS_PATCH_URL} -O dynos.patch -q || rm -f dynos.patch")
                    if os.path.isfile("dynos.patch"):
                        apply_patch("dynos.patch")
                        set_patched("dynos.patch")
                        os.remove("dynos.patch")

            # Apply the selected patches
            if any(state_patches):
                print("--- Applying patches...")
                for i, patch in enumerate(game_patches):
                    if state_patches[i]:
                        patch_name = patch["name"]
                        patch_file = patch["path"]
                        patch_file = patch_file[patch_file.rfind('/') + 1:]
                        patch_path = "../../" + patch["path"]
                        if not is_patched(patch_file) and os.path.isfile(patch_path):
                            print("Applying " + patch_name + "...")
                            apply_patch(patch_path)
                            set_patched(patch_file)

            # Copy OMM contents, apply the Makefile patch and run the patcher
            if not is_patched(OMM_PATCH_TRUENAME):
                print("--- Applying OMM patch...")
                os.system("cp -rf \"../../" + OMM_PATCH_DIRNAME + "/.\" .")

                # Patch Makefile
                if not os.path.isfile("Makefile"):
                    raise_error("Missing Makefile.")
                file = open("Makefile", "r", newline="\n", encoding="utf-8", errors="ignore")
                data = file.read()
                file.close()
                what = "include omm.mk"
                if data.find(what) == -1:
                    where = data.find("BIN_DIRS := bin bin/$(VERSION)")
                    if where == -1:
                        raise_error("Missing string 'BIN_DIRS := bin bin/$(VERSION)' in Makefile")
                    data = data[:where] + what + "\n" + data[where:]
                    file = open("Makefile", "w", newline="\n", encoding="utf-8", errors="ignore")
                    file.write(data)
                    file.close()

                # Run patcher
                omm_patcher_args = "-p " + game_path + (" dynos" if state_dynos else "")
                print("python3 omm_patcher.py " + omm_patcher_args)
                os.system("python3 omm_patcher.py " + omm_patcher_args)
                set_patched(OMM_PATCH_TRUENAME)

            # Copy the baserom and build the game
            print("--- Building game...")
            os.system("cp -f \"../../" + OMM_BUILDER_BASEROM + "\" baserom.us.z64")

            # Make
            make_cmd = "make" + make_speed + " OMM_BUILDER=1" + " VERSION=us" + make_rapi + make_debug

            # OMM make flags
            for arg in sys.argv:
                if "OMM_" in arg and ("=0" in arg or "=1" in arg):
                    make_cmd += " " + arg

            # Let's-a-go
            print(make_cmd)
            make_tries = 5
            make_logs = f"../../{game_path}.logs.txt"
            for make_try in range(make_tries):
                os.system(make_cmd + " 2>&1 | tee " + make_logs)

                # If the executable is built, everything went right
                if get_exe(".") is not None:
                    break

                # It's becoming self-aware...
                print("\nCompilation failed. Trying to understand the cause...")
                if not os.path.isfile(make_logs):
                    raise_error(f"Cannot read logs: Missing file '{game_path}.logs.txt'.")

                # Jk, it's just reading logs
                retry = False
                with open(make_logs, "r", newline="\n", encoding="utf-8", errors="ignore") as f:
                    data = f.readlines()
                    for line in data:

                        # Check texture error
                        if line[line.find("fatal error"):line.find("No such file or directory")].find("rgba16.inc.c") != -1:
                            print("Result: Some assets were not generated in time.")
                            print("Running make again... Remaining tries: " + str(make_tries - 1 - make_try) + "\n")
                            retry = True
                            break

                # Do make again?
                if not retry:
                    break

            # Whoops
            exe = get_exe(".")
            if exe is None:
                raise_error("Something went wrong. Aborting building process...")

            # Install texture packs
            if any(state_textures):
                print("--- Installing Texture Packs...")
                for i, pack in enumerate(game_textures):
                    if state_textures[i]:
                        pack_name = pack["name"]
                        pack_path = pack["path"]
                        pack_dest = pack_name.replace(' ', '_').lower() + ".zip"
                        print("Installing " + pack_name + "...")
                        os.system("( cd \"../../" + pack_path + "\" && zip -qr \"" + pack_dest + "\" * )")
                        os.system("mv -f \"../../" + pack_path + "/" + pack_dest + "\" \"build/us_pc/res/" + pack_dest + "\"")

            # Install sound packs
            if any(state_sounds):
                print("--- Installing Sound Packs...")
                for i, pack in enumerate(game_sounds):
                    if state_sounds[i]:
                        pack_name = pack["name"]
                        pack_path = pack["path"]
                        pack_dest = pack_name.replace(' ', '_').lower() + ".zip"
                        print("Installing " + pack_name + "...")
                        os.system("( cd \"../../" + pack_path + "\" && zip -qr \"" + pack_dest + "\" * )")
                        os.system("mv -f \"../../" + pack_path + "/" + pack_dest + "\" \"build/us_pc/res/" + pack_dest + "\"")

            # Install model packs
            if any(state_models):
                print("--- Installing Model Packs...")
                os.makedirs("build/us_pc/dynos/packs", exist_ok=True)
                for i, pack in enumerate(game_models):
                    if state_models[i]:
                        pack_name = pack["name"]
                        pack_path = pack["path"]
                        print("Installing " + pack_name + "...")
                        os.system("cp -rf \"../../" + pack_path + "\" \"build/us_pc/dynos/packs/" + pack_name + "\"")

            # Install audio packs
            if any(state_audios):
                print("--- Installing Audio Packs...")
                os.makedirs("build/us_pc/dynos/audio", exist_ok=True)
                for i, pack in enumerate(game_audios):
                    if state_audios[i]:
                        pack_name = pack["name"]
                        pack_path = pack["path"]
                        print("Installing " + pack_name + "...")
                        os.system("cp -rf \"../../" + pack_path + "/.\" \"build/us_pc/dynos/audio/\"")

            # Fix permissions
            os.system("chmod 755 -f -R ./build/us_pc/res")
            os.system("chmod 755 -f -R ./build/us_pc/dynos")
            os.system("chmod 755 -f " + exe)

            # Run the game
            if not check_arg(OMM_BUILDER_ARG_BUILD):
                print("--- Starting " + game + "...")
                start_game(".")
            exit(0)

#
# Main
#

if __name__ == "__main__":
    omm_builder_init()
    omm_builder_check_dependencies()
    omm_builder_check_for_updates()
    omm_builder_check_omm_commands()
    omm_builder_check_omm_version()
    omm_builder_extract_zips()
    resources = omm_builder_scan_custom()
    state = omm_builder_gui_init(resources)
    state = omm_builder_gui_update(state)
    omm_builder_process_command(state)
    exit(0)