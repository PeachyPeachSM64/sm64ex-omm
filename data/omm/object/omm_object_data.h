#ifndef OMM_OBJECT_DATA_H
#define OMM_OBJECT_DATA_H

//
// Flags
//

//      ACTIVE_FLAG_DEACTIVATED                             0         // 0x0000
//      ACTIVE_FLAG_ACTIVE                                  (1 <<  0) // 0x0001
//      ACTIVE_FLAG_FAR_AWAY                                (1 <<  1) // 0x0002
//      ACTIVE_FLAG_UNK2                                    (1 <<  2) // 0x0004
//      ACTIVE_FLAG_IN_DIFFERENT_ROOM                       (1 <<  3) // 0x0008
//      ACTIVE_FLAG_UNIMPORTANT                             (1 <<  4) // 0x0010
//      ACTIVE_FLAG_INITIATED_TIME_STOP                     (1 <<  5) // 0x0020
//      ACTIVE_FLAG_MOVE_THROUGH_GRATE                      (1 <<  6) // 0x0040
//      ACTIVE_FLAG_DITHERED_ALPHA                          (1 <<  7) // 0x0080
//      ACTIVE_FLAG_UNK8                                    (1 <<  8) // 0x0100
//      ACTIVE_FLAG_UNK9                                    (1 <<  9) // 0x0200
//      ACTIVE_FLAG_UNK10                                   (1 << 10) // 0x0400
#define ACTIVE_FLAG_DORMANT                                 (1 << 11) // 0x0800
#define ACTIVE_FLAG_BILLBOARD_ROLL                          (1 << 12) // 0x1000

//      OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE                   (1 <<  0) // 0x00000001
//      OBJ_FLAG_MOVE_XZ_USING_FVEL                         (1 <<  1) // 0x00000002
//      OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL                   (1 <<  2) // 0x00000004
//      OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW                   (1 <<  3) // 0x00000008
//      OBJ_FLAG_SET_FACE_ANGLE_TO_MOVE_ANGLE               (1 <<  4) // 0x00000010
//      OBJ_FLAG_0020                                       (1 <<  5) // 0x00000020
//      OBJ_FLAG_COMPUTE_DIST_TO_MARIO                      (1 <<  6) // 0x00000040
//      OBJ_FLAG_ACTIVE_FROM_AFAR                           (1 <<  7) // 0x00000080
//      OBJ_FLAG_0100                                       (1 <<  8) // 0x00000100
//      OBJ_FLAG_TRANSFORM_RELATIVE_TO_PARENT               (1 <<  9) // 0x00000200
//      OBJ_FLAG_HOLDABLE                                   (1 << 10) // 0x00000400
//      OBJ_FLAG_SET_THROW_MATRIX_FROM_TRANSFORM            (1 << 11) // 0x00000800
//      OBJ_FLAG_1000                                       (1 << 12) // 0x00001000
//      OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO                     (1 << 13) // 0x00002000
//      OBJ_FLAG_PERSISTENT_RESPAWN                         (1 << 14) // 0x00004000
//      OBJ_FLAG_8000                                       (1 << 15) // 0x00008000
#define OBJ_FLAG_GFX_INITED                                 (1 << 16) // 0x00010000
#define OBJ_FLAG_INSTANT_WARP                               (1 << 17) // 0x00020000
#define OBJ_FLAG_DONT_RENDER_ON_INIT                        (1 << 18) // 0x00040000
#define OBJ_FLAG_UPDATE_AREA_INDEX                          (1 << 19) // 0x00080000
#define OBJ_FLAG_SHADOW_COPY_OBJ_POS                        (1 << 20) // 0x00100000
#define OBJ_FLAG_NO_SHADOW                                  (1 << 21) // 0x00200000
#define OBJ_FLAG_MONEYBAG_COIN_INTERACTED                   (1 << 22) // 0x00400000
#define OBJ_FLAG_YOSHI_DESTROY                              (1 << 23) // 0x00800000
#define OBJ_FLAG_SPARKLY_NOT_ENEMY                          (1 << 24) // 0x01000000
#define OBJ_FLAG_DESTROYED                                  (1 << 25) // 0x02000000
#define OBJ_FLAG_CAPTURE_AFTER_WARP                         (1 << 26) // 0x04000000
#define OBJ_FLAG_INVISIBLE_MODE                             (1 << 27) // 0x04000000
//      OBJ_FLAG_30                                         (1 << 30) // 0x40000000

//      INT_STATUS_MARIO_STUNNED                            (1 <<  0) // 0x00000001
//      INT_STATUS_MARIO_KNOCKBACK_DMG                      (1 <<  1) // 0x00000002
//      INT_STATUS_MARIO_UNK2                               (1 <<  2) // 0x00000004
//      INT_STATUS_MARIO_DROP_OBJECT                        (1 <<  3) // 0x00000008
//      INT_STATUS_MARIO_SHOCKWAVE                          (1 <<  4) // 0x00000010
//      INT_STATUS_MARIO_UNK5                               (1 <<  5) // 0x00000020
//      INT_STATUS_MARIO_UNK6                               (1 <<  6) // 0x00000040
//      INT_STATUS_MARIO_UNK7                               (1 <<  7) // 0x00000080
//      INT_STATUS_GRABBED_MARIO                            (1 << 11) // 0x00000800
//      INT_STATUS_ATTACKED_MARIO                           (1 << 13) // 0x00002000
//      INT_STATUS_WAS_ATTACKED                             (1 << 14) // 0x00004000
//      INT_STATUS_INTERACTED                               (1 << 15) // 0x00008000
#define INT_STATUS_ENTERING_STAR_DOOR                       (1 << 16) // 0x00010000
#define INT_STATUS_ENTERING_DOOR                            (1 << 17) // 0x00020000
#define INT_STATUS_EXITING_WARP_DOOR                        (1 << 18) // 0x00040000
#define INT_STATUS_EXITING_WARP_DOOR_2                      (1 << 19) // 0x00080000
//      INT_STATUS_TRAP_TURN                                (1 << 20) // 0x00100000
//      INT_STATUS_HIT_MINE                                 (1 << 21) // 0x00200000
//      INT_STATUS_STOP_RIDING                              (1 << 22) // 0x00400000
//      INT_STATUS_TOUCHED_BOB_OMB                          (1 << 23) // 0x00800000

//
// Custom object fields
//

#define gCurrGraphNodeObject                                ((struct Object *) gCurGraphNodeObject)
#define oCameraToObject                                     header.gfx.cameraToObject
#define oThrowMatrix                                        header.gfx.throwMatrix
#define oGraphNode                                          header.gfx.sharedChild
#define oNodeFlags                                          header.gfx.node.flags
#define oAreaIndex                                          header.gfx.mAreaIndex
#define oActiveAreaIndex                                    header.gfx.mActiveAreaIndex
#define oGfxPos                                             header.gfx.pos
#define oGfxAngle                                           header.gfx.angle
#define oGfxScale                                           header.gfx.scale
#define oScaleX                                             header.gfx.scale[0]
#define oScaleY                                             header.gfx.scale[1]
#define oScaleZ                                             header.gfx.scale[2]
#define oAnimInfo                                           header.gfx.mAnimInfo
#define oAnimID                                             header.gfx.mAnimInfo.animID
#define oAnimFrame                                          header.gfx.mAnimInfo.animFrame
#define oCurrAnim                                           header.gfx.mAnimInfo.curAnim
#define oCurrAnimRef                                        header.gfx.mAnimInfo.curAnimRef
#define oDistToFloor                                        oPosY - o->oFloorHeight
#define oPrevPosX                                           OBJECT_FIELD_F32(0x2C) // oParentRelativePosX
#define oPrevPosY                                           OBJECT_FIELD_F32(0x2D) // oParentRelativePosY
#define oPrevPosZ                                           OBJECT_FIELD_F32(0x2E) // oParentRelativePosZ
#define oWall                                               OBJECT_FIELD_SURFACE(0x03)
#define oCeil                                               OBJECT_FIELD_SURFACE(0x04)
#define oCaptureData                                        OBJECT_FIELD_CVPTR(0x30)
#define oGeoData                                            OBJECT_FIELD_VPTR(0x47)
#define oSurfaces                                           OBJECT_FIELD_VPTR(0x48)
#define oTransparency                                       OBJECT_FIELD_S16(0x0D, 0)
#define oInteractId                                         OBJECT_FIELD_S16(0x0D, 1)
#define oSafeStep                                           OBJECT_FIELD_S32(0x0E)
// FREE SLOT                                                unused1
// FREE SLOT                                                unused2

// Cappy
#define oCappyLifeTimer                                     OBJECT_FIELD_S32(0x1B)
#define oCappyBehavior                                      OBJECT_FIELD_S16(0x1C, 0)
#define oCappyFlags                                         OBJECT_FIELD_S16(0x1C, 1)
#define oCappyYaw                                           OBJECT_FIELD_S16(0x1D, 0)
#define oCappyThrowStrength                                 OBJECT_FIELD_S16(0x1D, 1)

// Perry
#define oPerryType                                          OBJECT_FIELD_S32(0x1A) // oAnimState
#define oPerryFlags                                         OBJECT_FIELD_S32(0x1B)
#define oPerryRightHandRot(k)                               OBJECT_FIELD_S16(0x1C + ((k) / 2), ((k) % 2))
#define oPerryHitboxPos(k)                                  OBJECT_FIELD_S16(0x1C + (((k) + 3) / 2), (((k) + 3) % 2))
#define oPerryHitboxRadius                                  OBJECT_FIELD_S16(0x1F, 0)
#define oPerryHitboxHeight                                  OBJECT_FIELD_S16(0x1F, 1)
#define oPerryShockwaveBlast                                OBJECT_FIELD_S16(0x1B, 0)
#define oPerryShockwaveDelay                                OBJECT_FIELD_S16(0x1B, 1)
#define oPerryShockwaveAngleYaw                             OBJECT_FIELD_S32(0x1C)
#define oPerryShockwaveAngleDir                             OBJECT_FIELD_S32(0x1D)
#define oPerryShockwaveBaseScale                            OBJECT_FIELD_F32(0x1E)
#define oPerryChargeSwordTimer                              OBJECT_FIELD_S32(0x1C)
#define oPerryChargeHandTimer                               OBJECT_FIELD_S32(0x1D)

// Bowser
#define oBowserMine                                         OBJECT_FIELD_VPTR(0x1B)
#define oBowserHit                                          OBJECT_FIELD_S32(0x1C)
#define oBowserFireDuration                                 OBJECT_FIELD_S32(0x1B)
#define oIsBowserClone                                      OBJECT_FIELD_S32(0x1D)
#define oBowserShockwaveFireRadius                          OBJECT_FIELD_F32(0x1B)
#define oBowserShockwaveFireWidth                           OBJECT_FIELD_F32(0x1C)
#define oBowserShockwaveFireHeight                          OBJECT_FIELD_F32(0x1D)
#define oBowserShockwaveFireSpeed                           OBJECT_FIELD_F32(0x1E)
#define oBowserShockwaveFireDuration                        OBJECT_FIELD_S32(0x1F)
#define oBowserShockwaveFireTextureWave                     OBJECT_FIELD_CVPTR(0x20)
#define oBowserShockwaveFireTextureFire                     OBJECT_FIELD_CVPTR(0x21)

// Flaming Bob-omb
#define oFlamingBobombAura                                  OBJECT_FIELD_OBJ(0x00)
//      oBobombBlinkTimer                                   OBJECT_FIELD_S32(0x1B)
#define oFlamingBobombIndex                                 OBJECT_FIELD_S32(0x1C)
#define oFlamingBobombCount                                 OBJECT_FIELD_S32(0x1D)
#define oFlamingBobombMaxRadius                             OBJECT_FIELD_F32(0x1E)
#define oFlamingBobombMaxHeight                             OBJECT_FIELD_F32(0x1F)

// Goomba Stack
#define oGoombaStackParent                                  OBJECT_FIELD_OBJ(0x00)

// Free Chain Chomp
#define oChainChompFreeState                                OBJECT_FIELD_S32(0x1F)
#define oChainChompFreeTimer                                OBJECT_FIELD_S32(0x20)
#define oChainChompFreeAngle                                OBJECT_FIELD_S32(0x21)

// Monty Mole
#define oMontyMoleTargetHole                                OBJECT_FIELD_OBJ(0x1D)

// Mips
#define oMipsCurrentWaypoint                                OBJECT_FIELD_S32(0x1D)
#define oMipsTargetWaypoint                                 OBJECT_FIELD_S32(0x1E)
#define oMipsGrabbedCounter                                 OBJECT_FIELD_S32(0x1F)
#define oMipsEscapePath                                     OBJECT_FIELD_S32(0x20)
#define oMipsEscapeIndex                                    OBJECT_FIELD_S32(0x21)

// Hidden Star Trigger
#define oHiddenStarTriggerCollected                         OBJECT_FIELD_S32(0x1B)

// Celebration Star
#define oCelebStarRadius                                    OBJECT_FIELD_F32(0x1B)
#define oCelebStarHeight                                    OBJECT_FIELD_F32(0x1C)
#define oCelebStarBehavior                                  OBJECT_FIELD_CVPTR(0x1E)

// Sparkly Star
//      oStarSpawnDisFromHome                               OBJECT_FIELD_F32(0x1B)
#define oSparklyStarMode                                    OBJECT_FIELD_S32(0x1C)
//      oStarSpawnUnkFC                                     OBJECT_FIELD_F32(0x1D)
#define oSparklyStarPosX                                    OBJECT_FIELD_F32(0x1E)
#define oSparklyStarPosY                                    OBJECT_FIELD_F32(0x1F)
#define oSparklyStarPosZ                                    OBJECT_FIELD_F32(0x20)

// Sparkly Star Box
#define oSparklyStarBoxMaxHeight                            OBJECT_FIELD_F32(0x1D)

// Star Ring
#define oStarRingDot0                                       OBJECT_FIELD_F32(0x1B)
#define oStarRingDot1                                       OBJECT_FIELD_F32(0x1C)

// Rising Lava
#define oRisingLavaMinY                                     OBJECT_FIELD_F32(0x1B)
#define oRisingLavaMaxY                                     OBJECT_FIELD_F32(0x1C)
#define oRisingLavaVelY                                     OBJECT_FIELD_F32(0x1D)
#define oRisingLavaRadius                                   OBJECT_FIELD_F32(0x1E)
#define oRisingLavaRotVel                                   OBJECT_FIELD_S32(0x1F)
#define oRisingLavaShake                                    OBJECT_FIELD_S32(0x20)

// Wall Warp
#define oWallWarpKind                                       OBJECT_FIELD_S32(0x1B)
#define oWallWarpDestLevel                                  OBJECT_FIELD_S16(0x1C, 0)
#define oWallWarpDestArea                                   OBJECT_FIELD_S16(0x1C, 1)
#define oWallWarpDestNode                                   OBJECT_FIELD_S16(0x1D, 0)
#define oWallWarpDuration                                   OBJECT_FIELD_S16(0x1D, 1)
#define oWallWarpHalfWidth                                  OBJECT_FIELD_F32(0x1E)
#define oWallWarpHalfHeight                                 OBJECT_FIELD_F32(0x1F)
#define oWallWarpTransitionType                             OBJECT_FIELD_S16(0x20, 0)
#define oWallWarpTransitionDuration                         OBJECT_FIELD_S16(0x20, 1)
#define oWallWarpTransitionColor                            OBJECT_FIELD_U32(0x21)
#define oWallWarpSparkleGeo                                 OBJECT_FIELD_CVPTR(0x22)

// Shockwave
#define oShockwaveScaleMin                                  OBJECT_FIELD_F32(0x1B)
#define oShockwaveScaleMax                                  OBJECT_FIELD_F32(0x1C)
#define oShockwaveAlphaMin                                  OBJECT_FIELD_S16(0x1D, 0)
#define oShockwaveAlphaMax                                  OBJECT_FIELD_S16(0x1D, 1)
#define oShockwaveDuration                                  OBJECT_FIELD_S32(0x1E)

// Projectiles
#define oHeldProjectile                                     OBJECT_FIELD_OBJ(0x00)
#define oObjectPower                                        OBJECT_FIELD_F32(0x17)
#define oSnufitBallStrong                                   OBJECT_FIELD_S32(0x17)
#define oMoneybagParentObj                                  OBJECT_FIELD_OBJ(0x1C)

// Yoshi Egg
#define oYoshiEggTarget                                     OBJECT_FIELD_OBJ(0x1B)
#define oYoshiEggLastInteracted                             OBJECT_FIELD_OBJ(0x1C)
#define oYoshiEggPosY(x)                                    OBJECT_FIELD_F32(0x1D + (x))

//
// Object function flags
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
#define OBJ_TYPE_UNIMPORTANT                                (OBJ_LIST_UNIMPORTANT << 16)    // No collision check, always update, even during time stop

#define OBJ_FLOOR_TYPE_NONE                                 (0)
#define OBJ_FLOOR_TYPE_GROUND                               (1)
#define OBJ_FLOOR_TYPE_WATER                                (2)
#define OBJ_FLOOR_TYPE_LAVA                                 (3)

#define OBJ_OVERLAP_FLAG_HITBOX                             (1 << 0)
#define OBJ_OVERLAP_FLAG_HURTBOX_HITBOX_IF_ZERO             (1 << 1)
#define OBJ_OVERLAP_FLAG_HITBOX_HURTBOX_MAX                 (1 << 2)
#define OBJ_OVERLAP_FLAG_IGNORE_INTANGIBLE                  (1 << 3)

#define OBJ_STEP_UPDATE_HOME                                (1 << 0)
#define OBJ_STEP_UPDATE_PREV_POS                            (1 << 1)
#define OBJ_STEP_MOVE_THROUGH_WALLS                         (1 << 2)
#define OBJ_STEP_STICKY_FEET                                (1 << 3)
#define OBJ_STEP_CHECK_ON_GROUND                            (1 << 4)
#define OBJ_STEP_KEEP_UNDERWATER                            (1 << 5)
#define OBJ_STEP_STOP_IF_OUT_OF_BOUNDS                      (1 << 6)

#define OBJ_PARTICLE_NONE                                   (0 << 0)
#define OBJ_PARTICLE_MIST                                   (1 << 0)
#define OBJ_PARTICLE_SMOKE                                  (1 << 1)
#define OBJ_PARTICLE_WATER_TRAIL                            (1 << 2)
#define OBJ_PARTICLE_WATER_DROPLET                          (1 << 3)
#define OBJ_PARTICLE_FIRE                                   (1 << 4)
#define OBJ_PARTICLE_FLAME                                  (1 << 5)
#define OBJ_PARTICLE_BUBBLE                                 (1 << 6)

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

#define OBJ_INT_RESULT_NONE                                 (0)
#define OBJ_INT_RESULT_CONTACT                              (1)
#define OBJ_INT_RESULT_INTERACT                             (2)
#define OBJ_INT_RESULT_STOP                                 (3)

#define OBJ_INT_ATTACK_WEAK                                 (1 << 0) // Common attacks
#define OBJ_INT_ATTACK_STRONG                               (1 << 1) // Strong attacks, such as charged snowballs or beams
#define OBJ_INT_ATTACK_ONE_HIT                              (1 << 2) // Powerful attacks, defeat bosses in one hit
#define OBJ_INT_ATTACK_DESTRUCTIBLE                         (1 << 3) // Destructive attacks, such as explosions
#define OBJ_INT_ATTACK_BREAKABLE                            (1 << 4) // Fragile/Breakable objects, such as cork boxes
#define OBJ_INT_ATTACK_FLAMES                               (1 << 5) // Flame-type objects
#define OBJ_INT_COLLECT_COINS                               (1 << 6) // Coins
#define OBJ_INT_COLLECT_TRIGGERS                            (1 << 7) // Water rings, secrets, 1up-mushrooms and invisible triggers
#define OBJ_INT_COLLECT_STARS                               (1 << 8) // Stars and keys
#define OBJ_INT_COLLECT_CAPS                                (1 << 9) // Caps
#define OBJ_INT_GRAB_OBJECTS                                (1 << 10) // Perform a grab instead of attacking
#define OBJ_INT_HEAL_MARIO                                  (1 << 11) // Heal Mario after attacking/destroying an enemy
#define OBJ_INT_NOT_INTERACTED                              (1 << 12) // omm_obj_process_interactions() always returns NULL
#define OBJ_INT_ATTRACT_COINS_WEAK                          (1 << 13) // Attract nearby coins (weak pull)
#define OBJ_INT_ATTRACT_COINS_STRONG                        (1 << 14) // Attract nearby coins (strong pull)
#define OBJ_INT_ATTRACT_COINS_RANGE(x)                      ((((u32) (x / 100)) & 0xF) << 16) // Attraction range (4 bits, values from 0 to 1500)
#define OBJ_INT_ATTRACT_COINS_RANGE_GET(intFlags)           (((u32) ((intFlags >> 16) & 0xF)) * 100)
#define OBJ_INT_PERRY_ATTACK                                (1 << 24)
#define OBJ_INT_PERRY_SWORD                                 (1 << 25)
#define OBJ_INT_PERRY_TRAIL                                 (1 << 26)
#define OBJ_INT_PERRY_CHARGE                                (1 << 27)

#define OBJ_INT_PRESET_ATTACK_SURFACE                       (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE)
#define OBJ_INT_PRESET_CAPPY                                (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_GRAB_OBJECTS | (OBJ_INT_COLLECT_STARS * OMM_CHEAT_CAPPY_CAN_COLLECT_STARS))
#define OBJ_INT_PRESET_CAPPY_STRONG                         (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | (OBJ_INT_COLLECT_STARS * OMM_CHEAT_CAPPY_CAN_COLLECT_STARS))
#define OBJ_INT_PRESET_EXPLOSION                            (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_DESTRUCTIBLE)
#define OBJ_INT_PRESET_WHOMP_SHOCKWAVE                      (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_SPINDRIFT_SHOCKWAVE                  (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_COLLECT_STARS)
#define OBJ_INT_PRESET_SNOWBALL_SMALL                       (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_SNOWBALL_LARGE                       (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_MR_I_BEAM_SMALL                      (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_MR_I_BEAM_LARGE                      (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_SNUFIT_BALL_SMALL                    (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_SNUFIT_BALL_LARGE                    (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_MONTY_MOLE_ROCK_SMALL                (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_MONTY_MOLE_ROCK_LARGE                (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_FIREBALL                             (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_BLARGG_FIRE                          (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_POKEY_BALL                           (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_AMP_DISCHARGE_SMALL                  (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_WEAK)
#define OBJ_INT_PRESET_AMP_DISCHARGE_LARGE                  (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_STRONG)
#define OBJ_INT_PRESET_MONEYBAG_COIN_SMALL                  (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_FLAMES | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_STRONG | OBJ_INT_ATTRACT_COINS_RANGE(400))
#define OBJ_INT_PRESET_MONEYBAG_COIN_LARGE                  (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_ONE_HIT | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_FLAMES | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_STRONG | OBJ_INT_ATTRACT_COINS_RANGE(800))
#define OBJ_INT_PRESET_SPINY_BALL_SMALL                     (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_SPINY_BALL_LARGE                     (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_YOSHI_EGG_SMALL                      (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_YOSHI_EGG_LARGE                      (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_YOSHI_FIREBALL                       (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)

#define OBJ_INT_PRESET_ATTACK_WEAK                          (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_ATTACK_STRONG                        (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_ATTACK_DESTRUCTIBLE                  (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS)
#define OBJ_INT_PRESET_ATTACK_AMP                           (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_STRONG | OBJ_INT_ATTRACT_COINS_RANGE(400))
#define OBJ_INT_PRESET_ATTACK_SPARKLY_STAR_BOX              (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_COLLECT_TRIGGERS)

#define OBJ_INT_PRESET_PEACH_VIBE_JOY_TORNADO               (OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_WEAK | OBJ_INT_ATTRACT_COINS_RANGE(900) | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_JOY_GUST                  (OBJ_INT_ATTACK_WEAK | OBJ_INT_COLLECT_COINS | OBJ_INT_COLLECT_TRIGGERS | OBJ_INT_ATTRACT_COINS_STRONG | OBJ_INT_ATTRACT_COINS_RANGE(600) | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_GLOOM_AURA                (OBJ_INT_ATTACK_FLAMES | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_RAGE_AURA                 (OBJ_INT_ATTACK_WEAK | (OBJ_INT_ATTACK_ONE_HIT * omm_mario_is_ground_pounding(gMarioState)) | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_RAGE_GROUND_POUND_LAND    (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_DESTRUCTIBLE | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_ATTACK_ONE_HIT | OBJ_INT_PERRY_ATTACK)
#define OBJ_INT_PRESET_PEACH_VIBE_RAGE_SHOCKWAVE            (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG | OBJ_INT_ATTACK_BREAKABLE | OBJ_INT_PERRY_ATTACK)

#define OBJ_GOOMBA_STACK_MAX                                (10)
#define OBJ_POKEY_BALLS_MAX                                 (5)
#define OBJ_YOSHI_EGGS_MAX                                  (5)

#define obj_get_interaction_flags_from_power(power, preset) \
((f32) (power) >= 0.65f ? CAT(preset, _LARGE) : CAT(preset, _SMALL))

//
// Step results
//

#define OBJECT_STEP_NONE                                    (0)
#define OBJECT_STEP_HIT_FLOOR                               (1)
#define OBJECT_STEP_HIT_CEILING                             (2)
#define OBJECT_STEP_HIT_WALL                                (3)
#define OBJECT_STEP_OUT_OF_BOUNDS                           (4)

#define OBJECT_STEP_CHECK_OUT_OF_BOUNDS                     (1 << 0)
#define OBJECT_STEP_CHECK_HIT_FLOOR                         (1 << 1)
#define OBJECT_STEP_CHECK_HIT_FLOOR_WITH_NEGATIVE_VEL       (1 << 2)
#define OBJECT_STEP_CHECK_HIT_SLANTED_FLOOR                 (1 << 3)
#define OBJECT_STEP_CHECK_HIT_CEILING                       (1 << 4)
#define OBJECT_STEP_CHECK_HIT_CEILING_WITH_POSITIVE_VEL     (1 << 5)
#define OBJECT_STEP_CHECK_HIT_SLANTED_CEILING               (1 << 6)
#define OBJECT_STEP_CHECK_HIT_WALL                          (1 << 7)

//
// Gfx/Geo macros
//

#define OMM_VTX_RECT_XY(xmin, xmax, ymin, ymax, row, col, nrows, ncols) \
    { { { xmin, ymin, 0 }, 0, { (512 * 32 * (col + 0)) / ncols, (512 * 32 * (row + 1)) / nrows }, { 0xFF, 0xFF, 0xFF, 0xFF } } }, \
    { { { xmax, ymin, 0 }, 0, { (512 * 32 * (col + 1)) / ncols, (512 * 32 * (row + 1)) / nrows }, { 0xFF, 0xFF, 0xFF, 0xFF } } }, \
    { { { xmax, ymax, 0 }, 0, { (512 * 32 * (col + 1)) / ncols, (512 * 32 * (row + 0)) / nrows }, { 0xFF, 0xFF, 0xFF, 0xFF } } }, \
    { { { xmin, ymax, 0 }, 0, { (512 * 32 * (col + 0)) / ncols, (512 * 32 * (row + 0)) / nrows }, { 0xFF, 0xFF, 0xFF, 0xFF } } }

#define OMM_VTX_RECT(size, row, col, nrows, ncols) \
    OMM_VTX_RECT_XY(-size, +size, -size, +size, row, col, nrows, ncols)

#define OMM_GFX_QUAD(vertices, frame) \
    gsSPVertex(vertices + 4 * frame, 4, 0), \
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), \
    gsSPEndDisplayList()

#define OMM_GFX_DL_COMB(name, texture, triangles, frame, ...) \
static const Gfx name##_##frame##_gfx[] = { \
    gsSPClearGeometryMode(G_LIGHTING), \
    gsDPSetCombineLERP(__VA_ARGS__), \
    gsDPLoadTextureBlock(texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, 512, 512, 0, 0, 0, 0, 0, 0, 0), \
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON), \
    gsSPDisplayList(triangles + 3 * frame), \
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF), \
    gsSPSetGeometryMode(G_LIGHTING), \
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE), \
    gsSPEndDisplayList(), \
}

#define OMM_GFX_DL(name, texture, triangles, frame) \
    OMM_GFX_DL_COMB(name, texture, triangles, frame, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0)

#define OMM_GEO_BRANCH(geo) \
    GEO_NODE_START(), \
    GEO_OPEN_NODE(), \
        GEO_BRANCH(1, geo), \
    GEO_CLOSE_NODE(), \
    GEO_END()

#define OMM_GEO_BILLBOARD_AND_BRANCH(geo) \
    GEO_NODE_START(), \
    GEO_OPEN_NODE(), \
        GEO_BILLBOARD(), \
        GEO_OPEN_NODE(), \
            GEO_BRANCH(1, geo), \
        GEO_CLOSE_NODE(), \
    GEO_CLOSE_NODE(), \
    GEO_END()

//
// Geo layouts
//

typedef uintptr_t GeoLayout;
#define OMM_GEO_(geo, ...) extern const GeoLayout geo[];
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
