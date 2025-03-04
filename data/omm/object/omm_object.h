#ifndef OMM_OBJECT_H
#define OMM_OBJECT_H

#include "types.h"

//
// For loops
//

extern s32 gOmmCappyObjectLists[];
extern s32 gOmmInteractionObjectLists[];
extern s32 gOmmUnimportantObjectLists[];
extern s32 gOmmAllObjectLists[];

#define for_each_(_T_, _item_, _start_, _size_)             u32 i_##_item_ = 0; for (_T_ *_item_ = _start_; i_##_item_ != _size_; ++i_##_item_, _item_++)
#define for_each_in_(_T_, _item_, ...)                      _T_ arr_##_item_[] = __VA_ARGS__; u32 i_##_item_ = 0; for (_T_ *_item_ = arr_##_item_; i_##_item_ != array_length(arr_##_item_); ++i_##_item_, _item_++)
#define for_each_until_null(_T_, _item_, _start_)           for (_T_ *_item_ = _start_; *_item_; _item_++)
#define for_each_object_in_list(_obj_, _list_)              for (struct Object* _obj_ = obj_get_first(_list_); _obj_; _obj_ = obj_get_next(_obj_, _list_))
#define for_each_object_in_cappy_lists(_obj_)               for (s32 *_list_ = gOmmCappyObjectLists; *_list_ != -1; ++_list_) for (struct Object* _obj_ = obj_get_first(*_list_); _obj_; _obj_ = obj_get_next(_obj_, *_list_))
#define for_each_object_in_interaction_lists(_obj_)         for (s32 *_list_ = gOmmInteractionObjectLists; *_list_ != -1; ++_list_) for (struct Object* _obj_ = obj_get_first(*_list_); _obj_; _obj_ = obj_get_next(_obj_, *_list_))
#define for_each_object_in_unimportant_lists(_obj_)         for (s32 *_list_ = gOmmUnimportantObjectLists; *_list_ != -1; ++_list_) for (struct Object* _obj_ = obj_get_first(*_list_); _obj_; _obj_ = obj_get_next(_obj_, *_list_))
#define for_each_object_in_all_lists(_obj_)                 for (s32 *_list_ = gOmmAllObjectLists; *_list_ != -1; ++_list_) for (struct Object* _obj_ = obj_get_first(*_list_); _obj_; _obj_ = obj_get_next(_obj_, *_list_))
#define for_each_object_with_behavior(_obj_, _bhv_)         for (struct Object* _obj_ = obj_get_first_with_behavior(_bhv_); _obj_; _obj_ = obj_get_next_with_behavior(_obj_, _bhv_))

//
// Objects
//

struct Object *obj_spawn_from_geo(struct Object *parent, const GeoLayout *geoLayout, const BehaviorScript *behavior);
struct Object *obj_get_first(s32 list);
struct Object *obj_get_next(struct Object *o, s32 list);
struct Object *obj_get_first_with_behavior(const BehaviorScript *behavior);
struct Object *obj_get_first_with_behavior_and_parent(struct Object *parent, const BehaviorScript *behavior);
struct Object *obj_get_next_with_behavior(struct Object *o, const BehaviorScript *behavior);
struct Object *obj_get_first_with_behavior_and_field_s32(const BehaviorScript *behavior, s32 fieldIndex, s32 value);
struct Object *obj_get_first_with_behavior_and_field_f32(const BehaviorScript *behavior, s32 fieldIndex, f32 value);
struct Object *obj_get_nearest_with_behavior(struct Object *o, const BehaviorScript *behavior);
struct Object *obj_get_nearest_with_behavior_and_radius(struct Object *o, const BehaviorScript *behavior, f32 distMax);
s32            obj_get_slot_index(struct Object *o);
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
f32  obj_get_distance_vec3f(struct Object *o, Vec3f pos);
bool obj_has_model(struct Object *o, s32 modelId);
bool obj_has_geo_layout(struct Object *o, const GeoLayout *georef);
bool obj_has_graph_node(struct Object *o, struct GraphNode *node);
bool obj_is_surface(struct Object *o);
bool obj_is_on_ground(struct Object *o);
bool obj_is_underwater(struct Object *o, f32 waterLevel);
void obj_set_underwater(struct Object *o, f32 waterLevel);
s32  obj_get_room(struct Object *o);
s32  obj_get_object1_angle_yaw_to_object2(struct Object *o1, struct Object *o2);
bool obj_is_object1_facing_object2(struct Object *o1, struct Object *o2, s16 angleRange);
bool obj_is_object2_hit_from_above(struct Object *o1, struct Object *o2);
bool obj_is_object2_hit_from_below(struct Object *o1, struct Object *o2);
bool obj_has_hitbox(struct Object *o, u32 flags);
bool obj_detect_hitbox_overlap(struct Object *o1, struct Object *o2, u32 obj1Flags, u32 obj2Flags);
f32 *obj_pos_as_vec3f(struct Object *o, Vec3f pos);
f32 *obj_vel_as_vec3f(struct Object *o, Vec3f vel);
s16 *obj_angle_as_vec3s(struct Object *o, Vec3s angle);
f32 *obj_scale_as_vec3f(struct Object *o, Vec3f scale);
void obj_set_pos_vec3f(struct Object *o, Vec3f pos);
void obj_set_angle_vec3s(struct Object *o, Vec3s angle);
void obj_set_scale_vec3f(struct Object *o, Vec3f scale);
void obj_set_xyz(struct Object *o, f32 x, f32 y, f32 z);
void obj_set_home(struct Object *o, f32 x, f32 y, f32 z);
void obj_set_vel(struct Object *o, f32 x, f32 y, f32 z);
void obj_set_throw_vel(struct Object *o, struct Object *thrower, f32 forwardVel, f32 yVel);
void obj_set_forward_vel(struct Object *o, s16 yaw, f32 mag, f32 velMax);
void obj_set_forward_and_y_vel(struct Object *o, f32 forwardVel, f32 yVel);
void obj_set_angle_vel(struct Object *o, s16 pitch, s16 yaw, s16 roll);
void obj_set_scale(struct Object *o, f32 x, f32 y, f32 z);
void obj_apply_drag_xz(struct Object *o, f32 dragStrength);
void obj_apply_displacement(struct Object *o, struct Object *obj, Vec3f prevPos, Vec3s prevAngle, Vec3f prevScale, bool updatePrev);
void obj_approach_orbit(struct Object *o, f32 t, f32 radius, f32 period, f32 offset, f32 rubberband, s16 pitchVel, s16 yawVel);
f32  obj_bounce_on_wall(struct Object *o, struct Surface *surf, bool updateForwardVel, bool updateFaceYaw);
void obj_safe_step(struct Object *o, bool updated);
void obj_set_interact_id(struct Object *o, struct Object *child);
void obj_update_billboard(struct Object *o);
void obj_rotate_billboard(struct Object *o, s16 angle);
void obj_copy_visibility_and_transparency(struct Object *dst, struct Object *src);
void obj_update_blink_state(struct Object *o, s32 *timer, s16 base, s16 range, s16 length);
void obj_random_blink(struct Object *o, s32 *timer);
void obj_produce_water_effects(struct Object *o, bool isOnGround, bool isUnderwater, bool wasUnderwater, f32 waterLevel);
bool obj_make_step_sound_and_particle(struct Object *o, f32 *dist, f32 distMin, f32 distInc, s32 soundBits, u32 particles);
void obj_play_sound(struct Object *o, s32 soundBits);
void obj_create_sound_spawner(struct Object *o, s32 soundBits);
s32  obj_get_model_id(struct Object *o);
s32  obj_get_coin_type(struct Object *o);
void obj_collect_coins(struct Object *o, s32 numCoins);
void obj_spawn_coins(struct Object *o, s32 numCoins);
void obj_spawn_star_no_cutscene(struct Object *o, f32 x, f32 y, f32 z, s32 starIndex, bool noExit);
void obj_spawn_star_with_cutscene(struct Object *o, f32 xFrom, f32 yFrom, f32 zFrom, f32 xTo, f32 yTo, f32 zTo, s32 starIndex, bool noExit, bool redCoinStarCutscene);
void obj_spawn_white_puff(struct Object *o, s32 soundBits);
void obj_spawn_white_puff_at(f32 x, f32 y, f32 z, s32 soundBits);
void obj_spawn_triangle_break_particles(struct Object *o, s32 count, s32 model, f32 size, s32 type);
void obj_spawn_break_particles(struct Object *o, s32 count, u8 r, u8 g, u8 b, f32 offsetY, f32 velMin, f32 velMax, f32 velY, f32 sizeMin, f32 sizeMax);
void obj_spawn_particles(struct Object *o, s32 count, s32 model, f32 yOffset, f32 forwardVelBase, f32 forwardVelRange, f32 yVelBase, f32 yVelRange, f32 gravity, f32 scaleBase, f32 scaleRange);
void obj_spawn_particle_preset(struct Object *o, u32 particle, bool setActiveParticle);
void obj_destroy(struct Object *o);
void obj_create_respawner(struct Object *o, s32 model, const BehaviorScript *behavior, f32 minSpawnDist);
void obj_update_gfx(struct Object *o);
void obj_update_held_mario(struct Object *o, f32 forwardVel, f32 yVel);
void obj_set_params(struct Object *o, s32 interactType, s32 damageOrCoinValue, s32 health, s32 numLootCoins, bool setTangible);
void obj_reset_hitbox(struct Object *o, f32 hitboxRadius, f32 hitboxHeight, f32 hurtboxRadius, f32 hurtboxHeight, f32 wallHitboxRadius, f32 hitboxDownOffset);
bool obj_fix_hitbox(struct Object *o);
bool obj_path_check_failsafe(struct Object *o);
void obj_drop_to_floor(struct Object *o);
void obj_load_collision_model(struct Object *o);
bool obj_is_always_rendered(struct Object *o);
void obj_set_always_rendered(struct Object *o, bool set);
bool obj_is_dormant(struct Object *o);
void obj_set_dormant(struct Object *o, bool set);
bool obj_dialog_start(s16 dialogId);
bool obj_dialog_update();
bool obj_cutscene_start(u8 cutsceneId, struct Object *o);
bool obj_cutscene_update();
struct Waypoint *obj_path_get_nearest_waypoint(struct Object *o, struct Waypoint *list, f32 radiusMax);
struct Object *find_unimportant_object();
struct Object *obj_get_red_coin_star();
struct Object *obj_get_secret_star();
#define obj_get_model obj_get_model_id

//
// Types and interactions
//

bool omm_obj_is_interactable(struct Object *o);
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
bool omm_obj_is_bully(struct Object *o);
bool omm_obj_is_flame(struct Object *o);
bool omm_obj_is_intangible_to_cappy(struct Object *o);
bool omm_obj_is_kickable_board(struct Object *o);
bool omm_obj_is_wf_star_wall(struct Object *o);
bool omm_obj_is_exclamation_box(struct Object *o);
bool omm_obj_is_unagis_tail(struct Object *o);
bool omm_obj_is_water_diamond(struct Object *o);
bool omm_obj_is_treasure_chest(struct Object *o);
bool omm_obj_is_collectible(struct Object *o);
bool omm_obj_is_unimportant(struct Object *o);
bool omm_obj_is_no_cam_col(struct Object *o);
bool omm_obj_is_goomba(struct Object *o);
bool omm_obj_is_koopa_the_quick(struct Object *o);
bool omm_obj_is_boo(struct Object *o);
bool omm_obj_is_metal_ball(struct Object *o);
bool omm_obj_is_door(struct Object *o);
bool omm_obj_is_metal_destructible(struct Object *o);
bool omm_obj_is_enemy(struct Object *o);
bool omm_obj_is_enemy_defeated(struct Object *o);
bool omm_obj_is_goomba_stack(struct Object *o);
bool omm_obj_is_playable_yoshi(struct Object *o);
bool omm_obj_is_yoshi_tongue(struct Object *o);
bool omm_obj_check_interaction(struct Object *o, struct MarioState *m, bool ignoreTangibility);
bool omm_obj_interact_treasure_chest(struct Object *o, struct Object *chest);
s32  omm_obj_process_one_surface_interaction(struct Object *o, struct Object *target, u32 interactionFlags);
s32  omm_obj_process_one_object_interaction(struct Object *o, struct Object *target, u32 interactionFlags);
struct Object *omm_obj_process_interactions(struct Object *o, u32 interactionFlags);

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
OmmArray omm_obj_get_sparkly_enemy_behaviors();

//
// Spawners
//

// Common
struct Object *omm_obj_spawn_orange_number(struct Object *o, s32 num, f32 offsetY, s32 soundBits, s32 variation, const GeoLayout *geoLayout);
struct Object *omm_obj_spawn_blue_coins_number(struct Object *o);
struct Object *omm_obj_spawn_star_number(struct Object *o);
struct Object *omm_obj_spawn_life_up(struct Object *o);
struct Object *omm_obj_spawn_star_celebration(struct Object *o, f32 radius, f32 height, const BehaviorScript *starBehavior);
struct Object *omm_obj_spawn_wf_transition(struct Object *o);
struct Object *omm_obj_spawn_wall_warp(
    struct Object *o, s32 kind, f32 centerX, f32 centerY, f32 centerZ, f32 halfWidth, f32 halfHeight, s16 yaw,
    s16 destLevel, s16 destArea, s16 destNode, s16 warpDuration, s16 transitionType, s16 transitionDuration, u32 transitionColor,
    f32 scaleX, f32 scaleY, f32 scaleZ, const GeoLayout *wallGeo, const GeoLayout *sparkleGeo
);

// Particles
struct Object *omm_obj_spawn_fire_smoke(struct Object *o, s32 type);
struct Object *omm_obj_spawn_sparkle(struct Object *o, u8 r, u8 g, u8 b, f32 translationRange, f32 minScale, f32 maxScale);
struct Object *omm_obj_spawn_break_particle(struct Object *o, u8 r, u8 g, u8 b, f32 offsetY, f32 velMin, f32 velMax, f32 velY, f32 scaleMin, f32 scaleMax);
struct Object *omm_obj_spawn_whirlpool(struct Object *o);

// Mario
struct Object *omm_obj_spawn_damage_mario(struct Object *o, s32 interactType, s32 damage);
struct Object *omm_obj_spawn_wing_glow_and_trail(struct Object *o);
struct Object *omm_obj_spawn_metal_sparkle(struct Object *o);
struct Object *omm_obj_spawn_vanish_mist(struct Object *o);
struct Object *omm_obj_spawn_stats_board(struct Object *o, f32 x, f32 y, f32 z, s16 yaw);

// Captures
struct Object *omm_obj_spawn_explosion(struct Object *o, f32 offsetY, f32 scaleMin, f32 scaleMax, s32 duration, const GeoLayout *geoLayout);
struct Object *omm_obj_spawn_whomp_shockwave(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_obj_spawn_snowball(struct Object *o);
struct Object *omm_obj_spawn_spindrift_shockwave(struct Object *o);
struct Object *omm_obj_spawn_mr_i_beam(struct Object *o, f32 power);
struct Object *omm_obj_spawn_snufit_ball(struct Object *o, s32 delay, bool strong);
struct Object *omm_obj_spawn_monty_mole_rock(struct Object *o, f32 power);
struct Object *omm_obj_spawn_blargg_fireball(struct Object *o);
struct Object *omm_obj_spawn_fly_guy_fireball(struct Object *o);
struct Object *omm_obj_spawn_pokey_ball(struct Object *o);
struct Object *omm_obj_spawn_shockwave(struct Object *o, f32 scaleMin, f32 scaleMax, u8 alphaMin, u8 alphaMax, s32 duration);
struct Object *omm_obj_spawn_amp_discharge(struct Object *o, f32 power);
struct Object *omm_obj_spawn_fire_spitter_flame(struct Object *o, bool flamethrower);
struct Object *omm_obj_spawn_moneybag_coin(struct Object *o, f32 power);
struct Object *omm_obj_spawn_spiny_ball(struct Object *o);
struct Object *omm_obj_spawn_afterimage(struct Object *o, f32 scaleMin, f32 scaleMax, u8 alphaMin, u8 alphaMax, s32 duration);
struct Object *omm_obj_spawn_flaming_bobomb(struct Object *o, f32 x, f32 y, f32 z, s32 index, s32 count, f32 maxRadius, f32 maxHeight);
struct Object *omm_obj_spawn_flaming_bobomb_explosion(struct Object *o);
struct Object *omm_obj_spawn_yoshi(struct Object *o, s16 faceAngleYaw);
struct Object *omm_obj_spawn_yoshi_bubble(struct Object *o);
struct Object *omm_obj_spawn_yoshi_fire(struct Object *o);
struct Object *omm_obj_spawn_yoshi_fireball(struct Object *o);
struct Object *omm_obj_spawn_yoshi_egg(struct Object *o, s32 index);
struct Object *omm_obj_spawn_yoshi_tongue(struct Object *o);
struct Object *omm_obj_spawn_yoshi_wings(struct Object *o);
struct Object *omm_obj_spawn_yoshi_mode_yoshi(struct Object *o);

// Bowser
struct Object *omm_obj_spawn_bowser_mine(struct Object *o, f32 x, f32 y, f32 z, s16 yaw);
struct Object *omm_obj_spawn_bowser_flame(struct Object *o, f32 x, f32 y, f32 z, s32 duration);
struct Object *omm_obj_spawn_bowser_fireball(struct Object *o, f32 x, f32 y, f32 z, f32 forwardVel, f32 maxDistance, s16 angle);
struct Object *omm_obj_spawn_bowser_fireball_flame(struct Object *o, s32 duration);
struct Object *omm_obj_spawn_bowser_shockwave_fire(struct Object *o, f32 radius, f32 width, f32 height, f32 speed, f32 distMax, const void *textureWave, const void *textureFire);

// Sparkly Stars
struct Object *omm_obj_spawn_sparkly_star(struct Object *o, s32 sparklyMode, f32 x, f32 y, f32 z, bool isCondStar);
struct Object *omm_obj_spawn_sparkly_star_hint(struct Object *o, s32 sparklyMode, f32 x, f32 y, f32 z, s16 yaw, s32 dialogId);
struct Object *omm_obj_spawn_sparkly_star_block(struct Object *o, s32 sparklyMode, f32 x, f32 y, f32 z);
struct Object *omm_obj_spawn_sparkly_star_sparkle(struct Object *o, s32 sparklyMode, f32 yOffset, f32 vel, f32 scale, f32 offset);
struct Object *omm_obj_spawn_sparkly_star_sparkle_star_spawn(struct Object *o, s32 sparklyMode, s32 duration, f32 scale, f32 offset);
struct Object *omm_obj_spawn_sparkly_star_sparkle_mario(struct Object *o, s32 sparklyMode, f32 yOffset, f32 vel, f32 scale, f32 offset);
struct Object *omm_obj_spawn_sparkly_star_celebration(struct Object *o, s32 sparklyMode);
struct Object *omm_obj_spawn_sparkly_star_box(struct Object *o, s32 sparklyMode, f32 x, f32 y, f32 z, f32 maxHeight);
struct Object *omm_obj_spawn_mips(struct Object *o, f32 x, f32 y, f32 z, f32 fVel);
struct Object *omm_obj_spawn_rising_lava(struct Object *o, f32 x, f32 y, f32 z, f32 yMin, f32 yMax, f32 yVel, f32 radius, f32 rotVel, s32 shakeEnv);
struct Object *omm_obj_spawn_star_ring(struct Object *o, f32 x, f32 y, f32 z, bool vertical, s32 yaw);
struct Object *omm_obj_spawn_big_flame(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_obj_spawn_bitfs_pillar(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_obj_spawn_problem(struct Object *o);
struct Object *omm_obj_spawn_peach(struct Object *o);

// Peach
struct Object *omm_obj_spawn_peach_vibe_aura(struct Object *o, bool full);
struct Object *omm_obj_spawn_peach_vibe_sparkle(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_obj_spawn_peach_vibe_joy_tornado(struct Object *o);
struct Object *omm_obj_spawn_peach_vibe_joy_sparkle(struct Object *o, f32 radius, f32 height, s16 yaw, s32 yawVel, u8 opacity);
struct Object *omm_obj_spawn_peach_vibe_joy_gust(struct Object *o, f32 fvel, s16 angle);
struct Object *omm_obj_spawn_peach_vibe_rage_aura(struct Object *o);
struct Object *omm_obj_spawn_peach_vibe_rage_shockwave(struct Object *o);
struct Object *omm_obj_spawn_peach_vibe_gloom_aura(struct Object *o);
struct Object *omm_obj_spawn_peach_vibe_gloom_tear(struct Object *o);
struct Object *omm_obj_spawn_peach_vibe_gloom_tear_small(struct Object *o, f32 x, f32 y, f32 z);
struct Object *omm_obj_spawn_peach_vibe_calm_aura(struct Object *o);
struct Object *omm_obj_spawn_peach_vibe_calm_sparkle(struct Object *o);
struct Object *omm_obj_spawn_perry_shockwave(struct Object *o, s32 delay, s32 type, bool clockwise);
struct Object *omm_obj_spawn_perry_blast(struct Object *o, s32 type);

//
// World
//

void omm_world_update(struct MarioState *m);
bool omm_world_is_cold();
bool omm_world_is_frozen();
bool omm_world_is_flooded();
bool omm_world_is_dark();
bool omm_bowser_is_active();

//
// Stars
//

u8   omm_stars_get_flags();
u8   omm_stars_get_level_flags(s32 levelNum, s32 modeIndex);
u8   omm_stars_get_course_flags(s32 courseNum, s32 modeIndex);
s32  omm_stars_get_total_star_count(s32 modeIndex);
u32  omm_stars_get_color(s32 levelNum, s32 modeIndex);
void omm_stars_set_flags(u8 starFlags);
bool omm_stars_is_collected(s32 starIndex);
bool omm_stars_all_collected(s32 levelNum, s32 modeIndex);
bool omm_stars_get_star_data(s32 levelNum, s32 areaIndex, s32 starIndex, const BehaviorScript **bhv, s32 *behParams);
#define OMM_ALL_STARS omm_stars_all_collected(gCurrLevelNum, OMM_GAME_MODE)

#endif // OMM_OBJECT_H
