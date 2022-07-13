#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static s32 sOmmPlayer = OMM_PLAYER_MARIO;

//
// Properties
//

typedef struct { f32 ground; f32 air; f32 jump; } Phy;
typedef struct { s32 id; const GeoLayout *geo; } IdGeo;
typedef struct {
    const char *name;
    u32 color;
    Phy phy;
    IdGeo body;
    IdGeo cap;
    IdGeo wcap;
    IdGeo mcap;
    IdGeo wmcap;
    void *anims;
} OmmPlayerProperties;

static const OmmPlayerProperties sOmmPlayerProperties[] = {

    /* Mario */ {
    OMM_TEXT_MARIO_UPPER, 0xFF0000FF,
    { 1.00f, 1.00f, 1.00f },
    { MODEL_MARIO, mario_geo },
    { MODEL_MARIOS_CAP, marios_cap_geo },
    { MODEL_MARIOS_WING_CAP, marios_wing_cap_geo },
    { MODEL_MARIOS_METAL_CAP, marios_metal_cap_geo },
    { MODEL_MARIOS_WINGED_METAL_CAP, marios_winged_metal_cap_geo },
    &gMarioAnimsData },
    
    /* Peach */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF,
    { 1.00f, 1.00f, 1.00f },
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    &gMarioAnimsData },
    
#if OMM_GAME_IS_R96A
    /* Luigi */ {
    OMM_TEXT_LUIGI_UPPER, 0x00C000FF,
    { 1.10f, 0.90f, 1.05f },
    { MODEL_LUIGI, luigi_geo },
    { MODEL_LUIGIS_CAP, luigis_cap_geo },
    { MODEL_LUIGIS_WING_CAP, luigis_wing_cap_geo },
    { MODEL_LUIGIS_METAL_CAP, luigis_metal_cap_geo },
    { MODEL_LUIGIS_WINGED_METAL_CAP, luigis_winged_metal_cap_geo },
    &Data_LuigiAnims },
    
    /* Wario */ {
    OMM_TEXT_WARIO_UPPER, 0xD0D000FF,
    { 0.90f, 1.10f, 0.95f },
    { MODEL_WARIO, wario_geo },
    { MODEL_WARIOS_CAP, warios_cap_geo },
    { MODEL_WARIOS_WING_CAP, warios_wing_cap_geo },
    { MODEL_WARIOS_METAL_CAP, warios_metal_cap_geo },
    { MODEL_WARIOS_WINGED_METAL_CAP, warios_winged_metal_cap_geo },
    &Data_WarioAnims },
#endif
    
    /* Peach (Joy) */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF,
    { 1.00f, 1.00f, 1.00f },
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    &gMarioAnimsData },
    
    /* Peach (Rage) */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF,
    { 0.80f, 0.80f, 0.90f },
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    &gMarioAnimsData },
    
    /* Peach (Gloom) */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF,
    { 1.50f, 1.50f, 1.20f },
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    &gMarioAnimsData },
    
    /* Peach (Calm) */ {
    OMM_TEXT_PEACH_UPPER, 0xFF80FFFF,
    { 1.00f, 1.00f, 1.00f },
    { MODEL_PEACH_OMM, omm_geo_peach },
    { MODEL_PEACHS_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_WING_CAP_OMM, omm_geo_peachs_cap },
    { MODEL_PEACHS_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    { MODEL_PEACHS_WINGED_METAL_CAP_OMM, omm_geo_peachs_metal_cap },
    &gMarioAnimsData },
};

#if OMM_GAME_IS_R96A

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
        r96_play_collect_jingle(R96_EVENT_SOLVE_PUZZLE);
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
        case OMM_PLAYER_PEACH: return omm_ssd_is_completed(OMM_SSM_NORMAL);
#if OMM_GAME_IS_R96A
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
// Gfx properties
//

const char *omm_player_get_name(s32 index) {
    return sOmmPlayerProperties[index].name;
}

u32 omm_player_get_color(s32 index) {
    return sOmmPlayerProperties[index].color;
}

s32 omm_player_get_model(s32 index) {
    return sOmmPlayerProperties[index].body.id;
}

s32 omm_player_get_normal_cap(s32 index) {
    return sOmmPlayerProperties[index].cap.id;
}

s32 omm_player_get_wing_cap(s32 index) {
    return sOmmPlayerProperties[index].wcap.id;
}

s32 omm_player_get_metal_cap(s32 index) {
    return sOmmPlayerProperties[index].mcap.id;
}

s32 omm_player_get_winged_metal_cap(s32 index) {
    return sOmmPlayerProperties[index].wmcap.id;
}

s32 omm_player_get_cap(s32 index, bool wing, bool metal) {
    if (wing && metal) return omm_player_get_winged_metal_cap(index);
    if (        metal) return omm_player_get_metal_cap(index);
    if (wing         ) return omm_player_get_wing_cap(index);
    else               return omm_player_get_normal_cap(index);
}

//
// Physics properties
//

f32 omm_player_get_ground_speed_multiplier(s32 index) {
    f32 mult = sOmmPlayerProperties[index].phy.ground;
    return mult * (gMarioState->action == ACT_WALKING ? sqrtf(mult) : 1.f);
}

f32 omm_player_get_air_speed_multiplier(s32 index) {
    return sOmmPlayerProperties[index].phy.air;
}

f32 omm_player_get_jump_multiplier(s32 index) {
    return sOmmPlayerProperties[index].phy.jump;
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
        const OmmPlayerProperties *pp   = &sOmmPlayerProperties[sOmmPlayer];
        gLoadedGraphNodes[pp->body.id]  = geo_layout_to_graph_node(NULL, pp->body.geo);
        gLoadedGraphNodes[pp->cap.id]   = geo_layout_to_graph_node(NULL, pp->cap.geo);
        gLoadedGraphNodes[pp->wcap.id]  = geo_layout_to_graph_node(NULL, pp->wcap.geo);
        gLoadedGraphNodes[pp->mcap.id]  = geo_layout_to_graph_node(NULL, pp->mcap.geo);
        gLoadedGraphNodes[pp->wmcap.id] = geo_layout_to_graph_node(NULL, pp->wmcap.geo);
        gMarioObject->oGraphNode        = geo_layout_to_graph_node(NULL, pp->body.geo);
        gMarioAnimations                = (MarioAnimationsStruct *) pp->anims;
#if OMM_GAME_IS_R96A
        Cheats.ChaosPlayAs = 0;
        Cheats.PlayAs = 0;
#endif
    }
}
