#ifndef LEVEL_SCRIPT_H
#define LEVEL_SCRIPT_H

#include "types.h"

#define LEVEL_SCRIPT_CONTINUE               0
#define LEVEL_SCRIPT_NEXT                   1
#define LEVEL_SCRIPT_RETURN                 2
#define LEVEL_SCRIPT_STOP                   3

#define LEVEL_CMD_EXECUTE                   0x00
#define LEVEL_CMD_EXIT_AND_EXECUTE          0x01
#define LEVEL_CMD_EXIT                      0x02
#define LEVEL_CMD_SLEEP                     0x03
#define LEVEL_CMD_SLEEP_BEFORE_EXIT         0x04
#define LEVEL_CMD_JUMP                      0x05
#define LEVEL_CMD_JUMP_LINK                 0x06
#define LEVEL_CMD_RETURN                    0x07
#define LEVEL_CMD_JUMP_LINK_PUSH_ARG        0x08
#define LEVEL_CMD_JUMP_N_TIMES              0x09
#define LEVEL_CMD_LOOP_BEGIN                0x0A
#define LEVEL_CMD_LOOP_UNTIL                0x0B
#define LEVEL_CMD_JUMP_IF                   0x0C
#define LEVEL_CMD_JUMP_LINK_IF              0x0D
#define LEVEL_CMD_SKIP_IF                   0x0E
#define LEVEL_CMD_SKIP                      0x0F
#define LEVEL_CMD_SKIP_NOP                  0x10
#define LEVEL_CMD_CALL                      0x11
#define LEVEL_CMD_CALL_LOOP                 0x12
#define LEVEL_CMD_SET_REG                   0x13
#define LEVEL_CMD_PUSH_POOL                 0x14
#define LEVEL_CMD_POP_POOL                  0x15
#define LEVEL_CMD_FIXED_LOAD                0x16
#define LEVEL_CMD_LOAD_RAW                  0x17
#define LEVEL_CMD_LOAD_MIO0                 0x18
#define LEVEL_CMD_LOAD_MARIO_HEAD           0x19
#define LEVEL_CMD_LOAD_MIO0_TEXTURE         0x1A
#define LEVEL_CMD_INIT_LEVEL                0x1B
#define LEVEL_CMD_CLEAR_LEVEL               0x1C
#define LEVEL_CMD_ALLOC_LEVEL_POOL          0x1D
#define LEVEL_CMD_FREE_LEVEL_POOL           0x1E
#define LEVEL_CMD_AREA                      0x1F
#define LEVEL_CMD_END_AREA                  0x20
#define LEVEL_CMD_LOAD_MODEL_FROM_DL        0x21
#define LEVEL_CMD_LOAD_MODEL_FROM_GEO       0x22
#define LEVEL_CMD_CMD23                     0x23
#define LEVEL_CMD_OBJECT_WITH_ACTS          0x24
#define LEVEL_CMD_MARIO                     0x25
#define LEVEL_CMD_WARP_NODE                 0x26
#define LEVEL_CMD_PAINTING_WARP_NODE        0x27
#define LEVEL_CMD_INSTANT_WARP              0x28
#define LEVEL_CMD_LOAD_AREA                 0x29
#define LEVEL_CMD_CMD2A                     0x2A
#define LEVEL_CMD_MARIO_POS                 0x2B
#define LEVEL_CMD_CMD2C                     0x2C
#define LEVEL_CMD_CMD2D                     0x2D
#define LEVEL_CMD_TERRAIN                   0x2E
#define LEVEL_CMD_ROOMS                     0x2F
#define LEVEL_CMD_SHOW_DIALOG               0x30
#define LEVEL_CMD_TERRAIN_TYPE              0x31
#define LEVEL_CMD_NOP                       0x32
#define LEVEL_CMD_TRANSITION                0x33
#define LEVEL_CMD_BLACKOUT                  0x34
#define LEVEL_CMD_GAMMA                     0x35
#define LEVEL_CMD_SET_BACKGROUND_MUSIC      0x36
#define LEVEL_CMD_SET_MENU_MUSIC            0x37
#define LEVEL_CMD_STOP_MUSIC                0x38
#define LEVEL_CMD_MACRO_OBJECTS             0x39
#define LEVEL_CMD_CMD3A                     0x3A
#define LEVEL_CMD_WHIRLPOOL                 0x3B
#define LEVEL_CMD_GET_OR_SET                0x3C
#define LEVEL_CMD_ADV_DEMO                  0x3D
#define LEVEL_CMD_CLEAR_DEMO_PTR            0x3E
#define LEVEL_CMD_JUMP_AREA                 0x3F
#define LEVEL_CMD_BRANCH(from, to, script)  \
OMM_AT_STARTUP static void script##__create_branch() { \
    extern void level_create_branch(void *, void *, void *); \
    level_create_branch((void *) from, (void *) to, (void *) script); \
}

#define level_cmd_shift                     (sizeof(void *) >> 3)
#define level_cmd_offset(offset)            (((offset) & 3) | (((offset) & ~3) << level_cmd_shift))
#define level_cmd_get(cmd, type, offset)    (*(type *) (((u8 *) (cmd)) + level_cmd_offset(offset)))

extern const LevelScript level_script_entry[];

typedef s32 (*LevelScriptPreprocessFunc)(u8, void *);
void level_script_preprocess(const LevelScript *script, LevelScriptPreprocessFunc func);
void *level_script_find(const LevelScript *script, const LevelScript *commands, s32 numCommands);

struct LevelCommand { u8 type; u8 size; };
struct LevelCommand *level_script_execute(struct LevelCommand *cmd);

#endif
