#ifndef OMM_OBJECT_DATA_H
#define OMM_OBJECT_DATA_H

typedef uintptr_t GeoLayout;
typedef uintptr_t BehaviorScript;
typedef uintptr_t LevelScript;
typedef s16 Collision;

//
// Flags
//

// Notes:
// - Object types are sorted by their update order
// - Spawner and Surface objects are updated way before other objects
// - Unimportant objects are not affected by time stop
#define OBJ_TYPE_SPAWNER                                    (OBJ_LIST_SPAWNER << 16)        // No collision check
#define OBJ_TYPE_SURFACE                                    (OBJ_LIST_SURFACE << 16)        // Collision check with OBJ_LIST_PLAYER, OBJ_LIST_DESTRUCTIVE
#define OBJ_TYPE_POLELIKE                                   (OBJ_LIST_POLELIKE << 16)       // Collision check with OBJ_LIST_PLAYER
#define OBJ_TYPE_PLAYER                                     (OBJ_LIST_PLAYER << 16)         // Collision check with OBJ_LIST_PLAYER, OBJ_LIST_POLELIKE, OBJ_LIST_LEVEL, OBJ_LIST_GENACTOR, OBJ_LIST_PUSHABLE, OBJ_LIST_SURFACE, OBJ_LIST_DESTRUCTIVE
#define OBJ_TYPE_PUSHABLE                                   (OBJ_LIST_PUSHABLE << 16)       // Collision check with OBJ_LIST_PUSHABLE
#define OBJ_TYPE_GENACTOR                                   (OBJ_LIST_GENACTOR << 16)       // Collision check with OBJ_LIST_PLAYER, OBJ_LIST_DESTRUCTIVE
#define OBJ_TYPE_DESTRUCTIVE                                (OBJ_LIST_DESTRUCTIVE << 16)    // Collision check with OBJ_LIST_PLAYER, OBJ_LIST_DESTRUCTIVE
#define OBJ_TYPE_LEVEL                                      (OBJ_LIST_LEVEL << 16)          // Collision check with OBJ_LIST_PLAYER
#define OBJ_TYPE_SPECIAL                                    (OBJ_LIST_LEVEL << 16)
#define OBJ_TYPE_DEFAULT                                    (OBJ_LIST_DEFAULT << 16)        // No collision check
#define OBJ_TYPE_UNIMPORTANT                                (OBJ_LIST_UNIMPORTANT << 16)    // No collision check

#define OBJ_FLOOR_TYPE_NONE                                 0
#define OBJ_FLOOR_TYPE_GROUND                               1
#define OBJ_FLOOR_TYPE_WATER                                2
#define OBJ_FLOOR_TYPE_LAVA                                 3

#define OBJ_OVERLAP_FLAG_HITBOX                             (1 << 0)
#define OBJ_OVERLAP_FLAG_HURTBOX_HITBOX_IF_ZERO             (1 << 1)
#define OBJ_OVERLAP_FLAG_HITBOX_HURTBOX_MAX                 (1 << 2)
#define OBJ_OVERLAP_FLAG_IGNORE_INTANGIBLE                  (1 << 3)

#define OBJ_STEP_UPDATE_HOME                                (1 << 0)
#define OBJ_STEP_MOVE_THROUGH_WALLS                         (1 << 1)
#define OBJ_STEP_STICKY_FEET                                (1 << 2)
#define OBJ_STEP_CHECK_ON_GROUND                            (1 << 3)

#define OBJ_PARTICLE_NONE                                   (0 << 0)
#define OBJ_PARTICLE_MIST                                   (1 << 0)
#define OBJ_PARTICLE_SMOKE                                  (1 << 1)
#define OBJ_PARTICLE_WATER_TRAIL                            (1 << 2)
#define OBJ_PARTICLE_WATER_DROPLET                          (1 << 3)
#define OBJ_PARTICLE_FIRE                                   (1 << 4)
#define OBJ_PARTICLE_FLAME                                  (1 << 5)

#define OBJ_SPAWN_TRI_BREAK_PRESET_COINS                    20, MODEL_YELLOW_COIN,    1.0f, 0
#define OBJ_SPAWN_TRI_BREAK_PRESET_DIRT                     20, MODEL_DIRT_ANIMATION, 0.7f, 3
#define OBJ_SPAWN_TRI_BREAK_PRESET_DIRT_SMALL               20, MODEL_DIRT_ANIMATION, 0.3f, 3
#define OBJ_SPAWN_TRI_BREAK_PRESET_BOX_RED                  20, MODEL_CARTOON_STAR,   0.3f, 0
#define OBJ_SPAWN_TRI_BREAK_PRESET_BOX_GREEN                20, MODEL_CARTOON_STAR,   0.3f, 1
#define OBJ_SPAWN_TRI_BREAK_PRESET_BOX_BLUE                 20, MODEL_CARTOON_STAR,   0.3f, 2
#define OBJ_SPAWN_TRI_BREAK_PRESET_BOX_YELLOW               20, MODEL_CARTOON_STAR,   0.3f, 3
#define OBJ_SPAWN_TRI_BREAK_PRESET_SWITCH_RED               60, MODEL_CARTOON_STAR,   0.3f, 0
#define OBJ_SPAWN_TRI_BREAK_PRESET_SWITCH_GREEN             60, MODEL_CARTOON_STAR,   0.3f, 1
#define OBJ_SPAWN_TRI_BREAK_PRESET_SWITCH_BLUE              60, MODEL_CARTOON_STAR,   0.3f, 2
#define OBJ_SPAWN_TRI_BREAK_PRESET_SWITCH_YELLOW            60, MODEL_CARTOON_STAR,   0.3f, 3
#define OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_20             20, MODEL_DIRT_ANIMATION, 3.0f, 4
#define OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_30             30, MODEL_DIRT_ANIMATION, 3.0f, 4

#define OBJ_INT_ATTACK_WEAK                                 (1 << 0) // Common attacks
#define OBJ_INT_ATTACK_STRONG                               (1 << 1) // Strong attacks, such as charged snowballs or beams
#define OBJ_INT_ATTACK_ONE_HIT                              (1 << 2) // Powerful attacks, defeat bosses in one hit
#define OBJ_INT_ATTACK_DESTRUCTIBLE                         (1 << 3) // Destructive attacks, such as explosions
#define OBJ_INT_ATTACK_BREAKABLE                            (1 << 4) // Fragile/Breakable objects, such as cork boxes
#define OBJ_INT_ATTACK_FLAMES                               (1 << 5) // Flame-type objects
#define OBJ_INT_ATTACK_KNOCKBACK                            (1 << 6) // Turn attacks into knockbacks (only for some enemies)
#define OBJ_INT_COLLECT_COINS                               (1 << 7) // Coins
#define OBJ_INT_COLLECT_TRIGGERS                            (1 << 8) // Water rings, secrets, 1up-mushrooms and invisible triggers
#define OBJ_INT_COLLECT_STARS                               (1 << 9) // Stars and keys
#define OBJ_INT_COLLECT_CAPS                                (1 << 10) // Caps
#define OBJ_INT_GRAB_OBJECTS                                (1 << 11) // Perform a grab instead of attacking
#define OBJ_INT_HEAL_MARIO                                  (1 << 12) // Heal Mario after attacking/destroying an enemy
#define OBJ_INT_NOT_INTERACTED                              (1 << 13) // omm_obj_process_interactions() always returns NULL
#define OBJ_INT_ATTRACT_COINS_WEAK                          (1 << 14) // Attract nearby coins (weak pull)
#define OBJ_INT_ATTRACT_COINS_STRONG                        (1 << 15) // Attract nearby coins (strong pull)
#define OBJ_INT_ATTRACT_COINS_RANGE(x)                      ((((u32) (x / 100)) & 0xF) << 16) // Attraction range (4 bits, values from 0 to 1500)
#define OBJ_INT_ATTRACT_COINS_RANGE_GET(intFlags)           (((u32) ((intFlags >> 16) & 0xF)) * 100)
#define OBJ_INT_PERRY_ATTACK                                (1 << 24)
#define OBJ_INT_PERRY_SWORD                                 (1 << 25)
#define OBJ_INT_PERRY_TRAIL                                 (1 << 26)

#define OBJ_INT_PRESET_ATTACK_SURFACE                       (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE)
#define OBJ_INT_PRESET_CAPPY                                (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_GRAB_OBJECTS | (OBJ_INT_COLLECT_STARS * OMM_CHEAT_CAPPY_CAN_COLLECT_STARS))
#define OBJ_INT_PRESET_EXPLOSION                            (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_DESTRUCTIBLE)
#define OBJ_INT_PRESET_SHOCKWAVE                            (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_ATTACK_AND_COLLECT                   (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_COLLECT_STARS)
#define OBJ_INT_PRESET_SNOWBALL_SMALL                       (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_SNOWBALL_LARGE                       (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_BEAM_SMALL                           (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_BEAM_LARGE                           (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_SNUFIT_BALL_SMALL                    (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_SNUFIT_BALL_LARGE                    (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_ROCK                                 (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_BLARGG_FIRE                          (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)

#define OBJ_INT_PRESET_PEACH_VIBE_JOY_TORNADO               (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_KNOCKBACK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_WEAK | OBJ_INT_ATTRACT_COINS_RANGE(900) | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_JOY_GUST                  (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_STRONG | OBJ_INT_ATTRACT_COINS_RANGE(600) | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_GLOOM_AURA                (OBJ_INT_ATTACK_FLAMES | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_RAGE_AURA                 (OBJ_INT_ATTACK_WEAK | (OBJ_INT_ATTACK_ONE_HIT * omm_mario_is_ground_pounding(gMarioState)) | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_RAGE_GROUND_POUND_LAND    (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_ONE_HIT | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_RAGE_SHOCKWAVE            (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_PERRY_ATTACK)

#define OBJ_GOOMBA_STACK_MAX                                10

//
// Gfx/Geo macros
//

#define OMM_VTX_RECT(size, fleft, ftop, fmax)               { { { -size, -size, 0 }, 0, { (512 * 32 * (fleft + 0)) / fmax, (512 * 32 * (ftop + 1)) / fmax }, { 0xFF, 0xFF, 0xFF, 0xFF } } }, { { { +size, -size, 0 }, 0, { (512 * 32 * (fleft + 1)) / fmax, (512 * 32 * (ftop + 1)) / fmax }, { 0xFF, 0xFF, 0xFF, 0xFF } } }, { { { +size, +size, 0 }, 0, { (512 * 32 * (fleft + 1)) / fmax, (512 * 32 * (ftop + 0)) / fmax }, { 0xFF, 0xFF, 0xFF, 0xFF } } }, { { { -size, +size, 0 }, 0, { (512 * 32 * (fleft + 0)) / fmax, (512 * 32 * (ftop + 0)) / fmax }, { 0xFF, 0xFF, 0xFF, 0xFF } } }
#define OMM_GFX_QUAD(vertices, frame)                       gsSPVertex(vertices + 4 * frame, 4, 0), gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), gsSPEndDisplayList()
#define OMM_GFX_DL(name, texture, triangles, frame)         static const Gfx name##_##frame##_gfx[] = { gsSPClearGeometryMode(G_LIGHTING), gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0), gsDPLoadTextureBlock(texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0), gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON), gsSPDisplayList(triangles +  3 * frame), gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF), gsSPSetGeometryMode(G_LIGHTING), gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE), gsSPEndDisplayList(), }
#define OMM_GEO_BRANCH(geo)                                 GEO_NODE_START(), GEO_OPEN_NODE(), GEO_BRANCH(1, geo), GEO_CLOSE_NODE(), GEO_END()
#define OMM_GEO_BILLBOARD_AND_BRANCH(geo)                   GEO_NODE_START(), GEO_OPEN_NODE(), GEO_BILLBOARD(), GEO_OPEN_NODE(), GEO_BRANCH(1, geo), GEO_CLOSE_NODE(), GEO_CLOSE_NODE(), GEO_END()

//
// Geo layouts
//

extern const GeoLayout omm_geo_star_0_opaque[];
extern const GeoLayout omm_geo_star_1_opaque[];
extern const GeoLayout omm_geo_star_2_opaque[];
extern const GeoLayout omm_geo_star_3_opaque[];
extern const GeoLayout omm_geo_star_4_opaque[];
extern const GeoLayout omm_geo_star_5_opaque[];
extern const GeoLayout omm_geo_star_6_opaque[];
extern const GeoLayout omm_geo_star_7_opaque[];
extern const GeoLayout omm_geo_star_8_opaque[];
extern const GeoLayout omm_geo_star_9_opaque[];
extern const GeoLayout omm_geo_star_10_opaque[];
extern const GeoLayout omm_geo_star_11_opaque[];
extern const GeoLayout omm_geo_star_12_opaque[];
extern const GeoLayout omm_geo_star_13_opaque[];
extern const GeoLayout omm_geo_star_14_opaque[];
extern const GeoLayout omm_geo_star_15_opaque[];
extern const GeoLayout omm_geo_star_16_opaque[];
extern const GeoLayout omm_geo_star_17_opaque[];
extern const GeoLayout omm_geo_star_18_opaque[];
extern const GeoLayout omm_geo_star_19_opaque[];
extern const GeoLayout omm_geo_star_0_transparent[];
extern const GeoLayout omm_geo_star_1_transparent[];
extern const GeoLayout omm_geo_star_2_transparent[];
extern const GeoLayout omm_geo_star_3_transparent[];
extern const GeoLayout omm_geo_star_4_transparent[];
extern const GeoLayout omm_geo_star_5_transparent[];
extern const GeoLayout omm_geo_star_6_transparent[];
extern const GeoLayout omm_geo_star_7_transparent[];
extern const GeoLayout omm_geo_star_8_transparent[];
extern const GeoLayout omm_geo_star_9_transparent[];
extern const GeoLayout omm_geo_star_10_transparent[];
extern const GeoLayout omm_geo_star_11_transparent[];
extern const GeoLayout omm_geo_star_12_transparent[];
extern const GeoLayout omm_geo_star_13_transparent[];
extern const GeoLayout omm_geo_star_14_transparent[];
extern const GeoLayout omm_geo_star_15_transparent[];
extern const GeoLayout omm_geo_star_16_transparent[];
extern const GeoLayout omm_geo_star_17_transparent[];
extern const GeoLayout omm_geo_star_18_transparent[];
extern const GeoLayout omm_geo_star_19_transparent[];
extern const GeoLayout omm_geo_number[];
extern const GeoLayout omm_geo_star_number[];
extern const GeoLayout omm_geo_fire_smoke_red[];
extern const GeoLayout omm_geo_fire_smoke_blue[];
extern const GeoLayout omm_geo_metal_sparkle[];
extern const GeoLayout omm_geo_vanish_mist[];
extern const GeoLayout omm_geo_sparkle[];
extern const GeoLayout omm_geo_break_particle[];
extern const GeoLayout omm_geo_snowball[];
extern const GeoLayout omm_geo_mr_i_beam[];
extern const GeoLayout omm_geo_snufit_ball[];
extern const GeoLayout omm_geo_rock[];
extern const GeoLayout omm_geo_explosion[];
extern const GeoLayout omm_geo_blargg_fire_ball[];
extern const GeoLayout omm_geo_shockwave_whomp[];
extern const GeoLayout omm_geo_shockwave_spindrift[];
extern const GeoLayout omm_geo_shockwave_fire[];
extern const GeoLayout omm_geo_rising_lava[];
extern const GeoLayout omm_geo_star_ring[];
extern const GeoLayout omm_geo_bowser_mine[];
extern const GeoLayout omm_geo_bowser_flame[];
extern const GeoLayout omm_geo_bowser_fireball[];
extern const GeoLayout omm_geo_bowser_fireball_flame[];
extern const GeoLayout omm_geo_bowser_mad_aura[];
extern const GeoLayout omm_geo_flaming_bobomb[];
extern const GeoLayout omm_geo_flaming_bobomb_aura[];
extern const GeoLayout omm_geo_flaming_bobomb_explosion[];
extern const GeoLayout omm_geo_sparkly_star_1_opaque[];
extern const GeoLayout omm_geo_sparkly_star_1_transparent[];
extern const GeoLayout omm_geo_sparkly_star_1_sparkle[];
extern const GeoLayout omm_geo_sparkly_star_1_block[];
extern const GeoLayout omm_geo_sparkly_star_1_hint[];
extern const GeoLayout omm_geo_sparkly_star_2_opaque[];
extern const GeoLayout omm_geo_sparkly_star_2_transparent[];
extern const GeoLayout omm_geo_sparkly_star_2_sparkle[];
extern const GeoLayout omm_geo_sparkly_star_2_block[];
extern const GeoLayout omm_geo_sparkly_star_2_hint[];
extern const GeoLayout omm_geo_sparkly_star_3_opaque[];
extern const GeoLayout omm_geo_sparkly_star_3_transparent[];
extern const GeoLayout omm_geo_sparkly_star_3_sparkle[];
extern const GeoLayout omm_geo_sparkly_star_3_block[];
extern const GeoLayout omm_geo_sparkly_star_3_hint[];
extern const GeoLayout omm_geo_peach[];
extern const GeoLayout omm_geo_peachs_cap[];
extern const GeoLayout omm_geo_peachs_metal_cap[];
extern const GeoLayout omm_geo_perry[];
extern const GeoLayout omm_geo_perry_trail[];
extern const GeoLayout omm_geo_perry_shockwave[];
extern const GeoLayout omm_geo_perry_blast[];
extern const GeoLayout omm_geo_perry_charge[];
extern const GeoLayout omm_geo_peach_vibe_aura[];
extern const GeoLayout omm_geo_peach_vibe_sparkle[];
extern const GeoLayout omm_geo_peach_vibe_joy_tornado[];
extern const GeoLayout omm_geo_peach_vibe_joy_gust[];
extern const GeoLayout omm_geo_peach_vibe_rage_aura[];
extern const GeoLayout omm_geo_peach_vibe_rage_shockwave[];
extern const GeoLayout omm_geo_peach_vibe_gloom_aura[];
extern const GeoLayout omm_geo_peach_vibe_gloom_tear[];
extern const GeoLayout omm_geo_peach_vibe_gloom_tear_small[];
extern const GeoLayout omm_geo_peach_vibe_calm_aura[];
extern const GeoLayout omm_geo_peach_vibe_calm_sparkle[];
extern const GeoLayout omm_geo_menu_button_mario[];
extern const GeoLayout omm_geo_menu_button_peach[];
extern const GeoLayout omm_geo_menu_button_luigi[];
extern const GeoLayout omm_geo_menu_button_wario[];
extern const GeoLayout omm_geo_problem[];

//
// Behaviors
//

extern const BehaviorScript omm_bhv_grab[];
extern const BehaviorScript omm_bhv_damage_mario[];
extern const BehaviorScript omm_bhv_dummy[];
extern const BehaviorScript omm_bhv_cappy[];
extern const BehaviorScript omm_bhv_possessed_object[];
extern const BehaviorScript omm_bhv_possessed_object_cap[];
extern const BehaviorScript omm_bhv_possessed_koopa_shell[];
extern const BehaviorScript omm_bhv_act_select_star[];
extern const BehaviorScript omm_bhv_star_celebration[];
extern const BehaviorScript omm_bhv_life_up[];
extern const BehaviorScript omm_bhv_star_number[];
extern const BehaviorScript omm_bhv_fire_smoke[];
extern const BehaviorScript omm_bhv_wing_glow_left_hand[];
extern const BehaviorScript omm_bhv_wing_glow_right_hand[];
extern const BehaviorScript omm_bhv_wing_trail[];
extern const BehaviorScript omm_bhv_metal_sparkle[];
extern const BehaviorScript omm_bhv_vanish_mist[];
extern const BehaviorScript omm_bhv_break_particle[];
extern const BehaviorScript omm_bhv_snowball[];
extern const BehaviorScript omm_bhv_mr_i_beam[];
extern const BehaviorScript omm_bhv_snufit_ball[];
extern const BehaviorScript omm_bhv_rock[];
extern const BehaviorScript omm_bhv_explosion[];
extern const BehaviorScript omm_bhv_blargg_fire_drop[];
extern const BehaviorScript omm_bhv_blargg_fire_trail[];
extern const BehaviorScript omm_bhv_blargg_fire_ball[];
extern const BehaviorScript omm_bhv_shockwave_whomp[];
extern const BehaviorScript omm_bhv_shockwave_spindrift[];
extern const BehaviorScript omm_bhv_shockwave_fire[];
extern const BehaviorScript omm_bhv_rising_lava[];
extern const BehaviorScript omm_bhv_star_ring[];
extern const BehaviorScript omm_bhv_bitfs_pillar[];
extern const BehaviorScript omm_bhv_goomba_stack[];
extern const BehaviorScript omm_bhv_goomba_stack_capture[];
extern const BehaviorScript omm_bhv_chain_chomp_free[];
extern const BehaviorScript omm_bhv_mips[];
extern const BehaviorScript omm_bhv_bowser[];
extern const BehaviorScript omm_bhv_bowser_clone[];
extern const BehaviorScript omm_bhv_bowser_mine[];
extern const BehaviorScript omm_bhv_bowser_flame[];
extern const BehaviorScript omm_bhv_bowser_fireball[];
extern const BehaviorScript omm_bhv_bowser_fireball_flame[];
extern const BehaviorScript omm_bhv_bowser_mad_aura[];
extern const BehaviorScript omm_bhv_flaming_bobomb[];
extern const BehaviorScript omm_bhv_flaming_bobomb_aura[];
extern const BehaviorScript omm_bhv_flaming_bobomb_explosion[];
extern const BehaviorScript omm_bhv_sparkly_star[];
extern const BehaviorScript omm_bhv_sparkly_star_hint[];
extern const BehaviorScript omm_bhv_sparkly_star_1_block[];
extern const BehaviorScript omm_bhv_sparkly_star_2_block[];
extern const BehaviorScript omm_bhv_sparkly_star_3_block[];
extern const BehaviorScript omm_bhv_sparkly_star_sparkle[];
extern const BehaviorScript omm_bhv_sparkly_star_sparkle_mario[];
extern const BehaviorScript omm_bhv_sparkly_star_celebration[];
extern const BehaviorScript omm_bhv_perry[];
extern const BehaviorScript omm_bhv_perry_trail[];
extern const BehaviorScript omm_bhv_perry_shockwave[];
extern const BehaviorScript omm_bhv_perry_blast[];
extern const BehaviorScript omm_bhv_perry_charge[];
extern const BehaviorScript omm_bhv_peach_vibe_aura[];
extern const BehaviorScript omm_bhv_peach_vibe_sparkle[];
extern const BehaviorScript omm_bhv_peach_vibe_joy_tornado[];
extern const BehaviorScript omm_bhv_peach_vibe_joy_sparkle[];
extern const BehaviorScript omm_bhv_peach_vibe_joy_gust[];
extern const BehaviorScript omm_bhv_peach_vibe_rage_aura[];
extern const BehaviorScript omm_bhv_peach_vibe_rage_shockwave[];
extern const BehaviorScript omm_bhv_peach_vibe_gloom_aura[];
extern const BehaviorScript omm_bhv_peach_vibe_gloom_tear[];
extern const BehaviorScript omm_bhv_peach_vibe_gloom_tear_small[];
extern const BehaviorScript omm_bhv_peach_vibe_calm_aura[];
extern const BehaviorScript omm_bhv_peach_vibe_calm_sparkle[];
extern const BehaviorScript omm_bhv_menu_character_select_button[];
extern const BehaviorScript omm_bhv_problem[];

#endif // OMM_OBJECT_DATA_H
