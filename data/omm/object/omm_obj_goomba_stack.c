#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

static struct ObjectHitbox sGoombaStackHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 1,
    /* radius:            */ 72,
    /* height:            */ 60,
    /* hurtboxRadius:     */ 42,
    /* hurtboxHeight:     */ 40,
};

//
// Behavior
//

static void bhv_omm_goomba_stack_reset(struct Object *o) {
    o->curBhvCommand = o->behavior - 2;
    o->bhvStackIndex = 0;
    o->oAction = 0;
    o->oIntangibleTimer = 3;
    o->oSafeStep = 0;
}

static void bhv_omm_goomba_stack_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->oGoombaStackParent;

    // If the parent is dead, reset Goomba's state
    if (!p || !p->activeFlags || !omm_obj_is_goomba(p)) {
        bhv_omm_goomba_stack_reset(o);
        return;
    }

    // If one of the Goombas below is no longer stacked, unstack the current Goomba
    u32 missingGoombasBelow = (1 << o->oAction) - 2;
    omm_array_for_each(omm_obj_get_goomba_behaviors(), p_bhv) {
        const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            if (obj != o && omm_obj_is_goomba_stack(obj) && obj->oGoombaStackParent == o->oGoombaStackParent && obj->oAction < o->oAction) {
                missingGoombasBelow &= ~(1 << obj->oAction);
            }
        }
    }
    if (missingGoombasBelow != 0) {
        bhv_omm_goomba_stack_reset(o);
        return;
    }

    // If the Goomba is attacked, unstack it
    if ((o->oInteractStatus & (INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED)) != 0 &&
        (o->oInteractStatus & INT_STATUS_ATTACK_MASK) != 0) {
        bhv_omm_goomba_stack_reset(o);
        return;
    }

    // Enable interactions only if parent is not held
    // Hide stacked Goombas if parent is in Yoshi's mouth
    obj_set_hitbox(o, &sGoombaStackHitbox);
    o->oIntangibleTimer = 0;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    if (p->oHeldState != HELD_FREE) {
        o->oIntangibleTimer = -1;
        if (omm_cappy_yoshi_get_object_in_mouth(gOmmCapture) == p) {
            o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        }
    }

    // Always follow the bottom Goomba
    obj_set_xyz(o, p->oPosX, p->oPosY + o->oAction * o->oScaleY * omm_behavior_data_get_capture(p->behavior)->hitboxHeight, p->oPosZ);
    obj_set_home(o, p->oPosX, p->oPosY, p->oPosZ);
    obj_set_angle(o, 0, p->oFaceAngleYaw, 0);
    obj_copy_scale(o, p);
    obj_update_gfx(o);
    obj_anim_play(o, 0, 1.f);
    obj_update_blink_state(o, &o->oGoombaBlinkTimer, 30, 50, 5);
    obj_copy_visibility_and_transparency(o, p);
}

const BehaviorScript bhvOmmGoombaStack[] = {
    OBJ_TYPE_GENACTOR,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_goomba_stack_update),
    BHV_END_LOOP(),
};

const BehaviorScript bhvOmmGoombaStackCapture[] = {
    OBJ_TYPE_GENACTOR,
    BHV_BEGIN_LOOP(),
    BHV_END_LOOP(),
};

bool omm_obj_is_goomba_stack(struct Object *o) {
    return (o->curBhvCommand >= bhvOmmGoombaStack &&
            o->curBhvCommand <  bhvOmmGoombaStack + array_length(bhvOmmGoombaStack)) || (
            o->curBhvCommand >= bhvOmmGoombaStackCapture &&
            o->curBhvCommand <  bhvOmmGoombaStackCapture + array_length(bhvOmmGoombaStackCapture));
}
