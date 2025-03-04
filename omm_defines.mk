# -------
# Enabled
# -------

# 64-bit file offsets
VERSION_CFLAGS += -D_FILE_OFFSET_BITS=64
CUSTOM_C_DEFINES += -D_FILE_OFFSET_BITS=64

# No segmented memory
VERSION_CFLAGS += -DNO_SEGMENTED_MEMORY
CUSTOM_C_DEFINES += -DNO_SEGMENTED_MEMORY

# Widescreen
VERSION_CFLAGS += -DWIDESCREEN
CUSTOM_C_DEFINES += -DWIDESCREEN

# Options menu
EXT_OPTIONS_MENU := 1
VERSION_CFLAGS += -DEXT_OPTIONS_MENU
CUSTOM_C_DEFINES += -DEXT_OPTIONS_MENU

# Better cam/Puppy cam
BETTERCAMERA := 1
VERSION_CFLAGS += -DBETTERCAMERA
CUSTOM_C_DEFINES += -DBETTERCAMERA

# Cheats
CHEATS_ACTIONS := 1
VERSION_CFLAGS += -DCHEATS_ACTIONS
CUSTOM_C_DEFINES += -DCHEATS_ACTIONS

# No drawing distance
NODRAWINGDISTANCE := 1
VERSION_CFLAGS += -DNODRAWINGDISTANCE
CUSTOM_C_DEFINES += -DNODRAWINGDISTANCE

# External data
EXTERNAL_DATA := 1
VERSION_CFLAGS += -DEXTERNAL_DATA -DFS_BASEDIR="\"$(BASEDIR)\""
CUSTOM_C_DEFINES += -DEXTERNAL_DATA -DFS_BASEDIR="\"$(BASEDIR)\""

# Smoke texture fix
TEXTURE_FIX := 1
VERSION_CFLAGS += -DTEXTURE_FIX
CUSTOM_C_DEFINES += -DTEXTURE_FIX

# Check custom skyboxes
ifeq ($(or $(and $(wildcard textures/skyboxes/Skybox_Rules.ld),1),0),1)
  VERSION_CFLAGS += -DOMM_HAS_CUSTOM_SKYBOXES=1
  CUSTOM_C_DEFINES += -DOMM_HAS_CUSTOM_SKYBOXES=1
else
  VERSION_CFLAGS += -DOMM_HAS_CUSTOM_SKYBOXES=0
  CUSTOM_C_DEFINES += -DOMM_HAS_CUSTOM_SKYBOXES=0
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
TOUCH_CONTROLS := 0

# ------
# Custom
# ------

# Debug
ifeq ($(DEBUG),1)
  VERSION_CFLAGS += -DDEBUG
  CUSTOM_C_DEFINES += -DDEBUG
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

# OMM Debug menu
OMM_DEBUG ?= -1
ifneq ($(OMM_DEBUG),-1)
  VERSION_CFLAGS += -DOMM_DEBUG=$(OMM_DEBUG)
  CUSTOM_C_DEFINES += -DOMM_DEBUG=$(OMM_DEBUG)
endif

# OMM Dev features
OMM_DEV ?= -1
ifneq ($(OMM_DEV),-1)
  VERSION_CFLAGS += -DOMM_DEV=$(OMM_DEV)
  CUSTOM_C_DEFINES += -DOMM_DEV=$(OMM_DEV)
endif

# -------
# Patches
# -------

# Remove the use of SKYCONV; Skyboxes are no longer tiled
NO_SKYBOX_TILES != \
  test -f Makefile.split && sed -i 's/.*SKYCONV.*/\t@touch $$@/' Makefile.split ;

# Fill the unused `length` member of `struct Animation` with the length of `index` and `values`
FIX_MARIO_ANIMS_LENGTH != \
  test -f tools/mario_anims_converter.py && sed -i 's;                offset_to_end + " - " + offset_to_struct;"((sizeof(gMarioAnims." + indices + ") / sizeof(u16)) << 20) | ((sizeof(gMarioAnims." + values + ") / sizeof(s16)) << 0)";' tools/mario_anims_converter.py ; \
  test -f tools/luigi_anims_converter.py && sed -i 's;                offset_to_end + " - " + offset_to_struct;"((sizeof(gLuigiAnims." + indices + ") / sizeof(u16)) << 20) | ((sizeof(gLuigiAnims." + values + ") / sizeof(s16)) << 0)";' tools/luigi_anims_converter.py ; \
  test -f tools/wario_anims_converter.py && sed -i 's;                offset_to_end + " - " + offset_to_struct;"((sizeof(gWarioAnims." + indices + ") / sizeof(u16)) << 20) | ((sizeof(gWarioAnims." + values + ") / sizeof(s16)) << 0)";' tools/wario_anims_converter.py ;

# Custom mkzip must be used to exclude copyrighted assets from the basepack
NO_MKZIP != \
  : > tools/mkzip.py ;

# Fix a bettercam bug
FIX_BETTERCAM != \
  test -f src/game/bettercamera.inc.h && sed -i 's/ivrt(newcam_invertX)/ivrt(0)/' src/game/bettercamera.inc.h; \
  test -f src/game/bettercamera.inc.h && sed -i 's/ivrt(newcam_invertY)/ivrt(1)/' src/game/bettercamera.inc.h;
