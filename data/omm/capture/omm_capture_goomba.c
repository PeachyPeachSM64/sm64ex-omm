#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_cappy_goomba_reset(struct Object *o, struct Object *obj, const BehaviorScript *bhv, s32 index, s32 timer) {
    obj->curBhvCommand = bhv;
    obj->bhvStackIndex = 0;
    obj->oAction = index;
    obj->oInteractStatus = 0;
    obj->oIntangibleTimer = timer;
    obj->oGoombaStackParent = o;
    obj->oSafeStep = 0;
}

static bool omm_cappy_goomba_can_stack(struct Object *o, struct Object *obj) {
    return omm_obj_is_goomba(obj) && (o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK) == (obj->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK);
}

static void omm_cappy_goomba_copy_graph_node_and_animations(struct Object *dest, struct Object *src) {
    const GeoLayout *georef = (src->oGraphNode ? src->oGraphNode->georef : NULL);
    struct Animation **anims = src->oAnimations;
    dest->oGraphNode = geo_layout_to_graph_node(NULL, georef);
    dest->oAnimations = anims;
}

static void omm_cappy_goomba_add_to_stack(struct Object *o, struct Object *obj) {
    obj_spawn_white_puff(o, POBJ_SOUND_DEATH);

    // Add new goomba object to the top of the stack
    gOmmObject->goomba.stackObj[gOmmObject->goomba.stackCount++] = obj;
    omm_cappy_goomba_reset(o, obj, bhvOmmGoombaStackCapture, gOmmObject->goomba.stackCount, -1);
    obj->oFlags &= ~OBJ_FLAG_GFX_INITED;

    // Rotate the graph nodes and animations to give the illusion the stack is moving up
    struct Object **stack = gOmmObject->goomba.stackObj;
    struct Object tmp[1];
    omm_cappy_goomba_copy_graph_node_and_animations(tmp, o);
    omm_cappy_goomba_copy_graph_node_and_animations(o, obj);
    for (s32 i = gOmmObject->goomba.stackCount - 1; i > 0; --i) {
        omm_cappy_goomba_copy_graph_node_and_animations(stack[i], stack[i - 1]);
    }
    omm_cappy_goomba_copy_graph_node_and_animations(stack[0], tmp);
}

static void omm_cappy_goomba_update_stack_gfx(struct Object *o) {
    for (s32 i = 0; i != gOmmObject->goomba.stackCount; ++i) {
        struct Object *obj = gOmmObject->goomba.stackObj[i];
        obj_set_xyz(obj, o->oPosX, o->oPosY + omm_capture_get_hitbox_height(o) * (i + 1), o->oPosZ);
        obj_set_home(obj, o->oPosX, o->oPosY, o->oPosZ);
        obj_set_angle(obj, o->oFaceAnglePitch, o->oGfxAngle[1], o->oFaceAngleRoll);
        obj_copy_scale(obj, o);
        obj_update_gfx(obj);
        obj_anim_play(obj, 0, 1.f);
        obj_update_blink_state(obj, &obj->oGoombaBlinkTimer, 30, 50, 5);
        obj_copy_visibility_and_transparency(obj, o);
    }
}

//
// Init
//

bool omm_cappy_goomba_init(struct Object *o) {

    // If it's a stacked Goomba, capture the lowest one
    if (omm_obj_is_goomba_stack(o)) {
        omm_mario_possess_object(gMarioState, o->oGoombaStackParent, 0);
        o->oInteractStatus = INT_STATUS_INTERACTED;
        o->oIntangibleTimer = -1;
        return false;
    }

    // Retrieve the nearest Goombas (up to OBJ_GOOMBA_STACK_MAX)
    // First Goomba is the bottom one (the one Mario is trying to capture)
    // Already stacked Goombas are prioritized over standalone Goombas
    struct Object *goombas[OBJ_GOOMBA_STACK_MAX * 2] = { o };
    s32 standaloneGoombaIndex = OBJ_GOOMBA_STACK_MAX;
    omm_array_for_each(omm_obj_get_goomba_behaviors(), p_bhv) {
        const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
        for_each_object_with_behavior(obj, bhv) {
            if (obj != o && omm_cappy_goomba_can_stack(o, obj)) {

                // Stacked goomba
                if (omm_obj_is_goomba_stack(obj)) {
                    if (obj->oGoombaStackParent == o && obj->oAction > 0 && obj->oAction < OBJ_GOOMBA_STACK_MAX) {
                        goombas[obj->oAction] = obj;
                    }
                }

                // Standalone goomba
                else if (standaloneGoombaIndex < OBJ_GOOMBA_STACK_MAX * 2) {
                    f32 dist = obj_get_horizontal_distance(o, obj);
                    f32 ydif = obj->oPosY - o->oPosY;
                    if (dist < 20.f && ydif > -20.f && ydif < (70.f * OBJ_GOOMBA_STACK_MAX * o->oScaleY)) {
                        goombas[standaloneGoombaIndex++] = obj;
                    }
                }
            }
        }
    }

    // Store the lowest Y value, this will be the Y value of the base
    f32 lowestY = CELL_HEIGHT_LIMIT;
    for (s32 i = 0; i != standaloneGoombaIndex; ++i) {
        if (goombas[i]) {
            lowestY = min_f(lowestY, goombas[i]->oPosY);
        }
    }

    // Setup the first Goomba
    o->oPosY = lowestY;
    o->oGoombaStackParent = o;
    obj_update_gfx(o);

    // Setup the Goomba stack
    gOmmObject->goomba.stackCount = 0;
    for (s32 i = 1; i != standaloneGoombaIndex; ++i) {
        if (goombas[i]) {
            gOmmObject->goomba.stackObj[gOmmObject->goomba.stackCount++] = goombas[i];
            omm_cappy_goomba_reset(o, goombas[i], bhvOmmGoombaStackCapture, gOmmObject->goomba.stackCount, -1);
            if (gOmmObject->goomba.stackCount == OBJ_GOOMBA_STACK_MAX - 1) {
                break;
            }
        }
    }
    omm_cappy_goomba_update_stack_gfx(o);
    return true;
}

void omm_cappy_goomba_end(struct Object *o) {
    struct MarioState *m = gMarioState;

    // Reset Goomba's state
    omm_cappy_goomba_reset(o, o, o->behavior, 0, 3);

    // Stay stacked when released, but break the stack if Mario gets damaged
    bool shouldStack = (
        m->action == ACT_IDLE ||
        m->action == ACT_FREEFALL ||
        m->action == ACT_WATER_IDLE ||
        m->action == ACT_OMM_LEAVE_OBJECT_JUMP ||
        m->action == ACT_OMM_LEAVE_OBJECT_WATER
    );
    for (s32 i = 0; i != gOmmObject->goomba.stackCount; ++i) {
        struct Object *obj = gOmmObject->goomba.stackObj[i];
        omm_cappy_goomba_reset(o, obj, shouldStack ? bhvOmmGoombaStack : obj->behavior, shouldStack * (i + 1), 3);
    }
}

u64 omm_cappy_goomba_get_type(struct Object *o) {
    switch (o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK) {
        case GOOMBA_SIZE_REGULAR: return OMM_CAPTURE_GOOMBA;
        case GOOMBA_SIZE_HUGE: return OMM_CAPTURE_GOOMBA_HUGE;
        case GOOMBA_SIZE_TINY: return OMM_CAPTURE_GOOMBA_TINY;
    }
    return 0;
}

f32 omm_cappy_goomba_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o) * (1 + gOmmObject->goomba.stackCount);
}

//
// Update
//

s32 omm_cappy_goomba_update(struct Object *o) {
    gOmmObject->state.actionTimer = max_s(0, gOmmObject->state.actionTimer - 1);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o) * (gOmmObject->goomba.stackCount + 1);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
        switch (pobj_jump(o, 1)) {
            case POBJ_RESULT_JUMP_START: {
                obj_play_sound(o, POBJ_SOUND_JUMP_1);
                if (gOmmMario->spin.timer) {
                    gOmmObject->state.actionTimer = 8;
                    o->oVelY = max_f(o->oVelY, 1.35f * pobj_get_jump_velocity(o));
                }
            } break;
            case POBJ_RESULT_JUMP_HOLD: {
                if (gOmmObject->state.actionTimer) {
                    o->oVelY = max_f(o->oVelY, 1.35f * pobj_get_jump_velocity(o));
                }
            } break;
        }
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions(

    // Goomba stack
    if (gOmmObject->goomba.stackCount < (OBJ_GOOMBA_STACK_MAX - 1) && omm_cappy_goomba_can_stack(o, obj) && obj->oIntangibleTimer == 0) {

        // Fall check
        if ((!obj_is_on_ground(o) && o->oVelY <= 0.f) || (obj_is_on_ground(o) && o->oPosY > obj->oPosY + obj->hitboxHeight / 4.f)) {

            // Height check
            f32 y1 = obj->oPosY;
            f32 y2 = obj->oPosY + obj->hitboxHeight * 1.6f;
            if (y1 < o->oPosY && o->oPosY < y2) {

                // Radius check
                f32 r2 = sqr_f(o->hitboxRadius + obj->hitboxRadius);
                f32 d2 = sqr_f(o->oPosX - obj->oPosX) + sqr_f(o->oPosZ - obj->oPosZ);
                if (d2 < r2) {

                    // Add goomba to stack
                    omm_cappy_goomba_add_to_stack(o, obj);
                    continue;
                }
            }
        }
    }

    );
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    obj_anim_play(o, 0, max_f(1.f, (o->oVelY <= 0.f) * POBJ_ABS_FORWARD_VEL * 2.f / pobj_get_walk_speed(o)));
    if (obj_is_on_ground(o)) {
        obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 8.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_GOOMBA, OBJ_PARTICLE_NONE);
        obj_spawn_particle_preset(o, PARTICLE_DUST * (gOmmMario->spin.timer != 0), false);
    } else {
        gOmmMario->spin.timer = 0;
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_goomba_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    obj_update_blink_state(o, &o->oGoombaBlinkTimer, 30, 50, 5);
    if (gOmmObject->state.actionTimer) {
        o->oGfxAngle[1] += 0x2000 * gOmmObject->state.actionTimer;
        obj_spawn_particle_preset(o, !POBJ_IS_STAR_DANCING * PARTICLE_SPARKLES, false);
    }

    // Cappy transform
    gOmmObject->cappy.object = gOmmObject->goomba.stackCount ? gOmmObject->goomba.stackObj[gOmmObject->goomba.stackCount - 1] : o;
    gOmmObject->cappy.post_update = omm_cappy_goomba_update_stack_gfx;
}
