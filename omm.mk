# -----------------------
# Odyssey Mario's Moveset
# -----------------------

OMM_VERSION_NUMBER := 8.1.1
OMM_DEVELOPER := PeachyPeach

# ------
# Source
# ------

SRC_DIRS += \
data/omm \
data/omm/system \
data/omm/engine \
data/omm/engine/miniaudio \
data/omm/object \
data/omm/models \
data/omm/mario \
data/omm/cappy \
data/omm/capture \
data/omm/peachy \
data/omm/level \
data/omm/time_trials \
data/omm/dev

INC_DIRS := \
-I data/omm/engine/headers \
-I data/omm/engine/headers/actors \
-I data/omm/engine/headers/include \
-I data/omm/engine/headers/include/PR \
-I data/omm/engine/headers/src \
-I data/omm/engine/headers/src/engine \
-I data/omm/engine/headers/src/extras \
-I data/omm/engine/headers/src/game \
-I data/omm/engine/headers/src/menu \
-I data/omm/engine/headers/src/pc

# ----
# Game
# ----

# Super Mario 64 Moonshine
ifeq ($(or $(and $(wildcard actors/Bee/geo.inc.c),1),0),1)
  OMM_GAME_NAME := Super Mario 64 Moonshine
  OMM_GAME_CODE := smms

# Super Mario Star Road
else ifeq ($(or $(and $(wildcard levels/zerolife/script.c),1),0),1)
  OMM_GAME_NAME := Super Mario Star Road
  OMM_GAME_CODE := smsr

# Super Mario 64: The Green Stars
else ifeq ($(or $(and $(wildcard data/smgs.data.custom.h),1),0),1)
  OMM_GAME_NAME := Super Mario 64: The Green Stars
  OMM_GAME_CODE := smgs

# Super Mario 74
else ifeq ($(or $(and $(wildcard text/us/coursesEE.h),1),0),1)
  OMM_GAME_NAME := Super Mario 74
  OMM_GAME_CODE := sm74

# Render96ex
else ifeq ($(or $(and $(wildcard data/r96/r96_defines.h),1),0),1)
  OMM_GAME_NAME := Render96
  OMM_GAME_CODE := r96x

# sm64ex-nightly
else
  OMM_GAME_NAME := Super Mario 64
  OMM_GAME_CODE := smex
endif

TARGET := sm64.$(OMM_GAME_CODE)
ifeq ($(WINDOWS_BUILD),1)
  EXE := $(BUILD_DIR)/$(TARGET).exe
else
  EXE := $(BUILD_DIR)/$(TARGET)
endif

BASEPACK := base.$(OMM_GAME_CODE).zip
OMM_GAME_BASEPACK_LST := $(BUILD_DIR)/$(BASEDIR)/basepack.lst
OMM_GAME_BASEPACK_ZIP := $(BUILD_DIR)/$(BASEDIR)/$(BASEPACK)

VERSION_CFLAGS += \
  -DOMM_VERSION="\"$(OMM_VERSION_NUMBER)\"" \
  -DOMM_DEVELOPER="\"$(OMM_DEVELOPER)\"" \
  -DOMM_GAME_NAME="\"$(OMM_GAME_NAME)\"" \
  -DOMM_GAME_CODE="\"$(OMM_GAME_CODE)\"" \
  -D$(OMM_GAME_CODE) \
  -DDYNOS_INL
CUSTOM_C_DEFINES += \
  -DOMM_VERSION="\"$(OMM_VERSION_NUMBER)\"" \
  -DOMM_DEVELOPER="\"$(OMM_DEVELOPER)\"" \
  -DOMM_GAME_NAME="\"$(OMM_GAME_NAME)\"" \
  -DOMM_GAME_CODE="\"$(OMM_GAME_CODE)\"" \
  -D$(OMM_GAME_CODE) \
  -DDYNOS_INL
BITS += -lz -lstdc++

# -------
# Defines
# -------

include omm_defines.mk

# -------
# Patches
# -------

ifeq ($(or $(and $(wildcard data/dynos.h),1),0),1)
  VERSION_CFLAGS += -DDYNOS
  CUSTOM_C_DEFINES += -DDYNOS
endif

ifeq ($(or $(and $(wildcard src/game/mario_cheats.h),1),0),1)
  VERSION_CFLAGS += -DCHEATER
  CUSTOM_C_DEFINES += -DCHEATER
endif

ifeq ($(or $(and $(wildcard src/game/time_trials.h),1),0),1)
  VERSION_CFLAGS += -DTIME_TRIALS
  CUSTOM_C_DEFINES += -DTIME_TRIALS
endif

# ------
# Builds
# ------

ifneq ($(OMM_DEV),1)
  OMM_PATCH != python3 omm_patcher.py -p
endif

ifeq ($(DEBUG),1)
  CC := $(CROSS)gcc $(INC_DIRS)
  CXX := $(CROSS)g++ $(INC_DIRS)
  ifeq ($(WERROR),1)
    CC += -Werror
    CXX += -Werror
  endif
else
  CC := $(CROSS)gcc -w $(INC_DIRS)
  CXX := $(CROSS)g++ -w $(INC_DIRS)
endif

# ---------
# OMM rules
# ---------

.PHONY: OMM_VERSION OMM_FIX_BANK_SETS_SIZE OMM_BASEPACK_ZIP OMM_BASEPACK_LST

all: OMM_VERSION OMM_FIX_BANK_SETS_SIZE OMM_BASEPACK_ZIP

basepack: OMM_BASEPACK_LST

OMM_VERSION:
	@printf -- "\
	                                                          _                                                 \n\
	   ____     _                          __    __          |_|     _        __    __                     _    \n\
	  / __ \  _| |_  _______________  __   | \  / |____  _ __ _  __ |/ ____   | \  / | ____  _____ _______| |__ \n\
	 / /  \ \/   \ \/ / __/ __/ _ \ \/ /   |  \/  |\__ \| '_/| |/  \  / __/   |  \/  |/  \ \/ / _ \ __/ _ \  _/ \n\
	 \ \__/ / (] |\  /\__ \__ \ __/\  /    | \  / |/ _  | |  | | () | \__ \   | \  / | () \  /  __/__ \ __/ |__ \n\
	  \____/ \___|/ / /___/___/___// /     |_|\/|_|\___/|_|  |_|\__/  /___/   |_|\/|_|\__/ \/ \___/___/___/___/ \n\
	             /_/              /_/                                                                           \n\
	\n\
	            +----------------------------------------------------------------------------------+\n\
	            | %-80s |\n\
	            +----------------------------------------------------------------------------------+\n\n" \
	"Odyssey Mario's Moveset v$(OMM_VERSION_NUMBER) by $(OMM_DEVELOPER) - $(OMM_GAME_NAME)"

ifneq ($(or $(and $(wildcard $(OMM_GAME_BASEPACK_LST)),1),0),1)

$(OMM_GAME_BASEPACK_LST): $(EXE)
	@echo "Preparing basepack..."
	@mkdir -p $(BUILD_DIR)/$(BASEDIR)
	@chmod 777 $(BUILD_DIR)/$(BASEDIR)
	@echo "$(BUILD_DIR)/sound/bank_sets sound/bank_sets" > $@
	@echo "$(BUILD_DIR)/sound/sequences.bin sound/sequences.bin" >> $@
	@echo "$(BUILD_DIR)/sound/sound_data.ctl sound/sound_data.ctl" >> $@
	@echo "$(BUILD_DIR)/sound/sound_data.tbl sound/sound_data.tbl" >> $@
	@find actors -name \*.png -exec echo "{} gfx/{}" >> $@ \;
	@find levels -name \*.png -exec echo "{} gfx/{}" >> $@ \;
	@find textures -name \*.png -not -path "textures/skybox.custom/*" -not -path "textures/skybox_tiles/*" -exec echo "{} gfx/{}" >> $@ \;

endif

OMM_BASEPACK_ZIP: $(EXE) $(OMM_GAME_BASEPACK_LST)
	@cp -f baserom.us.z64 $(BUILD_DIR)/baserom.us.z64
	@objdump -t $(EXE) > $(BUILD_DIR)/$(BASEDIR)/omm.$(OMM_GAME_CODE).map
	@python3 -u omm_map.py $(BUILD_DIR)/$(BASEDIR)/omm.$(OMM_GAME_CODE).map
	@python3 -u omm_mkzip.py $(OMM_GAME_BASEPACK_LST) $(OMM_GAME_BASEPACK_ZIP) omm/basepack.lst $(BUILD_DIR)/$(BASEDIR)/omm.zip
ifneq ($(DEBUG),1)
	@rm -f $(OMM_GAME_BASEPACK_LST)
	@ls $(BUILD_DIR) | grep -vE "^res$$|^sm64|^dynos$$|^baserom|\.dll$$" | awk '{print "$(BUILD_DIR)/"$$1}' | xargs rm -rf
	@objcopy -p --strip-unneeded $(EXE)
endif

OMM_BASEPACK_LST:
	@rm -f omm/basepack.lst
	@find omm -name \*.\* -exec bash -c "echo \"{} {}\" | sed 's| omm/| |' -" > basepack.lst \;
	@mv -f basepack.lst omm/basepack.lst

OMM_FIX_BANK_SETS_SIZE: $(BUILD_DIR)/sound/bank_sets
	@truncate --size='>256' $(BUILD_DIR)/sound/bank_sets
