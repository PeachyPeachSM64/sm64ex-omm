#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool omm_cappy_scuttlebug_init(UNUSED struct Object *o) {
    clear_surface_data(gOmmObject->scuttlebug.data);
    return true;
}

void omm_cappy_scuttlebug_end(struct Object *o) {
    o->oHomeY = FLOOR_LOWER_LIMIT;
}

u64 omm_cappy_scuttlebug_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SCUTTLEBUG;
}

f32 omm_cappy_scuttlebug_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_scuttlebug_update(struct Object *o) {
    if (gOmmObject->state.actionFlag) {
        o->oFloorHeight = o->oPosY;
    }
    struct Surface *wall = get_surface_from_data(gOmmObject->scuttlebug.data);

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
                gOmmObject->scuttlebug.data->pos,
                gOmmObject->scuttlebug.data->angle,
                gOmmObject->scuttlebug.data->scale,
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
        obj_set_forward_and_y_vel(o, gOmmMario->capture.stickMag * maxVel, 0.f);
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
        pobj_handle_special_floors(o);
        pobj_stop_if_unpossessed();

    } else {

        // Inputs
        if (pobj_process_inputs(o)) {
            pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
            if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
                obj_play_sound(o, POBJ_SOUND_JUMP_2);
            }
        }

        // Movement
        perform_object_step(o, POBJ_STEP_FLAGS);
        pobj_decelerate(o);
        pobj_apply_gravity(o, 1.f);
        pobj_handle_special_floors(o);
        pobj_stop_if_unpossessed();
    }

    // Stick to wall
    if (!POBJ_IS_OPENING_DOORS && o->oWall && o->oVelY <= 0.f && o->oDistToFloor > o->hitboxRadius && POBJ_B_BUTTON_DOWN) {
        o->oFaceAngleYaw = atan2s(-o->oWall->normal.z, -o->oWall->normal.x);
        o->oFloorHeight = o->oPosY;
        if (!gOmmObject->state.actionFlag) {
            obj_spawn_particle_preset(o, PARTICLE_VERTICAL_STAR, false);
            obj_play_sound(o, POBJ_SOUND_DEATH);
            gOmmObject->state.actionFlag = true;
        }
        get_surface_data(gOmmObject->scuttlebug.data, o->oWall);
    } else {
        gOmmObject->state.actionFlag = false;
        clear_surface_data(gOmmObject->scuttlebug.data);
    }

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Animation, sound and particles
    obj_anim_play(o, 0, max_f(1.f, (o->oVelY <= 0.f) * POBJ_ABS_FORWARD_VEL * 2.f / pobj_get_walk_speed(o)));
    if (obj_is_on_ground(o)) {
        obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 12.f, POBJ_ABS_FORWARD_VEL, POBJ_SOUND_WALK_SCUTTLEBUG, OBJ_PARTICLE_NONE);
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_scuttlebug_update_gfx(struct Object *o) {

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
