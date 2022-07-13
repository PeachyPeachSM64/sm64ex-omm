#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#if OMM_GAME_IS_R96A
#define cappy_mad_piano_play_song(...)
#else

static s32 sSeqCurrentId = -1;
OMM_ROUTINE_LEVEL_ENTRY(cappy_mad_piano_reset_seq_current_id) {
    sSeqCurrentId = -1;
}

static void cappy_mad_piano_play_song(bool aPress, bool bPress) {
    if (aPress && bPress) {
        music_fade_out(SEQ_PLAYER_LEVEL, 60);
        play_sequence(SEQ_PLAYER_LEVEL, SEQ_LEVEL_SPOOKY, 60);
    } else if (aPress) {
        sSeqCurrentId = (sSeqCurrentId + 1) % (SEQ_COUNT - 1);
        music_fade_out(SEQ_PLAYER_LEVEL, 60);
        play_sequence(SEQ_PLAYER_LEVEL, sSeqCurrentId + 1, 60);
    } else if (bPress) {
        sSeqCurrentId = random_u16() % (SEQ_COUNT - 1);
        music_fade_out(SEQ_PLAYER_LEVEL, 60);
        play_sequence(SEQ_PLAYER_LEVEL, sSeqCurrentId + 1, 60);
    }
}

#endif

//
// Init
//

bool cappy_mad_piano_init(struct Object *o) {
    o->oFaceAngleYaw += 0x4000;
    gOmmData->object->state.actionTimer = 0;
    gOmmData->object->state.actionState = 0;
    return true;
}

void cappy_mad_piano_end(struct Object *o) {
    obj_anim_play(o, 0, 1.f);
    obj_drop_to_floor(o);
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
}

f32 cappy_mad_piano_get_top(struct Object *o) {
    return 150.f * o->oScaleY;
}

//
// Update
//

s32 cappy_mad_piano_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_SAND;
    POBJ_SET_IMMUNE_TO_WIND;
    POBJ_SET_ATTACKING;

    // Inputs
    if (!omm_mario_is_locked(gMarioState)) {
        pobj_move(o, false, false, false);
        if (pobj_jump(o, 1.f, 1) != POBJ_RESULT_NONE) {
            obj_play_sound(o, SOUND_OBJ_MAD_PIANO_CHOMPING);
        }
        cappy_mad_piano_play_song(POBJ_A_BUTTON_PRESSED, POBJ_B_BUTTON_PRESSED);
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o, 0.80f, 0.95f);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    POBJ_STOP_IF_UNPOSSESSED;

    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    if (!obj_is_on_ground(o)) {
        gOmmData->object->state.actionState = 1;
        gOmmData->object->state.actionTimer = 1;
    } else if (gOmmData->object->state.actionTimer == 0) {
        gOmmData->object->state.actionState = 0;
    } else {
        gOmmData->object->state.actionTimer--;
    }
    obj_update_gfx(o);
    obj_anim_play(o, gOmmData->object->state.actionState, 1.f);
    o->oGfxAngle[1] -= 0x4000;

    // Cappy values
    gOmmData->object->cappy.offset[1] = 150.f;
    gOmmData->object->cappy.scale     = 1.5f;

    // OK
    POBJ_RETURN_OK;
}
