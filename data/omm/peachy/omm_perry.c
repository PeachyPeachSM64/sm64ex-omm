#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

const OmmPerryModifiers gOmmPerryModifiers[5] = {
    { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x40, 0xFF, 0xB0, 0x40 }, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 0, 0, 0 },
    { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x60, 0xFF, 0xC0, 0x60 }, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 0, OBJ_INT_ATTRACT_COINS_WEAK | OBJ_INT_ATTRACT_COINS_RANGE(400), OBJ_INT_ATTRACT_COINS_STRONG | OBJ_INT_ATTRACT_COINS_RANGE(500) },
    { { 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x30, 0xC0, 0x00, 0x00 }, 1.00f, 1.15f, 1.15f, 0.90f, 1.15f, OBJ_INT_ATTACK_STRONG, OBJ_INT_ATTACK_STRONG, OBJ_INT_ATTACK_ONE_HIT | OBJ_INT_ATTACK_DESTRUCTIBLE },
    { { 0xFF, 0xFF, 0xFF, 0x00, 0xC0, 0xFF, 0x00, 0x00, 0xC0 }, 1.00f, 0.90f, 0.90f, 1.15f, 0.90f, OBJ_INT_ATTACK_FLAMES, OBJ_INT_ATTACK_FLAMES | OBJ_INT_NOT_INTERACTED, OBJ_INT_ATTACK_FLAMES | OBJ_INT_NOT_INTERACTED },
    { { 0xFF, 0xFF, 0xFF, 0xC0, 0xFF, 0x00, 0x00, 0x80, 0x00 }, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, OBJ_INT_HEAL_MARIO, OBJ_INT_HEAL_MARIO, OBJ_INT_HEAL_MARIO },
};

struct Object *omm_peach_get_perry_object() {
    if (gOmmPerry && gOmmPerry->activeFlags && gOmmPerry->behavior == omm_bhv_perry) {
        return gOmmPerry;
    }
    return NULL;
}

s32 omm_peach_get_perry_type(struct MarioState *m) {
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

void omm_peach_update_perry_graphics(struct MarioState *m, Mat4 transform, Vec3f translation, Vec3s rotation) {
    struct Object *o = omm_peach_get_perry_object();
    if (o) {

        // Compute Peach's right arm/hand values
        Vec3f armPos, handPos; // Peach's right forearm and hand world positions
        Vec3f armVec, handVec; // Peach's right forearm and hand translations
        Vec3s armRot, handRot; // Peach's right forearm and hand rotations
        vec3f_copy(armPos, geo_get_marios_forearm_pos(false));
        vec3f_copy(handPos, geo_get_marios_hand_pos(false));
        vec3f_dif(armVec, handPos, armPos);
        vec3f_copy(handVec, translation);
        vec3s_set(armRot, atan2s(sqrtf(sqr_f(armVec[0]) + sqr_f(armVec[2])), armVec[1]) - 0x4000, atan2s(armVec[2], armVec[0]) + 0x8000, 0);
        vec3s_set(handRot, 0x0000, 0x0000, 0x4000);

        // Compute Perry's attributes based on Peach's current action
        o->oPerryType = omm_peach_get_perry_type(m);
        f32 perryScale = 0.f;
        u32 perryFlags = 0;
        switch (m->action) {

            // Charge (ground)
            case ACT_OMM_PEACH_PERRY_CHARGE_GROUND: {
                armRot[0]  += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 18, 58, 0x0000, 0x0A2B);
                armRot[1]  += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 18, 58, 0x0000, 0x8244);
                handRot[0] += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 18, 58, 0x0000, 0x4000);
                handRot[1] += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 18, 58, 0x0000, 0x3000);
                handRot[2] += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 18, 58, 0x0000, 0x2000);
                perryScale  = clamp_f(m->actionTimer / 5.f, 0.01f, 1.f);
                perryFlags  = OBJ_INT_PERRY_SWORD;
            } break;

            // Charge (air)
            case ACT_OMM_PEACH_PERRY_CHARGE_AIR: {
                armRot[0]  += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 0, 10, 0x0000, 0x0AB1);
                armRot[1]  += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 0, 10, 0x0000, 0x3824);
                handRot[0] += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 0, 10, 0x0000, 0x2000);
                handRot[1] += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 0, 10, 0x0000, 0x1800);
                handRot[2] += relerp_0_1_s(m->marioObj->oAnimInfo.animFrame, 0, 10, 0x0000, 0x0000);
                perryScale  = clamp_f(m->actionTimer / 5.f, 0.01f, 1.f);
                perryFlags  = OBJ_INT_PERRY_SWORD;
            } break;

            // Attack (ground)
            case ACT_OMM_PEACH_ATTACK_GROUND: {
                perryScale = clamp_f((m->actionTimer - 2) / 4.f, 0.01f, 1.f);
                perryFlags = OBJ_INT_PERRY_SWORD | OMM_PERRY_SWORD_INT_FLAGS | (OBJ_INT_GRAB_OBJECTS * (m->actionArg < 4)) | (OBJ_INT_PERRY_TRAIL * (m->actionState == 2));
            } break;

            // Attack (fast)
            case ACT_OMM_PEACH_ATTACK_FAST: {
                perryScale = clamp_f((m->actionTimer - 2) / 4.f, 0.01f, 1.f);
                perryFlags = OBJ_INT_PERRY_SWORD | OMM_PERRY_SWORD_INT_FLAGS | OBJ_INT_PERRY_TRAIL;
            } break;

            // Attack (air)
            case ACT_OMM_PEACH_ATTACK_AIR: {
                if (m->flags & MARIO_KICKING) {
                    perryScale = clamp_f((m->actionTimer - 2) / 4.f, 0.01f, 1.f);
                    perryFlags = OBJ_INT_PERRY_SWORD | OMM_PERRY_SWORD_INT_FLAGS | OBJ_INT_PERRY_TRAIL;
                }
            } break;

            // Glide
            case ACT_OMM_PEACH_GLIDE: {
                vec3s_set(armRot, 0, m->faceAngle[1] + 0x4000, 0);
                perryScale = clamp_f(m->actionTimer / 6.f, 0.01f, 1.f);
                perryFlags = OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_COLLECT_STARS | OBJ_INT_COLLECT_CAPS;
            } break;
        }

        // Set attributes
        if (perryFlags & OBJ_INT_PERRY_SWORD) {
            handVec[0] += 16;
            handVec[1] += 32 * coss(handRot[0]);
            handVec[2] += 32 * sins(handRot[0]);
            Mat4 matrix, identity;
            mtxf_identity(identity);
            mtxf_rotate_xyz_and_translate(matrix, handVec, handRot);
            mtxf_mul(matrix, matrix, transform);
            get_pos_from_transform_mtx(&o->oPosX, matrix, identity);
            o->oPerryRightHandRot(0) = handRot[0] - rotation[0];
            o->oPerryRightHandRot(1) = handRot[1] - rotation[1];
            o->oPerryRightHandRot(2) = handRot[2] - rotation[2];
        } else {
            vec3f_normalize(armVec);
            vec3f_mul(armVec, 8.f * m->marioObj->oScaleX);
            vec3f_add(handPos, armVec);
            obj_set_pos(o, handPos[0], handPos[1], handPos[2]);
        }
        obj_set_angle(o, armRot[0], armRot[1], armRot[2]);
        obj_scale_xyz(o, perryScale * m->marioObj->oScaleX, perryScale * m->marioObj->oScaleY, perryScale * m->marioObj->oScaleZ);
        obj_update_gfx(o);
        o->oAction = !(perryFlags & OBJ_INT_PERRY_SWORD);
        o->oOpacity = (perryScale != 0.f) * ((m->marioBodyState->modelState & 0x100) ? (m->marioBodyState->modelState & 0xFF) : 0xFF) * !(m->marioObj->oNodeFlags & GRAPH_RENDER_INVISIBLE);
        o->oPerryFlags = perryFlags;
    }
}