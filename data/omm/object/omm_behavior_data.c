#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define MOTOS_CONSTANTS_ONLY
#include "behaviors/motos.inc.c"
#undef MOTOS_CONSTANTS_ONLY
#if OMM_CODE_DEBUG
#include "levels/wf/header.h"
#include "levels/jrb/header.h"
#include "levels/ccm/header.h"
#include "levels/bbh/header.h"
#include "levels/hmc/header.h"
#include "levels/ssl/header.h"
#endif

#define CMD_BEHAVIOR        (1)
#define CMD_BHV_TYPES       (2)
#define CMD_BOSS            (3)
#define CMD_PERRY_ATK       (4)
#define CMD_HOLDABLE        (5)
#define CMD_HITBOX          (6)
#define CMD_CAPTURE         (7)
#define CMD_YOSHI_TONGUE    (8)
#define CMD_DESTROY         (9)

#if OMM_CODE_DEBUG
#define set_behavior(bhv, geo) \
    (s64) CMD_BEHAVIOR, \
    (s64) (bhv), \
    (s64) (geo), \
    (s64) #bhv
#else
#define set_behavior(bhv, ...) \
    (s64) CMD_BEHAVIOR, \
    (s64) (bhv)
#endif

#define set_bhv_types(types) \
    (s64) CMD_BHV_TYPES, \
    (s64) (types)

#define set_boss(type) \
    (s64) CMD_BOSS, \
    (s64) (type)

#define set_perry_atk(vibeSparkles, deathAction) \
    (s64) CMD_PERRY_ATK, \
    (s64) (vibeSparkles), \
    (s64) (deathAction)

#define set_holdable(actions, heldAnim, heldAnimAccel, thrownAnim, thrownAnimAccel, hitSurface, nextAction, interactionFlags) \
    (s64) CMD_HOLDABLE, \
    (s64) (actions), \
    (s64) (heldAnim), \
    (s64) (heldAnimAccel * 100.f), \
    (s64) (thrownAnim), \
    (s64) (thrownAnimAccel * 100.f), \
    (s64) (hitSurface), \
    (s64) (nextAction), \
    (s64) (interactionFlags)

#define set_hitbox(hitboxRadius, hitboxHeight, hurtboxRadius, hurtboxHeight, hitboxDownOffset, scale) \
    (s64) CMD_HITBOX, \
    (s64) (hitboxRadius), \
    (s64) (hitboxHeight), \
    (s64) (hurtboxRadius), \
    (s64) (hurtboxHeight), \
    (s64) (hitboxDownOffset), \
    (s64) (scale)

#define set_capture(id, baseScale, walkSpeed, runSpeed, dashSpeed, jumpVelocity, terminalVelocity, gravity, hitboxRadius, hitboxHeight, hitboxDownOffset, wallHitboxRadius, scaleModifier, referenceObject) \
    (s64) CMD_CAPTURE, \
    (s64) (omm_cappy_##id##_init), \
    (s64) (omm_cappy_##id##_update), \
    (s64) (omm_cappy_##id##_update_gfx), \
    (s64) (omm_cappy_##id##_end), \
    (s64) (omm_cappy_##id##_get_type), \
    (s64) (omm_cappy_##id##_get_top), \
    (s64) (baseScale * 100.f), \
    (s64) (walkSpeed), \
    (s64) (runSpeed), \
    (s64) (dashSpeed), \
    (s64) (jumpVelocity), \
    (s64) (terminalVelocity), \
    (s64) (gravity * 100.f), \
    (s64) (hitboxRadius), \
    (s64) (hitboxHeight), \
    (s64) (hitboxDownOffset), \
    (s64) (wallHitboxRadius), \
    (s64) (scaleModifier), \
    (s64) (referenceObject)

#define set_capture_npc(hitboxHeight) \
    (s64) CMD_CAPTURE, \
    (s64) (omm_cappy_npc_message_init), \
    (s64) (omm_cappy_npc_message_update), \
    (s64) (omm_cappy_npc_message_update_gfx), \
    (s64) (omm_cappy_npc_message_end), \
    (s64) (omm_cappy_npc_message_get_type), \
    (s64) (omm_cappy_npc_message_get_top), \
    (s64) (100), \
    (s64) (0), \
    (s64) (0), \
    (s64) (0), \
    (s64) (0), \
    (s64) (0), \
    (s64) (0), \
    (s64) (0), \
    (s64) (hitboxHeight), \
    (s64) (0), \
    (s64) (0), \
    (s64) (smod_none), \
    (s64) (false)

#define set_yoshi_tongue(type) \
    (s64) CMD_YOSHI_TONGUE, \
    (s64) (type)

#define set_destroy(type, cond, arg1, arg2, arg3, arg4, arg5, arg6) \
    (s64) CMD_DESTROY, \
    (s64) (type), \
    (s64) (cond), \
    (s64) (arg1), \
    (s64) (arg2), \
    (s64) (arg3), \
    (s64) (arg4), \
    (s64) (arg5), \
    (s64) (arg6)

#define set_destroy_white_puff(cond, numCoins, soundBits) \
    set_destroy(DESTROY_TYPE_WHITE_PUFF, cond, numCoins, soundBits, 0, 0, 0, 0)

#define set_destroy_white_puff_and_children(cond, numCoins, soundBits, childBhv) \
    set_destroy(DESTROY_TYPE_WHITE_PUFF_AND_CHILDREN, cond, numCoins, soundBits, childBhv, 0, 0, 0)

#define set_destroy_break_particles(cond, numCoins, soundBits, r, g, b) \
    set_destroy(DESTROY_TYPE_BREAK_PARTICLES, cond, numCoins, soundBits, r, g, b, 0)

#define set_destroy_triangle_particles_(cond, numCoins, soundBits, triCount, triModel, triSize, triType) \
    set_destroy(DESTROY_TYPE_TRIANGLE_PARTICLES, cond, numCoins, soundBits, triCount, triModel, triSize * 100.f, triType)

#define set_destroy_triangle_particles(cond, numCoins, soundBits, ...) \
    set_destroy_triangle_particles_(cond, numCoins, soundBits, __VA_ARGS__)

#define set_destroy_snow_particles(cond, numCoins, soundBits) \
    set_destroy(DESTROY_TYPE_SNOW_PARTICLES, cond, numCoins, soundBits, 0, 0, 0, 0)

#define set_destroy_explosion(cond, numCoins) \
    set_destroy(DESTROY_TYPE_EXPLOSION, cond, numCoins, 0, 0, 0, 0, 0)

#define set_destroy_explosion_and_respawn(cond, numCoins, model, minSpawnDist) \
    set_destroy(DESTROY_TYPE_EXPLOSION_AND_RESPAWN, cond, numCoins, model, minSpawnDist, 0, 0, 0)

#define set_destroy_water_bomb(cond, soundBits) \
    set_destroy(DESTROY_TYPE_WATER_BOMB, cond, soundBits, 0, 0, 0, 0, 0)

#define set_destroy_whomp_king(cond) \
    set_destroy(DESTROY_TYPE_WHOMP_KING, cond, 0, 0, 0, 0, 0, 0)

#define set_destroy_big_snowball(cond, numCoins, soundBits) \
    set_destroy(DESTROY_TYPE_BIG_SNOWBALL, cond, numCoins, soundBits, 0, 0, 0, 0)

#define set_destroy_boo(cond, soundBits) \
    set_destroy(DESTROY_TYPE_BOO, cond, soundBits, 0, 0, 0, 0, 0)

#define set_destroy_big_mr_i(cond, soundBits) \
    set_destroy(DESTROY_TYPE_BIG_MR_I, cond, soundBits, 0, 0, 0, 0, 0)

#define set_destroy_bully(cond, actDeath) \
    set_destroy(DESTROY_TYPE_BULLY, cond, actDeath, 0, 0, 0, 0, 0)

#define set_destroy_pokey(cond, numCoins, soundBits) \
    set_destroy(DESTROY_TYPE_POKEY, cond, numCoins, soundBits, 0, 0, 0, 0)

#define set_destroy_eyerok_hand(cond, soundBits, r, g, b) \
    set_destroy(DESTROY_TYPE_EYEROK_HAND, cond, soundBits, r, g, b, 0, 0)

#define set_destroy_ukiki_cage(cond) \
    set_destroy(DESTROY_TYPE_UKIKI_CAGE, cond, 0, 0, 0, 0, 0, 0)

#define set_destroy_fire_piranha_plant(cond, numCoins, soundBits) \
    set_destroy(DESTROY_TYPE_FIRE_PIRANHA_PLANT, cond, numCoins, soundBits, 0, 0, 0, 0)

#define set_destroy_wooden_post(cond, soundBits) \
    set_destroy(DESTROY_TYPE_WOODEN_POST, cond, 0, soundBits, 0, 0, 0, 0)

//
// Raw data
//

static f32 smod_none(f32 scale) { return scale; }
static f32 smod_goomba(f32 scale) { return 1.5f * sqrtf(scale / 1.5f); }

static const s64 OMM_BEHAVIOR_DATA[] = {

//
// Vanilla
//

set_behavior(bhv1Up, mushroom_1up_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 60, 0, 0, 0, true), /* 30, 30, 0, 0, 0 */
set_behavior(bhv1upJumpOnApproach, mushroom_1up_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 60, 0, 0, 0, true), /* 30, 30, 0, 0, 0 */
set_behavior(bhv1upRunningAway, mushroom_1up_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 60, 0, 0, 0, true), /* 30, 30, 0, 0, 0 */
set_behavior(bhv1upSliding, mushroom_1up_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 60, 0, 0, 0, true), /* 30, 30, 0, 0, 0 */
set_behavior(bhv1upWalking, mushroom_1up_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 60, 0, 0, 0, true), /* 30, 30, 0, 0, 0 */
set_behavior(bhvActSelector, star_geo),
    set_bhv_types(BHV_TYPE_STAR_MODEL),
set_behavior(bhvActSelectorStarType, NULL),
    set_bhv_types(0),
set_behavior(bhvActivatedBackAndForthPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvAirborneDeathWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvAirborneStarCollectWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvAirborneWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvAmbientSounds, NULL),
    set_bhv_types(0),
set_behavior(bhvAnimatedTexture, NULL),
    set_bhv_types(0),
set_behavior(bhvAnimatesOnFloorSwitchPress, NULL),
    set_bhv_types(0),
set_behavior(bhvAnotherElavator, NULL),
    set_bhv_types(0),
set_behavior(bhvAnotherTiltingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvArrowLift, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvBalconyBigBoo, boo_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_BOSS | BHV_TYPE_BOO),
    set_boss(BOSS_TYPE_BIG_BOO),
    set_perry_atk(8, 4),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED_BIG_BOO, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_hitbox(80, 100, 40, 60, 0, true), /* 140, 80, 40, 60, 0 */
set_behavior(bhvBbhTiltingTrapPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvBbhTumblingBridge, NULL),
    set_bhv_types(0),
set_behavior(bhvBeginningLakitu, NULL),
    set_bhv_types(0),
set_behavior(bhvBeginningPeach, NULL),
    set_bhv_types(0),
set_behavior(bhvBetaBowserAnchor, NULL),
    set_bhv_types(0),
set_behavior(bhvBetaChestBottom, NULL),
    set_bhv_types(0),
set_behavior(bhvBetaChestLid, NULL),
    set_bhv_types(0),
set_behavior(bhvBetaFishSplashSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBetaHoldableObject, NULL),
    set_bhv_types(0),
set_behavior(bhvBetaMovingFlames, NULL),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvBetaMovingFlamesSpawn, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvBetaTrampolineSpring, NULL),
    set_bhv_types(0),
set_behavior(bhvBetaTrampolineTop, NULL),
    set_bhv_types(0),
set_behavior(bhvBigBoulder, hmc_geo_000548),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_DEFEATED_IF_DESTROYED),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0xB0, 0xA0, 0x70),
    set_perry_atk(4, -1),
set_behavior(bhvBigBoulderGenerator, NULL),
    set_bhv_types(0),
set_behavior(bhvBigBully, bully_boss_geo),
    set_bhv_types(BHV_TYPE_BOSS | BHV_TYPE_BULLY),
    set_boss(BOSS_TYPE_BIG_BULLY),
    set_destroy_bully(DESTROY_COND_NONE, BULLY_ACT_LAVA_DEATH),
    set_perry_atk(8, -1),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 2.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvBigBullyWithMinions, bully_boss_geo),
    set_bhv_types(BHV_TYPE_BOSS | BHV_TYPE_BULLY),
    set_boss(BOSS_TYPE_BIG_BULLY),
    set_destroy_bully(DESTROY_COND_NONE, BULLY_ACT_LAVA_DEATH),
    set_perry_atk(8, -1),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 2.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvBigChillBully, chilly_chief_big_geo),
    set_bhv_types(BHV_TYPE_BOSS | BHV_TYPE_BULLY),
    set_boss(BOSS_TYPE_BIG_BULLY),
    set_destroy_bully(DESTROY_COND_NONE, BULLY_ACT_LAVA_DEATH),
    set_perry_atk(8, -1),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 2.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvBigSnowmanWhole, NULL),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
set_behavior(bhvBird, NULL),
    set_bhv_types(0),
set_behavior(bhvBirdsSoundLoop, NULL),
    set_bhv_types(0),
set_behavior(bhvBitfsSinkingCagePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvBitfsSinkingPlatforms, NULL),
    set_bhv_types(0),
set_behavior(bhvBitfsTiltingInvertedPyramid, NULL),
    set_bhv_types(0),
set_behavior(bhvBlackSmokeBowser, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBlackSmokeMario, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBlackSmokeUpward, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBlueBowserFlame, blue_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvBlueCoinJumping, blue_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvBlueCoinSliding, blue_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvBlueCoinSwitch, blue_coin_switch_geo),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
    set_holdable(HOLD_ACTIONS_0, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR, BLUE_COIN_SWITCH_ACT_RECEDING, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvBlueFish, NULL),
    set_bhv_types(0),
set_behavior(bhvBlueFlamesGroup, blue_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvBobBowlingBallSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvBobomb, black_bobomb_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_GRABBABLE | BHV_TYPE_SPARKLY_ENEMY),
    set_destroy_explosion_and_respawn(DESTROY_COND_NONE, 0, MODEL_BLACK_BOBOMB, 1500),
    set_perry_atk(2, -1),
    set_holdable(HOLD_ACTIONS_ALL, 1, 1.f, 1, 1.f, HOLD_HIT_ANY, BOBOMB_ACT_EXPLODE, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(bobomb, 1.00f, 24, 24, 24, 32, -75, -4.00f, 60, 100, 0, 40, smod_none, true),
set_behavior(bhvBobombAnchorMario, NULL),
    set_bhv_types(0),
set_behavior(bhvBobombBuddy, bobomb_buddy_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 0, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture_npc(100),
set_behavior(bhvBobombBuddyOpensCannon, bobomb_buddy_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 0, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(bobomb_buddy, 1.00f, 0, 0, 0, 0, 0, 0.00f, 60, 100, 0, 40, smod_none, false),
set_behavior(bhvBobombBullyDeathSmoke, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBobombExplosionBubble, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBobombExplosionBubble3600, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBobombFuseSmoke, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBoo, boo_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_BOO),
    set_destroy_boo(DESTROY_COND_NONE, SOUND_OBJ_DYING_ENEMY1),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(80, 100, 40, 60, 0, true), /* 140, 80, 40, 60, 0 */
    set_capture(boo, 1.00f, 24, 48, 48, 20, -20, -2.00f, 80, 120, 40, 50, smod_none, true),
set_behavior(bhvBooCage, NULL),
    set_bhv_types(0),
set_behavior(bhvBooInCastle, NULL),
    set_bhv_types(0),
set_behavior(bhvBooWithCage, boo_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_BOO),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_hitbox(150, 180, 80, 120, 0, false), /* 180, 140, 80, 120, 0 */
    set_capture(boo, 1.00f, 24, 48, 48, 20, -20, -2.00f, 80, 120, 40, 50, smod_none, true),
set_behavior(bhvBookSwitch, NULL),
    set_bhv_types(0),
set_behavior(bhvBookendSpawn, NULL),
    set_bhv_types(0),
set_behavior(bhvBouncingFireball, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvBouncingFireballFlame, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvBowlingBall, bowling_ball_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_DEFEATED_IF_DESTROYED | BHV_TYPE_METAL_BALL),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x40, 0x40, 0x40),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(100, 200, 0, 0, 0, true), /* 100, 150, 0, 0, 0 */
set_behavior(bhvBowser, bowser_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE | BHV_TYPE_BOWSER | BHV_TYPE_NOT_ENEMY),
    set_holdable(HOLD_ACTIONS_ALL, 2, 1.f, 25, 2.f, HOLD_HIT_FLOOR_OR_WALL, 1, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvBowserBodyAnchor, NULL),
    set_bhv_types(BHV_TYPE_BOSS | BHV_TYPE_INVULNERABLE | BHV_TYPE_BOWSER | BHV_TYPE_NOT_ENEMY),
    set_boss(BOSS_TYPE_BOWSER),
set_behavior(bhvBowserBomb, bowser_bomb_geo),
    set_bhv_types(BHV_TYPE_BOWSER),
set_behavior(bhvBowserBombExplosion, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBowserBombSmoke, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBowserCourseRedCoinStar, NULL),
    set_bhv_types(0),
set_behavior(bhvBowserFlameSpawn, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME | BHV_TYPE_BOWSER),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvBowserKey, bowser_key_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_STAR_OR_KEY),
set_behavior(bhvBowserKeyCourseExit, NULL),
    set_bhv_types(0),
set_behavior(bhvBowserKeyUnlockDoor, NULL),
    set_bhv_types(0),
set_behavior(bhvBowserShockWave, NULL),
    set_bhv_types(0),
set_behavior(bhvBowserSubDoor, NULL),
    set_bhv_types(0),
set_behavior(bhvBowserTailAnchor, NULL),
    set_bhv_types(BHV_TYPE_GRABBABLE | BHV_TYPE_NO_CAPPY | BHV_TYPE_BOWSER),
set_behavior(bhvBowsersSub, NULL),
    set_bhv_types(0),
set_behavior(bhvBreakBoxTriangle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBreakableBox, breakable_box_geo),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_triangle_particles(DESTROY_COND_NONE, DESTROY_COINS_REMAINING, SOUND_GENERAL_BREAK_BOX, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_30),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
set_behavior(bhvBreakableBoxSmall, breakable_box_geo),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_GRABBABLE | BHV_TYPE_NOT_ENEMY),
    set_destroy_triangle_particles(DESTROY_COND_NONE, 3, SOUND_GENERAL_BREAK_BOX, OBJ_SPAWN_TRI_BREAK_PRESET_DIRT),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
set_behavior(bhvBreathParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBub, bub_geo),
    set_bhv_types(0),
    set_hitbox(40, 60, 0, 0, 30, true), /* 20, 10, 0, 0, 10 */
    set_capture(cheep_cheep, 1.00f, 36, 36, 72, 0, 0, 0.00f, 50, 60, 30, 35, smod_none, false),
set_behavior(bhvBubba, bubba_geo),
    set_bhv_types(0),
set_behavior(bhvBubbleMaybe, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBubbleParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBubbleSplash, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvBulletBill, bullet_bill_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE | BHV_TYPE_METAL_DESTRUCTIBLE),
    set_destroy_explosion_and_respawn(DESTROY_COND_NONE, 0, MODEL_BULLET_BILL, 1000),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
    set_hitbox(200, 400, 0, 0, 200, true), /* 50, 50, 0, 0, 50 */
    set_capture(bullet_bill, 0.40f, 40, 40, 80, 0, 0, 0.00f, 80, 160, 80, 80, smod_none, true),
set_behavior(bhvBulletBillCannon, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvButterfly, NULL),
    set_bhv_types(0),
set_behavior(bhvCameraLakitu, NULL),
    set_bhv_types(0),
set_behavior(bhvCannon, NULL),
    set_bhv_types(0),
set_behavior(bhvCannonBarrel, NULL),
    set_bhv_types(0),
set_behavior(bhvCannonBarrelBubbles, NULL),
    set_bhv_types(0),
set_behavior(bhvCannonBaseUnused, NULL),
    set_bhv_types(0),
set_behavior(bhvCannonClosed, NULL),
    set_bhv_types(0),
set_behavior(bhvCapSwitch, NULL),
    set_bhv_types(0),
set_behavior(bhvCapSwitchBase, NULL),
    set_bhv_types(0),
set_behavior(bhvCarrySomething1, NULL),
    set_bhv_types(0),
set_behavior(bhvCarrySomething2, NULL),
    set_bhv_types(0),
set_behavior(bhvCarrySomething3, NULL),
    set_bhv_types(0),
set_behavior(bhvCarrySomething4, NULL),
    set_bhv_types(0),
set_behavior(bhvCarrySomething5, NULL),
    set_bhv_types(0),
set_behavior(bhvCarrySomething6, NULL),
    set_bhv_types(0),
set_behavior(bhvCastleFlagWaving, NULL),
    set_bhv_types(0),
set_behavior(bhvCastleFloorTrap, NULL),
    set_bhv_types(0),
set_behavior(bhvCcmTouchedStarSpawn, NULL),
    set_bhv_types(0),
set_behavior(bhvCelebrationStar, star_geo),
    set_bhv_types(BHV_TYPE_STAR_MODEL),
set_behavior(bhvCelebrationStarSparkle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvChainChomp, chain_chomp_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
    set_destroy_white_puff_and_children(DESTROY_COND_NONE, 0, SOUND_OBJ_DEFAULT_DEATH, bhvChainChompChainPart),
    set_hitbox(100, 240, 80, 160, 0, true), /* 80, 160, 80, 160, 0 */
    set_capture(chain_chomp, 2.00f, 48, 48, 160, 60, -90, -6.00f, 200, 360, 0, 150, smod_none, true),
set_behavior(bhvChainChompChainPart, NULL),
    set_bhv_types(0),
set_behavior(bhvChainChompGate, NULL),
    set_bhv_types(0),
set_behavior(bhvCheckerboardElevatorGroup, NULL),
    set_bhv_types(0),
set_behavior(bhvCheckerboardPlatformSub, NULL),
    set_bhv_types(0),
set_behavior(bhvChirpChirp, NULL),
    set_bhv_types(0),
set_behavior(bhvChirpChirpUnused, NULL),
    set_bhv_types(0),
set_behavior(bhvChuckya, chuckya_geo),
    set_bhv_types(BHV_TYPE_STRONG | BHV_TYPE_GRABBABLE | BHV_TYPE_SPARKLY_ENEMY),
    set_destroy_white_puff(DESTROY_COND_NONE, 5, SOUND_OBJ_CHUCKYA_DEATH),
    set_perry_atk(4, -1),
    set_holdable(HOLD_ACTIONS_ALL, 2, 1.f, 2, 1.f, HOLD_HIT_ANY, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(150, 160, 0, 0, 0, false), /* 150, 100, 0, 0, 0 */
    set_capture(chuckya, 2.00f, 36, 36, 36, 36, -75, -4.00f, 120, 200, 0, 90, smod_none, true),
set_behavior(bhvChuckyaAnchorMario, NULL),
    set_bhv_types(0),
set_behavior(bhvCirclingAmp, amp_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 0, 1.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_SPECIAL, OBJ_INT_PRESET_ATTACK_AMP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(50, 90, 60, 90, 0, true), /* 40, 50, 50, 60, 40 */
    set_capture(amp, 1.00f, 30, 30, 30, 33, -45, -3.00f, 55, 110, 0, 40, smod_none, true),
set_behavior(bhvClamShell, clam_shell_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 1, HOLD_ANIM_SET_FRAME(0), 1, HOLD_ANIM_SET_FRAME(0), HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvClockHourHand, NULL),
    set_bhv_types(0),
set_behavior(bhvClockMinuteHand, NULL),
    set_bhv_types(0),
set_behavior(bhvCloud, NULL),
    set_bhv_types(0),
set_behavior(bhvCloudPart, NULL),
    set_bhv_types(0),
set_behavior(bhvCoffin, geo_bbh_000658),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_BREAK_BOX, 0x60, 0x40, 0x20),
    set_perry_atk(5, -1),
set_behavior(bhvCoffinSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvCoinFormation, NULL),
    set_bhv_types(0),
set_behavior(bhvCoinFormationSpawn, NULL),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvCoinInsideBoo, NULL),
    set_bhv_types(0),
set_behavior(bhvCoinSparkles, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvControllablePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvControllablePlatformSub, NULL),
    set_bhv_types(0),
set_behavior(bhvCourtyardBooTriplet, NULL),
    set_bhv_types(0),
set_behavior(bhvCutOutObject, NULL),
    set_bhv_types(0),
set_behavior(bhvDDDPole, NULL),
    set_bhv_types(0),
set_behavior(bhvDddMovingPole, NULL),
    set_bhv_types(0),
set_behavior(bhvDddWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvDeathWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvDecorativePendulum, NULL),
    set_bhv_types(0),
set_behavior(bhvDirtParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvDonutPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvDonutPlatformSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvDoor, NULL),
    set_bhv_types(BHV_TYPE_DOOR),
set_behavior(bhvDoorWarp, NULL),
    set_bhv_types(BHV_TYPE_DOOR),
set_behavior(bhvDorrie, dorrie_geo),
    set_bhv_types(0),
    set_capture(dorrie, 1.00f, 30, 30, 30, 0, 0, 0.00f, 600, 300, 0, 300, smod_none, false),
set_behavior(bhvEndBirds1, NULL),
    set_bhv_types(0),
set_behavior(bhvEndBirds2, NULL),
    set_bhv_types(0),
set_behavior(bhvEndPeach, NULL),
    set_bhv_types(0),
set_behavior(bhvEndToad, NULL),
    set_bhv_types(0),
set_behavior(bhvEnemyLakitu, enemy_lakitu_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.5f, 3, 1.5f, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(55, 100, 40, 50, 0, true), /* 50, 50, 40, 50, 0 */
    set_capture(lakitu, 1.00f, 36, 36, 36, 40, -20, -2.00f, 60, 100, 20, 30, smod_none, true),
set_behavior(bhvExclamationBox, exclamation_box_geo),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_EXCLAMATION_BOX | BHV_TYPE_NO_CAM_COL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_ATTACK),
set_behavior(bhvExitPodiumWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvExplosion, NULL),
    set_bhv_types(0),
    set_hitbox(150, 200, 0, 0, 50, false), /* 150, 150, 0, 0, 150 */
set_behavior(bhvEyerokBoss, NULL),
    set_bhv_types(BHV_TYPE_INVULNERABLE | BHV_TYPE_BOSS),
set_behavior(bhvEyerokHand, NULL),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_BOSS | BHV_TYPE_NO_CAM_COL),
    set_boss(BOSS_TYPE_EYEROK_HAND),
    set_destroy_eyerok_hand(DESTROY_COND_NONE, SOUND_OBJ2_EYEROK_SOUND_SHORT, 0xC8, 0x90, 0x30),
    set_perry_atk(5, -1),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_ATTACK),
    set_hitbox(100, 100, 1, 1, 0, true), /* 150, 100, 1, 1, 0 */
set_behavior(bhvFadingWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvFallingBowserPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvFallingPillar, NULL),
    set_bhv_types(0),
set_behavior(bhvFallingPillarHitbox, NULL),
    set_bhv_types(0),
set_behavior(bhvFerrisWheelAxle, NULL),
    set_bhv_types(0),
set_behavior(bhvFerrisWheelPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvFireParticleSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvFirePiranhaPlant, piranha_plant_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_SPARKLY_ENEMY),
    set_destroy_fire_piranha_plant(DESTROY_COND_NONE, DESTROY_COINS_REMAINING, SOUND_OBJ_DEFAULT_DEATH),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_1, 9, 2.f, 9, 2.f, HOLD_HIT_FLOOR_OR_WATER, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
set_behavior(bhvFireSpitter, bowling_ball_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
    set_capture(fire_spitter, 0.25f, 25, 25, 25, 36, -75, -4.00f, 40, 80, 0, 30, smod_none, true),
set_behavior(bhvFish, NULL),
    set_bhv_types(0),
set_behavior(bhvFishGroup, NULL),
    set_bhv_types(0),
set_behavior(bhvFlame, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvFlameBouncing, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvFlameBowser, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvFlameFloatingLanding, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvFlameLargeBurningOut, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvFlameMovingForwardGrowing, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvFlamethrower, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvFlamethrowerFlame, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
    set_hitbox(60, 120, 0, 0, 60, false), /* 50, 25, 0, 0, 25 */
set_behavior(bhvFloorSwitchAnimatesObject, NULL),
    set_bhv_types(0),
set_behavior(bhvFloorSwitchGrills, NULL),
    set_bhv_types(0),
set_behavior(bhvFloorSwitchHardcodedModel, NULL),
    set_bhv_types(0),
set_behavior(bhvFloorSwitchHiddenObjects, NULL),
    set_bhv_types(0),
set_behavior(bhvFloorTrapInCastle, NULL),
    set_bhv_types(0),
set_behavior(bhvFlyGuy, flyguy_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 4.f, 0, 4.f, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(60, 100, 40, 50, 0, true), /* 70, 60, 40, 50, 0 */
    set_capture(fly_guy, 1.50f, 32, 32, 32, 36, -75, -4.00f, 90, 150, 0, 50, smod_none, true),
set_behavior(bhvFlyguyFlame, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME | BHV_TYPE_UNIMPORTANT),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvFlyingBookend, bookend_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 1, 1.f, 1, 1.f, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
set_behavior(bhvFlyingWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvFreeBowlingBall, bowling_ball_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_DEFEATED_IF_DESTROYED | BHV_TYPE_METAL_BALL),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x40, 0x40, 0x40),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(100, 200, 0, 0, 0, true), /* 100, 150, 0, 0, 0 */
set_behavior(bhvGhostHuntBigBoo, boo_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_BOSS | BHV_TYPE_BOO),
    set_boss(BOSS_TYPE_BIG_BOO),
    set_perry_atk(8, 4),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED_BIG_BOO, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_hitbox(80, 100, 40, 60, 0, true), /* 140, 80, 40, 60, 0 */
set_behavior(bhvGhostHuntBoo, boo_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_BOO),
    set_destroy_boo(DESTROY_COND_NONE, SOUND_OBJ_DYING_ENEMY1),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(80, 100, 40, 60, 0, true), /* 140, 80, 40, 60, 0 */
    set_capture(boo, 1.00f, 24, 48, 48, 20, -20, -2.00f, 80, 120, 40, 50, smod_none, true),
set_behavior(bhvGiantPole, NULL),
    set_bhv_types(0),
set_behavior(bhvGoldenCoinSparkles, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvGoomba, goomba_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_GOOMBA | BHV_TYPE_SPARKLY_ENEMY),
    set_destroy_white_puff(DESTROY_COND_TINY_GOOMBA, 1, SOUND_OBJ_ENEMY_DEATH_HIGH),
    set_destroy_white_puff(DESTROY_COND_REGULAR_GOOMBA, 1, SOUND_OBJ_ENEMY_DEATH_HIGH),
    set_destroy_white_puff(DESTROY_COND_HUGE_GOOMBA, DESTROY_COINS_BLUE, SOUND_OBJ_ENEMY_DEATH_LOW),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 2.f, 0, 2.f, HOLD_HIT_ANY, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(goomba, 1.50f, 21, 42, 42, 30, -75, -4.00f, 75, 95, 0, 45, smod_goomba, true),
set_behavior(bhvGoombaTripletSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvGrandStar, star_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_STAR_OR_KEY),
set_behavior(bhvGrindel, ssl_geo_000734),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0xFF, 0xFF, 0xA0),
    set_perry_atk(5, -1),
    set_capture(thwomp, 1.00f, 35, 35, 35, 50, -90, -4.50f, 240, 440, 0, 200, smod_none, true),
set_behavior(bhvHardAirKnockBackWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvHauntedBookshelf, NULL),
    set_bhv_types(0),
set_behavior(bhvHauntedBookshelfManager, NULL),
    set_bhv_types(0),
set_behavior(bhvHauntedChair, haunted_chair_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_DEFEATED_IF_DESTROYED),
    set_destroy_white_puff(DESTROY_COND_NONE, 0, SOUND_OBJ_DEFAULT_DEATH),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 0, 1.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
set_behavior(bhvHeaveHo, heave_ho_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE),
    set_destroy_white_puff_and_children(DESTROY_COND_NONE, DESTROY_COINS_BLUE, SOUND_OBJ_DEFAULT_DEATH, bhvHeaveHoThrowMario),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 2.f, 0, 2.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(heave_ho, 2.00f, 40, 40, 40, 40, -90, -4.50f, 120, 120, 0, 80, smod_none, true),
set_behavior(bhvHeaveHoThrowMario, NULL),
    set_bhv_types(0),
set_behavior(bhvHidden1up, mushroom_1up_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 60, 0, 0, 0, true), /* 30, 30, 0, 0, 0 */
set_behavior(bhvHidden1upInPole, mushroom_1up_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 60, 0, 0, 0, true), /* 30, 30, 0, 0, 0 */
set_behavior(bhvHidden1upInPoleSpawner, NULL),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
set_behavior(bhvHidden1upInPoleTrigger, NULL),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
set_behavior(bhvHidden1upTrigger, NULL),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
set_behavior(bhvHiddenAt120Stars, NULL),
    set_bhv_types(0),
set_behavior(bhvHiddenBlueCoin, blue_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvHiddenObject, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvHiddenRedCoinStar, NULL),
    set_bhv_types(0),
set_behavior(bhvHiddenStaircaseStep, NULL),
    set_bhv_types(0),
set_behavior(bhvHiddenStar, NULL),
    set_bhv_types(0),
set_behavior(bhvHiddenStarTrigger, purple_marble_geo),
    set_bhv_types(BHV_TYPE_SECRET),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_SECRET),
set_behavior(bhvHmcElevatorPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvHomingAmp, amp_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 0, 1.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_SPECIAL, OBJ_INT_PRESET_ATTACK_AMP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(50, 90, 60, 90, 0, true), /* 40, 50, 50, 60, 40 */
    set_capture(amp, 1.00f, 30, 30, 30, 33, -45, -3.00f, 55, 110, 0, 40, smod_none, true),
set_behavior(bhvHoot, hoot_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 0, 1.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(hoot, 1.00f, 30, 30, 30, 30, -20, -2.00f, 60, 100, 0, 40, smod_none, true),
set_behavior(bhvHorStarParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvHorizontalGrindel, ssl_geo_000734),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0xFF, 0xFF, 0xA0),
    set_perry_atk(5, -1),
    set_capture(thwomp, 1.00f, 35, 35, 35, 50, -90, -4.50f, 240, 440, 0, 200, smod_none, true),
set_behavior(bhvIdleWaterWave, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvIgloo, NULL),
    set_bhv_types(0),
set_behavior(bhvInSunkenShip, NULL),
    set_bhv_types(0),
set_behavior(bhvInSunkenShip2, NULL),
    set_bhv_types(0),
set_behavior(bhvInSunkenShip3, NULL),
    set_bhv_types(0),
set_behavior(bhvInitializeChangingWaterLevel, NULL),
    set_bhv_types(0),
set_behavior(bhvInsideCannon, NULL),
    set_bhv_types(0),
set_behavior(bhvInstantActiveWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvIntroScene, NULL),
    set_bhv_types(0),
set_behavior(bhvInvisibleObjectsUnderBridge, NULL),
    set_bhv_types(0),
set_behavior(bhvJetStream, NULL),
    set_bhv_types(0),
set_behavior(bhvJetStreamRingSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvJetStreamWaterRing, NULL),
    set_bhv_types(BHV_TYPE_WATER_RING),
set_behavior(bhvJrbFloatingBox, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvJrbFloatingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvJrbSlidingBox, jrb_geo_000960),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x50, 0x40, 0x00),
    set_perry_atk(5, -1),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvJumpingBox, breakable_box_geo),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_GRABBABLE | BHV_TYPE_NOT_ENEMY),
    set_destroy_triangle_particles(DESTROY_COND_NONE, 5, SOUND_GENERAL_BREAK_BOX, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_20),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_ANY, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(crazy_box, 0.50f, 36, 36, 36, 60, -75, -4.00f, 75, 100, 0, 50, smod_none, false),
set_behavior(bhvKickableBoard, NULL),
    set_bhv_types(BHV_TYPE_KICKABLE_BOARD),
set_behavior(bhvKingBobomb, king_bobomb_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE | BHV_TYPE_BOSS),
    set_boss(BOSS_TYPE_KING_BOBOMB),
    set_perry_atk(8, 8),
    set_holdable(HOLD_ACTIONS_KING_BOBOMB, 6, 1.f, 6, 1.f, HOLD_HIT_FLOOR, 4, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_hitbox(100, 250, 0, 0, 0, true), /* 100, 100, 0, 0, 0 */
set_behavior(bhvKlepto, klepto_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_holdable(HOLD_ACTIONS_ALL, 0, 3.f, 0, 3.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvKoopa, koopa_with_shell_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 2, 2.f, 3, 2.f, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(60, 60, 40, 30, 0, true), /* 60, 40, 40, 30, 0 */
    set_capture(koopa, 1.50f, 30, 45, 60, 42, -75, -4.00f, 90, 100, 0, 50, smod_none, true),
set_behavior(bhvKoopaFlag, NULL),
    set_bhv_types(0),
set_behavior(bhvKoopaRaceEndpoint, NULL),
    set_bhv_types(0),
set_behavior(bhvKoopaShell, koopa_shell_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(koopa_shell, 1.00f, 40, 60, 60, 42, -75, -4.00f, 90, 100, 0, 50, smod_none, true),
set_behavior(bhvKoopaShellFlame, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvKoopaShellUnderwater, koopa_shell_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(koopa_shell, 1.00f, 40, 60, 60, 42, -75, -4.00f, 90, 100, 0, 50, smod_none, true),
set_behavior(bhvLargeBomp, NULL),
    set_bhv_types(0),
set_behavior(bhvLaunchDeathWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvLaunchStarCollectWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvLeafParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvLllBowserPuzzle, NULL),
    set_bhv_types(0),
set_behavior(bhvLllBowserPuzzlePiece, NULL),
    set_bhv_types(0),
set_behavior(bhvLllDrawbridge, NULL),
    set_bhv_types(0),
set_behavior(bhvLllDrawbridgeSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvLllFloatingWoodBridge, NULL),
    set_bhv_types(0),
set_behavior(bhvLllHexagonalMesh, NULL),
    set_bhv_types(0),
set_behavior(bhvLllMovingOctagonalMeshPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvLllRollingLog, NULL),
    set_bhv_types(0),
set_behavior(bhvLllRotatingBlockWithFireBars, NULL),
    set_bhv_types(0),
set_behavior(bhvLllRotatingHexFlame, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvLllRotatingHexagonalPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvLllRotatingHexagonalRing, NULL),
    set_bhv_types(0),
set_behavior(bhvLllSinkingRectangularPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvLllSinkingRockBlock, NULL),
    set_bhv_types(0),
set_behavior(bhvLllSinkingSquarePlatforms, NULL),
    set_bhv_types(0),
set_behavior(bhvLllTiltingInvertedPyramid, NULL),
    set_bhv_types(0),
set_behavior(bhvLllTumblingBridge, NULL),
    set_bhv_types(0),
set_behavior(bhvLllVolcanoFallingTrap, NULL),
    set_bhv_types(0),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0xC0, 0xC0, 0xC0),
set_behavior(bhvLllWoodPiece, NULL),
    set_bhv_types(0),
set_behavior(bhvMacroUkiki, ukiki_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE),
    set_holdable(HOLD_ACTIONS_ALL, 12, 1.f, 3, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_hitbox(50, 80, 0, 0, 0, true), /* 40, 40, 0, 0, 0 */
    set_capture(ukiki, 1.00f, 32, 48, 48, 40, -75, -4.00f, 60, 90, 0, 40, smod_none, true),
set_behavior(bhvMadPiano, mad_piano_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_MAD_PIANO_CHOMPING, 0x40, 0x40, 0x40),
    set_perry_atk(5, -1),
    set_capture(mad_piano, 1.00f, 20, 20, 20, 14, -80, -2.00f, 200, 150, 0, 50, smod_none, true),
set_behavior(bhvMantaRay, NULL),
    set_bhv_types(0),
set_behavior(bhvMantaRayRingManager, NULL),
    set_bhv_types(0),
set_behavior(bhvMantaRayWaterRing, NULL),
    set_bhv_types(BHV_TYPE_WATER_RING),
set_behavior(bhvMario, mario_geo),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvMenuButton, NULL),
    set_bhv_types(0),
set_behavior(bhvMenuButtonManager, NULL),
    set_bhv_types(0),
set_behavior(bhvMerryGoRound, NULL),
    set_bhv_types(0),
set_behavior(bhvMerryGoRoundBigBoo, boo_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_BOSS | BHV_TYPE_BOO),
    set_boss(BOSS_TYPE_BIG_BOO),
    set_perry_atk(8, 4),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED_BIG_BOO, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_hitbox(80, 100, 40, 60, 0, true), /* 140, 80, 40, 60, 0 */
set_behavior(bhvMerryGoRoundBoo, boo_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_BOO),
    set_destroy_boo(DESTROY_COND_NONE, SOUND_OBJ_DYING_ENEMY1),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(80, 100, 40, 60, 0, true), /* 140, 80, 40, 60, 0 */
    set_capture(boo, 1.00f, 24, 48, 48, 20, -20, -2.00f, 80, 120, 40, 50, smod_none, true),
set_behavior(bhvMerryGoRoundBooManager, NULL),
    set_bhv_types(0),
set_behavior(bhvMeshElevator, NULL),
    set_bhv_types(0),
set_behavior(bhvMessagePanel, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvMetalCap, marios_metal_cap_geo),
    set_bhv_types(BHV_TYPE_CAP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_CAP),
set_behavior(bhvMips, mips_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE),
    set_holdable(HOLD_ACTIONS_ALL, 4, 1.f, 1, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE_MIPS, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(mips, 1.00f, 40, 60, 60, 40, -75, -4.00f, 60, 120, 0, 30, smod_none, true),
set_behavior(bhvMistCircParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvMistParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvMoatGrills, NULL),
    set_bhv_types(0),
set_behavior(bhvMoneybag, moneybag_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_destroy_white_puff(DESTROY_COND_NONE, 5, SOUND_GENERAL_SPLATTERING),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, 4, 1.f, 2, 1.f, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(moneybag, 1.00f, 32, 32, 32, 50, -75, -4.00f, 80, 120, 0, 55, smod_none, true),
set_behavior(bhvMoneybagHidden, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
set_behavior(bhvMontyMole, monty_mole_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_holdable(HOLD_ACTIONS_ALL, 1, 1.f, 7, 1.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(monty_mole, 1.50f, 30, 30, 30, 0, -75, -4.00f, 75, 75, 0, 45, smod_none, true),
set_behavior(bhvMontyMoleHole, NULL),
    set_bhv_types(0),
set_behavior(bhvMontyMoleRock, NULL),
    set_bhv_types(BHV_TYPE_METAL_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvMovingBlueCoin, blue_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvMovingYellowCoin, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvMrBlizzard, mr_blizzard_geo),
    set_bhv_types(BHV_TYPE_STRONG | BHV_TYPE_METAL_DESTRUCTIBLE),
    set_destroy_snow_particles(DESTROY_COND_NONE, 3, SOUND_OBJ_DEFAULT_DEATH),
    set_perry_atk(4, -1),
    set_holdable(HOLD_ACTIONS_ALL, 1, 1.5f, 0, 1.f, HOLD_HIT_ANY, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(mr_blizzard, 1.00f, 25, 25, 25, 50, -75, -4.00f, 80, 200, 0, 55, smod_none, true),
set_behavior(bhvMrBlizzardSnowball, NULL),
    set_bhv_types(BHV_TYPE_METAL_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvMrI, mr_i_geo),
    set_bhv_types(BHV_TYPE_STRONG | BHV_TYPE_METAL_DESTRUCTIBLE),
    set_destroy_white_puff(DESTROY_COND_NONE, DESTROY_COINS_BLUE, SOUND_OBJ_MRI_DEATH),
    set_destroy_big_mr_i(DESTROY_COND_BIG_MR_I, SOUND_OBJ_MRI_DEATH),
    set_perry_atk(4, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(mr_i, 1.00f, 30, 30, 30, 30, -40, -2.00f, 90, 180, 0, 65, smod_none, true),
set_behavior(bhvMrIBlueCoin, blue_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvMrIBody, mr_i_iris_geo),
    set_bhv_types(0),
set_behavior(bhvMrIParticle, NULL),
    set_bhv_types(BHV_TYPE_METAL_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvNormalCap, marios_cap_geo),
    set_bhv_types(BHV_TYPE_CAP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_CAP),
set_behavior(bhvObjectBubble, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvObjectWaterSplash, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvObjectWaterWave, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvObjectWaveTrail, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvOctagonalPlatformRotating, NULL),
    set_bhv_types(0),
set_behavior(bhvOneCoin, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvOpenableCageDoor, NULL),
    set_bhv_types(0),
set_behavior(bhvOpenableGrill, NULL),
    set_bhv_types(0),
set_behavior(bhvOrangeNumber, NULL),
    set_bhv_types(0),
set_behavior(bhvPaintingDeathWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvPaintingStarCollectWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvPenguinBaby, penguin_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(penguin_small, 1.00f, 20, 20, 50, 30, -60, -3.60f, 40, 60, 0, 30, smod_none, true),
set_behavior(bhvPenguinRaceFinishLine, NULL),
    set_bhv_types(0),
set_behavior(bhvPenguinRaceShortcutCheck, NULL),
    set_bhv_types(0),
set_behavior(bhvPillarBase, NULL),
    set_bhv_types(0),
set_behavior(bhvPiranhaPlant, piranha_plant_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_destroy_white_puff_and_children(DESTROY_COND_NONE, DESTROY_COINS_BLUE_ONCE, SOUND_OBJ_DEFAULT_DEATH, bhvPiranhaPlantBubble),
    set_perry_atk(3, PIRANHA_PLANT_ACT_WAIT_TO_RESPAWN),
    set_holdable(HOLD_ACTIONS_ALL, 9, 2.f, 9, 2.f, HOLD_HIT_ANY, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
set_behavior(bhvPiranhaPlantBubble, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvPiranhaPlantWakingBubbles, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvPitBowlingBall, bowling_ball_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_DEFEATED_IF_DESTROYED | BHV_TYPE_METAL_BALL),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x40, 0x40, 0x40),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(100, 200, 0, 0, 0, true), /* 100, 150, 0, 0, 0 */
set_behavior(bhvPlatformOnTrack, NULL),
    set_bhv_types(0),
set_behavior(bhvPlaysMusicTrackWhenTouched, NULL),
    set_bhv_types(0),
set_behavior(bhvPlungeBubble, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvPokey, NULL),
    set_bhv_types(BHV_TYPE_BOSS | BHV_TYPE_INVULNERABLE),
    set_destroy_pokey(DESTROY_COND_NONE, DESTROY_COINS_BLUE, SOUND_OBJ_DEFAULT_DEATH),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_WEAK),
    set_capture(pokey, 1.00f, 20, 30, 40, 30, -75, -4.00f, 75, 120, 0, 55, smod_none, true),
set_behavior(bhvPokeyBodyPart, pokey_body_part_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_NOT_ENEMY),
    set_destroy_pokey(DESTROY_COND_NONE, DESTROY_COINS_BLUE, SOUND_OBJ_DEFAULT_DEATH),
    set_perry_atk(1, -1),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
    set_hitbox(30, 40, 20, 20, 0, true), /* 40, 20, 20, 20, 10 */
    set_capture(pokey, 1.00f, 20, 30, 40, 30, -75, -4.00f, 75, 120, 0, 55, smod_none, true),
set_behavior(bhvPoleGrabbing, NULL),
    set_bhv_types(0),
set_behavior(bhvPoundTinyStarParticle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvPunchTinyTriangle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvPurpleParticle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvPurpleSwitchHiddenBoxes, NULL),
    set_bhv_types(0),
set_behavior(bhvPushableMetalBox, metal_box_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvPyramidElevator, NULL),
    set_bhv_types(0),
set_behavior(bhvPyramidElevatorTrajectoryMarkerBall, NULL),
    set_bhv_types(0),
set_behavior(bhvPyramidPillarTouchDetector, NULL),
    set_bhv_types(0),
    set_hitbox(100, 200, 0, 0, 0, false), /* 50, 50, 0, 0, 0 */
set_behavior(bhvPyramidTop, NULL),
    set_bhv_types(0),
set_behavior(bhvPyramidTopFragment, NULL),
    set_bhv_types(0),
set_behavior(bhvRacingPenguin, NULL),
    set_bhv_types(0),
set_behavior(bhvRandomAnimatedTexture, NULL),
    set_bhv_types(0),
set_behavior(bhvRecoveryHeart, NULL),
    set_bhv_types(0),
set_behavior(bhvRedCoin, red_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvRedCoinStarMarker, NULL),
    set_bhv_types(0),
set_behavior(bhvRespawner, NULL),
    set_bhv_types(0),
set_behavior(bhvRockSolid, NULL),
    set_bhv_types(0),
set_behavior(bhvRotatingCounterClockwise, NULL),
    set_bhv_types(0),
set_behavior(bhvRotatingExclamationMark, NULL),
    set_bhv_types(0),
set_behavior(bhvRotatingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvRrCruiserWing, NULL),
    set_bhv_types(0),
set_behavior(bhvRrElevatorPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvRrRotatingBridgePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvSLSnowmanWind, NULL),
    set_bhv_types(0),
set_behavior(bhvSLWalkingPenguin, NULL),
    set_bhv_types(0),
set_behavior(bhvSandSoundLoop, NULL),
    set_bhv_types(0),
set_behavior(bhvScuttlebug, scuttlebug_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 3.f, 0, 3.f, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(100, 70, 90, 60, 0, true), /* 130, 70, 90, 60, 0 */
    set_capture(scuttlebug, 1.00f, 20, 40, 40, 36, -75, -4.00f, 80, 90, 0, 55, smod_none, true),
set_behavior(bhvScuttlebugSpawn, NULL),
    set_bhv_types(0),
set_behavior(bhvSeaweed, NULL),
    set_bhv_types(0),
set_behavior(bhvSeaweedBundle, NULL),
    set_bhv_types(0),
set_behavior(bhvSeesawPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvShallowWaterSplash, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvShallowWaterWave, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvShipPart3, NULL),
    set_bhv_types(0),
set_behavior(bhvSignOnWall, NULL),
    set_bhv_types(0),
set_behavior(bhvSingleCoinGetsSpawned, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvSkeeter, skeeter_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 2, 2.f, 2, 2.f, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(180, 120, 150, 90, 0, true), /* 180, 100, 150, 90, 20 */
    set_capture(skeeter, 1.00f, 24, 36, 72, 36, -75, -4.00f, 120, 120, 0, 80, smod_none, true),
set_behavior(bhvSkeeterWave, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSlidingPlatform2, NULL),
    set_bhv_types(0),
set_behavior(bhvSlidingSnowMound, NULL),
    set_bhv_types(0),
set_behavior(bhvSmallBomp, NULL),
    set_bhv_types(0),
set_behavior(bhvSmallBully, bully_geo),
    set_bhv_types(BHV_TYPE_BULLY),
    set_destroy_bully(DESTROY_COND_NONE, BULLY_ACT_LAVA_DEATH),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 2.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(bully, 1.00f, 28, 28, 56, 36, -75, -4.00f, 75, 120, 0, 50, smod_none, true),
set_behavior(bhvSmallChillBully, chilly_chief_geo),
    set_bhv_types(BHV_TYPE_BULLY),
    set_destroy_bully(DESTROY_COND_NONE, BULLY_ACT_LAVA_DEATH),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 2.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(bully, 1.00f, 28, 28, 56, 36, -75, -4.00f, 75, 120, 0, 50, smod_none, true),
set_behavior(bhvSmallParticle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSmallParticleBubbles, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSmallParticleSnow, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSmallPenguin, penguin_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(penguin_small, 1.00f, 20, 20, 50, 30, -60, -3.60f, 40, 60, 0, 30, smod_none, true),
set_behavior(bhvSmallPiranhaFlame, red_flame_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvSmallWaterWave, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSmallWaterWave398, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSmallWhomp, whomp_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL | BHV_TYPE_SPARKLY_ENEMY),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0xE0, 0xE0, 0xE0),
    set_perry_atk(5, -1),
    set_capture(whomp, 1.00f, 20, 20, 20, 36, -90, -4.50f, 100, 400, 0, 150, smod_none, true),
set_behavior(bhvSmoke, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSnowBall, NULL),
    set_bhv_types(0),
set_behavior(bhvSnowMoundSpawn, NULL),
    set_bhv_types(0),
set_behavior(bhvSnowParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSnowmansBodyCheckpoint, NULL),
    set_bhv_types(0),
set_behavior(bhvSnowmansBottom, ccm_geo_0003F0),
    set_bhv_types(BHV_TYPE_INVULNERABLE | BHV_TYPE_METAL_DESTRUCTIBLE),
    set_destroy_big_snowball(DESTROY_COND_NONE, 0, SOUND_OBJ_DEFAULT_DEATH),
    set_capture(snowmans_body, 1.00f, 80, 80, 80, 0, -90, -6.00f, 200, 400, 0, 200, smod_none, true),
set_behavior(bhvSnowmansHead, NULL),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
set_behavior(bhvSnufit, snufit_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(80, 100, 70, 50, 40, true), /* 100, 60, 70, 50, 0 */
    set_capture(snufit, 1.00f, 25, 25, 25, 36, -45, -3.00f, 60, 100, 0, 35, smod_none, true),
set_behavior(bhvSnufitBalls, NULL),
    set_bhv_types(0),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvSoundSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSparkle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSparkleParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSparkleSpawn, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvSpawnedStar, star_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_STAR_OR_KEY),
set_behavior(bhvSpawnedStarNoLevelExit, star_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_STAR_OR_KEY),
set_behavior(bhvSpinAirborneCircleWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvSpinAirborneWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvSpindel, ssl_geo_000764),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0xFF, 0xFF, 0xA0),
    set_perry_atk(5, -1),
set_behavior(bhvSpindrift, spindrift_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 0, 1.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(90, 150, 80, 70, 0, true), /* 90, 80, 80, 70, 0 */
    set_capture(spindrift, 1.00f, 28, 28, 28, 40, -75, -4.00f, 80, 160, 0, 40, smod_none, true),
set_behavior(bhvSpiny, spiny_geo),
    set_bhv_types(BHV_TYPE_STRONG | BHV_TYPE_METAL_DESTRUCTIBLE),
    set_destroy_white_puff(DESTROY_COND_NONE, 0, SOUND_OBJ_DEFAULT_DEATH),
    set_perry_atk(2, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 2.f, 0, 2.f, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(80, 60, 40, 40, 0, true), /* 80, 50, 40, 40, 0 */
    set_capture(spiny, 1.00f, 75, 75, 150, 48, -75, -4.00f, 60, 80, 0, 40, smod_none, true),
set_behavior(bhvSquarishPathMoving, NULL),
    set_bhv_types(0),
set_behavior(bhvSquishablePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvSslMovingPyramidWall, NULL),
    set_bhv_types(0),
set_behavior(bhvStar, star_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_STAR_OR_KEY),
set_behavior(bhvStarDoor, NULL),
    set_bhv_types(BHV_TYPE_DOOR),
set_behavior(bhvStarKeyCollectionPuffSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvStarSpawnCoordinates, star_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_STAR_OR_KEY),
set_behavior(bhvStaticCheckeredPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvStaticObject, NULL),
#if OMM_GAME_IS_SM64
    set_bhv_types(BHV_TYPE_STAR_MODEL), // Grand star during ending cutscene
#else
    set_bhv_types(0),
#endif
set_behavior(bhvStrongWindParticle, NULL),
    set_bhv_types(0),
set_behavior(bhvStub, NULL),
    set_bhv_types(0),
set_behavior(bhvStub1D0C, NULL),
    set_bhv_types(0),
set_behavior(bhvStub1D70, NULL),
    set_bhv_types(0),
set_behavior(bhvSunkenShipPart, NULL),
    set_bhv_types(0),
set_behavior(bhvSunkenShipPart2, NULL),
    set_bhv_types(0),
set_behavior(bhvSushiShark, sushi_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
    set_hitbox(150, 200, 0, 0, 80, true), /* 100, 50, 0, 0, 50 */
    set_capture(sushi_shark, 1.00f, 45, 60, 90, 0, 0, 0.00f, 150, 200, 80, 120, smod_none, false),
set_behavior(bhvSushiSharkCollisionChild, NULL),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
set_behavior(bhvSwimmingWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvSwingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvSwoop, swoop_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 2.f, 0, 2.f, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(80, 80, 70, 70, 0, true), /* 100, 80, 70, 70, 0 */
    set_capture(swoop, 1.00f, 20, 40, 40, 24, -20, -2.00f, 50, 60, 0, 30, smod_none, true),
set_behavior(bhvTTC2DRotator, NULL),
    set_bhv_types(0),
set_behavior(bhvTTCCog, NULL),
    set_bhv_types(0),
set_behavior(bhvTTCElevator, NULL),
    set_bhv_types(0),
set_behavior(bhvTTCMovingBar, NULL),
    set_bhv_types(0),
set_behavior(bhvTTCPendulum, NULL),
    set_bhv_types(0),
set_behavior(bhvTTCPitBlock, NULL),
    set_bhv_types(0),
set_behavior(bhvTTCRotatingSolid, NULL),
    set_bhv_types(0),
set_behavior(bhvTTCSpinner, NULL),
    set_bhv_types(0),
set_behavior(bhvTTCTreadmill, NULL),
    set_bhv_types(0),
set_behavior(bhvTankFishGroup, NULL),
    set_bhv_types(0),
set_behavior(bhvTemporaryYellowCoin, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvTenCoinsSpawn, NULL),
    set_bhv_types(0),
set_behavior(bhvThiBowlingBallSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvThiHugeIslandTop, NULL),
    set_bhv_types(0),
set_behavior(bhvThiTinyIslandTop, NULL),
    set_bhv_types(0),
set_behavior(bhvThreeCoinsSpawn, NULL),
    set_bhv_types(0),
set_behavior(bhvThwomp, thwomp_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0x40, 0x80, 0xFF),
    set_perry_atk(5, -1),
    set_capture(thwomp, 1.40f, 35, 35, 35, 50, -90, -4.50f, 210, 420, 0, 170, smod_none, true),
set_behavior(bhvThwomp2, thwomp_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0x40, 0x80, 0xFF),
    set_perry_atk(5, -1),
    set_capture(thwomp, 1.40f, 35, 35, 35, 50, -90, -4.50f, 210, 420, 0, 170, smod_none, true),
set_behavior(bhvTiltingBowserLavaPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvTinyStrongWindParticle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvToadMessage, toad_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 6, 1.f, 6, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE_TOAD, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(toad, 1.00f, 18, 36, 36, 28, -75, -4.00f, 60, 100, 0, 30, smod_none, false),
set_behavior(bhvTower, NULL),
    set_bhv_types(0),
set_behavior(bhvTowerDoor, wf_geo_000BE0),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_DOOR | BHV_TYPE_NOT_ENEMY),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0xFF, 0xFF, 0xFF),
    set_perry_atk(5, -1),
set_behavior(bhvTowerPlatformGroup, NULL),
    set_bhv_types(0),
set_behavior(bhvToxBox, ssl_geo_000630),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0x80, 0x10, 0x00),
    set_perry_atk(5, -1),
set_behavior(bhvTrackBall, NULL),
    set_bhv_types(0),
set_behavior(bhvTreasureChestBottom, NULL),
    set_bhv_types(BHV_TYPE_TREASURE_CHEST),
set_behavior(bhvTreasureChestTop, NULL),
    set_bhv_types(BHV_TYPE_TREASURE_CHEST),
set_behavior(bhvTreasureChests, NULL),
    set_bhv_types(BHV_TYPE_TREASURE_CHEST),
set_behavior(bhvTreasureChestsJrb, NULL),
    set_bhv_types(BHV_TYPE_TREASURE_CHEST),
set_behavior(bhvTreasureChestsShip, NULL),
    set_bhv_types(BHV_TYPE_TREASURE_CHEST),
set_behavior(bhvTree, NULL),
    set_bhv_types(0),
set_behavior(bhvTreeLeaf, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvTreeSnow, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvTriangleParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvTripletButterfly, NULL),
    set_bhv_types(0),
set_behavior(bhvTtmBowlingBallSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvTtmRollingLog, NULL),
    set_bhv_types(0),
set_behavior(bhvTumblingBridgePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvTuxiesMother, penguin_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvTweester, NULL),
    set_bhv_types(0),
set_behavior(bhvTweesterSandParticle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvUkiki, ukiki_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE),
    set_holdable(HOLD_ACTIONS_ALL, 12, 1.f, 3, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_hitbox(50, 80, 0, 0, 0, true), /* 40, 40, 0, 0, 0 */
    set_capture(ukiki, 1.00f, 32, 48, 48, 40, -75, -4.00f, 60, 90, 0, 40, smod_none, true),
set_behavior(bhvUkikiCage, NULL),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_ukiki_cage(DESTROY_COND_NONE),
    set_perry_atk(5, -1),
set_behavior(bhvUkikiCageChild, NULL),
    set_bhv_types(0),
set_behavior(bhvUkikiCageStar, star_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL),
set_behavior(bhvUnagi, NULL),
    set_bhv_types(0),
set_behavior(bhvUnagiSubobject, NULL),
    set_bhv_types(BHV_TYPE_UNAGIS_TAIL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_ATTACK),
set_behavior(bhvUnlockDoorStar, NULL),
    set_bhv_types(BHV_TYPE_STAR_MODEL),
set_behavior(bhvUnused05A8, NULL),
    set_bhv_types(0),
set_behavior(bhvUnused0DFC, NULL),
    set_bhv_types(0),
set_behavior(bhvUnused1820, NULL),
    set_bhv_types(0),
set_behavior(bhvUnused20E0, penguin_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE),
    set_holdable(HOLD_ACTIONS_ALL, 3, 1.f, 1, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture(penguin_small, 1.00f, 20, 20, 50, 30, -60, -3.60f, 40, 60, 0, 30, smod_none, true),
set_behavior(bhvUnused2A10, NULL),
    set_bhv_types(0),
set_behavior(bhvUnused2A54, NULL),
    set_bhv_types(0),
set_behavior(bhvUnusedFakeStar, star_geo),
    set_bhv_types(BHV_TYPE_STAR_MODEL),
set_behavior(bhvUnusedParticleSpawn, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvUnusedPoundablePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvVanishCap, marios_cap_geo),
    set_bhv_types(BHV_TYPE_CAP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_CAP),
set_behavior(bhvVertStarParticleSpawner, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvVolcanoFlames, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvVolcanoSoundLoop, NULL),
    set_bhv_types(0),
set_behavior(bhvWallTinyStarParticle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWarp, NULL),
    set_bhv_types(0),
set_behavior(bhvWarpPipe, warp_pipe_geo),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvWaterAirBubble, NULL),
    set_bhv_types(0),
set_behavior(bhvWaterBomb, water_bomb_geo),
    set_bhv_types(BHV_TYPE_STRONG | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_NOT_ENEMY),
    set_destroy_water_bomb(DESTROY_COND_NONE, SOUND_OBJ_DIVING_IN_WATER),
    set_perry_atk(3, -1),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvWaterBombCannon, NULL),
    set_bhv_types(0),
set_behavior(bhvWaterBombShadow, NULL),
    set_bhv_types(0),
set_behavior(bhvWaterBombSpawner, NULL),
    set_bhv_types(0),
set_behavior(bhvWaterDroplet, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWaterDropletSplash, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWaterLevelDiamond, NULL),
    set_bhv_types(BHV_TYPE_WATER_DIAMOND),
    set_hitbox(70, 100, 0, 0, 0, false), /* 70, 30, 0, 0, 0 */
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_ATTACK),
set_behavior(bhvWaterLevelPillar, NULL),
    set_bhv_types(0),
set_behavior(bhvWaterMist, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWaterMist2, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWaterSplash, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWaterfallSoundLoop, NULL),
    set_bhv_types(0),
set_behavior(bhvWaveTrail, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWdwExpressElevator, NULL),
    set_bhv_types(0),
set_behavior(bhvWdwExpressElevatorPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvWdwRectangularFloatingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvWdwSquareFloatingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvWfBreakableWallLeft, wf_geo_000B90),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NOT_ENEMY),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x80, 0x60, 0x40),
    set_perry_atk(5, -1),
set_behavior(bhvWfBreakableWallRight, wf_geo_000B78),
    set_bhv_types(BHV_TYPE_WF_STAR_WALL | BHV_TYPE_NOT_ENEMY),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x80, 0x60, 0x40),
    set_perry_atk(5, -1),
set_behavior(bhvWfElevatorTowerPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvWfRotatingWoodenPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvWfSlidingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvWfSlidingTowerPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvWfSolidTowerPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvWfTumblingBridge, NULL),
    set_bhv_types(0),
set_behavior(bhvWhirlpool, NULL),
    set_bhv_types(0),
set_behavior(bhvWhitePuff1, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWhitePuff2, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWhitePuffExplosion, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWhitePuffSmoke, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWhitePuffSmoke2, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWhompKingBoss, whomp_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_BOSS | BHV_TYPE_NO_CAM_COL),
    set_boss(BOSS_TYPE_KING_WHOMP),
    set_destroy_whomp_king(DESTROY_COND_NONE),
    set_perry_atk(8, 9),
set_behavior(bhvWigglerBody, wiggler_body_geo),
    set_bhv_types(0),
set_behavior(bhvWigglerHead, wiggler_head_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_BOSS),
    set_boss(BOSS_TYPE_WIGGLER),
    set_perry_atk(8, WIGGLER_ACT_FALL_THROUGH_FLOOR),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_ATTACK),
set_behavior(bhvWind, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvWingCap, marios_wing_cap_geo),
    set_bhv_types(BHV_TYPE_CAP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_CAP),
set_behavior(bhvWoodenPost, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
    set_destroy_wooden_post(DESTROY_COND_NONE, SOUND_GENERAL_BREAK_BOX),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvYellowBackgroundInMenu, NULL),
    set_bhv_types(0),
set_behavior(bhvYellowBall, NULL),
    set_bhv_types(0),
set_behavior(bhvYellowCoin, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvYoshi, yoshi_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 1, 2.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
#if OMM_GAME_IS_SMSR
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture_npc(200),
#else
    set_capture(yoshi, 1.00f, 42, 42, 56, 40, -75, -4.00f, 80, 180, 0, 50, smod_none, true),
#endif

//
// Moonshine
//

#if OMM_GAME_IS_SMMS
set_behavior(bhvBee, Bee_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_hitbox(70, 120, 50, 120, 0, true), /* 70, 60, 40, 50, 0 */
    set_capture(bee, 1.50f, 28, 28, 28, 40, -75, -4.00f, 80, 140, 0, 40, smod_none, true),
set_behavior(bhvgoombone, goombone_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_GOOMBA),
    set_destroy_white_puff(DESTROY_COND_TINY_GOOMBA, 1, SOUND_OBJ_ENEMY_DEATH_HIGH),
    set_destroy_white_puff(DESTROY_COND_REGULAR_GOOMBA, 1, SOUND_OBJ_ENEMY_DEATH_HIGH),
    set_destroy_white_puff(DESTROY_COND_HUGE_GOOMBA, DESTROY_COINS_BLUE, SOUND_OBJ_ENEMY_DEATH_LOW),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 2.f, 0, 2.f, HOLD_HIT_ANY, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(goomba, 1.50f, 21, 42, 42, 30, -75, -4.00f, 75, 95, 0, 45, smod_goomba, true),
#endif

//
// Render96
//

#if OMM_GAME_IS_R96X
set_behavior(bhvBetaBooKey, small_key_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 80, 0, 0, 0, false), /* 32, 64, 0, 0, 0 */
set_behavior(bhvWarioCoin, wario_coin_mario_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(50, 100, 0, 0, 0, false), /* 32, 64, 0, 0, 0 */
set_behavior(bhvMovingYellowCoinWario, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvBlueCoinMotos, blue_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvMotosHand, NULL),
    set_bhv_types(0),
set_behavior(bhvMotos, motos_geo),
    set_bhv_types(BHV_TYPE_BULLY | BHV_TYPE_GRABBABLE),
    set_destroy_bully(DESTROY_COND_NONE, MOTOS_ACT_DEATH),
    set_perry_atk(4, -1),
    set_holdable(HOLD_ACTIONS_ALL, 1, 1.5f, 1, 1.5f, HOLD_HIT_FLOOR, 3, OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_hitbox(100, 150, 0, 0, 0, false), /* 100, 100, 0, 0, 0 */
    set_capture(motos, 2.00f, 24, 24, 24, 32, -90, -4.50f, 100, 220, 0, 80, smod_none, true),
set_behavior(bhvYoshiEgg, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvBlargg, blargg_geo),
    set_bhv_types(BHV_TYPE_FLAME),
    set_perry_atk(4, -1),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
    set_capture(blargg, 1.50f, 30, 30, 30, 0, 0, 0.00f, 180, 150, 0, 70, smod_none, true),
set_behavior(bhvFriendlyBlargg, blargg_geo),
    set_bhv_types(0),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
    set_capture(friendly_blargg, 2.00f, 60, 90, 90, 0, 0, 0.00f, 120, 100, 0, 70, smod_none, true),
set_behavior(bhvCharacterSwitchPipe, warp_pipe_boo_red_geo),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvCharacterSwitchPipeWarioLocked, warp_pipe_boo_yellow_locked_geo),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvCharacterSwitchPipeWarioUnlocked, warp_pipe_boo_yellow_unlocked_geo),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvCharacterSwitchPipeLuigiLocked, warp_pipe_boo_green_locked_geo),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvCharacterSwitchPipeLuigiUnlocked, warp_pipe_boo_green_unlocked_geo),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvMilk, milk_bottle_geo),
    set_bhv_types(BHV_TYPE_MUSHROOM_1UP),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_MUSHROOM_1UP),
    set_hitbox(40, 80, 0, 0, 0, false), /* 30, 30, 0, 0, 0 */
set_behavior(bhvSpambaYellowCoin, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvSpambaRedCoin, red_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvSpambaBlueCoin, blue_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
#endif

//
// Star Road
//

#if OMM_GAME_IS_SMSR
set_behavior(bhvCustomSMSRBreakableRock, NULL),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_triangle_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_BREAK_BOX, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_30),
    set_perry_atk(4, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvCustomSMSRWoodenOctagonalPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRShyGuy, flyguy_geo),
    set_bhv_types(BHV_TYPE_WEAK | BHV_TYPE_GOOMBA),
    set_destroy_white_puff(DESTROY_COND_TINY_GOOMBA, 1, SOUND_OBJ_ENEMY_DEATH_HIGH),
    set_destroy_white_puff(DESTROY_COND_REGULAR_GOOMBA, 1, SOUND_OBJ_ENEMY_DEATH_HIGH),
    set_destroy_white_puff(DESTROY_COND_HUGE_GOOMBA, DESTROY_COINS_BLUE, SOUND_OBJ_ENEMY_DEATH_LOW),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, 0, 4.f, 0, 4.f, HOLD_HIT_ANY, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(goomba, 1.50f, 21, 42, 42, 30, -75, -4.00f, 75, 95, 0, 45, smod_goomba, true),
set_behavior(bhvCustomSMSRBreakableWindow, NULL),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NOT_ENEMY),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0xFF, 0xFF, 0xFF),
    set_perry_atk(4, -1),
set_behavior(bhvCustomSMSRStarReplica, star_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_STAR_OR_KEY),
set_behavior(bhvCustomSMSRRedOctagonalPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRRedSinkingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRRedWavePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRPushableTomb, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvCustomSMSRLampPath, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRRotatingDonutPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRWindTurbine, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRLightsOnSwitch, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRRotatingWoodenGear, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRPeachMessage, peach_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 4, 1.f, 4, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture_npc(180),
set_behavior(bhvCustomSMSRYoshiMessage, yoshi_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 1, 2.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture_npc(200),
set_behavior(bhvCustomSMSRStaticMessage, boo_geo),
    set_bhv_types(BHV_TYPE_BOO),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture_npc(50),
set_behavior(bhvCustomSMSRStoneCubePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSR30StarDoorWall, NULL),
    set_bhv_types(BHV_TYPE_DOOR),
set_behavior(bhvCustomSMSRSpecialBreakeableBox, NULL),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_triangle_particles(DESTROY_COND_NONE, DESTROY_COINS_REMAINING, SOUND_GENERAL_BREAK_BOX, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_30),
    set_perry_atk(5, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_DESTROY, OBJ_INT_PRESET_ATTACK_STRONG),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
set_behavior(bhvCustomSMSRRecoveryBubbleWater, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRBreakableFloor, NULL),
    set_bhv_types(BHV_TYPE_BREAKABLE),
    set_perry_atk(4, -1),
set_behavior(bhvCustomSMSRBoatOnTrack, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRAngrySun, NULL),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvCustomSMSRYoshiCoin, yellow_coin_geo),
    set_bhv_types(BHV_TYPE_COIN),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_COIN),
set_behavior(bhvCustomSMSRPiranhaPlantWild, piranha_plant_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
set_behavior(bhvCustomSMSRMovingMushroom, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRMipsMessage, mips_geo),
    set_bhv_types(0),
    set_holdable(HOLD_ACTIONS_ALL, 4, 1.f, 1, 1.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
    set_capture_npc(65),
set_behavior(bhvCustomSMSRBigLeaves, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRLilyPad, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRInvisibleCannon, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRTambourine, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRSmallBee, yoshi_egg_geo),
    set_bhv_types(BHV_TYPE_WEAK),
    set_perry_atk(3, -1),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_ANY, HOLD_ACT_ATTACKED, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DEFAULT),
    set_capture(bee, 1.50f, 28, 28, 28, 40, -75, -4.00f, 80, 80, 0, 40, smod_none, true),
set_behavior(bhvCustomSMSRDrumStick, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvCustomSMSRStarMoving, star_geo),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_STAR_OR_KEY),
set_behavior(bhvCustomSMSRFallingDomino, NULL),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 0, SOUND_GENERAL_WALL_EXPLOSION, 0x40, 0x40, 0x40),
    set_perry_atk(5, -1),
set_behavior(bhvCustomSMSRLavaLift, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRRotatingLavaPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRSmallSwingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRSinkingDonut, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRFloatingThwomp, thwomp_geo),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0x40, 0x80, 0xFF),
    set_perry_atk(5, -1),
    set_capture(thwomp, 1.00f, 30, 30, 30, 60, -90, -3.00f, 150, 300, 0, 120, smod_none, true),
set_behavior(bhvCustomSMSRTiltingPyramid, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRPlatformLift, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRRisingLava, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRRisingTallPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRSinkingPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRToxicWastePlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRAttractedSpaceBox, NULL),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0x80, 0x10, 0x00),
    set_perry_atk(5, -1),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvCustomSMSRSpaceBox, NULL),
    set_bhv_types(BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_NO_CAM_COL),
    set_destroy_break_particles(DESTROY_COND_NONE, 5, SOUND_OBJ_THWOMP, 0x80, 0x10, 0x00),
    set_perry_atk(5, -1),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
set_behavior(bhvCustomSMSRSpaceOctagonPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRSpaceRedPlatform, NULL),
    set_bhv_types(0),
set_behavior(bhvCustomSMSRBulletMine, bowser_bomb_geo),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_METAL_DESTRUCTIBLE | BHV_TYPE_DEFEATED_IF_DESTROYED),
    set_destroy_explosion_and_respawn(DESTROY_COND_NONE, 0, MODEL_WATER_MINE, 1000),
    set_perry_atk(2, -1),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_DESTROY),
    set_hitbox(40, 80, 0, 0, 40, true), /* 50, 50, 0, 0, 50 */
set_behavior(bhvCustomSMSRPlayMusicAt130Stars, NULL),
    set_bhv_types(0),
#endif

//
// Common
//

set_behavior(bhvOmmDummy, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmBlueCoinsNumber, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmStarNumber, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmLifeUp, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmActSelectStar, star_geo),
    set_bhv_types(BHV_TYPE_STAR_MODEL),
set_behavior(bhvOmmStarCelebration, star_geo),
    set_bhv_types(BHV_TYPE_STAR_MODEL),
set_behavior(bhvOmmWfTransition, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmWallWarp, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),

//
// Particles
//

set_behavior(bhvOmmFireSmoke, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvOmmBreakParticle, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvOmmWhirlpool, NULL),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),

//
// Mario
//

set_behavior(bhvOmmGrab, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmDamageMario, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmCappy, marios_cap_geo),
    set_bhv_types(BHV_TYPE_CAP | BHV_TYPE_PLAYER),
set_behavior(bhvOmmPossessedObject, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmPossessedObjectCap, marios_cap_geo),
    set_bhv_types(BHV_TYPE_CAP | BHV_TYPE_PLAYER),
set_behavior(bhvOmmPossessedKoopaShell, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmWingGlowLeft, NULL),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmWingGlowRight, NULL),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmWingTrailLeft, NULL),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmWingTrailRight, NULL),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmMetalSparkle, NULL),
    set_bhv_types(BHV_TYPE_PLAYER | BHV_TYPE_UNIMPORTANT),
set_behavior(bhvOmmVanishMist, NULL),
    set_bhv_types(BHV_TYPE_PLAYER | BHV_TYPE_UNIMPORTANT),
set_behavior(bhvOmmStatsBoard, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmMainMenuMario, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmMainMenuCappy, NULL),
    set_bhv_types(0),

//
// Captures
//

set_behavior(bhvOmmGoombaStack, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmGoombaStackCapture, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmExplosion, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmChainChompFree, chain_chomp_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
    set_destroy_white_puff_and_children(DESTROY_COND_NONE, 0, SOUND_OBJ_DEFAULT_DEATH, bhvChainChompChainPart),
    set_capture(chain_chomp, 2.00f, 48, 48, 160, 60, -90, -6.00f, 200, 360, 0, 150, smod_none, true),
set_behavior(bhvOmmWhompShockwave, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmSnowball, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmSpindriftShockwave, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmMrIBeam, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmSnufitBall, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmMontyMoleRock, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmDorrie, dorrie_geo),
    set_bhv_types(0),
    set_capture(omm_dorrie, 1.00f, 90, 90, 90, 0, 0, 0.00f, 400, 600, 200, 300, smod_none, true),
set_behavior(bhvOmmBlarggFireDrop, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmBlarggFireTrail, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmBlarggFireball, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmFlyGuyFireball, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmPokeyBall, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmShockwave, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmAmpDischarge, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmFireSpitterFlame, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmMoneybagCoin, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmSpinyBall, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmAfterimage, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmFlamingBobomb, black_bobomb_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
    set_capture(flaming_bobomb, 1.00f, 24, 24, 24, 30, -60, -3.60f, 45, 90, 0, 40, smod_none, false),
set_behavior(bhvOmmFlamingBobombAura, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmFlamingBobombExplosion, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmYoshi, yoshi_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
    set_holdable(HOLD_ACTIONS_ALL, 0, 1.f, 1, 2.f, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_capture(yoshi, 1.00f, 42, 42, 56, 40, -75, -4.00f, 80, 180, 0, 50, smod_none, true),
set_behavior(bhvOmmYoshiBubble, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmYoshiFire, red_flame_geo),
    set_bhv_types(0),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvOmmYoshiFireball, red_flame_geo),
    set_bhv_types(0),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvOmmYoshiEgg, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmYoshiTongue, omm_geo_yoshi_tongue),
    set_bhv_types(0),
set_behavior(bhvOmmYoshiWings, omm_geo_yoshi_wings),
    set_bhv_types(0),
set_behavior(bhvOmmYoshiModeYoshi, yoshi_geo),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmYoshiModeCap, marios_cap_geo),
    set_bhv_types(BHV_TYPE_PLAYER),

//
// Bowser
//

set_behavior(bhvOmmBowser, bowser_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
set_behavior(bhvOmmBowserClone, bowser_geo),
    set_bhv_types(BHV_TYPE_INVULNERABLE),
set_behavior(bhvOmmBowserMine, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmBowserFlame, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmBowserFireball, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmBowserFireballFlame, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmBowserMadAura, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmBowserShockwaveFire, NULL),
    set_bhv_types(0),

//
// Sparkly Stars
//

set_behavior(bhvOmmSparklyStar, omm_geo_sparkly_star_1_opaque),
    set_bhv_types(BHV_TYPE_STAR_OR_KEY),
set_behavior(bhvOmmSparklyStarHint, omm_geo_sparkly_star_1_hint),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
    set_holdable(HOLD_ACTIONS_ALL, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR, HOLD_ACT_BOUNCE, OBJ_INT_PRESET_ATTACK_WEAK),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvOmmSparklyStarBlock1, omm_geo_sparkly_star_1_block),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_NO_CAM_COL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_ATTACK),
set_behavior(bhvOmmSparklyStarBlock2, omm_geo_sparkly_star_2_block),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_NO_CAM_COL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_ATTACK),
set_behavior(bhvOmmSparklyStarBlock3, omm_geo_sparkly_star_3_block),
    set_bhv_types(BHV_TYPE_BREAKABLE | BHV_TYPE_NO_CAM_COL),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_ATTACK),
set_behavior(bhvOmmSparklyStarSparkle, omm_geo_sparkly_star_1_sparkle),
    set_bhv_types(BHV_TYPE_UNIMPORTANT),
set_behavior(bhvOmmSparklyStarSparkleMario, omm_geo_sparkly_star_1_sparkle),
    set_bhv_types(BHV_TYPE_PLAYER | BHV_TYPE_UNIMPORTANT),
set_behavior(bhvOmmSparklyStarCelebration, omm_geo_sparkly_star_1_opaque),
    set_bhv_types(0),
set_behavior(bhvOmmSparklyStarBox, omm_geo_sparkly_star_1_box),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
    set_holdable(HOLD_ACTIONS_0, HOLD_ANIM_NONE, 0, HOLD_ANIM_NONE, 0, HOLD_HIT_FLOOR_OR_WALL, HOLD_ACT_BOUNCE_SPARKLY_STAR_BOX, OBJ_INT_PRESET_ATTACK_SPARKLY_STAR_BOX),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_THROWABLE),
set_behavior(bhvOmmSparklyGrandStarEnding, omm_geo_sparkly_star_1_opaque),
    set_bhv_types(0),
set_behavior(bhvOmmMips, mips_geo),
    set_bhv_types(BHV_TYPE_GRABBABLE | BHV_TYPE_NO_CAPPY),
set_behavior(bhvOmmRisingLava, omm_geo_rising_lava),
    set_bhv_types(0),
set_behavior(bhvOmmStarRing, omm_geo_star_ring),
    set_bhv_types(0),
set_behavior(bhvOmmBigFlame, omm_geo_big_flame),
    set_bhv_types(BHV_TYPE_FLAME),
    set_yoshi_tongue(YOSHI_TONGUE_TYPE_FIRE),
set_behavior(bhvOmmBigFlame2, omm_geo_big_flame_2),
    set_bhv_types(0),
set_behavior(bhvOmmBitfsPillar, NULL),
    set_bhv_types(BHV_TYPE_NO_CAM_COL),
set_behavior(bhvOmmPeachyRoomTrigger, NULL),
    set_bhv_types(0),
set_behavior(bhvOmmProblem, omm_geo_problem),
    set_bhv_types(0),

//
// Peach
//

set_behavior(bhvOmmPeachVibeAura, omm_geo_peach_vibe_aura),
    set_bhv_types(0),
set_behavior(bhvOmmPeachVibeSparkle, omm_geo_peach_vibe_sparkle),
    set_bhv_types(0),
set_behavior(bhvOmmPeachVibeJoyTornado, omm_geo_peach_vibe_joy_tornado),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPeachVibeJoySparkle, omm_geo_peach_vibe_sparkle),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPeachVibeJoyGust, omm_geo_peach_vibe_joy_gust),
    set_bhv_types(0),
set_behavior(bhvOmmPeachVibeRageAura, omm_geo_peach_vibe_rage_aura),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPeachVibeRageShockwave, omm_geo_peach_vibe_rage_shockwave),
    set_bhv_types(0),
set_behavior(bhvOmmPeachVibeGloomAura, omm_geo_peach_vibe_gloom_aura),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPeachVibeGloomTear, omm_geo_peach_vibe_gloom_tear),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPeachVibeGloomTearSmall, omm_geo_peach_vibe_gloom_tear_small),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPeachVibeCalmAura, omm_geo_peach_vibe_calm_aura),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPeachVibeCalmSparkle, omm_geo_peach_vibe_calm_sparkle),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPerry, omm_geo_perry),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPerryTrail, omm_geo_perry_trail),
    set_bhv_types(BHV_TYPE_PLAYER),
set_behavior(bhvOmmPerryShockwave, omm_geo_perry_shockwave),
    set_bhv_types(0),
set_behavior(bhvOmmPerryBlast, omm_geo_perry_blast),
    set_bhv_types(0),
set_behavior(bhvOmmPerryCharge, omm_geo_perry_charge),
    set_bhv_types(BHV_TYPE_PLAYER),
0};

//
// Processed data
//

static OmmArray_(const BehaviorScript *) sBehaviorsList = omm_array_zero;
static OmmHMap_(OmmBhvData *) sOmmBehaviorData = omm_hmap_zero;

static void omm_behavior_data_init() {
    OMM_DO_ONCE {
        OmmBhvData *data = NULL;
        for (const s64 *raw = OMM_BEHAVIOR_DATA; *raw;) {
            switch (*(raw++)) {
                case CMD_BEHAVIOR: {
                    data = mem_new(OmmBhvData, 1);
                    data->bhv = (const BehaviorScript *) *(raw++);
#if OMM_CODE_DEBUG
                    data->geo = (const GeoLayout *) *(raw++);
                    data->bhvName = (const char *) *(raw++);
#endif
                    omm_hmap_insert(sOmmBehaviorData, (uintptr_t) data->bhv, data);
                    omm_array_add(sBehaviorsList, ptr, data->bhv);
                } break;

                case CMD_BHV_TYPES: {
                    data->types = (u64) *(raw++);
                } break;

                case CMD_BOSS: {
                    data->boss = mem_new(OmmBhvDataBoss, 1);
                    data->boss->type = (s32) *(raw++);
                } break;

                case CMD_PERRY_ATK: {
                    data->perryAttack = mem_new(OmmBhvDataPerryAttack, 1);
                    data->perryAttack->vibeSparkles = (s32) *(raw++);
                    data->perryAttack->deathAction = (s32) *(raw++);
                } break;

                case CMD_HOLDABLE: {
                    data->holdable = mem_new(OmmBhvDataHoldable, 1);
                    data->holdable->actions = (s32) *(raw++);
                    data->holdable->heldAnim = (s32) *(raw++);
                    data->holdable->heldAnimAccel = (f32) *(raw++) / 100.f;
                    data->holdable->thrownAnim = (s32) *(raw++);
                    data->holdable->thrownAnimAccel = (f32) *(raw++) / 100.f;
                    data->holdable->hitSurface = (s32) *(raw++);
                    data->holdable->nextAction = (s32) *(raw++);
                    data->holdable->interactionFlags = (u32) *(raw++);
                } break;

                case CMD_HITBOX: {
                    data->hitbox = mem_new(OmmBhvDataHitbox, 1);
                    data->hitbox->hitboxRadius = (f32) *(raw++);
                    data->hitbox->hitboxHeight = (f32) *(raw++);
                    data->hitbox->hurtboxRadius = (f32) *(raw++);
                    data->hitbox->hurtboxHeight = (f32) *(raw++);
                    data->hitbox->hitboxDownOffset = (f32) *(raw++);
                    data->hitbox->scale = (bool) *(raw++);
                } break;

                case CMD_CAPTURE: {
                    data->capture = mem_new(OmmBhvDataCapture, 1);
                    data->capture->init = (void *) *(raw++);
                    data->capture->update = (void *) *(raw++);
                    data->capture->update_gfx = (void *) *(raw++);
                    data->capture->end = (void *) *(raw++);
                    data->capture->get_type = (void *) *(raw++);
                    data->capture->get_top = (void *) *(raw++);
                    f32 baseScale = (f32) *(raw++) / 100.f;
                    data->capture->walkSpeed = (f32) *(raw++) / baseScale;
                    data->capture->runSpeed = (f32) *(raw++) / baseScale;
                    data->capture->dashSpeed = (f32) *(raw++) / baseScale;
                    data->capture->jumpVelocity = (f32) *(raw++) / baseScale;
                    data->capture->terminalVelocity = (f32) *(raw++) / baseScale;
                    data->capture->gravity = (f32) *(raw++) / (baseScale * 100.f);
                    data->capture->hitboxRadius = (f32) *(raw++) / baseScale;
                    data->capture->hitboxHeight = (f32) *(raw++) / baseScale;
                    data->capture->hitboxDownOffset = (f32) *(raw++) / baseScale;
                    data->capture->wallHitboxRadius = (f32) *(raw++) / baseScale;
                    data->capture->scale_modifier = (void *) *(raw++);
                    data->capture->referenceObject = (bool) *(raw++);
                } break;

                case CMD_YOSHI_TONGUE: {
                    data->yoshiTongue = mem_new(OmmBhvDataYoshiTongue, 1);
                    data->yoshiTongue->type = (s32) *(raw++);
                } break;

                case CMD_DESTROY: {
                    OmmBhvDataDestroy *destroy = mem_new(OmmBhvDataDestroy, 1);
                    destroy->type = (s32) *(raw++);
                    destroy->cond = (s32) *(raw++);
                    destroy->args[0] = *(raw++);
                    destroy->args[1] = *(raw++);
                    destroy->args[2] = *(raw++);
                    destroy->args[3] = *(raw++);
                    destroy->args[4] = *(raw++);
                    destroy->args[5] = *(raw++);
                    destroy->next = data->destroy;
                    data->destroy = destroy;
                } break;
            }
        }
    }
}

//
// Public
//

const OmmArray *omm_get_behaviors_list() {
    omm_behavior_data_init();
    return &sBehaviorsList;
}

const OmmBhvData *omm_behavior_data_get(const BehaviorScript *bhv) {
    omm_behavior_data_init();
    s32 i = omm_hmap_find(sOmmBehaviorData, (uintptr_t) bhv);
    if (i != -1) return omm_hmap_get(sOmmBehaviorData, const OmmBhvData *, i);
    return NULL;
}

const OmmBhvDataBoss *omm_behavior_data_get_boss(const BehaviorScript *bhv) {
    const OmmBhvData *data = omm_behavior_data_get(bhv);
    if (data) return data->boss;
    return NULL;
}

const OmmBhvDataPerryAttack *omm_behavior_data_get_perry_attack(const BehaviorScript *bhv) {
    const OmmBhvData *data = omm_behavior_data_get(bhv);
    if (data) return data->perryAttack;
    return NULL;
}

const OmmBhvDataHoldable *omm_behavior_data_get_holdable(const BehaviorScript *bhv) {
    const OmmBhvData *data = omm_behavior_data_get(bhv);
    if (data) return data->holdable;
    return NULL;
}

const OmmBhvDataHitbox *omm_behavior_data_get_hitbox(const BehaviorScript *bhv) {
    const OmmBhvData *data = omm_behavior_data_get(bhv);
    if (data) return data->hitbox;
    return NULL;
}

const OmmBhvDataCapture *omm_behavior_data_get_capture(const BehaviorScript *bhv) {
    const OmmBhvData *data = omm_behavior_data_get(bhv);
    if (data) return data->capture;
    return NULL;
}

const OmmBhvDataYoshiTongue *omm_behavior_data_get_yoshi_tongue(const BehaviorScript *bhv) {
    const OmmBhvData *data = omm_behavior_data_get(bhv);
    if (data) return data->yoshiTongue;
    return NULL;
}

const OmmBhvDataDestroy *omm_behavior_data_get_destroy(const BehaviorScript *bhv) {
    const OmmBhvData *data = omm_behavior_data_get(bhv);
    if (data) return data->destroy;
    return NULL;
}

u64 omm_behavior_data_get_types(const BehaviorScript *bhv) {
    const OmmBhvData *data = omm_behavior_data_get(bhv);
    if (data) return data->types;
    return 0;
}

u64 omm_behavior_data_get_capture_type(struct Object *o) {
    const OmmBhvDataCapture *capture = omm_behavior_data_get_capture(o->behavior);
    if (capture && capture->get_type) {
        return capture->get_type(o);
    }
    return 0;
}
