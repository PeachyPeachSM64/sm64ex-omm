#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/omm_constants.h"
#include "behavior_commands.h"

//
// Behavior
//

// Since this function is called in the middle of the bhvOmmHold behavior:
// [o->bhvStackIndex - 1] holds the address of the LOOP command
// [o->bhvStackIndex - 2] holds the stored behavior values
// [o->bhvStackIndex - 3] holds the stored behavior command
static void bhv_omm_hold_restore_behavior(struct Object *o) {
    uintptr_t bhvCommand = o->bhvStack[o->bhvStackIndex - 3];
    uintptr_t bhvFlags = o->bhvStack[o->bhvStackIndex - 2];
    o->curBhvCommand = ((const BehaviorScript *) bhvCommand) - 2;
    o->bhvStackIndex -= 3;
    o->oFlags = (u32) bhvFlags;
    o->oIntangibleTimer = 0;
    o->oHeldState = HELD_FREE;
}

static u32 bhv_omm_hold_update_throw(struct Object *o, const OmmBhvDataHoldable *h, f32 *prevVelY) {
    if (h->thrownAnim != HOLD_ANIM_NONE) {
        obj_anim_play(o, h->thrownAnim, max_f(h->thrownAnimAccel, 1.f / 65536.f));
        if (h->thrownAnimAccel < 0.f) {
            obj_anim_set_frame(o, HOLD_ANIM_SET_FRAME(h->thrownAnimAccel));
        }
    }
    if (o->oWallHitboxRadius == 0.f) {
        o->oWallHitboxRadius = max_s(o->hitboxRadius, o->hurtboxRadius) * 0.75f;
    }

    // Perform step
    *prevVelY = o->oVelY;
    bool wasUnderwater = obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ));
    perform_object_step(o, 0);
    obj_update_gfx(o);
    o->oMoveFlags = 0;

    // Apply gravity
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
    bool isOnGround = o->oFloor != NULL && o->oPosY <= o->oFloorHeight;
    bool isUnderwater = obj_is_underwater(o, waterLevel);
    if (isOnGround) {
        o->oVelY = 0.f;
    } else if (isUnderwater) {
        o->oVelY = max_f(o->oVelY - 2.f, -40.f);
    } else {
        o->oVelY = max_f(o->oVelY - 4.f, -75.f);
    }

    // If Bowser, check collision with the nearest Bowser mine
    if (o->behavior == bhvBowser) {
        struct Object *mine = obj_get_nearest_with_behavior(o, bhvBowserBomb);
        if (mine && obj_get_distance(o, mine) < 600.f) {
            isOnGround = true;
        }
    }

    // Water effects
    obj_produce_water_effects(o, isOnGround, isUnderwater, wasUnderwater, waterLevel);

    // Interactions
    o->oIntangibleTimer = 0;
    omm_obj_process_interactions(o, h->interactionFlags);
    o->oIntangibleTimer = -1;
    o->oInteractStatus = 0;

    // Return collisions
    return (
        (HOLD_HIT_FLOOR * isOnGround) |
        (HOLD_HIT_WALL * (o->oWall != NULL)) |
        (HOLD_HIT_WATER * isUnderwater)
    );
}

static void bhv_omm_hold_bounce(struct Object *o, f32 velY) {
    obj_set_forward_and_y_vel(o, o->oForwardVel / 3.f, abs_f(velY) / 3.f);
    o->oHeldState = HELD_DROPPED;
}

static void bhv_omm_hold_update() {
    struct Object *o = gCurrentObject;
    const OmmBhvDataHoldable *h = omm_behavior_data_get_holdable(o->behavior);
    o->oIntangibleTimer = -1;
    o->oRoom = -1;
    o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    o->activeFlags &= ~ACTIVE_FLAG_IN_DIFFERENT_ROOM;
    switch (o->oHeldState) {

        // Held (the holder updates its position and angle)
        case HELD_HELD: {
            if (h->heldAnim != HOLD_ANIM_NONE) {
                obj_anim_play(o, h->heldAnim, max_f(h->heldAnimAccel, 1.f / 65536.f));
                if (h->heldAnimAccel < 0.f) {
                    obj_anim_set_frame(o, HOLD_ANIM_SET_FRAME(h->heldAnimAccel));
                }
            }
            obj_update_gfx(o);

            // Wake up the piranha plant
            if (o->behavior == bhvPiranhaPlant) {
                struct Object *bubble = obj_get_first_with_behavior_and_parent(o, bhvPiranhaPlantBubble);
                if (bubble) {
                    bubble->oAction = PIRANHA_PLANT_BUBBLE_ACT_IDLE;
                }
#if OMM_GAME_IS_R96X
                if (dynos_jingle_is_playing(R96_EVENT_PIRANHA_PLANT)) {
                    r96_stop_jingle();
                }
#else
                func_80321080(50);
#endif
            }
        } break;

        // Thrown
        case HELD_THROWN: {
            f32 prevVelY = 0;
            u32 hit = bhv_omm_hold_update_throw(o, h, &prevVelY);

            // Check hit floor/wall/water
            if ((hit & h->hitSurface) & (HOLD_HIT_FLOOR | HOLD_HIT_WALL | HOLD_HIT_WATER)) {

                // Perform next action
                switch (h->nextAction) {

                    // Attack the object
                    case HOLD_ACT_ATTACKED: {
                        bhv_omm_hold_restore_behavior(o);
                        o->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                        obj_set_vel(o, 0, 0, 0);
                    } break;

                    // Attack the Big Boo
                    case HOLD_ACT_ATTACKED_BIG_BOO: {
                        bhv_omm_hold_restore_behavior(o);
                        o->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                        o->oAction = 1;
                        o->oPrevAction = 1;
                        o->oSubAction = 0;
                        o->oTimer = 0;
                        obj_set_vel(o, 0, 0, 0);
                    } break;

                    // Destroy the object
                    case HOLD_ACT_DESTROY: {
                        bhv_omm_hold_restore_behavior(o);
                        obj_destroy(o);
                    } break;

                    // Make the object bounce
                    case HOLD_ACT_BOUNCE: {
                        bhv_omm_hold_bounce(o, prevVelY);
                    } break;

                    // Spawn Toad's star then bounce
                    // If the Toad hits a moat pillar, pound it
                    case HOLD_ACT_BOUNCE_TOAD: {
                        s32 starCount = omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE);
                        switch (o->oToadMessageDialogId) {
                            case TOAD_STAR_1_DIALOG: if (starCount >= TOAD_STAR_1_REQUIREMENT) { bhv_spawn_star_no_level_exit(0); o->oToadMessageDialogId = TOAD_STAR_1_DIALOG_AFTER; } break;
                            case TOAD_STAR_2_DIALOG: if (starCount >= TOAD_STAR_2_REQUIREMENT) { bhv_spawn_star_no_level_exit(1); o->oToadMessageDialogId = TOAD_STAR_2_DIALOG_AFTER; } break;
                            case TOAD_STAR_3_DIALOG: if (starCount >= TOAD_STAR_3_REQUIREMENT) { bhv_spawn_star_no_level_exit(2); o->oToadMessageDialogId = TOAD_STAR_3_DIALOG_AFTER; } break;
                        }
                        o->oToadMessageState = 1;
                        o->oToadMessageRecentlyTalked = 1;
#if OMM_GAME_IS_SM64
                        struct Object *obj = o->oFloor->object;
                        if (obj && obj->behavior == bhvWaterLevelPillar && !obj->oWaterLevelPillarUnkF8 && obj->oAction == 0) {
                            obj->oAction = 1;
                            obj_spawn_white_puff(o, SOUND_GENERAL_WALL_EXPLOSION);
                        }
#endif
                        bhv_omm_hold_bounce(o, prevVelY);
                    } break;

                    // Spawn Mips's star then bounce
                    case HOLD_ACT_BOUNCE_MIPS: {
                        if (o->oMipsStarStatus != MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR) {
                            bhv_spawn_star_no_level_exit(3 + o->oBehParams2ndByte);
                            o->oMipsStarStatus = MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR;
                        }
                        o->oAction = MIPS_ACT_FALL_DOWN;
                        bhv_omm_hold_bounce(o, prevVelY);
                    } break;

                    // Make the object bounce if floor collision
                    case HOLD_ACT_BOUNCE_SPARKLY_STAR_BOX: {
                        if (hit & HOLD_HIT_FLOOR) {
                            bhv_omm_hold_bounce(o, prevVelY);
                        } else {
                            o->oHeldState = HELD_DROPPED;
                        }
                    } break;

                    // Return to the object behavior, but set a specific action
                    default: {
                        bhv_omm_hold_restore_behavior(o);
                        o->oAction = h->nextAction;
                        o->oPrevAction = h->nextAction;
                        o->oSubAction = 0;
                        o->oTimer = 0;
                        obj_set_vel(o, 0, prevVelY - 4.f, 0);
                    } break;
                }
            }
        } break;

        // Dropped (bounce)
        case HELD_DROPPED: {
            f32 prevVelY = 0;
            u32 hit = bhv_omm_hold_update_throw(o, h, &prevVelY);

            // Check action
            switch (h->nextAction) {

                // Bounce also on walls, keep bouncing if above max height
                case HOLD_ACT_BOUNCE_SPARKLY_STAR_BOX: {
                    if (hit & HOLD_HIT_WALL) {
                        f32 dot = obj_bounce_on_wall(o, o->oWall, true, true);
                        f32 dec = relerp_0_1_f(dot, -0.1f, -1.f, 1.f, 0.5f);
                        o->oVelX *= dec;
                        o->oVelZ *= dec;
                        o->oForwardVel *= dec;
                        if (dot < -0.1f) {
                            obj_play_sound(o, SOUND_ACTION_BONK);
                        }
                    }
                    if ((hit & HOLD_HIT_FLOOR) && o->oPosY > o->oSparklyStarBoxMaxHeight) {
                        f32 fvel = abs_f(o->oForwardVel);
                        f32 yvel = abs_f(prevVelY);
                        if (fvel < 32.f || yvel < 16.f) {
                            obj_set_forward_vel(o, o->oFaceAngleYaw, sign_f(o->oForwardVel), max_f(fvel, 32.f));
                            prevVelY = sign_f(prevVelY) * max_f(yvel, 16.f);
                            if (fvel < 32.f && o->oFloor->normal.y < 0.95f) {
                                obj_bounce_on_wall(o, o->oFloor, true, true);
                            }
                        }
                    }
                } break;

                // Bounce until speed is too low
                default: break;
            }

            // Check floor collision
            if (hit & HOLD_HIT_FLOOR) {
                obj_set_forward_and_y_vel(o, o->oForwardVel / 2.f, abs_f(prevVelY) / 2.f);
                if (o->oVelY < 4.f || abs_f(o->oForwardVel) < 1.f) {
                    bhv_omm_hold_restore_behavior(o);
                    obj_set_vel(o, 0, 0, 0);
                }
            }
        } break;

        // Released
        case HELD_FREE: {
            bhv_omm_hold_restore_behavior(o);
        } break;
    }
}

static const BehaviorScript bhvOmmHold[] = {
    OBJ_TYPE_DEFAULT,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_hold_update),
    BHV_END_LOOP(),
};

static const BehaviorScript *bhvOmmHoldBegin = bhvOmmHold;
static const BehaviorScript *bhvOmmHoldEnd = bhvOmmHold + array_length(bhvOmmHold);

//
// Functions
//

bool omm_obj_is_holdable(struct Object *o) {
    if (o->behavior == bhvBowser) {
        return IS_BOWSER_INTERACTABLE[o->oAction];
    }
    const OmmBhvDataHoldable *h = omm_behavior_data_get_holdable(o->behavior);
    switch (h->actions) {
        case HOLD_ACTIONS_ALL: return true;
        case HOLD_ACTIONS_0: return o->oAction == 0;
        case HOLD_ACTIONS_1: return o->oAction == 1;
        case HOLD_ACTIONS_KING_BOBOMB: return o->oAction == 1 || o->oAction == 2 || o->oAction == 6;
    }
    return false;
}

static OmmArray omm_obj_get_holdable_behaviors() {
    static OmmArray sOmmHoldableBehaviors = omm_array_zero;
    if (!sOmmHoldableBehaviors.p) {
        const OmmArray *behaviors = omm_get_behaviors_list();
        omm_array_for_each(*behaviors, p_bhv) {
            const BehaviorScript *bhv = p_bhv->as_ptr;
            if (omm_behavior_data_get_holdable(bhv)) {
                omm_array_add(sOmmHoldableBehaviors, ptr, bhv);
            }
        }
    }
    return sOmmHoldableBehaviors;
}

void pobj_hold_object(struct Object *obj) {
    obj_set_home(obj, obj->oPosX, obj->oPosY, obj->oPosZ);
    if (obj->curBhvCommand < bhvOmmHoldBegin || obj->curBhvCommand >= bhvOmmHoldEnd) {
        obj->bhvStack[obj->bhvStackIndex++] = (uintptr_t) obj->curBhvCommand;
        obj->bhvStack[obj->bhvStackIndex++] = (uintptr_t) obj->oFlags;
        obj->curBhvCommand = bhvOmmHold;
    }
    obj->oFlags &= ~(OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL);
    obj->oHeldState = HELD_HELD;
    obj->oIntangibleTimer = -1;
    obj->oRoom = -1;
    obj->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    obj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    obj->activeFlags &= ~ACTIVE_FLAG_IN_DIFFERENT_ROOM;
    for_each_object_in_all_lists(child) {
        if (child->activeFlags && child->parentObj == obj) {
            child->oRoom = -1;
            child->oNodeFlags |= GRAPH_RENDER_ACTIVE;
            child->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            child->activeFlags &= ~ACTIVE_FLAG_IN_DIFFERENT_ROOM;
        }
    }
    gOmmObject->state.heldObj = obj;

    // Surface objects
    // Move the object to the end of the object list
    // to fix collision issues caused by object order
    if (obj_get_list_index(obj) == OBJ_LIST_SURFACE) {
        struct ObjectNode *head = &gObjectLists[OBJ_LIST_SURFACE];
        struct ObjectNode *last = head->prev;
        if (&obj->header != last) {
            struct ObjectNode *prev = obj->header.prev;
            struct ObjectNode *next = obj->header.next;
            prev->next = next;
            next->prev = prev;
            last->next = &obj->header;
            head->prev = &obj->header;
            obj->header.prev = last;
            obj->header.next = head;
        }
    }
}

bool pobj_try_to_hold_object(struct Object *o, f32 radius, f32 height, f32 offset, u16 angleRange) {
    if (!gOmmObject->state.heldObj) {

        // Hold hitbox
        struct Object holdHitbox[1] = {0};
        holdHitbox->oPosX = o->oPosX;
        holdHitbox->oPosY = o->oPosY;
        holdHitbox->oPosZ = o->oPosZ;
        holdHitbox->hitboxRadius = o->hitboxRadius + radius;
        holdHitbox->hitboxHeight = o->hitboxHeight + height;
        holdHitbox->hitboxDownOffset = -offset;

        // Holdable behaviors
        omm_array_for_each(omm_obj_get_holdable_behaviors(), p_bhv) {
            const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;

            // Holdable objects
            for_each_object_with_behavior(obj, bhv) {

                // Check holdable
                if (obj != o && !obj_is_dormant(obj) && obj->oHeldState == HELD_FREE && omm_obj_is_holdable(obj)) {

                    // Check angle
                    u16 angleDiff = (u16) abs_s((s16) (obj_get_object1_angle_yaw_to_object2(o, obj) - o->oFaceAngleYaw));
                    if (angleDiff < angleRange) {

                        // Check hitbox overlap
                        if (obj_detect_hitbox_overlap(holdHitbox, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX_HURTBOX_MAX | OBJ_OVERLAP_FLAG_IGNORE_INTANGIBLE)) {

                            // Hold object
                            pobj_hold_object(obj);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool pobj_update_held_object(struct Object *o, f32 dx, f32 dy, f32 dz, f32 dh, f32 dt) {
    struct Object *obj = gOmmObject->state.heldObj;
    if (obj) {
        Vec3f p0 = { obj->oHomeX, obj->oHomeY, obj->oHomeZ };
        Vec3f p1 = { dx * o->oScaleX, dy * o->oScaleY, dz * o->oScaleZ };
        vec3f_rotate_zxy(p1, p1, o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
        vec3f_add(p1, &o->oPosX);
        obj_set_xyz(obj, lerp_f(dt, p0[0], p1[0]), lerp_f(dt, p0[1], p1[1]) + dh * sins(dt * 0x8000), lerp_f(dt, p0[2], p1[2]));
        obj_set_angle(obj, o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
        obj_update_gfx(obj);
        obj->oOpacity = 0xFF;
        obj->oNodeFlags |= GRAPH_RENDER_ACTIVE;
        obj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
        obj->activeFlags &= ~ACTIVE_FLAG_IN_DIFFERENT_ROOM;
        obj->oIntangibleTimer = -1;
        obj->oRoom = -1;
        return true;
    }
    return false;
}

bool pobj_throw_held_object(struct Object *o, f32 forwardVel, f32 yVel) {
    struct Object *obj = gOmmObject->state.heldObj;
    if (obj) {
        obj_set_angle(obj, 0, o->oFaceAngleYaw, 0);
        obj_set_throw_vel(obj, o, forwardVel, yVel);
        obj->oHeldState = HELD_THROWN;
        obj->oIntangibleTimer = -1;
        gOmmObject->state.heldObj = NULL;
        return true;
    }
    return false;
}

bool pobj_release_held_object() {
    struct Object *obj = gOmmObject->state.heldObj;
    if (obj) {
        obj->oHeldState = HELD_FREE;
        gOmmObject->state.heldObj = NULL;
        return true;
    }
    return false;
}
