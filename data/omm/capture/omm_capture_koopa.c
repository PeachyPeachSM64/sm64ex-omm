#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_KOOPA_STATE_SHELLED_IDLE          (0)
#define OMM_CAPPY_KOOPA_STATE_SHELLED_WALKING       (1)
#define OMM_CAPPY_KOOPA_STATE_SHELLED_SLIDING       (2)
#define OMM_CAPPY_KOOPA_STATE_SHELLED_JUMPING       (3)
#define OMM_CAPPY_KOOPA_STATE_UNSHELLED_IDLE        (4)
#define OMM_CAPPY_KOOPA_STATE_UNSHELLED_WALKING     (5)
#define OMM_CAPPY_KOOPA_STATE_UNSHELLED_RUNNING     (6)
#define OMM_CAPPY_KOOPA_STATE_UNSHELLED_JUMPING     (7)

typedef struct { s32 animID; f32 animAccel; s32 sfx; const GeoLayout *georef; } OmmCappyKoopaAnimData;
static const OmmCappyKoopaAnimData OMM_CAPPY_KOOPA_ANIM_DATA[] = {
    [OMM_CAPPY_KOOPA_STATE_SHELLED_IDLE]      = {  7,   0, 0, koopa_with_shell_geo    },
    [OMM_CAPPY_KOOPA_STATE_SHELLED_WALKING]   = {  9,   1, 1, koopa_with_shell_geo    },
    [OMM_CAPPY_KOOPA_STATE_SHELLED_SLIDING]   = {  0,   0, 2, koopa_shell_geo         },
    [OMM_CAPPY_KOOPA_STATE_SHELLED_JUMPING]   = { 12, 100, 0, koopa_with_shell_geo    },
    [OMM_CAPPY_KOOPA_STATE_UNSHELLED_IDLE]    = {  7,   0, 0, koopa_without_shell_geo },
    [OMM_CAPPY_KOOPA_STATE_UNSHELLED_WALKING] = {  9,   1, 1, koopa_without_shell_geo },
    [OMM_CAPPY_KOOPA_STATE_UNSHELLED_RUNNING] = {  3,   0, 1, koopa_without_shell_geo },
    [OMM_CAPPY_KOOPA_STATE_UNSHELLED_JUMPING] = { 12, 100, 0, koopa_without_shell_geo }
};

//
// Init
//

bool omm_cappy_koopa_init(struct Object *o) {

    // Can't possess KTQ
    if (omm_obj_is_koopa_the_quick(o)) {
        return false;
    }

    gOmmObject->koopa.scale = o->oScaleY;
    return true;
}

void omm_cappy_koopa_end(struct Object *o) {
    if (gOmmObject->state.actionState < OMM_CAPPY_KOOPA_STATE_UNSHELLED_IDLE) {
        o->oKoopaMovementType = KOOPA_BP_NORMAL;
        o->oGraphNode = geo_layout_to_graph_node(NULL, koopa_with_shell_geo);
    } else {
        o->oKoopaMovementType = KOOPA_BP_UNSHELLED;
        o->oGraphNode = geo_layout_to_graph_node(NULL, koopa_without_shell_geo);
    }
    o->oBehParams2ndByte = o->oKoopaMovementType;
}

u64 omm_cappy_koopa_get_type(struct Object *o) {
    return !omm_obj_is_koopa_the_quick(o) ? OMM_CAPTURE_KOOPA : 0;
}

f32 omm_cappy_koopa_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_koopa_update(struct Object *o) {
    obj_scale(o, gOmmObject->koopa.scale);

    // Inputs
    if (pobj_process_inputs(o)) {
        bool shelled = (o->oKoopaMovementType != KOOPA_BP_UNSHELLED);
        pobj_move(o, !shelled && POBJ_B_BUTTON_DOWN, shelled && POBJ_B_BUTTON_DOWN, false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
        }
    }

    // States
    if (o->oKoopaMovementType != KOOPA_BP_UNSHELLED) {
        if (POBJ_B_BUTTON_DOWN) {
            gOmmObject->state.actionState = OMM_CAPPY_KOOPA_STATE_SHELLED_SLIDING;
        } else if (obj_is_on_ground(o)) {
            if (POBJ_IS_WALKING) {
                gOmmObject->state.actionState = OMM_CAPPY_KOOPA_STATE_SHELLED_WALKING;
            } else {
                gOmmObject->state.actionState = OMM_CAPPY_KOOPA_STATE_SHELLED_IDLE;
            }
        } else {
            gOmmObject->state.actionState = OMM_CAPPY_KOOPA_STATE_SHELLED_JUMPING;
        }
    } else {
        if (obj_is_on_ground(o)) {
            if (POBJ_IS_RUNNING) {
                gOmmObject->state.actionState = OMM_CAPPY_KOOPA_STATE_UNSHELLED_RUNNING;
            } else if (POBJ_IS_WALKING) {
                gOmmObject->state.actionState = OMM_CAPPY_KOOPA_STATE_UNSHELLED_WALKING;
            } else {
                gOmmObject->state.actionState = OMM_CAPPY_KOOPA_STATE_UNSHELLED_IDLE;
            }
        } else {
            gOmmObject->state.actionState = OMM_CAPPY_KOOPA_STATE_UNSHELLED_JUMPING;
        }
    }

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    bool shellSlide = (gOmmObject->state.actionState == OMM_CAPPY_KOOPA_STATE_SHELLED_SLIDING);
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_IMMUNE_TO_LAVA * shellSlide;
    POBJ_SET_IMMUNE_TO_QUICKSAND * shellSlide;
    POBJ_SET_ABLE_TO_MOVE_ON_WATER * shellSlide;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES * shellSlide;
    POBJ_SET_ABLE_TO_OPEN_DOORS;
    POBJ_SET_ATTACKING_BREAKABLE * shellSlide;

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions(

    // Koopa shell
    if (obj->behavior == bhvKoopaShell && o->oKoopaMovementType == KOOPA_BP_UNSHELLED) {
        if (obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {
            o->oKoopaMovementType = KOOPA_BP_NORMAL;
            obj_spawn_white_puff(obj, POBJ_SOUND_STAR);
            obj_mark_for_deletion(obj);
            obj->oInteractStatus = INT_STATUS_INTERACTED;
        }
    }

    );
    pobj_stop_if_unpossessed();

    // Animation
    const OmmCappyKoopaAnimData *animData = &OMM_CAPPY_KOOPA_ANIM_DATA[gOmmObject->state.actionState];
    o->oGraphNode = geo_layout_to_graph_node(NULL, animData->georef);
    obj_anim_play(o, animData->animID, max_f(1.f, animData->animAccel * max_f(1.f, POBJ_ABS_FORWARD_VEL * 2.f / pobj_get_walk_speed(o))));

    // Sound and particles
    switch (animData->sfx) {
        case 1: {
            if (obj_is_on_ground(o)) {
                obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 8.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_KOOPA, OBJ_PARTICLE_NONE);
            }
        } break;

        case 2: {
            if (o->oFloorType == OBJ_FLOOR_TYPE_GROUND) {
                obj_make_step_sound_and_particle(o, NULL, 0, 0, POBJ_SOUND_RIDING_SHELL_GROUND, OBJ_PARTICLE_MIST);
            }
        } break;
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_koopa_update_gfx(struct Object *o) {

    // Gfx
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_update_gfx(o);
    obj_update_blink_state(o, &o->oKoopaBlinkTimer, 20, 50, 4);
    if (gOmmObject->state.actionState == OMM_CAPPY_KOOPA_STATE_SHELLED_SLIDING) {
        obj_scale(o, gOmmObject->koopa.scale / 1.5f);
        o->oGfxAngle[1] = gGlobalTimer * 0x2000;
        o->oFlags |= OBJ_FLAG_SHADOW_COPY_OBJ_POS;
    } else {
        obj_scale(o, gOmmObject->koopa.scale);
        o->oFlags &= ~OBJ_FLAG_SHADOW_COPY_OBJ_POS;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}
