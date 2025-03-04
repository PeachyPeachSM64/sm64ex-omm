#ifndef BEHAVIOR_SCRIPT_H
#define BEHAVIOR_SCRIPT_H

#include "types.h"

#define obj_get_int(o, offset)          (o)->OBJECT_FIELD_S32(offset)
#define obj_get_float(o, offset)        (o)->OBJECT_FIELD_F32(offset)
#define obj_add_float(o, offset, value) (o)->OBJECT_FIELD_F32(offset) += (f32) (value)
#define obj_set_float(o, offset, value) (o)->OBJECT_FIELD_F32(offset)  = (f32) (value)
#define obj_add_int(o, offset, value)   (o)->OBJECT_FIELD_S32(offset) += (s32) (value)
#define obj_set_int(o, offset, value)   (o)->OBJECT_FIELD_S32(offset)  = (s32) (value)
#define obj_or_int(o, offset, value)    (o)->OBJECT_FIELD_S32(offset) |= (s32) (value)
#define obj_and_int(o, offset, value)   (o)->OBJECT_FIELD_S32(offset) &= (s32) (value)

void stub_behavior_script_2(void);
void obj_update(struct Object *o);
void cur_obj_update(void);

#endif
