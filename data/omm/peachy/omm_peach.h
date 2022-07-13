#ifndef OMM_PEACH_H
#define OMM_PEACH_H

#include "data/omm/omm_includes.h"
#include "omm_perry.h"

#define MODEL_PEACH_OMM                     0x30
#define MODEL_PEACHS_CAP_OMM                0x31
#define MODEL_PEACHS_WING_CAP_OMM           0x32
#define MODEL_PEACHS_METAL_CAP_OMM          0x33
#define MODEL_PEACHS_WINGED_METAL_CAP_OMM   0x34

bool omm_peach_vibe_is_active();
bool omm_peach_vibe_is_joy();
bool omm_peach_vibe_is_rage();
bool omm_peach_vibe_is_gloom();
bool omm_peach_vibe_is_calm();
bool omm_peach_vibe_activate(struct MarioState *m, s32 vibe);
bool omm_peach_vibe_deactivate(struct MarioState *m);
void omm_peach_vibe_update(struct MarioState *m);

s32 omm_act_peach_float(struct MarioState *m);
s32 omm_act_peach_glide(struct MarioState *m);
s32 omm_act_peach_attack_ground(struct MarioState *m);
s32 omm_act_peach_attack_fast(struct MarioState *m);
s32 omm_act_peach_attack_air(struct MarioState *m);
s32 omm_act_peach_perry_charge_ground(struct MarioState *m);
s32 omm_act_peach_perry_charge_air(struct MarioState *m);

s32 omm_act_peach_vibe_joy_move(struct MarioState *m);
s32 omm_act_peach_vibe_joy_fly(struct MarioState *m);
s32 omm_act_peach_vibe_joy_attack(struct MarioState *m);

#endif
