#ifndef OMM_OBJECT_H
#define OMM_OBJECT_H

#include "data/omm/omm_includes.h"
#include "omm_object_data.h"
#include "omm_object_fields.h"

//
// Flags
//

#define ACTIVE_FLAG_DORMANT         (1 << 11) // 0x0800
#define ACTIVE_FLAG_KNOCKED_BACK    (1 << 12) // 0x1000
#define ACTIVE_FLAG_RED_COIN_STAR   (1 << 13) // 0x2000

//
// For loops
//

extern s32 gOmmCappyObjectLists[];
extern s32 gOmmInteractionObjectLists[];

#define for_each_(__type__, __item__, __size__, ...)            s32 index_##__item__ = 0; for (__type__ *__item__ = __VA_ARGS__; index_##__item__ != __size__; ++index_##__item__, ++__item__)
#define for_each_until_null(__type__, __item__, ...)            for (__type__ *__item__ = __VA_ARGS__; *__item__ != NULL; ++__item__)
#define for_each_object_in_list(__obj__, __list__)              for (struct Object* __obj__ = obj_get_first(__list__); __obj__ != NULL; __obj__ = obj_get_next(__obj__, __list__))
#define for_each_object_in_cappy_lists(__obj__)                 for (s32 *__list__ = gOmmCappyObjectLists; *__list__ != -1; ++__list__) for (struct Object* __obj__ = obj_get_first(*__list__); __obj__ != NULL; __obj__ = obj_get_next(__obj__, *__list__))
#define for_each_object_in_interaction_lists(__obj__)           for (s32 *__list__ = gOmmInteractionObjectLists; *__list__ != -1; ++__list__) for (struct Object* __obj__ = obj_get_first(*__list__); __obj__ != NULL; __obj__ = obj_get_next(__obj__, *__list__))
#define for_each_object_with_behavior(__obj__, __behavior__)    for (struct Object* __obj__ = obj_get_first_with_behavior(__behavior__); __obj__ != NULL; __obj__ = obj_get_next_with_behavior(__obj__, __behavior__))

//
// Objects
//

struct Object *obj_spawn_from_geo(struct Object *parent, const GeoLayout *geoLayout, const BehaviorScript *behavior);
struct Object *obj_get_first(s32 list);
struct Object *obj_get_next(struct Object *o, s32 list);
struct Object *obj_get_first_with_behavior(const BehaviorScript *behavior);
struct Object *obj_get_next_with_behavior(struct Object *o, const BehaviorScript *behavior);
struct Object *obj_get_first_with_behavior_and_field_s32(const BehaviorScript *behavior, s32 fieldIndex, s32 value);
struct Object *obj_get_first_with_behavior_and_field_f32(const BehaviorScript *behavior, s32 fieldIndex, f32 value);
struct Object *obj_get_nearest_with_behavior(struct Object *o, const BehaviorScript *behavior);
struct Object *obj_get_nearest_with_behavior_and_radius(struct Object *o, const BehaviorScript *behavior, f32 distMax);
s32            obj_get_list_index(struct Object *o);
s32            obj_get_count_with_behavior(const BehaviorScript *behavior);
s32            obj_get_count_with_behavior_and_field_s32(const BehaviorScript *behavior, s32 fieldIndex, s32 value);
s32            obj_get_count_with_behavior_and_field_f32(const BehaviorScript *behavior, s32 fieldIndex, f32 value);
void           obj_deactivate_all_with_behavior(const BehaviorScript *behavior);
void           obj_unload_all_with_behavior(const BehaviorScript *behavior);

//
// Helpers
//

f32  obj_get_horizontal_distance(struct Object *o1, struct Object *o2);
f32  obj_get_distance(struct Object *o1, struct Object *o2);
bool obj_check_model(struct Object *o, s32 modelId);
bool obj_has_graph_node(struct Object *o, struct GraphNode *node);
bool obj_is_surface(struct Object *o);
bool obj_is_on_ground(struct Object *o);
bool obj_is_underwater(struct Object *o, f32 waterLevel);
s32  obj_get_room(struct Object *o);
s32  obj_get_object1_angle_yaw_to_object2(struct Object *o1, struct Object *o2);
bool obj_is_object1_facing_object2(struct Object *o1, struct Object *o2, s16 angleRange);
bool obj_is_object2_hit_from_above(struct Object *o1, struct Object *o2);
bool obj_is_object2_hit_from_below(struct Object *o1, struct Object *o2);
bool obj_is_object2_pushing_object1_backwards(struct Object *o1, struct Object *o2, bool setObj1FaceYaw);
bool obj_check_hitbox(struct Object *o, u32 flags);
bool obj_detect_hitbox_overlap(struct Object *o1, struct Object *o2, u32 obj1Flags, u32 obj2Flags);
void obj_set_home(struct Object *o, f32 x, f32 y, f32 z);
void obj_set_vel(struct Object *o, f32 x, f32 y, f32 z);
void obj_set_forward_vel(struct Object *o, s16 yaw, f32 mag, f32 velMax);
void obj_set_forward_and_y_vel(struct Object *o, f32 forwardVel, f32 yVel);
void obj_set_angle_vel(struct Object *o, s16 pitch, s16 yaw, s16 roll);
void obj_set_scale(struct Object *o, f32 x, f32 y, f32 z);
void obj_safe_step(struct Object *o, s32 update);
void obj_update_blink_state(struct Object *o, s32 *timer, s16 base, s16 range, s16 length);
void obj_random_blink(struct Object *o, s32 *timer);
void obj_make_step_sound_and_particle(struct Object *o, f32 *dist, f32 distMin, f32 distInc, s32 soundBits, u32 particles);
void obj_play_sound(struct Object *o, s32 soundBits);
s32  obj_get_model_id(struct Object *o);
s32  obj_get_coin_type(struct Object *o);
void obj_spawn_star(struct Object *o, f32 xFrom, f32 yFrom, f32 zFrom, f32 xTo, f32 yTo, f32 zTo, s32 starIndex, bool noExit);
void obj_spawn_white_puff(struct Object *o, s32 soundBits);
void obj_spawn_white_puff_at(f32 x, f32 y, f32 z, s32 soundBits);
void obj_spawn_triangle_break_particles(struct Object *o, s32 count, s32 model, f32 size, s32 type);
void obj_spawn_break_particles(struct Object *o, s32 count, u8 r, u8 g, u8 b, f32 offsetY, f32 velMin, f32 velMax, f32 velY, f32 sizeMin, f32 sizeMax);
void obj_spawn_particles(struct Object *o, s32 count, s32 model, f32 yOffset, f32 forwardVelBase, f32 forwardVelRange, f32 yVelBase, f32 yVelRange, f32 gravity, f32 scaleBase, f32 scaleRange);
void obj_destroy(struct Object *o);
void obj_create_respawner(struct Object *o, s32 model, const BehaviorScript *behavior, f32 minSpawnDist);
void obj_update_gfx(struct Object *o);
void obj_set_params(struct Object *o, s32 interactType, s32 damageOrCoinValue, s32 health, s32 numLootCoins, bool setTangible);
void obj_reset_hitbox(struct Object *o, f32 hitboxRadius, f32 hitboxHeight, f32 hurtboxRadius, f32 hurtboxHeight, f32 wallHitboxRadius, f32 hitboxDownOffset);
void obj_set_shadow_pos_to_object_pos(struct Object *o);
void obj_fix_shadow_pos(struct GraphNodeObject *gfx, Vec3f shadowPos);
void obj_drop_to_floor(struct Object *o);
void obj_load_collision_model(struct Object *o);
bool obj_is_always_rendered(struct Object *o);
void obj_set_always_rendered(struct Object *o, bool set);
bool obj_is_dormant(struct Object *o);
void obj_set_dormant(struct Object *o, bool set);
bool obj_is_knocked_back(struct Object *o);
bool obj_set_knockback(struct Object *o, struct Object *from, s32 knockbackType, f32 fvel, f32 yvel);
void obj_open_door(struct Object *o, struct Object *door);
bool obj_update_door(struct Object *o);
bool obj_dialog_start(s16 dialogId);
bool obj_dialog_update();
bool obj_cutscene_start(u8 cutsceneId, struct Object *o);
bool obj_cutscene_update();
struct Waypoint *obj_path_get_nearest_waypoint(struct Object *o, struct Waypoint *list, f32 radiusMax);
struct Object *obj_get_red_coin_star();

//
// Animation
//

void obj_anim_sync_frame_counters(struct AnimInfoStruct *animInfo);
s32  obj_anim_get_id(struct Object *o);
f32  obj_anim_get_frame(struct Object *o);
void obj_anim_play(struct Object *o, s32 animID, f32 animAccel);
void obj_anim_play_with_sound(struct Object *o, s32 animID, f32 animAccel, s32 soundBits, bool restart);
void obj_anim_loop(struct Object *o);
void obj_anim_extend(struct Object *o);
void obj_anim_advance(struct Object *o, f32 frames);
void obj_anim_set_frame(struct Object *o, f32 frame);
void obj_anim_clamp_frame(struct Object *o, f32 frameMin, f32 frameMax);
bool obj_anim_is_past_frame(struct Object *o, f32 frame);
bool obj_anim_is_near_end(struct Object *o);
bool obj_anim_is_at_end(struct Object *o);

//
// Types and interactions
//

bool omm_obj_is_interactible(struct Object *o);
bool omm_obj_is_coin(struct Object *o);
bool omm_obj_is_water_ring(struct Object *o);
bool omm_obj_is_mushroom_1up(struct Object *o);
bool omm_obj_is_star_or_key(struct Object *o);
bool omm_obj_is_star_model(struct Object *o);
bool omm_obj_is_cap(struct Object *o);
bool omm_obj_is_secret(struct Object *o);
bool omm_obj_is_weak(struct Object *o);
bool omm_obj_is_strong(struct Object *o);
bool omm_obj_is_destructible(struct Object *o);
bool omm_obj_is_breakable(struct Object *o);
bool omm_obj_is_boss(struct Object *o);
bool omm_obj_is_invulnerable(struct Object *o);
bool omm_obj_is_grabbable(struct Object *o);
bool omm_obj_is_goomba(struct Object *o);
bool omm_obj_is_bully(struct Object *o);
bool omm_obj_is_flame(struct Object *o);
bool omm_obj_is_intangible_to_cappy(struct Object *o);
bool omm_obj_is_kickable_board(struct Object *o);
bool omm_obj_is_wf_star_wall(struct Object *o);
bool omm_obj_is_exclamation_box(struct Object *o);
bool omm_obj_is_unagis_tail(struct Object *o);
bool omm_obj_is_collectible(struct Object *o);
bool omm_obj_is_goomba_stack(struct Object *o);
bool omm_obj_check_interaction(struct Object *o, struct MarioState *m, bool ignoreTangibility);
bool omm_obj_process_one_surface_interaction(struct Object *o, struct Object *target, u32 interactionFlags);
bool omm_obj_process_one_object_interaction(struct Object *o, struct Object *target, u32 interactionFlags);
struct Object *omm_obj_process_interactions(struct Object *o, u32 interactionFlags);

//
// Hold (possessed Motos/Chuckya)
//

bool omm_obj_is_holdable(struct Object *o);
bool omm_obj_hold(struct Object *o);
bool omm_obj_update_held_object(struct Object *o, struct Object *holder, f32 xOffset, f32 yOffset, f32 zOffset);
bool omm_obj_throw(struct Object *o, f32 forwardVel, f32 yVel);

//
// Object behaviors
//

OmmArray omm_obj_get_coin_behaviors();
OmmArray omm_obj_get_star_or_key_behaviors();
OmmArray omm_obj_get_star_model_behaviors();
OmmArray omm_obj_get_cap_behaviors();
OmmArray omm_obj_get_goomba_behaviors();
OmmArray omm_obj_get_player_behaviors();
OmmArray omm_obj_get_bowser_behaviors();
OmmArray omm_obj_get_holdable_behaviors();

//
// Spawners
//

struct Object *omm_spawn_damage_mario(struct Object *o, s32 interactType, s32 damage);
struct Object *omm_spawn_star_celebration(struct Object *o, f32 radius, f32 height);
struct Object *omm_spawn_life_up(struct Object *o);
struct Object *omm_spawn_number(struct Object *o, s32 n);
struct Object *omm_spawn_star_number(struct Object *o);
struct Object *omm_spawn_fire_smoke(struct Object *o, s32 type);
struct Object *omm_spawn_wing_glow_and_trail(struct Object *o);
struct Object *omm_spawn_metal_sparkle(struct Object *o);
struct Object *omm_spawn_vanish_mist(struct Object *o);
struct Object *omm_spawn_sparkle(struct Object *o, u8 r, u8 g, u8 b, f32 translationRange, f32 minScale, f32 maxScale);
struct Object *omm_spawn_break_particle(struct Object *o, u8 r, u8 g, u8 b, f32 offsetY, f32 velMin, f32 velMax, f32 velY, f32 scaleMin, f32 scaleMax);
struct Object *omm_spawn_snowball(struct Object *o);
struct Object *omm_spawn_mr_i_beam(struct Object *o, f32 power);
struct Object *omm_spawn_snufit_ball(struct Object *o, s32 delay, bool strong);
struct Object *omm_spawn_rock(struct Object *o);
struct Object *omm_spawn_explosion(struct Object *o);
struct Object *omm_spawn_blargg_fire_ball(struct Object *o);
struct Object *omm_spawn_mips(struct Object *o, f32 x, f32 y, f32 z, f32 fVel);
struct Object *omm_spawn_shockwave_whomp(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_spawn_shockwave_spindrift(struct Object *o);
struct Object *omm_spawn_shockwave_fire(struct Object *o, f32 radius, f32 width, f32 height, f32 speed, f32 distMax, const void *textureWave, const void *textureFire);
struct Object *omm_spawn_rising_lava(struct Object *o, f32 x, f32 y, f32 z, f32 yMin, f32 yMax, f32 yVel, f32 radius, f32 rotVel, s32 shakeEnv);
struct Object *omm_spawn_star_ring(struct Object *o, f32 x, f32 y, f32 z, bool vertical, s32 yaw);
struct Object *omm_spawn_bitfs_pillar(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_spawn_bowser_mine(struct Object *o, f32 x, f32 y, f32 z, s16 yaw);
struct Object *omm_spawn_bowser_flame(struct Object *o, f32 x, f32 y, f32 z, s32 duration);
struct Object *omm_spawn_bowser_fireball(struct Object *o, f32 x, f32 y, f32 z, f32 forwardVel, f32 maxDistance, s16 angle);
struct Object *omm_spawn_bowser_fireball_flame(struct Object *o, s32 duration);
struct Object *omm_spawn_flaming_bobomb(struct Object *o, f32 x, f32 y, f32 z, s32 index, s32 count, f32 maxRadius, f32 maxHeight);
struct Object *omm_spawn_flaming_bobomb_explosion(struct Object *o);
struct Object *omm_spawn_sparkly_star(struct Object *o, s32 mode, f32 x, f32 y, f32 z, bool isCondStar);
struct Object *omm_spawn_sparkly_star_hint(struct Object *o, s32 mode, f32 x, f32 y, f32 z, s16 yaw, s32 dialogId);
struct Object *omm_spawn_sparkly_star_block(struct Object *o, s32 mode, f32 x, f32 y, f32 z);
struct Object *omm_spawn_sparkly_star_sparkle(struct Object *o, s32 mode, f32 yOffset, f32 vel, f32 scale, f32 offset);
struct Object *omm_spawn_sparkly_star_sparkle_mario(struct Object *o, s32 mode, f32 yOffset, f32 vel, f32 scale, f32 offset);
struct Object *omm_spawn_sparkly_star_celebration(struct Object *o, s32 mode);
struct Object *omm_spawn_perry_shockwave(struct Object *o, s32 delay, s32 type, bool clockwise);
struct Object *omm_spawn_perry_blast(struct Object *o, s32 type);
struct Object *omm_spawn_peach_vibe_aura(struct Object *o);
struct Object *omm_spawn_peach_vibe_sparkle(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_spawn_peach_vibe_joy_tornado(struct Object *o);
struct Object *omm_spawn_peach_vibe_joy_sparkle(struct Object *o, f32 radius, f32 height, s16 yaw, s32 yawVel, u8 opacity);
struct Object *omm_spawn_peach_vibe_joy_gust(struct Object *o, f32 fvel, s16 angle);
struct Object *omm_spawn_peach_vibe_rage_aura(struct Object *o);
struct Object *omm_spawn_peach_vibe_rage_shockwave(struct Object *o);
struct Object *omm_spawn_peach_vibe_gloom_aura(struct Object *o);
struct Object *omm_spawn_peach_vibe_gloom_tear(struct Object *o);
struct Object *omm_spawn_peach_vibe_gloom_tear_small(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_spawn_peach_vibe_calm_aura(struct Object *o);
struct Object *omm_spawn_peach_vibe_calm_sparkle(struct Object *o);
struct Object *omm_spawn_problem(struct Object *o);

//
// World
//

void omm_world_update(struct MarioState *m);
bool omm_world_is_frozen();
bool omm_world_is_flooded();
bool omm_world_is_vanished();

//
// Stars
//

u8 omm_stars_get_bits();
u8 omm_stars_get_bits_total(s32 level);
u32 omm_stars_get_color(s32 level);
bool omm_stars_is_collected(s32 index);
bool omm_stars_all_collected(s32 level);
void omm_stars_set_bits(u8 bits);
#define OMM_ALL_STARS (omm_stars_all_collected(gCurrLevelNum) && !OMM_SSM_IS_LUNATIC)

#endif // OMM_OBJECT_H
