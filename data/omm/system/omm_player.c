#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static s32 sOmmPlayer = OMM_PLAYER_MARIO;

//
// Properties
//

typedef struct {
    f32 ground;  // affects ground speed, except sliding speed
    f32 walk;    // affects ground speed if walking
    f32 air;     // affects horizontal air speed and act_flying
    f32 swim;    // affects swimming speed
    f32 jump;    // affects jumps initial velocity
    f32 gravity; // affects gravity if velocity is negative
    f32 slide;   // affects ground acceleration while sliding
} OmmPlayerPhysics;

typedef struct { s32 id; const GeoLayout *geo; } IdGeo;
typedef struct {
    IdGeo body;
    IdGeo cap;
    IdGeo wcap;
    IdGeo mcap;
    IdGeo wmcap;
} OmmPlayerGraphics;

typedef struct {
    const char *name;
    u32 color;
    void *anims;
    OmmPlayerPhysics physics;
    OmmPlayerGraphics graphics;
} OmmPlayerProperties;

static const OmmPlayerProperties sOmmPlayerProperties[] = {

    /* Mario */ {
    OMM_TEXT_MARIO_UPPER, 0xFF0000FF, &gMarioAnimsData,
    { 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f }, {
    { MODEL_MARIO, mario_geo },
    { MODEL_MARIOS_CAP, marios_cap_geo },
    { MODEL_MARIOS_WING_CAP, marios_wing_cap_geo },
    { MODEL_MARIOS_METAL_CAP, marios_metal_cap_geo },
    { MODEL_MARIOS_WINGED_METAL_CAP, marios_winged_metal_cap_geo },
    } },
    
    /* Peach */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF, &gMarioAnimsData,
    { 1.00f, 1.00f, 1.00f, 1.00f, 0.98f, 0.98f, 1.00f }, {
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    } },
    
#if OMM_GAME_IS_R96X
    /* Luigi */ {
    OMM_TEXT_LUIGI_UPPER, 0x00C000FF, &Data_LuigiAnims,
    { 1.10f, 1.15f, 0.90f, 1.05f, 1.04f, 0.96f, 0.90f }, {
    { MODEL_LUIGI, luigi_geo },
    { MODEL_LUIGIS_CAP, luigis_cap_geo },
    { MODEL_LUIGIS_WING_CAP, luigis_wing_cap_geo },
    { MODEL_LUIGIS_METAL_CAP, luigis_metal_cap_geo },
    { MODEL_LUIGIS_WINGED_METAL_CAP, luigis_winged_metal_cap_geo },
    } },

    /* Wario */ {
    OMM_TEXT_WARIO_UPPER, 0xD0D000FF, &Data_WarioAnims,
    { 0.90f, 0.85f, 1.10f, 0.95f, 0.96f, 1.04f, 1.10f }, {
    { MODEL_WARIO, wario_geo },
    { MODEL_WARIOS_CAP, warios_cap_geo },
    { MODEL_WARIOS_WING_CAP, warios_wing_cap_geo },
    { MODEL_WARIOS_METAL_CAP, warios_metal_cap_geo },
    { MODEL_WARIOS_WINGED_METAL_CAP, warios_winged_metal_cap_geo },
    } },
#endif
    
    /* Peach (Joy) */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF, &gMarioAnimsData,
    { 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f }, {
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    } },
    
    /* Peach (Rage) */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF, &gMarioAnimsData,
    { 0.80f, 0.72f, 0.80f, 1.00f, 0.90f, 1.10f, 1.15f }, {
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    } },

    /* Peach (Gloom) */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF, &gMarioAnimsData,
    { 1.50f, 1.85f, 1.50f, 1.00f, 1.20f, 0.98f, 1.00f }, {
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    } },

    /* Peach */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF, &gMarioAnimsData,
    { 1.00f, 1.00f, 1.00f, 1.00f, 0.98f, 0.98f, 1.00f }, {
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    } },
};

#if OMM_GAME_IS_R96X

//
// Render96 characters functions
// Redefined to fit the player system of OMM
//

// Ignore 'characterType', set the model according to the selected character
void setCharacterModel(UNUSED s32 characterType) {
    omm_player_select(sOmmPlayer);
}

// Useless, always return 0
s32 getCharacterType() {
    return 0;
}

// Handled by other functions, always return 1
f32 getCharacterMultiplier() {
    return 1.f;
}

// Check if Luigi is selected
s32 isLuigi() {
    return OMM_PLAYER_IS_LUIGI;
}

// Check if Wario is selected
s32 isWario() {
    return OMM_PLAYER_IS_WARIO;
}

//
// Luigi notification
//

s8 notificationStatus = FALSE;

void triggerLuigiNotification() {
    if (!notificationStatus) {
        audio_play_puzzle_jingle();
        set_notification_state(TRUE);
        notificationStatus = TRUE;
    }    
}

void set_notification_status(s8 newState) {
    notificationStatus = newState;
}

s8 get_notification_status() {
    return notificationStatus;
}

//
// Is Luigi/Wario unlocked?
// In the main menu, checks all files, but in-game, checks only the current savefile
//

static bool is_unlocked(s32 (get_collectibles_count(s32)), s32 required) {
    for (s32 i = (omm_is_main_menu() ? 0 : gCurrSaveFileNum - 1); i != (omm_is_main_menu() ? NUM_SAVE_FILES : gCurrSaveFileNum); ++i) {
        if (get_collectibles_count(i) >= required) {
            return true;
        }
    }
    return false;
}

#endif

//
// Select
//

bool omm_player_is_unlocked(s32 index) {
    switch (index) {
        case OMM_PLAYER_MARIO: return true;
        case OMM_PLAYER_PEACH: return OMM_SPARKLY_IS_PEACH_UNLOCKED;
#if OMM_GAME_IS_R96X
        case OMM_PLAYER_LUIGI: return is_unlocked(save_file_get_keys, NUM_KEYS);
        case OMM_PLAYER_WARIO: return is_unlocked(save_file_get_wario_coins, NUM_WARIO_COINS);
#endif
    }
    return false;
}

// If the game tries to select a locked character, select Mario instead
void omm_player_select(s32 index) {
    sOmmPlayer = (omm_player_is_unlocked(index) ? index : OMM_PLAYER_MARIO);
    gOmmCharacter = sOmmPlayer;
    gSaveFileModified = true;
}

bool omm_player_is_selected(s32 index) {
    return sOmmPlayer == index;
}

s32 omm_player_get_selected_index() {
    return sOmmPlayer;
}

//
// Player properties
//

const char *omm_player_properties_get_name(s32 index) {
    return sOmmPlayerProperties[index].name;
}

u32 omm_player_properties_get_color(s32 index) {
    return sOmmPlayerProperties[index].color;
}

const void *omm_player_properties_get_anims(s32 index) {
    return sOmmPlayerProperties[index].anims;
}

//
// Physics properties
//

f32 omm_player_physics_get_ground(s32 index) {
    return (gMarioState->action == ACT_WALKING ? sOmmPlayerProperties[index].physics.walk : sOmmPlayerProperties[index].physics.ground);
}

f32 omm_player_physics_get_air(s32 index) {
    return sOmmPlayerProperties[index].physics.air;
}

f32 omm_player_physics_get_swim(s32 index) {
    return sOmmPlayerProperties[index].physics.swim;
}

f32 omm_player_physics_get_jump(s32 index) {
    return sOmmPlayerProperties[index].physics.jump;
}

f32 omm_player_physics_get_gravity(s32 index) {
    return sOmmPlayerProperties[index].physics.gravity;
}

f32 omm_player_physics_get_slide(s32 index) {
    return sOmmPlayerProperties[index].physics.slide;
}

//
// Graphics properties
//

s32 omm_player_graphics_get_model(s32 index) {
    return sOmmPlayerProperties[index].graphics.body.id;
}

s32 omm_player_graphics_get_normal_cap(s32 index) {
    return sOmmPlayerProperties[index].graphics.cap.id;
}

s32 omm_player_graphics_get_wing_cap(s32 index) {
    return sOmmPlayerProperties[index].graphics.wcap.id;
}

s32 omm_player_graphics_get_metal_cap(s32 index) {
    return sOmmPlayerProperties[index].graphics.mcap.id;
}

s32 omm_player_graphics_get_winged_metal_cap(s32 index) {
    return sOmmPlayerProperties[index].graphics.wmcap.id;
}

s32 omm_player_graphics_get_cap(s32 index, bool wing, bool metal) {
    if (wing && metal) return omm_player_graphics_get_winged_metal_cap(index);
    if (        metal) return omm_player_graphics_get_metal_cap(index);
    if (wing         ) return omm_player_graphics_get_wing_cap(index);
    else               return omm_player_graphics_get_normal_cap(index);
}

//
// Update
//

OMM_ROUTINE_LEVEL_ENTRY(omm_player_init) {
    omm_player_select(gOmmCharacter);
}

OMM_ROUTINE_UPDATE(omm_player_update) {
    omm_opt_select_available(
        gOmmCharacter,
        OMM_PLAYER_MARIO,
        OMM_NUM_PLAYABLE_CHARACTERS,
        omm_player_is_unlocked(gOmmCharacter)
    );
}

OMM_ROUTINE_PRE_RENDER(omm_player_update_gfx) {
    if (gLoadedGraphNodes && gMarioObject) {
        const OmmPlayerProperties *pp            = &sOmmPlayerProperties[sOmmPlayer];
        gLoadedGraphNodes[pp->graphics.body.id]  = geo_layout_to_graph_node(NULL, pp->graphics.body.geo);
        gLoadedGraphNodes[pp->graphics.cap.id]   = geo_layout_to_graph_node(NULL, pp->graphics.cap.geo);
        gLoadedGraphNodes[pp->graphics.wcap.id]  = geo_layout_to_graph_node(NULL, pp->graphics.wcap.geo);
        gLoadedGraphNodes[pp->graphics.mcap.id]  = geo_layout_to_graph_node(NULL, pp->graphics.mcap.geo);
        gLoadedGraphNodes[pp->graphics.wmcap.id] = geo_layout_to_graph_node(NULL, pp->graphics.wmcap.geo);
        gMarioObject->oGraphNode                 = geo_layout_to_graph_node(NULL, pp->graphics.body.geo);
        gMarioAnimations                         = (MarioAnimationsStruct *) pp->anims;
#if OMM_GAME_IS_R96X
        Cheats.ChaosPlayAs = 0;
        Cheats.PlayAs = 0;
#endif
    }
}
