#include <string>
#include <vector>
#include <map>
#include <algorithm>
extern "C" {
#include "data/omm/omm_includes.h"
#include "data/omm/models/omm_models_cs_char_sound.h"
#include "mario_animation_ids.h"
extern const GeoLayout mario_geo[];
}

#define LUA_MAGIC_0 0x1B
#define LUA_MAGIC_1 0x4C
#define LUA_MAGIC_2 0x75
#define LUA_MAGIC_3 0x61

//
// Debug
//

#if OMM_DEBUG

#define omm_models_cs_print_pack_folder() \
    { omm_printf("\n"); omm_log("\"%s\"\n",, pack_folder.c_str()); }

#define omm_models_cs_print_file() \
    { omm_printf("  File: \"%s\"\n",, strrchr(path, '/') + 1); }

#define omm_models_cs_print(fmt, ...) \
    { omm_printf("    " fmt "\n",, __VA_ARGS__); }

#define omm_models_cs_warning(fmt, ...) \
    { omm_printf("\033[33m    (!) " fmt "\033[0m\n",, __VA_ARGS__); }

#define omm_models_cs_error(fmt, ...) \
    { omm_printf("\033[31m    <!> " fmt "\033[0m\n",, __VA_ARGS__); }

#else

#define omm_models_cs_print_pack_folder()

#define omm_models_cs_print_file()

#define omm_models_cs_print(fmt, ...)

#define omm_models_cs_warning(fmt, ...)

#define omm_models_cs_error(fmt, ...)

#endif

//
// Constants
//

typedef struct CsFunction {
    const char *function;
    s32 model_id_arg;
    s32 property_arg;
    s32 property_index;
} CsFunction;

typedef struct CsSoundVariant {
    s32 char_sound;
    u32 size;
    s32 variant;
    u32 offset;
    u32 range;
} CsSoundVariant;

static const CsFunction CS_FUNCTIONS[] = {
    { "character_add(",                4, 0, 0 },
    { "char_add(",                     4, 0, 0 },
    { "character_add_costume(",        5, 1, 0 },
    { "char_add_costume(",             5, 1, 0 },
    { "character_edit(",               5, 1, 0 },
    { "char_edit(",                    5, 1, 0 },
    { "character_add_caps(",           0, 1, 1 },
    { "char_add_caps(",                0, 1, 1 },
    { "character_add_palette_preset(", 0, 1, 2 },
    { "char_add_palette(",             0, 1, 2 },
    { "character_add_voice(",          0, 1, 3 },
    { "char_add_voice(",               0, 1, 3 },
    { "character_add_animations(",     0, 1, 4 },
    { "char_add_animations(",          0, 1, 4 },
};

static const char *CS_PROPERTIES[] = {
    "name",
    "caps",
    "palette",
    "voices",
    "anims"
};

static const char *CS_PROPERTY_COMMON_NAMES[][4] = {
    { "name", NULL },
    { "caps", NULL },
    { "palette", "pal", NULL },
    { "voices", "sounds", "audio", NULL },
    { "anims", "animations", NULL },
};

static const char *CS_CAP_TYPES[] = {
    "normal",
    "wing",
    "metal",
    "metalWing",
};

static const s32 CS_CAP_DEFAULTS[][3] = {
    [0] = { 1, 2, 3 }, // normal > wing > metal > metalWing
    [1] = { 0, 3, 2 }, // wing > normal > metalWing > metal
    [2] = { 3, 0, 1 }, // metal > metalWing > normal > wing
    [3] = { 2, 1, 0 }, // metalWing > metal > wing > normal
};

static const char *CS_PALETTE_COLORS[] = {
    "PANTS",
    "SHIRT",
    "GLOVES",
    "SHOES",
    "HAIR",
    "SKIN",
    "CAP",
    "EMBLEM",
};

static const char *CS_CHAR_SOUNDS[] = {
    [CHAR_SOUND_YAH_WAH_HOO] = "CHAR_SOUND_YAH_WAH_HOO",
    [CHAR_SOUND_HOOHOO] = "CHAR_SOUND_HOOHOO",
    [CHAR_SOUND_YAHOO] = "CHAR_SOUND_YAHOO",
    [CHAR_SOUND_UH] = "CHAR_SOUND_UH",
    [CHAR_SOUND_HRMM] = "CHAR_SOUND_HRMM",
    [CHAR_SOUND_WAH2] = "CHAR_SOUND_WAH2",
    [CHAR_SOUND_WHOA] = "CHAR_SOUND_WHOA",
    [CHAR_SOUND_EEUH] = "CHAR_SOUND_EEUH",
    [CHAR_SOUND_ATTACKED] = "CHAR_SOUND_ATTACKED",
    [CHAR_SOUND_OOOF] = "CHAR_SOUND_OOOF",
    [CHAR_SOUND_OOOF2] = "CHAR_SOUND_OOOF2",
    [CHAR_SOUND_HERE_WE_GO] = "CHAR_SOUND_HERE_WE_GO",
    [CHAR_SOUND_YAWNING] = "CHAR_SOUND_YAWNING",
    [CHAR_SOUND_SNORING1] = "CHAR_SOUND_SNORING1",
    [CHAR_SOUND_SNORING2] = "CHAR_SOUND_SNORING2",
    [CHAR_SOUND_WAAAOOOW] = "CHAR_SOUND_WAAAOOOW",
    [CHAR_SOUND_HAHA] = "CHAR_SOUND_HAHA",
    [CHAR_SOUND_HAHA_2] = "CHAR_SOUND_HAHA_2",
    [CHAR_SOUND_UH2] = "CHAR_SOUND_UH2",
    [CHAR_SOUND_UH2_2] = "CHAR_SOUND_UH2_2",
    [CHAR_SOUND_ON_FIRE] = "CHAR_SOUND_ON_FIRE",
    [CHAR_SOUND_DYING] = "CHAR_SOUND_DYING",
    [CHAR_SOUND_PANTING_COLD] = "CHAR_SOUND_PANTING_COLD",
    [CHAR_SOUND_PANTING] = "CHAR_SOUND_PANTING",
    [CHAR_SOUND_COUGHING1] = "CHAR_SOUND_COUGHING1",
    [CHAR_SOUND_COUGHING2] = "CHAR_SOUND_COUGHING2",
    [CHAR_SOUND_COUGHING3] = "CHAR_SOUND_COUGHING3",
    [CHAR_SOUND_PUNCH_YAH] = "CHAR_SOUND_PUNCH_YAH",
    [CHAR_SOUND_PUNCH_HOO] = "CHAR_SOUND_PUNCH_HOO",
    [CHAR_SOUND_MAMA_MIA] = "CHAR_SOUND_MAMA_MIA",
    [CHAR_SOUND_GROUND_POUND_WAH] = "CHAR_SOUND_GROUND_POUND_WAH",
    [CHAR_SOUND_DROWNING] = "CHAR_SOUND_DROWNING",
    [CHAR_SOUND_PUNCH_WAH] = "CHAR_SOUND_PUNCH_WAH",
    [CHAR_SOUND_YAHOO_WAHA_YIPPEE] = "CHAR_SOUND_YAHOO_WAHA_YIPPEE",
    [CHAR_SOUND_DOH] = "CHAR_SOUND_DOH",
    [CHAR_SOUND_GAME_OVER] = "CHAR_SOUND_GAME_OVER",
    [CHAR_SOUND_HELLO] = "CHAR_SOUND_HELLO",
    [CHAR_SOUND_PRESS_START_TO_PLAY] = "CHAR_SOUND_PRESS_START_TO_PLAY",
    [CHAR_SOUND_TWIRL_BOUNCE] = "CHAR_SOUND_TWIRL_BOUNCE",
    [CHAR_SOUND_SNORING3] = "CHAR_SOUND_SNORING3",
    [CHAR_SOUND_SO_LONGA_BOWSER] = "CHAR_SOUND_SO_LONGA_BOWSER",
    [CHAR_SOUND_IMA_TIRED] = "CHAR_SOUND_IMA_TIRED",
    [CHAR_SOUND_LETS_A_GO] = "CHAR_SOUND_LETS_A_GO",
    [CHAR_SOUND_OKEY_DOKEY] = "CHAR_SOUND_OKEY_DOKEY",
};

static const CsSoundVariant CS_CHAR_SOUND_VARIANTS[] = {

// YAH_WAH_HOO

    // Yah, Wah, Hoo
    { .char_sound = CHAR_SOUND_YAH_WAH_HOO, .size = 3, .variant = 0, .offset = 0, .range = 1 },
    { .char_sound = CHAR_SOUND_YAH_WAH_HOO, .size = 3, .variant = 1, .offset = 1, .range = 1 },
    { .char_sound = CHAR_SOUND_YAH_WAH_HOO, .size = 3, .variant = 2, .offset = 2, .range = 1 },

// YAHOO_WAHA_YIPPEE

    // 1x Yahoo, 1x Waha, 1x Yippee
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 3, .variant = 0, .offset = 0, .range = 1 },
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 3, .variant = 1, .offset = 1, .range = 1 },
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 3, .variant = 2, .offset = 2, .range = 1 },

    // 3x Yahoo, 1x Waha, 1x Yippee
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 5, .variant = 0, .offset = 0, .range = 3 },
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 5, .variant = 1, .offset = 3, .range = 1 },
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 5, .variant = 2, .offset = 4, .range = 1 },

    // 4x Yahoo, 1x Waha, 1x Yippee
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 6, .variant = 0, .offset = 0, .range = 4 },
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 6, .variant = 1, .offset = 4, .range = 1 },
    { .char_sound = CHAR_SOUND_YAHOO_WAHA_YIPPEE, .size = 6, .variant = 2, .offset = 5, .range = 1 },

// PANTING

    // Panting 1, 2, 3
    { .char_sound = CHAR_SOUND_PANTING, .size = 3, .variant = 0, .offset = 0, .range = 1 },
    { .char_sound = CHAR_SOUND_PANTING, .size = 3, .variant = 1, .offset = 1, .range = 1 },
    { .char_sound = CHAR_SOUND_PANTING, .size = 3, .variant = 2, .offset = 2, .range = 1 },
};

static const char *CS_CHAR_ANIMS[] = {
    [MARIO_ANIM_SLOW_LEDGE_GRAB] = "CHAR_ANIM_SLOW_LEDGE_GRAB",
    [MARIO_ANIM_FALL_OVER_BACKWARDS] = "CHAR_ANIM_FALL_OVER_BACKWARDS",
    [MARIO_ANIM_BACKWARD_AIR_KB] = "CHAR_ANIM_BACKWARD_AIR_KB",
    [MARIO_ANIM_DYING_ON_BACK] = "CHAR_ANIM_DYING_ON_BACK",
    [MARIO_ANIM_BACKFLIP] = "CHAR_ANIM_BACKFLIP",
    [MARIO_ANIM_CLIMB_UP_POLE] = "CHAR_ANIM_CLIMB_UP_POLE",
    [MARIO_ANIM_GRAB_POLE_SHORT] = "CHAR_ANIM_GRAB_POLE_SHORT",
    [MARIO_ANIM_GRAB_POLE_SWING_PART1] = "CHAR_ANIM_GRAB_POLE_SWING_PART1",
    [MARIO_ANIM_GRAB_POLE_SWING_PART2] = "CHAR_ANIM_GRAB_POLE_SWING_PART2",
    [MARIO_ANIM_HANDSTAND_IDLE] = "CHAR_ANIM_HANDSTAND_IDLE",
    [MARIO_ANIM_HANDSTAND_JUMP] = "CHAR_ANIM_HANDSTAND_JUMP",
    [MARIO_ANIM_START_HANDSTAND] = "CHAR_ANIM_START_HANDSTAND",
    [MARIO_ANIM_RETURN_FROM_HANDSTAND] = "CHAR_ANIM_RETURN_FROM_HANDSTAND",
    [MARIO_ANIM_IDLE_ON_POLE] = "CHAR_ANIM_IDLE_ON_POLE",
    [MARIO_ANIM_A_POSE] = "CHAR_ANIM_A_POSE",
    [MARIO_ANIM_SKID_ON_GROUND] = "CHAR_ANIM_SKID_ON_GROUND",
    [MARIO_ANIM_STOP_SKID] = "CHAR_ANIM_STOP_SKID",
    [MARIO_ANIM_CROUCH_FROM_FAST_LONGJUMP] = "CHAR_ANIM_CROUCH_FROM_FAST_LONGJUMP",
    [MARIO_ANIM_CROUCH_FROM_SLOW_LONGJUMP] = "CHAR_ANIM_CROUCH_FROM_SLOW_LONGJUMP",
    [MARIO_ANIM_FAST_LONGJUMP] = "CHAR_ANIM_FAST_LONGJUMP",
    [MARIO_ANIM_SLOW_LONGJUMP] = "CHAR_ANIM_SLOW_LONGJUMP",
    [MARIO_ANIM_AIRBORNE_ON_STOMACH] = "CHAR_ANIM_AIRBORNE_ON_STOMACH",
    [MARIO_ANIM_WALK_WITH_LIGHT_OBJ] = "CHAR_ANIM_WALK_WITH_LIGHT_OBJ",
    [MARIO_ANIM_RUN_WITH_LIGHT_OBJ] = "CHAR_ANIM_RUN_WITH_LIGHT_OBJ",
    [MARIO_ANIM_SLOW_WALK_WITH_LIGHT_OBJ] = "CHAR_ANIM_SLOW_WALK_WITH_LIGHT_OBJ",
    [MARIO_ANIM_SHIVERING_WARMING_HAND] = "CHAR_ANIM_SHIVERING_WARMING_HAND",
    [MARIO_ANIM_SHIVERING_RETURN_TO_IDLE] = "CHAR_ANIM_SHIVERING_RETURN_TO_IDLE",
    [MARIO_ANIM_SHIVERING] = "CHAR_ANIM_SHIVERING",
    [MARIO_ANIM_CLIMB_DOWN_LEDGE] = "CHAR_ANIM_CLIMB_DOWN_LEDGE",
    [MARIO_ANIM_CREDITS_WAVING] = "CHAR_ANIM_CREDITS_WAVING",
    [MARIO_ANIM_CREDITS_LOOK_UP] = "CHAR_ANIM_CREDITS_LOOK_UP",
    [MARIO_ANIM_CREDITS_RETURN_FROM_LOOK_UP] = "CHAR_ANIM_CREDITS_RETURN_FROM_LOOK_UP",
    [MARIO_ANIM_CREDITS_RAISE_HAND] = "CHAR_ANIM_CREDITS_RAISE_HAND",
    [MARIO_ANIM_CREDITS_LOWER_HAND] = "CHAR_ANIM_CREDITS_LOWER_HAND",
    [MARIO_ANIM_CREDITS_TAKE_OFF_CAP] = "CHAR_ANIM_CREDITS_TAKE_OFF_CAP",
    [MARIO_ANIM_CREDITS_START_WALK_LOOK_UP] = "CHAR_ANIM_CREDITS_START_WALK_LOOK_UP",
    [MARIO_ANIM_CREDITS_LOOK_BACK_THEN_RUN] = "CHAR_ANIM_CREDITS_LOOK_BACK_THEN_RUN",
    [MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN] = "CHAR_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN",
    [MARIO_ANIM_FINAL_BOWSER_WING_CAP_TAKE_OFF] = "CHAR_ANIM_FINAL_BOWSER_WING_CAP_TAKE_OFF",
    [MARIO_ANIM_CREDITS_PEACE_SIGN] = "CHAR_ANIM_CREDITS_PEACE_SIGN",
    [MARIO_ANIM_STAND_UP_FROM_LAVA_BOOST] = "CHAR_ANIM_STAND_UP_FROM_LAVA_BOOST",
    [MARIO_ANIM_FIRE_LAVA_BURN] = "CHAR_ANIM_FIRE_LAVA_BURN",
    [MARIO_ANIM_WING_CAP_FLY] = "CHAR_ANIM_WING_CAP_FLY",
    [MARIO_ANIM_HANG_ON_OWL] = "CHAR_ANIM_HANG_ON_OWL",
    [MARIO_ANIM_LAND_ON_STOMACH] = "CHAR_ANIM_LAND_ON_STOMACH",
    [MARIO_ANIM_AIR_FORWARD_KB] = "CHAR_ANIM_AIR_FORWARD_KB",
    [MARIO_ANIM_DYING_ON_STOMACH] = "CHAR_ANIM_DYING_ON_STOMACH",
    [MARIO_ANIM_SUFFOCATING] = "CHAR_ANIM_SUFFOCATING",
    [MARIO_ANIM_COUGHING] = "CHAR_ANIM_COUGHING",
    [MARIO_ANIM_THROW_CATCH_KEY] = "CHAR_ANIM_THROW_CATCH_KEY",
    [MARIO_ANIM_DYING_FALL_OVER] = "CHAR_ANIM_DYING_FALL_OVER",
    [MARIO_ANIM_IDLE_ON_LEDGE] = "CHAR_ANIM_IDLE_ON_LEDGE",
    [MARIO_ANIM_FAST_LEDGE_GRAB] = "CHAR_ANIM_FAST_LEDGE_GRAB",
    [MARIO_ANIM_HANG_ON_CEILING] = "CHAR_ANIM_HANG_ON_CEILING",
    [MARIO_ANIM_PUT_CAP_ON] = "CHAR_ANIM_PUT_CAP_ON",
    [MARIO_ANIM_TAKE_CAP_OFF_THEN_ON] = "CHAR_ANIM_TAKE_CAP_OFF_THEN_ON",
    [MARIO_ANIM_QUICKLY_PUT_CAP_ON] = "CHAR_ANIM_QUICKLY_PUT_CAP_ON",
    [MARIO_ANIM_HEAD_STUCK_IN_GROUND] = "CHAR_ANIM_HEAD_STUCK_IN_GROUND",
    [MARIO_ANIM_GROUND_POUND_LANDING] = "CHAR_ANIM_GROUND_POUND_LANDING",
    [MARIO_ANIM_TRIPLE_JUMP_GROUND_POUND] = "CHAR_ANIM_TRIPLE_JUMP_GROUND_POUND",
    [MARIO_ANIM_START_GROUND_POUND] = "CHAR_ANIM_START_GROUND_POUND",
    [MARIO_ANIM_GROUND_POUND] = "CHAR_ANIM_GROUND_POUND",
    [MARIO_ANIM_BOTTOM_STUCK_IN_GROUND] = "CHAR_ANIM_BOTTOM_STUCK_IN_GROUND",
    [MARIO_ANIM_IDLE_WITH_LIGHT_OBJ] = "CHAR_ANIM_IDLE_WITH_LIGHT_OBJ",
    [MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ] = "CHAR_ANIM_JUMP_LAND_WITH_LIGHT_OBJ",
    [MARIO_ANIM_JUMP_WITH_LIGHT_OBJ] = "CHAR_ANIM_JUMP_WITH_LIGHT_OBJ",
    [MARIO_ANIM_FALL_LAND_WITH_LIGHT_OBJ] = "CHAR_ANIM_FALL_LAND_WITH_LIGHT_OBJ",
    [MARIO_ANIM_FALL_WITH_LIGHT_OBJ] = "CHAR_ANIM_FALL_WITH_LIGHT_OBJ",
    [MARIO_ANIM_FALL_FROM_SLIDING_WITH_LIGHT_OBJ] = "CHAR_ANIM_FALL_FROM_SLIDING_WITH_LIGHT_OBJ",
    [MARIO_ANIM_SLIDING_ON_BOTTOM_WITH_LIGHT_OBJ] = "CHAR_ANIM_SLIDING_ON_BOTTOM_WITH_LIGHT_OBJ",
    [MARIO_ANIM_STAND_UP_FROM_SLIDING_WITH_LIGHT_OBJ] = "CHAR_ANIM_STAND_UP_FROM_SLIDING_WITH_LIGHT_OBJ",
    [MARIO_ANIM_RIDING_SHELL] = "CHAR_ANIM_RIDING_SHELL",
    [MARIO_ANIM_WALKING] = "CHAR_ANIM_WALKING",
    [MARIO_ANIM_FORWARD_FLIP] = "CHAR_ANIM_FORWARD_FLIP",
    [MARIO_ANIM_JUMP_RIDING_SHELL] = "CHAR_ANIM_JUMP_RIDING_SHELL",
    [MARIO_ANIM_LAND_FROM_DOUBLE_JUMP] = "CHAR_ANIM_LAND_FROM_DOUBLE_JUMP",
    [MARIO_ANIM_DOUBLE_JUMP_FALL] = "CHAR_ANIM_DOUBLE_JUMP_FALL",
    [MARIO_ANIM_SINGLE_JUMP] = "CHAR_ANIM_SINGLE_JUMP",
    [MARIO_ANIM_LAND_FROM_SINGLE_JUMP] = "CHAR_ANIM_LAND_FROM_SINGLE_JUMP",
    [MARIO_ANIM_AIR_KICK] = "CHAR_ANIM_AIR_KICK",
    [MARIO_ANIM_DOUBLE_JUMP_RISE] = "CHAR_ANIM_DOUBLE_JUMP_RISE",
    [MARIO_ANIM_START_FORWARD_SPINNING] = "CHAR_ANIM_START_FORWARD_SPINNING",
    [MARIO_ANIM_THROW_LIGHT_OBJECT] = "CHAR_ANIM_THROW_LIGHT_OBJECT",
    [MARIO_ANIM_FALL_FROM_SLIDE_KICK] = "CHAR_ANIM_FALL_FROM_SLIDE_KICK",
    [MARIO_ANIM_BEND_KNESS_RIDING_SHELL] = "CHAR_ANIM_BEND_KNESS_RIDING_SHELL",
    [MARIO_ANIM_LEGS_STUCK_IN_GROUND] = "CHAR_ANIM_LEGS_STUCK_IN_GROUND",
    [MARIO_ANIM_GENERAL_FALL] = "CHAR_ANIM_GENERAL_FALL",
    [MARIO_ANIM_GENERAL_LAND] = "CHAR_ANIM_GENERAL_LAND",
    [MARIO_ANIM_BEING_GRABBED] = "CHAR_ANIM_BEING_GRABBED",
    [MARIO_ANIM_GRAB_HEAVY_OBJECT] = "CHAR_ANIM_GRAB_HEAVY_OBJECT",
    [MARIO_ANIM_SLOW_LAND_FROM_DIVE] = "CHAR_ANIM_SLOW_LAND_FROM_DIVE",
    [MARIO_ANIM_FLY_FROM_CANNON] = "CHAR_ANIM_FLY_FROM_CANNON",
    [MARIO_ANIM_MOVE_ON_WIRE_NET_RIGHT] = "CHAR_ANIM_MOVE_ON_WIRE_NET_RIGHT",
    [MARIO_ANIM_MOVE_ON_WIRE_NET_LEFT] = "CHAR_ANIM_MOVE_ON_WIRE_NET_LEFT",
    [MARIO_ANIM_MISSING_CAP] = "CHAR_ANIM_MISSING_CAP",
    [MARIO_ANIM_PULL_DOOR_WALK_IN] = "CHAR_ANIM_PULL_DOOR_WALK_IN",
    [MARIO_ANIM_PUSH_DOOR_WALK_IN] = "CHAR_ANIM_PUSH_DOOR_WALK_IN",
    [MARIO_ANIM_UNLOCK_DOOR] = "CHAR_ANIM_UNLOCK_DOOR",
    [MARIO_ANIM_START_REACH_POCKET] = "CHAR_ANIM_START_REACH_POCKET",
    [MARIO_ANIM_REACH_POCKET] = "CHAR_ANIM_REACH_POCKET",
    [MARIO_ANIM_STOP_REACH_POCKET] = "CHAR_ANIM_STOP_REACH_POCKET",
    [MARIO_ANIM_GROUND_THROW] = "CHAR_ANIM_GROUND_THROW",
    [MARIO_ANIM_GROUND_KICK] = "CHAR_ANIM_GROUND_KICK",
    [MARIO_ANIM_FIRST_PUNCH] = "CHAR_ANIM_FIRST_PUNCH",
    [MARIO_ANIM_SECOND_PUNCH] = "CHAR_ANIM_SECOND_PUNCH",
    [MARIO_ANIM_FIRST_PUNCH_FAST] = "CHAR_ANIM_FIRST_PUNCH_FAST",
    [MARIO_ANIM_SECOND_PUNCH_FAST] = "CHAR_ANIM_SECOND_PUNCH_FAST",
    [MARIO_ANIM_PICK_UP_LIGHT_OBJ] = "CHAR_ANIM_PICK_UP_LIGHT_OBJ",
    [MARIO_ANIM_PUSHING] = "CHAR_ANIM_PUSHING",
    [MARIO_ANIM_START_RIDING_SHELL] = "CHAR_ANIM_START_RIDING_SHELL",
    [MARIO_ANIM_PLACE_LIGHT_OBJ] = "CHAR_ANIM_PLACE_LIGHT_OBJ",
    [MARIO_ANIM_FORWARD_SPINNING] = "CHAR_ANIM_FORWARD_SPINNING",
    [MARIO_ANIM_BACKWARD_SPINNING] = "CHAR_ANIM_BACKWARD_SPINNING",
    [MARIO_ANIM_BREAKDANCE] = "CHAR_ANIM_BREAKDANCE",
    [MARIO_ANIM_RUNNING] = "CHAR_ANIM_RUNNING",
    [MARIO_ANIM_RUNNING_UNUSED] = "CHAR_ANIM_RUNNING_UNUSED",
    [MARIO_ANIM_SOFT_BACK_KB] = "CHAR_ANIM_SOFT_BACK_KB",
    [MARIO_ANIM_SOFT_FRONT_KB] = "CHAR_ANIM_SOFT_FRONT_KB",
    [MARIO_ANIM_DYING_IN_QUICKSAND] = "CHAR_ANIM_DYING_IN_QUICKSAND",
    [MARIO_ANIM_IDLE_IN_QUICKSAND] = "CHAR_ANIM_IDLE_IN_QUICKSAND",
    [MARIO_ANIM_MOVE_IN_QUICKSAND] = "CHAR_ANIM_MOVE_IN_QUICKSAND",
    [MARIO_ANIM_ELECTROCUTION] = "CHAR_ANIM_ELECTROCUTION",
    [MARIO_ANIM_SHOCKED] = "CHAR_ANIM_SHOCKED",
    [MARIO_ANIM_BACKWARD_KB] = "CHAR_ANIM_BACKWARD_KB",
    [MARIO_ANIM_FORWARD_KB] = "CHAR_ANIM_FORWARD_KB",
    [MARIO_ANIM_IDLE_HEAVY_OBJ] = "CHAR_ANIM_IDLE_HEAVY_OBJ",
    [MARIO_ANIM_STAND_AGAINST_WALL] = "CHAR_ANIM_STAND_AGAINST_WALL",
    [MARIO_ANIM_SIDESTEP_LEFT] = "CHAR_ANIM_SIDESTEP_LEFT",
    [MARIO_ANIM_SIDESTEP_RIGHT] = "CHAR_ANIM_SIDESTEP_RIGHT",
    [MARIO_ANIM_START_SLEEP_IDLE] = "CHAR_ANIM_START_SLEEP_IDLE",
    [MARIO_ANIM_START_SLEEP_SCRATCH] = "CHAR_ANIM_START_SLEEP_SCRATCH",
    [MARIO_ANIM_START_SLEEP_YAWN] = "CHAR_ANIM_START_SLEEP_YAWN",
    [MARIO_ANIM_START_SLEEP_SITTING] = "CHAR_ANIM_START_SLEEP_SITTING",
    [MARIO_ANIM_SLEEP_IDLE] = "CHAR_ANIM_SLEEP_IDLE",
    [MARIO_ANIM_SLEEP_START_LYING] = "CHAR_ANIM_SLEEP_START_LYING",
    [MARIO_ANIM_SLEEP_LYING] = "CHAR_ANIM_SLEEP_LYING",
    [MARIO_ANIM_DIVE] = "CHAR_ANIM_DIVE",
    [MARIO_ANIM_SLIDE_DIVE] = "CHAR_ANIM_SLIDE_DIVE",
    [MARIO_ANIM_GROUND_BONK] = "CHAR_ANIM_GROUND_BONK",
    [MARIO_ANIM_STOP_SLIDE_LIGHT_OBJ] = "CHAR_ANIM_STOP_SLIDE_LIGHT_OBJ",
    [MARIO_ANIM_SLIDE_KICK] = "CHAR_ANIM_SLIDE_KICK",
    [MARIO_ANIM_CROUCH_FROM_SLIDE_KICK] = "CHAR_ANIM_CROUCH_FROM_SLIDE_KICK",
    [MARIO_ANIM_SLIDE_MOTIONLESS] = "CHAR_ANIM_SLIDE_MOTIONLESS",
    [MARIO_ANIM_STOP_SLIDE] = "CHAR_ANIM_STOP_SLIDE",
    [MARIO_ANIM_FALL_FROM_SLIDE] = "CHAR_ANIM_FALL_FROM_SLIDE",
    [MARIO_ANIM_SLIDE] = "CHAR_ANIM_SLIDE",
    [MARIO_ANIM_TIPTOE] = "CHAR_ANIM_TIPTOE",
    [MARIO_ANIM_TWIRL_LAND] = "CHAR_ANIM_TWIRL_LAND",
    [MARIO_ANIM_TWIRL] = "CHAR_ANIM_TWIRL",
    [MARIO_ANIM_START_TWIRL] = "CHAR_ANIM_START_TWIRL",
    [MARIO_ANIM_STOP_CROUCHING] = "CHAR_ANIM_STOP_CROUCHING",
    [MARIO_ANIM_START_CROUCHING] = "CHAR_ANIM_START_CROUCHING",
    [MARIO_ANIM_CROUCHING] = "CHAR_ANIM_CROUCHING",
    [MARIO_ANIM_CRAWLING] = "CHAR_ANIM_CRAWLING",
    [MARIO_ANIM_STOP_CRAWLING] = "CHAR_ANIM_STOP_CRAWLING",
    [MARIO_ANIM_START_CRAWLING] = "CHAR_ANIM_START_CRAWLING",
    [MARIO_ANIM_SUMMON_STAR] = "CHAR_ANIM_SUMMON_STAR",
    [MARIO_ANIM_RETURN_STAR_APPROACH_DOOR] = "CHAR_ANIM_RETURN_STAR_APPROACH_DOOR",
    [MARIO_ANIM_BACKWARDS_WATER_KB] = "CHAR_ANIM_BACKWARDS_WATER_KB",
    [MARIO_ANIM_SWIM_WITH_OBJ_PART1] = "CHAR_ANIM_SWIM_WITH_OBJ_PART1",
    [MARIO_ANIM_SWIM_WITH_OBJ_PART2] = "CHAR_ANIM_SWIM_WITH_OBJ_PART2",
    [MARIO_ANIM_FLUTTERKICK_WITH_OBJ] = "CHAR_ANIM_FLUTTERKICK_WITH_OBJ",
    [MARIO_ANIM_WATER_ACTION_END_WITH_OBJ] = "CHAR_ANIM_WATER_ACTION_END_WITH_OBJ",
    [MARIO_ANIM_STOP_GRAB_OBJ_WATER] = "CHAR_ANIM_STOP_GRAB_OBJ_WATER",
    [MARIO_ANIM_WATER_IDLE_WITH_OBJ] = "CHAR_ANIM_WATER_IDLE_WITH_OBJ",
    [MARIO_ANIM_DROWNING_PART1] = "CHAR_ANIM_DROWNING_PART1",
    [MARIO_ANIM_DROWNING_PART2] = "CHAR_ANIM_DROWNING_PART2",
    [MARIO_ANIM_WATER_DYING] = "CHAR_ANIM_WATER_DYING",
    [MARIO_ANIM_WATER_FORWARD_KB] = "CHAR_ANIM_WATER_FORWARD_KB",
    [MARIO_ANIM_FALL_FROM_WATER] = "CHAR_ANIM_FALL_FROM_WATER",
    [MARIO_ANIM_SWIM_PART1] = "CHAR_ANIM_SWIM_PART1",
    [MARIO_ANIM_SWIM_PART2] = "CHAR_ANIM_SWIM_PART2",
    [MARIO_ANIM_FLUTTERKICK] = "CHAR_ANIM_FLUTTERKICK",
    [MARIO_ANIM_WATER_ACTION_END] = "CHAR_ANIM_WATER_ACTION_END",
    [MARIO_ANIM_WATER_PICK_UP_OBJ] = "CHAR_ANIM_WATER_PICK_UP_OBJ",
    [MARIO_ANIM_WATER_GRAB_OBJ_PART2] = "CHAR_ANIM_WATER_GRAB_OBJ_PART2",
    [MARIO_ANIM_WATER_GRAB_OBJ_PART1] = "CHAR_ANIM_WATER_GRAB_OBJ_PART1",
    [MARIO_ANIM_WATER_THROW_OBJ] = "CHAR_ANIM_WATER_THROW_OBJ",
    [MARIO_ANIM_WATER_IDLE] = "CHAR_ANIM_WATER_IDLE",
    [MARIO_ANIM_WATER_STAR_DANCE] = "CHAR_ANIM_WATER_STAR_DANCE",
    [MARIO_ANIM_RETURN_FROM_WATER_STAR_DANCE] = "CHAR_ANIM_RETURN_FROM_WATER_STAR_DANCE",
    [MARIO_ANIM_GRAB_BOWSER] = "CHAR_ANIM_GRAB_BOWSER",
    [MARIO_ANIM_SWINGING_BOWSER] = "CHAR_ANIM_SWINGING_BOWSER",
    [MARIO_ANIM_RELEASE_BOWSER] = "CHAR_ANIM_RELEASE_BOWSER",
    [MARIO_ANIM_HOLDING_BOWSER] = "CHAR_ANIM_HOLDING_BOWSER",
    [MARIO_ANIM_HEAVY_THROW] = "CHAR_ANIM_HEAVY_THROW",
    [MARIO_ANIM_WALK_PANTING] = "CHAR_ANIM_WALK_PANTING",
    [MARIO_ANIM_WALK_WITH_HEAVY_OBJ] = "CHAR_ANIM_WALK_WITH_HEAVY_OBJ",
    [MARIO_ANIM_TURNING_PART1] = "CHAR_ANIM_TURNING_PART1",
    [MARIO_ANIM_TURNING_PART2] = "CHAR_ANIM_TURNING_PART2",
    [MARIO_ANIM_SLIDEFLIP_LAND] = "CHAR_ANIM_SLIDEFLIP_LAND",
    [MARIO_ANIM_SLIDEFLIP] = "CHAR_ANIM_SLIDEFLIP",
    [MARIO_ANIM_TRIPLE_JUMP_LAND] = "CHAR_ANIM_TRIPLE_JUMP_LAND",
    [MARIO_ANIM_TRIPLE_JUMP] = "CHAR_ANIM_TRIPLE_JUMP",
    [MARIO_ANIM_FIRST_PERSON] = "CHAR_ANIM_FIRST_PERSON",
    [MARIO_ANIM_IDLE_HEAD_LEFT] = "CHAR_ANIM_IDLE_HEAD_LEFT",
    [MARIO_ANIM_IDLE_HEAD_RIGHT] = "CHAR_ANIM_IDLE_HEAD_RIGHT",
    [MARIO_ANIM_IDLE_HEAD_CENTER] = "CHAR_ANIM_IDLE_HEAD_CENTER",
    [MARIO_ANIM_HANDSTAND_LEFT] = "CHAR_ANIM_HANDSTAND_LEFT",
    [MARIO_ANIM_HANDSTAND_RIGHT] = "CHAR_ANIM_HANDSTAND_RIGHT",
    [MARIO_ANIM_WAKE_FROM_SLEEP] = "CHAR_ANIM_WAKE_FROM_SLEEP",
    [MARIO_ANIM_WAKE_FROM_LYING] = "CHAR_ANIM_WAKE_FROM_LYING",
    [MARIO_ANIM_START_TIPTOE] = "CHAR_ANIM_START_TIPTOE",
    [MARIO_ANIM_SLIDEJUMP] = "CHAR_ANIM_SLIDEJUMP",
    [MARIO_ANIM_START_WALLKICK] = "CHAR_ANIM_START_WALLKICK",
    [MARIO_ANIM_STAR_DANCE] = "CHAR_ANIM_STAR_DANCE",
    [MARIO_ANIM_RETURN_FROM_STAR_DANCE] = "CHAR_ANIM_RETURN_FROM_STAR_DANCE",
    [MARIO_ANIM_FORWARD_SPINNING_FLIP] = "CHAR_ANIM_FORWARD_SPINNING_FLIP",
    [MARIO_ANIM_TRIPLE_JUMP_FLY] = "CHAR_ANIM_TRIPLE_JUMP_FLY",
};

static const char *CS_MARIO_ACTORS[] = {
    "mario_geo",
    "marios_cap_geo",
    "marios_wing_cap_geo",
    "marios_metal_cap_geo",
    "marios_winged_metal_cap_geo",
};

//
// Types
//

typedef std::string String;
template <typename T> using Array = std::vector<T>;
template <typename K, typename V> using Map = std::map<K, V>;

struct TableValue;
typedef Map<String, struct TableValue> Table;

typedef struct TableValue {
    bool is_table;
    union {
        const char *as_string;
        Table *as_table;
    };
} TableValue;

typedef Array<String> SplitString;

typedef struct CsEntry {
    SplitString model_id;
    SplitString properties[array_length(CS_PROPERTIES)]; // name, caps, palette, voices, anims
} CsEntry;

typedef struct CsIterator {
    String identifier;
    const Table *table;
} CsIterator;

typedef struct CsPack {
    String pack_folder;
    String pack_name;
    String char_name;
    String model_geo;
    String caps_geo[array_length(CS_CAP_TYPES)];
    u32 *palette[array_length(CS_PALETTE_COLORS)] = {0};
    Array<String> voices[array_length(CS_CHAR_SOUNDS)];
    String animations[array_length(CS_CHAR_ANIMS)];
    const char **actor_names[1 + array_length(CS_CAP_TYPES)] = {0};
} CsPack;

typedef struct CsAnimation {
    String pack_folder;
    String anim_name;
    Animation *animation;
} CsAnimation;

typedef struct CsState {
    String pack_folder;
    String buffer;
    CsIterator iterator;
    Array<CsEntry> cs_entries[2]; // full, incomplete
    Array<const char *> strings;
    Map<String, Table *> tables;
    Array<String> anims_tables;
    Map<String, u32> names_counter;
} CsState;

//
// State
//

#define static_def(_T_, _P_, _R_, name) OMM_INLINE _T_ _P_ get_##name() { static _T_ name##_; return _R_ name##_; }
#define static_get(name) get_##name()

static_def(CsState, *, &, sCsState);
#define sCsState static_get(sCsState)

static_def(Array<CsPack *>, &, , sCsPacks);
#define sCsPacks static_get(sCsPacks)

static_def(Array<CsAnimation *>, &, , sCsAnimations);
#define sCsAnimations static_get(sCsAnimations)

static void omm_models_cs_state_clear_table(Table *table) {
    for (auto &kv : *table) {
        if (kv.second.is_table) {
            omm_models_cs_state_clear_table(kv.second.as_table);
        } else {
            mem_del(kv.second.as_string);
        }
    }
    delete table;
}

static void omm_models_cs_state_clear() {
    sCsState->pack_folder.clear();
    sCsState->buffer.clear();
    sCsState->iterator.identifier.clear();
    sCsState->iterator.table = NULL;
    for (auto &s : sCsState->strings) {
        mem_del(s);
    }
    sCsState->strings.clear();
    for (auto &kv : sCsState->tables) {
        omm_models_cs_state_clear_table(kv.second);
    }
    sCsState->tables.clear();
    sCsState->anims_tables.clear();
    sCsState->names_counter.clear();
}

//
// Utils
//

OMM_INLINE bool is_valid_char_for_variable_name(char c) {
    return (
        (c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        (c == '_')
    );
}

OMM_INLINE bool is_quote(char c) {
    return c == '\'' || c == '\"';
}

OMM_INLINE bool is_whitespace(char c) {
    return (u8) c <= (u8) ' ';
}

OMM_INLINE char get_quote(char quote, char c) {
    if (is_quote(c)) {
        if (!quote) return c; // Entering quoted string
        if (quote == c) return 0; // Exiting quoted string
    }
    return quote;
}

//
// String stuff
//

template <typename ...Args>
static String string_format(const String &format, Args ...args) {
    s32 len = snprintf(NULL, 0, format.c_str(), args...) + 1;
    char buffer[len];
    snprintf(buffer, len, format.c_str(), args...);
    return String(buffer);
}

static void string_replace_all(String &string, const String &from, const String &to) {
    size_t i = 0;
    while((i = string.find(from, i)) != String::npos) {
        string.replace(i, from.length(), to);
        i += to.length();
    }
}

static String string_lower(const String &string) {
    String lower;
    for (const auto &c : string) {
        lower += tolower(c);
    }
    return lower;
}

static SplitString string_split(const String &string, const char *delimiter) {
    SplitString split_string;
    size_t start = 0;
    size_t end = 0;
    size_t len = strlen(delimiter);
    do {
        end = string.find(delimiter, start);
        String substring = string.substr(start, end - start);
        if (!substring.empty()) {
            split_string.push_back(substring);
        }
        start = end + len;
    } while (end != String::npos);
    return split_string;
}

#define starts_with(substr) \
    rfind(substr, 0) == 0

static String split_string_concatenate(const SplitString &split_string, const char *delimiter) {
    if (!split_string.empty()) {
        String string = split_string[0];
        for (s32 i = 1; i != split_string.size(); ++i) {
            string += delimiter;
            string += split_string[i];
        }
        return string;
    }
    return "";
}

//
// Array and Map stuff
//

template <typename T>
static T *array_get(Array<T> &array, s32 index) {
    if (index >= array.size()) {
        return NULL;
    }
    return &array.at(index);
}

template <typename T>
static const T *array_get(const Array<T> &array, s32 index) {
    if (index >= array.size()) {
        return NULL;
    }
    return &array.at(index);
}

template <typename K, typename V>
static V *map_get(Map<K, V> &map, const K &key) {
    if (!map.count(key)) {
        return NULL;
    }
    return &map.at(key);
}

template <typename K, typename V>
static const V *map_get(const Map<K, V> &map, const K &key) {
    if (!map.count(key)) {
        return NULL;
    }
    return &map.at(key);
}

//
// Table stuff
//

static void table_insert(Table &table, const String &key, Table *value) {
    table[key].is_table = true;
    table[key].as_table = value;
}

static void table_insert(Table &table, const String &key, const char *value) {
    if (strcmp(value, "nil") != 0) {
        table[key].is_table = false;
        table[key].as_string = str_dup(value);
    }
}

// static void table_print(const Table &table, s32 indent = 0) {
//     omm_printf("{\n");
//     for (auto const &kv : table) {
//         omm_printf("%*s%s = ",, indent + 2, "", kv.first.c_str());
//         if (kv.second.is_table) {
//             table_print(*kv.second.as_table, indent + 2);
//         } else {
//             omm_printf("%s",, kv.second.as_string);
//         }
//         omm_printf(",\n");
//     }
//     omm_printf("%*s}",, indent, "");
// }

//
// Placeholders
//

#define CONCAT "#"
#define ITER__ "iter__"
#define STR_ID "STR@"
#define TBL_ID "TBL@"

static String str_id_to_string(s32 index) {
    return string_format(STR_ID "%04d", index);
}

static s32 str_id_from_string(const String &str_id) {
    if (str_id.length() == sizeof(STR_ID) + 3 && str_id.starts_with(STR_ID)) {
        s32 index;
        if (sscanf(str_id.c_str() + sizeof(STR_ID) - 1, "%04d", &index) == 1) {
            return index;
        }
    }
    return -1;
}

static String tbl_id_to_string(const Table *table) {
    return string_format(TBL_ID "%016llX", (u64) (uintptr_t) table);
}

static const Table *tbl_id_from_string(const String &tbl_id) {
    if (tbl_id.length() == sizeof(TBL_ID) + 15 && tbl_id.starts_with(TBL_ID)) {
        u64 ptr;
        if (sscanf(tbl_id.c_str() + sizeof(TBL_ID) - 1, "%016llX", &ptr) == 1) {
            return (const Table *) ((uintptr_t) ptr);
        }
    }
    return NULL;
}

//
// Finders
//

static bool read_number(const char *head, u32 &number) {
    while (is_whitespace(*head)) { head++; }
    return (
        memcmp(head, "0x", 2) == 0 ?
        sscanf(head + 2, "%X", &number) :
        sscanf(head, "%u", &number)
    ) == 1;
}

static Array<String> extract_function_args(const char *head) {
    Array<String> args;
    String arg;
    for (s32 scope = 0; *head && scope >= 0; head++) {
        char c = *head;
        scope += (c == '(' || c == '{') - (c == ')' || c == '}');
        if (scope < 0 || (scope == 0 && c == ',')) {
            if (!arg.empty()) {
                args.push_back(arg);
                arg.clear();
            }
        } else if (!is_whitespace(c)) {
            arg += c;
        }
    }
    return args;
}

static const char *get_variable_value_start(const char *buffer, const String &var_name) {

    // Check if it's a local variable
    String pattern = string_format("local%s=", var_name.c_str());
    const char *var = strstr(buffer, pattern.c_str());
    if (var) {
        return var + pattern.length();
    }

    // Check if it's a local variable defined outside of the current file
    var = strstr(sCsState->buffer.c_str(), pattern.c_str());
    if (var) {
        return var + pattern.length();
    }

    // Check if it's a global variable
    var = strstr(buffer, pattern.c_str() + 5);
    if (var) {
        return var + pattern.length() - 5;
    }

    // Check if it's a global variable defined outside of the current file
    var = strstr(sCsState->buffer.c_str(), pattern.c_str() + 5);
    if (var) {
        return var + pattern.length() - 5;
    }

    // Not found
    return NULL;
}

static String get_variable_value(const char *var) {
    String value;
    for (; *var; var++) {
        char c = *var;
        if (is_whitespace(c) || c == ',' || c == ')') {
            break;
        }
        if (c == '(') {
            value.clear();
        } else {
            value += c;
        }
    }
    return value;
}

static const char *get_string(const String &str_id) {
    s32 index = str_id_from_string(str_id);
    if (index >= 0 && index < sCsState->strings.size()) {
        return sCsState->strings[index];
    }
    return NULL;
}

static const Table *get_table(const String &table_name) {
    const Table *const *table = map_get(sCsState->tables, table_name);
    return table ? *table : tbl_id_from_string(table_name);
}

static const TableValue *get_table_value(const Table *table, const String &key) {

    // Try to find the key
    const TableValue *v = map_get(*table, key);
    if (v) {
        return v;
    }

    // If the key is an STR_ID, retrieve the string
    if (key.starts_with(STR_ID)) {
        const char *str = get_string(key);
        if (!str) {
            omm_models_cs_error("Cannot find string with id: `%s`", key.c_str());
            return NULL;
        }
        return map_get(*table, String(str));
    }

    // Try to turn the string into an STR_ID
    for (u32 index = 0; index != sCsState->strings.size(); ++index) {
        if (strcmp(sCsState->strings[index], key.c_str()) == 0) {
            String str_id = str_id_to_string(index);
            return map_get(*table, str_id);
        }
    }

    // Not found
    return NULL;
}

//
// Hacks
//

static String resolve_string_and_assemble(const char *buffer, const SplitString &split_string);

static bool try_to_find_name_in_iterator_table(const char *buffer, String& char_name) {
    if (sCsState->iterator.table) {
        for_each_until_null(const char *, name, CS_PROPERTY_COMMON_NAMES[0]) {
            const TableValue *value = get_table_value(sCsState->iterator.table, String(*name));
            if (value && !value->is_table) {
                char_name = resolve_string_and_assemble(buffer, { value->as_string });
                return !char_name.empty();
            }
        }
    }
    return false;
}

static const Table *try_to_find_property_table_in_iterator_table(s32 property_index) {
    if (sCsState->iterator.table) {
        for_each_until_null(const char *, name, CS_PROPERTY_COMMON_NAMES[property_index]) {
            const TableValue *value = get_table_value(sCsState->iterator.table, String(*name));
            if (value && value->is_table) {
                return value->as_table;
            }
        }
    }
    return NULL;
}

//
// Resolvers
//

static String assemble_string(const SplitString &split_string) {
    SplitString result;
    for (const auto &str_id : split_string) {
        const char *str = get_string(str_id);
        if (!str) {
            omm_models_cs_error("Cannot find string with id: `%s`", str_id.c_str());
            return "";
        }
        result.push_back(str);
    }
    return split_string_concatenate(result, "");
}

static bool resolve_table(String &string, SplitString &split_string, s32 index) {

    // If the next string is a valid key, replace the current string by the value and remove the next string
    if (index + 1 < split_string.size()) {
        String &key = split_string[index + 1];
        const Table *table = get_table(string);
        if (table && table->count(key)) {
            const TableValue &value = table->at(key);
            if (value.is_table) {
                string = tbl_id_to_string(value.as_table);
            } else {
                string = value.as_string;
            }
            key.clear();
            return true;
        }
    }

    return false;
}

static String resolve_sub_tables(const Table *table, const String &str) {
    SplitString keys = string_split(str, ".");
    String result;
    for (const String &key : keys) {
        if (!table) {
            return "";
        }
        const TableValue *value = get_table_value(table, key);
        if (!value) {
            return "";
        }
        if (value->is_table) {
            table = value->as_table;
        } else {
            result = value->as_string;
            table = NULL;
        }
    }
    if (table) {
        return tbl_id_to_string(table);
    }
    return result;
}

static bool resolve_string(const char *buffer, SplitString &split_string, s32 index) {
    String &string = split_string[index];

    // Check if it's an empty string
    if (string.empty()) {
        return true;
    }

    // Check if it's nil
    if (string == "nil") {
        string.clear();
        return true;
    }

    // Check if it's a string
    if (string.starts_with(STR_ID)) {
        return false;
    }

    // Check if it's a table pointer
    if (string.starts_with(TBL_ID)) {
        return resolve_table(string, split_string, index);
    }

    // Check if it's a table name
    if (sCsState->tables.count(string)) {
        return resolve_table(string, split_string, index);
    }

    // Check if it's an iterator
    size_t i = string.find(".");
    if (i != String::npos) {
        if (!sCsState->iterator.identifier.empty()) {
            if (!(string.starts_with(sCsState->iterator.identifier + "."))) {
                omm_models_cs_error("Iterators don't match: `%s`, should be `%s`", string.substr(0, i).c_str(), sCsState->iterator.identifier.c_str());
                return false;
            }
            String key = string.substr(i + 1);
            string = resolve_sub_tables(sCsState->iterator.table, key);
            return true;
        }
        return false;
    }

    // Check if it's a variable
    const char *var = get_variable_value_start(buffer, string);
    if (var) {
        string = get_variable_value(var);
        return true;
    }

    // Not found
    omm_models_cs_error("Cannot resolve name: `%s`", string.c_str());
    return false;
}

static void resolve_property(const char *buffer, SplitString &property) {
    for (bool done = false; !done;) {
        done = true;

        // First, split strings
        SplitString result;
        for (auto &string : property) {

            // Iterator
            size_t i = string.find("[i].");
            if (i != String::npos) {
                String iter_name = ITER__ + string.substr(0, i);
                string = iter_name + string.substr(i + 3);
            }

            string_replace_all(string, "[", CONCAT);
            string_replace_all(string, "]", CONCAT);
            SplitString split_string = string_split(string, CONCAT);
            result.insert(
                result.end(),
                std::make_move_iterator(split_string.begin()),
                std::make_move_iterator(split_string.end())
            );
        }
        if (property.size() != result.size()) {
            property = result;
            done = false;
        }

        // Second, resolve strings
        for (s32 i = 0; i != property.size(); ++i) {
            if (resolve_string(buffer, property, i)) {
                done = false;
            }
        }
    }
}

static SplitString resolve_split_string(const char *buffer, const SplitString &split_string) {
    SplitString new_split_string = split_string;
    resolve_property(buffer, new_split_string);
    return new_split_string;
}

static String resolve_string_and_assemble(const char *buffer, const SplitString &split_string) {
    return assemble_string(resolve_split_string(buffer, split_string));
}

static String resolve_table_name(const char *buffer, const SplitString &split_string) {
    return split_string_concatenate(resolve_split_string(buffer, split_string), "");
}

template <typename T, typename Func>
static void resolve_table_and_fill_array(
    const char *buffer,
    const SplitString &property,
    s32 property_index,
    const char **keys,
    s32 num_keys,
    T *array,
    Func func
) {
    String table_name = resolve_table_name(buffer, property);
    const Table *table = get_table(table_name);
    if (!table && property_index != 0) {
        table = try_to_find_property_table_in_iterator_table(property_index);
    }
    if (table) {
        for (s32 i = 0; i != num_keys; ++i) {
            String key = String(keys[i]);
            const TableValue *v = get_table_value(table, key);
            if (v) {
                func(buffer, key, array[i], v);
            }
        }
    }
}

//
// Scan tables
//

static String omm_models_cs_tables_get_name(const char *head) {
    const char *end = head--;
    for (; is_valid_char_for_variable_name(*head); head--);
    return String(head + 1, end);
}

template <typename T>
static void omm_models_cs_tables_insert(Table &table, String &key, s32 &index, T value) {
    if (!key.empty()) {
        table_insert(table, key, value);
    } else {
        table_insert(table, string_format("%d", index++), value);
    }
    key.clear();
}

static Table *omm_models_cs_tables_create(const char *&head) {
    Table *table = new Table;
    String key, value;
    s32 index = 1;
    for (; *head && *head != '}'; head++) {
        char c = *head;
        if (!is_whitespace(c)) {
            switch (c) {

                // Sub-table
                case '{': {
                    omm_models_cs_tables_insert(*table, key, index, omm_models_cs_tables_create(++head));
                    value.clear();
                } break;

                // Table entry has key
                case '=': {
                    key = value;
                    value.clear();
                } break;

                // New key-value pair
                case ',': {
                    if (!value.empty()) {
                        omm_models_cs_tables_insert(*table, key, index, value.c_str());
                        value.clear();
                    }
                } break;

                // Function call: we don't need this
                case '(': {
                    value.clear();
                } break;

                // Skip these
                case ')': break;
                case '[': break;
                case ']': break;

                // Value char
                default: {
                    value += c;
                } break;
            }
        }
    }

    // Insert last element
    if (!value.empty()) {
        omm_models_cs_tables_insert(*table, key, index, value.c_str());
    }

    head++;
    return table;
}

static void omm_models_cs_tables_find(const char *head) {
    while ((head = strstr(head, "={")) != NULL) {
        String table_name = omm_models_cs_tables_get_name(head);
        if (!table_name.empty()) {
            if (table_name.starts_with("local")) {
                table_name = table_name.substr(5);
            }
            head += 2;
            Table *table = omm_models_cs_tables_create(head);
            if (table) {
                sCsState->tables[table_name] = table;
            }
        } else {
            head++;
        }
    }
}

//
// Find entries
//

static CsEntry *omm_models_cs_entries_get_from_model_id(const String &model_id) {
    for (auto &cs_entries : sCsState->cs_entries) {
        for (auto &cs_entry : cs_entries) {
            if (cs_entry.model_id[0] == model_id) {
                return &cs_entry;
            }
        }
    }
    return NULL;
}

static void omm_models_cs_entries_look_for_function(const char *head, const CsFunction *cs_function) {
    while ((head = strstr(++head, cs_function->function)) != NULL) {
        Array<String> args = extract_function_args(strchr(head, '(') + 1);

        // Model id
        const String *model_id = array_get(args, cs_function->model_id_arg);
        if (!model_id) {
            omm_models_cs_error("Cannot retrieve model id from function `%s`", cs_function->function);
            continue;
        }
        if (*model_id == "nil") {
            continue;
        }

        // Property
        const String *property = array_get(args, cs_function->property_arg);
        if (!property) {
            omm_models_cs_error("Cannot retrieve property for model id `%s`", model_id->c_str());
            continue;
        }
        if (*property == "nil") {
            continue;
        }

        // Create new entry or add property to existing
        CsEntry *cs_entry = omm_models_cs_entries_get_from_model_id(*model_id);
        if (!cs_entry) {
            sCsState->cs_entries[cs_function->property_index != 0].push_back({ .model_id = SplitString({ *model_id }) });
            cs_entry = omm_models_cs_entries_get_from_model_id(*model_id);
        }
        if (cs_entry->properties[cs_function->property_index].empty()) {
            cs_entry->properties[cs_function->property_index] = SplitString({ *property });
        } else {
            omm_models_cs_warning("Entry for model id `%s` already exists", model_id->c_str());
        }
    }
}

static void omm_models_cs_entries_find(const char *buffer) {

    // Find all entries
    for (s32 i = 0; i != array_length(CS_FUNCTIONS); ++i) {
        omm_models_cs_entries_look_for_function(buffer, CS_FUNCTIONS + i);
    }

    // Resolve properties
    for (auto &cs_entries : sCsState->cs_entries) {
        for (auto &cs_entry : cs_entries) {
            resolve_property(buffer, cs_entry.model_id);
            for (s32 i = 0; i != array_length(CS_PROPERTIES); ++i) {
                resolve_property(buffer, cs_entry.properties[i]);
            }
        }
    }

    // Remove entries that don't have a name (property 0)
    auto &cs_entries = sCsState->cs_entries[0];
    for (s32 i = cs_entries.size(); i != 0; --i) {
        if (cs_entries[i - 1].properties[0].empty()) {
            cs_entries.erase(cs_entries.begin() + (i - 1));
        }
    }

    // Print CS entries
    for (u32 i = 0; i != array_length(sCsState->cs_entries); ++i) {
        const auto &cs_entries = sCsState->cs_entries[i];
        for (auto &cs_entry : cs_entries) {
            omm_models_cs_print("\033[%s%s", (i ? "34m" : "36m"), "cs_entry: {");
            omm_models_cs_print("  model_id: %s", split_string_concatenate(cs_entry.model_id, "+").c_str());
            for (s32 i = 0; i != array_length(CS_PROPERTIES); ++i) {
                omm_models_cs_print("  %s: %s", CS_PROPERTIES[i], split_string_concatenate(cs_entry.properties[i], "+").c_str());
            }
            omm_models_cs_print("%s\033[0m", "}");
        }
    }
}

static void omm_models_cs_entries_fill_properties(CsEntry &cs_entry, const SplitString &model_id, const CsEntry &other_entry, const SplitString &other_model_id) {
    if (std::equal(model_id.begin(), model_id.end(), other_model_id.begin(), other_model_id.end())) {
        for (s32 i = 0; i != array_length(CS_PROPERTIES); ++i) {
            if (cs_entry.properties[i].empty()) {
                cs_entry.properties[i] = other_entry.properties[i];
            }
        }
    }
}

static String omm_models_cs_packs_get_iterator(const SplitString &model_id);
static const Table *omm_models_cs_packs_get_iterator_table(const char *buffer, const String &iterator);

static void omm_models_cs_entries_fill(const char *buffer, CsEntry &cs_entry, const SplitString &model_id) {
    for (const auto &other_entry : sCsState->cs_entries[1]) {
        String iterator = omm_models_cs_packs_get_iterator(other_entry.model_id);
        if (!iterator.empty()) {
            const Table *table = omm_models_cs_packs_get_iterator_table(buffer, iterator);
            if (!table) {
                omm_models_cs_error("Cannot find table associated to iterator `%s`", iterator.c_str());
                return;
            }
            for (const auto &kv : *table) {
                if (kv.second.is_table) {
                    String iterator_identifier = sCsState->iterator.identifier;
                    const Table *iterator_table = sCsState->iterator.table;
                    sCsState->iterator.identifier = iterator;
                    sCsState->iterator.table = kv.second.as_table;
                    SplitString other_model_id = resolve_split_string(buffer, other_entry.model_id);
                    sCsState->iterator.identifier = iterator_identifier;
                    sCsState->iterator.table = iterator_table;
                    omm_models_cs_entries_fill_properties(cs_entry, model_id, other_entry, other_model_id);
                }
            }
        } else {
            omm_models_cs_entries_fill_properties(cs_entry, model_id, other_entry, other_entry.model_id);
        }
    }
}

static void omm_models_cs_entries_clear() {
    sCsState->cs_entries[0].clear();
    sCsState->cs_entries[1].clear();
}

//
// Animations
//

static const CsAnimation *omm_models_cs_animations_get(const String &pack_folder, const String &anim_name) {
    for (const auto &animation : sCsAnimations) {
        if (animation->anim_name == anim_name && animation->pack_folder == pack_folder) {
            return animation;
        }
    }
    return NULL;
}

static void omm_models_cs_animations_find_tables(const char *buffer) {
    const char *head = buffer - 1;
    while ((head = strstr(++head, "smlua_anim_util_set_animation")) != NULL) {
        Array<String> args = extract_function_args(strchr(head, '(') + 1);
        if (args.size() >= 2) {
            SplitString anims_table_name = { args[1] };
            resolve_property(buffer, anims_table_name);
            if (anims_table_name.size() != 0 && get_table(anims_table_name[0])) {
                sCsState->anims_tables.push_back(anims_table_name[0]);
                omm_models_cs_print("Found animations table: `%s`", anims_table_name[0].c_str());
            }
        }
    }
}

static bool omm_models_cs_animations_read_values(const char *buffer, const String &arg, Array<u32> &values) {

    // Raw table
    if (arg.starts_with("{")) {
        SplitString tokens = string_split(arg.substr(1, arg.length() - 2), ",");
        for (const auto &token : tokens) {
            u32 value;
            if (!token.empty()) {
                if (!read_number(token.c_str(), value)) {
                    omm_models_cs_error("`%s` is not a valid number for animation values", token.c_str());
                    return false;
                }
                values.push_back(value);
            }
        }
        return true;
    }

    // Table name
    String table_name = resolve_table_name(buffer, { arg });
    const Table *table = get_table(table_name);
    if (table) {
        for (const auto &kv : *table) {
            const TableValue &table_value = kv.second;
            if (table_value.is_table) {
                omm_models_cs_error("%s", "Field in animation values table cannot be of type Table");
                return false;
            }
            u32 value;
            if (!read_number(table_value.as_string, value)) {
                omm_models_cs_error("`%s` is not a valid number for animation values", table_value.as_string);
                return false;
            }
            values.push_back(value);
        }
        return true;
    }
    return false;
}

static void omm_models_cs_animations_find(const char *buffer) {
    const char *head = buffer - 1;
    while ((head = strstr(++head, "smlua_anim_util_register_animation")) != NULL) {
        Array<String> args = extract_function_args(strchr(head, '(') + 1);
        if (args.size() >= 8) {

            String anim_name = resolve_string_and_assemble(buffer, { args[0] });
            if (anim_name.empty()) {
                omm_models_cs_error("Cannot retrieve animation name from: `%s`", args[0].c_str());
                continue;
            }

            if (omm_models_cs_animations_get(sCsState->pack_folder, anim_name)) {
                omm_models_cs_warning("Animation \"%s\" already exists", anim_name.c_str());
                continue;
            }

            u32 flags;
            if (!read_number(args[1].c_str(), flags)) {
                omm_models_cs_error("Cannot retrieve animation \"%s\" flags from: `%s`", anim_name.c_str(), args[1].c_str());
                continue;
            }

            u32 start_frame;
            if (!read_number(args[3].c_str(), start_frame)) {
                omm_models_cs_error("Cannot retrieve animation \"%s\" start frame from: `%s`", anim_name.c_str(), args[3].c_str());
                continue;
            }

            u32 loop_start;
            if (!read_number(args[4].c_str(), loop_start)) {
                omm_models_cs_error("Cannot retrieve animation \"%s\" loop start from: `%s`", anim_name.c_str(), args[4].c_str());
                continue;
            }

            u32 loop_end;
            if (!read_number(args[5].c_str(), loop_end)) {
                omm_models_cs_error("Cannot retrieve animation \"%s\" loop end from: `%s`", anim_name.c_str(), args[5].c_str());
                continue;
            }

            Array<u32> values;
            if (!omm_models_cs_animations_read_values(buffer, args[6], values)) {
                omm_models_cs_error("Cannot retrieve animation \"%s\" values", anim_name.c_str());
                continue;
            }

            Array<u32> index;
            if (!omm_models_cs_animations_read_values(buffer, args[7], index)) {
                omm_models_cs_error("Cannot retrieve animation \"%s\" index", anim_name.c_str());
                continue;
            }

            s16 *anim_values = mem_new(s16, values.size());
            for (u32 i = 0; i != values.size(); ++i) {
                anim_values[i] = (s16) values[i];
            }

            u16 *anim_index = mem_new(u16, index.size());
            for (u32 i = 0; i != index.size(); ++i) {
                anim_index[i] = (u16) index[i];
            }

            CsAnimation *animation = new CsAnimation;
            animation->anim_name = anim_name;
            animation->pack_folder = sCsState->pack_folder;
            animation->animation = mem_new(Animation, 1);
            animation->animation->flags = (s16) flags;
            animation->animation->mStartFrame = (s16) start_frame;
            animation->animation->mLoopStart = (s16) loop_start;
            animation->animation->mLoopEnd = (s16) loop_end;
            animation->animation->values = anim_values;
            animation->animation->index = anim_index;
            animation->animation->length = ANIM_LENGTH(index.size(), values.size());
            sCsAnimations.push_back(animation);
            omm_models_cs_print("Found animation: \"%s\"", anim_name.c_str());
        }
    }
}

//
// Create packs
//

static String omm_models_cs_packs_get_iterator(const SplitString &model_id) {
    for (const auto &string : model_id) {
        size_t i = string.find('.');
        if (i != String::npos) {
            return string.substr(0, i);
        }
    }
    return "";
}

static const Table *omm_models_cs_packs_get_iterator_table(const char *buffer, const String &iterator) {

    // Check if it's a known iterator
    if (iterator.starts_with(ITER__)) {
        return get_table(iterator.substr(sizeof(ITER__) - 1));
    }

    // Check if it's a known table
    // Hack: a known table is probably not intended to be iterable,
    // so treat it like a sub-table of a table
    const Table *table = get_table(iterator);
    if (table) {
        static Table sSingleTable;
        sSingleTable["1"] = { .is_table = true, .as_table = (Table *) table };
        return &sSingleTable;
    }

    // Check if it's a variable
    const char *var = get_variable_value_start(buffer, iterator);
    if (var) {
        String table_name = get_variable_value(var);
        return get_table(table_name.substr(0, table_name.find('[')));
    }

    // Check if it's an "ipairs" variable
    String pattern = string_format("%sinipairs(", iterator.c_str());
    const char *var_begin = strstr(buffer, pattern.c_str());
    if (var_begin) {
        var_begin += pattern.length();
        const char *var_end = strchr(var_begin, ')');
        if (var_end) {
            return get_table(String(var_begin, var_end));
        }
    }

    // Not found
    return NULL;
}

static void omm_models_cs_packs_create_single(const char *buffer, CsEntry cs_entry) {

    // Fill missing fields of a CS entry from incomplete entries
    SplitString model_id = resolve_split_string(buffer, cs_entry.model_id);
    omm_models_cs_entries_fill(buffer, cs_entry, model_id);

    // Model geo
    String model_geo = assemble_string(model_id);
    if (model_geo.empty()) {
        omm_models_cs_error("Cannot retrieve model geo from: `%s`", split_string_concatenate(cs_entry.model_id, "+").c_str());
        return;
    }

    // Character name
    String char_name = resolve_string_and_assemble(buffer, cs_entry.properties[0]);
    if (char_name.empty() && !try_to_find_name_in_iterator_table(buffer, char_name)) {
        omm_models_cs_error("Cannot retrieve character name from: `%s`", split_string_concatenate(cs_entry.properties[0], "+").c_str());
        return;
    }

    // Check if a CS pack already exists
    for (const auto &cs_pack : sCsPacks) {
        if (cs_pack->char_name == char_name) {

            // Reject if the name and geo are the same
            if (cs_pack->model_geo == model_geo) {
                omm_models_cs_warning("CS Pack \"%s\" already exists", char_name.c_str());
                return;
            }

            // If the geo are different, add an index next to the char name
            if (sCsState->names_counter.count(char_name)) {
                sCsState->names_counter[char_name]++;
            } else {
                sCsState->names_counter[char_name] = 1;
            }
            char_name += string_format(" (%d)", sCsState->names_counter[char_name] + 1);
        }
    }

    // Create new CS pack
    CsPack *cs_pack = new CsPack;
    sCsPacks.push_back(cs_pack);
    cs_pack->pack_folder = sCsState->pack_folder;
    cs_pack->pack_name = "(CS) " + char_name;
    cs_pack->char_name = char_name;
    cs_pack->model_geo = model_geo;
    string_replace_all(cs_pack->pack_name, "[", "(");
    string_replace_all(cs_pack->pack_name, "]", ")");
    string_replace_all(cs_pack->pack_name, "_", " ");

    // Caps geo
    resolve_table_and_fill_array(buffer, cs_entry.properties[1], 1, CS_CAP_TYPES, array_length(CS_CAP_TYPES), cs_pack->caps_geo,
        [] (const char *buffer, const String &cap_type, String &cap_geo, const TableValue *v) {
            if (v->is_table) {
                omm_models_cs_error("Field `%s` in cap table cannot be of type Table", cap_type.c_str());
                return;
            }
            cap_geo = resolve_string_and_assemble(buffer, string_split(v->as_string, CONCAT));
        }
    );

    // Fill missing caps
    for (s32 i = 0; i != array_length(CS_CAP_TYPES); ++i) {
        String &cap_geo = cs_pack->caps_geo[i];
        if (cap_geo.empty()) {
            for (s32 j = 0; j != 3; ++j) {
                s32 k = CS_CAP_DEFAULTS[i][j];
                const String &cap_default = cs_pack->caps_geo[k];
                if (!cap_default.empty()) {
                    omm_models_cs_warning("Missing `%s` cap geo, using `%s` cap geo", CS_CAP_TYPES[i], CS_CAP_TYPES[k]);
                    cap_geo = cap_default;
                    break;
                }
            }
        }
    }

    // Palette
    resolve_table_and_fill_array(buffer, cs_entry.properties[2], 2, CS_PALETTE_COLORS, array_length(CS_PALETTE_COLORS), cs_pack->palette,
        [] (const char *buffer, const String &color_part, u32 *&color, const TableValue *v) {
            u32 color_value = 0;
            if (v->is_table) {
                const Table *table_rgb = v->as_table;
                for_each_in_(const char *, channel, { "r", "g", "b" }) {
                    if (!table_rgb->count(*channel)) {
                        omm_models_cs_error("Field `%s` in RGB table `%s` is missing", *channel, color_part.c_str());
                        return;
                    }
                    const TableValue &value = table_rgb->at(*channel);
                    if (value.is_table) {
                        omm_models_cs_error("Field `%s` in RGB table `%s` cannot be of type Table", *channel, color_part.c_str());
                        return;
                    }
                    u32 rgb;
                    if (!read_number(value.as_string, rgb)) {
                        omm_models_cs_error("`%s` is not a valid number for field `%s` in RGB table `%s`", *channel, color_part.c_str(), value.as_string);
                        return;
                    }
                    color_value = (color_value << 8) | (rgb & 0xFF);
                }
            } else {
                String color_hex = resolve_string_and_assemble(buffer, string_split(v->as_string, CONCAT));
                if (sscanf(color_hex.c_str(), "%06X", &color_value) != 1) {
                    omm_models_cs_error("`%s` is not a valid number for color part `%s`", color_hex.c_str(), color_part.c_str());
                    return;
                }
            }
            color = mem_new(u32, 1);
            *color = color_value;
        }
    );

    // Voices
    resolve_table_and_fill_array(buffer, cs_entry.properties[3], 3, CS_CHAR_SOUNDS, array_length(CS_CHAR_SOUNDS), cs_pack->voices,
        [] (const char *buffer, const String &char_sound, Array<String> &voices, const TableValue *v) {
            if (v->is_table) {
                for (const auto &kv : *v->as_table) {
                    const TableValue &value = kv.second;
                    if (value.is_table) {
                        omm_models_cs_error("Character sound `%s` cannot be of type Table", char_sound.c_str());
                    } else {
                        voices.push_back(resolve_string_and_assemble(buffer, string_split(value.as_string, CONCAT)));
                    }
                }
            } else {
                voices.push_back(resolve_string_and_assemble(buffer, string_split(v->as_string, CONCAT)));
            }
        }
    );

    // Animations
    resolve_table_and_fill_array(buffer, cs_entry.properties[4], 4, CS_CHAR_ANIMS, array_length(CS_CHAR_ANIMS), cs_pack->animations,
        [] (const char *buffer, const String &char_anim, String &animation, const TableValue *v) {
            if (v->is_table) {
                omm_models_cs_error("Field `%s` in animations table cannot be of type Table", char_anim.c_str());
                return;
            }
            animation = resolve_string_and_assemble(buffer, string_split(v->as_string, CONCAT));
        }
    );

    // Try to match animations from the animations tables found previously with the character geo name
    for (const auto &anims_table : sCsState->anims_tables) {
        resolve_table_and_fill_array(buffer, { anims_table }, 0, CS_CHAR_ANIMS, array_length(CS_CHAR_ANIMS), cs_pack->animations,
            [&model_geo] (const char *buffer, const String &char_anim, String &animation, const TableValue *v) {
                if (animation.empty()) {
                    if (v->is_table) {
                        omm_models_cs_error("Field `%s` in animations table cannot be of type Table", char_anim.c_str());
                        return;
                    }
                    String anim_name = resolve_string_and_assemble(buffer, string_split(v->as_string, CONCAT));
                    String name = string_lower(anim_name.substr(0, anim_name.find('_') + 1));
                    if (!name.empty() && string_lower(model_geo).starts_with(name)) {
                        animation = anim_name;
                    }
                }
            }
        );
    }

    // Actor names
    for (u32 i = 0; i != 1 + array_length(CS_CAP_TYPES); ++i) {
        const String &geo = (i == 0 ? cs_pack->model_geo : cs_pack->caps_geo[i - 1]);
        if (!geo.empty()) {
            String actor_name = "actors/" + geo;
            const char **actor_names = mem_new(const char *, 2);
            actor_names[0] = str_dup(actor_name.c_str());
            actor_names[1] = NULL;
            cs_pack->actor_names[i] = actor_names;
        } else {
            cs_pack->actor_names[i] = NULL;
        }
    }

    // Print CS pack
    omm_models_cs_print("\033[32m%s", "cs_pack: {");
    omm_models_cs_print("  model_geo: \"%s\"", cs_pack->model_geo.c_str());
    omm_models_cs_print("  char_name: \"%s\"", cs_pack->char_name.c_str());
    omm_models_cs_print("  caps_geo: %s", "{");
    for (s32 i = 0; i != array_length(CS_CAP_TYPES); ++i) {
        if (!cs_pack->caps_geo[i].empty()) {
            omm_models_cs_print("    %s: \"%s\"", CS_CAP_TYPES[i], cs_pack->caps_geo[i].c_str());
        }
    }
    omm_models_cs_print("  %s", "}");
    omm_models_cs_print("  palette: %s", "{");
    for (s32 i = 0; i != array_length(CS_PALETTE_COLORS); ++i) {
        if (cs_pack->palette[i]) {
            omm_models_cs_print("    %s: %06X", CS_PALETTE_COLORS[i], *cs_pack->palette[i]);
        }
    }
    omm_models_cs_print("  %s", "}");
    omm_models_cs_print("  voices: %s", "{");
    for (s32 i = 0; i != array_length(CS_CHAR_SOUNDS); ++i) {
        if (!cs_pack->voices[i].empty()) {
            if (cs_pack->voices[i].size() > 1) {
                omm_models_cs_print("    %s: {", CS_CHAR_SOUNDS[i]);
                for (const auto &voice : cs_pack->voices[i]) {
                    omm_models_cs_print("      \"%s\"", voice.c_str());
                }
                omm_models_cs_print("    %s", "}");
            } else {
                omm_models_cs_print("    %s: \"%s\"", CS_CHAR_SOUNDS[i], cs_pack->voices[i][0].c_str());
            }
        }
    }
    omm_models_cs_print("  %s", "}");
    omm_models_cs_print("  animations: %s", "{");
    for (s32 i = 0; i != array_length(CS_CHAR_ANIMS); ++i) {
        if (!cs_pack->animations[i].empty()) {
            omm_models_cs_print("    %s: \"%s\"", CS_CHAR_ANIMS[i], cs_pack->animations[i].c_str());
        }
    }
    omm_models_cs_print("  %s", "}");
    omm_models_cs_print("%s\033[0m", "}");
}

static void omm_models_cs_packs_create_multi(const char *buffer, const CsEntry &cs_entry, const String &iterator) {
    const Table *table = omm_models_cs_packs_get_iterator_table(buffer, iterator);
    if (!table) {
        omm_models_cs_error("Cannot find table associated to iterator `%s`", iterator.c_str());
        return;
    }
    for (const auto &kv : *table) {
        if (kv.second.is_table) {
            sCsState->iterator.identifier = iterator;
            sCsState->iterator.table = kv.second.as_table;
            omm_models_cs_packs_create_single(buffer, cs_entry);
        }
    }
    sCsState->iterator.identifier.clear();
    sCsState->iterator.table = NULL;
}

static void omm_models_cs_packs_create(const char *buffer) {
    for (const auto &cs_entry : sCsState->cs_entries[0]) {
        String iterator = omm_models_cs_packs_get_iterator(cs_entry.model_id);
        if (!iterator.empty()) {
            omm_models_cs_packs_create_multi(buffer, cs_entry, iterator);
        } else {
            omm_models_cs_packs_create_single(buffer, cs_entry);
        }
    }
}

//
// Scan files
//

static bool omm_models_cs_file_read(fs_file_t *file, char *data, s32 size) {

    // Read first 4 bytes
    if (fs_read(file, data, 4) < 4) {
        return false;
    }

    // Check if it's a binary lua
    if (data[0] == LUA_MAGIC_0 &&
        data[1] == LUA_MAGIC_1 &&
        data[2] == LUA_MAGIC_2 &&
        data[3] == LUA_MAGIC_3) {
        return false;
    }

    // Read the rest of the file
    if (fs_read(file, data + 4, size - 4) < size - 4) {
        return false;
    }

    return true;
}

static void omm_models_cs_file_remove_comments(char *data) {
    char *begin = data;
    while ((begin = strstr(begin, "--")) != NULL) {

        // Multi-line comment
        if (strstr(begin, "[[") == begin + 2) {
            char *end = strstr(begin, "]]");
            if (!end) {
                *begin = 0;
                break;
            }
            mem_set(begin, ' ', end - begin + 2);
            begin = end + 2;
        }

        // Single line comment
        else {
            char *end = strchr(begin, '\n');
            if (!end) {
                *begin = 0;
                break;
            }
            mem_set(begin, ' ', end - begin);
            begin = end;
        }
    }
}

static s32 omm_models_cs_file_create_str_id(const char *str) {
    for (s32 i = 0; i != sCsState->strings.size(); ++i) {
        if (strcmp(sCsState->strings[i], str) == 0) {
            return i;
        }
    }
    sCsState->strings.push_back(str_dup(str));
    return sCsState->strings.size() - 1;
}

// Remove spaces and tabs, and store strings into an array
static String omm_models_cs_file_simplify(const char *data, s32 size) {
    String buffer;
    char str[1024] = {0};
    for (s32 i = 0, str_i = 0, quote = 0; i < size; ++i) {
        char c = data[i];

        // Get the current quote
        quote = get_quote(quote, c);

        // Inside a quoted string
        if (quote) {
            str[str_i] = c;
            str_i = min_s(str_i + 1, sizeof(str) - 1);
            continue;
        }

        // Exiting a quoted string
        if (!quote && str_i > 0) {
            str[str_i] = 0;
            s32 index = omm_models_cs_file_create_str_id(str + 1);
            buffer += str_id_to_string(index);
            str_i = 0;
            continue;
        }

        // Skip spaces and tabs
        if (!quote && c != ' ' && c != '\t') {

            // Replace concatenation symbol .. by #
            if (c == '.' && data[i + 1] == '.') {
                buffer += CONCAT;
                i++;
            } else {
                buffer += c;
            }
        }
    }
    return buffer;
}

//
// Other
//

static const OmmPackData *omm_models_cs_find_enabled_or_disable_packs(u32 cs_index, bool find) {
    omm_array_for_each(gOmmPackList, p_pack) {
        OmmPackData *pack = (OmmPackData *) p_pack->as_ptr;
        if (pack && pack->enabled && pack->cs_index > 0 && pack->cs_index <= sCsPacks.size() && pack->cs_index != cs_index) {
            if (find) {
                return pack;
            }
            pack->enabled = false;
        }
    }
    return NULL;
}

//
// Main
//

extern "C" {

void omm_models_cs_init(const char *path) {
    const char *sep = strrchr(path, '/');
    if (sep) {
        String pack_folder = String(path, sep);

        // Reset state when scanning a new pack folder
        if (sCsState->pack_folder != pack_folder) {
            omm_models_cs_state_clear();
            sCsState->pack_folder = pack_folder;
            omm_models_cs_print_pack_folder();
        }

        // Open file
        fs_file_t *file = fs_open(path);
        if (file) {
            s32 size = (s32) fs_size(file);
            char *data = mem_new(char, size + 1);

            // Read file, unless it's a binary LUA
            if (!omm_models_cs_file_read(file, data, size)) {
                fs_close(file);
                mem_del(data);
                return;
            }
            fs_close(file);
            omm_models_cs_print_file();

            // Simplify file for parsing
            omm_models_cs_file_remove_comments(data);
            String buffer = omm_models_cs_file_simplify(data, size);
            mem_del(data);
            sCsState->buffer += buffer;

            // Hacky way to detect the original Character Select mod
            if (strstr(buffer.c_str(), "_G.charSelect={")) {
                omm_models_cs_warning("Character Select library detected: Skipping.", "");
                return;
            }

            // if (strstr(path, "main.lua")) {
            //     sys_path_t fn;
            //     str_fmt(fn, sizeof(fn), "%s.lua", sCsState->pack_folder.c_str());
            //     str_rep(fn, sizeof(fn), fn, '/', '_');
            //     FILE *f = fopen(fn, "wb");
            //     if (f) {
            //         fwrite(buffer.c_str(), 1, buffer.length(), f);
            //         fclose(f);
            //     }
            // }

            // Parse the source
            omm_models_cs_tables_find(buffer.c_str());
            omm_models_cs_entries_find(buffer.c_str());
            omm_models_cs_animations_find_tables(buffer.c_str());
            omm_models_cs_animations_find(buffer.c_str());

            // Create CS packs
            omm_models_cs_packs_create(buffer.c_str());

            // Clear stuff
            omm_models_cs_entries_clear();
        }
    }
}

void omm_models_cs_register_packs() {
    omm_models_cs_state_clear();
    for (u32 i = 0; i != sCsPacks.size(); ++i) {
        const auto &cs_pack = sCsPacks[i];

        // Check if the pack is already registered
        OmmPackData *existing_pack = NULL;
        omm_array_for_each(gOmmPackList, p_pack) {
            OmmPackData *pack = (OmmPackData *) p_pack->as_ptr;
            if (pack && cs_pack->pack_name == pack->name) {
                existing_pack = pack;
                break;
            }
        }

        // Create pack
        if (!existing_pack) {
            OmmPackData *new_pack = mem_new(OmmPackData, 1);
            str_cpy(new_pack->name, sizeof(new_pack->name), cs_pack->pack_name.c_str());
            str_cpy(new_pack->path, sizeof(new_pack->path), cs_pack->pack_folder.c_str());
            new_pack->exists = true;
            new_pack->enabled = false;
            new_pack->cs_index = i + 1;
            omm_array_add(gOmmPackList, ptr, new_pack);
            omm_printf("CS pack found: \"%s\"\n",, cs_pack->pack_name.c_str());
        }

        // Load pack
        else if (!existing_pack->exists) {
            str_cpy(existing_pack->path, sizeof(existing_pack->path), cs_pack->pack_folder.c_str());
            existing_pack->exists = true;
            existing_pack->cs_index = i + 1;
            omm_printf("CS pack found: \"%s\"\n",, cs_pack->pack_name.c_str());
        }
    }
}

OmmGfxData *omm_models_cs_load(u32 cs_index, const char *actor_name, u64 *loaded_bytes) {
    if (cs_index > 0 && cs_index <= sCsPacks.size()) {
        const CsPack *cs_pack = sCsPacks[cs_index - 1];
        array_for_each_(const char *, mario_actor, CS_MARIO_ACTORS) {
            if (strcmp(actor_name, *mario_actor) == 0) {
                if (cs_pack->actor_names[i_mario_actor]) {
                    OmmGfxData *gfx_data = omm_models_load_from_binary(cs_pack->pack_folder.c_str(), cs_pack->actor_names[i_mario_actor], loaded_bytes);
                    if (gfx_data) {
                        gfx_data->priority = 0xFF;
                        return gfx_data;
                    }
                }
                return NULL;
            }
        }
    }
    return NULL;
}

void omm_models_cs_load_all(u32 cs_index, u64 *loaded_bytes) {
    if (cs_index > 0 && cs_index <= sCsPacks.size()) {
        const CsPack *cs_pack = sCsPacks[cs_index - 1];

        // Load geo layouts
        array_for_each_(const char *, mario_actor, CS_MARIO_ACTORS) {
            OmmGfxData *gfx_data = omm_models_cs_load(cs_index, *mario_actor, loaded_bytes);
            if (gfx_data) {
                OmmDataNode_GeoLayout *geo_layout_node = (OmmDataNode_GeoLayout *) omm_array_get(gfx_data->geo_layouts, ptr, omm_array_count(gfx_data->geo_layouts) - 1);
                geo_layout_to_graph_node(NULL, geo_layout_node->data);
            }
        }

        // Load voices
        for (const auto &voices : cs_pack->voices) {
            for (const auto &voice : voices) {
                omm_models_cs_sound_load(cs_pack->pack_folder.c_str(), voice.c_str(), loaded_bytes);
            }
        }
    }
}

void omm_models_cs_update_current() {
    static u32 current_cs_index = 0;
    const OmmPackData *enabled = omm_models_cs_find_enabled_or_disable_packs(current_cs_index, true);
    if (enabled) {
        omm_models_cs_find_enabled_or_disable_packs(enabled->cs_index, false);
        current_cs_index = enabled->cs_index;
    }
}

u64 omm_models_cs_get_size(u32 cs_index) {
    if (cs_index > 0 && cs_index <= sCsPacks.size()) {
        const CsPack *cs_pack = sCsPacks[cs_index - 1];
        Map<String, u64> sizes; // Store file sizes in a map so duplicate files are counted only once

        // Geo layouts
        array_for_each_(const char ** const, actor_names, cs_pack->actor_names) {
            if (*actor_names) {
                const char *actor_name = **actor_names;
                sys_path_t filename;
                str_cat(filename, sizeof(filename), cs_pack->pack_folder.c_str(), "/", actor_name, ".bin");
                s64 fsize = fs_fsize(filename);
                if (fsize != -1) {
                    sizes[actor_name] = fsize;
                }
            }
        }

        // Voices
        for (const auto &voices : cs_pack->voices) {
            for (const auto &voice : voices) {
                sys_path_t filename;
                str_cat(filename, sizeof(filename), cs_pack->pack_folder.c_str(), "/sound/", voice.c_str(), ".bin");
                s64 fsize = fs_fsize(filename);
                if (fsize != -1) {
                    sizes[voice] = fsize;
                }
            }
        }

        u64 size = 0;
        for (const auto &kv : sizes) {
            size += kv.second;
        }
        return size;
    }
    return 0;
}

u32 omm_models_cs_get_current_cs_index() {
    s32 actor_index = omm_models_get_actor_index(mario_geo);
    if (actor_index >= 0 && actor_index < omm_array_count(gOmmActorList)) {
        s32 pack_index = ((const OmmActorGfx *) omm_array_get(gOmmActorList, ptr, actor_index))->pack_index;
        if (pack_index >= 0 && pack_index < omm_array_count(gOmmPackList)) {
            return ((const OmmPackData *) omm_array_get(gOmmPackList, ptr, pack_index))->cs_index;
        }
    }
    return 0;
}

const u32 **omm_models_cs_get_palette_preset(u32 cs_index) {
    if (cs_index > 0 && cs_index <= sCsPacks.size()) {
        return (const u32 **) sCsPacks[cs_index - 1]->palette;
    }
    return NULL;
}

Animation *omm_models_cs_get_animation(u32 cs_index, s32 anim_index) {
    if (cs_index > 0 && cs_index <= sCsPacks.size()) {
        const CsPack *cs_pack = sCsPacks[cs_index - 1];
        if (anim_index >= 0 && anim_index < array_length(CS_CHAR_ANIMS)) {
            const String &anim_name = cs_pack->animations[anim_index];
            if (!anim_name.empty()) {
                const CsAnimation *cs_anim = omm_models_cs_animations_get(cs_pack->pack_folder, anim_name);
                if (cs_anim) {
                    return cs_anim->animation;
                }
            }
        }
    }
    return NULL;
}

const char *omm_models_cs_get_pack_folder(u32 cs_index) {
    if (cs_index > 0 && cs_index <= sCsPacks.size()) {
        const CsPack *cs_pack = sCsPacks[cs_index - 1];
        return cs_pack->pack_folder.c_str();
    }
    return NULL;
}

const char *omm_models_cs_get_sound_name(u32 cs_index, s32 char_sound, s32 variant) {
    if (cs_index > 0 && cs_index <= sCsPacks.size()) {
        const CsPack *cs_pack = sCsPacks[cs_index - 1];
        if (char_sound >= 0 && char_sound < array_length(CS_CHAR_SOUNDS)) {
            const Array<String> &voices = cs_pack->voices[char_sound];
            if (!voices.empty()) {
                u32 offset = 0;
                u32 range = voices.size();
                array_for_each_(const CsSoundVariant, sound_variant, CS_CHAR_SOUND_VARIANTS) {
                    if (sound_variant->char_sound == char_sound &&
                        sound_variant->size == range &&
                        sound_variant->variant == variant
                    ) {
                        offset = sound_variant->offset;
                        range = sound_variant->range;
                        break;
                    }
                }
                return voices[offset + (random_u16() % range)].c_str();
            }
        }
    }
    return NULL;
}

}
