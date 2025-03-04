#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#if OMM_GAME_IS_R96X
#define omm_cappy_mad_piano_play_song(...)
#else

static s32 sSeqCurrentId = -1;
OMM_ROUTINE_LEVEL_ENTRY(cappy_mad_piano_reset_seq_current_id) {
    sSeqCurrentId = -1;
}

static void omm_cappy_mad_piano_play_song(bool aPress, bool bPress) {
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

static void omm_cappy_mad_piano_destroy_door(struct Object *door) {
    if (door) {
        obj_spawn_white_puff(door, POBJ_SOUND_BREAK_BOX);
        obj_spawn_triangle_break_particles(door, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_30);
        obj_mark_for_deletion(door);
    }
}

//
// Init
//

bool omm_cappy_mad_piano_init(struct Object *o) {
    o->oFaceAngleYaw += 0x4000;
    gOmmObject->state.actionTimer = 0;
    return true;
}

void omm_cappy_mad_piano_end(struct Object *o) {
    obj_anim_play(o, 0, 1.f);
    obj_drop_to_floor(o);
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
}

u64 omm_cappy_mad_piano_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_MAD_PIANO;
}

f32 omm_cappy_mad_piano_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_mad_piano_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_INVULNERABLE;
    POBJ_SET_IMMUNE_TO_FIRE;
    POBJ_SET_IMMUNE_TO_LAVA;
    POBJ_SET_IMMUNE_TO_QUICKSAND;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS;
    POBJ_SET_ATTACKING_BREAKABLE;
    POBJ_SET_GROUND_POUNDING;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, false, false);
        omm_cappy_mad_piano_play_song(POBJ_A_BUTTON_PRESSED, POBJ_B_BUTTON_PRESSED);
    }

    // Movement
    if (pobj_hop(o, 1.f)) {
        obj_play_sound(o, SOUND_OBJ_MAD_PIANO_CHOMPING);
    }
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();
    omm_obj_process_interactions(o, POBJ_INT_PRESET_MAD_PIANO);

    // Break doors
    if (o->oWall && o->oWall->object) {
        struct Object *door = o->oWall->object;
        if (door->oInteractType & INTERACT_DOOR) {
            if (door->oInteractionSubtype & INT_SUBTYPE_STAR_DOOR) {
                omm_cappy_mad_piano_destroy_door(obj_get_nearest_with_behavior(door, door->behavior));
            }
            omm_cappy_mad_piano_destroy_door(door);
        }
    }

    // Animation, sound and particles
    gOmmObject->state.actionTimer = obj_is_on_ground(o) ? max_s(0, gOmmObject->state.actionTimer - 1) : 2;
    obj_anim_play(o, gOmmObject->state.actionTimer != 0, 1.f);

    // OK
    pobj_return_ok;
}

void omm_cappy_mad_piano_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    o->oGfxAngle[1] -= 0x4000;

    // Cappy transform
    gOmmObject->cappy.object = o;
}
