#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Perry data
//

const OmmPerryModifiers gOmmPerryModifiers[5] = {

[OMM_PEACH_VIBE_TYPE_NONE] = {
    .colors              = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x40, 0xFF, 0xB0, 0x40 },
    .swordHitbox         = 1.00f,
    .shockwaveRadius     = 1.00f,
    .shockwaveHeight     = 1.00f,
    .shockwaveForwardVel = 1.00f,
    .blastRadius         = 1.00f,
    .swordFlags          = 0,
    .shockwaveFlags      = 0,
    .blastFlags          = 0,
},

[OMM_PEACH_VIBE_TYPE_JOY] = {
    .colors              = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x60, 0xFF, 0xC0, 0x60 },
    .swordHitbox         = 1.00f,
    .shockwaveRadius     = 1.00f,
    .shockwaveHeight     = 1.00f,
    .shockwaveForwardVel = 1.00f,
    .blastRadius         = 1.00f,
    .swordFlags          = 0,
    .shockwaveFlags      = OBJ_INT_ATTRACT_COINS_WEAK | OBJ_INT_ATTRACT_COINS_RANGE(400),
    .blastFlags          = OBJ_INT_ATTRACT_COINS_STRONG | OBJ_INT_ATTRACT_COINS_RANGE(600),
},

[OMM_PEACH_VIBE_TYPE_RAGE] = {
    .colors              = { 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x30, 0xC0, 0x00, 0x00 },
    .swordHitbox         = 1.00f,
    .shockwaveRadius     = 1.15f,
    .shockwaveHeight     = 1.15f,
    .shockwaveForwardVel = 0.90f,
    .blastRadius         = 1.15f,
    .swordFlags          = OBJ_INT_ATTACK_STRONG,
    .shockwaveFlags      = OBJ_INT_ATTACK_STRONG,
    .blastFlags          = OBJ_INT_ATTACK_ONE_HIT | OBJ_INT_ATTACK_DESTRUCTIBLE,
},

[OMM_PEACH_VIBE_TYPE_GLOOM] = {
    .colors              = { 0xFF, 0xFF, 0xFF, 0x00, 0xC0, 0xFF, 0x00, 0x00, 0xC0 },
    .swordHitbox         = 1.00f,
    .shockwaveRadius     = 0.90f,
    .shockwaveHeight     = 0.90f,
    .shockwaveForwardVel = 1.15f,
    .blastRadius         = 0.90f,
    .swordFlags          = OBJ_INT_ATTACK_FLAMES,
    .shockwaveFlags      = OBJ_INT_ATTACK_FLAMES | OBJ_INT_NOT_INTERACTED,
    .blastFlags          = OBJ_INT_ATTACK_FLAMES | OBJ_INT_NOT_INTERACTED,
},

[OMM_PEACH_VIBE_TYPE_CALM] = {
    .colors              = { 0xFF, 0xFF, 0xFF, 0xC0, 0xFF, 0x00, 0x00, 0x80, 0x00 },
    .swordHitbox         = 1.00f,
    .shockwaveRadius     = 1.00f,
    .shockwaveHeight     = 1.00f,
    .shockwaveForwardVel = 1.00f,
    .blastRadius         = 1.00f,
    .swordFlags          = OBJ_INT_HEAL_MARIO,
    .shockwaveFlags      = OBJ_INT_HEAL_MARIO,
    .blastFlags          = OBJ_INT_HEAL_MARIO,
},

};

//
// Perry actions
//

static void omm_perry_compute_attributes(struct MarioState *m, struct Object *o, Vec3s armRot, Vec3s handRot, f32 *scale) {
    o->oPerryType = omm_perry_get_type(m);
    o->oPerryFlags = 0;
    o->oPerryHitboxPos(0) = 0;
    o->oPerryHitboxPos(1) = 0;
    o->oPerryHitboxPos(2) = 0;
    o->oPerryHitboxRadius = 0;
    o->oPerryHitboxHeight = 0;

    // Actions
    switch (m->action) {

        // Charge (ground)
        case ACT_OMM_PEACH_PERRY_CHARGE_GROUND: {
            armRot[0]  += relerp_0_1_s(m->marioObj->oAnimFrame, 18, 58, 0x0000, 0x0A2B);
            armRot[1]  += relerp_0_1_s(m->marioObj->oAnimFrame, 18, 58, 0x0000, 0x8244);
            handRot[0] += relerp_0_1_s(m->marioObj->oAnimFrame, 18, 58, 0x0000, 0x4000);
            handRot[1] += relerp_0_1_s(m->marioObj->oAnimFrame, 18, 58, 0x0000, 0x3000);
            handRot[2] += relerp_0_1_s(m->marioObj->oAnimFrame, 18, 58, 0x0000, 0x2000);
            *scale = clamp_f(m->actionTimer / 5.f, 0.01f, 1.f);
            o->oPerryFlags = OBJ_INT_PERRY_SWORD | OBJ_INT_PERRY_CHARGE;
        } break;

        // Charge (air)
        case ACT_OMM_PEACH_PERRY_CHARGE_AIR: {
            armRot[0]  += relerp_0_1_s(m->marioObj->oAnimFrame, 0, 10, 0x0000, 0x0AB1);
            armRot[1]  += relerp_0_1_s(m->marioObj->oAnimFrame, 0, 10, 0x0000, 0x3824);
            handRot[0] += relerp_0_1_s(m->marioObj->oAnimFrame, 0, 10, 0x0000, 0x2000);
            handRot[1] += relerp_0_1_s(m->marioObj->oAnimFrame, 0, 10, 0x0000, 0x1800);
            handRot[2] += relerp_0_1_s(m->marioObj->oAnimFrame, 0, 10, 0x0000, 0x0000);
            *scale = clamp_f(m->actionTimer / 5.f, 0.01f, 1.f);
            o->oPerryFlags = OBJ_INT_PERRY_SWORD | OBJ_INT_PERRY_CHARGE;
        } break;

        // Attack (ground)
        case ACT_OMM_PEACH_ATTACK_GROUND: {
            *scale = 1.f;
            o->oPerryFlags = OBJ_INT_PERRY_SWORD | OMM_PERRY_SWORD_INT_FLAGS;
            if (m->actionState >= 2) {
                o->oPerryFlags |= OBJ_INT_PERRY_TRAIL;

                // Spin attack
                if (m->actionArg >= 4) {
                    o->oPerryHitboxRadius = OMM_PERRY_SWORD_OFFSET + OMM_PERRY_SWORD_RADIUS;
                    o->oPerryHitboxHeight = OMM_PERRY_SWORD_HEIGHT;
                }

                // Swings
                else {
                    s16 angle = m->faceAngle[1] - 0x2000 + 0x4000 * (m->actionArg < 2 ? 1 : -1) * relerp_0_1_f(m->actionTimer, 0, 8, -1, 1);
                    o->oPerryHitboxPos(0) = OMM_PERRY_SWORD_OFFSET * sins(angle);
                    o->oPerryHitboxPos(2) = OMM_PERRY_SWORD_OFFSET * coss(angle);
                    o->oPerryHitboxRadius = OMM_PERRY_SWORD_RADIUS;
                    o->oPerryHitboxHeight = OMM_PERRY_SWORD_HEIGHT;
                    o->oPerryFlags |= OBJ_INT_GRAB_OBJECTS;
                }
            }
        } break;

        // Attack (fast)
        case ACT_OMM_PEACH_ATTACK_FAST: {
            *scale = 1.f;
            o->oPerryFlags = OBJ_INT_PERRY_SWORD | OMM_PERRY_SWORD_INT_FLAGS | OBJ_INT_PERRY_TRAIL;
            o->oPerryHitboxRadius = OMM_PERRY_SWORD_OFFSET * 0.8f + OMM_PERRY_SWORD_RADIUS;
            o->oPerryHitboxHeight = OMM_PERRY_SWORD_HEIGHT;
        } break;

        // Attack (air)
        case ACT_OMM_PEACH_ATTACK_AIR: {
            if (m->actionArg) {
                *scale = 1.f;
                o->oPerryFlags = OBJ_INT_PERRY_SWORD | OMM_PERRY_SWORD_INT_FLAGS | OBJ_INT_PERRY_TRAIL;
                o->oPerryHitboxRadius = OMM_PERRY_SWORD_OFFSET + OMM_PERRY_SWORD_RADIUS;
                o->oPerryHitboxHeight = OMM_PERRY_SWORD_HEIGHT;
            }
        } break;

        // Glide
        case ACT_OMM_PEACH_GLIDE: {
            vec3s_set(armRot, 0, m->faceAngle[1] + 0x4000, 0);
            *scale = clamp_f(m->actionTimer / 6.f, 0.01f, 1.f);
            o->oPerryFlags = OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_COLLECT_STARS | OBJ_INT_COLLECT_CAPS;
            o->oPerryHitboxPos(1) = 80.f;
            o->oPerryHitboxRadius = OMM_PERRY_SWORD_RADIUS;
            o->oPerryHitboxHeight = OMM_PERRY_SWORD_HEIGHT;
        } break;
    }
}

//
// Getters
//

struct Object *omm_perry_get_object() {
    if (gOmmPerry && gOmmPerry->activeFlags && gOmmPerry->behavior == bhvOmmPerry) {
        return gOmmPerry;
    }
    return NULL;
}

s32 omm_perry_get_type(struct MarioState *m) {
    if (omm_peach_vibe_is_active()) {
        if (omm_peach_vibe_is_joy()) return OMM_PEACH_VIBE_TYPE_JOY;
        if (omm_peach_vibe_is_rage()) return OMM_PEACH_VIBE_TYPE_RAGE;
        if (omm_peach_vibe_is_gloom()) return OMM_PEACH_VIBE_TYPE_GLOOM;
        if (omm_peach_vibe_is_calm()) return OMM_PEACH_VIBE_TYPE_CALM;
    } else {
        if (omm_mario_has_wing_cap(m)) return OMM_PEACH_VIBE_TYPE_JOY;
        if (omm_mario_has_metal_cap(m)) return OMM_PEACH_VIBE_TYPE_RAGE;
        if (omm_mario_has_vanish_cap(m)) return OMM_PEACH_VIBE_TYPE_GLOOM;
    }
    return OMM_PEACH_VIBE_TYPE_NONE;
}

//
// Behavior
//

static void bhv_omm_perry_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    obj_set_params(o, 0, 0, 0, 0, 0);
    obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);

    // Update type and model
    o->oPerryType = omm_perry_get_type(m);
    o->oGraphNode = geo_layout_to_graph_node(NULL, OMM_CHEAT_SHADOW_MARIO ? omm_geo_brush : omm_geo_perry);

    // Preprocess to update the close/open state
    geo_preprocess_object_graph_node(o);

    // Hitbox and interactions
    if (o->oPerryHitboxRadius > 0 && o->oPerryHitboxHeight > 0) {
        o->oPosX = m->pos[0] + o->oPerryHitboxPos(0) * o->oScaleX;
        o->oPosY = m->pos[1] + o->oPerryHitboxPos(1) * o->oScaleY + 80.f * m->marioObj->oScaleY;
        o->oPosZ = m->pos[2] + o->oPerryHitboxPos(2) * o->oScaleZ;
        obj_set_params(o, 0, 0, 0, 0, true);
        obj_reset_hitbox(o, o->oPerryHitboxRadius, o->oPerryHitboxHeight, 0, 0, 0, o->oPerryHitboxHeight / 2.f);

        // Process interactions
        struct Object *interacted = omm_obj_process_interactions(o, o->oPerryFlags);
        if (interacted && (o->oPerryFlags & OBJ_INT_PERRY_SWORD) && !omm_obj_is_collectible(interacted)) {
            obj_play_sound(interacted, SOUND_ACTION_HIT_2);
        }

        // Reset values
        obj_set_pos_vec3f(o, o->oGfxPos);
        obj_set_params(o, 0, 0, 0, 0, false);
        obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    }
}

const BehaviorScript bhvOmmPerry[] = {
    OBJ_TYPE_SPECIAL, // This object must be updated before bhvOmmPerryTrail
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_perry_update),
    BHV_END_LOOP(),
};

//
// Spawner (auto)
//

OMM_ROUTINE_UPDATE(omm_obj_spawn_perry) {
    if (gMarioObject) {
        if (OMM_PERRY_IS_AVAILABLE) {
            struct Object *perry = omm_perry_get_object();
            if (!perry) {
                gOmmPerry = spawn_object(gMarioObject, MODEL_NONE, bhvOmmPerry);
            }
        } else {
            obj_deactivate_all_with_behavior(bhvOmmPerry);
            obj_deactivate_all_with_behavior(bhvOmmPerryTrail);
            obj_deactivate_all_with_behavior(bhvOmmPerryShockwave);
            obj_deactivate_all_with_behavior(bhvOmmPerryBlast);
            obj_deactivate_all_with_behavior(bhvOmmPerryCharge);
        }
    }
}

//
// Update
//

void omm_perry_update_graphics(struct MarioState *m, Mat4 transform, Vec3f translation, Vec3s rotation) {
    struct Object *o = omm_perry_get_object();
    if (o) {

        // Compute Peach's right arm/hand values
        Vec3f armPos, handPos; // Peach's right forearm and hand world positions
        Vec3f armVec, handVec; // Peach's right forearm and hand translations
        Vec3s armRot, handRot; // Peach's right forearm and hand rotations
        geo_get_marios_anim_part_pos(NULL, armPos, MARIO_ANIM_PART_RIGHT_FOREARM);
        geo_get_marios_anim_part_pos(NULL, handPos, MARIO_ANIM_PART_RIGHT_HAND);
        vec3f_dif(armVec, handPos, armPos);
        vec3f_copy(handVec, translation);
        vec3s_set(armRot, atan2s(sqrtf(sqr_f(armVec[0]) + sqr_f(armVec[2])), armVec[1]) - 0x4000, atan2s(armVec[2], armVec[0]) + 0x8000, 0);
        vec3s_set(handRot, 0x0000, 0x0000, 0x4000);

        // Compute Perry's attributes based on Peach's current action
        f32 scale = 0.f;
        omm_perry_compute_attributes(m, o, armRot, handRot, &scale);

        // Set attributes
        if (o->oPerryFlags & OBJ_INT_PERRY_SWORD) {
            f32 handRotMult = 1.f;
            if (OMM_PLAYER_MODEL_IS_PEACH) {
                handVec[0] += 16;
                handVec[1] += 32 * coss(handRot[0]);
                handVec[2] += 32 * sins(handRot[0]);
            } else if (o->oPerryFlags & OBJ_INT_PERRY_CHARGE) {
                handVec[0] += 32;
                handVec[1] += 32 * coss(handRot[0]) - 24;
                handVec[2] += 64 * sins(handRot[0]);
            } else {
                handVec[0] *= 1.5f;
                handVec[1] *= 1.5f;
                handVec[2] *= 1.5f;
                handRotMult = 0.25f;
            }
            Mat4 matrix, identity;
            mtxf_identity(identity);
            mtxf_rotate_xyz_and_translate(matrix, handVec, handRot);
            mtxf_mul(matrix, matrix, transform);
            get_pos_from_transform_mtx(&o->oPosX, matrix, identity);
            o->oPerryRightHandRot(0) = (handRot[0] - rotation[0]) * handRotMult;
            o->oPerryRightHandRot(1) = (handRot[1] - rotation[1]) * handRotMult;
            o->oPerryRightHandRot(2) = (handRot[2] - rotation[2]) * handRotMult;
        } else {
            vec3f_normalize(armVec);
            vec3f_mul(armVec, 8.f * m->marioObj->oScaleX);
            vec3f_add(handPos, armVec);
            obj_set_xyz(o, handPos[0], handPos[1], handPos[2]);
        }
        obj_set_angle(o, armRot[0], armRot[1], armRot[2]);
        obj_scale_xyz(o, scale * m->marioObj->oScaleX, scale * m->marioObj->oScaleY, scale * m->marioObj->oScaleZ);
        obj_update_gfx(o);
        u8 marioOpacity = ((m->marioBodyState->modelState & 0x500) == 0x100 ? (m->marioBodyState->modelState & 0xFF) : 0xFF);
        o->oAction = !(o->oPerryFlags & OBJ_INT_PERRY_SWORD);
        o->oOpacity = marioOpacity * (scale != 0.f) * !(m->marioObj->oNodeFlags & GRAPH_RENDER_INVISIBLE);
    }
}
