#ifndef OMM_OBJECT_DATA_H
#define OMM_OBJECT_DATA_H

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

typedef uintptr_t GeoLayout;
#define OMM_GEO_(geo) extern const GeoLayout geo[];
#include "omm_object_data_geo.inl"
#undef OMM_GEO_

//
// Behaviors
//

typedef uintptr_t BehaviorScript;
#define OMM_BHV_(bhv) extern const BehaviorScript bhv[];
#include "omm_object_data_bhv.inl"
#undef OMM_BHV_

#endif // OMM_OBJECT_DATA_H
