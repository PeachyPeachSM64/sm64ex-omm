#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"
#define COURSE_IS_BOWSER_COURSE(courseNum) ((courseNum) >= COURSE_BITDW && (courseNum) <= COURSE_BITS)

#if OMM_CODE_DEBUG
#define time_trials_ghost_data_debug_log(index, fmt, ...) { \
    str_t ghost_data; \
    if (index.isSparkly) { \
        str_fmt(ghost_data, sizeof(ghost_data), "sparkly ghost data P%d-M%d-C%d", index.pi, index.mi, index.ci); \
    } else { \
        str_fmt(ghost_data, sizeof(ghost_data), "ghost data P%d-F%d-M%d-C%d-S%d", index.pi, index.fi, index.mi, index.ci, index.si); \
    } \
    omm_debug_log(fmt,, ghost_data, __VA_ARGS__); \
}
#else
#define time_trials_ghost_data_debug_log(index, fmt, ...)
#endif

// Constants
#define TT_FILENAME_EXT             ".omm.ttg"
#define TT_SPARKLY_DIR              "sparkly"
#define TT_TIME_LIMIT               ((10 * 60 * 30) - 1) // 9'59"99
#define TT_TIME_UNDEFINED           (-1)
#define TT_STAR_NUM_100_COINS       (7)
#define TT_STAR_NUM_BOWSER_KEY      (TT_NUM_STARS_PER_COURSE + 1)
#define TT_STAR_NUM_ALL_STARS       (TT_NUM_STARS_PER_COURSE + 1)
#define TT_NUM_SAVE_FILES           (NUM_SAVE_FILES)
#define TT_NUM_SAVE_MODES           (NUM_SAVE_MODES)
#define TT_NUM_COURSES              (COURSE_MAX + 1 - COURSE_MIN)
#define TT_NUM_STARS_PER_COURSE     (OMM_NUM_STARS_MAX_PER_COURSE)
#define TT_NUM_TIMES_PER_COURSE     (TT_STAR_NUM_ALL_STARS)

// Index
typedef struct { bool isSparkly; s32 pi, fi, mi, ci, si; } TT_Index;
#define TT_INDEX(playerIndex, fileNum, modeIndex, courseNum, starNum) (TT_Index) { false, playerIndex, fileNum - 1, modeIndex, courseNum - COURSE_MIN, starNum - 1 }
#define TT_INDEX_SPARKLY(playerIndex, sparklyMode, starIndex)         (TT_Index) { true, playerIndex, 0, sparklyMode - 1, starIndex, 0 }

// Checks
#define TT_CHECK_INDEX(index, x) { \
    if (index.isSparkly) { \
        if (index.pi < 0 || index.pi >= OMM_NUM_PLAYABLE_CHARACTERS) { return x; } \
        if (index.fi != 0) { return x; } \
        if (index.mi < 0 || index.mi >= OMM_SPARKLY_MODE_COUNT - 1) { return x; } \
        if (index.ci < 0 || index.ci >= OMM_SPARKLY_STARS_MAX) { return x; } \
        if (index.si != 0) { return x; } \
    } else { \
        if (index.pi < 0 || index.pi >= OMM_NUM_PLAYABLE_CHARACTERS) { return x; } \
        if (index.fi < 0 || index.fi >= TT_NUM_SAVE_FILES) { return x; } \
        if (index.mi < 0 || index.mi >= TT_NUM_SAVE_MODES) { return x; } \
        if (index.ci < 0 || index.ci >= TT_NUM_COURSES) { return x; } \
        if (index.si < 0 || index.si >= TT_NUM_TIMES_PER_COURSE) { return x; } \
    } \
}

bool time_trials_is_enabled() {
    return gOmmTimeTrialsEnabled && !omm_is_main_menu() && gCurrSaveFileNum <= NUM_SAVE_FILES;
}

//
// Ghost behaviors
//

static const BehaviorScript bhvTimeTrialsGhost[TT_NUM_TIMES_PER_COURSE][2] = {
    { BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BEGIN_LOOP(), BHV_END_LOOP() },
};

static const BehaviorScript bhvTimeTrialsGhostNumber[TT_NUM_TIMES_PER_COURSE][3] = {
    { BHV_BILLBOARD(), BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BILLBOARD(), BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BILLBOARD(), BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BILLBOARD(), BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BILLBOARD(), BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BILLBOARD(), BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BILLBOARD(), BHV_BEGIN_LOOP(), BHV_END_LOOP() },
    { BHV_BILLBOARD(), BHV_BEGIN_LOOP(), BHV_END_LOOP() },
};

//
// Ghost models
//

#include "time_trials_ghost_geo.inl"

enum {
    TT_GHOST_MARIO_DEFAULT,   // Blue
    TT_GHOST_MARIO_BOWSER,    // Red
    TT_GHOST_MARIO_SLIDE,     // Green
    TT_GHOST_MARIO_100_COINS, // Yellow
    TT_GHOST_MARIO_ALL_STARS, // Pink
    TT_GHOST_MARIO_PINK_GOLD, // Pink-Gold
    TT_GHOST_MARIO_CRYSTAL,   // Crystal
    TT_GHOST_MARIO_NEBULA,    // Nebula
};

static struct GraphNode *time_trials_get_ghost_mario_graph_node(s32 type) {
    switch (type) {
        case TT_GHOST_MARIO_DEFAULT:   return geo_layout_to_graph_node(NULL, tt_ghost_mario_blue_geo);
        case TT_GHOST_MARIO_BOWSER:    return geo_layout_to_graph_node(NULL, tt_ghost_mario_red_geo);
        case TT_GHOST_MARIO_SLIDE:     return geo_layout_to_graph_node(NULL, tt_ghost_mario_green_geo);
        case TT_GHOST_MARIO_100_COINS: return geo_layout_to_graph_node(NULL, tt_ghost_mario_gold_geo);
        case TT_GHOST_MARIO_ALL_STARS: return geo_layout_to_graph_node(NULL, tt_ghost_mario_pink_geo);
        case TT_GHOST_MARIO_PINK_GOLD: return geo_layout_to_graph_node(NULL, tt_ghost_mario_pink_gold_geo);
        case TT_GHOST_MARIO_CRYSTAL:   return geo_layout_to_graph_node(NULL, tt_ghost_mario_crystal_geo);
        case TT_GHOST_MARIO_NEBULA:    return geo_layout_to_graph_node(NULL, tt_ghost_mario_nebula_geo);
    }
    return NULL;
}

//
// Ghost data
//

typedef struct {
    s16 posX, posY, posZ;
    s8 pitch, yaw, roll;
    u8 scaleX, scaleY, scaleZ;
    u8 animIndex, animFrame;
    u8 levelNum, areaIndex;
} TT_Frame;
typedef TT_Frame TT_Ghost[TT_TIME_LIMIT + 1];

typedef struct {
    TT_Frame *data;
    s32 length;
    s32 time;
} TT_Data;

typedef struct {
    TT_Data stars[TT_NUM_TIMES_PER_COURSE];
} TT_Course;

typedef struct {
    TT_Course courses[TT_NUM_COURSES];
} TT_Mode;

typedef struct {
    TT_Mode modes[TT_NUM_SAVE_MODES];
} TT_File;

typedef struct {
    TT_Data stars[OMM_SPARKLY_STARS_MAX];
} TT_Sparkly;

typedef struct {
    TT_File files[TT_NUM_SAVE_FILES];
    TT_Sparkly sparklies[OMM_SPARKLY_MODE_COUNT - 1];
} TT_Player;

typedef struct {
    TT_Player players[OMM_NUM_PLAYABLE_CHARACTERS];
} TT_Cache;

static TT_Cache sTimeTrialsCache[1] = {0};

static TT_Data *time_trials_get(TT_Index index) {
    if (index.isSparkly) {
        return &sTimeTrialsCache->players[index.pi].sparklies[index.mi].stars[index.ci];
    }
    return &sTimeTrialsCache->players[index.pi].files[index.fi].modes[index.mi].courses[index.ci].stars[index.si];
}

//
// Time Trials data
//

enum { TT_STATE_DISABLED, TT_STATE_RUNNING, TT_STATE_STOPPED };

static struct {
    s32 state;
    s32 timer;
    bool hiScore;
    TT_Ghost ghostR[TT_NUM_TIMES_PER_COURSE];
    TT_Ghost ghostW;
    bool isLevelEntry;
    s32 sparklyMode;
    s32 prevLevelNum;
} sTimeTrials[1];

//
// Files
//

static const char *time_trials_get_player_prefix(s32 pi) {
    switch (pi) {
        case OMM_PLAYER_MARIO: return ""; // No prefix, backwards compatibility
        case OMM_PLAYER_PEACH: return "p_";
        case OMM_PLAYER_LUIGI: return "l_";
        case OMM_PLAYER_WARIO: return "w_";
    }
    return "";
}

static const char *time_trials_get_dirname(sys_path_t dst, TT_Index index) {
    if (index.isSparkly) {
        str_fmt(dst, sizeof(sys_path_t), "%s/time_trials_" TT_SPARKLY_DIR, sys_user_path());
    } else {
        str_fmt(dst, sizeof(sys_path_t), "%s/time_trials_" OMM_GAME_MENU "%s", sys_user_path(),
#if OMM_GAME_IS_SM74
            (index.mi == OMM_SM74_MODE_EXTREME ? "ee" : "")
#else
            ""
#endif
        );
    }
    return dst;
}

static const char *time_trials_get_filename(sys_path_t dst, TT_Index index) {
    sys_path_t dirname;
    if (index.isSparkly) {
        str_fmt(dst, sizeof(sys_path_t), "%s/%s%d.%02d" TT_FILENAME_EXT, time_trials_get_dirname(dirname, index), time_trials_get_player_prefix(index.pi), index.mi, index.ci);
    } else {
        str_fmt(dst, sizeof(sys_path_t), "%s/%s%d.%02d.%d" TT_FILENAME_EXT, time_trials_get_dirname(dirname, index), time_trials_get_player_prefix(index.pi), index.fi, index.ci, index.si);
    }
    return dst;
}

//
// Read
//

static void time_trials_cache_ghost_data(TT_Index index, TT_Frame *ghostData, s32 length) {
    TT_Data *data = time_trials_get(index);
    mem_del(data->data);
    data->data = (ghostData ? mem_dup(ghostData, sizeof(TT_Frame) * length) : mem_new(TT_Frame, 1));
    data->length = length;
    if (length > 0) {
        time_trials_ghost_data_debug_log(index, "Caching %s: %d\n", length);
    }
}

static s32 time_trials_read(TT_Index index, TT_Frame *ghostData) {

    // Free buffer
    if (ghostData) {
        mem_zero(ghostData, sizeof(TT_Ghost));
    }

    // Find in cache
    TT_Data *data = time_trials_get(index);
    if (data->data) {
        mem_cpy(ghostData, data->data, sizeof(TT_Frame) * data->length);
        return data->length;
    }

    // Load from file
    sys_path_t filename;
    FILE *f = fopen(time_trials_get_filename(filename, index), "rb");
    if (f) {
        s32 t = 0;
        if (fread(&t, sizeof(s32), 1, f) && t > 0) {
            if (ghostData) {
                time_trials_ghost_data_debug_log(index, "Loading %s from file: %s\n", filename);
                fread(ghostData, sizeof(TT_Frame), t, f);
                time_trials_cache_ghost_data(index, ghostData, t);
            }
            fclose(f);
            return t;
        }
        fclose(f);
    }

    // No ghost, cache it as empty
    time_trials_cache_ghost_data(index, NULL, 0);
    return 0;
}

//
// Write
//

static void time_trials_write(TT_Index index) {
    if (sTimeTrials->timer > 0 && sTimeTrials->timer < TT_TIME_LIMIT) {
        sys_path_t dirname, filename;
        fs_sys_mkdir(time_trials_get_dirname(dirname, index));
        FILE *f = fopen(time_trials_get_filename(filename, index), "wb");
        if (f) {
            time_trials_ghost_data_debug_log(index, "Writing %s to file: %s\n", filename);
            fwrite(&sTimeTrials->timer, sizeof(s32), 1, f);
            fwrite(sTimeTrials->ghostW, sizeof(TT_Frame), sTimeTrials->timer, f);
            fclose(f);
        }
        time_trials_cache_ghost_data(index, sTimeTrials->ghostW, sTimeTrials->timer);
    }
    time_trials_get(index)->time = sTimeTrials->timer;
}

static void time_trials_write_if_new_record(TT_Index index) {
    TT_CHECK_INDEX(index,);
    s32 t = time_trials_get(index)->time;
    if (t == TT_TIME_UNDEFINED || t > sTimeTrials->timer) {
        time_trials_write(index);
        sTimeTrials->hiScore = true;
    } else {
        sTimeTrials->hiScore = false;
    }
}

//
// Get
//

static s32 time_trials_get_time(s32 playerIndex, s32 fileNum, s32 modeIndex, s32 courseNum, s32 starNum) {
    TT_Index index = TT_INDEX(playerIndex, fileNum, modeIndex, courseNum, starNum);
    TT_CHECK_INDEX(index, TT_TIME_UNDEFINED);
    return time_trials_get(index)->time;
}

static s32 time_trials_get_time_sparkly(s32 playerIndex, s32 sparklyMode, s32 starIndex) {
    TT_Index index = TT_INDEX_SPARKLY(playerIndex, sparklyMode, starIndex);
    TT_CHECK_INDEX(index, TT_TIME_UNDEFINED);
    return time_trials_get(index)->time;
}

static s32 time_trials_get_course_time(s32 playerIndex, s32 fileNum, s32 modeIndex, s32 courseNum) {
    TT_Index index = TT_INDEX(playerIndex, fileNum, modeIndex, courseNum, 1);
    TT_CHECK_INDEX(index, TT_TIME_UNDEFINED);
    s32 courseTime = 0;
    u8 starCourseFlags = omm_stars_get_course_flags(courseNum, modeIndex);
    for (index.si = 0; index.si != TT_NUM_STARS_PER_COURSE; ++index.si) {
        if ((starCourseFlags >> index.si) & 1) {
            s32 t = time_trials_get(index)->time;
            if (t == TT_TIME_UNDEFINED) {
                return TT_TIME_UNDEFINED;
            }
            courseTime += t;
        }
    }
    return courseTime;
}

static s32 time_trials_get_total_time(s32 playerIndex, s32 fileNum, s32 modeIndex) {
    TT_Index index = TT_INDEX(playerIndex, fileNum, modeIndex, COURSE_MIN, 1);
    TT_CHECK_INDEX(index, TT_TIME_UNDEFINED);
    s32 totalTime = 0;
    for (s32 courseNum = COURSE_MIN; courseNum <= COURSE_MAX; ++courseNum) {
        s32 courseTime = time_trials_get_course_time(playerIndex, fileNum, modeIndex, courseNum);
        if (courseTime == TT_TIME_UNDEFINED) {
            return TT_TIME_UNDEFINED;
        }
        totalTime += courseTime;
    }
    return totalTime;
}

static s32 time_trials_get_total_time_sparkly(s32 playerIndex, s32 sparklyMode) {
    TT_Index index = TT_INDEX_SPARKLY(playerIndex, sparklyMode, 0);
    TT_CHECK_INDEX(index, TT_TIME_UNDEFINED);
    s32 totalTime = 0;
    for (s32 starIndex = 0; starIndex != omm_sparkly_get_num_stars(sparklyMode); ++starIndex) {
        s32 starTime = time_trials_get_time_sparkly(playerIndex, sparklyMode, starIndex);
        if (starTime == TT_TIME_UNDEFINED) {
            return TT_TIME_UNDEFINED;
        }
        totalTime += starTime;
    }
    return totalTime;
}

//
// Ghost
//

typedef struct {
    struct Animation anim;
    u8 animationData[0x40000];
    s32 type;
    s32 prevValidAnimIndex;
} TT_Anim;
static TT_Anim sTimeTrialsGhostAnimData[TT_NUM_TIMES_PER_COURSE];

static s32 time_trials_ghost_retrieve_current_animation_index() {
    struct_MarioAnimDmaTable *dmaTable = gMarioAnimDmaTable;
    void *a = gMarioCurrAnimAddr;
    for (s32 i = 0; i != (s32) dmaTable->count; ++i) {
        void *b = dmaTable->srcAddr + dmaTable->anim[i].offset;
        if (a == b) {
            return i;
        }
    }
    return 0;
}

static bool time_trials_ghost_load_animation(TT_Anim *animData, s32 animIndex) {
    struct_MarioAnimDmaTable *dmaTable = gMarioAnimDmaTable;
    if (animIndex < 0 || animIndex >= (s32) dmaTable->count) {
        return false;
    }

    // Raw data
    u8 *addr = dmaTable->srcAddr + dmaTable->anim[animIndex].offset;
    u32 size = dmaTable->anim[animIndex].size;
    mem_cpy(animData->animationData, addr, size);

    // Seek index and values pointers
    struct Animation *anm = (struct Animation *) animData->animationData;
    const u16 *indexStart = (const u16 *) ((uintptr_t) (animData->animationData) + (uintptr_t) (anm->index));
    const s16 *valueStart = (const s16 *) ((uintptr_t) (animData->animationData) + (uintptr_t) (anm->values));

    // Fill ghost anim data
    mem_cpy(&animData->anim, animData->animationData, sizeof(struct Animation));
    animData->anim.index = indexStart;
    animData->anim.values = valueStart;
    return true;
}

static void time_trials_ghost_update_animation(TT_Anim *animData, struct Object *obj, s32 animIndex, s32 animFrame) {
    if (!obj->oCurrAnim) {
        animData->prevValidAnimIndex = -1;
    }

    // Load & set anim
    if (animIndex != animData->prevValidAnimIndex && time_trials_ghost_load_animation(animData, animIndex)) {
        obj->oCurrAnim = &animData->anim;
        obj->oAnimInfo.animAccel = 0;
        animData->prevValidAnimIndex = animIndex;
    }

    // Set frame
    if (obj->oCurrAnim) {
        obj->oAnimFrame = min_s(animFrame, obj->oCurrAnim->mLoopEnd - 1);
    }
}

static void time_trials_ghost_load(s32 playerIndex, s32 fileNum, s32 modeIndex, s32 courseNum, s32 starNum, s32 type) {
    TT_Index index = TT_INDEX(playerIndex, fileNum, modeIndex, courseNum, starNum);
    TT_CHECK_INDEX(index,);

    // Should we load the ghost?
    if ((type == TT_GHOST_MARIO_DEFAULT && !gOmmTimeTrialsShowStarGhosts) ||
        (type == TT_GHOST_MARIO_BOWSER && !gOmmTimeTrialsShowBowserGhosts) ||
        (type == TT_GHOST_MARIO_SLIDE && !gOmmTimeTrialsShowSlideGhosts) ||
        (type == TT_GHOST_MARIO_100_COINS && !gOmmTimeTrialsShowCoinsGhosts) ||
        (type == TT_GHOST_MARIO_ALL_STARS && !gOmmTimeTrialsShowAllStarsGhosts)) {
        return;
    }

    // Load the ghost
    if (time_trials_read(index, sTimeTrials->ghostR[index.si])) {
        mem_zero(&sTimeTrialsGhostAnimData[index.si], sizeof(sTimeTrialsGhostAnimData[index.si]));
        sTimeTrialsGhostAnimData[index.si].type = type;
        sTimeTrialsGhostAnimData[index.si].prevValidAnimIndex = -1;
    }
}

static void time_trials_ghost_load_stars(s32 playerIndex, s32 fileNum, s32 modeIndex, s32 courseNum, s32 numStars, s32 type) {
    u8 starCourseFlags = omm_stars_get_course_flags(courseNum, modeIndex);
    for (s32 si = 0; si != numStars; ++si) {
        if (starCourseFlags & (1 << si)) {
            time_trials_ghost_load(playerIndex, fileNum, modeIndex, courseNum, si + 1, type);
        }
    }
}

static void time_trials_ghost_load_sparkly(s32 playerIndex, s32 sparklyMode, s32 starIndex) {
    TT_Index index = TT_INDEX_SPARKLY(playerIndex, sparklyMode, starIndex);
    TT_CHECK_INDEX(index,);

    // Should we load the ghost?
    if (!gOmmTimeTrialsShowStarGhosts) {
        return;
    }

    // Load the ghost
    if (time_trials_read(index, sTimeTrials->ghostR[index.si])) {
        mem_zero(&sTimeTrialsGhostAnimData[index.si], sizeof(sTimeTrialsGhostAnimData[index.si]));
        sTimeTrialsGhostAnimData[index.si].type = TT_GHOST_MARIO_PINK_GOLD + index.mi;
        sTimeTrialsGhostAnimData[index.si].prevValidAnimIndex = -1;
    }
}

static void time_trials_ghost_update(struct MarioState *m, s32 starNum, s32 frame) {
    TT_Index index = TT_INDEX(0, 1, 0, COURSE_MIN, starNum);
    TT_CHECK_INDEX(index,);

    TT_Frame *ghostData = sTimeTrials->ghostR[index.si] + frame;
    TT_Anim *animData = &sTimeTrialsGhostAnimData[index.si];
    const BehaviorScript *ghostBhv = bhvTimeTrialsGhost[index.si];
    const BehaviorScript *numberBhv = bhvTimeTrialsGhostNumber[index.si];
    struct Object *ghostObj = obj_get_first_with_behavior(ghostBhv);
    struct Object *numberObj = obj_get_first_with_behavior(numberBhv);

    // Should we unload the ghost?
    if ((animData->type == TT_GHOST_MARIO_DEFAULT && !gOmmTimeTrialsShowStarGhosts) ||
        (animData->type == TT_GHOST_MARIO_BOWSER && !gOmmTimeTrialsShowBowserGhosts) ||
        (animData->type == TT_GHOST_MARIO_SLIDE && !gOmmTimeTrialsShowSlideGhosts) ||
        (animData->type == TT_GHOST_MARIO_100_COINS && !gOmmTimeTrialsShowCoinsGhosts) ||
        (animData->type == TT_GHOST_MARIO_ALL_STARS && !gOmmTimeTrialsShowAllStarsGhosts) ||
        (animData->type >= TT_GHOST_MARIO_PINK_GOLD && !gOmmTimeTrialsShowStarGhosts)) {
        if (ghostObj) obj_mark_for_deletion(ghostObj);
        if (numberObj) obj_mark_for_deletion(numberObj);
        return;
    }

    // If timer reached its max or frame data is ended, unload the ghost
    if (frame < 0 || frame >= TT_TIME_LIMIT || !ghostData->levelNum) {
        if (ghostObj) obj_mark_for_deletion(ghostObj);
        if (numberObj) obj_mark_for_deletion(numberObj);
        return;
    }

    // Spawn ghost if not loaded
    if (!ghostObj) {
        ghostObj = spawn_object(m->marioObj, 0, ghostBhv);
        numberObj = spawn_object(m->marioObj, MODEL_NUMBER, numberBhv);
    }

    // Hide ghost if disabled, on first frame or its level or area differs from Mario
    if (frame == 0 || ghostData->levelNum != (u8) gCurrLevelNum || ghostData->areaIndex != (u8) gCurrAreaIndex) {
        ghostObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        numberObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        return;
    }

    // Update ghost
    obj_reset_hitbox(ghostObj, 0, 0, 0, 0, 0, 0);
    obj_set_params(ghostObj, 0, 0, 0, 0, false);
    obj_set_xyz(ghostObj, ghostData->posX, ghostData->posY, ghostData->posZ);
    obj_set_angle(ghostObj, ghostData->pitch * 0x100, ghostData->yaw * 0x100, ghostData->roll * 0x100);
    obj_set_scale(ghostObj, ghostData->scaleX / 100.f, ghostData->scaleY / 100.f, ghostData->scaleZ / 100.f);
    obj_update_gfx(ghostObj);
    ghostObj->oOpacity = 0xA0;
    ghostObj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    ghostObj->oGraphNode = time_trials_get_ghost_mario_graph_node(animData->type);
    time_trials_ghost_update_animation(animData, ghostObj, (u16) ghostData->animIndex, (u16) ghostData->animFrame);

    // Update number
    obj_reset_hitbox(numberObj, 0, 0, 0, 0, 0, 0);
    obj_set_params(numberObj, 0, 0, 0, 0, false);
    obj_set_xyz(numberObj, ghostData->posX, ghostData->posY + ghostData->scaleY * 2.f, ghostData->posZ);
    obj_set_angle(numberObj, 0, 0, 0);
    obj_set_scale(numberObj, 0.8f, 0.8f, 0.8f);
    obj_update_gfx(numberObj);
    numberObj->oAnimState = starNum;

    // Hide number for 100 coins star ghost, Bowser key ghost, All-stars ghost or Sparkly star ghost
    if (animData->type == TT_GHOST_MARIO_100_COINS ||
        animData->type == TT_GHOST_MARIO_BOWSER ||
        animData->type == TT_GHOST_MARIO_ALL_STARS ||
        animData->type >= TT_GHOST_MARIO_PINK_GOLD) {
        numberObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    } else {
        numberObj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    }
}

static void time_trials_ghost_record(struct MarioState *m, s32 frame) {
    if (frame >= 0 && frame < TT_TIME_LIMIT) {
        if (m->marioObj && m->marioObj->oCurrAnim) {
            sTimeTrials->ghostW[frame].posX      = (s16) m->marioObj->oGfxPos[0];
            sTimeTrials->ghostW[frame].posY      = (s16) m->marioObj->oGfxPos[1];
            sTimeTrials->ghostW[frame].posZ      = (s16) m->marioObj->oGfxPos[2];
            sTimeTrials->ghostW[frame].pitch     = (s8) (m->marioObj->oGfxAngle[0] / 0x100);
            sTimeTrials->ghostW[frame].yaw       = (s8) (m->marioObj->oGfxAngle[1] / 0x100);
            sTimeTrials->ghostW[frame].roll      = (s8) (m->marioObj->oGfxAngle[2] / 0x100);
            sTimeTrials->ghostW[frame].scaleX    = (u8) clamp_f(m->marioObj->oGfxScale[0] * 100.f, 0.f, 255.f);
            sTimeTrials->ghostW[frame].scaleY    = (u8) clamp_f(m->marioObj->oGfxScale[1] * 100.f, 0.f, 255.f);
            sTimeTrials->ghostW[frame].scaleZ    = (u8) clamp_f(m->marioObj->oGfxScale[2] * 100.f, 0.f, 255.f);
            sTimeTrials->ghostW[frame].animIndex = (u8) time_trials_ghost_retrieve_current_animation_index();
            sTimeTrials->ghostW[frame].animFrame = (u8) clamp_s(min_s(m->marioObj->oAnimFrame, m->marioObj->oCurrAnim->mLoopEnd - 1), 0, 255);
            sTimeTrials->ghostW[frame].levelNum  = (u8) gCurrLevelNum;
            sTimeTrials->ghostW[frame].areaIndex = (u8) gCurrAreaIndex;
        } else if (frame > 0) {
            sTimeTrials->ghostW[frame]           = sTimeTrials->ghostW[frame - 1];
        }
    }
}

static void time_trials_ghost_unload_all() {
    for (s32 si = 0; si != TT_NUM_TIMES_PER_COURSE; ++si) {
        obj_unload_all_with_behavior(bhvTimeTrialsGhost[si]);
        obj_unload_all_with_behavior(bhvTimeTrialsGhostNumber[si]);
    }
}

//
// Level entry
//

OMM_ROUTINE_LEVEL_ENTRY(time_trials_level_entry) {
    if (omm_is_main_menu()) return;
    sTimeTrials->isLevelEntry = true;
    sTimeTrials->sparklyMode = OMM_SPARKLY_STARS_MODE;

    // Entering or exiting a Bowser fight doesn't reset the timer
    if ((gCurrLevelNum == LEVEL_BOWSER_1 && sTimeTrials->prevLevelNum == LEVEL_BITDW   ) ||
        (gCurrLevelNum == LEVEL_BITDW    && sTimeTrials->prevLevelNum == LEVEL_BOWSER_1) ||
        (gCurrLevelNum == LEVEL_BOWSER_2 && sTimeTrials->prevLevelNum == LEVEL_BITFS   ) ||
        (gCurrLevelNum == LEVEL_BITFS    && sTimeTrials->prevLevelNum == LEVEL_BOWSER_2) ||
        (gCurrLevelNum == LEVEL_BOWSER_3 && sTimeTrials->prevLevelNum == LEVEL_BITS    ) ||
        (gCurrLevelNum == LEVEL_BITS     && sTimeTrials->prevLevelNum == LEVEL_BOWSER_3)) {
        sTimeTrials->isLevelEntry = false;
    }

    // Warping directly to a Bowser fight sets the timer to 9'59"99
    if ((gCurrLevelNum == LEVEL_BOWSER_1 && sTimeTrials->prevLevelNum != LEVEL_BITDW) ||
        (gCurrLevelNum == LEVEL_BOWSER_2 && sTimeTrials->prevLevelNum != LEVEL_BITFS) ||
        (gCurrLevelNum == LEVEL_BOWSER_3 && sTimeTrials->prevLevelNum != LEVEL_BITS)) {
        sTimeTrials->timer = TT_TIME_LIMIT;
        sTimeTrials->isLevelEntry = false;
    }

    sTimeTrials->prevLevelNum = gCurrLevelNum;
}

//
// Init
//

static void time_trials_load_time(TT_Index index) {
    s32 t = time_trials_read(index, NULL);
    if (t <= 0 || t > TT_TIME_LIMIT) {
        t = TT_TIME_UNDEFINED;
    }
    time_trials_get(index)->time = t;
}

static void time_trials_load_all_times() {

    // Regular stars
    for (s32 pi = 0; pi != OMM_NUM_PLAYABLE_CHARACTERS; ++pi)
    for (s32 fi = 0; fi != TT_NUM_SAVE_FILES; ++fi)
    for (s32 mi = 0; mi != TT_NUM_SAVE_MODES; ++mi)
    for (s32 ci = 0; ci != TT_NUM_COURSES; ++ci)
    for (s32 si = 0; si != TT_NUM_TIMES_PER_COURSE; ++si) {
        TT_Index index = { false, pi, fi, mi, ci, si };
        time_trials_load_time(index);
    }

    // Sparkly stars
    for (s32 pi = 0; pi != OMM_NUM_PLAYABLE_CHARACTERS; ++pi)
    for (s32 mi = 0; mi != OMM_SPARKLY_MODE_COUNT - 1; ++mi)
    for (s32 ci = 0; ci != OMM_SPARKLY_STARS_MAX; ++ci) {
        TT_Index index = { true, pi, 0, mi, ci, 0 };
        time_trials_load_time(index);
    }
}

//
// Update
//

static s32 time_trials_get_sparkly_star_index(s32 sparklyMode, s32 levelNum, s32 areaIndex) {
    if (levelNum == OMM_SPARKLY_BOWSER_4_LEVEL && areaIndex == OMM_SPARKLY_BOWSER_4_AREA) {
        return omm_sparkly_get_bowser_4_index(sparklyMode);
    }
    return omm_sparkly_get_star_index_in_level(gOmmSparklyMode, levelNum, areaIndex);
}

static bool time_trials_is_course_valid(s32 levelNum, s32 courseNum, s32 areaIndex) {
    if (sTimeTrials->sparklyMode) {
        return (
            sTimeTrials->sparklyMode == gOmmSparklyMode &&
            time_trials_get_sparkly_star_index(gOmmSparklyMode, levelNum, areaIndex) != -1
        );
    }
    return courseNum != COURSE_NONE;
}

static bool time_trials_should_stop_timer(struct MarioState *m) {
    if (sTimeTrials->sparklyMode) {
        return (
            (m->action == ACT_OMM_SPARKLY_STAR_DANCE) ||
            (m->action == ACT_JUMBO_STAR_CUTSCENE && omm_sparkly_is_bowser_4_battle())
        );
    }
    return (
        (m->action == ACT_STAR_DANCE_EXIT) ||
        (m->action == ACT_STAR_DANCE_NO_EXIT) ||
        (m->action == ACT_STAR_DANCE_WATER) ||
        (m->action == ACT_FALL_AFTER_STAR_GRAB) ||
        (m->action == ACT_JUMBO_STAR_CUTSCENE && !omm_sparkly_is_bowser_4_battle()) ||
        (m->usedObj && m->usedObj->behavior == bhvBowserKey) ||
        (m->interactObj && m->interactObj->behavior == bhvBowserKey) ||
        (m->usedObj && m->usedObj->behavior == bhvGrandStar) ||
        (m->interactObj && m->interactObj->behavior == bhvGrandStar)
    );
}

OMM_ROUTINE_PRE_RENDER(time_trials_update) {
    struct MarioState *m = gMarioState;
    s32 playerIndex = omm_player_get_selected_index();

    // Reset Time Trials state if not available
    if (!time_trials_is_enabled()) {
        time_trials_ghost_unload_all();
        sTimeTrials->timer = TT_TIME_LIMIT;
        sTimeTrials->state = TT_STATE_DISABLED;
        sTimeTrials->isLevelEntry = false;
        return;
    }

    // Load all times at startup
    OMM_DO_ONCE {
        time_trials_load_all_times();
    }

    // Update running ghosts, record ghost data, and advance timer if it's running
    if (sTimeTrials->state == TT_STATE_RUNNING) {
        sTimeTrials->hiScore = false;

        // Update ghosts and advance timer if not paused
        if (!omm_is_game_paused()) {
            for (s32 i = 0; i != (sTimeTrials->sparklyMode ? 1 : TT_NUM_TIMES_PER_COURSE); ++i) {
                time_trials_ghost_update(m, i + 1, sTimeTrials->timer);
            }
            time_trials_ghost_record(m, sTimeTrials->timer);
            sTimeTrials->timer = clamp_s(sTimeTrials->timer + 1, 0, TT_TIME_LIMIT);
        }
    }

    // Runs timer if course is TTable
    if (time_trials_is_course_valid(gCurrLevelNum, gCurrCourseNum, gCurrAreaIndex)) {

        // Start timer and init ghost if a course entry is detected
        if (gMarioObject && sTimeTrials->isLevelEntry) {
            sTimeTrials->state = TT_STATE_RUNNING;
            sTimeTrials->hiScore = false;
            sTimeTrials->timer = 0;
            sTimeTrials->isLevelEntry = false;

            // Reset ghost data
            time_trials_ghost_unload_all();
            mem_zero(sTimeTrials->ghostW, sizeof(sTimeTrials->ghostW));
            mem_zero(sTimeTrials->ghostR, sizeof(sTimeTrials->ghostR));

            // Sparkly star
            // Load the star ghost
            if (sTimeTrials->sparklyMode) {
                s32 starIndex = time_trials_get_sparkly_star_index(gOmmSparklyMode, gCurrLevelNum, gCurrAreaIndex);
                time_trials_ghost_load_sparkly(playerIndex, gOmmSparklyMode, starIndex);
            }

            // Main courses
            // Load selected act ghost or all 6 stars ghosts if Non-Stop and 100 coins ghost
            else if (COURSE_IS_MAIN_COURSE(gCurrCourseNum)) {
                if (OMM_STARS_NON_STOP) {
                    time_trials_ghost_load_stars(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, TT_NUM_STARS_PER_COURSE - 1, TT_GHOST_MARIO_DEFAULT);
                } else {
                    time_trials_ghost_load(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, gCurrActNum, TT_GHOST_MARIO_DEFAULT);
                }
                time_trials_ghost_load(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, TT_STAR_NUM_100_COINS, TT_GHOST_MARIO_100_COINS);
            }

            // Bowser course
            // Load all stars ghosts as Blue ghosts and Bowser fight as Red ghost
            else if (COURSE_IS_BOWSER_COURSE(gCurrCourseNum)) {
                time_trials_ghost_load_stars(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, TT_NUM_STARS_PER_COURSE, TT_GHOST_MARIO_DEFAULT);
#if !OMM_GAME_IS_SMMS
                time_trials_ghost_load(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, TT_STAR_NUM_BOWSER_KEY, TT_GHOST_MARIO_BOWSER);
#endif
            }

            // Sliding level
            // Load all stars ghosts as blue ghosts but Slide star as Green ghost
            else if (gCurrLevelNum == OMM_LEVEL_SLIDE) {
                time_trials_ghost_load_stars(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, TT_NUM_STARS_PER_COURSE, TT_GHOST_MARIO_DEFAULT);
                time_trials_ghost_load(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, 2, TT_GHOST_MARIO_SLIDE);
            }

            // Other courses
            // Load all stars ghosts as blue ghosts
            else {
                time_trials_ghost_load_stars(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, TT_NUM_STARS_PER_COURSE, TT_GHOST_MARIO_DEFAULT);
            }

            // All-stars ghost
            if (!sTimeTrials->sparklyMode && OMM_STARS_NON_STOP && !COURSE_IS_BOWSER_COURSE(gCurrCourseNum)) {
                time_trials_ghost_load(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, TT_NUM_TIMES_PER_COURSE, TT_GHOST_MARIO_ALL_STARS);
            }
        }

        // Stop timer and save time when Mario collects a star, a key or a Grand star
        bool timerIsRunning = sTimeTrials->state == TT_STATE_RUNNING;
        bool shouldStopTimer = time_trials_should_stop_timer(m);
        if (timerIsRunning && shouldStopTimer) {
            s32 starNum = (
                sTimeTrials->sparklyMode ?
                time_trials_get_sparkly_star_index(gOmmSparklyMode, gCurrLevelNum, gCurrAreaIndex) :
                gLastCompletedStarNum
            );

            // Force Mario to leave the course if he collects a main course star
            // Ignore this if OMM Non-Stop mode (or Sparkly stars) is enabled
            if (!sTimeTrials->sparklyMode && COURSE_IS_MAIN_COURSE(gCurrCourseNum) && (!OMM_STARS_NON_STOP || OMM_ALL_STARS)) {
                drop_queued_background_music();
                fadeout_level_music(120);
                m->actionArg = 0;
            }

            // Bowser Key or Grand Star
            if (!sTimeTrials->sparklyMode && OMM_LEVEL_IS_BOWSER_FIGHT(gCurrLevelNum)) {
                starNum = TT_STAR_NUM_BOWSER_KEY;
                sTimeTrials->state = TT_STATE_STOPPED;
            }

            // Write time and ghost data if new record
            if (sTimeTrials->sparklyMode) {
                time_trials_write_if_new_record(TT_INDEX_SPARKLY(playerIndex, gOmmSparklyMode, starNum));
            } else {
                time_trials_write_if_new_record(TT_INDEX(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, starNum));
                if (OMM_STARS_NON_STOP && OMM_ALL_STARS) {
                    time_trials_write_if_new_record(TT_INDEX(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, TT_STAR_NUM_ALL_STARS));
                }
            }

            // Stop the timer if Mario leaves the course
            if (sTimeTrials->sparklyMode || m->actionArg == 0) {
                sTimeTrials->state = TT_STATE_STOPPED;
            }
        }

        // Disable the stopped timer after the star dance
        else if (!timerIsRunning && !shouldStopTimer) {
            sTimeTrials->timer = TT_TIME_LIMIT;
            sTimeTrials->state = TT_STATE_DISABLED;
            time_trials_ghost_unload_all();
        }
    } else {
        sTimeTrials->timer = TT_TIME_LIMIT;
        sTimeTrials->state = TT_STATE_DISABLED;
        time_trials_ghost_unload_all();
    }
}

//
// HUD
//

InterpData gTimeTrialsTimer[1];

#define TT_HUD_TIMER_RUNNING_CLOCK_W        ((OMM_RENDER_GLYPH_SIZE * 8) / 5)
#define TT_HUD_TIMER_RUNNING_CLOCK_X        (OMM_RENDER_TIMER_X - (TT_HUD_TIMER_RUNNING_CLOCK_W * 3) / 2)
#define TT_HUD_TIMER_RUNNING_CLOCK_Y        (SCREEN_HEIGHT - (OMM_RENDER_VALUE_Y + (TT_HUD_TIMER_RUNNING_CLOCK_W - OMM_RENDER_GLYPH_SIZE) / 2) - OMM_RENDER_GLYPH_SIZE)

#define TT_HUD_TIMER_RUNNING_CLOCK_HAND_W   ((TT_HUD_TIMER_RUNNING_CLOCK_W * 3) / 4)
#define TT_HUD_TIMER_RUNNING_CLOCK_HAND_X   (TT_HUD_TIMER_RUNNING_CLOCK_X + (TT_HUD_TIMER_RUNNING_CLOCK_W / 2))
#define TT_HUD_TIMER_RUNNING_CLOCK_HAND_Y   (TT_HUD_TIMER_RUNNING_CLOCK_Y + (TT_HUD_TIMER_RUNNING_CLOCK_W / 2))

#define TT_HUD_TIMER_RUNNING_DIGIT_W        (OMM_RENDER_GLYPH_SIZE)
#define TT_HUD_TIMER_RUNNING_DIGIT_X        (OMM_RENDER_TIMER_X)
#define TT_HUD_TIMER_RUNNING_DIGIT_Y        (SCREEN_HEIGHT - OMM_RENDER_VALUE_Y - OMM_RENDER_GLYPH_SIZE)

#define TT_HUD_TIMER_NEW_RECORD_CROWN_W     ((OMM_RENDER_GLYPH_SIZE * 8) / 5)
#define TT_HUD_TIMER_NEW_RECORD_CROWN_X     (OMM_RENDER_TIMER_X - (TT_HUD_TIMER_RUNNING_CLOCK_W * 3) / 2)
#define TT_HUD_TIMER_NEW_RECORD_CROWN_Y     (SCREEN_HEIGHT - (OMM_RENDER_VALUE_Y + (TT_HUD_TIMER_RUNNING_CLOCK_W - OMM_RENDER_GLYPH_SIZE) / 2) - OMM_RENDER_GLYPH_SIZE + 1)

#define TT_HUD_TIMER_STAR_SELECT_CROWN_W    ((OMM_RENDER_GLYPH_SIZE * 6) / 5)
#define TT_HUD_TIMER_STAR_SELECT_CROWN_X    ((SCREEN_WIDTH / 2) - 8)
#define TT_HUD_TIMER_STAR_SELECT_CROWN_Y    (OMM_RENDER_STAR_SELECT_SCORE_Y + OMM_RENDER_GLYPH_SIZE * 2 - ((OMM_RENDER_GLYPH_SIZE - 8) / 2))

#define TT_HUD_TIMER_STAR_SELECT_DIGIT_W    (OMM_RENDER_GLYPH_SIZE)
#define TT_HUD_TIMER_STAR_SELECT_DIGIT_X    (TT_HUD_TIMER_STAR_SELECT_CROWN_X + 1 + (OMM_RENDER_VALUE_NUMBER_X - OMM_RENDER_VALUE_GLYPH_X))
#define TT_HUD_TIMER_STAR_SELECT_DIGIT_Y    (TT_HUD_TIMER_STAR_SELECT_CROWN_Y + (TT_HUD_TIMER_STAR_SELECT_CROWN_W - TT_HUD_TIMER_STAR_SELECT_DIGIT_W) / 2)

static Gfx *time_trials_render_timer_clock_hand(Gfx *pos, Vtx *vtx, f32 timer) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    s16 handW = TT_HUD_TIMER_RUNNING_CLOCK_HAND_W;
    s16 handX = TT_HUD_TIMER_RUNNING_CLOCK_HAND_X;
    s16 handY = TT_HUD_TIMER_RUNNING_CLOCK_HAND_Y;
    s16 handR = ((timer * 0x10000) / 30.f);
    vtx[0] = (Vtx) {{{ handX + handW * coss(0xA000 - handR), handY + handW * sins(0xA000 - handR), 0 }, 0, { 0x0000, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[1] = (Vtx) {{{ handX + handW * coss(0xE000 - handR), handY + handW * sins(0xE000 - handR), 0 }, 0, { 0x2000, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[2] = (Vtx) {{{ handX + handW * coss(0x2000 - handR), handY + handW * sins(0x2000 - handR), 0 }, 0, { 0x2000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    vtx[3] = (Vtx) {{{ handX + handW * coss(0x6000 - handR), handY + handW * sins(0x6000 - handR), 0 }, 0, { 0x0000, 0x0000 }, { 0xFF, 0xFF, 0xFF, 0xFF }}};
    gDPSetTexturePersp(pos++, G_TP_NONE);
    gDPSetRenderMode(pos++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetCombineLERP(pos++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetTextureFilter(pos++, G_TF_POINT);
    gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPLoadTextureBlock(pos++, OMM_TEXTURE_HUD_CLOCK_HAND, G_IM_FMT_RGBA, G_IM_SIZ_32b, 256, 256, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gSPVertex(pos++, vtx, 4, 0);
    gSP2Triangles(pos++, 0, 1, 2, 0, 0, 2, 3, 0);
    gDPSetTexturePersp(pos++, G_TP_PERSP);
    gDPSetRenderMode(pos++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineLERP(pos++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
    gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetTextureFilter(pos++, G_TF_BILERP);
    gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    return pos;
}

static Gfx *time_trials_render_timer_digits(Gfx *pos, f32 timer, s16 x, s16 y, s16 w, f32 color, f32 alpha) {
    static const char *TIME_TRIALS_TIMER_GLYPHS[] = {
        OMM_TEXTURE_HUD_0, OMM_TEXTURE_HUD_1,
        OMM_TEXTURE_HUD_2, OMM_TEXTURE_HUD_3,
        OMM_TEXTURE_HUD_4, OMM_TEXTURE_HUD_5,
        OMM_TEXTURE_HUD_6, OMM_TEXTURE_HUD_7,
        OMM_TEXTURE_HUD_8, OMM_TEXTURE_HUD_9,
        OMM_TEXTURE_HUD_M, OMM_TEXTURE_HUD_S,
    };
    OMM_RENDER_DEFINE_GLYPH_SIZE(w);

    s32 ms = timer >= TT_TIME_LIMIT ? 599999 : (s32) (1000.f * (timer / 30.f));
    s32 glyphs[7] = {
        ((ms /  60000) % 10),
        10,
        ((ms /  10000) %  6),
        ((ms /   1000) % 10),
        11,
        ((ms /    100) % 10),
        ((ms /     10) % 10),
    };
    for (s32 i = 0, j = 0; i != 7; ++i) {
        j = max_s(j, glyphs[i] * (glyphs[i] < 10));
        s16 dx = i * OMM_RENDER_TIMER_OFFSET_X;
        gDPSetTexturePersp(pos++, G_TP_NONE);
        gDPSetRenderMode(pos++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
        gDPSetCombineLERP(pos++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
        gDPSetEnvColor(pos++, 0xFF * color, 0xFF * color, 0xFF * color, (0xFF - (0xAA * (j == 0))) * alpha);
        gDPSetTextureFilter(pos++, G_TF_POINT);
        gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(pos++, TIME_TRIALS_TIMER_GLYPHS[glyphs[i]], G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
        gSPTextureRectangle(pos++, (x + dx) << 2, (SCREEN_HEIGHT - TT_HUD_TIMER_RUNNING_DIGIT_W - y) << 2, (x + dx + TT_HUD_TIMER_RUNNING_DIGIT_W) << 2, (SCREEN_HEIGHT - y) << 2, G_TX_RENDERTILE, 0, 0, (0x4000 / TT_HUD_TIMER_RUNNING_DIGIT_W), (0x4000 / TT_HUD_TIMER_RUNNING_DIGIT_W));
        gDPSetTexturePersp(pos++, G_TP_PERSP);
        gDPSetRenderMode(pos++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        gDPSetCombineLERP(pos++, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE);
        gDPSetEnvColor(pos++, 0xFF, 0xFF, 0xFF, 0xFF);
        gDPSetTextureFilter(pos++, G_TF_BILERP);
        gSPTexture(pos++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    }
    return pos;
}

static void time_trials_render_clock_or_crown(const char *texture, s16 x, s16 y, s16 w, f32 color, f32 alpha) {
    omm_render_texrect(x, y, w, w, 16, 16, 0xFF * color, 0xFF * color, 0xFF * color, 0xFF * alpha, texture, false);
}

Gfx *time_trials_render_timer_interpolated(Gfx *pos, Vtx *vtx, f32 timer) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    pos = time_trials_render_timer_clock_hand(pos, vtx, timer);
    pos = time_trials_render_timer_digits(pos, timer, TT_HUD_TIMER_RUNNING_DIGIT_X, TT_HUD_TIMER_RUNNING_DIGIT_Y, OMM_RENDER_GLYPH_SIZE, 1.f, 1.f);
    return pos;
}

static void time_trials_render_timer_running() {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    Vtx *vtx = omm_alloc_vtx(4);
    time_trials_render_clock_or_crown(OMM_TEXTURE_HUD_CLOCK, TT_HUD_TIMER_RUNNING_CLOCK_X, TT_HUD_TIMER_RUNNING_CLOCK_Y, TT_HUD_TIMER_RUNNING_CLOCK_W, 1.f, 1.f);
    interp_data_update(gTimeTrialsTimer, gTimeTrialsTimer->s1 >= (f32) gGlobalTimer - 1, gDisplayListHead, NULL, vtx, 0, 0, 0, 0, gGlobalTimer, sTimeTrials->timer);
    gDisplayListHead = time_trials_render_timer_interpolated(gDisplayListHead, gTimeTrialsTimer->vtx, gTimeTrialsTimer->t0);
}

static void time_trials_render_timer_new_record(f32 color) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(gOmmHudSize);
    time_trials_render_clock_or_crown(OMM_TEXTURE_HUD_CROWN, TT_HUD_TIMER_NEW_RECORD_CROWN_X, TT_HUD_TIMER_NEW_RECORD_CROWN_Y, TT_HUD_TIMER_NEW_RECORD_CROWN_W, color, 1.f);
    interp_data_reset(gTimeTrialsTimer);
    gDisplayListHead = time_trials_render_timer_digits(gDisplayListHead, sTimeTrials->timer, TT_HUD_TIMER_RUNNING_DIGIT_X, TT_HUD_TIMER_RUNNING_DIGIT_Y, OMM_RENDER_GLYPH_SIZE, color, 1.f);
}

static void time_trials_render_timer_star_select(s32 timer, f32 alpha) {
    OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);
    time_trials_render_clock_or_crown(OMM_TEXTURE_HUD_CROWN, TT_HUD_TIMER_STAR_SELECT_CROWN_X, TT_HUD_TIMER_STAR_SELECT_CROWN_Y, TT_HUD_TIMER_STAR_SELECT_CROWN_W, 1.f, alpha);
    interp_data_reset(gTimeTrialsTimer);
    gDisplayListHead = time_trials_render_timer_digits(gDisplayListHead, timer, TT_HUD_TIMER_STAR_SELECT_DIGIT_X, TT_HUD_TIMER_STAR_SELECT_DIGIT_Y, OMM_RENDER_GLYPH_SIZE, 1.f, alpha);
}

void time_trials_render_hud_timer() {
    if (sTimeTrials->state != TT_STATE_DISABLED) {
        if (sTimeTrials->hiScore && sTimeTrials->state == TT_STATE_STOPPED) {
            time_trials_render_timer_new_record(relerp_0_1_f(sins(gGlobalTimer * 0x1000), -1.f, 1.f, 0.5f, 1.f));
        } else if (sTimeTrials->timer > 0) {
            time_trials_render_timer_running();
        } else {
            interp_data_reset(gTimeTrialsTimer);
        }
    }
}

void time_trials_render_star_select_time(s32 starIndex, u8 alpha) {
    if (time_trials_is_enabled()) {
        s32 playerIndex = omm_player_get_selected_index();
        s32 bestTime = time_trials_get_time(playerIndex, gCurrSaveFileNum, OMM_GAME_MODE, gCurrCourseNum, starIndex + 1);
        if (bestTime != TT_TIME_UNDEFINED) {
            OMM_RENDER_DEFINE_GLYPH_SIZE(OMM_RENDER_GLYPH_SIZE_DEFAULT);
            u8 *bestTimeText = omm_text_convert(OMM_TEXT_TT_BEST_TIME, false);
            s32 bestTimeX = (SCREEN_WIDTH / 2) - (omm_render_get_string_width(bestTimeText) + 11);
            omm_render_string(bestTimeX - 1, OMM_RENDER_STAR_SELECT_SCORE_Y + OMM_RENDER_GLYPH_SIZE * 2,     0x40, 0x40, 0x00, alpha, bestTimeText, false);
            omm_render_string(bestTimeX + 1, OMM_RENDER_STAR_SELECT_SCORE_Y + OMM_RENDER_GLYPH_SIZE * 2,     0x40, 0x40, 0x00, alpha, bestTimeText, false);
            omm_render_string(bestTimeX,     OMM_RENDER_STAR_SELECT_SCORE_Y + OMM_RENDER_GLYPH_SIZE * 2 - 1, 0x40, 0x40, 0x00, alpha, bestTimeText, false);
            omm_render_string(bestTimeX,     OMM_RENDER_STAR_SELECT_SCORE_Y + OMM_RENDER_GLYPH_SIZE * 2 + 1, 0x40, 0x40, 0x00, alpha, bestTimeText, false);
            omm_render_string(bestTimeX,     OMM_RENDER_STAR_SELECT_SCORE_Y + OMM_RENDER_GLYPH_SIZE * 2,     0xFF, 0xFF, 0x00, alpha, bestTimeText, false);
            time_trials_render_timer_star_select(bestTime, alpha / 255.f);
        }
    }
}

//
// Pause menu
//

#define TT_PAUSE_NUM_LINES                  (OMM_SPARKLY_MODE_IS_ENABLED ? 6 : 4)

#define TT_PAUSE_BACKGROUND_W               (OMM_SPARKLY_MODE_IS_ENABLED ? 256 : 300)
#define TT_PAUSE_BACKGROUND_H               (TT_PAUSE_NUM_LINES * TT_PAUSE_OFFSET_LINE_Y + 2 * TT_PAUSE_OFFSET_LINE_3_2_Y + 2 * TT_PAUSE_OFFSET_Y - 8)
#define TT_PAUSE_BACKGROUND_X               (SCREEN_WIDTH / 2)
#define TT_PAUSE_BACKGROUND_Y               (SCREEN_HEIGHT / 2 - 16)

#define TT_PAUSE_TITLE_Y                    (TT_PAUSE_BACKGROUND_Y + TT_PAUSE_BACKGROUND_H / 2 + 20)
#define TT_PAUSE_TRIANGLE_UP_Y              (TT_PAUSE_BACKGROUND_Y + TT_PAUSE_BACKGROUND_H / 2 + 4)
#define TT_PAUSE_TRIANGLE_DOWN_Y            (TT_PAUSE_BACKGROUND_Y - TT_PAUSE_BACKGROUND_H / 2 - 12)
#define TT_PAUSE_TRIANGLE_LEFT_X            (TT_PAUSE_BACKGROUND_X - TT_PAUSE_BACKGROUND_W / 2 - 12)
#define TT_PAUSE_TRIANGLE_RIGHT_X           (TT_PAUSE_BACKGROUND_X + TT_PAUSE_BACKGROUND_W / 2 + 4)

#define TT_PAUSE_OFFSET_X                   (10)
#define TT_PAUSE_OFFSET_Y                   (10)
#define TT_PAUSE_OFFSET_LINE_Y              (16)
#define TT_PAUSE_OFFSET_LINE_3_2_Y          ((TT_PAUSE_OFFSET_LINE_Y * 3) / 2)

#define TT_PAUSE_LEFT_X                     (TT_PAUSE_BACKGROUND_X - TT_PAUSE_BACKGROUND_W / 2 + TT_PAUSE_OFFSET_X)
#define TT_PAUSE_LEFT_TIME_X                (TT_PAUSE_BACKGROUND_X - TT_PAUSE_OFFSET_X)
#define TT_PAUSE_RIGHT_X                    (TT_PAUSE_BACKGROUND_X + TT_PAUSE_OFFSET_X)
#define TT_PAUSE_RIGHT_TIME_X               (TT_PAUSE_BACKGROUND_X + TT_PAUSE_BACKGROUND_W / 2 - TT_PAUSE_OFFSET_X)

#define TT_PAUSE_COURSE_NAME_Y              (TT_PAUSE_BACKGROUND_Y + TT_PAUSE_BACKGROUND_H / 2 - TT_PAUSE_OFFSET_Y - 8)
#define TT_PAUSE_STAR_Y0                    (TT_PAUSE_COURSE_NAME_Y - TT_PAUSE_OFFSET_LINE_3_2_Y)
#define TT_PAUSE_ALL_STARS_Y                (TT_PAUSE_STAR_Y0 - (TT_PAUSE_NUM_LINES - 1) * TT_PAUSE_OFFSET_LINE_Y)
#define TT_PAUSE_COURSE_Y                   (TT_PAUSE_STAR_Y0 - (TT_PAUSE_NUM_LINES - 1) * TT_PAUSE_OFFSET_LINE_Y - TT_PAUSE_OFFSET_LINE_3_2_Y)
#define TT_PAUSE_TOTAL_Y                    (TT_PAUSE_COURSE_Y)

static Scroll sTimeTrialsScrollCourse  = {0};
static Scroll sTimeTrialsScrollSparkly = {0};
static Scroll sTimeTrialsScrollPlayer  = {0};

static s16 time_trials_render_time_char(s16 x, s16 y, s16 w, u8 rgb, u8 alpha, u8 c) {
    u8 text[] = { c, 0xFF };
    omm_render_string(x - w, y, rgb, rgb, rgb, alpha, text, false);
    return w;
}

static void time_trials_render_time(s16 x, s16 y, u8 alpha, s32 time, s32 star) {
    s32 time_ms = (s32) (1000.f * (time / 30.f));
    u8 time_rgb = (time == TT_TIME_UNDEFINED ? 0x80 : 0xFF);

    x -= time_trials_render_time_char(x, y, 7, time_rgb, alpha, time == TT_TIME_UNDEFINED ? 0x9F : ((time_ms / 10) % 10));
    x -= time_trials_render_time_char(x, y, 7, time_rgb, alpha, time == TT_TIME_UNDEFINED ? 0x9F : ((time_ms / 100) % 10));
    x -= time_trials_render_time_char(x, y, 7, time_rgb, alpha, 0xF6);
    x -= time_trials_render_time_char(x, y, 7, time_rgb, alpha, time == TT_TIME_UNDEFINED ? 0x9F : ((time_ms / 1000) % 10));
    x -= time_trials_render_time_char(x, y, 7, time_rgb, alpha, time == TT_TIME_UNDEFINED ? 0x9F : ((time_ms / 10000) % 6));
    x -= time_trials_render_time_char(x, y, 4, time_rgb, alpha, 0x3E);
    x -= time_trials_render_time_char(x, y, 7, time_rgb, alpha, time == TT_TIME_UNDEFINED ? 0x9F : ((time_ms / 60000) % 10));
    for (time_ms /= 600000; time_ms; time_ms /= 10) {
    x -= time_trials_render_time_char(x, y, 7, time_rgb, alpha, time_ms % 10);
    }
    if (star >= 0) {
    x -= time_trials_render_time_char(x, y, 14, 0x80 + 0x7F * (star == 1), alpha, DIALOG_CHAR_STAR_FILLED);
    }
}

static void time_trials_render_pause_castle_courses(u8 alpha) {

    // Handle menu scrolling
    omm_render_update_scroll(&sTimeTrialsScrollCourse, TT_NUM_COURSES, -gPlayer1Controller->stickY);
    s32 dir = sign_s(sTimeTrialsScrollCourse.inc);
    for (;;) {
        sTimeTrialsScrollCourse.idx = (sTimeTrialsScrollCourse.idx + TT_NUM_COURSES) % TT_NUM_COURSES;
        if (omm_stars_get_course_flags(sTimeTrialsScrollCourse.idx + 1, OMM_GAME_MODE)) { break; }
        sTimeTrialsScrollCourse.idx += dir;
    }
    s32 courseNum = sTimeTrialsScrollCourse.idx + 1;
    s32 levelNum = omm_level_from_course(courseNum);

    // Course name
    ustr_t textCourseName;
    omm_level_get_course_name(textCourseName, levelNum, OMM_GAME_MODE, false, true);
    omm_render_string(TT_PAUSE_LEFT_X, TT_PAUSE_COURSE_NAME_Y, 0xFF, 0xFF, 0xFF, alpha, textCourseName + (courseNum < 10), true);

    // Stars 1-7
    u8 starCourseFlags = omm_stars_get_course_flags(courseNum, OMM_GAME_MODE);
    for (s32 starIndex = 0, k = 0; starIndex != OMM_NUM_STARS_MAX_PER_COURSE; ++starIndex) {
        if (starCourseFlags & (1 << starIndex)) {
            const char *stringStar = NULL;
#if OMM_GAME_IS_SMSR
            const BehaviorScript *bhv = NULL;
            if (omm_stars_get_star_data(levelNum, 1, starIndex, &bhv, NULL) && bhv == bhvCustomSMSRStarReplica) {
                stringStar = OMM_TEXT_TT_STAR_REPLICA;
            } else
#endif
            if (COURSE_IS_MAIN_COURSE(courseNum) && starIndex == 6) {
                stringStar = OMM_TEXT_TT_STAR_COIN;
            } else switch (k) {
                case 0: stringStar = OMM_TEXT_TT_STAR_1; break;
                case 1: stringStar = OMM_TEXT_TT_STAR_2; break;
                case 2: stringStar = OMM_TEXT_TT_STAR_3; break;
                case 3: stringStar = OMM_TEXT_TT_STAR_4; break;
                case 4: stringStar = OMM_TEXT_TT_STAR_5; break;
                case 5: stringStar = OMM_TEXT_TT_STAR_6; break;
                case 6: stringStar = OMM_TEXT_TT_STAR_7; break;
            }

            // Star
            const u8 *textStar = omm_text_convert(stringStar, false);
            s16 x = ((k & 1) ? TT_PAUSE_RIGHT_X : TT_PAUSE_LEFT_X);
            s16 y = TT_PAUSE_STAR_Y0 - (k >> 1) * TT_PAUSE_OFFSET_LINE_Y;
            omm_render_string(x, y, 0xFF, 0xFF, 0xFF, alpha, textStar, false);

            // Time
            s32 time = time_trials_get_time(sTimeTrialsScrollPlayer.idx, gCurrSaveFileNum, OMM_GAME_MODE, courseNum, starIndex + 1);
            s32 star = ((omm_save_file_get_star_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, courseNum - 1) >> starIndex) & 1);
            time_trials_render_time((k & 1) ? TT_PAUSE_RIGHT_TIME_X : TT_PAUSE_LEFT_TIME_X, y, alpha, time, star);
            k++;
        }
    }

    // All Stars
    if (!COURSE_IS_BOWSER_COURSE(courseNum)) {
        const u8 *textAllStars = omm_text_convert(OMM_TEXT_TT_ALL_STARS, false);
        omm_render_string(TT_PAUSE_RIGHT_X, TT_PAUSE_ALL_STARS_Y, 0xFF, 0xFF, 0x00, alpha, textAllStars, false);
        s32 time = time_trials_get_time(sTimeTrialsScrollPlayer.idx, gCurrSaveFileNum, OMM_GAME_MODE, courseNum, TT_STAR_NUM_ALL_STARS);
        time_trials_render_time(TT_PAUSE_RIGHT_TIME_X, TT_PAUSE_ALL_STARS_Y, alpha, time, -1);
    }

#if OMM_LEVEL_HAS_BOWSER
    // Bowser
    else {
        const u8 *textBowser = omm_text_convert(OMM_TEXT_TT_BOWSER, false);
        omm_render_string(TT_PAUSE_RIGHT_X, TT_PAUSE_ALL_STARS_Y, 0xFF, 0x80, 0x00, alpha, textBowser, false);
        s32 time = time_trials_get_time(sTimeTrialsScrollPlayer.idx, gCurrSaveFileNum, OMM_GAME_MODE, courseNum, TT_STAR_NUM_BOWSER_KEY);
        time_trials_render_time(TT_PAUSE_RIGHT_TIME_X, TT_PAUSE_ALL_STARS_Y, alpha, time, -1);
    }
#endif

    // Course time
    const u8 *textCourse = omm_text_convert(OMM_TEXT_TT_COURSE, false);
    omm_render_string(TT_PAUSE_LEFT_X, TT_PAUSE_COURSE_Y, 0xFF, 0xFF, 0xFF, alpha, textCourse, true);
    s32 courseTime = time_trials_get_course_time(sTimeTrialsScrollPlayer.idx, gCurrSaveFileNum, OMM_GAME_MODE, courseNum);
    time_trials_render_time(TT_PAUSE_LEFT_TIME_X, TT_PAUSE_COURSE_Y, alpha, courseTime, -1);

    // Total time
    const u8 *textTotal = omm_text_convert(OMM_TEXT_TT_TOTAL, false);
    omm_render_string(TT_PAUSE_RIGHT_X, TT_PAUSE_TOTAL_Y, 0xFF, 0xFF, 0xFF, alpha, textTotal, true);
    s32 totalTime = time_trials_get_total_time(sTimeTrialsScrollPlayer.idx, gCurrSaveFileNum, OMM_GAME_MODE);
    time_trials_render_time(TT_PAUSE_RIGHT_TIME_X, TT_PAUSE_TOTAL_Y, alpha, totalTime, -1);
}

static void time_trials_render_pause_castle_sparkly_timer(s16 x, s16 y, u8 alpha, s32 sparklyMode) {
    s32 timer = omm_sparkly_get_timer(sparklyMode);
    s32 time_s = timer / 30;
    u8 allStars = omm_sparkly_is_grand_star_collected(sparklyMode);
    u8 blink = alpha / ((!allStars && (timer % 30) >= 15) ? 3 : 1);

    x -= time_trials_render_time_char(x, y, 7, 0xFF, alpha, (time_s / 1) % 10);
    x -= time_trials_render_time_char(x, y, 7, 0xFF, alpha, (time_s / 10) % 6);
    x -= time_trials_render_time_char(x, y, 5, 0xFF, blink, 0xE6);
    x -= time_trials_render_time_char(x, y, 7, 0xFF, alpha, (time_s / 60) % 10);
    x -= time_trials_render_time_char(x, y, 7, 0xFF, alpha, (time_s / 600) % 6);
    x -= time_trials_render_time_char(x, y, 5, 0xFF, blink, 0xE6);
    x -= time_trials_render_time_char(x, y, 7, 0xFF, alpha, (time_s / 3600) % 10);
    x -= time_trials_render_time_char(x, y, 7, 0xFF, alpha, (time_s / 36000));
    x -= time_trials_render_time_char(x, y, 14, 0x80 + 0x7F * allStars, alpha, DIALOG_CHAR_STAR_FILLED);
}

static void time_trials_render_pause_castle_sparkly_stars(u8 alpha) {
    // TODO: change the logic when nebulas
    s32 sparklyMode = gOmmSparklyMode;
    s32 numStars = omm_sparkly_get_num_stars(sparklyMode);
    s32 numPages = numStars / 6;

    // Handle menu scrolling
    omm_render_update_scroll(&sTimeTrialsScrollSparkly, numPages, -gPlayer1Controller->stickY);

    // Mode name
    const u8 *textModeName = omm_text_convert(OMM_SPARKLY_TEXT_STARS[sparklyMode], false);
    u8 sparklyR = OMM_SPARKLY_HUD_COLOR_R[sparklyMode];
    u8 sparklyG = OMM_SPARKLY_HUD_COLOR_G[sparklyMode];
    u8 sparklyB = OMM_SPARKLY_HUD_COLOR_B[sparklyMode];
    omm_render_string(TT_PAUSE_LEFT_X, TT_PAUSE_COURSE_NAME_Y, sparklyR, sparklyG, sparklyB, alpha, textModeName, true);

    // Stars
    for (s32 i = 0; i != 6; ++i) {
        s32 starIndex = sTimeTrialsScrollSparkly.idx * 6 + i;
        s16 y = TT_PAUSE_STAR_Y0 - i * TT_PAUSE_OFFSET_LINE_Y;

        // Level name
        ustr_t textLevelName; u8 rgb = 0xFF;
        if (omm_sparkly_get_level_name(textLevelName, sparklyMode, starIndex)) {
            if (!((starIndex == numStars - 1 && omm_sparkly_is_grand_star_collected(sparklyMode)) || omm_sparkly_is_star_collected(sparklyMode, starIndex))) {
                rgb = 0x80;
                if (starIndex == numStars - 1 && !omm_sparkly_is_completed(sparklyMode)) {
                    omm_text_copy(textLevelName, sizeof(textLevelName), omm_text_convert(OMM_TEXT_UNKNOWN, false));
                }
            }
        }
        omm_render_string(TT_PAUSE_LEFT_X, y, rgb, rgb, rgb, alpha, textLevelName, false);

        // Time
        s32 time = time_trials_get_time_sparkly(sTimeTrialsScrollPlayer.idx, sparklyMode, starIndex);
        time_trials_render_time(TT_PAUSE_RIGHT_TIME_X, y, alpha, time, rgb == 0xFF);
    }

    // Current time
    const u8 *textTimer = omm_text_convert(OMM_TEXT_TT_TIMER, false);
    omm_render_string(TT_PAUSE_LEFT_X, TT_PAUSE_COURSE_Y, sparklyR, sparklyG, sparklyB, alpha, textTimer, true);
    time_trials_render_pause_castle_sparkly_timer(TT_PAUSE_LEFT_TIME_X, TT_PAUSE_COURSE_Y, alpha, sparklyMode);

    // Total time
    const u8 *textTotal = omm_text_convert(OMM_TEXT_TT_TOTAL, false);
    omm_render_string(TT_PAUSE_RIGHT_X, TT_PAUSE_TOTAL_Y, 0xFF, 0xFF, 0xFF, alpha, textTotal, true);
    s32 totalTime = time_trials_get_total_time_sparkly(sTimeTrialsScrollPlayer.idx, sparklyMode);
    time_trials_render_time(TT_PAUSE_RIGHT_TIME_X, TT_PAUSE_TOTAL_Y, alpha, totalTime, -1);
}

bool time_trials_render_pause_castle(u8 alpha) {
    if (!time_trials_is_enabled()) {
        return false;
    }

    // Time Trials
    u8 *textTimeTrials = omm_text_convert(OMM_TEXT_TT_TITLE, false);
    omm_render_string_hud_centered(TT_PAUSE_TITLE_Y, 0xFF, 0xFF, 0xFF, alpha, textTimeTrials, false);

    // Black background
    s16 bgx = TT_PAUSE_BACKGROUND_X - (TT_PAUSE_BACKGROUND_W / 2);
    s16 bgy = TT_PAUSE_BACKGROUND_Y - (TT_PAUSE_BACKGROUND_H / 2);
    omm_render_texrect(bgx, bgy, TT_PAUSE_BACKGROUND_W, TT_PAUSE_BACKGROUND_H, 32, 32, 0x00, 0x00, 0x00, alpha / 2, OMM_TEXTURE_MISC_WHITE, false);

    // Vertical arrows
    s16 vaxl = TT_PAUSE_BACKGROUND_X - 8;
    s16 vaxr = TT_PAUSE_BACKGROUND_X;
    s16 vayu = TT_PAUSE_TRIANGLE_UP_Y;
    s16 vayd = TT_PAUSE_TRIANGLE_DOWN_Y;
    omm_render_texrect(vaxl, vayu, 8, 8, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MISC_WHITE_DOWN_RIGHT, false);
    omm_render_texrect(vaxr, vayu, 8, 8, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MISC_WHITE_DOWN_LEFT, false);
    omm_render_texrect(vaxl, vayd, 8, 8, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MISC_WHITE_UP_RIGHT, false);
    omm_render_texrect(vaxr, vayd, 8, 8, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MISC_WHITE_UP_LEFT, false);

    // Horizontal arrows
    s16 haxl = TT_PAUSE_TRIANGLE_LEFT_X;
    s16 haxr = TT_PAUSE_TRIANGLE_RIGHT_X;
    s16 hayu = TT_PAUSE_BACKGROUND_Y;
    s16 hayd = TT_PAUSE_BACKGROUND_Y - 8;
    omm_render_texrect(haxl, hayu, 8, 8, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MISC_WHITE_DOWN_RIGHT, false);
    omm_render_texrect(haxl, hayd, 8, 8, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MISC_WHITE_UP_RIGHT, false);
    omm_render_texrect(haxr, hayu, 8, 8, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MISC_WHITE_DOWN_LEFT, false);
    omm_render_texrect(haxr, hayd, 8, 8, 32, 32, 0xFF, 0xFF, 0xFF, 0xFF, OMM_TEXTURE_MISC_WHITE_UP_LEFT, false);

    // Player name
    omm_render_update_scroll(&sTimeTrialsScrollPlayer, OMM_NUM_PLAYABLE_CHARACTERS, gPlayer1Controller->stickX);
    const u8 *textPlayerName = omm_text_convert(omm_player_properties_get_name_upper(sTimeTrialsScrollPlayer.idx), false);
    u32 playerRGBA = omm_player_properties_get_color(sTimeTrialsScrollPlayer.idx);
    omm_render_string(
        TT_PAUSE_RIGHT_TIME_X - omm_render_get_string_width(textPlayerName), TT_PAUSE_COURSE_NAME_Y,
        (playerRGBA >> 24) & 0xFF, (playerRGBA >> 16) & 0xFF, (playerRGBA >> 8) & 0xFF, alpha,
        textPlayerName, true
    );

    // Courses or Sparkly stars
    if (OMM_SPARKLY_MODE_IS_ENABLED) {
        time_trials_render_pause_castle_sparkly_stars(alpha);
    } else {
        time_trials_render_pause_castle_courses(alpha);
    }

    return true;
}
