#ifndef ANIMATION_H
#define ANIMATION_H

#include "types.h"

#define ANIM_FLAG_NO_LOOP           (1 << 0)
#define ANIM_FLAG_REVERSE           (1 << 1)
#define ANIM_FLAG_FREEZE            (1 << 2)
#define ANIM_FLAG_Y_TRANS           (1 << 3)
#define ANIM_FLAG_XZ_TRANS          (1 << 4)
#define ANIM_FLAG_ENABLED           (1 << 5)
#define ANIM_FLAG_NO_TRANS          (1 << 6)
#define ANIM_FLAG_LOOP              (1 << 8)

#define ANIM_ACCEL_ONE              (0x10000)
#define ANIM_ACCEL_BITS             (ANIM_ACCEL_ONE - 1)

#define ANIM_LENGTH(ilen, vlen)     ((((ilen) << 20) & 0xFFF00000) | (((vlen) << 0) & 0x000FFFFF))
#define ANIM_INDEX_LENGTH(anim)     (((anim)->length & 0xFFF00000) >> 20)
#define ANIM_VALUES_LENGTH(anim)    (((anim)->length & 0x000FFFFF) >>  0)

//
// Geo animation
//

void geo_obj_sync_anim_frame(struct_AnimInfo *animInfo);
void geo_obj_sync_anim_frame_accel_assist(struct_AnimInfo *animInfo, bool clearBits);
void geo_obj_init_animation(struct GraphNodeObject *node, struct Animation **animPtrAddr);
void geo_obj_init_animation_accel(struct GraphNodeObject *node, struct Animation **animPtrAddr, s32 animAccel);
s16  geo_obj_retrieve_animation_value(const u16 *indices, u32 indicesLen, const s16 *values, u32 valuesLen, s32 frame, u32 *offset);
s32  geo_obj_update_animation_frame(struct_AnimInfo *animInfo);

//
// Object animation
//

void obj_anim_play(struct Object *o, s32 animID, f32 animAccel);
void obj_anim_play_with_sound(struct Object *o, s32 animID, f32 animAccel, s32 soundBits, bool restart);
void obj_anim_loop(struct Object *o);
void obj_anim_extend(struct Object *o);
void obj_anim_set_speed(struct Object *o, f32 animAccel);
void obj_anim_set_frame(struct Object *o, s16 frame);
void obj_anim_clamp_frame(struct Object *o, s16 frameMin, s16 frameMax);
bool obj_anim_is_past_frame(struct Object *o, s16 frame);
bool obj_anim_is_near_end(struct Object *o);
bool obj_anim_is_at_end(struct Object *o);

#endif
