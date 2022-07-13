#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Behavior
//

static void omm_bhv_goomba_stack_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->oGoombaStackParent;

    // If the parent is dead, reset Goomba's state
    if (!p || !p->activeFlags || !omm_obj_is_goomba(p)) {
        gCurBhvCommand = o->behavior - 2;
        o->curBhvCommand = o->behavior;
        o->bhvStackIndex = 0;
        o->parentObj = o;
        o->oAction = 0;
        return;
    }

    // Mario can interact with it, but if damaged, the bottom Goomba takes the hit
    p->oInteractStatus |= o->oInteractStatus;
    obj_set_params(o, INTERACT_BOUNCE_TOP, 1, 0, 1, true);
    obj_reset_hitbox(o, 72, 50, 42, 40, 40, 0);

    // Always follow the bottom Goomba
    o->oPosX = p->oPosX;
    o->oPosY = p->oPosY + o->oAction * o->oScaleY * 65.f;
    o->oPosZ = p->oPosZ;
    o->oFaceAnglePitch = 0;
    o->oFaceAngleYaw = p->oFaceAngleYaw;
    o->oFaceAngleRoll = 0;
    o->oMoveAnglePitch = 0;
    o->oMoveAngleYaw = p->oFaceAngleYaw;
    o->oMoveAngleRoll = 0;
    obj_update_gfx(o);
    obj_anim_play(o, 0, 1.f);
    obj_update_blink_state(o, &o->oGoombaBlinkTimer, 30, 50, 5);
}

const BehaviorScript omm_bhv_goomba_stack[] = {
    OBJ_TYPE_GENACTOR,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_goomba_stack_update,
    0x09000000,
};

const BehaviorScript omm_bhv_goomba_stack_capture[] = {
    OBJ_TYPE_GENACTOR,
    0x08000000,
    0x09000000,
};

//
// Goomba functions
//

bool omm_obj_is_goomba_stack(struct Object *o) {
    return (o->curBhvCommand >= omm_bhv_goomba_stack &&
            o->curBhvCommand <  omm_bhv_goomba_stack + OMM_ARRAY_SIZE(omm_bhv_goomba_stack)) || (
            o->curBhvCommand >= omm_bhv_goomba_stack_capture &&
            o->curBhvCommand <  omm_bhv_goomba_stack_capture + OMM_ARRAY_SIZE(omm_bhv_goomba_stack_capture));
}

//
// Update
//

OMM_ROUTINE_PRE_RENDER(omm_goomba_stack_update) {
    struct Object *goombaStacks[64][OBJ_GOOMBA_STACK_MAX] = { { NULL } };
    s32 goombaStacksCount = 0;

    // Add Goombas to a stack based on its parent
    omm_array_for_each(omm_obj_get_goomba_behaviors(), p) {
        const BehaviorScript *bhv = (const BehaviorScript *) p->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            if (omm_obj_is_goomba_stack(obj)) {
                for (s32 i = 0; i <= goombaStacksCount; ++i) {
                    if (i == goombaStacksCount) {
                        goombaStacks[i][0] = obj->oGoombaStackParent;
                        goombaStacks[i][obj->oAction] = obj;
                        goombaStacksCount++;
                        break;
                    } else if (obj->oGoombaStackParent == goombaStacks[i][0]) {
                        goombaStacks[i][obj->oAction] = obj;
                        break;
                    }
                }
            }
        }
    }

    // Renumber the Goombas to fill the gaps
    for (s32 i = 0; i != goombaStacksCount; ++i) {
        for (s32 j = 1, k = 1; j != OBJ_GOOMBA_STACK_MAX; ++j) {
            if (goombaStacks[i][j]) {
                goombaStacks[i][j]->oAction = k++;
            }
        }
    }
}
