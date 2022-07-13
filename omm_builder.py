import os
import sys
import stat
import shutil
import filecmp
from distutils.dir_util import copy_tree
from xml.etree.ElementTree import VERSION

def usage_info():
    print("")
    print("================ OMM Builder Python script ================")
    print("The easiest way to build Odyssey Mario's Moveset")
    print("")
    print("Usage:")
    print("")
    print("python3 omm_builder.py [version] [build_speed] [args...] | Compile the game with the version specified.")
    print("python3 omm_builder.py [version] run                     | Run the game with the version specified.")
    print("python3 omm_builder.py [version] clear                   | Clear the build directory of the selected version.")
    print("python3 omm_builder.py [version] reset                   | Reset the version directory without deleting it.")
    print("python3 omm_builder.py [version] delete                  | Delete the version directory.")
    print("")
    print("[version] must be one of the following:")
    print("  smex  | Super Mario 64 ex-nightly")
    print("  smms  | Super Mario 64 Moonshine")
    print("  xalo  | Super Mario 64 ex-alo")
    print("  sm74  | Super Mario 74")
    print("  smsr  | Super Mario Star Road")
    print("  r96a  | Render96 ex-alpha")
  # print("  rt64  | Render96 RT64")
  # print("  saex  | Saturn (sm64ex-nightly)")
    print("")
    print("[build_speed] must be one of the following:")
    print("  slow     | Build the game by compiling files one by one.")
    print("  fast     | Enable multi-processing to build the game faster.")
    print("  faster   | Build the game even faster (not recommended for weak PCs).")
    print("  fastest  | Use 100% of CPU to build the game as fast as possible.")
    print("")
    print("[args...] can be any of the following:")
    print("  DYNOS     | Download and install the latest version of DynOS.")
    print("  PATCHES   | Apply patches from the 'custom/patches' directory.")
    print("  DIRECT_X  | Replace SDL/OpenGL APIs by DirectX APIs.")
    print("  AUTO_RUN  | Start the game after building.")
    print("")
    print("Requirements:")
    print("- To build Super Mario 64 Moonshine, you must extract the mod archive ('MOONSHINE_FINALUPDATE.rar') into a directory named 'moonshine'.")
  # print("- To run Render96 RT64, an NVIDIA GPU with DXR support (RTX or GTX > 1060) is required.")
    print("")
    print("Custom patches:")
    print("- To build the game with custom patches, place a copy of your '.patch' files inside the 'custom/patches' directory and run the command with the 'PATCHES' option.")
    print("- Not all patches or combination of patches are supported.")
    print("")
    print("Texture and sound packs:")
    print("- Customize your game's textures and sounds by placing your packs '.zip' archives inside the 'custom/res' directory.")
    print("- Texture packs must be '.zip' archives with a 'gfx' directory inside them.")
    print("- Sound packs must be '.zip' archives with a 'sound' directory inside them.")
    print("")
    print("Model packs (DynOS and Render96 only):")
    print("- To be able to swap actors models in-game, copy your model packs directories inside the 'custom/dynos/packs' directory and run the command with the 'DYNOS' option.")
    print("- Model packs must be either directories of '.bin' files or filled with actors sub-directories, each sub-directory containing at least one 'model.inc.c' and one 'geo.inc.c' file as well as textures '.png' files.")
    print("")
    print("Musics, jingles and sounds (Render96 only):")
    print("- Make your game more unique with custom musics, jingles and sounds! Put your audio data inside a 'custom/dynos/audio' directory before running the command.")
    print("- Render96 audio packs must be directories of sub-directories and '.txt' files, with each '.txt' file corresponding to one sub-directory.")
    print("- The sub-directories must be: 'jingles', 'levels', 'sfx_mario', 'sfx_mario_peach', 'sfx_luigi', 'sfx_luigi_peach', 'sfx_wario', 'sfx_wario_peach'.")
    print("- The associated '.txt' files are: 'jingle.txt', 'music.txt', 'sfx_mario.txt', 'sfx_mario_peach.txt', 'sfx_luigi.txt', 'sfx_luigi_peach.txt', 'sfx_wario.txt', 'sfx_wario_peach.txt'.")
    print("")
    sys.exit(0)

def raise_error(error, usageInfo):
    print("ERROR: " + error)
    if usageInfo: usage_info()
    sys.exit(0)

def onerror(func, path, exc_info):
    if not os.access(path, os.W_OK):
        os.chmod(path, stat.S_IWUSR)
        func(path)
    else:
        raise

def rm_rf(dir):
    if len(dir) > 0 and os.path.isdir(dir):
        shutil.rmtree(dir, onerror=onerror)
        return True
    return False

def vn2i(vn):
    i = 0
    for s in vn.split('.'):
        i = i * 100 + int(s)
    return i

def get_omm_patch_dir():
    for path in os.listdir("."):
        if os.path.isdir(path) and "omm." in path:
            return path
    return ""

def get_omm_version(filepath):
    version = {
        "OMM_VERSION_NUMBER": "Unknown",
        "OMM_VERSION_REVISION": "0",
    }
    if os.path.isfile(filepath):
        with open(filepath, "r") as file:
            data = file.read()
            for k in version.keys():
                i0 = data.find(k + " :=")
                i1 = data.find("\n", i0)
                if i0 != -1 and i1 != -1:
                    version[k] = data[i0 + len(k + " :="):i1].lstrip(' \t\r\n').rstrip(' \t\r\n')
    return version

def create_omm_patch_file():
    path = get_omm_patch_dir()
    if len(path) > 0:
        os.system("git clone https://github.com/sm64pc/sm64ex.git -b nightly temp -q")
        if os.path.isdir("temp"):
            os.chdir("temp")
            copy_tree("../" + path, ".")
            if os.path.isfile("Makefile"):
                file = open("Makefile", "r", newline="\n", encoding="utf-8", errors="ignore")
                data = file.read()
                file.close()
                where = data.find("BIN_DIRS := bin bin/$(VERSION)")
                if where != -1:
                    data = data[:where] + "include omm.mk\n" + data[where:]
                    file = open("Makefile", "w", newline="\n", encoding="utf-8", errors="ignore")
                    file.write(data)
                    file.close()
                    os.system("git add .")
                    os.system("git diff --diff-algorithm=minimal --unified=2 -p --staged --binary > ../omm.patch")
            os.chdir("..")
            rm_rf("temp")

def rm_omm_patches():
    while rm_rf(get_omm_patch_dir()):
        continue

def rm_files(root, condition):
    for path, _, files in os.walk(root):
        for filename in files:
            filepath = path + "/" + filename
            if os.path.isfile(filepath) and condition(root, filepath):
                os.remove(filepath)

def check_patched(name):
    return os.path.isfile("enhancements/" + name + ".patched")

def set_patched(name):
    with open("enhancements/" + name + ".patched", 'w') as f: f.write("")

def apply_patch(filepath):
    if not os.path.isfile(filepath):
        raise_error(f"The patch file '{filepath}' is missing.", False)
    os.system(f"git apply --reject --whitespace=nowarn {filepath}")

def fix_typo(filepath, strFrom, strTo):
    if os.path.isfile(filepath):
        file = open(filepath, "r", newline="\n", encoding="utf-8", errors="ignore")
        data = file.read()
        file.close()
        edit = False
        done = False
        iloc = 0
        while not done:
            iloc = data.find(strFrom, iloc)
            if iloc != -1:
                data = data[:iloc] + strTo + data[iloc + len(strFrom):]
                iloc += len(strTo)
                edit = True
            else:
                done = True
        if edit:
            file = open(filepath, "w", newline="\n", encoding="utf-8", errors="ignore")
            file.write(data)
            file.close()

def get_exe(dir):
    for path, _, files in os.walk(f"{dir}/build/us_pc"):
        for filename in files:
            if ".exe" in filename or ".f3dex2e" in filename:
                return path + "/" + filename
    return None

def check_executable(dir):
    return get_exe(dir) is not None

def start_game(dir):
    exe = get_exe(dir)
    if exe is None:
        raise_error("Unable to locate the game executable.", False)
    os.system(f"start {exe}")



if __name__ == "__main__":

    # Constants
    VERSIONS = {
        "smex": { "name": "Super Mario 64 ex-nightly", "repo": "https://github.com/sm64pc/sm64ex.git -b nightly",                "dependency": "",          "args": [] },
        "smms": { "name": "Super Mario 64 Moonshine",  "repo": "https://github.com/sm64pc/sm64ex.git -b nightly",                "dependency": "moonshine", "args": [] },
        "xalo": { "name": "Super Mario 64 ex-alo",     "repo": "https://github.com/AloXado320/sm64ex-alo.git -b master",         "dependency": "",          "args": [] },
        "sm74": { "name": "Super Mario 74",            "repo": "https://github.com/PeachyPeachSM64/sm64pc-omm.git -b sm74",      "dependency": "",          "args": [] },
        "smsr": { "name": "Super Mario Star Road",     "repo": "https://github.com/PeachyPeachSM64/sm64pc-omm.git -b smsr",      "dependency": "",          "args": [] },
        "r96a": { "name": "Render96 ex-alpha",         "repo": "https://github.com/Render96/Render96ex.git -b tester_rt64alpha", "dependency": "",          "args": ["DYNOS"] },
      # "rt64": { "name": "Render96 RT64",             "repo": "https://github.com/Render96/Render96ex.git -b tester_rt64alpha", "dependency": "",          "args": ["DYNOS"] },
      # "saex": { "name": "Saturn (sm64ex-nightly)",   "repo": "https://github.com/Llennpie/Saturn -b legacy",                   "dependency": "",          "args": ["DYNOS"] },
    }
    BUILD_SPEEDS = {
        "slow"   : { "name": "Slow",    "jobs": "" },
        "fast"   : { "name": "Fast",    "jobs": " -j6" },
        "faster" : { "name": "Faster",  "jobs": " -j12" },
        "fastest": { "name": "Fastest", "jobs": " -j" },
        "run"    : {},
        "clear"  : {},
        "reset"  : {},
        "delete" : {},
    }
    ARGUMENTS = {
        "DEBUG"    : ["debug"],
        "DYNOS"    : ["dynos"],
        "PATCHES"  : ["patches"],
        "DIRECT_X" : ["direct_x", "directx", "dx"],
        "AUTO_RUN" : ["auto_run", "autorun"],
        "NO_UPDATE": ["no_update", "noupdate"],
    }
    DIRECTORIES = {
        "repos"      : "repos",
        "root"       : "../../",
        "dependency" : "../../",
        "patches"    : "../../custom/patches",
        "resources"  : "../../custom/res",
        "dynos_packs": "../../custom/dynos/packs",
        "dynos_audio": "../../custom/dynos/audio",
    }

    # Check builder
    os.system("wget --no-check-certificate --no-cache --no-cookies https://raw.githubusercontent.com/PeachyPeachSM64/sm64pc-omm/master/omm_builder.py -O omm_builder.version -q || rm -f omm_builder.version")
    if os.path.isfile("omm_builder.version"):
        filecmp.clear_cache()
        isUpToDate = filecmp.cmp("omm_builder.py", "omm_builder.version", shallow=False)
        os.remove("omm_builder.version")
        if not isUpToDate:
            print("Your OMM builder is not up-to-date.")
            print("Do you want to download and install the latest version? [y/n] ", end='')
            answer = ""
            while not answer in ["y", "n"]:
                answer = input().lower()
            if answer == "y":
                print("Updating OMM builder...")
                os.system("git checkout . -q")
                os.system("git pull -q")
                print("Done.")
                sys.exit(0)

    # OMM patch commands
    if len(sys.argv) >= 2:
        command = sys.argv[1].lower()

        # Create a git diff patch file
        if command == "patch":
            print("--- Creating OMM patch file...")
            create_omm_patch_file()
            print("Done.")
            sys.exit(0)

        # Delete omm.* directories
        if command == "delete":
            print("--- Deleting OMM patches...")
            rm_omm_patches()
            print("Done.")
            sys.exit(0)

    # Usage info
    if len(sys.argv) < 3:
        usage_info()

    # Version
    version = sys.argv[1].lower()
    if not version in VERSIONS.keys():
        raise_error(f"'{version}' is not a valid OMM version.", True)
    versionDir = DIRECTORIES["repos"] + "/" + version

    # Build speed
    speed = sys.argv[2].lower()
    if not speed in BUILD_SPEEDS.keys():
        raise_error(f"'{speed}' is not a valid build speed.", True)

    # Arguments
    args = {}
    for k, v in ARGUMENTS.items():
        args[k] = any([(arg.lower() in v) for arg in sys.argv])
    for arg in VERSIONS[version]["args"]:
        args[arg] = True

    # Run the game
    if speed == "run":
        print("--- Starting game...")
        start_game(versionDir)
        sys.exit(0)

    # Clear build directory
    if speed == "clear":
        print("--- Clearing " + version + " build directory...")
        rm_rf(versionDir + "/build")
        print("Done.")
        sys.exit(0)

    # Reset target directory
    if speed == "reset":
        print("--- Resetting target repository...")
        if os.path.isdir(versionDir):
            os.chdir(versionDir)
            os.system("git reset -q --hard")
            os.system("git clean -f -d -x -q")
            os.system("git checkout . -q")
            os.system("git pull -q")
        print("Done.")
        sys.exit(0)

    # Delete target directory
    if speed == "delete":
        print("--- Deleting target repository...")
        rm_rf(versionDir)
        print("Done.")
        sys.exit(0)

    # Retrieve OMM version number
    versionLocal = get_omm_version(get_omm_patch_dir() + "/omm.mk")
    if args["NO_UPDATE"]:
        print("NO_UPDATE specified. Using local OMM patch...")
        OMM_VERSION_NUMBER = versionLocal["OMM_VERSION_NUMBER"]
        OMM_VERSION_REVISION = versionLocal["OMM_VERSION_REVISION"]
        OMM_PATCH_DIR = get_omm_patch_dir()
        if OMM_PATCH_DIR == "":
            raise_error("Cannot find any OMM patch.", False)
    else:
        os.system("wget --no-check-certificate --no-cache --no-cookies https://raw.githubusercontent.com/PeachyPeachSM64/sm64pc-omm/nightly/omm.mk -O omm.version -q || rm -f omm.version")
        if os.path.isfile("omm.version"):
            versionRemote = get_omm_version("omm.version")
            if versionRemote["OMM_VERSION_NUMBER"] == "":
                raise_error("Cannot retrieve remote OMM version number.", False)
            OMM_VERSION_NUMBER = versionRemote["OMM_VERSION_NUMBER"]
            OMM_VERSION_REVISION = versionRemote["OMM_VERSION_REVISION"]
            OMM_PATCH_DIR = "omm." + OMM_VERSION_NUMBER + "." + OMM_VERSION_REVISION
            os.remove("omm.version")
        else:
            print("Unable to retrieve remote OMM version number.")
            print("Using local OMM patch...")
            OMM_VERSION_NUMBER = versionLocal["OMM_VERSION_NUMBER"]
            OMM_VERSION_REVISION = versionLocal["OMM_VERSION_REVISION"]
            OMM_PATCH_DIR = get_omm_patch_dir()
            if OMM_PATCH_DIR == "":
                raise_error("Cannot find any OMM patch.", False)
    OMM_PATCH_NAME = "omm." + OMM_VERSION_NUMBER + "." + OMM_VERSION_REVISION

    # Print some info before starting
    print("")
    print("Ver. number : {} (revision {})".format(OMM_VERSION_NUMBER, OMM_VERSION_REVISION))
    print("Version     : {}".format(VERSIONS[version]["name"]))
    print("Repository  : {}".format(VERSIONS[version]["repo"]))
    print("Dependency  : {}".format(VERSIONS[version]["dependency"]))
    print("Build speed : {}".format(BUILD_SPEEDS[speed]["name"]))
    print("Arguments   : {}".format(" ".join([k for k, v in args.items() if v])))
    print("")

    # Check compatibility
    print("--- Checking compatibility...")
    if version in ["saex"] and vn2i(OMM_VERSION_NUMBER) < vn2i("7.0.9"):
        raise_error("{} ({}) can be built only with OMM v7.0.9 and later.".format(VERSIONS[version]["name"], version), False)
    if version in ["xalo", "smsr", "r96a", "rt64"] and vn2i(OMM_VERSION_NUMBER) < vn2i("6.1.0"):
        raise_error("{} ({}) can be built only with OMM v6.1.0 and later.".format(VERSIONS[version]["name"], version), False)
    if version in ["xalo", "sm74", "smsr"] and args["DYNOS"]:
        raise_error("{} ({}) cannot be built with DynOS.".format(VERSIONS[version]["name"], version), False)

    # Check baserom
    print("--- Checking baserom...")
    if not os.path.isfile("baserom.us.z64"):
        raise_error("The file 'baserom.us.z64' is missing.", False)

    # Check OMM patch: Create it or update it to the latest version
    print("--- Checking OMM patch (1/3)...")
    upToDate = (versionLocal["OMM_VERSION_NUMBER"] == OMM_VERSION_NUMBER) and (versionLocal["OMM_VERSION_REVISION"] == OMM_VERSION_REVISION)
    if not upToDate or not os.path.isdir(OMM_PATCH_DIR):

        # Delete all omm.* directories
        print("Removing old OMM patches...")
        rm_omm_patches()

        # Clone the OMM repository
        OMM_PATCH_DIR = OMM_PATCH_NAME
        print("Creating " + OMM_PATCH_DIR + " from the latest version...")
        ommRepository = "https://github.com/PeachyPeachSM64/sm64pc-omm.git -b nightly"
        os.system("git clone " + ommRepository + " " + OMM_PATCH_DIR)
        if not os.path.isdir(OMM_PATCH_DIR):
            raise_error("Cannot clone the git repository: " + ommRepository, False)

    # Check OMM patch: Delete the .git, .github and .vscode directories and keep only the files that contain "omm/", "omm_" or "omm."
    print("--- Checking OMM patch (2/3)...")
    rm_rf(OMM_PATCH_DIR + "/.git")
    rm_rf(OMM_PATCH_DIR + "/.github")
    rm_rf(OMM_PATCH_DIR + "/.vscode")
    is_not_omm_file = lambda root, filepath : not any([ pattern in filepath[len(root):] for pattern in ["omm/", "omm_", "omm."] ])
    rm_files(OMM_PATCH_DIR, is_not_omm_file)

    # Clone the target repository
    print("--- Cloning target repository...")
    if not os.path.isdir(versionDir):
        os.system("git clone {} {}".format(VERSIONS[version]["repo"], versionDir))
        if not os.path.isdir(versionDir):
            raise_error("Cannot clone the git repository: " + VERSIONS[version]["repo"], False)
    os.chdir(versionDir)

    # Check OMM patch: if a omm.*.patched file exists but is not the latest version, reset the directory
    print("--- Checking OMM patch (3/3)...")
    if not upToDate or not check_patched(OMM_PATCH_NAME):
        for path in os.listdir("enhancements"):
            if os.path.isfile("enhancements/" + path) and "omm." in path and ".patched" in path:
                print("Old version of OMM detected!")
                print("Resetting " + version + "...")
                os.system("git reset -q --hard")
                os.system("git clean -f -d -x -q")
                os.system("git checkout . -q")
                os.system("git pull -q")
                break

    # Apply the mod patchfile or copy its contents
    dependency = VERSIONS[version]["dependency"]
    if len(dependency) != 0 and not check_patched(dependency):
        if ".patch" in dependency:
            print("--- Applying dependency patch...")
            if not os.path.isfile(DIRECTORIES["dependency"] + dependency):
                raise_error(f"The patch file '{dependency}' is missing.", False)
            apply_patch(DIRECTORIES["dependency"] + dependency)
        else:
            print("--- Copying dependency contents...")
            if not os.path.isdir(DIRECTORIES["dependency"] + dependency):
                raise_error(f"The directory '{dependency}' is missing.", False)
            copy_tree(DIRECTORIES["dependency"] + dependency, ".")
        set_patched(dependency)

    # Delete shitty files
    print("--- Eliminating bad files...")
    is_bad_file = lambda filepath : ".png.png" in filepath
    for path, _, files in os.walk("."):
        for filename in files:
            filepath = path + "/" + filename
            if os.path.isfile(filepath) and is_bad_file(filepath):
                os.remove(filepath)

    # Fix typos
    print("--- Fixing typos...")
    if os.path.isfile("sound/sound_data.s"):
        fix_typo("Makefile",
            "$(SOUND_BIN_DIR)/sound_data.o: $(SOUND_BIN_DIR)/sound_data.ctl $(SOUND_BIN_DIR)/sound_data.tbl $(SOUND_BIN_DIR)/sequences.bin $(SOUND_BIN_DIR)/bank_sets",
            "$(SOUND_BIN_DIR)/sound_data.o: $(SOUND_BIN_DIR)/sound_data.ctl.inc.c $(SOUND_BIN_DIR)/sound_data.tbl.inc.c $(SOUND_BIN_DIR)/sequences.bin.inc.c $(SOUND_BIN_DIR)/bank_sets.inc.c"
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
    if args["DYNOS"] and not version in ["r96a", "rt64", "saex"]:
        print("--- Applying DynOS patch...")
        if not check_patched("dynos.patch"):
            os.system("wget --no-check-certificate --no-cache --no-cookies https://sm64pc.info/downloads/patches/DynOS.1.1.patch -O dynos.patch -q || rm -f dynos.patch")
            if os.path.isfile("dynos.patch"):
                apply_patch("dynos.patch")
                set_patched("dynos.patch")
                os.remove("dynos.patch")

    # Apply the patches from the patches directory
    if args["PATCHES"]:
        print("--- Applying patches...")
        is_patch_file = lambda filepath : ".patch" in filepath
        if os.path.isdir(DIRECTORIES["patches"]):
            for path, _, files in os.walk(DIRECTORIES["patches"]):
                for filename in files:
                    if not check_patched(filename):
                        filepath = path + "/" + filename
                        if os.path.isfile(filepath) and is_patch_file(filepath):
                            apply_patch(filepath)
                            shutil.copyfile(filepath, "enhancements/" + filename)
                            set_patched(filename)

    # Copy OMM contents, apply the Makefile patch and run the patcher
    print("--- Applying OMM patch...")
    if not check_patched(OMM_PATCH_NAME):

        # Copy patch contents
        copy_tree(DIRECTORIES["root"] + OMM_PATCH_DIR, ".")

        # Patch Makefile
        if not os.path.isfile("Makefile"):
            raise_error("Missing Makefile.", False)
        file = open("Makefile", "r", newline="\n", encoding="utf-8", errors="ignore")
        data = file.read()
        file.close()
        what = "include omm.mk"
        if data.find(what) == -1:
            where = data.find("BIN_DIRS := bin bin/$(VERSION)")
            if where == -1:
                raise_error("Missing string 'BIN_DIRS := bin bin/$(VERSION)' in Makefile", False)
            data = data[:where] + what + "\n" + data[where:]
            file = open("Makefile", "w", newline="\n", encoding="utf-8", errors="ignore")
            file.write(data)
            file.close()

        # Run patcher
        ommPatcherArgs = "-p " + version + (" dynos" if args["DYNOS"] else "")
        print("python3 omm_patcher.py " + ommPatcherArgs)
        os.system("python3 omm_patcher.py " + ommPatcherArgs)
        set_patched(OMM_PATCH_NAME)

    # Copy the baserom and build the game
    print("--- Building game...")
    if not os.path.isfile("baserom.us.z64"):
        shutil.copyfile(DIRECTORIES["root"] + "baserom.us.z64", "baserom.us.z64")

    # Make
    makeCmd = "make" + BUILD_SPEEDS[speed]["jobs"] + " OMM_BUILDER=1" + " VERSION=us"

    # APIs
    if version in ["rt64"]: makeCmd += " RENDER_API=RT64 WINDOW_API=SDL2 AUDIO_API=SDL2 CONTROLLER_API=SDL2 GRUCODE=f3dex2e"
    elif args["DIRECT_X"]:  makeCmd += " RENDER_API=D3D11 WINDOW_API=DXGI AUDIO_API=SDL2 CONTROLLER_API=SDL2 GRUCODE=f3dex2e"
    else:                   makeCmd += " RENDER_API=GL WINDOW_API=SDL2 AUDIO_API=SDL2 CONTROLLER_API=SDL2 GRUCODE=f3dex2e"

    # Debug
    makeCmd += " DEBUG=1" if args["DEBUG"] else ""

    # OMM flags
    for arg in sys.argv:
        if "OMM_" in arg and ("=0" in arg or "=1" in arg):
            makeCmd += " " + arg

    # Let's-a-go
    print(makeCmd)
    makeNumTries = 5
    for tries in range(makeNumTries):
        os.system(makeCmd + " 2>&1 | tee {}logs.txt".format(DIRECTORIES["root"]))

        # If the executable is built, everything went right
        if check_executable("."):
            break

        # It's becoming self-aware...
        print("\nCompilation failed. Trying to understand the cause...")
        if not os.path.isfile(DIRECTORIES["root"] + "logs.txt"):
            raise_error("Cannot read logs: Missing file 'logs.txt'.", False)

        # Jk, it's just reading logs
        file = open(DIRECTORIES["root"] + "logs.txt", "r", newline="\n", encoding="utf-8", errors="ignore")
        data = file.readlines()
        file.close()
        retry = False
        for line in data:

            # Check dependencies
            if (line.find("make: No such file or directory") != -1 or
                line.find("git: No such file or directory") != -1 or
                line.find("gcc: No such file or directory") != -1 or
                line.find("cc: No such file or directory") != -1 or
                line.find("CalledProcessError") != -1):
                raise_error("Missing dependencies. To fix this problem, please do the following:\n- For Windows users, make sure you're using MINGW32 or MINGW64, not MSYS.\n- Install or re-install the dependencies by running sm64pcBuilder2 on Windows or follow the instructions on https://github.com/sm64pc/sm64ex/wiki/Compiling-on-Linux for other systems.", False)

            # Check texture error
            if line[line.find("fatal error"):line.find("No such file or directory")].find("rgba16.inc.c") != -1:
                print("Result: Some assets were not generated in time.")
                print("Running make again... Remaining tries: " + str(makeNumTries - 1 - tries) + "\n")
                retry = True
                break

        # Do make again?
        if not retry:
            break

    # Whoops
    if not check_executable("."):
        raise_error("Something went wrong. Aborting building process...", False)

    # Copy external resources to the build res directory
    if os.path.isdir(DIRECTORIES["resources"]) and os.path.isdir("build/us_pc/res"):
        print("--- Installing external resources...")
        copy_tree(DIRECTORIES["resources"], "build/us_pc/res")

    # Copy DynOS model packs
    if args["DYNOS"] and os.path.isdir(DIRECTORIES["dynos_packs"]) and os.path.isdir("build/us_pc/dynos"):
        print("--- Installing DynOS model packs...")
        os.makedirs("build/us_pc/dynos/packs", exist_ok=True)
        copy_tree(DIRECTORIES["dynos_packs"], "build/us_pc/dynos/packs")

    # Copy Render96 custom audio data
    if version in ["r96a", "rt64"] and os.path.isdir(DIRECTORIES["dynos_audio"]) and os.path.isdir("build/us_pc/dynos"):
        print("--- Installing Render96 audio data...")
        os.makedirs("build/us_pc/dynos/audio", exist_ok=True)
        copy_tree(DIRECTORIES["dynos_audio"], "build/us_pc/dynos/audio")

    # Run the game if AUTO_RUN is set
    if args["AUTO_RUN"]:
        print("--- Starting game...")
        start_game(".")
    else:
        print("Done.")

    # The script ends here
    sys.exit(0)
