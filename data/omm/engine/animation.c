#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo animation
//

static bool geo_obj_check_animation(struct_AnimInfo *animInfo, struct Animation *anim) {
    return anim != NULL && (
        animInfo->curAnim == NULL || (
        animInfo->curAnimRef != NULL ?
        animInfo->curAnimRef != anim :
        animInfo->curAnim != anim
    ));
}

void geo_obj_sync_anim_frame(struct_AnimInfo *animInfo) {
    animInfo->animFrame = animInfo->animFrameAccelAssist / ANIM_ACCEL_ONE;
}

void geo_obj_sync_anim_frame_accel_assist(struct_AnimInfo *animInfo, bool clearBits) {
    animInfo->animFrameAccelAssist &= ANIM_ACCEL_BITS * !clearBits;
    animInfo->animFrameAccelAssist |= animInfo->animFrame * ANIM_ACCEL_ONE;
}

void geo_obj_init_animation(struct GraphNodeObject *node, struct Animation **animPtrAddr) {
    struct Animation *anim = *animPtrAddr;
    struct_AnimInfo *animInfo = &node->mAnimInfo;
    if (geo_obj_check_animation(animInfo, anim)) {
        animInfo->curAnim = anim;
        animInfo->curAnimRef = anim;
        animInfo->animFrame = anim->mStartFrame + ((anim->flags & ANIM_FLAG_REVERSE) ? 1 : -1);
    }
    animInfo->animAccel = ANIM_ACCEL_ONE;
    animInfo->animFlags = 0;
    geo_obj_sync_anim_frame_accel_assist(animInfo, true);
}

void geo_obj_init_animation_accel(struct GraphNodeObject *node, struct Animation **animPtrAddr, s32 animAccel) {
    struct Animation *anim = *animPtrAddr;
    struct_AnimInfo *animInfo = &node->mAnimInfo;
    if (geo_obj_check_animation(animInfo, anim)) {
        animInfo->curAnim = anim;
        animInfo->curAnimRef = anim;
        animInfo->animFrameAccelAssist = anim->mStartFrame * ANIM_ACCEL_ONE + ((anim->flags & ANIM_FLAG_REVERSE) ? animAccel : -animAccel);
    }
    animInfo->animAccel = animAccel;
    animInfo->animFlags = 0;
    geo_obj_sync_anim_frame(animInfo);
}

s16 geo_obj_retrieve_animation_value(const u16 *indices, u32 indicesLen, const s16 *values, u32 valuesLen, s32 frame, u32 *offset) {
    if (indices && values && (!indicesLen || *offset < indicesLen - 1)) {
        u16 index0 = (indices + *offset)[0];
        u16 index1 = (indices + *offset)[1];
        s32 index = index1 + clamp_s(frame, 0, index0 - 1);
        if (valuesLen) {
            index = clamp_s(index, 0, valuesLen - 1);
        }
        *offset += 2;
        return values[index];
    }
    return 0;
}

s32 geo_obj_update_animation_frame(struct_AnimInfo *animInfo) {
    struct Animation *anim = animInfo->curAnim;

    // Sync animFrameAccelAssist to animFrame
    geo_obj_sync_anim_frame_accel_assist(animInfo, false);

    // Frames
    s16 animFlags = anim->flags | animInfo->animFlags;
    s32 currFrame = animInfo->animFrameAccelAssist;
    s32 loopStart = anim->mLoopStart * ANIM_ACCEL_ONE;
    s32 loopEnd   = anim->mLoopEnd * ANIM_ACCEL_ONE;

    // If the animation is already updated (or shouldn't be updated), return the current frame
    if (animInfo->animTimer == gAreaUpdateCounter || (animFlags & ANIM_FLAG_FREEZE)) {
        return clamp_s(currFrame, 0, loopEnd - 1);
    }

    // Animation going backwards
    if (animFlags & ANIM_FLAG_REVERSE) {
        currFrame -= animInfo->animAccel;
        if (currFrame < loopStart) {
            if ((animFlags & ANIM_FLAG_NO_LOOP) && !(animFlags & ANIM_FLAG_LOOP)) {
                currFrame = loopStart + (currFrame & ANIM_ACCEL_BITS);
            } else {
                currFrame = loopEnd + (currFrame & ANIM_ACCEL_BITS) - ANIM_ACCEL_ONE;
            }
        }
    }

    // Animation going forward
    else {
        currFrame += animInfo->animAccel;
        if (currFrame >= loopEnd) {
            if ((animFlags & ANIM_FLAG_NO_LOOP) && !(animFlags & ANIM_FLAG_LOOP)) {
                currFrame = loopEnd + (currFrame & ANIM_ACCEL_BITS) - ANIM_ACCEL_ONE;
            } else {
                currFrame = loopStart + (currFrame & ANIM_ACCEL_BITS);
            }
        }
    }

    // Return the updated frame
    return clamp_s(currFrame, 0, loopEnd - 1);
}

//
// Object animation
//

void obj_anim_play(struct Object *o, s32 animID, f32 animAccel) {
    obj_anim_play_with_sound(o, animID, animAccel, 0, false);
}

void obj_anim_play_with_sound(struct Object *o, s32 animID, f32 animAccel, s32 soundBits, bool restart) {
    if (restart) {
        o->oAnimID = -1;
        o->oCurrAnim = NULL;
        o->oCurrAnimRef = NULL;
    }
    if (o == gMarioObject) {
        set_mario_anim_with_accel(gMarioState, animID, animAccel * ANIM_ACCEL_ONE);
    } else if (o->oAnimations) {
        geo_obj_init_animation_accel(&o->header.gfx, &o->oAnimations[animID], animAccel * ANIM_ACCEL_ONE);
        o->oAnimID = animID;
        o->oSoundStateID = animID;
    }
    obj_play_sound(o, soundBits);
}

void obj_anim_loop(struct Object *o) {
    o->oAnimInfo.animFlags |= ANIM_FLAG_LOOP;
}

void obj_anim_extend(struct Object *o) {
    o->oAnimInfo.animFlags |= ANIM_FLAG_NO_LOOP;
}

void obj_anim_set_speed(struct Object *o, f32 animAccel) {
    o->oAnimInfo.animAccel = animAccel * ANIM_ACCEL_ONE;
}

void obj_anim_set_frame(struct Object *o, s16 frame) {
    if (!o->oCurrAnim) return;
    o->oAnimFrame = frame;
    geo_obj_sync_anim_frame_accel_assist(&o->oAnimInfo, true);
}

void obj_anim_clamp_frame(struct Object *o, s16 frameMin, s16 frameMax) {
    if (o->oAnimFrame < frameMin) obj_anim_set_frame(o, frameMin);
    if (o->oAnimFrame > frameMax) obj_anim_set_frame(o, frameMax);
}

bool obj_anim_is_past_frame(struct Object *o, s16 frame) {
    geo_obj_sync_anim_frame_accel_assist(&o->oAnimInfo, false);
    s32 a = frame * ANIM_ACCEL_ONE + o->oAnimInfo.animAccel / 2;
    s32 b = frame * ANIM_ACCEL_ONE - o->oAnimInfo.animAccel / 2;
    f32 c = o->oAnimInfo.animFrameAccelAssist;
    return b <= c && c < a;
}

bool obj_anim_is_near_end(struct Object *o) {
    geo_obj_sync_anim_frame_accel_assist(&o->oAnimInfo, false);
    if (!o->oCurrAnim) return false;
    return (
        (o->oCurrAnim->flags & ANIM_FLAG_REVERSE) != 0 ?
        (o->oAnimInfo.animFrameAccelAssist <= o->oCurrAnim->mLoopStart * ANIM_ACCEL_ONE + 2 * o->oAnimInfo.animAccel) :
        (o->oAnimInfo.animFrameAccelAssist >= o->oCurrAnim->mLoopEnd   * ANIM_ACCEL_ONE - 2 * o->oAnimInfo.animAccel)
    );
}

bool obj_anim_is_at_end(struct Object *o) {
    geo_obj_sync_anim_frame_accel_assist(&o->oAnimInfo, false);
    if (!o->oCurrAnim) return false;
    return (
        (o->oCurrAnim->flags & ANIM_FLAG_REVERSE) != 0 ?
        (o->oAnimInfo.animFrameAccelAssist <= o->oCurrAnim->mLoopStart * ANIM_ACCEL_ONE + o->oAnimInfo.animAccel) :
        (o->oAnimInfo.animFrameAccelAssist >= o->oCurrAnim->mLoopEnd   * ANIM_ACCEL_ONE - o->oAnimInfo.animAccel)
    );
}

//
// Mario animation
//

s16 set_mario_animation(struct MarioState *m, s32 targetAnimID) {
    return set_mario_anim_with_accel(m, targetAnimID, ANIM_ACCEL_ONE);
}

s16 set_mario_anim_with_accel(struct MarioState *m, s32 targetAnimID, s32 accel) {
    struct Object *o = m->marioObj;
    struct Animation *targetAnim = gMarioTargetAnim;
    struct_AnimInfo *animInfo = &m->marioObj->oAnimInfo;

    if (load_patchable_table(gMarioAnimations, targetAnimID)) {
        targetAnim->values = (void *) ((u8 *) targetAnim + (uintptr_t) targetAnim->values);
        targetAnim->index  = (void *) ((u8 *) targetAnim + (uintptr_t) targetAnim->index);
    }

    if (o->oAnimID != targetAnimID) {
        o->oAnimID = targetAnimID;
        o->oCurrAnim = targetAnim;
        if (targetAnim->flags & ANIM_FLAG_FREEZE) {
            animInfo->animFrameAccelAssist = (targetAnim->mStartFrame * ANIM_ACCEL_ONE);
        } else if (targetAnim->flags & ANIM_FLAG_REVERSE) {
            animInfo->animFrameAccelAssist = (targetAnim->mStartFrame * ANIM_ACCEL_ONE) + accel;
        } else {
            animInfo->animFrameAccelAssist = (targetAnim->mStartFrame * ANIM_ACCEL_ONE) - accel;
        }
    }
    animInfo->animAccel = accel;
    animInfo->animFlags = 0;
    geo_obj_sync_anim_frame(animInfo);
    return animInfo->animFrame;
}

void set_anim_to_frame(struct MarioState *m, s16 animFrame) {
    struct Animation *anim = m->marioObj->oCurrAnim;
    struct_AnimInfo *animInfo = &m->marioObj->oAnimInfo;
    if (!anim) return;
    if (anim->flags & ANIM_FLAG_FREEZE) {
        animInfo->animFrameAccelAssist = animFrame * ANIM_ACCEL_ONE;
    } else if (anim->flags & ANIM_FLAG_REVERSE) {
        animInfo->animFrameAccelAssist = animFrame * ANIM_ACCEL_ONE + animInfo->animAccel;
    } else {
        animInfo->animFrameAccelAssist = animFrame * ANIM_ACCEL_ONE - animInfo->animAccel;
    }
    geo_obj_sync_anim_frame(animInfo);
}

s32 is_anim_at_end(struct MarioState *m) {
    return obj_anim_is_at_end(m->marioObj);
}

s32 is_anim_past_end(struct MarioState *m) {
    return obj_anim_is_near_end(m->marioObj);
}

s32 is_anim_past_frame(struct MarioState *m, s16 animFrame) {
    return obj_anim_is_past_frame(m->marioObj, animFrame);
}

s16 find_mario_anim_flags_and_translation(struct Object *obj, s32 yaw, Vec3s translation) {
    struct Animation *currAnim = obj->oCurrAnim;
    if (currAnim) {
        s32 animFrame = geo_obj_update_animation_frame(&obj->oAnimInfo) / ANIM_ACCEL_ONE;
        const u16 *indices = currAnim->index;
        const s16 *values = currAnim->values;
        u32 indicesLen = ANIM_INDEX_LENGTH(currAnim);
        u32 valuesLen = ANIM_VALUES_LENGTH(currAnim);
        u32 offset = 0;
        f32 dx = geo_obj_retrieve_animation_value(indices, indicesLen, values, valuesLen, animFrame, &offset) / 4.f;
        translation[1] = geo_obj_retrieve_animation_value(indices, indicesLen, values, valuesLen, animFrame, &offset) / 4.0f;
        f32 dz = geo_obj_retrieve_animation_value(indices, indicesLen, values, valuesLen, animFrame, &offset) / 4.0f;
        f32 s = (f32) sins(yaw);
        f32 c = (f32) coss(yaw);
        translation[0] = (dx * c) + (dz * s);
        translation[2] = (-dx * s) + (dz * c);
        return currAnim->flags;
    }
    return 0;
}
