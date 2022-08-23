# -----------------------
# Odyssey Mario's Moveset
# -----------------------

# ------
# Source
# ------

SRC_DIRS += \
data/omm \
data/omm/system \
data/omm/engine \
data/omm/object \
data/omm/mario \
data/omm/cappy \
data/omm/capture \
data/omm/peachy \
data/omm/level \
data/omm/dev

INC_DIRS := \
-I data/omm/engine/headers \
-I data/omm/engine/headers/include \
-I data/omm/engine/headers/include/PR \
-I data/omm/engine/headers/src \
-I data/omm/engine/headers/src/engine \
-I data/omm/engine/headers/src/extras \
-I data/omm/engine/headers/src/game \
-I data/omm/engine/headers/src/pc

# -------
# Version
# -------

OMM_VERSION_NUMBER := 7.2.0
OMM_VERSION_REVISION := 9
OMM_DEVELOPER := PeachyPeach
OMM_FLAGS := -p
VERSION_CFLAGS += -DOMM_VERSION="$(OMM_VERSION_NUMBER)"
VERSION_CFLAGS += -DOMM_REVISION="$(OMM_VERSION_REVISION)"
VERSION_CFLAGS += -DOMM_DEVELOPER="$(OMM_DEVELOPER)"
DEFINES += OMM_VERSION="$(OMM_VERSION_NUMBER)"
DEFINES += OMM_REVISION="$(OMM_VERSION_REVISION)"
DEFINES += OMM_DEVELOPER="$(OMM_DEVELOPER)"

# Super Mario 64 Moonshine
ifeq      ($(or $(and $(wildcard actors/Bee/geo.inc.c),1),0),1)
  VERSION_CFLAGS += -DSMMS
  DEFINES += SMMS=1
  OMM_FLAGS += smms
  OMM_VERSION_SUB := Super Mario 64 Moonshine

# Super Mario Star Road
else ifeq ($(or $(and $(wildcard levels/zerolife/script.c),1),0),1)
  VERSION_CFLAGS += -DSMSR
  DEFINES += SMSR=1
  OMM_FLAGS += smsr
  OMM_VERSION_SUB := Super Mario Star Road
	
# Super Mario 74
else ifeq ($(or $(and $(wildcard text/us/coursesEE.h),1),0),1)
  VERSION_CFLAGS += -DSM74
  DEFINES += SM74=1
  OMM_FLAGS += sm74
  OMM_VERSION_SUB := Super Mario 74

# Render96ex
else ifeq ($(or $(and $(wildcard data/r96/r96_defines.h),1),0),1)
  VERSION_CFLAGS += -DR96X
  DEFINES += R96X=1
  OMM_FLAGS += r96x
  OMM_VERSION_SUB := Render96

# sm64ex-alo (Refresh 15+)
else ifeq ($(or $(and $(wildcard src/extras/bettercamera.h),1),0),1)
  VERSION_CFLAGS += -DXALO
  DEFINES += XALO=1
  OMM_FLAGS += xalo
  OMM_VERSION_SUB := Super Mario 64 ex-alo

# sm64ex-nightly
else 
  VERSION_CFLAGS += -DSMEX
  DEFINES += SMEX=1
  OMM_FLAGS += smex
  OMM_VERSION_SUB := Super Mario 64 ex-nightly
endif

# --------------
# Initialization
# --------------

OMM_INIT := \
  mkdir -p $(BUILD_DIR)/$(BASEDIR); \
  cp -rf omm/gfx $(BUILD_DIR)/$(BASEDIR) 2>/dev/null || true; \
  cp -rf omm/sound $(BUILD_DIR)/$(BASEDIR) 2>/dev/null || true; \
  echo 'ok'

ifneq ($(shell $(call OMM_INIT)),ok)
  $(error Could not initialize OMM before building. Aborting make..)
endif

# -------
# Defines
# -------

include omm_defines.mk

# -------
# Patches
# -------

ifeq ($(or $(and $(wildcard data/dynos.h),1),0),1)
  VERSION_CFLAGS += -DDYNOS
  DEFINES += DYNOS=1
  OMM_FLAGS += dynos
endif

ifeq ($(or $(and $(wildcard src/game/mario_cheats.h),1),0),1)
  VERSION_CFLAGS += -DCHEATER
  DEFINES += CHEATER=1
endif

ifeq ($(or $(and $(wildcard src/game/time_trials.h),1),0),1)
  VERSION_CFLAGS += -DTIME_TRIALS
  DEFINES += TIME_TRIALS=1
endif

# ---------------
# Builder/Patcher
# ---------------

ifeq ($(OMM_BUILDER),1)
  CC := $(CROSS)gcc -w $(INC_DIRS)
  CXX := $(CROSS)g++ -w $(INC_DIRS)
else
  ifneq ($(OMM_DEV),1)
    OMM_PATCH := $(shell python3 omm_patcher.py $(OMM_FLAGS))
  endif
  CC := $(CROSS)gcc $(INC_DIRS)
  CXX := $(CROSS)g++ $(INC_DIRS)
endif

# ---------
# OMM rules
# ---------

all: OMM_VERSION OMM_EXE_MAP

OMM_VERSION:
	@printf -- \
	"--------------------------------------------------------------------------------------------------------------------------------\n\
	Odyssey Mario's Moveset v$(OMM_VERSION_NUMBER) by $(OMM_DEVELOPER) - $(OMM_VERSION_SUB) detected\n\
	--------------------------------------------------------------------------------------------------------------------------------\n"

OMM_EXE_MAP: $(EXE)
	@objdump -t $(EXE) > $(BUILD_DIR)/$(BASEDIR)/omm.map