#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

f32 obj_get_horizontal_distance(struct Object *o1, struct Object *o2) {
    return sqrtf(sqr_f(o1->oPosX - o2->oPosX) + sqr_f(o1->oPosZ - o2->oPosZ));
}

f32 obj_get_distance(struct Object *o1, struct Object *o2) {
    return sqrtf(sqr_f(o1->oPosX - o2->oPosX) + sqr_f(o1->oPosY - o2->oPosY) + sqr_f(o1->oPosZ - o2->oPosZ));
}

bool obj_check_model(struct Object *o, s32 modelId) {
    if (!o || !o->oGraphNode) {
        return false;
    }
    struct Object *p = gCurrentObject;
    gCurrentObject = o;
    bool res = cur_obj_has_model(modelId);
    gCurrentObject = p;
    return res;
}

bool obj_has_graph_node(struct Object *o, struct GraphNode *node) {
    return
#if OMM_CODE_DYNOS
           (o->oGraphNode && node && o->oGraphNode->georef == node->georef) ||
#endif
           (o->oGraphNode == node);
}

bool obj_is_surface(struct Object *o) {
    s32 type = obj_get_list_index(o);
    return (type == OBJ_LIST_SURFACE) || (type == OBJ_LIST_POLELIKE);
}

bool obj_is_on_ground(struct Object *o) {
    return o->oFloor && (o->oDistToFloor <= 0.f);
}

// Goomba has to be handled separately due to the Goomba stack
// Return true only if the bottom Goomba is completely submerged
bool obj_is_underwater(struct Object *o, f32 waterLevel) {
    if (omm_obj_is_goomba(o)) {
        if (gMarioState->action == ACT_OMM_POSSESSION && gOmmCapture == o) {
            return (o->oPosY + (omm_capture_get_hitbox_height(o)) - (omm_capture_get_hitbox_down_offset(o))) < waterLevel;        
        }
        return (o->oPosY + o->hitboxHeight - o->hitboxDownOffset) < waterLevel;
    }
    return (o->oPosY + (o->hitboxHeight / 2) - o->hitboxDownOffset) < waterLevel;
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
    s32 yawDiff = (s32) ((s16) ((s32) (yawObj1ToObj2) - (s32) (o1->oFaceAngleYaw)));
    return (-angleRange <= yawDiff) && (yawDiff <= angleRange);
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

bool obj_is_object2_pushing_object1_backwards(struct Object *o1, struct Object *o2, bool setObj1FaceYaw) {
    s16 yawObj1toObj2 = obj_get_object1_angle_yaw_to_object2(o1, o2);
    s32 yawDiff = (s32) ((s16) ((s32) (yawObj1toObj2) - (s32) (o1->oFaceAngleYaw)));
    if (setObj1FaceYaw) o1->oFaceAngleYaw = yawObj1toObj2;
    return (-0x4000 <= yawDiff) && (yawDiff <= +0x4000);
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

bool obj_check_hitbox(struct Object *o, u32 flags) {
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

    // Radius check
    f32 r2 = sqr_f(box1.radius + box2.radius);
    f32 d2 = sqr_f(o1->oPosX - o2->oPosX) + sqr_f(o1->oPosZ - o2->oPosZ);
    if (d2 > r2) {
        return false;
    }

    // Height check
    f32 o1lb = o1->oPosY - o1->hitboxDownOffset;
    f32 o1ub = box1.height + o1lb;
    f32 o2lb = o2->oPosY - o2->hitboxDownOffset;
    f32 o2ub = box2.height + o2lb;
    f32 osoh = box1.height + box2.height;
    if (((o2ub - o1lb) > osoh) ||
        ((o1ub - o2lb) > osoh)) {
        return false;
    }

    return true;
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

void obj_safe_step(struct Object *o, s32 update) {
    static Vec3f sLastValidPos[OBJECT_POOL_CAPACITY];
    s32 objIndex = (s32) (o - gObjectPool);
    o->oFloorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &o->oFloor);
    if (update) {
        if (o->oFloorHeight - sLastValidPos[objIndex][1] < -100.f) {
            o->oPosX = sLastValidPos[objIndex][0];
            o->oPosZ = sLastValidPos[objIndex][2];
            o->oFloorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &o->oFloor);
            if (o->oPosY < o->oFloorHeight) {
                o->oPosY = o->oFloorHeight;
                o->oVelY *= o->oBounciness;
                if (o->oVelY > 5.f) {
                    o->oMoveFlags |= OBJ_MOVE_BOUNCE;
                } else {
                    o->oVelY = 0;
                    o->oMoveFlags |= OBJ_MOVE_ON_GROUND;
                }
            }
        }
    } else {
        sLastValidPos[objIndex][0] = o->oPosX;
        sLastValidPos[objIndex][1] = o->oFloorHeight;
        sLastValidPos[objIndex][2] = o->oPosZ;
    }
    if (o->oVelY <= 0.f && (o->oNodeFlags & GRAPH_RENDER_ACTIVE) && !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE)) {
        o->oIntangibleTimer = 0;
    }
}

void obj_update_blink_state(struct Object *o, s32 *timer, s16 base, s16 range, s16 length) {
    if (*timer != 0) {
        (*timer)--;
    } else {
        *timer = base + (s16)(range * random_float());
    }

    if (*timer > length) {
        o->oAnimState = 0;
    } else {
        o->oAnimState = 1;
    }
}

void obj_random_blink(struct Object *o, s32 *timer) {
    if (*timer == 0) {
        if ((s16)(random_float() * 100.0f) == 0) {
            o->oAnimState = 1;
            *timer = 1;
        }
    } else {
        (*timer)++;
        if (*timer >= 6) {
            o->oAnimState = 0;
        }
        if (*timer >= 11) {
            o->oAnimState = 1;
        }
        if (*timer >= 16) {
            o->oAnimState = 0;
            *timer = 0;
        }
    }
}

void obj_make_step_sound_and_particle(struct Object *o, f32 *dist, f32 distMin, f32 distInc, s32 soundBits, u32 particles) {
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
                drop->oVelY = random_float() * 30.0f;
                obj_translate_xz_random(drop, 110.0f);
            }
        }
        if (particles & OBJ_PARTICLE_FIRE) {
            omm_spawn_fire_smoke(o, 0);
        }
        if (particles & OBJ_PARTICLE_FLAME) {
            for (s32 i = 0; i != 2; ++i) {
                spawn_object(o, MODEL_RED_FLAME, bhvKoopaShellFlame);
            }
        }

        // Sound effect
        obj_play_sound(o, soundBits);
    }
}

void obj_play_sound(struct Object *o, s32 soundBits) {
    if (soundBits) {
        if (o) {
            play_sound(soundBits, o->oCameraToObject);
        } else {
            play_sound(soundBits, gGlobalSoundArgs);
        }
    }
}

s32 obj_get_model_id(struct Object *o) {
    for (s32 i = 0; i != 0x100; ++i) {
        if (obj_check_model(o, i)) {
            return i;
        }
    }
    return -1;
}

s32 obj_get_coin_type(struct Object *o) {

    // Yellow coin
    if (obj_check_model(o, MODEL_YELLOW_COIN) || obj_check_model(o, MODEL_YELLOW_COIN_NO_SHADOW)) {
        return 0;
    }

    // Red coin
    if (obj_check_model(o, MODEL_RED_COIN) || obj_check_model(o, MODEL_RED_COIN_NO_SHADOW)) {
        return 1;
    }

    // Blue coin
    if (obj_check_model(o, MODEL_BLUE_COIN) || obj_check_model(o, MODEL_BLUE_COIN_NO_SHADOW)) {
        return 2;
    }

    // Not a coin
    return -1;
}

void obj_spawn_star(struct Object *o, f32 xFrom, f32 yFrom, f32 zFrom, f32 xTo, f32 yTo, f32 zTo, s32 starIndex, bool noExit) {
    struct Object *star = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStarSpawnCoordinates, xFrom, yFrom, zFrom, 0, 0, 0);
    star->oBehParams = ((starIndex & 0x1F) << 24);
    star->oHomeX = xTo;
    star->oHomeY = yTo;
    star->oHomeZ = zTo;
    star->oFaceAnglePitch = 0;
    star->oFaceAngleRoll = 0;
    if (noExit) star->oInteractionSubtype |= INT_SUBTYPE_NO_EXIT;
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
    if (soundBits != -1) {
        struct Object *sound = spawn_object(gMarioObject, 0, bhvSoundSpawner);
        if (sound) {
            sound->oPosX = x;
            sound->oPosY = y;
            sound->oPosZ = z;
            if (!soundBits) {
                sound->oSoundEffectUnkF4 = SOUND_OBJ_DEFAULT_DEATH;
            } else {
                sound->oSoundEffectUnkF4 = soundBits;
            }
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
        omm_spawn_break_particle(o, r, g, b, offsetY, velMin, velMax, velY, scaleMin, scaleMax);
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

static void spawn_coins(struct Object *o, s32 numCoins) {
    o->oNumLootCoins = numCoins;
    if (o->oNumLootCoins < 0) {
        spawn_object(o, MODEL_BLUE_COIN, bhvMrIBlueCoin);
    } else {
        obj_spawn_loot_yellow_coins(o, o->oNumLootCoins, 20.f);
    }
}

static void obj_destroy_white_puff(struct Object *o, s32 numCoins, s32 soundBits) {
    spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    obj_mark_for_deletion(o);
}

static void obj_destroy_piranha_plant(struct Object *o, s32 soundBits) {
    for_each_object_with_behavior(obj, bhvPiranhaPlantBubble) {
        if (obj->parentObj == o) {
            obj_mark_for_deletion(obj);
        }
    }
    if (o->oNumLootCoins) {
        spawn_coins(o, -1);
    }
    obj_spawn_white_puff(o, soundBits);
    obj_mark_for_deletion(o);
}

static void obj_destroy_water_bomb(struct Object *o, s32 soundBits) {
    for_each_object_with_behavior(obj, bhvWaterBombShadow) {
        if (obj->parentObj == o) {
            obj_mark_for_deletion(obj);
        }
    }
    o->parentObj->oWaterBombSpawnerBombActive = FALSE;
    obj_play_sound(o, soundBits);
    obj_spawn_particles(o, 5, MODEL_BUBBLE, 25, 20, 60, 10, 10, -2, 3.5f, 1.f);
    obj_mark_for_deletion(o);
}

static void obj_destroy_triangle_particles(struct Object *o, s32 numCoins, s32 soundBits, s32 triCount, s32 triModel, f32 triSize, s32 triType) {
    spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    obj_spawn_triangle_break_particles(o, triCount, triModel, triSize, triType);
    obj_mark_for_deletion(o);
}

static void obj_destroy_snow_particles(struct Object *o, s32 numCoins, s32 soundBits) {
    spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    f32 scale = o->oScaleX;
    obj_spawn_particles(o, 8, MODEL_WHITE_PARTICLE, -o->hitboxDownOffset, 4 * scale, 2 * scale, 8 * scale, 4 * scale, -2 * sqrtf(scale), 0.25f * scale, 0.25f * scale);
    obj_mark_for_deletion(o);
}

static void obj_destroy_big_snowball(struct Object *o, s32 numCoins, s32 soundBits) {
    spawn_coins(o, numCoins);
    obj_spawn_white_puff(o, soundBits);
    f32 scale = o->oScaleX;
    obj_spawn_particles(o, 16, MODEL_WHITE_PARTICLE, 200 * scale, 0, 60 * scale, -30 * scale, 90 * scale, -4.f * scale, 3.f * scale, 1.f * scale);
    obj_mark_for_deletion(o);
    obj_create_respawner(o, MODEL_CCM_SNOWMAN_BASE, bhvSnowmansBottom, 2000.f);
}

static void obj_destroy_explosion(struct Object *o, s32 numCoins) {
    spawn_coins(o, numCoins);
    set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
    obj_spawn_white_puff(o, SOUND_GENERAL2_BOBOMB_EXPLOSION);
    struct Object *explosion = spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
    explosion->oGraphYOffset += 100.0f;
    obj_mark_for_deletion(o);
}

static void obj_destroy_bobomb(struct Object *o, s32 numCoins) {
    obj_destroy_explosion(o, numCoins);
    obj_create_respawner(o, MODEL_BLACK_BOBOMB, bhvBobomb, 1500.f);
}

static void obj_destroy_bullet_bill(struct Object *o, s32 numCoins) {
    obj_destroy_explosion(o, numCoins);
    obj_create_respawner(o, MODEL_BULLET_BILL, bhvBulletBill, 1000.f);
}

#if OMM_GAME_IS_SMSR
static void obj_destroy_water_mine(struct Object *o, s32 numCoins) {
    obj_destroy_explosion(o, numCoins);
    obj_create_respawner(o, MODEL_WATER_MINE, bhvCustomSMSRBulletMine, 1000.f);
}
#endif

static void obj_destroy_break_particles(struct Object *o, s32 numCoins, s32 soundBits, u8 r, u8 g, u8 b) {
    spawn_coins(o, numCoins);
    set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
    obj_spawn_white_puff(o, soundBits);
    obj_spawn_break_particles(o, 24, r, g, b, 150.f, 0, 40, 32, 1.f, 2.f);
    obj_mark_for_deletion(o);
}

static void obj_destroy_spawn_star(struct Object *o, s32 soundBits, f32 xFrom, f32 yFrom, f32 zFrom, f32 xTo, f32 yTo, f32 zTo, s32 starIndex, bool noExit) {
    spawn_coins(o, 0);
    obj_spawn_white_puff(o, soundBits);
    obj_spawn_star(o, xFrom, yFrom, zFrom, xTo, yTo, zTo, starIndex, noExit);
    obj_mark_for_deletion(o);
}

static void obj_destroy_whomp_king(struct Object *o, UNUSED s32 unused) {
    play_sound(SOUND_OBJ2_WHOMP_SOUND_SHORT, o->oCameraToObject);
    play_sound(SOUND_OBJ_KING_WHOMP_DEATH, o->oCameraToObject);
    o->oHealth = 0;
    o->oAction = 8;
}

static void obj_destroy_eyerock_hand(struct Object *o, s32 soundBits, u8 r, u8 g, u8 b) {
    if (o->parentObj->oAction >= EYEROK_BOSS_ACT_FIGHT) {
        o->parentObj->oEyerokBossNumHands--;
        set_camera_shake_from_point(SHAKE_POS_SMALL, o->oPosX, o->oPosY, o->oPosZ);
        obj_spawn_white_puff(o, soundBits);
        obj_spawn_break_particles(o, 24, r, g, b, 150.f, 0, 40, 32, 1.f, 2.f);
        obj_mark_for_deletion(o);
    }
}

#define destroy_preset(bhv, cond, func, ...)    \
    if ((o->behavior == bhv) && (cond)) {       \
        func(o, __VA_ARGS__);                   \
        return;                                 \
    }

void obj_destroy(struct Object *o) {

    // Presets
    destroy_preset(bhvGoomba, o->oGoombaSize == GOOMBA_SIZE_TINY, obj_destroy_white_puff, 1, SOUND_OBJ_ENEMY_DEATH_HIGH);
    destroy_preset(bhvGoomba, o->oGoombaSize == GOOMBA_SIZE_REGULAR, obj_destroy_white_puff, 1, SOUND_OBJ_ENEMY_DEATH_HIGH);
    destroy_preset(bhvGoomba, o->oGoombaSize == GOOMBA_SIZE_HUGE, obj_destroy_white_puff, -1, SOUND_OBJ_ENEMY_DEATH_LOW);
    destroy_preset(bhvBobomb, 1, obj_destroy_bobomb, 0);
    destroy_preset(bhvWaterBomb, 1, obj_destroy_water_bomb, SOUND_OBJ_DIVING_IN_WATER);
    destroy_preset(bhvChuckya, 1, obj_destroy_white_puff, 5, SOUND_OBJ_CHUCKYA_DEATH);
    destroy_preset(bhvBowlingBall, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x40, 0x40, 0x40);
    destroy_preset(bhvPitBowlingBall, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x40, 0x40, 0x40);
    destroy_preset(bhvFreeBowlingBall, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x40, 0x40, 0x40);
    destroy_preset(bhvMadPiano, 1, obj_destroy_break_particles, 5, SOUND_OBJ_MAD_PIANO_CHOMPING, 0x40, 0x40, 0x40);
    destroy_preset(bhvBreakableBox, 1, obj_destroy_triangle_particles, o->oNumLootCoins, SOUND_GENERAL_BREAK_BOX, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_30);
    destroy_preset(bhvBreakableBoxSmall, 1, obj_destroy_triangle_particles, 3, SOUND_GENERAL_BREAK_BOX, OBJ_SPAWN_TRI_BREAK_PRESET_DIRT);
    destroy_preset(bhvJumpingBox, 1, obj_destroy_triangle_particles, 5, SOUND_GENERAL_BREAK_BOX, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_20);
    destroy_preset(bhvMrBlizzard, 1, obj_destroy_snow_particles, 3, SOUND_OBJ_DEFAULT_DEATH);
    destroy_preset(bhvSnowmansBottom, 1, obj_destroy_big_snowball, 0, SOUND_OBJ_DEFAULT_DEATH);
    destroy_preset(bhvBulletBill, 1, obj_destroy_bullet_bill, 0);
    destroy_preset(bhvHauntedChair, 1, obj_destroy_white_puff, 0, SOUND_OBJ_DEFAULT_DEATH);
    destroy_preset(bhvJrbSlidingBox, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x50, 0x40, 0x00);
    destroy_preset(bhvMrI, o->oBehParams2ndByte, obj_destroy_spawn_star, SOUND_OBJ_MRI_DEATH, o->oPosX, o->oPosY + 100.f, o->oPosZ, 1370, 2000, -320, (o->oBehParams >> 24), false);
    destroy_preset(bhvMrI, 1, obj_destroy_white_puff, -1, SOUND_OBJ_MRI_DEATH);
    destroy_preset(bhvSpiny, 1, obj_destroy_white_puff, 0, SOUND_OBJ_DEFAULT_DEATH);
    destroy_preset(bhvSmallWhomp, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0xE0, 0xE0, 0xE0);
    destroy_preset(bhvWhompKingBoss, 1, obj_destroy_whomp_king, 0);
    destroy_preset(bhvThwomp, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0x40, 0x80, 0xFF);
    destroy_preset(bhvThwomp2, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0x40, 0x80, 0xFF);
    destroy_preset(bhvTowerDoor, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0xFF, 0xFF, 0xFF);
    destroy_preset(bhvWfBreakableWallLeft, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x80, 0x60, 0x40);
    destroy_preset(bhvWfBreakableWallRight, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x80, 0x60, 0x40);
    destroy_preset(bhvLllVolcanoFallingTrap, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0xC0, 0xC0, 0xC0);
    destroy_preset(bhvToxBox, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0x80, 0x10, 0x00);
    destroy_preset(bhvGrindel, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0xFF, 0xFF, 0xA0);
    destroy_preset(bhvHorizontalGrindel, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0xFF, 0xFF, 0xA0);
    destroy_preset(bhvSpindel, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0xFF, 0xFF, 0xA0);
    destroy_preset(bhvEyerokHand, 1, obj_destroy_eyerock_hand, SOUND_OBJ2_EYEROK_SOUND_SHORT, 0xC8, 0x90, 0x30);
    destroy_preset(bhvBigBoulder, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0xB0, 0xA0, 0x70);
    destroy_preset(bhvHeaveHo, 1, obj_destroy_white_puff, -1, SOUND_OBJ_DEFAULT_DEATH);
    destroy_preset(bhvPiranhaPlant, 1, obj_destroy_piranha_plant, SOUND_OBJ_DEFAULT_DEATH);
#if OMM_GAME_IS_SMMS
    destroy_preset(bhvgoombone, o->oGoombaSize == GOOMBA_SIZE_TINY, obj_destroy_white_puff, 1, SOUND_OBJ_ENEMY_DEATH_HIGH);
    destroy_preset(bhvgoombone, o->oGoombaSize == GOOMBA_SIZE_REGULAR, obj_destroy_white_puff, 1, SOUND_OBJ_ENEMY_DEATH_HIGH);
    destroy_preset(bhvgoombone, o->oGoombaSize == GOOMBA_SIZE_HUGE, obj_destroy_white_puff, -1, SOUND_OBJ_ENEMY_DEATH_LOW);
#endif
#if OMM_GAME_IS_SMSR
    destroy_preset(bhvCustomSMSRShyGuy, o->oGoombaSize == GOOMBA_SIZE_TINY, obj_destroy_white_puff, 1, SOUND_OBJ_ENEMY_DEATH_HIGH);
    destroy_preset(bhvCustomSMSRShyGuy, o->oGoombaSize == GOOMBA_SIZE_REGULAR, obj_destroy_white_puff, 1, SOUND_OBJ_ENEMY_DEATH_HIGH);
    destroy_preset(bhvCustomSMSRShyGuy, o->oGoombaSize == GOOMBA_SIZE_HUGE, obj_destroy_white_puff, -1, SOUND_OBJ_ENEMY_DEATH_LOW);
    destroy_preset(bhvCustomSMSRBreakableWindow, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0xFF, 0xFF, 0xFF);
    destroy_preset(bhvCustomSMSRFallingDomino, 1, obj_destroy_break_particles, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x40, 0x40, 0x40);
    destroy_preset(bhvCustomSMSRFloatingThwomp, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0x40, 0x80, 0xFF);
    destroy_preset(bhvCustomSMSRAttractedSpaceBox, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0x80, 0x10, 0x00);
    destroy_preset(bhvCustomSMSRSpaceBox, 1, obj_destroy_break_particles, 5, SOUND_OBJ_THWOMP, 0x80, 0x10, 0x00);
    destroy_preset(bhvCustomSMSRBulletMine, 1, obj_destroy_water_mine, 0);
#endif

    // Default (white puff)
    if (o->oDeathSound) {
        obj_destroy_white_puff(o, o->oNumLootCoins, o->oDeathSound);
    } else {
        obj_destroy_white_puff(o, o->oNumLootCoins, SOUND_OBJ_DEFAULT_DEATH);
    }
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

void obj_set_params(struct Object *o, s32 interactType, s32 damageOrCoinValue, s32 health, s32 numLootCoins, bool setTangible) {
    o->oInteractType = interactType;
    o->oDamageOrCoinValue = damageOrCoinValue;
    o->oHealth = health;
    o->oNumLootCoins = numLootCoins;
    o->oIntangibleTimer = (setTangible ? 0 : -1);
}

void obj_reset_hitbox(struct Object *o, f32 hitboxRadius, f32 hitboxHeight, f32 hurtboxRadius, f32 hurtboxHeight, f32 wallHitboxRadius, f32 hitboxDownOffset) {
    o->hitboxRadius = o->oScaleX * hitboxRadius;
    o->hitboxHeight = o->oScaleY * hitboxHeight;
    o->hurtboxRadius = o->oScaleX * hurtboxRadius;
    o->hurtboxHeight = o->oScaleY * hurtboxHeight;
    o->oWallHitboxRadius = o->oScaleX * wallHitboxRadius;
    o->hitboxDownOffset = o->oScaleY * hitboxDownOffset;
    o->oInteractStatus = 0;
    o->oInteractionSubtype = 0;
}

static OmmArray sShadowObjects = omm_array_zero;
void obj_set_shadow_pos_to_object_pos(struct Object *o) {
    if (omm_array_find(sShadowObjects, ptr, o) == -1) {
        omm_array_add(sShadowObjects, ptr, o);
    }
}

// Called only once, in rendering_graph_node.c
void obj_fix_shadow_pos(struct GraphNodeObject *gfx, Vec3f shadowPos) {
    omm_array_for_each(sShadowObjects, p) {
        struct Object *obj = (struct Object *) p->as_ptr;
        if (&obj->header.gfx == gfx) {
            shadowPos[0] = obj->oPosX;
            shadowPos[1] = obj->oPosY;
            shadowPos[2] = obj->oPosZ;
            omm_array_remove(sShadowObjects, i_p);
            return;
        }
    }
}

void obj_drop_to_floor(struct Object *o) {
    o->oFloorHeight = find_floor(o->oPosX, o->oPosY + 20.f, o->oPosZ, &o->oFloor);
    o->oPosY = o->oFloorHeight;
    o->oMoveFlags |= OBJ_MOVE_ON_GROUND;
    o->oMoveFlags &= ~(OBJ_MOVE_LANDED | OBJ_MOVE_LEFT_GROUND | OBJ_MOVE_ENTERED_WATER | OBJ_MOVE_AT_WATER_SURFACE | OBJ_MOVE_UNDERWATER_OFF_GROUND | OBJ_MOVE_UNDERWATER_ON_GROUND | OBJ_MOVE_IN_AIR | OBJ_MOVE_LEAVING_WATER);
}

void obj_load_collision_model(struct Object *o) {
    if (o) {
        struct Object *p = gCurrentObject;
        gCurrentObject = o;
        load_object_collision_model();
        gCurrentObject = p;
    }
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

bool obj_is_knocked_back(struct Object *o) {
    return (o && (o->activeFlags & ACTIVE_FLAG_KNOCKED_BACK));
}

bool obj_set_knockback(struct Object *o, struct Object *from, s32 knockbackType, f32 fvel, f32 yvel) {
    if (knockbackType >= 0) {
        if (from && !omm_obj_is_goomba_stack(o)) {
            s16 angle = obj_get_object1_angle_yaw_to_object2(from, o);
            o->oVelX = fvel * sins(angle);
            o->oVelY = yvel;
            o->oVelZ = fvel * coss(angle);
            o->oForwardVel = fvel;
            o->oAction |= ((knockbackType + 1) << 16);
            o->activeFlags |= ACTIVE_FLAG_KNOCKED_BACK;
            play_sound(SOUND_ACTION_BOUNCE_OFF_OBJECT, o->oCameraToObject);
            return true;
        }
    } else {
        o->activeFlags &= ~ACTIVE_FLAG_KNOCKED_BACK;
    }
    return false;
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

struct Object *obj_get_red_coin_star() {

    // Hidden red coin star
    struct Object *hiddenRedCoinStar = obj_get_first_with_behavior(bhvHiddenRedCoinStar);
    if (hiddenRedCoinStar) {
        return hiddenRedCoinStar;
    }

    // Bowser red coin star
    struct Object *bowserRedCoinStar = obj_get_first_with_behavior(bhvBowserCourseRedCoinStar);
    if (bowserRedCoinStar) {
        return bowserRedCoinStar;
    }

    // Actual red coin star
    for_each_until_null(const BehaviorScript *, bhv, OMM_ARRAY_OF(const BehaviorScript *) { bhvStar, bhvStarSpawnCoordinates, NULL }) {
        for_each_object_with_behavior(star, *bhv) {
            if (star->activeFlags & ACTIVE_FLAG_RED_COIN_STAR) {
                return star;
            }
        }
    }

    // No red coin star
    return NULL;
}

static struct {
    struct Object *obj;
    Vec3f vel;
    s32 duration;
} sOmmDoor[2];

static void open_door(struct Object *o, struct Object *door1, struct Object *door2, s32 duration) {
    if (o == gOmmCapture) {
        omm_mario_lock(gMarioState, -1);
    }

    if (!door2) { door2 = door1; }
    f32 cX = (door1->oPosX + door2->oPosX) / 2.f;
    f32 cZ = (door1->oPosZ + door2->oPosZ) / 2.f;
    s16 cY = (door1->oMoveAngleYaw);
    f32 cR = (door1->hitboxRadius + door2->hitboxRadius + o->hitboxRadius);
    f32 vX = sins(cY);
    f32 vZ = coss(cY);
    f32 dX = o->oPosX - cX;
    f32 dZ = o->oPosZ - cZ;
    s16 dY = atan2s(dZ, dX);
    s16 dA = min_s((u16) (cY - dY), (u16) (dY - cY));
    f32 dS = (dA < 0x4000 ? -1.f : +1.f);
    Vec2f pA = { cX, cZ };
    Vec2f pB = { cX + vX, cZ + vZ };
    Vec2f pP = { o->oPosX, o->oPosZ };
    Vec2f pQ; vec2f_get_projected_point_on_line(pQ, pA, pB, pP);

    // Init first door
    sOmmDoor[0].obj         = door1;
    sOmmDoor[0].vel[0]      = (pQ[0] - pP[0]) / 20.f;
    sOmmDoor[0].vel[2]      = (pQ[1] - pP[1]) / 20.f;
    sOmmDoor[0].duration    = duration;
    door1->oAction          = 1;
    door1->oInteractStatus  = INT_STATUS_INTERACTED;

    // Init second door
    sOmmDoor[1].obj         = door2;
    sOmmDoor[1].duration    = duration;
    sOmmDoor[1].vel[0]      = (dS * vX * cR * 1.6f) / (duration - 20.f);
    sOmmDoor[1].vel[2]      = (dS * vZ * cR * 1.6f) / (duration - 20.f);
    door2->oAction          = 1;
    door2->oInteractStatus  = INT_STATUS_INTERACTED;
}

void obj_open_door(struct Object *o, struct Object *door) {
    if (obj_is_on_ground(o) && sOmmDoor[0].duration == 0) {

        // Door
        if (door->behavior == bhvDoor && obj_detect_hitbox_overlap(o, door, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {
            open_door(o, door, NULL, 80);
        }

        // Star door
        if (door->behavior == bhvStarDoor && obj_detect_hitbox_overlap(o, door, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX)) {
            open_door(o, door, obj_get_nearest_with_behavior(door, bhvStarDoor), 68);
        }
    }
}

bool obj_update_door(struct Object *o) {
    static s32 sUpdateDoorTimer = 0;
    if (sOmmDoor[0].duration == 0) {
        sUpdateDoorTimer = 0;
        return false;
    }

    // Move 1
    if (sUpdateDoorTimer < 20) {
        o->oVelX = sOmmDoor[0].vel[0];
        o->oVelZ = sOmmDoor[0].vel[2];
    }

    // Move 2
    else if (sUpdateDoorTimer < sOmmDoor[0].duration) {
        o->oVelX = sOmmDoor[1].vel[0];
        o->oVelZ = sOmmDoor[1].vel[2];
    }

    // Close door
    else {
        if (o == gOmmCapture) {
            omm_mario_unlock(gMarioState);
        }
        sOmmDoor[0].obj->oAction = 0;
        sOmmDoor[0].obj->oInteractStatus = 0;
        sOmmDoor[0].duration = 0;
        sOmmDoor[1].obj->oAction = 0;
        sOmmDoor[1].obj->oInteractStatus = 0;
        sOmmDoor[1].duration = 0;
        sUpdateDoorTimer = 0;
        return false;
    }

    // Update
    f32 forwardVel      = sqrtf(sqr_f(o->oVelX) + sqr_f(o->oVelZ));
    s16 faceAngleYaw    = (forwardVel == 0.f ? o->oFaceAngleYaw : atan2s(o->oVelZ, o->oVelX));
    o->oForwardVel      = forwardVel;
    o->oFaceAngleYaw    = faceAngleYaw;
    o->oFaceAnglePitch  = 0;
    o->oFaceAngleRoll   = 0;
    o->oMoveAngleYaw    = faceAngleYaw;
    o->oMoveAnglePitch  = 0;
    o->oMoveAngleRoll   = 0;
    sUpdateDoorTimer   += 1;
    return true;
}

static s16 sObjDialogId = -1;
static s32 sObjDialogState = 0;

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

static u8 sObjCutsceneId = 0;
static s32 sObjCutsceneState = 0;
static struct Object *sObjCutsceneObj = NULL;

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

//
// Animation
//

static void obj_anim_update_frame_from_accel_assist(struct AnimInfoStruct *animInfo) {
    animInfo->animFrame = (s16) (animInfo->animFrameAccelAssist >> 16);
}

void obj_anim_sync_frame_counters(struct AnimInfoStruct *animInfo) {
    if (!animInfo->animAccel) animInfo->animAccel = 0x10000;
    s32 animFrameH = ((s32) animInfo->animFrame) << 16;
    s32 animFrameL = animInfo->animFrameAccelAssist & 0xFFFF;
    animInfo->animFrameAccelAssist = animFrameH | animFrameL;
}

s32 obj_anim_get_id(struct Object *o) {
    return o->oAnimID;
}

f32 obj_anim_get_frame(struct Object *o) {
    obj_anim_sync_frame_counters(&o->oAnimInfo);
    return (o->oAnimInfo.animFrameAccelAssist / 65536.f);
}

void obj_anim_play(struct Object *o, s32 animID, f32 animAccel) {
    obj_anim_play_with_sound(o, animID, animAccel, 0, false);
}

void obj_anim_play_with_sound(struct Object *o, s32 animID, f32 animAccel, s32 soundBits, bool restart) {
    if (restart) {
        o->oAnimID = -1;
        o->oCurrAnim = NULL;
    }
    if (o == gMarioObject) {
        set_mario_anim_with_accel(gMarioState, animID, animAccel * 0x10000);
    } else if (o->oAnimations) {
        geo_obj_init_animation_accel(&o->header.gfx, &o->oAnimations[animID], animAccel * 0x10000);
        o->oAnimID = animID;
        o->oSoundStateID = animID;
    }
    obj_anim_sync_frame_counters(&o->oAnimInfo);
    obj_play_sound(o, soundBits);
}

void obj_anim_loop(struct Object *o) {
    if (obj_anim_is_at_end(o)) {
        obj_anim_set_frame(o, o->oCurrAnim->mLoopStart);
    }
}

void obj_anim_extend(struct Object *o) {
    if (obj_anim_is_near_end(o)) {
        obj_anim_advance(o, -1);
    }
}

void obj_anim_advance(struct Object *o, f32 frames) {
    obj_anim_sync_frame_counters(&o->oAnimInfo);
    if (o->oCurrAnim) {
        if (o->oCurrAnim->flags & ANIM_FLAG_FORWARD) {
            o->oAnimInfo.animFrameAccelAssist = (s32) ((f32) o->oAnimInfo.animFrameAccelAssist - frames * (f32) o->oAnimInfo.animAccel);
            if (o->oAnimInfo.animFrameAccelAssist < (o->oCurrAnim->mLoopStart << 16)) {
                if (o->oCurrAnim->flags & ANIM_FLAG_NOLOOP) {
                    o->oAnimInfo.animFrameAccelAssist = (o->oCurrAnim->mLoopStart << 16);
                } else {
                    o->oAnimInfo.animFrameAccelAssist = ((o->oCurrAnim->mLoopEnd - 1) << 16);
                }
            }
        } else {
            o->oAnimInfo.animFrameAccelAssist = (s32) ((f32) o->oAnimInfo.animFrameAccelAssist + frames * (f32) o->oAnimInfo.animAccel);
            if (o->oAnimInfo.animFrameAccelAssist >= (o->oCurrAnim->mLoopEnd << 16)) {
                if (o->oCurrAnim->flags & ANIM_FLAG_NOLOOP) {
                    o->oAnimInfo.animFrameAccelAssist = ((o->oCurrAnim->mLoopEnd - 1) << 16);
                } else {
                    o->oAnimInfo.animFrameAccelAssist = (o->oCurrAnim->mLoopStart << 16);
                }
            }
        }
    }
    obj_anim_update_frame_from_accel_assist(&o->oAnimInfo);
}

void obj_anim_set_frame(struct Object *o, f32 frame) {
    o->oAnimInfo.animFrameAccelAssist = (s32) (frame * 65536.f);
    if (!o->oAnimInfo.animAccel) o->oAnimInfo.animAccel = 0x10000;
    obj_anim_update_frame_from_accel_assist(&o->oAnimInfo);
}

void obj_anim_clamp_frame(struct Object *o, f32 frameMin, f32 frameMax) {
    if (obj_anim_get_frame(o) < frameMin) obj_anim_set_frame(o, frameMin);
    if (obj_anim_get_frame(o) > frameMax) obj_anim_set_frame(o, frameMax);
}

bool obj_anim_is_past_frame(struct Object *o, f32 frame) {
    f32 frameCurrent = obj_anim_get_frame(o);
    f32 frameBefore  = frame - (o->oAnimInfo.animAccel / 131072.f);
    f32 frameAfter   = frame + (o->oAnimInfo.animAccel / 131072.f);
    return frameBefore <= frameCurrent && frameCurrent < frameAfter;
}

bool obj_anim_is_near_end(struct Object *o) {
    return o->oCurrAnim && obj_anim_get_frame(o) >= (o->oCurrAnim->mLoopEnd - 2);
}

bool obj_anim_is_at_end(struct Object *o) {
    return o->oCurrAnim && obj_anim_get_frame(o) >= (o->oCurrAnim->mLoopEnd - 1);
}
