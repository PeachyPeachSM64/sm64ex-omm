#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "buffers/framebuffers.h"
#include "buffers/zbuffer.h"
s32 gDemoInputListID_2;
#define WHIRLPOOL_COND_BOWSER2 1
#if LEVEL_SCRIPT_PRESSED_START
extern s32 gPressedStart;
#define VAR_PRESSED_START 5
#endif

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
    sLevelCommand = (struct LevelCommand *) level_main_menu_entry_1;
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
    OMM_MEMSET(gMarioSpawnInfo, 0, sizeof(*gMarioSpawnInfo));
    gMarioSpawnInfo->activeAreaIndex = -1;
    gMarioSpawnInfo->behaviorArg = 1;
    gMarioSpawnInfo->behaviorScript = (void *) bhvMario;
    gMarioSpawnInfo->mModel = gLoadedGraphNodes[MODEL_MARIO];
    sLevelCommand = level_cmd_next;
}

static void level_cmd_place_object(void) {
    u8 act = (1 << (gCurrActNum - 1));
    u8 acts = level_cmd_get(u8, 2);
    if (sCurrAreaIndex != -1 && (OMM_STARS_NON_STOP_NOT_ENDING_CUTSCENE || (acts == (ALL_ACTS_MACRO)) || (acts & act))) {
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
        if (gAreas[sCurrAreaIndex].paintingWarpNodes == NULL) {
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
    s32 beatBowser2 = (save_file_get_flags() & (SAVE_FLAG_HAVE_KEY_2 | SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR)) != 0;
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
        OMM_MEMCPY(gAreas[sCurrAreaIndex].terrainData, data, size);
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
        OMM_MEMCPY(gAreas[sCurrAreaIndex].macroObjects, data, size * sizeof(MacroObject));
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
#if LEVEL_SCRIPT_PRESSED_START
            case VAR_PRESSED_START:      sRegister = gPressedStart; break;
#endif
        } break;
        case OP_SET: switch (var) {
            case VAR_CURR_SAVE_FILE_NUM: gCurrSaveFileNum = sRegister; break;
            case VAR_CURR_COURSE_NUM:    gCurrCourseNum = sRegister; break;
            case VAR_CURR_ACT_NUM:       gCurrActNum = sRegister; break;
            case VAR_CURR_LEVEL_NUM:     gCurrLevelNum = sRegister; break;
            case VAR_CURR_AREA_INDEX:    gCurrAreaIndex = sRegister; break;
#if LEVEL_SCRIPT_PRESSED_START
            case VAR_PRESSED_START:      gPressedStart = sRegister; break;
#endif
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
        LevelBranch *branch = OMM_MEMNEW(LevelBranch, 1);
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
    level_cmd_load_and_execute,
    level_cmd_exit_and_execute,
    level_cmd_exit,
    level_cmd_sleep,
    level_cmd_sleep,
    level_cmd_jump,
    level_cmd_jump_and_link,
    level_cmd_return,
    level_cmd_jump_and_link_push_arg,
    level_cmd_jump_repeat,
    level_cmd_loop_begin,
    level_cmd_loop_until,
    level_cmd_jump_if,
    level_cmd_jump_and_link_if,
    level_cmd_skip_if,
    level_cmd_skip,
    level_cmd_skippable_nop,
    level_cmd_call,
    level_cmd_call_loop,
    level_cmd_set_register,
    level_cmd_push_pool_state,
    level_cmd_pop_pool_state,
    level_cmd_load_to_fixed_address,
    level_cmd_load_raw,
    level_cmd_load_mio0,
    level_cmd_load_mario_head,
    level_cmd_load_mio0_texture,
    level_cmd_init_level,
    level_cmd_clear_level,
    level_cmd_alloc_level_pool,
    level_cmd_free_level_pool,
    level_cmd_begin_area,
    level_cmd_end_area,
    level_cmd_load_model_from_dl,
    level_cmd_load_model_from_geo,
    level_cmd_23,
    level_cmd_place_object,
    level_cmd_init_mario,
    level_cmd_create_warp_node,
    level_cmd_create_painting_warp_node,
    level_cmd_create_instant_warp,
    level_cmd_load_area,
    level_cmd_unload_area,
    level_cmd_set_mario_start_pos,
    level_cmd_2C,
    level_cmd_2D,
    level_cmd_set_terrain_data,
    level_cmd_set_rooms,
    level_cmd_show_dialog,
    level_cmd_set_terrain_type,
    level_cmd_nop,
    level_cmd_set_transition,
    level_cmd_set_blackout,
    level_cmd_set_gamma,
    level_cmd_set_music,
    level_cmd_set_menu_music,
    level_cmd_38,
    level_cmd_set_macro_objects,
    level_cmd_3A,
    level_cmd_create_whirlpool,
    level_cmd_get_or_set_var,
    level_cmd_advdemo,
    level_cmd_cleardemoptr,
    level_cmd_jump_area,
};

struct LevelCommand *level_script_execute(struct LevelCommand *cmd) {

    // OMM routines update
    omm_debug_start_counter(OMM_COUNTER_OMM);
    omm_update();
    omm_debug_end_counter(OMM_COUNTER_OMM);

    // Level script update
    omm_debug_start_counter(OMM_COUNTER_LVL);
    sLevelCommand = cmd;
    for (sIsRunning = true; sIsRunning;) {
        level_cmd_branch();
        void *currCommand = sLevelCommand;
        sLevelCmdTable[sLevelCommand->type & 0x3F]();
        void *nextCommand = omm_update_cmd(currCommand, sRegister);
        if (nextCommand) sLevelCommand = nextCommand;
#if OMM_CODE_DYNOS
        nextCommand = dynos_update_cmd(currCommand);
        if (nextCommand) sLevelCommand = nextCommand;
#endif
    }
    omm_debug_end_counter(OMM_COUNTER_LVL);
    init_scene_rendering();

    // OMM pre-render routines
    omm_debug_start_counter(OMM_COUNTER_PRE);
    omm_pre_render();
    omm_debug_end_counter(OMM_COUNTER_PRE);

    // geo_process_root and render_hud
    omm_debug_start_counter(OMM_COUNTER_GEO);
    render_game();
    omm_debug_end_counter(OMM_COUNTER_GEO);

    // End of update
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
    uintptr_t levelStack[32]; OMM_MEMCPY(levelStack, sLevelStack, sizeof(sLevelStack));

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
    OMM_MEMCPY(sLevelStack, levelStack, sizeof(levelStack));
}

void *level_script_find(const LevelScript *script, const LevelScript *commands, s32 numCommands) {
    for (struct LevelCommand *cmd = (struct LevelCommand *) script;; cmd = (struct LevelCommand *) (((u8 *) cmd) + (cmd->size << __shift))) {
        if (OMM_MEMCMP(cmd, commands, numCommands * sizeof(LevelScript))) {
            return (void *) cmd;
        }
        if (cmd->type == LEVEL_CMD_RETURN || cmd->type == LEVEL_CMD_EXIT || cmd->type == LEVEL_CMD_EXIT_AND_EXECUTE) {
            break;
        }
    }
    return NULL;
}
