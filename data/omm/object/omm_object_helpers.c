#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"
#define MOTOS_CONSTANTS_ONLY
#include "behaviors/motos.inc.c"
#undef MOTOS_CONSTANTS_ONLY

f32 obj_get_horizontal_distance(struct Object *o1, struct Object *o2) {
    return sqrtf(sqr_f(o1->oPosX - o2->oPosX) + sqr_f(o1->oPosZ - o2->oPosZ));
}

f32 obj_get_distance(struct Object *o1, struct Object *o2) {
    return sqrtf(sqr_f(o1->oPosX - o2->oPosX) + sqr_f(o1->oPosY - o2->oPosY) + sqr_f(o1->oPosZ - o2->oPosZ));
}

f32 obj_get_distance_vec3f(struct Object *o, Vec3f pos) {
    return sqrtf(sqr_f(o->oPosX - pos[0]) + sqr_f(o->oPosY - pos[1]) + sqr_f(o->oPosZ - pos[2]));
}

bool obj_has_model(struct Object *o, s32 modelId) {
    if (!o || !o->oGraphNode) {
        return false;
    }
    struct Object *p = gCurrentObject;
    gCurrentObject = o;
    bool res = cur_obj_has_model(modelId);
    gCurrentObject = p;
    return res;
}

bool obj_has_geo_layout(struct Object *o, const GeoLayout *georef) {
    return o->oGraphNode && o->oGraphNode->georef == georef;
}

bool obj_has_graph_node(struct Object *o, struct GraphNode *node) {
    return (o->oGraphNode && node && o->oGraphNode->georef == node->georef) || (o->oGraphNode == node);
}

bool obj_is_surface(struct Object *o) {
    s32 type = obj_get_list_index(o);
    return type == OBJ_LIST_SURFACE ||
           type == OBJ_LIST_POLELIKE;
}

bool obj_is_on_ground(struct Object *o) {
    return o->oFloor && o->oDistToFloor < 1.f;
}

// Goomba has to be handled separately due to the Goomba stack
// Return true only if the bottom Goomba is completely submerged
bool obj_is_underwater(struct Object *o, f32 waterLevel) {
    if (omm_obj_is_goomba(o)) {
        if (omm_mario_is_capture(gMarioState) && gOmmCapture == o) {
            return (o->oPosY + omm_capture_get_hitbox_height(o)) < waterLevel;
        }
        return (o->oPosY + o->hitboxHeight - o->hitboxDownOffset) < waterLevel;
    }
    return (o->oPosY + (o->hitboxHeight / 2) - o->hitboxDownOffset) < waterLevel;
}

void obj_set_underwater(struct Object *o, f32 waterLevel) {
    if (omm_obj_is_goomba(o)) {
        if (omm_mario_is_capture(gMarioState) && gOmmCapture == o) {
            o->oPosY = waterLevel - omm_capture_get_hitbox_height(o) - 1.f;
        } else {
            o->oPosY = waterLevel - (o->hitboxHeight - o->hitboxDownOffset) - 1.f;
        }
    } else {
        o->oPosY = waterLevel - (o->hitboxHeight / 2 - o->hitboxDownOffset) - 1.f;
    }
}

s32 obj_get_room(struct Object *o) {
    if (o) {
        struct Surface *floor = NULL;
        find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
        if (floor) {
            return floor->room;
        }
    }
    return -1;
}

s32 obj_get_object1_angle_yaw_to_object2(struct Object *o1, struct Object *o2) {
    return (s32) atan2s(o2->oPosZ - o1->oPosZ, o2->oPosX - o1->oPosX);
}

bool obj_is_object1_facing_object2(struct Object *o1, struct Object *o2, s16 angleRange) {
    s16 yawObj1ToObj2 = obj_get_object1_angle_yaw_to_object2(o1, o2);
    s32 yawDiff = (s32) ((s16) ((s32) yawObj1ToObj2 - (s32) o1->oFaceAngleYaw));
    return -angleRange <= yawDiff && yawDiff <= angleRange;
}

bool obj_is_object2_hit_from_above(struct Object *o1, struct Object *o2) {
    f32 o1l = o1->oPosY - o1->hitboxDownOffset;
    f32 o1u = o1l + o1->hitboxHeight;
    f32 o2l = o2->oPosY - o2->hitboxDownOffset;
    f32 o2u = o2l + o2->hitboxHeight;
    return o1->oVelY < 0.f &&               // going down
           o1u > o2u &&                     // o1's head must be above o2's head
           o1l > o2l + 0.25f * (o2u - o2l); // o1's feet must be slightly above o2's feet
}

bool obj_is_object2_hit_from_below(struct Object *o1, struct Object *o2) {
    f32 o1l = o1->oPosY - o1->hitboxDownOffset;
    f32 o1u = o1l + o1->hitboxHeight;
    f32 o2l = o2->oPosY - o2->hitboxDownOffset;
    f32 o2u = o2l + o2->hitboxHeight;
    return o1->oVelY > 0.f &&               // going up
           o1l < o2l &&                     // o1's feet must be below o2's feet
           o1u < o2u - 0.25f * (o2u - o2l); // o1's head must be slightly below o2's head
}

struct Box { f32 radius; f32 height; };
static struct Box obj_get_box_for_overlap_check(struct Object *o, u32 flags) {
    struct Box box;
    f32 hitboxRadius  = o->hitboxRadius;
    f32 hitboxHeight  = o->hitboxHeight;
    f32 hurtboxRadius = o->hurtboxRadius;
    f32 hurtboxHeight = o->hurtboxHeight;

    if (flags & OBJ_OVERLAP_FLAG_HITBOX_HURTBOX_MAX) {
        box.radius = max_f(hitboxRadius, hurtboxRadius);
        box.height = max_f(hitboxHeight, hurtboxHeight);
    } else if (flags & OBJ_OVERLAP_FLAG_HURTBOX_HITBOX_IF_ZERO) {
        box.radius = hurtboxRadius;
        box.height = hurtboxHeight;
        if (box.radius <= 0 || box.height <= 0) {
            box.radius = hitboxRadius;
            box.height = hitboxHeight;
        }
    } else if (flags & OBJ_OVERLAP_FLAG_HITBOX) {
        box.radius = hitboxRadius;
        box.height = hitboxHeight;
    } else { // No box by default
        box.radius = 0;
        box.height = 0;
    }
    return box;
}

bool obj_has_hitbox(struct Object *o, u32 flags) {
    struct Box box = obj_get_box_for_overlap_check(o, flags);
    return box.radius != 0 && box.height != 0;
}

bool obj_detect_hitbox_overlap(struct Object *o1, struct Object *o2, u32 obj1Flags, u32 obj2Flags) {

    // Tangibility check
    if ((o1->oIntangibleTimer != 0 && !(obj1Flags & OBJ_OVERLAP_FLAG_IGNORE_INTANGIBLE)) ||
        (o2->oIntangibleTimer != 0 && !(obj2Flags & OBJ_OVERLAP_FLAG_IGNORE_INTANGIBLE))) {
        return false;
    }

    // Box dimensions check
    struct Box box1 = obj_get_box_for_overlap_check(o1, obj1Flags);
    struct Box box2 = obj_get_box_for_overlap_check(o2, obj2Flags);
    if (box1.radius == 0 || box1.height == 0 ||
        box2.radius == 0 || box2.height == 0) {
        return false;
    }

    // Cylinder overlap check
    return vec3f_check_cylinder_overlap(
        &o1->oPosX, box1.radius, box1.height, o1->hitboxDownOffset,
        &o2->oPosX, box2.radius, box2.height, o2->hitboxDownOffset
    );
}

f32 *obj_pos_as_vec3f(struct Object *o, Vec3f pos) {
    pos[0] = o->oPosX;
    pos[1] = o->oPosY;
    pos[2] = o->oPosZ;
    return pos;
}

f32 *obj_vel_as_vec3f(struct Object *o, Vec3f vel) {
    vel[0] = o->oVelX;
    vel[1] = o->oVelY;
    vel[2] = o->oVelZ;
    return vel;
}

s16 *obj_angle_as_vec3s(struct Object *o, Vec3s angle) {
    angle[0] = o->oFaceAnglePitch;
    angle[1] = o->oFaceAngleYaw;
    angle[2] = o->oFaceAngleRoll;
    return angle;
}

f32 *obj_scale_as_vec3f(struct Object *o, Vec3f scale) {
    scale[0] = o->oScaleX;
    scale[1] = o->oScaleY;
    scale[2] = o->oScaleZ;
    return scale;
}

void obj_set_pos_vec3f(struct Object *o, Vec3f pos) {
    o->oPosX = pos[0];
    o->oPosY = pos[1];
    o->oPosZ = pos[2];
}

void obj_set_angle_vec3s(struct Object *o, Vec3s angle) {
    o->oFaceAnglePitch = angle[0];
    o->oFaceAngleYaw = angle[1];
    o->oFaceAngleRoll = angle[2];
}

void obj_set_scale_vec3f(struct Object *o, Vec3f scale) {
    o->oScaleX = scale[0];
    o->oScaleY = scale[1];
    o->oScaleZ = scale[2];
}

void obj_set_xyz(struct Object *o, f32 x, f32 y, f32 z) {
    o->oPosX = x;
    o->oPosY = y;
    o->oPosZ = z;
}

void obj_set_home(struct Object *o, f32 x, f32 y, f32 z) {
    o->oHomeX = x;
    o->oHomeY = y;
    o->oHomeZ = z;
}

void obj_set_vel(struct Object *o, f32 x, f32 y, f32 z) {
    o->oVelX = x;
    o->oVelY = y;
    o->oVelZ = z;
    o->oForwardVel = sqrtf(sqr_f(x) + sqr_f(z));
}

void obj_set_throw_vel(struct Object *o, struct Object *thrower, f32 forwardVel, f32 yVel) {
    o->oVelX = (forwardVel != 0.f) ? (thrower->oVelX + forwardVel * sins(o->oFaceAngleYaw)) : 0.f;
    o->oVelY = (yVel       != 0.f) ? (max_f(thrower->oVelY, 0.f) + yVel)                    : 0.f;
    o->oVelZ = (forwardVel != 0.f) ? (thrower->oVelZ + forwardVel * coss(o->oFaceAngleYaw)) : 0.f;
    o->oForwardVel = forwardVel;
}

void obj_set_forward_vel(struct Object *o, s16 yaw, f32 mag, f32 velMax) {
    o->oVelX = sins(yaw) * mag * velMax;
    o->oVelZ = coss(yaw) * mag * velMax;
    o->oForwardVel = mag * velMax;
}

void obj_set_forward_and_y_vel(struct Object *o, f32 forwardVel, f32 yVel) {
    o->oVelX = sins(o->oFaceAngleYaw) * forwardVel;
    o->oVelY = yVel;
    o->oVelZ = coss(o->oFaceAngleYaw) * forwardVel;
    o->oForwardVel = forwardVel;
}

void obj_set_angle_vel(struct Object *o, s16 pitch, s16 yaw, s16 roll) {
    o->oAngleVelPitch = pitch;
    o->oAngleVelYaw = yaw;
    o->oAngleVelRoll = roll;
}

void obj_set_scale(struct Object *o, f32 x, f32 y, f32 z) {
    o->oScaleX = x;
    o->oScaleY = y;
    o->oScaleZ = z;
}

static void apply_drag_to_value(f32 *value, f32 dragStrength) {
    if (*value != 0) {
        f32 decel = (f32) ((f64) (*value) * (f64) (*value) * (f64) dragStrength * 0.0001);
        *value -= (*value > 0 ? decel : -decel);
        if (abs_f(*value) < 0.001f) {
            *value = 0;
        }
    }
}

void obj_apply_drag_xz(struct Object *o, f32 dragStrength) {
    s16 angle = atan2s(o->oVelZ, o->oVelX);
    apply_drag_to_value(&o->oVelX, dragStrength);
    apply_drag_to_value(&o->oVelZ, dragStrength);
    f32 mag = sqrtf(o->oVelX * o->oVelX + o->oVelZ * o->oVelZ);
    o->oVelX = mag * sins(angle);
    o->oVelZ = mag * coss(angle);
}

void obj_apply_displacement(struct Object *o, struct Object *obj, Vec3f prevPos, Vec3s prevAngle, Vec3f prevScale, bool updatePrev) {
    Vec3f offset = {
        o->oPosX - prevPos[0],
        o->oPosY - prevPos[1],
        o->oPosZ - prevPos[2],
    };
    Vec3s rotation = {
        obj->oFaceAnglePitch - prevAngle[0],
        obj->oFaceAngleYaw   - prevAngle[1],
        obj->oFaceAngleRoll  - prevAngle[2],
    };
    Vec3f scale = {
        obj->oScaleX / max_f(0.01f, prevScale[0]),
        obj->oScaleY / max_f(0.01f, prevScale[1]),
        obj->oScaleZ / max_f(0.01f, prevScale[2]),
    };

    // Apply rotation to offset
    if (rotation[0] != 0 || rotation[1] != 0 || rotation[2] != 0) {
        Mat4 displaceMatrix;
        Vec3f relativeOffset;
        Vec3s currAngle;
        obj_angle_as_vec3s(obj, currAngle);
        mtxf_rotate_zxy_and_translate(displaceMatrix, offset, prevAngle);
        linear_mtxf_transpose_mul_vec3f(displaceMatrix, relativeOffset, offset);
        mtxf_rotate_zxy_and_translate(displaceMatrix, offset, currAngle);
        linear_mtxf_mul_vec3f(displaceMatrix, offset, relativeOffset);
    }

    // Compute new position
    o->oPosX = obj->oPosX + scale[0] * offset[0];
    o->oPosY = obj->oPosY + scale[1] * offset[1];
    o->oPosZ = obj->oPosZ + scale[2] * offset[2];
    o->oFaceAngleYaw += rotation[1];

    // Update values
    if (updatePrev) {
        obj_pos_as_vec3f(obj, prevPos);
        obj_angle_as_vec3s(obj, prevAngle);
        obj_scale_as_vec3f(obj, prevScale);
    }
}

void obj_approach_orbit(struct Object *o, f32 t, f32 radius, f32 period, f32 offset, f32 rubberband, s16 pitchVel, s16 yawVel) {

    // Angles
    f32 angle0 = (2 * M_PI * (t    )) / period;
    f32 angle1 = (2 * M_PI * (t - 1)) / period;
    f32 cos0 = cosf(angle0);
    f32 sin0 = sinf(angle0);
    f32 cos1 = cosf(angle1);
    f32 sin1 = sinf(angle1);

    // Previous values
    f32 prevPosX = o->oPosX;
    f32 prevPosY = o->oPosY;
    f32 prevPosZ = o->oPosZ;
    s16 prevPitch = o->oFaceAnglePitch;
    s16 prevYaw = o->oFaceAngleYaw;

    // Compute target position
    f32 targetX = o->oHomeX + radius * sin0;
    f32 targetY = o->oHomeY + offset * sin0;
    f32 targetZ = o->oHomeZ + radius * cos0;

    // Compute difference between current and target
    Vec3f diff = {
        targetX - o->oPosX,
        targetY - o->oPosY,
        targetZ - o->oPosZ,
    };

    // Compute max speed
    Vec3f speed = {
        radius * (sin0 - sin1),
        offset * (sin0 - sin1),
        radius * (cos0 - cos1),
    };
    f32 distToHome = vec3f_hdist(&o->oPosX, &o->oHomeX);
    f32 distToTarget = vec3f_length(diff);
    f32 speedValue = vec3f_length(speed);
    f32 speedMult = lerp_f(distToHome / radius, 1.f, rubberband) / relerp_0_1_f(distToTarget / speedValue, 1.f, 5.f, sqrtf(rubberband), 1.f);
    f32 maxSpeed = speedValue * speedMult;

    // Try to get as close as possible to target
    if (o->oTimer == 0 || distToTarget <= maxSpeed) {
        o->oPosX = targetX;
        o->oPosY = targetY;
        o->oPosZ = targetZ;
    } else {
        vec3f_set_mag(diff, maxSpeed);
        o->oPosX += diff[0];
        o->oPosY += diff[1];
        o->oPosZ += diff[2];
    }

    // Update velocity
    o->oVelX = o->oPosX - prevPosX;
    o->oVelY = o->oPosY - prevPosY;
    o->oVelZ = o->oPosZ - prevPosZ;

    // Update angles
    if (vec3f_length(&o->oVelX) > 1.f) {
        f32 dist; s16 pitch, yaw;
        vec3f_get_dist_and_angle(gVec3fZero, &o->oVelX, &dist, &pitch, &yaw);
        obj_set_angle(o, approach_s16_symmetric(o->oFaceAnglePitch, -pitch, pitchVel), approach_s16_symmetric(o->oFaceAngleYaw, yaw, yawVel), 0);
    }

    // Update angle velocities
    o->oAngleVelPitch = o->oFaceAnglePitch - prevPitch;
    o->oAngleVelYaw = o->oFaceAngleYaw - prevYaw;
}

f32 obj_bounce_on_wall(struct Object *o, struct Surface *surf, bool updateForwardVel, bool updateFaceYaw) {
    Vec2f v = { o->oVelX, o->oVelZ };
    Vec2f n = { surf->normal.x, surf->normal.z };
    vec2f_normalize(n);
    f32 dot = vec2f_dot(v, n);
    if (dot < 0.f) {
        o->oVelX = v[0] - 2 * dot * n[0];
        o->oVelZ = v[1] - 2 * dot * n[1];
        if (updateForwardVel) {
            o->oForwardVel = sign_f(o->oForwardVel) * sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelZ));
        }
        if (updateFaceYaw) {
            o->oFaceAngleYaw = atan2s(o->oVelZ, o->oVelX) + 0x8000 * (o->oForwardVel < 0.f);
        }
    }
    f32 mag = vec2f_length(v);
    return mag ? (dot / mag) : 0.f;
}

static bool obj_should_safe_step(struct Object *o) {

    // Coin
    if (omm_obj_is_coin(o)) {
        return true;
    }

    // Goomba
    if (omm_obj_is_goomba(o) && !omm_obj_is_goomba_stack(o)) {
        return true;
    }

    // Bob-omb
    if (o->behavior == bhvBobomb && (o->oAction == BOBOMB_ACT_PATROL || o->oAction == BOBOMB_ACT_CHASE_MARIO)) {
        return true;
    }

    return false;
}

static void obj_safe_step_set(struct Object *o, f32 floorHeight, bool floorObject) {
    o->oSafeStep = (((s32) (floorHeight)) << 2) | (floorObject << 1) | 1;
    o->oPrevPosX = o->oPosX;
    o->oPrevPosY = o->oPosY;
    o->oPrevPosZ = o->oPosZ;
}

static bool obj_safe_step_get(struct Object *o, f32 *floorHeight, bool *floorObject) {
    if (o->oSafeStep & 1) {
        *floorHeight = (f32) (o->oSafeStep >> 2);
        *floorObject = (o->oSafeStep >> 1) & 1;
        return true;
    }
    return false;
}

static void obj_safe_step_before_update(struct Object *o) {
    struct Surface *floor;
    f32 floorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
    if (floor && !SURFACE_IS_LETHAL(floor->type)) {
        obj_safe_step_set(o, floorHeight, floor->object != NULL);
    } else {
        o->oSafeStep = 0;
    }
}

static void obj_safe_step_after_update(struct Object *o) {
    f32 prevFloorHeight;
    bool prevFloorObject;
    if (obj_safe_step_get(o, &prevFloorHeight, &prevFloorObject)) {
        struct Surface *floor;
        f32 floorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
        if (!floor || SURFACE_IS_LETHAL(floor->type) || (floor->object && !prevFloorObject) || (prevFloorHeight - floorHeight) > 300.f) {
            o->oPosX = o->oPrevPosX;
            o->oPosZ = o->oPrevPosZ;
            if (o->oPrevPosY <= prevFloorHeight + 5.f) {
                o->oPosY = prevFloorHeight;
                o->oForwardVel = 0.f;
            }
        }
    }
    o->oSafeStep = 0;
}

void obj_safe_step(struct Object *o, bool updated) {
    if (o->activeFlags && o != gOmmCapture && o->oAction < 100 && o->oHeldState == HELD_FREE && obj_should_safe_step(o)) {
        if (updated) {
            obj_safe_step_after_update(o);
        } else {
            obj_safe_step_before_update(o);
        }
    }
}

void obj_set_interact_id(struct Object *o, struct Object *child) {
    static s16 sInteractId = 1;
    if (!o->oInteractId || !child) {
        o->oInteractId = sInteractId++;
    }
    if (child) {
        child->oInteractId = o->oInteractId;
    }
}

void obj_update_billboard(struct Object *o) {
    if (o->oGraphNode->noBillboard) {
        o->oNodeFlags &= ~GRAPH_RENDER_BILLBOARD;
    } else {
        o->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
    }
}

void obj_rotate_billboard(struct Object *o, s16 angle) {
    if (o->oGraphNode->noBillboard) {
        o->activeFlags &= ~ACTIVE_FLAG_BILLBOARD_ROLL;
        o->oNodeFlags &= ~GRAPH_RENDER_BILLBOARD;
        o->oFaceAnglePitch += angle;
        o->oFaceAngleRoll = 0;
    } else {
        o->activeFlags |= ACTIVE_FLAG_BILLBOARD_ROLL;
        o->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
        o->oFaceAnglePitch = 0;
        o->oFaceAngleRoll += angle;
    }
}

void obj_copy_visibility_and_transparency(struct Object *dst, struct Object *src) {
    dst->oFlags &= ~OBJ_FLAG_INVISIBLE_MODE;
    dst->oFlags |= (src->oFlags & OBJ_FLAG_INVISIBLE_MODE);
    dst->oNodeFlags &= ~(GRAPH_RENDER_ACTIVE | GRAPH_RENDER_INVISIBLE | GRAPH_RENDER_ALWAYS);
    dst->oNodeFlags |= (src->oNodeFlags & (GRAPH_RENDER_ACTIVE | GRAPH_RENDER_INVISIBLE | GRAPH_RENDER_ALWAYS));
    dst->oTransparency = src->oTransparency;
}

void obj_update_blink_state(struct Object *o, s32 *timer, s16 base, s16 range, s16 length) {
    if (*timer > 0) (*timer)--;
    else *timer = base + (s16) (range * random_float());
    o->oAnimState = (*timer <= length);
}

void obj_random_blink(struct Object *o, s32 *timer) {
    if (*timer > 0) {
        (*timer)++;
        *timer *= (*timer < 16);
        o->oAnimState = 1 - (((*timer + 9) / 5) & 1);
    } else if (random_u16() % 100 == 0) {
        o->oAnimState = 1;
        *timer = 1;
    }
}

void obj_produce_water_effects(struct Object *o, bool isOnGround, bool isUnderwater, bool wasUnderwater, f32 waterLevel) {

    // Shallow water
    if (isOnGround && !isUnderwater && o->oPosY < waterLevel) {
        spawn_object_with_scale(o, MODEL_WAVE_TRAIL, bhvObjectWaveTrail, o->hitboxRadius / 50.f);
        if (abs_f(o->oForwardVel) > 2.f && (gGlobalTimer % 4) == 0) {
            obj_spawn_particle_preset(o, PARTICLE_SHALLOW_WATER_WAVE, false);
        }
    }

    // Transition above/under water
    if (isUnderwater != wasUnderwater) {
        obj_spawn_particle_preset(o, PARTICLE_WATER_SPLASH, false);
        obj_play_sound(o, POBJ_SOUND_WATER_SPLASH);
    }

    // Spawn bubbles underwater
    if (isUnderwater && (gGlobalTimer % 15) == 0) {
        if (random_u16() % 4 == 0) {
            spawn_object(o, MODEL_BUBBLE, bhvBubbleParticleSpawner);
        } else {
            spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvObjectBubble);
        }
    }
}

bool obj_make_step_sound_and_particle(struct Object *o, f32 *dist, f32 distMin, f32 distInc, s32 soundBits, u32 particles) {
    static f32 sdist = 0.f;
    if (!dist) {
        dist = &sdist;
        distMin = 0.f;
        distInc = 0.f;
    }
    *dist += distInc;
    if (*dist >= distMin) {
        *dist -= distMin;

        // Particles
        if (particles & OBJ_PARTICLE_MIST) {
            spawn_object(o, MODEL_NONE, bhvMistParticleSpawner);
        }
        if (particles & OBJ_PARTICLE_SMOKE) {
            spawn_object(o, MODEL_SMOKE, bhvWhitePuffSmoke);
        }
        if (particles & OBJ_PARTICLE_WATER_TRAIL) {
            spawn_object(o, MODEL_WAVE_TRAIL, bhvObjectWaveTrail);
        }
        if (particles & OBJ_PARTICLE_WATER_DROPLET) {
            for (s32 i = 0; i != 2; ++i) {
                struct Object *drop = spawn_object_with_scale(o, MODEL_WHITE_PARTICLE_SMALL, bhvWaterDroplet, 1.5f);
                drop->oVelY = random_float() * 30.f;
                obj_translate_xz_random(drop, 110.f);
            }
        }
        if (particles & OBJ_PARTICLE_FIRE) {
            omm_obj_spawn_fire_smoke(o, 0);
        }
        if (particles & OBJ_PARTICLE_FLAME) {
            spawn_object(o, MODEL_RED_FLAME, bhvKoopaShellFlame);
            spawn_object(o, MODEL_RED_FLAME, bhvKoopaShellFlame);
        }
        if (particles & OBJ_PARTICLE_BUBBLE) {
            spawn_object(o, MODEL_BUBBLE, bhvBubbleParticleSpawner);
        }

        // Sound effect
        obj_play_sound(o, soundBits);
        return true;
    }
    return false;
}

void obj_play_sound(struct Object *o, s32 soundBits) {
    if (soundBits != NO_SOUND) {
        if (o) {
            play_sound(soundBits, o->oCameraToObject);
        } else {
            play_sound(soundBits, gGlobalSoundArgs);
        }
    }
}

void obj_create_sound_spawner(struct Object *o, s32 soundBits) {
    struct Object *obj = spawn_object(o, 0, bhvSoundSpawner);
    obj->oSoundEffectUnkF4 = soundBits;
}

s32 obj_get_model_id(struct Object *o) {
    for (s32 i = 0; i != 0x100; ++i) {
        if (obj_has_model(o, i)) {
            return i;
        }
    }
    return MODEL_NONE;
}

s32 obj_get_coin_type(struct Object *o) {

    // Yellow coin
    if (obj_has_model(o, MODEL_YELLOW_COIN) || obj_has_model(o, MODEL_YELLOW_COIN_NO_SHADOW)) {
        return 0;
    }

    // Red coin
    if (obj_has_model(o, MODEL_RED_COIN) || obj_has_model(o, MODEL_RED_COIN_NO_SHADOW)) {
        return 1;
    }

    // Blue coin
    if (obj_has_model(o, MODEL_BLUE_COIN) || obj_has_model(o, MODEL_BLUE_COIN_NO_SHADOW)) {
        return 2;
    }

    // Not a coin
    return -1;
}

void obj_collect_coins(UNUSED struct Object *o, s32 numCoins) {
    static struct Object coin[1];
    if (numCoins < 0) {
        coin->oDamageOrCoinValue = 5;
        omm_mario_interact_coin(gMarioState, coin);
    } else while (numCoins--) {
        coin->oDamageOrCoinValue = 1;
        omm_mario_interact_coin(gMarioState, coin);
    }
}

void obj_spawn_coins(struct Object *o, s32 numCoins) {
    if (o->oFlags & OBJ_FLAG_YOSHI_DESTROY) {
        obj_collect_coins(o, numCoins);
    } else if (numCoins < 0) {
        spawn_object(o, MODEL_BLUE_COIN, bhvMrIBlueCoin);
    } else {
        obj_spawn_loot_yellow_coins(o, (o->oNumLootCoins = numCoins), 20.f);
    }
    o->oNumLootCoins = 0;
}

void obj_spawn_star_no_cutscene(struct Object *o, f32 x, f32 y, f32 z, s32 starIndex, bool noExit) {
    struct Object *star = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, x, y, z, 0, 0, 0);
    star->oBehParams = ((starIndex & 0x1F) << 24);
    star->oHomeX = x;
    star->oHomeY = y;
    star->oHomeZ = z;
    star->oFaceAnglePitch = 0;
    star->oFaceAngleRoll = 0;
    if (noExit) star->oInteractionSubtype |= INT_SUBTYPE_NO_EXIT;
}

void obj_spawn_star_with_cutscene(struct Object *o, f32 xFrom, f32 yFrom, f32 zFrom, f32 xTo, f32 yTo, f32 zTo, s32 starIndex, bool noExit, bool redCoinStarCutscene) {
    struct Object *star = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStarSpawnCoordinates, xFrom, yFrom, zFrom, 0, 0, 0);
    star->oBehParams = ((starIndex & 0x1F) << 24);
    star->oHomeX = xTo;
    star->oHomeY = yTo;
    star->oHomeZ = zTo;
    star->oFaceAnglePitch = 0;
    star->oFaceAngleRoll = 0;
    if (noExit) star->oInteractionSubtype |= INT_SUBTYPE_NO_EXIT;
    if (redCoinStarCutscene) star->oBehParams2ndByte = 1;
}

void obj_spawn_white_puff(struct Object *o, s32 soundBits) {
    return obj_spawn_white_puff_at(o->oPosX, o->oPosY, o->oPosZ, soundBits);
}

void obj_spawn_white_puff_at(f32 x, f32 y, f32 z, s32 soundBits) {
    for (s32 i = 0; i < 16; ++i) {
        struct Object *particle = spawn_object(gMarioObject, MODEL_MIST, bhvWhitePuffExplosion);
        if (particle) {
            particle->oPosX = x;
            particle->oPosY = y;
            particle->oPosZ = z;
            particle->oBehParams2ndByte = 2;
            particle->oMoveAngleYaw = random_u16();
            particle->oGravity = -4.f;
            particle->oDragStrength = 30.f;
            particle->oForwardVel = random_float() * 10.f + 40.f;
            particle->oVelY = random_float() * 20.f + 30.f;
            f32 scale = random_float() * 1.f + 5.f;
            obj_scale_xyz(particle, scale, scale, scale);
        }
    }
    if (soundBits != NO_SOUND) {
        struct Object *sound = spawn_object(gMarioObject, 0, bhvSoundSpawner);
        if (sound) {
            sound->oPosX = x;
            sound->oPosY = y;
            sound->oPosZ = z;
            sound->oSoundEffectUnkF4 = soundBits;
        }
    }
}

void obj_spawn_triangle_break_particles(struct Object *o, s32 count, s32 model, f32 size, s32 type) {
    for (s32 i = 0; i != count; ++i) {
        struct Object *triangle   = spawn_object(o, model, bhvBreakBoxTriangle);
        triangle->oAnimState      = type;
        triangle->oPosY          += 100.f;
        triangle->oMoveAngleYaw   = random_u16();
        triangle->oFaceAngleYaw   = triangle->oMoveAngleYaw;
        triangle->oFaceAnglePitch = random_u16();
        triangle->oVelY           = random_f32_around_zero(50.f);
        triangle->oForwardVel     = 30.f;
        triangle->oAngleVelPitch  = 0x1800 - 0x0800 * (model == MODEL_DIRT_ANIMATION);
        triangle->oAngleVelYaw    = 0x0000 + 0x0500 * (model == MODEL_DIRT_ANIMATION);
        obj_scale(triangle, size);
    }
}

void obj_spawn_break_particles(struct Object *o, s32 count, u8 r, u8 g, u8 b, f32 offsetY, f32 velMin, f32 velMax, f32 velY, f32 scaleMin, f32 scaleMax) {
    for (s32 i = 0; i < count; i++) {
        omm_obj_spawn_break_particle(o, r, g, b, offsetY, velMin, velMax, velY, scaleMin, scaleMax);
    }
}

void obj_spawn_particles(struct Object *o, s32 count, s32 model, f32 yOffset, f32 forwardVelBase, f32 forwardVelRange, f32 yVelBase, f32 yVelRange, f32 gravity, f32 scaleBase, f32 scaleRange) {
    for (s32 i = 0; i < count; i++) {
        struct Object *particle = spawn_object(o, model, bhvWhitePuffExplosion);
        particle->oMoveAngleYaw = random_u16();
        particle->oGravity = gravity;
        particle->oPosY += yOffset;
        particle->oForwardVel = forwardVelBase + forwardVelRange * random_float();
        particle->oVelY = yVelBase + yVelRange * random_float();
        f32 scale = scaleBase + scaleRange * random_float();
        obj_scale_xyz(particle, scale, scale, scale);
    }
}

void obj_spawn_particle_preset(struct Object *o, u32 particle, bool setActiveParticle) {
    static const struct { u32 particle; u32 active; s32 model; const BehaviorScript *behavior; } OBJ_PARTICLE_PRESETS[] = {
        { PARTICLE_DUST,                 ACTIVE_PARTICLE_DUST,                 MODEL_MIST,                 bhvMistParticleSpawner     },
        { PARTICLE_VERTICAL_STAR,        ACTIVE_PARTICLE_V_STAR,               MODEL_NONE,                 bhvVertStarParticleSpawner },
        { PARTICLE_HORIZONTAL_STAR,      ACTIVE_PARTICLE_H_STAR,               MODEL_NONE,                 bhvHorStarParticleSpawner  },
        { PARTICLE_SPARKLES,             ACTIVE_PARTICLE_SPARKLES,             MODEL_SPARKLES,             bhvSparkleParticleSpawner  },
        { PARTICLE_BUBBLE,               ACTIVE_PARTICLE_BUBBLE,               MODEL_BUBBLE,               bhvBubbleParticleSpawner   },
        { PARTICLE_WATER_SPLASH,         ACTIVE_PARTICLE_WATER_SPLASH,         MODEL_WATER_SPLASH,         bhvWaterSplash             },
        { PARTICLE_IDLE_WATER_WAVE,      ACTIVE_PARTICLE_IDLE_WATER_WAVE,      MODEL_IDLE_WATER_WAVE,      bhvIdleWaterWave           },
        { PARTICLE_PLUNGE_BUBBLE,        ACTIVE_PARTICLE_PLUNGE_BUBBLE,        MODEL_WHITE_PARTICLE_SMALL, bhvPlungeBubble            },
        { PARTICLE_WAVE_TRAIL,           ACTIVE_PARTICLE_WAVE_TRAIL,           MODEL_WAVE_TRAIL,           bhvWaveTrail               },
        { PARTICLE_FIRE,                 ACTIVE_PARTICLE_FIRE,                 MODEL_RED_FLAME,            bhvFireParticleSpawner     },
        { PARTICLE_SHALLOW_WATER_WAVE,   ACTIVE_PARTICLE_SHALLOW_WATER_WAVE,   MODEL_NONE,                 bhvShallowWaterWave        },
        { PARTICLE_SHALLOW_WATER_SPLASH, ACTIVE_PARTICLE_SHALLOW_WATER_SPLASH, MODEL_NONE,                 bhvShallowWaterSplash      },
        { PARTICLE_LEAF,                 ACTIVE_PARTICLE_LEAF,                 MODEL_NONE,                 bhvLeafParticleSpawner     },
        { PARTICLE_SNOW,                 ACTIVE_PARTICLE_SNOW,                 MODEL_NONE,                 bhvSnowParticleSpawner     },
        { PARTICLE_BREATH,               ACTIVE_PARTICLE_BREATH,               MODEL_NONE,                 bhvBreathParticleSpawner   },
        { PARTICLE_DIRT,                 ACTIVE_PARTICLE_DIRT,                 MODEL_NONE,                 bhvDirtParticleSpawner     },
        { PARTICLE_MIST_CIRCLE,          ACTIVE_PARTICLE_MIST_CIRCLE,          MODEL_NONE,                 bhvMistCircParticleSpawner },
        { PARTICLE_TRIANGLE,             ACTIVE_PARTICLE_TRIANGLE,             MODEL_NONE,                 bhvTriangleParticleSpawner },
    };
    if (!setActiveParticle || !(o->oActiveParticleFlags & particle)) {
        for (s32 i = 0; i != array_length(OBJ_PARTICLE_PRESETS); ++i) {
            if (OBJ_PARTICLE_PRESETS[i].particle == particle) {
                o->oActiveParticleFlags |= (OBJ_PARTICLE_PRESETS[i].active * setActiveParticle);
                spawn_object(o, OBJ_PARTICLE_PRESETS[i].model, OBJ_PARTICLE_PRESETS[i].behavior);
                return;
            }
        }
    }
}

static void obj_destroy_white_puff(struct Object *o, s32 numCoins, s32 soundBits) {
    obj_spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    obj_mark_for_deletion(o);
}

static void obj_destroy_white_puff_and_children(struct Object *o, s32 numCoins, s32 soundBits, const BehaviorScript *childBehavior) {
    for_each_object_with_behavior(obj, childBehavior) {
        if (obj->parentObj == o) {
            obj_mark_for_deletion(obj);
        }
    }
    obj_destroy_white_puff(o, numCoins, soundBits);
}

static void obj_destroy_water_bomb(struct Object *o, s32 soundBits) {
    for_each_object_with_behavior(obj, bhvWaterBombShadow) {
        if (obj->parentObj == o) {
            obj_mark_for_deletion(obj);
        }
    }
    o->parentObj->oWaterBombSpawnerBombActive = FALSE;
    obj_create_sound_spawner(o, soundBits);
    obj_spawn_particles(o, 5, MODEL_BUBBLE, 25, 20, 60, 10, 10, -2, 3.5f, 1.f);
    obj_mark_for_deletion(o);
}

static void obj_destroy_triangle_particles(struct Object *o, s32 numCoins, s32 soundBits, s32 triCount, s32 triModel, f32 triSize, s32 triType) {
    obj_spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    obj_spawn_triangle_break_particles(o, triCount, triModel, triSize, triType);
    obj_mark_for_deletion(o);
}

static void obj_destroy_snow_particles(struct Object *o, s32 numCoins, s32 soundBits) {
    obj_spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    f32 scale = o->oScaleX;
    obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, -o->hitboxDownOffset, 4 * scale, 2 * scale, 8 * scale, 4 * scale, -2 * sqrtf(scale), 0.25f * scale, 0.25f * scale);
    obj_mark_for_deletion(o);
}

static void obj_destroy_big_snowball(struct Object *o, s32 numCoins, s32 soundBits) {
    obj_spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    f32 scale = o->oScaleX;
    obj_spawn_particles(o, 16, MODEL_WHITE_PARTICLE, 200 * scale, 0, 60 * scale, -30 * scale, 90 * scale, -4.f * scale, 3.f * scale, 1.f * scale);
    obj_mark_for_deletion(o);
    obj_create_respawner(o, MODEL_CCM_SNOWMAN_BASE, bhvSnowmansBottom, 2000.f);
}

static void obj_destroy_explosion(struct Object *o, s32 numCoins) {
    obj_spawn_coins(o, numCoins);
    set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
    obj_spawn_white_puff(o, SOUND_GENERAL2_BOBOMB_EXPLOSION);
    if (!(o->oFlags & OBJ_FLAG_YOSHI_DESTROY)) {
        struct Object *explosion = spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
        explosion->oGraphYOffset += 100.f;
    }
    obj_mark_for_deletion(o);
}

static void obj_destroy_explosion_and_respawn(struct Object *o, s32 numCoins, s32 model, f32 minSpawnDist) {
    obj_destroy_explosion(o, numCoins);
    obj_create_respawner(o, model, o->behavior, minSpawnDist);
}

static void obj_destroy_break_particles(struct Object *o, s32 numCoins, s32 soundBits, u8 r, u8 g, u8 b) {
    obj_spawn_coins(o, numCoins);
    set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
    obj_spawn_white_puff(o, soundBits);
    obj_spawn_break_particles(o, 24, r, g, b, 150.f, 0, 40, 32, 1.f, 2.f);
    obj_mark_for_deletion(o);
}

static void obj_destroy_whomp_king(struct Object *o) {
    obj_play_sound(o, SOUND_OBJ2_WHOMP_SOUND_SHORT);
    obj_play_sound(o, SOUND_OBJ_KING_WHOMP_DEATH);
    o->oIntangibleTimer = -1;
    o->oHealth = 0;
    o->oAction = 8;
}

static void obj_destroy_boo(struct Object *o, s32 soundBits) {

    // Ghost Hunt boo
    if (o->behavior == bhvGhostHuntBoo && o->oBooParentBigBoo) {
        o->oBooParentBigBoo->oBigBooNumMinionBoosKilled++;
        if (o->oBooParentBigBoo->oBigBooNumMinionBoosKilled == 5) {
            audio_play_puzzle_jingle();
        }
    }

    // Merry-Go-Round boo
    if (o->behavior == bhvMerryGoRoundBoo && o->parentObj && o->parentObj->behavior == bhvMerryGoRoundBooManager) {
        o->parentObj->oMerryGoRoundBooManagerNumBoosKilled++;
    }

    // Coin inside boo
    struct Object *coin = obj_get_first_with_behavior_and_parent(o, bhvCoinInsideBoo);
    if (coin) {
        obj_spawn_coins(o, obj_get_coin_type(coin) == 2 ? -1 : 1);
        obj_mark_for_deletion(coin);
    }
    obj_spawn_white_puff(o, soundBits);
    obj_mark_for_deletion(o);
}

static void obj_destroy_big_mr_i(struct Object *o, s32 soundBits) {
    obj_play_sound(o, soundBits);
    o->oIntangibleTimer = -1;
    o->oAction = 3;
    o->oPrevAction = 3;
    o->oSubAction = 0;
    o->oTimer = 103;
}

static void obj_destroy_bully(struct Object *o, s32 actDeath) {
    if (o->oFlags & OBJ_FLAG_YOSHI_DESTROY) {
        if (o->parentObj && o->oBehParams2ndByte == BULLY_BP_SIZE_SMALL && o->oBullySubtype == BULLY_STYPE_MINION) {
            o->parentObj->oBullyKBTimerAndMinionKOCounter++;
        }
        obj_destroy_white_puff(o, 1, SOUND_OBJ_DEFAULT_DEATH);
    } else {
        o->oIntangibleTimer = -1;
        o->oAction = actDeath;
        o->oPrevAction = actDeath;
        o->oSubAction = 0;
        if (!o->oFloor) {
            find_floor(o->oPosX, o->oPosY, o->oPosZ, &o->oFloor);
        }
        if (o->oFloor && (o->oFloor->type == SURFACE_BURNING || o->oFloor->type == SURFACE_INSTANT_QUICKSAND || o->oFloor->type == SURFACE_INSTANT_MOVING_QUICKSAND)) {
            o->oTimer = 0;
        } else {
            o->oTimer = 31; // Skip the sinking part
        }
    }
}

static void obj_destroy_pokey(struct Object *o, s32 numCoins, s32 soundBits) {
    struct Object *pokey = (o->behavior == bhvPokey ? o : o->parentObj);
    if (o == pokey || o->oBehParams2ndByte == 0) {
        obj_spawn_coins(o, numCoins);
        obj_spawn_white_puff(o, soundBits);
        while ((o = obj_get_first_with_behavior_and_parent(pokey, bhvPokeyBodyPart)) != NULL) {
            obj_mark_for_deletion(o);
        }
        obj_mark_for_deletion(pokey);
    } else {
        pokey->oPokeyNumAliveBodyParts--;
        pokey->oPokeyAliveBodyPartFlags &= ~(1 << o->oBehParams2ndByte);
        obj_spawn_white_puff(o, soundBits);
        obj_mark_for_deletion(o);
    }
}

static void obj_destroy_eyerok_hand(struct Object *o, s32 soundBits, u8 r, u8 g, u8 b) {
    if (o->parentObj->oAction >= EYEROK_BOSS_ACT_FIGHT) {
        o->parentObj->oEyerokBossNumHands--;
        set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
        obj_spawn_white_puff(o, soundBits);
        obj_spawn_break_particles(o, 24, r, g, b, 150.f, 0, 40, 32, 1.f, 2.f);
        obj_mark_for_deletion(o);
    }
}

static void obj_destroy_ukiki_cage(struct Object *o) {
    if (o->oAction == UKIKI_CAGE_ACT_WAIT_FOR_UKIKI) {
        audio_play_puzzle_jingle();
        set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
        obj_spawn_white_puff(o, SOUND_GENERAL_BREAK_BOX);
        obj_spawn_triangle_break_particles(o, OBJ_SPAWN_TRI_BREAK_PRESET_DIRT);
        struct Object *ukikiCage = obj_get_first_with_behavior_and_field_s32(bhvUkiki, _FIELD(oBehParams2ndByte), UKIKI_CAGE);
        if (ukikiCage) {
            obj_mark_for_deletion(ukikiCage);
        }
        struct Object *ukikiCageChild = obj_get_first_with_behavior_and_parent(o, bhvUkikiCageChild);
        if (ukikiCageChild) {
            obj_mark_for_deletion(ukikiCageChild);
        }
        struct Object *ukikiCageStar = obj_get_first_with_behavior_and_parent(o, bhvUkikiCageStar);
        if (ukikiCageStar) {
            spawn_object(ukikiCageStar, MODEL_STAR, bhvStar)->oBehParams = ukikiCageStar->oBehParams;
            obj_mark_for_deletion(ukikiCageStar);
        }
        obj_mark_for_deletion(o);
    }
}

static void obj_destroy_fire_piranha_plant(struct Object *o, s32 numCoins, s32 soundBits) {
    obj_spawn_coins(o, numCoins);
    if (o->oBehParams >> 16) {
        o->oHealth = 0;
        o->oAction = FIRE_PIRANHA_PLANT_ACT_HIDE;
        o->oFirePiranhaPlantDeathSpinTimer = 0;
        o->oFirePiranhaPlantScale = 0;
        o->oFirePiranhaPlantActive = TRUE;
    } else {
        obj_spawn_white_puff(o, soundBits);
        obj_mark_for_deletion(o);
    }
}

static void obj_destroy_wooden_post(struct Object *o, s32 numCoins, s32 soundBits) {
    struct Object *chainChomp = o->parentObj;
    if (chainChomp && chainChomp->behavior == bhvChainChomp && chainChomp != gOmmCapture) {
        chainChomp->behavior = bhvOmmChainChompFree;
        chainChomp->curBhvCommand = bhvOmmChainChompFree;
        chainChomp->bhvStackIndex = 0;
    }
    obj_spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    obj_spawn_triangle_break_particles(o, OBJ_SPAWN_TRI_BREAK_PRESET_DIRT);
    obj_mark_for_deletion(o);
}

static bool obj_destroy_check_condition(struct Object *o, const OmmBhvDataDestroy *destroy) {
    switch (destroy->cond) {
        case DESTROY_COND_NONE:           return true;
        case DESTROY_COND_REGULAR_GOOMBA: return (o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK) == GOOMBA_SIZE_REGULAR;
        case DESTROY_COND_TINY_GOOMBA:    return (o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK) == GOOMBA_SIZE_TINY;
        case DESTROY_COND_HUGE_GOOMBA:    return (o->oBehParams2ndByte & GOOMBA_BP_SIZE_MASK) == GOOMBA_SIZE_HUGE;
        case DESTROY_COND_BIG_MR_I:       return (o->oBehParams2ndByte != 0);
    }
    return false;
}

static s32 obj_destroy_get_num_coins(struct Object *o, s64 arg) {
    switch (arg) {
        case DESTROY_COINS_BLUE:      return -1;
        case DESTROY_COINS_BLUE_ONCE: return (o->oNumLootCoins ? -1 : 0);
        case DESTROY_COINS_REMAINING: return o->oNumLootCoins;
    }
    return (s32) arg;
}

void obj_destroy(struct Object *o) {
    for (const OmmBhvDataDestroy *destroy = omm_behavior_data_get_destroy(o->behavior); destroy; destroy = destroy->next) {
        if (obj_destroy_check_condition(o, destroy)) {
            switch (destroy->type) {
                default:
                case DESTROY_TYPE_WHITE_PUFF: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 soundBits = (s32) destroy->args[1];
                    obj_destroy_white_puff(o, numCoins, soundBits);
                } break;

                case DESTROY_TYPE_WHITE_PUFF_AND_CHILDREN: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 soundBits = (s32) destroy->args[1];
                    const BehaviorScript *childBehavior = (const BehaviorScript *) destroy->args[2];
                    obj_destroy_white_puff_and_children(o, numCoins, soundBits, childBehavior);
                } break;

                case DESTROY_TYPE_BREAK_PARTICLES: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 soundBits = (s32) destroy->args[1];
                    u8 r = (u8) destroy->args[2];
                    u8 g = (u8) destroy->args[3];
                    u8 b = (u8) destroy->args[4];
                    obj_destroy_break_particles(o, numCoins, soundBits, r, g, b);
                } break;

                case DESTROY_TYPE_TRIANGLE_PARTICLES: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 soundBits = (s32) destroy->args[1];
                    s32 triCount = (s32) destroy->args[2];
                    s32 triModel = (s32) destroy->args[3];
                    f32 triSize = (f32) destroy->args[4] / 100.f;
                    s32 triType = (s32) destroy->args[5];
                    obj_destroy_triangle_particles(o, numCoins, soundBits, triCount, triModel, triSize, triType);
                } break;

                case DESTROY_TYPE_SNOW_PARTICLES: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 soundBits = (s32) destroy->args[1];
                    obj_destroy_snow_particles(o, numCoins, soundBits);
                } break;

                case DESTROY_TYPE_EXPLOSION: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    obj_destroy_explosion(o, numCoins);
                } break;

                case DESTROY_TYPE_EXPLOSION_AND_RESPAWN: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 model = (s32) destroy->args[1];
                    f32 minSpawnDist = (f32) destroy->args[2];
                    obj_destroy_explosion_and_respawn(o, numCoins, model, minSpawnDist);
                } break;

                case DESTROY_TYPE_WATER_BOMB: {
                    s32 soundBits = (s32) destroy->args[0];
                    obj_destroy_water_bomb(o, soundBits);
                } break;

                case DESTROY_TYPE_WHOMP_KING: {
                    obj_destroy_whomp_king(o);
                } break;

                case DESTROY_TYPE_BIG_SNOWBALL: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 soundBits = (s32) destroy->args[1];
                    obj_destroy_big_snowball(o, numCoins, soundBits);
                } break;

                case DESTROY_TYPE_BOO: {
                    s32 soundBits = (s32) destroy->args[0];
                    obj_destroy_boo(o, soundBits);
                } break;

                case DESTROY_TYPE_BIG_MR_I: {
                    s32 soundBits = (s32) destroy->args[0];
                    obj_destroy_big_mr_i(o, soundBits);
                } break;

                case DESTROY_TYPE_BULLY: {
                    s32 actDeath = (s32) destroy->args[0];
                    obj_destroy_bully(o, actDeath);
                } break;

                case DESTROY_TYPE_POKEY: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 soundBits = (s32) destroy->args[1];
                    obj_destroy_pokey(o, numCoins, soundBits);
                } break;

                case DESTROY_TYPE_EYEROK_HAND: {
                    s32 soundBits = (s32) destroy->args[0];
                    u8 r = (u8) destroy->args[1];
                    u8 g = (u8) destroy->args[2];
                    u8 b = (u8) destroy->args[3];
                    obj_destroy_eyerok_hand(o, soundBits, r, g, b);
                } break;

                case DESTROY_TYPE_UKIKI_CAGE: {
                    obj_destroy_ukiki_cage(o);
                } break;

                case DESTROY_TYPE_FIRE_PIRANHA_PLANT: {
                    s32 numCoins = obj_destroy_get_num_coins(o, destroy->args[0]);
                    s32 soundBits = (s32) destroy->args[1];
                    obj_destroy_fire_piranha_plant(o, numCoins, soundBits);
                } break;

                case DESTROY_TYPE_WOODEN_POST: {
                    s32 numCoins = (o->oBehParams & WOODEN_POST_BP_NO_COINS_MASK) ? 0 : 5;
                    s32 soundBits = (s32) destroy->args[1];
                    obj_destroy_wooden_post(o, numCoins, soundBits);
                } break;
            }
            o->oFlags |= OBJ_FLAG_DESTROYED;
            return;
        }
    }

    // Default (white puff with death sound)
    obj_destroy_white_puff(o, o->oNumLootCoins, o->oDeathSound != NO_SOUND ? o->oDeathSound : (s32) SOUND_OBJ_DEFAULT_DEATH);
    o->oFlags |= OBJ_FLAG_DESTROYED;
}

void obj_create_respawner(struct Object *o, s32 model, const BehaviorScript *behavior, f32 minSpawnDist) {
    struct Object *respawner = spawn_object(o, MODEL_NONE, bhvRespawner);
    respawner->oPosX = o->oHomeX;
    respawner->oPosY = o->oHomeY;
    respawner->oPosZ = o->oHomeZ;
    respawner->oBehParams = o->oBehParams;
    respawner->oRespawnerModelToRespawn = model;
    respawner->oRespawnerMinSpawnDist = minSpawnDist;
    respawner->oRespawnerBehaviorToRespawn = behavior;
    if (o->behavior == bhvBulletBill) {
        respawner->oMoveAngleYaw = o->oBulletBillInitialMoveYaw;
    }
}

void obj_update_gfx(struct Object *o) {
    o->oGfxPos[0] = o->oPosX;
    o->oGfxPos[1] = o->oPosY + o->oGraphYOffset;
    o->oGfxPos[2] = o->oPosZ;
    o->oGfxAngle[0] = o->oFaceAnglePitch & 0xFFFF;
    o->oGfxAngle[1] = o->oFaceAngleYaw   & 0xFFFF;
    o->oGfxAngle[2] = o->oFaceAngleRoll  & 0xFFFF;
}

static void release_mario(struct MarioState *m) {
    if (m->action == ACT_OMM_GRABBED_WATER) {
        omm_mario_set_action(m, ACT_WATER_IDLE, 0, 0);
    } else if (m->action == ACT_GRABBED) {
        omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
    }
    m->invincTimer = 1; // Prevent grabbing loop
    m->interactObj = NULL;
    m->usedObj = NULL;
}

void obj_update_held_mario(struct Object *o, f32 forwardVel, f32 yVel) {
    struct MarioState *m = gMarioState;
    struct Object *p = o->parentObj;
    bool isHoldingMario = omm_mario_is_grabbed(m) && (m->usedObj == p || m->interactObj == p);

    // Check parent object
    if (!p->activeFlags) {
        if (isHoldingMario) {
            release_mario(m);
        }
        obj_mark_for_deletion(o);
        return;
    }

    // Check anchor state
    switch (p->oChuckyaUnk88) {

        // Not grabbing Mario
        case 0: {
            o->oAction = -1;
            o->oMoveAngleYaw = m->faceAngle[1];

            // Release Mario if this anchor is still holding him
            if (isHoldingMario) {
                release_mario(m);
            }
        } break;

        // Grabbing Mario
        case 1: {
            if (o->oAction == -1) {
                o->oAction = p->oAction;
                m->faceAngle[1] = o->oMoveAngleYaw;
            }

            // If Mario is no longer grabbed or this anchor doesn't hold Mario, reset its state
            // As long as the parent doesn't change its action, don't release Mario
            if (isHoldingMario) {
                if (p->oAction == o->oAction && p->oHeldState == HELD_FREE) {
                    o->oMoveAngleYaw = p->oMoveAngleYaw;
                    obj_set_gfx_pos_at_obj_pos(m->marioObj, o);
                    vec3f_copy(&m->marioObj->oPosX, m->marioObj->oGfxPos);
                    vec3f_copy(m->pos, m->marioObj->oGfxPos);
                } else {
                    release_mario(m);
                    p->oChuckyaUnk88 = 0;
                }
            } else {
                p->oChuckyaUnk88 = 0;
            }
        } break;

        // Throwing Mario
        case 2: {
            if (isHoldingMario) {
                m->marioObj->oInteractStatus |= (INT_STATUS_MARIO_UNK6 | INT_STATUS_MARIO_UNK2);
                m->forwardVel = forwardVel;
                m->vel[1] = yVel;
            }
            p->oChuckyaUnk88 = 0;
        } break;

        // Losing grip on Mario
        case 3: {
            if (isHoldingMario) {
                m->marioObj->oInteractStatus |= (INT_STATUS_MARIO_UNK6 | INT_STATUS_MARIO_UNK2);
                m->forwardVel = 10.f;
                m->vel[1] = 10.f;
            }
            p->oChuckyaUnk88 = 0;
        } break;
    }
}

void obj_set_params(struct Object *o, s32 interactType, s32 damageOrCoinValue, s32 health, s32 numLootCoins, bool setTangible) {
    o->oInteractType = interactType;
    o->oDamageOrCoinValue = damageOrCoinValue;
    o->oHealth = health;
    o->oNumLootCoins = numLootCoins;
    o->oIntangibleTimer = (setTangible ? 0 : -1);
}

void obj_reset_hitbox(struct Object *o, f32 hitboxRadius, f32 hitboxHeight, f32 hurtboxRadius, f32 hurtboxHeight, f32 wallHitboxRadius, f32 hitboxDownOffset) {
    o->hitboxRadius = abs_f(o->oScaleX) * hitboxRadius;
    o->hitboxHeight = abs_f(o->oScaleY) * hitboxHeight;
    o->hurtboxRadius = abs_f(o->oScaleX) * hurtboxRadius;
    o->hurtboxHeight = abs_f(o->oScaleY) * hurtboxHeight;
    o->oWallHitboxRadius = abs_f(o->oScaleX) * wallHitboxRadius;
    o->hitboxDownOffset = abs_f(o->oScaleY) * hitboxDownOffset;
    o->oInteractStatus = 0;
}

bool obj_fix_hitbox(struct Object *o) {
    if (OMM_MOVESET_ODYSSEY) {
        const OmmBhvDataHitbox *hitbox = omm_behavior_data_get_hitbox(o->behavior);
        if (hitbox) {
            o->hitboxRadius = hitbox->hitboxRadius * (hitbox->scale ? abs_f(o->oScaleX) : 1.f);
            o->hitboxHeight = hitbox->hitboxHeight * (hitbox->scale ? abs_f(o->oScaleY) : 1.f);
            o->hurtboxRadius = hitbox->hurtboxRadius * (hitbox->scale ? abs_f(o->oScaleX) : 1.f);
            o->hurtboxHeight = hitbox->hurtboxHeight * (hitbox->scale ? abs_f(o->oScaleY) : 1.f);
            o->hitboxDownOffset = hitbox->hitboxDownOffset * (hitbox->scale ? abs_f(o->oScaleY) : 1.f);
            return true;
        }
    }
    return false;
}

bool obj_path_check_failsafe(struct Object *o) {
    const Trajectory *traj = (const Trajectory *) o->oPathedPrevWaypoint;
    if (traj && traj[0] != TRAJECTORY_END() && traj[4] != TRAJECTORY_END()) {
        Vec3f p0 = { traj[1], traj[2], traj[3] };
        Vec3f p1 = { traj[5], traj[6], traj[7] };
        Vec3f p; f32 t;
        vec3f_get_projected_point_on_line(p, &t, &o->oPosX, p0, p1);
        if (o->oPosY < p[1] - 500) {
            vec3f_interpolate(&o->oPosX, p0, p1, clamp_0_1_f(t));
            o->oPosY = find_floor_height(o->oPosX, o->oPosY + 300, o->oPosZ);
            return true;
        }
    }
    return false;
}

void obj_drop_to_floor(struct Object *o) {
    o->oFloorHeight = find_floor(o->oPosX, o->oPosY + 20.f, o->oPosZ, &o->oFloor);
    o->oPosY = o->oFloorHeight;
    o->oMoveFlags |= OBJ_MOVE_ON_GROUND;
    o->oMoveFlags &= ~(OBJ_MOVE_LANDED | OBJ_MOVE_LEFT_GROUND | OBJ_MOVE_ENTERED_WATER | OBJ_MOVE_AT_WATER_SURFACE | OBJ_MOVE_UNDERWATER_OFF_GROUND | OBJ_MOVE_UNDERWATER_ON_GROUND | OBJ_MOVE_IN_AIR | OBJ_MOVE_LEAVING_WATER);
}

bool obj_is_always_rendered(struct Object *o) {
    return
        !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE) &&
         (o->oNodeFlags & GRAPH_RENDER_ACTIVE) &&
         (o->oNodeFlags & GRAPH_RENDER_ALWAYS);
}

void obj_set_always_rendered(struct Object *o, bool set) {
    if (set) {
        o->oNodeFlags |= GRAPH_RENDER_ALWAYS;
    } else {
        o->oNodeFlags &= ~GRAPH_RENDER_ALWAYS;
    }
}

bool obj_is_dormant(struct Object *o) {
    if (o && (o->activeFlags & ACTIVE_FLAG_DORMANT)) {
        o->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
        o->oInteractStatus = INT_STATUS_INTERACTED;
        return true;
    }
    return false;
}

void obj_set_dormant(struct Object *o, bool set) {
    if (set) {
        o->activeFlags |= ACTIVE_FLAG_DORMANT;
        o->oNodeFlags &= ~GRAPH_RENDER_ACTIVE;
        o->oInteractStatus = INT_STATUS_INTERACTED;
        o->oIntangibleTimer = -1;
    } else if (o->activeFlags & ACTIVE_FLAG_DORMANT) {
        o->activeFlags &= ~ACTIVE_FLAG_DORMANT;
        o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
        o->oInteractStatus = 0;
        o->oIntangibleTimer = 0;
    }
}

struct Waypoint *obj_path_get_nearest_waypoint(struct Object *o, struct Waypoint *list, f32 radiusMax) {
    struct Waypoint *nearest = NULL;
    f32 nearestDist;
    for (; list->flags != WAYPOINT_FLAGS_END; list++) {
        f32 dist = sqrtf(
            sqr_f(list->pos[0] - o->oPosX) +
            sqr_f(list->pos[1] - o->oPosY) +
            sqr_f(list->pos[2] - o->oPosZ)
        );
        if ((radiusMax <= 0.f || dist < radiusMax) && (!nearest || dist < nearestDist)) {
            nearest = list;
            nearestDist = dist;
        }
    }
    return nearest;
}

struct Object *find_unimportant_object() {
    for_each_object_in_unimportant_lists(obj) {
        if (obj != gCurrentObject && obj->activeFlags && (*_list_ == OBJ_LIST_UNIMPORTANT || omm_obj_is_unimportant(obj))) {
            return obj;
        }
    }
    return NULL;
}

//
// Red coin star
//

struct Object *obj_get_red_coin_star() {

    // Hidden red coin star
    struct Object *hiddenRedCoinStar = obj_get_first_with_behavior(bhvHiddenRedCoinStar);
    if (hiddenRedCoinStar && hiddenRedCoinStar->oAction <= 1) {
        return hiddenRedCoinStar;
    }

    // Bowser red coin star
    struct Object *bowserRedCoinStar = obj_get_first_with_behavior(bhvBowserCourseRedCoinStar);
    if (bowserRedCoinStar && bowserRedCoinStar->oAction <= 1) {
        return bowserRedCoinStar;
    }

    // Actual red coin star
    for_each_in_(const BehaviorScript *, bhv, { bhvStar, bhvStarSpawnCoordinates }) {
        for_each_object_with_behavior(star, *bhv) {
            if ((star->oBehParams >> 24) == gOmmArea->redCoinStarIndex) {
                return star;
            }
        }
    }

    // No red coin star
    return NULL;
}

//
// Secret star
//

struct Object *obj_get_secret_star() {

    // Hidden secret star
    struct Object *hiddenStar = obj_get_first_with_behavior(bhvHiddenStar);
    if (hiddenStar && hiddenStar->oAction <= 1) {
        return hiddenStar;
    }

    // Actual secret star
    for_each_in_(const BehaviorScript *, bhv, { bhvStar, bhvStarSpawnCoordinates }) {
        for_each_object_with_behavior(star, *bhv) {
            if ((star->oBehParams >> 24) == gOmmArea->secretStarIndex) {
                return star;
            }
        }
    }

    // No secret star
    return NULL;
}

//
// Dialog
//

s16 sObjDialogId = -1;
s32 sObjDialogState = 0;

bool obj_dialog_start(s16 dialogId) {
    if (sObjDialogState != 0 || dialogId < 0) {
        return false;
    }
    sObjDialogId = dialogId;
    sObjDialogState = 1;
    return true;
}

// Returns true if the dialog is finished
bool obj_dialog_update() {
    switch (sObjDialogState) {
        case 0: {
            return true;
        } break;

        case 1: {
            create_dialog_box(sObjDialogId);
            sObjDialogState = 2;
        } break;

        case 2: {
            if (get_dialog_id() < 0) {
                sObjDialogState = 3;
            }
        } break;

        case 3: {
            sObjDialogId = -1;
            sObjDialogState = 0;
        } break;
    }
    return false;
}

OMM_ROUTINE_LEVEL_ENTRY(obj_reset_dialog_state) {
    sObjDialogId = -1;
    sObjDialogState = 0;
}

//
// Cutscene
//

u8 sObjCutsceneId = 0;
s32 sObjCutsceneState = 0;
struct Object *sObjCutsceneObj = NULL;

bool obj_cutscene_start(u8 cutsceneId, struct Object *o) {
    if (sObjCutsceneState || !cutsceneId || !o) {
        return false;
    }
    sObjCutsceneId = cutsceneId;
    sObjCutsceneObj = o;
    sObjCutsceneState = 1;
    return true;
}

// Returns true if the cutscene is finished
bool obj_cutscene_update() {
    switch (sObjCutsceneState) {
        case 0: {
            return true;
        } break;

        case 1: {
            if (cutscene_object(sObjCutsceneId, sObjCutsceneObj) == 1) {
                sObjCutsceneState = 2;
            } else {
                sObjCutsceneState = 3;
            }
        } break;

        case 2: {
            if (cutscene_object(sObjCutsceneId, sObjCutsceneObj) == -1) {
                sObjCutsceneState = 3;
            }
        } break;

        case 3: {
            sObjCutsceneId = 0;
            sObjCutsceneObj = NULL;
            sObjCutsceneState = 0;
        } break;
    }
    return false;
}

OMM_ROUTINE_LEVEL_ENTRY(obj_reset_cutscene_state) {
    sObjCutsceneId = 0;
    sObjCutsceneState = 0;
    sObjCutsceneObj = NULL;
}
