#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/omm_constants.h"
#include "buffers/framebuffers.h"
#include "buffers/zbuffer.h"
#include "level_commands.h"
s32 gDemoInputListID_2;
#define WHIRLPOOL_COND_BOWSER2 1

#undef level_cmd_get
#define __shift                         (sizeof(void *) >> 3)
#define __offset(offset)                (((offset) & 3) | (((offset) & ~3) << __shift))
#define level_cmd_get(type, offset)     (*(type *) (((u8 *) sLevelCommand) + __offset(offset)))
#define level_cmd_next                  ((struct LevelCommand *) (((u8 *) sLevelCommand) + (sLevelCommand->size << __shift)))
#define level_stack_push(value)         *(sLevelStackTop++) = (uintptr_t) (value)
#define level_stack_pop(type)           ((type) *(--sLevelStackTop))
#define level_stack_get(type, index)    ((type) *(sLevelStackTop - index))
#define level_stack_dec(dec)            sLevelStackTop -= dec

//
// Data
//

typedef s32 (*LevelScriptFunc)(s16, s32);
static struct LevelCommand *sLevelCommand;
static struct AllocOnlyPool *sLevelPool = NULL;
static uintptr_t sLevelStack[32];
static uintptr_t *sLevelStackTop = sLevelStack;
static uintptr_t *sLevelStackBase = NULL;
static s16 sCurrAreaIndex = -1;
static s32 sRegister;
static bool sIsRunning;

OMM_INLINE bool eval_script_op(s8 op, s32 value) {
    switch (op) {
        case OP_AND:  return  (sRegister &  value);
        case OP_NAND: return !(sRegister &  value);
        case OP_EQ:   return  (sRegister == value);
        case OP_NEQ:  return  (sRegister != value);
        case OP_LT:   return  (sRegister <  value);
        case OP_LEQ:  return  (sRegister <= value);
        case OP_GT:   return  (sRegister >  value);
        case OP_GEQ:  return  (sRegister >= value);
        default:      return  false;
    }
}

//
// Level script commands
//

static void level_cmd_load_and_execute(void) {
    main_pool_push_state();
    level_stack_push(level_cmd_next);
    level_stack_push(sLevelStackBase);
    sLevelStackBase = sLevelStackTop;
    sLevelCommand = level_cmd_get(struct LevelCommand *, 12);
}

static void level_cmd_exit_and_execute(void) {
    main_pool_pop_state();
    main_pool_push_state();
    sLevelStackTop = sLevelStackBase;
    sLevelCommand = level_cmd_get(struct LevelCommand *, 12);
}

static void level_cmd_exit(void) {
    main_pool_pop_state();
    sLevelStackTop = sLevelStackBase;
    sLevelStackBase = level_stack_pop(uintptr_t *);
    sLevelCommand = level_stack_pop(struct LevelCommand *);
}

static void level_cmd_sleep(void) {
    static s32 sSleepFrames = 0;
    sIsRunning = false;
    if (!sSleepFrames) {
        sSleepFrames = level_cmd_get(s16, 2);
    } else if (--sSleepFrames == 0) {
        sLevelCommand = level_cmd_next;
        sIsRunning = true;
    }
}

static void level_cmd_jump(void) {
    sLevelCommand = level_cmd_get(struct LevelCommand *, 4);
}

static void level_cmd_jump_and_link(void) {
    level_stack_push(level_cmd_next);
    sLevelCommand = level_cmd_get(struct LevelCommand *, 4);
}

static void level_cmd_return(void) {
    sLevelCommand = level_stack_pop(struct LevelCommand *);
}

static void level_cmd_jump_and_link_push_arg(void) {
    level_stack_push(level_cmd_next);
    level_stack_push(level_cmd_get(s16, 2));
    sLevelCommand = level_cmd_next;
}

static void level_cmd_jump_repeat(void) {
    s32 val = level_stack_pop(s32) - 1;
    if (val == 0) {
        sLevelCommand = level_cmd_next;
        level_stack_dec(1);
    } else {
        sLevelCommand = level_stack_get(struct LevelCommand *, 1);
        level_stack_push(val);
    }
}

static void level_cmd_loop_begin(void) {
    level_stack_push(level_cmd_next);
    level_stack_push(0);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_loop_until(void) {
    u8 op = level_cmd_get(u8, 2);
    s32 value = level_cmd_get(s32, 4);
    if (eval_script_op(op, value)) {
        sLevelCommand = level_cmd_next;
        level_stack_dec(2);
    } else {
        sLevelCommand = level_stack_get(struct LevelCommand *, 2);
    }
}

static void level_cmd_jump_if(void) {
    u8 op = level_cmd_get(u8, 2);
    s32 value = level_cmd_get(s32, 4);
    if (eval_script_op(op, value)) {
        sLevelCommand = level_cmd_get(struct LevelCommand *, 8);
    } else {
        sLevelCommand = level_cmd_next;
    }
}

static void level_cmd_jump_and_link_if(void) {
    u8 op = level_cmd_get(u8, 2);
    s32 value = level_cmd_get(s32, 4);
    if (eval_script_op(op, value)) {
        level_stack_push(level_cmd_next);
        sLevelCommand = level_cmd_get(struct LevelCommand *, 8);
    } else {
        sLevelCommand = level_cmd_next;
    }
}

static void level_cmd_skip_if(void) {
    u8 op = level_cmd_get(u8, 2);
    s32 value = level_cmd_get(s32, 4);
    if (!eval_script_op(op, value)) {
        do { sLevelCommand = level_cmd_next;
        } while (sLevelCommand->type == LEVEL_CMD_SKIP || sLevelCommand->type == LEVEL_CMD_SKIP_NOP);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_skip(void) {
    do { sLevelCommand = level_cmd_next;
    } while (sLevelCommand->type == LEVEL_CMD_SKIP_NOP);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_skippable_nop(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_call(void) {
    s16 param = level_cmd_get(s16, 2);
    LevelScriptFunc func = level_cmd_get(LevelScriptFunc, 4);
    sRegister = func(param, sRegister);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_call_loop(void) {
    s16 param = level_cmd_get(s16, 2);
    LevelScriptFunc func = level_cmd_get(LevelScriptFunc, 4);
    sRegister = func(param, sRegister);
    if (!sRegister) {
        sIsRunning = false;
    } else {
        sIsRunning = true;
        sLevelCommand = level_cmd_next;
    }
}

static void level_cmd_set_register(void) {
    sRegister = level_cmd_get(s16, 2);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_push_pool_state(void) {
    main_pool_push_state();
    sLevelCommand = level_cmd_next;
}

static void level_cmd_pop_pool_state(void) {
    main_pool_pop_state();
    sLevelCommand = level_cmd_next;
}

static void level_cmd_load_to_fixed_address(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_load_raw(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_load_mio0(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_load_mario_head(void) {
    sLevelCommand = (struct LevelCommand *) level_script_file_select;
}

static void level_cmd_load_mio0_texture(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_init_level(void) {
    init_graph_node_start(NULL, (struct GraphNodeStart *) &gObjParentGraphNode);
    clear_objects();
    clear_areas();
    main_pool_push_state();
    sLevelCommand = level_cmd_next;
}

static void level_cmd_clear_level(void) {
    clear_objects();
    clear_area_graph_nodes();
    clear_areas();
    main_pool_pop_state();
    sLevelCommand = level_cmd_next;
}

static void level_cmd_alloc_level_pool(void) {
    if (!sLevelPool) {
        sLevelPool = alloc_only_pool_init(main_pool_available() - sizeof(struct AllocOnlyPool), MEMORY_POOL_LEFT);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_free_level_pool(void) {
    alloc_only_pool_resize(sLevelPool, sLevelPool->usedSpace);
    sLevelPool = NULL;
    for (s32 i = 0; i != 8; ++i) {
        if (gAreaData[i].terrainData) {
            gCCMEnteredSlide = 0;
            reset_red_coins_collected();
            break;
        }
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_begin_area(void) {
    s32 areaIndex = level_cmd_get(u8, 2);
    const GeoLayout *geoLayout = level_cmd_get(const GeoLayout *, 4);
    if (areaIndex < 8) {
        struct GraphNodeRoot *screenArea = (struct GraphNodeRoot *) geo_layout_to_graph_node(sLevelPool, geoLayout);
        struct GraphNodeCamera *cameraNode = (struct GraphNodeCamera *) screenArea->views[0];
        sCurrAreaIndex = areaIndex;
        screenArea->areaIndex = areaIndex;
        gAreas[areaIndex].unk04 = screenArea;
        if (cameraNode) {
            gAreas[areaIndex].camera = (struct Camera *) cameraNode->config.camera;
        } else {
            gAreas[areaIndex].camera = NULL;
        }
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_end_area(void) {
    sCurrAreaIndex = -1;
    sLevelCommand = level_cmd_next;
}

static void level_cmd_load_model_from_dl(void) {
    s32 modelId = level_cmd_get(s16, 2) & 0xFF;
    s32 layer = level_cmd_get(u16, 2) >> 12;
    void *displayList = level_cmd_get(void *, 4);
    gLoadedGraphNodes[modelId] = (struct GraphNode *) init_graph_node_display_list(sLevelPool, NULL, layer, displayList);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_load_model_from_geo(void) {
    s32 modelId = level_cmd_get(s16, 2) & 0xFF;
    const GeoLayout *geoLayout = level_cmd_get(const GeoLayout *, 4);
    gLoadedGraphNodes[modelId] = geo_layout_to_graph_node(NULL, geoLayout);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_23(void) {
    s32 modelId = level_cmd_get(s16, 2) & 0xFF;
    s32 layer = level_cmd_get(u16, 2) >> 12;
    f32 scale = level_cmd_get(f32, 8);
    void *displayList = level_cmd_get(void *, 4);
    gLoadedGraphNodes[modelId] = (struct GraphNode *) init_graph_node_scale(sLevelPool, NULL, layer, displayList, scale);
    sLevelCommand = level_cmd_next;
}

// It's-a me, Mario!
static void level_cmd_init_mario(void) {
    mem_zero(gMarioSpawnInfo, sizeof(*gMarioSpawnInfo));
    gMarioSpawnInfo->activeAreaIndex = -1;
    gMarioSpawnInfo->behaviorArg = 1;
    gMarioSpawnInfo->behaviorScript = (void *) bhvMario;
    gMarioSpawnInfo->mModel = gLoadedGraphNodes[MODEL_MARIO];
    sLevelCommand = level_cmd_next;
}

static void level_cmd_place_object(void) {
    u8 actBit = (1 << (gCurrActNum - 1));
    u8 actBits = level_cmd_get(u8, 2);
    if (sCurrAreaIndex != -1 && (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE || (actBits == (ALL_ACTS_MACRO)) || (actBits & actBit))) {
        struct SpawnInfo *spawnInfo = alloc_only_pool_alloc(sLevelPool, sizeof(struct SpawnInfo));
        spawnInfo->startPos[0] = level_cmd_get(s16, 4);
        spawnInfo->startPos[1] = level_cmd_get(s16, 6);
        spawnInfo->startPos[2] = level_cmd_get(s16, 8);
        spawnInfo->startAngle[0] = (level_cmd_get(s16, 10) * 0x8000) / 180;
        spawnInfo->startAngle[1] = (level_cmd_get(s16, 12) * 0x8000) / 180;
        spawnInfo->startAngle[2] = (level_cmd_get(s16, 14) * 0x8000) / 180;
        spawnInfo->areaIndex = sCurrAreaIndex;
        spawnInfo->activeAreaIndex = sCurrAreaIndex;
        spawnInfo->behaviorArg = level_cmd_get(u32, 16);
        spawnInfo->behaviorScript = level_cmd_get(void *, 20);
        spawnInfo->mModel = gLoadedGraphNodes[level_cmd_get(u8, 3)];
        spawnInfo->next = gAreas[sCurrAreaIndex].objectSpawnInfos;
        gAreas[sCurrAreaIndex].objectSpawnInfos = spawnInfo;
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_create_warp_node(void) {
    if (sCurrAreaIndex != -1) {
        struct ObjectWarpNode *warpNode = alloc_only_pool_alloc(sLevelPool, sizeof(struct ObjectWarpNode));
        u8 checkpoint = level_cmd_get(u8, 6);
        warpNode->node.id = level_cmd_get(u8, 2);
        warpNode->node.destLevel = level_cmd_get(u8, 3) + checkpoint;
        warpNode->node.destArea = level_cmd_get(u8, 4);
        warpNode->node.destNode = level_cmd_get(u8, 5);
        warpNode->object = NULL;
#if OMM_GAME_IS_SMMS
        // Moonshine: Fix SSL broken warp nodes
        if (warpNode->node.destLevel == LEVEL_CASTLE && warpNode->node.destArea == 0x01) {
            if (warpNode->node.id == WARP_NODE_STAR_EXIT && warpNode->node.destNode == 0x39) warpNode->node.destNode = 0x22;
            if (warpNode->node.id == WARP_NODE_DEATH && warpNode->node.destNode == 0x40) warpNode->node.destNode = 0x24;
        }
#endif
        warpNode->next = gAreas[sCurrAreaIndex].warpNodes;
        gAreas[sCurrAreaIndex].warpNodes = warpNode;
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_create_instant_warp(void) {
    if (sCurrAreaIndex != -1) {
        if (!gAreas[sCurrAreaIndex].instantWarps) {
            gAreas[sCurrAreaIndex].instantWarps = alloc_only_pool_alloc(sLevelPool, 4 * sizeof(struct InstantWarp));
            for (s32 i = INSTANT_WARP_INDEX_START; i < INSTANT_WARP_INDEX_STOP; i++) {
                gAreas[sCurrAreaIndex].instantWarps[i].id = 0;
            }
        }
        struct InstantWarp *warp = &gAreas[sCurrAreaIndex].instantWarps[level_cmd_get(u8, 2)];
        warp->id = 1;
        warp->area = level_cmd_get(u8, 3);
        warp->displacement[0] = level_cmd_get(s16, 4);
        warp->displacement[1] = level_cmd_get(s16, 6);
        warp->displacement[2] = level_cmd_get(s16, 8);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_terrain_type(void) {
    if (sCurrAreaIndex != -1) {
        gAreas[sCurrAreaIndex].terrainType |= level_cmd_get(s16, 2);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_create_painting_warp_node(void) {
    if (sCurrAreaIndex != -1) {
        if (!gAreas[sCurrAreaIndex].paintingWarpNodes) {
            gAreas[sCurrAreaIndex].paintingWarpNodes = alloc_only_pool_alloc(sLevelPool, 45 * sizeof(struct WarpNode));
            for (s32 i = 0; i < 45; i++) {
                gAreas[sCurrAreaIndex].paintingWarpNodes[i].id = 0;
            }
        }
        struct WarpNode *node = &gAreas[sCurrAreaIndex].paintingWarpNodes[level_cmd_get(u8, 2)];
        u8 checkpoint = level_cmd_get(u8, 6);
        node->id = 1;
        node->destLevel = level_cmd_get(u8, 3) + checkpoint;
        node->destArea = level_cmd_get(u8, 4);
        node->destNode = level_cmd_get(u8, 5);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_3A(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_create_whirlpool(void) {
    s32 index = level_cmd_get(u8, 2);
    s32 cond = level_cmd_get(u8, 3);
    s32 beatBowser2 = (omm_save_file_get_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE) & (SAVE_FLAG_HAVE_KEY_2 | SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR)) != 0;
    if (cond == WHIRLPOOL_COND_ALWAYS ||
       (cond == WHIRLPOOL_COND_BOWSER2 && !beatBowser2) ||
       (cond == WHIRLPOOL_COND_BOWSER2_BEATEN && beatBowser2) ||
       (cond == WHIRLPOOL_COND_AT_LEAST_SECOND_STAR && gCurrActNum >= 2)) {
        if (sCurrAreaIndex != -1 && index < 2) {
            struct Whirlpool *whirlpool = gAreas[sCurrAreaIndex].whirlpools[index];
            if (!whirlpool) {
                whirlpool = alloc_only_pool_alloc(sLevelPool, sizeof(struct Whirlpool));
                gAreas[sCurrAreaIndex].whirlpools[index] = whirlpool;
            }
            whirlpool->pos[0] = level_cmd_get(s16, 4);
            whirlpool->pos[1] = level_cmd_get(s16, 6);
            whirlpool->pos[2] = level_cmd_get(s16, 8);
            whirlpool->strength = level_cmd_get(s16, 10);
        }
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_blackout(void) {
    osViBlack(level_cmd_get(u8, 2));
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_gamma(void) {
    osViSetSpecialFeatures(level_cmd_get(u8, 2) ? OS_VI_GAMMA_ON : OS_VI_GAMMA_OFF);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_terrain_data(void) {
    if (sCurrAreaIndex != -1) {
        Collision *data = level_cmd_get(Collision *, 4);
        s32 size = (s32) (get_area_terrain_size(data) * sizeof(Collision));
        gAreas[sCurrAreaIndex].terrainData = alloc_only_pool_alloc(sLevelPool, size);
        mem_cpy(gAreas[sCurrAreaIndex].terrainData, data, size);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_rooms(void) {
    if (sCurrAreaIndex != -1) {
        gAreas[sCurrAreaIndex].surfaceRooms = level_cmd_get(s8 *, 4);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_macro_objects(void) {
    if (sCurrAreaIndex != -1) {
        MacroObject *data = level_cmd_get(MacroObject *, 4);
        s32 size = 0; while (data[size++] != MACRO_OBJECT_END()) { size += 4; }
        gAreas[sCurrAreaIndex].macroObjects = alloc_only_pool_alloc(sLevelPool, size * sizeof(MacroObject));
        mem_cpy(gAreas[sCurrAreaIndex].macroObjects, data, size * sizeof(MacroObject));
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_load_area(void) {
    s32 areaIndex = level_cmd_get(u8, 2);
    sound_stop_continuous();
    load_area(areaIndex);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_unload_area(void) {
    unload_area();
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_mario_start_pos(void) {
    gMarioSpawnInfo->areaIndex = level_cmd_get(u8, 2);
    gMarioSpawnInfo->startPos[0] = level_cmd_get(s16, 6);
    gMarioSpawnInfo->startPos[1] = level_cmd_get(s16, 8);
    gMarioSpawnInfo->startPos[2] = level_cmd_get(s16, 10);
    gMarioSpawnInfo->startAngle[1] = (level_cmd_get(s16, 4) * 0x8000) / 180;
    sLevelCommand = level_cmd_next;
}

static void level_cmd_2C(void) {
    unload_mario_area();
    sLevelCommand = level_cmd_next;
}

static void level_cmd_2D(void) {
    area_update_objects();
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_transition(void) {
    if (gCurrentArea) {
        s16 type = level_cmd_get(u8, 2);
        s16 time = level_cmd_get(u8, 3);
        u8 red   = level_cmd_get(u8, 4);
        u8 green = level_cmd_get(u8, 5);
        u8 blue  = level_cmd_get(u8, 6);
        play_transition(type, time, red, green, blue);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_nop(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_show_dialog(void) {
    if (sCurrAreaIndex != -1) {
        s32 dialogIndex = level_cmd_get(u8, 2);
        if (dialogIndex < 2) {
            gAreas[sCurrAreaIndex].dialog[dialogIndex] = level_cmd_get(u8, 3);
        }
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_music(void) {
    if (sCurrAreaIndex != -1) {
        gAreas[sCurrAreaIndex].musicParam = level_cmd_get(s16, 2);
        gAreas[sCurrAreaIndex].musicParam2 = level_cmd_get(s16, 4);
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_set_menu_music(void) {
    set_background_music(0, level_cmd_get(s16, 2), 0);
    sLevelCommand = level_cmd_next;
}

static void level_cmd_38(void) {
    fadeout_music(level_cmd_get(s16, 2));
    sLevelCommand = level_cmd_next;
}

static void level_cmd_get_or_set_var(void) {
    u8 op = level_cmd_get(u8, 2);
    u8 var = level_cmd_get(u8, 3);
    switch (op) {
        case OP_GET: switch (var) {
            case VAR_CURR_SAVE_FILE_NUM: sRegister = gCurrSaveFileNum; break;
            case VAR_CURR_COURSE_NUM:    sRegister = gCurrCourseNum; break;
            case VAR_CURR_ACT_NUM:       sRegister = gCurrActNum; break;
            case VAR_CURR_LEVEL_NUM:     sRegister = gCurrLevelNum; break;
            case VAR_CURR_AREA_INDEX:    sRegister = gCurrAreaIndex; break;
        } break;
        case OP_SET: switch (var) {
            case VAR_CURR_SAVE_FILE_NUM: gCurrSaveFileNum = sRegister; break;
            case VAR_CURR_COURSE_NUM:    gCurrCourseNum = sRegister; break;
            case VAR_CURR_ACT_NUM:       gCurrActNum = sRegister; break;
            case VAR_CURR_LEVEL_NUM:     gCurrLevelNum = sRegister; break;
            case VAR_CURR_AREA_INDEX:    gCurrAreaIndex = sRegister; break;
        } break;
    }
    sLevelCommand = level_cmd_next;
}

static void level_cmd_advdemo(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_cleardemoptr(void) {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_jump_area(void) {
    s32 areaIndex = level_cmd_get(s32, 4);
    if (sWarpDest.areaIdx == areaIndex) {
        sLevelCommand = level_cmd_get(struct LevelCommand *, 8);
        return;
    }
    sLevelCommand = level_cmd_next;
}

//
// Branches
//

typedef struct { void *from; void *to; void *script; } LevelBranch;
static OmmMap sLevelBranches = omm_map_zero;
void level_create_branch(void *from, void *to, void *script) {
    if (from && to && script) {
        LevelBranch *branch = mem_new(LevelBranch, 1);
        branch->from = from;
        branch->to = to;
        branch->script = script;
        omm_map_add(sLevelBranches, ptr, from, ptr, branch);
    }
}

static void level_cmd_branch() {
    s32 i = omm_map_find_key(sLevelBranches, ptr, sLevelCommand);
    if (i != -1) {
        LevelBranch *branch = omm_map_get_val(sLevelBranches, ptr, i);
        level_stack_push(branch->to);
        sLevelCommand = branch->script;
    }
}

//
// Level script table
//

typedef void (*LevelCommandProc)(void);
static LevelCommandProc sLevelCmdTable[] = {
/* 0x00 */ level_cmd_load_and_execute,
/* 0x01 */ level_cmd_exit_and_execute,
/* 0x02 */ level_cmd_exit,
/* 0x03 */ level_cmd_sleep,
/* 0x04 */ level_cmd_sleep,
/* 0x05 */ level_cmd_jump,
/* 0x06 */ level_cmd_jump_and_link,
/* 0x07 */ level_cmd_return,
/* 0x08 */ level_cmd_jump_and_link_push_arg,
/* 0x09 */ level_cmd_jump_repeat,
/* 0x0A */ level_cmd_loop_begin,
/* 0x0B */ level_cmd_loop_until,
/* 0x0C */ level_cmd_jump_if,
/* 0x0D */ level_cmd_jump_and_link_if,
/* 0x0E */ level_cmd_skip_if,
/* 0x0F */ level_cmd_skip,
/* 0x10 */ level_cmd_skippable_nop,
/* 0x11 */ level_cmd_call,
/* 0x12 */ level_cmd_call_loop,
/* 0x13 */ level_cmd_set_register,
/* 0x14 */ level_cmd_push_pool_state,
/* 0x15 */ level_cmd_pop_pool_state,
/* 0x16 */ level_cmd_load_to_fixed_address,
/* 0x17 */ level_cmd_load_raw,
/* 0x18 */ level_cmd_load_mio0,
/* 0x19 */ level_cmd_load_mario_head,
/* 0x1A */ level_cmd_load_mio0_texture,
/* 0x1B */ level_cmd_init_level,
/* 0x1C */ level_cmd_clear_level,
/* 0x1D */ level_cmd_alloc_level_pool,
/* 0x1E */ level_cmd_free_level_pool,
/* 0x1F */ level_cmd_begin_area,
/* 0x20 */ level_cmd_end_area,
/* 0x21 */ level_cmd_load_model_from_dl,
/* 0x22 */ level_cmd_load_model_from_geo,
/* 0x23 */ level_cmd_23,
/* 0x24 */ level_cmd_place_object,
/* 0x25 */ level_cmd_init_mario,
/* 0x26 */ level_cmd_create_warp_node,
/* 0x27 */ level_cmd_create_painting_warp_node,
/* 0x28 */ level_cmd_create_instant_warp,
/* 0x29 */ level_cmd_load_area,
/* 0x2A */ level_cmd_unload_area,
/* 0x2B */ level_cmd_set_mario_start_pos,
/* 0x2C */ level_cmd_2C,
/* 0x2D */ level_cmd_2D,
/* 0x2E */ level_cmd_set_terrain_data,
/* 0x2F */ level_cmd_set_rooms,
/* 0x30 */ level_cmd_show_dialog,
/* 0x31 */ level_cmd_set_terrain_type,
/* 0x32 */ level_cmd_nop,
/* 0x33 */ level_cmd_set_transition,
/* 0x34 */ level_cmd_set_blackout,
/* 0x35 */ level_cmd_set_gamma,
/* 0x36 */ level_cmd_set_music,
/* 0x37 */ level_cmd_set_menu_music,
/* 0x38 */ level_cmd_38,
/* 0x39 */ level_cmd_set_macro_objects,
/* 0x3A */ level_cmd_3A,
/* 0x3B */ level_cmd_create_whirlpool,
/* 0x3C */ level_cmd_get_or_set_var,
/* 0x3D */ level_cmd_advdemo,
/* 0x3E */ level_cmd_cleardemoptr,
/* 0x3F */ level_cmd_jump_area,
};

struct LevelCommand *level_script_execute(struct LevelCommand *cmd) {

    // OMM routines update
    omm_profiler_start(OMM_PRF_OMM);
    omm_update();
    omm_profiler_stop(OMM_PRF_OMM);

    // Level script update
    omm_profiler_start(OMM_PRF_LVL);
    sLevelCommand = cmd;
    for (sIsRunning = true; sIsRunning;) {
        level_cmd_branch();
        void *currCommand = sLevelCommand;
        sLevelCmdTable[sLevelCommand->type & 0x3F]();
        void *nextCommand = omm_update_cmd(currCommand, sRegister);
        if (nextCommand) sLevelCommand = nextCommand;
    }
    omm_profiler_stop(OMM_PRF_LVL);
    init_render_image();

    // OMM pre-render routines
    omm_profiler_start(OMM_PRF_PRE);
    omm_pre_render();
    omm_profiler_stop(OMM_PRF_PRE);

    // geo_process_root and render_hud
    omm_profiler_start(OMM_PRF_GEO);
    omm_models_update();
    render_game();
    omm_profiler_stop(OMM_PRF_GEO);

    // End of update
    omm_profiler_display();
    end_master_display_list();
    return sLevelCommand;
}

//
// Level script preprocessing
// - Ifs are always true
// - Skips are always false
// - Loops break after the first iteration
//

static void level_cmd_preprocess_next() {
    sLevelCommand = level_cmd_next;
}

static void level_cmd_preprocess_load_and_execute(void) {
    level_stack_push(level_cmd_next);
    level_stack_push(sLevelStackBase);
    sLevelStackBase = sLevelStackTop;
    sLevelCommand = level_cmd_get(struct LevelCommand *, 12);
}

static void level_cmd_preprocess_exit_and_execute(void) {
    sLevelStackTop = sLevelStackBase;
    sLevelCommand = level_cmd_get(struct LevelCommand *, 12);
}

static void level_cmd_preprocess_exit(void) {
    sLevelStackTop = sLevelStackBase;
    sLevelStackBase = level_stack_pop(uintptr_t *);
    sLevelCommand = level_stack_pop(struct LevelCommand *);
}

static void level_cmd_preprocess_jump(void) {
    sLevelCommand = level_cmd_get(struct LevelCommand *, 4);
}

static void level_cmd_preprocess_jump_and_link(void) {
    level_stack_push(level_cmd_next);
    sLevelCommand = level_cmd_get(struct LevelCommand *, 4);
}

static void level_cmd_preprocess_return(void) {
    sLevelCommand = level_stack_pop(struct LevelCommand *);
}

static void level_cmd_preprocess_jump_if(void) {
    level_stack_push(level_cmd_next); // Not an error, that's intentional
    sLevelCommand = level_cmd_get(struct LevelCommand *, 8);
}

static void level_cmd_preprocess_jump_and_link_if(void) {
    level_stack_push(level_cmd_next);
    sLevelCommand = level_cmd_get(struct LevelCommand *, 8);
}

static void level_cmd_preprocess_jump_area(void) {
    level_stack_push(level_cmd_next);
    level_stack_push(sLevelStackBase);
    sLevelStackBase = sLevelStackTop;
    sLevelCommand = level_cmd_get(struct LevelCommand *, 8);
}

//
// Level script table for preprocessing
//

static LevelCommandProc sLevelCmdTablePreprocess[] = {
    level_cmd_preprocess_load_and_execute,
    level_cmd_preprocess_exit_and_execute,
    level_cmd_preprocess_exit,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_jump,
    level_cmd_preprocess_jump_and_link,
    level_cmd_preprocess_return,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_jump_if,
    level_cmd_preprocess_jump_and_link_if,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_next,
    level_cmd_preprocess_jump_area,
};

void level_script_preprocess(const LevelScript *script, LevelScriptPreprocessFunc func) {

    // Back-up the level script state
    struct LevelCommand *levelCommand = sLevelCommand;
    uintptr_t *levelStackTop = sLevelStackTop;
    uintptr_t *levelStackBase = sLevelStackBase;
    uintptr_t levelStack[32]; mem_cpy(levelStack, sLevelStack, sizeof(sLevelStack));

    // Init level script state for preprocessing
    sLevelCommand = (struct LevelCommand *) script;
    sLevelStack[0] = 0;
    sLevelStackTop = sLevelStack;
    sLevelStackBase = sLevelStack;

    // Preprocess Level script
    while (sLevelCommand) {
        level_cmd_branch();
        u8 type = sLevelCommand->type;
        s32 action = func(type, (void *) sLevelCommand);
        switch (action) {
            case LEVEL_SCRIPT_CONTINUE: sLevelCmdTablePreprocess[type & 0x3F](); break;
            case LEVEL_SCRIPT_NEXT:     sLevelCommand = level_cmd_next; break;
            case LEVEL_SCRIPT_RETURN:   sLevelCommand = level_stack_pop(struct LevelCommand *); break;
            case LEVEL_SCRIPT_STOP:     sLevelCommand = NULL; break;
        }
    }

    // Restore the level script state
    sLevelCommand = levelCommand;
    sLevelStackTop = levelStackTop;
    sLevelStackBase = levelStackBase;
    mem_cpy(sLevelStack, levelStack, sizeof(levelStack));
}

void *level_script_find(const LevelScript *script, const LevelScript *commands, s32 numCommands) {
    for (struct LevelCommand *cmd = (struct LevelCommand *) script;; cmd = (struct LevelCommand *) (((u8 *) cmd) + (cmd->size << __shift))) {
        if (mem_eq(cmd, commands, numCommands * sizeof(LevelScript))) {
            return (void *) cmd;
        }
        if (cmd->type == LEVEL_CMD_RETURN || cmd->type == LEVEL_CMD_EXIT || cmd->type == LEVEL_CMD_EXIT_AND_EXECUTE) {
            break;
        }
    }
    return NULL;
}
