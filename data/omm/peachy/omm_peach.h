#ifndef OMM_PEACH_H
#define OMM_PEACH_H

#include "types.h"

#define MODEL_PEACH_OMM                             0x30
#define MODEL_PEACHS_CAP_OMM                        0x31
#define MODEL_PEACHS_WING_CAP_OMM                   0x32
#define MODEL_PEACHS_METAL_CAP_OMM                  0x33
#define MODEL_PEACHS_WINGED_METAL_CAP_OMM           0x34

#define GEO_PREPROCESS_PEACH_UP_VEC                 GEO_CULLING_RADIUS(0x01)
#define GEO_PREPROCESS_PEACH_FORWARD_VEC            GEO_CULLING_RADIUS(0x02)
#define GEO_PREPROCESS_PEACH_LATERAL_VEC            GEO_CULLING_RADIUS(0x03)
#define GEO_PREPROCESS_PEACH_TORSO_POS              GEO_CULLING_RADIUS(0x04)
#define GEO_PREPROCESS_PEACH_LEFT_LEG_POS           GEO_CULLING_RADIUS(0x05)
#define GEO_PREPROCESS_PEACH_LEFT_FOOT_POS          GEO_CULLING_RADIUS(0x06)
#define GEO_PREPROCESS_PEACH_RIGHT_LEG_POS          GEO_CULLING_RADIUS(0x07)
#define GEO_PREPROCESS_PEACH_RIGHT_FOOT_POS         GEO_CULLING_RADIUS(0x08)
#define GEO_PREPROCESS_PEACH_HIPS_ROT_AND_LEGS      GEO_CULLING_RADIUS(0x09)
#define GEO_PREPROCESS_PEACH_DRESS_ROT              GEO_CULLING_RADIUS(0x0A)
#define GEO_PREPROCESS_PEACH_HEAD_ROT               GEO_CULLING_RADIUS(0x0B)
#define GEO_PREPROCESS_PEACH_HAIR_ROT               GEO_CULLING_RADIUS(0x0C)
#define GEO_PREPROCESS_PEACH_CROWN_TIARA            GEO_CULLING_RADIUS(0x0D)
#define GEO_PREPROCESS_PEACH_PERRY_STATE            GEO_CULLING_RADIUS(0x0E)
#define GEO_PREPROCESS_PEACH_HAND_ROT               GEO_CULLING_RADIUS(0x0F)

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
