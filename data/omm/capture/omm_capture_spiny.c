#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define OMM_CAPPY_SPINY_AFTERIMAGES_VEL (60.f)

//
// Init
//

bool omm_cappy_spiny_init(struct Object* o) {
    if (o->oAction != SPINY_ACT_WALK) {
        return false;
    }

    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_scale(o, 1.f);
    return true;
}

void omm_cappy_spiny_end(struct Object *o) {
    o->oGraphNode = geo_layout_to_graph_node(NULL, spiny_geo);
    o->oAnimations = (struct Animation **) spiny_seg5_anims_05016EAC;
    obj_anim_play(o, 0, 1.f);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    obj_scale(o, 1.f);
    obj_update_gfx(o);
}

u64 omm_cappy_spiny_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_SPINY;
}

f32 omm_cappy_spiny_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_spiny_update(struct Object *o) {
    bool wasOnGround = obj_is_on_ground(o);
    obj_scale(o, 1.f);

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Releasing Spin dash
    bool releaseSpinDash = (!POBJ_B_BUTTON_DOWN || !wasOnGround);
    if (gOmmObject->state.actionState == 1 && (releaseSpinDash || POBJ_A_BUTTON_PRESSED || POBJ_IS_OPENING_DOORS || omm_mario_is_locked(gMarioState))) {
        if (releaseSpinDash) {
            omm_sound_play(OMM_SOUND_EFFECT_SONIC_RELEASE, o->oCameraToObject);
            o->oForwardVel = pobj_get_dash_speed(o) * relerp_0_1_f(gOmmObject->state.actionTimer, 0, 15, 0.5f, 1.f);
            o->oAngleVelPitch = 0x2000;
            gOmmMario->capture.stickMag = 1.f;
            gOmmObject->state.actionState = 2;
            gOmmObject->state.actionFlag = true;
        } else {
            o->oAngleVelPitch = 0;
            gOmmObject->state.actionState = 0;
            gOmmObject->state.actionFlag = false;
        }
    }

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND * (gOmmObject->state.actionState != 3);
    POBJ_SET_AFFECTED_BY_CANNON * (gOmmObject->state.actionState != 3);
    POBJ_SET_ABLE_TO_MOVE_ON_WATER * (POBJ_ABS_FORWARD_VEL > OMM_CAPPY_SPINY_AFTERIMAGES_VEL && wasOnGround && !obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ)));
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS;
    POBJ_SET_ATTACKING * (gOmmObject->state.actionFlag != 0);
    POBJ_SET_ATTACKING_BREAKABLE * (gOmmObject->state.actionState >= 2);
    POBJ_SET_GROUND_POUNDING * (gOmmObject->state.actionState == 3);

    // Inputs
    if (pobj_process_inputs(o)) {

        // Charging Spin dash
        if (gOmmObject->state.actionState == 1) {
            gOmmObject->state.actionTimer++;
            gOmmObject->state.actionFlag = true;
            gOmmMario->capture.stickMag = 0.001f;
            o->oAngleVelPitch = 0x2000;
        }

        // Move and jump
        if (gOmmObject->state.actionState == 3) {
            pobj_move_max_vel(o, pobj_get_walk_speed(o) / 4.f);
        } else {
            pobj_move_max_vel(o, max_f(POBJ_ABS_FORWARD_VEL - 0.5f, pobj_get_walk_speed(o)));
            if (pobj_jump(o, 2) == POBJ_RESULT_JUMP_START) {
                if (wasOnGround) {
                    o->oAngleVelPitch = 0x1800;
                    omm_sound_play(OMM_SOUND_EFFECT_SONIC_JUMP_1, o->oCameraToObject);
                } else {
                    o->oAngleVelPitch = 0x2000;
                    omm_sound_play(OMM_SOUND_EFFECT_SONIC_JUMP_2, o->oCameraToObject);
                }
                gOmmObject->state.actionFlag = true;
            }
        }

        // Spin dash, Spin pound
        if (POBJ_B_BUTTON_PRESSED) {
            if (obj_is_on_ground(o)) {
                omm_sound_play(OMM_SOUND_EFFECT_SONIC_CHARGE, o->oCameraToObject);
                gOmmObject->state.actionState = 1;
                gOmmObject->state.actionTimer = 0;
                gOmmObject->state.actionFlag = true;
            } else if (gOmmObject->state.actionState != 3) {
                omm_sound_play(OMM_SOUND_EFFECT_SONIC_SPIN, o->oCameraToObject);
                obj_set_forward_vel(o, o->oFaceAngleYaw, 0, 0);
                o->oAngleVelPitch = 0x3000;
                gOmmObject->state.actionState = 3;
                gOmmObject->state.actionFlag = true;
            }
        }
    }
    pobj_stop_if_unpossessed();

    // Friction
    if (gOmmObject->state.actionState == 2) {
        gOmmObject->state.friction[0] = 0.99f;
        gOmmObject->state.friction[2] = 0.99f;
    }

    // Movement
    if (gOmmObject->state.actionState == 3) {
        o->oVelY = 1.6f * pobj_get_terminal_velocity(o);
    }
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // Spin pound
    if (gOmmObject->state.actionState == 3) {
        if (obj_is_on_ground(o)) {
            o->oVelY = pobj_get_jump_velocity(o);
            o->oAngleVelPitch = 0x1800;
            obj_spawn_particle_preset(o, PARTICLE_MIST_CIRCLE, false);
            obj_spawn_particle_preset(o, PARTICLE_HORIZONTAL_STAR, false);
            obj_play_sound(o, SOUND_ACTION_TERRAIN_HEAVY_LANDING);
            set_camera_shake_from_hit(SHAKE_GROUND_POUND);
            gOmmObject->state.actionState = 0;
        } else {
            obj_spawn_particle_preset(o, PARTICLE_DUST, false);
        }
    }

    // Spin dash
    else if (gOmmObject->state.actionState == 1) {
        struct Object *particle = spawn_object(o, MODEL_SMOKE, bhvWhitePuffSmoke2);
        obj_set_angle(particle, 0, o->oFaceAngleYaw + 0x8000 + random_f32_around_zero(0x1000), 0);
        obj_set_forward_and_y_vel(particle, 15.f + random_float() * 15.f, random_float() * 10.f);
        obj_scale(particle, 1.f + random_float());
        particle->oPosY -= 20.f;
        particle->oGravity = 0.f;
        particle->oTimer = 1;
    }

    // Return to hedgehog form when landing or not enough speed
    else if (obj_is_on_ground(o) && (gOmmObject->state.actionState == 0 || !wasOnGround || POBJ_ABS_FORWARD_VEL < pobj_get_walk_speed(o))) {
        gOmmObject->state.actionFlag = false;
        gOmmObject->state.actionState = 0;
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_spiny_update_gfx(struct Object *o) {

    // Gfx
    o->oFlags |= OBJ_FLAG_SHADOW_COPY_OBJ_POS;

    // Ball form
    if (gOmmObject->state.actionFlag) {
        o->oGraphNode = geo_layout_to_graph_node(NULL, spiny_ball_geo);
        o->oAnimations = (struct Animation **) spiny_egg_seg5_anims_050157E4;
        o->oFaceAnglePitch += o->oAngleVelPitch * relerp_0_1_f(POBJ_ABS_FORWARD_VEL, pobj_get_walk_speed(o), pobj_get_dash_speed(o), 1.f, 2.f);
        obj_scale(o, 1.25f);
        obj_update_gfx(o);

        // The spiny ball would be a perfectly centered sphere, if there wasn't some random offsets in its geo layout...
        Vec3f offset = { -18, -34, 10 };
        Vec3f scale = { o->oScaleX * 0.25f, o->oScaleY * 0.25f, o->oScaleZ * 0.25f };
        vec3f_transform(offset, offset, gVec3fZero, o->oGfxAngle, scale);
        o->oGfxPos[0] += -offset[0];
        o->oGfxPos[1] += -offset[1] + o->hitboxHeight * 0.625f;
        o->oGfxPos[2] += -offset[2];
    }

    // Hedgehog form
    else {
        o->oGraphNode = geo_layout_to_graph_node(NULL, spiny_geo);
        o->oAnimations = (struct Animation **) spiny_seg5_anims_05016EAC;
        o->oFaceAnglePitch = 0;
        obj_update_gfx(o);
    }

    // Update animation
    obj_anim_play(o, 0, gOmmObject->state.actionFlag ? 1.f : max_f(1.f, 4.f * POBJ_ABS_FORWARD_VEL / pobj_get_walk_speed(o)));

    // Spawn afterimages if fast enough
    if (vec3f_length(&o->oVelX) > OMM_CAPPY_SPINY_AFTERIMAGES_VEL) {
        omm_obj_spawn_afterimage(o, 1.f, 1.25f, 0x00, 0x80, 6);
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}
