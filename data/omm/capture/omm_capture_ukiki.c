#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_ukiki_init(struct Object *o) {

    // Undefined Ukiki
    if (o->oBehParams2ndByte != UKIKI_CAGE && o->oBehParams2ndByte != UKIKI_HAT) {
        return false;
    }

    // Already has a hat
    if (o->oUkikiHasHat & UKIKI_HAT_ON) {
        return false;
    }

    // Don't interrupt the "jump and spin on cage" sequence
    if (o->oAction == UKIKI_ACT_GO_TO_CAGE && o->oSubAction >= UKIKI_SUB_ACT_CAGE_TALK_TO_MARIO) {
        return false;
    }

    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    clear_surface_data(gOmmObject->ukiki.data);
    return true;
}

void omm_cappy_ukiki_end(struct Object *o) {
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    o->oHomeY = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
    o->oDistanceToMario = 0;

    // Trigger the "jump and spin on cage" sequence
    if (gOmmObject->state.actionState) {
        o->oAction = UKIKI_ACT_GO_TO_CAGE;
        o->oPrevAction = UKIKI_ACT_GO_TO_CAGE;
        o->oSubAction = UKIKI_SUB_ACT_CAGE_JUMP_TO_CAGE;
    }

    // Restart "run to cage" action to recompute nearest waypoint
    else if (o->oBehParams2ndByte == UKIKI_CAGE && o->oAction == UKIKI_ACT_GO_TO_CAGE) {
        o->oSubAction = UKIKI_SUB_ACT_CAGE_RUN_TO_CAGE;
        o->oPathedStartWaypoint = NULL;
        o->oPathedPrevWaypoint = NULL;
        o->oPathedPrevWaypointFlags = 0;
    }
}

u64 omm_cappy_ukiki_get_type(struct Object *o) {
    switch (o->oBehParams2ndByte) {
        case UKIKI_CAGE: return OMM_CAPTURE_CAGE_UKIKI;
        case UKIKI_HAT: return OMM_CAPTURE_HAT_UKIKI;
    }
    return 0;
}

f32 omm_cappy_ukiki_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_ukiki_update(struct Object *o) {
    if (gOmmObject->state.actionFlag) {
        o->oFloorHeight = o->oPosY;
    }
    bool wasOnGround = obj_is_on_ground(o);
    struct Surface *wall = get_surface_from_data(gOmmObject->ukiki.data);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Wall check
    if (!POBJ_B_BUTTON_DOWN || POBJ_A_BUTTON_PRESSED || POBJ_IS_OPENING_DOORS || omm_mario_is_locked(gMarioState) || !wall) {
        gOmmObject->state.actionFlag = false;
    }

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS * !gOmmObject->state.actionFlag;

    // States
    if (gOmmObject->state.actionFlag) {

        // Apply platform displacement
        if (wall->object) {
            obj_apply_displacement(o,
                wall->object,
                gOmmObject->ukiki.data->pos,
                gOmmObject->ukiki.data->angle,
                gOmmObject->ukiki.data->scale,
                true
            );
        }

        // Inputs
        s16 wallAngle = atan2s(wall->normal.z, wall->normal.x);
        s16 signAngle = sign_f(gOmmMario->capture.stickX);
        gOmmMario->capture.stickYaw = wallAngle + 0x4000 * signAngle;
        gOmmMario->capture.stickMag = abs_f(gOmmMario->capture.stickX);

        // Movement
        o->oPosX -= 5.f * wall->normal.x;
        o->oPosZ -= 5.f * wall->normal.z;
        o->oFaceAngleYaw = gOmmMario->capture.stickYaw;
        f32 maxVel = pobj_get_max_speed(o, POBJ_B_BUTTON_DOWN, false, false) / 1.5f;
        obj_set_forward_and_y_vel(o, gOmmMario->capture.stickMag * maxVel, gOmmMario->capture.stickY * maxVel);
        perform_object_step(o, POBJ_STEP_FLAGS);
        if (!o->oWall) {
            Vec3f orig; obj_pos_as_vec3f(o, orig);
            Vec3f dir = { -wall->normal.x * 2.f * o->oWallHitboxRadius, 0, -wall->normal.z * 2.f * o->oWallHitboxRadius };
            RayCollisionData hits;
            if (!find_collisions_on_ray(orig, dir, &hits, 1.f, RAYCAST_FLAG_WALLS)) {
                vec3f_add(orig, dir);
                vec3f_rotate_around_n(dir, dir, gVec3fY, 0x4000 * signAngle);
                find_collisions_on_ray(orig, dir, &hits, 1.f, RAYCAST_FLAG_WALLS);
            }
            if (hits.count) {
                const RayHit *hit = &hits.hits[0];
                o->oWall = hit->surf;
                o->oPosX = hit->pos[0] + hit->surf->normal.x * o->oWallHitboxRadius;
                o->oPosZ = hit->pos[2] + hit->surf->normal.z * o->oWallHitboxRadius;
                o->oFaceAngleYaw = atan2s(hit->surf->normal.z, hit->surf->normal.x) + 0x4000 * signAngle;
            } else {
                o->oFaceAngleYaw = 0x8000 + wallAngle;
            }
        }
        o->oForwardVel = sqrtf(sqr_f(o->oForwardVel) + sqr_f(o->oVelY));
        o->oVelY = 0.f;
        pobj_handle_special_floors(o);
        pobj_stop_if_unpossessed();

    } else {

        // Inputs
        if (pobj_process_inputs(o)) {
            pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
            if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
                obj_play_sound(o, POBJ_SOUND_JUMP_UKIKI);
            }
        }

        // Movement
        perform_object_step(o, POBJ_STEP_FLAGS);
        pobj_decelerate(o);
        pobj_apply_gravity(o, 1.f);
        pobj_handle_special_floors(o);
        pobj_stop_if_unpossessed();
    }

    // Ukiki star cage
    if (o->oBehParams2ndByte == UKIKI_CAGE && (
        (obj_is_on_ground(o) && o->oFloor->object && o->oFloor->object->behavior == bhvUkikiCage) ||
        (o->oWall            && o->oWall->object  && o->oWall->object->behavior  == bhvUkikiCage) ||
        (o->oCeil            && o->oCeil->object  && o->oCeil->object->behavior  == bhvUkikiCage))
    ) {
        gOmmObject->state.actionState = 1;
        omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 0);
        pobj_stop_if_unpossessed();
    }

    // Stick to wall
    if (!POBJ_IS_OPENING_DOORS && o->oWall && o->oVelY <= 0.f && o->oDistToFloor > o->hitboxRadius && POBJ_B_BUTTON_DOWN) {
        o->oFaceAngleYaw = atan2s(-o->oWall->normal.z, -o->oWall->normal.x);
        o->oFloorHeight = o->oPosY;
        gOmmObject->state.actionFlag = true;
        get_surface_data(gOmmObject->ukiki.data, o->oWall);
    } else {
        gOmmObject->state.actionFlag = false;
        clear_surface_data(gOmmObject->ukiki.data);
    }

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    bool isOnGround = obj_is_on_ground(o);
    if (isOnGround) {
        if (gOmmObject->state.actionFlag || POBJ_IS_WALKING) {
            obj_anim_play(o, UKIKI_ANIM_RUN, max_f(1.f, POBJ_ABS_FORWARD_VEL / pobj_get_walk_speed(o)));
        } else if (!wasOnGround) {
            obj_anim_play(o, UKIKI_ANIM_LAND, 1.f);
        } else if (o->oAnimID != UKIKI_ANIM_LAND || obj_anim_is_at_end(o)) {
            obj_anim_play(o, UKIKI_ANIM_ITCH, 1.f);
        }
        if (obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 12.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_UKIKI, OBJ_PARTICLE_NONE)) {
            gOmmObject->state.actionTimer = 3;
        } else if (--gOmmObject->state.actionTimer == 0) {
            obj_play_sound(o, POBJ_SOUND_WALK_UKIKI);
        }
    } else {
        obj_anim_play(o, UKIKI_ANIM_JUMP, 1.f);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_ukiki_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);
    if (gOmmObject->state.actionFlag) {
        s16 pullAngle = atan2s(-o->oWall->normal.z, -o->oWall->normal.x);
        o->oGfxPos[0] += o->oWallHitboxRadius * sins(pullAngle);
        o->oGfxPos[2] += o->oWallHitboxRadius * coss(pullAngle);
        o->oGfxAngle[0] = -0x4000;
        o->oGfxAngle[1] = pullAngle;
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}
