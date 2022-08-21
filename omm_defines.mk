# -------
# Enabled
# -------

CUSTOM_C_DEFINES :=

# No segmented memory
CUSTOM_C_DEFINES += -DNO_SEGMENTED_MEMORY
VERSION_CFLAGS += -DNO_SEGMENTED_MEMORY
DEFINES += NO_SEGMENTED_MEMORY=1

# Widescreen
CUSTOM_C_DEFINES += -DWIDESCREEN
VERSION_CFLAGS += -DWIDESCREEN
DEFINES += WIDESCREEN=1

# Options menu
EXT_OPTIONS_MENU := 1
CUSTOM_C_DEFINES += -DEXT_OPTIONS_MENU
VERSION_CFLAGS += -DEXT_OPTIONS_MENU
DEFINES += EXT_OPTIONS_MENU=1

# Better cam/Puppy cam
BETTERCAMERA := 1
CUSTOM_C_DEFINES += -DBETTERCAMERA
VERSION_CFLAGS += -DBETTERCAMERA
DEFINES += BETTERCAMERA=1

# Cheats
CHEATS_ACTIONS := 1
CUSTOM_C_DEFINES += -DCHEATS_ACTIONS
VERSION_CFLAGS += -DCHEATS_ACTIONS
DEFINES += CHEATS_ACTIONS=1

# No drawing distance
NODRAWINGDISTANCE := 1
CUSTOM_C_DEFINES += -DNODRAWINGDISTANCE
VERSION_CFLAGS += -DNODRAWINGDISTANCE
DEFINES += NODRAWINGDISTANCE=1

# External data
EXTERNAL_DATA := 1
CUSTOM_C_DEFINES += -DEXTERNAL_DATA -DFS_BASEDIR="\"$(BASEDIR)\""
VERSION_CFLAGS += -DEXTERNAL_DATA -DFS_BASEDIR="\"$(BASEDIR)\""
DEFINES += EXTERNAL_DATA=1 FS_BASEDIR="\"$(BASEDIR)\""

# Smoke texture fix
TEXTURE_FIX := 1
CUSTOM_C_DEFINES += -DTEXTURE_FIX
VERSION_CFLAGS += -DTEXTURE_FIX
DEFINES += TEXTURE_FIX=1

# Enable Mario colors
OMM_MARIO_COLORS := \
	function check_mario_lights { \
		if [ -z "$$(grep -e 'mario_red_lights_group' actors/mario/model.inc.c)" ]; then return 1; fi; \
		if [ -z "$$(grep -e 'mario_brown2_lights_group' actors/mario/model.inc.c)" ]; then return 1; fi; \
		if [ -z "$$(grep -e 'mario_beige_lights_group' actors/mario/model.inc.c)" ]; then return 1; fi; \
		if [ -z "$$(grep -e 'mario_white_lights_group' actors/mario/model.inc.c)" ]; then return 1; fi; \
		if [ -z "$$(grep -e 'mario_blue_lights_group' actors/mario/model.inc.c)" ]; then return 1; fi; \
		if [ -z "$$(grep -e 'mario_brown1_lights_group' actors/mario/model.inc.c)" ]; then return 1; fi; \
		if [ -z "$$(grep -e 'mario_cap_seg3_lights_0301CF38' actors/mario_cap/model.inc.c)" ]; then return 1; fi; \
		if [ -z "$$(grep -e 'mario_cap_seg3_lights_0301CF08' actors/mario_cap/model.inc.c)" ]; then return 1; fi; \
		return 0; \
	}; check_mario_lights; echo $$?;

ifeq ($(shell $(call OMM_MARIO_COLORS)),0)
  VERSION_CFLAGS += -DOMM_MK_MARIO_COLORS=1
  DEFINES += OMM_MK_MARIO_COLORS=1
else
  VERSION_CFLAGS += -DOMM_MK_MARIO_COLORS=0
  DEFINES += OMM_MK_MARIO_COLORS=0
endif

# --------
# Disabled
# --------

# Built-in debug menu
EXT_DEBUG_MENU := 0

# Built-in extended bounds
EXTENDED_BOUNDS := 0

# Built-in frame interpolation
HIGH_FPS_PC := 0

# No rumble
RUMBLE_FEEDBACK := 0

# No Goddard
GODDARD_MFACE := 0

# No Discord RPC
DISCORDRPC := 0

# No textsaves
TEXTSAVES := 0

# No extra QoL
VANILLA_CHECKS := 0
QOL_FIXES := 0
QOL_FEATURES := 0
QOL_REDONE := 0

# ------
# Custom
# ------

# Debug
ifeq ($(DEBUG),1)
  CUSTOM_C_DEFINES += -DDEBUG
  VERSION_CFLAGS += -DDEBUG
  DEFINES += DEBUG=1
  COMPILER_OPT := debug
endif

# Mouse (no DirectX yet)
ifneq ($(WINDOW_API),DXGI)
  CUSTOM_C_DEFINES += -DMOUSE_ACTIONS
endif

# CLI
ifeq ($(COMMAND_LINE_OPTIONS),1)
  CUSTOM_C_DEFINES += -DCOMMAND_LINE_OPTIONS
endif

# MOP
ifeq ($(PORT_MOP_OBJS),1)
  CUSTOM_C_DEFINES += -DPORT_MOP_OBJS
endif

# OMM Bowser
OMM_BOWSER ?= -1
ifneq ($(OMM_BOWSER),-1)
  CUSTOM_C_DEFINES += -DOMM_BOWSER=$(OMM_BOWSER)
  VERSION_CFLAGS += -DOMM_BOWSER=$(OMM_BOWSER)
  DEFINES += OMM_BOWSER=$(OMM_BOWSER)
endif

# OMM Debug menu
OMM_DEBUG ?= -1
ifneq ($(OMM_DEBUG),-1)
  CUSTOM_C_DEFINES += -DOMM_DEBUG=$(OMM_DEBUG)
  VERSION_CFLAGS += -DOMM_DEBUG=$(OMM_DEBUG)
  DEFINES += OMM_DEBUG=$(OMM_DEBUG)
endif

# OMM Dev features
OMM_DEV ?= -1
ifneq ($(OMM_DEV),-1)
  CUSTOM_C_DEFINES += -DOMM_DEV=$(OMM_DEV)
  VERSION_CFLAGS += -DOMM_DEV=$(OMM_DEV)
  DEFINES += OMM_DEV=$(OMM_DEV)
endif
