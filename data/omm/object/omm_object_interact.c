#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define BHV_TYPE_COIN               (1 << 0) // Coin
#define BHV_TYPE_WATER_RING         (1 << 1) // Water ring
#define BHV_TYPE_MUSHROOM_1UP       (1 << 2) // Mushroom 1up
#define BHV_TYPE_STAR_OR_KEY        (1 << 3) // Interactible Star or key
#define BHV_TYPE_STAR_MODEL         (1 << 4) // Star or key
#define BHV_TYPE_CAP                (1 << 5) // Cap object
#define BHV_TYPE_SECRET             (1 << 6) // Secret
#define BHV_TYPE_WEAK               (1 << 7) // Vulnerable to weak attacks
#define BHV_TYPE_STRONG             (1 << 8) // Vulnerable to strong attacks (snowballs, beams)
#define BHV_TYPE_DESTRUCTIBLE       (1 << 9) // Vulnerable to destructible attacks (explosions)
#define BHV_TYPE_BREAKABLE          (1 << 10) // Fragile/Breakable
#define BHV_TYPE_BOSS               (1 << 11) // Boss flag, affected by OBJ_INT_ATTACK_ONE_HIT
#define BHV_TYPE_INVULNERABLE       (1 << 12) // Invulnerable enemies
#define BHV_TYPE_GRABBABLE          (1 << 13) // Grabbable
#define BHV_TYPE_KNOCKABLE_0        (1 << 14) // Knockback 0
#define BHV_TYPE_KNOCKABLE_1        (1 << 15) // Knockback 1
#define BHV_TYPE_GOOMBA             (1 << 16) // Goomba-like behavior
#define BHV_TYPE_BULLY              (1 << 17) // Bully-type
#define BHV_TYPE_FLAME              (1 << 18) // Flame-type objects
#define BHV_TYPE_NO_CAPPY           (1 << 19) // Intangible to Cappy
#define BHV_TYPE_KICKABLE_BOARD     (1 << 20) // Kickable board
#define BHV_TYPE_WF_STAR_WALL       (1 << 21) // Whomp Fortress Star inside wall
#define BHV_TYPE_EXCLAMATION_BOX    (1 << 22) // Yellow exclamation box
#define BHV_TYPE_UNAGIS_TAIL        (1 << 23) // Unagi's tail
#define BHV_TYPE_COLLECTIBLE        (BHV_TYPE_COIN | BHV_TYPE_WATER_RING | BHV_TYPE_MUSHROOM_1UP | BHV_TYPE_STAR_OR_KEY | BHV_TYPE_CAP | BHV_TYPE_SECRET)
#define BHV_TYPE_KNOCKABLE          (BHV_TYPE_KNOCKABLE_0 | BHV_TYPE_KNOCKABLE_1)

//
// Data
//

typedef struct {
    const BehaviorScript *behavior;
    u32 types;
} OmmBehavior;

static const OmmBehavior sOmmBehaviorList[] = {
    { bhv1Up, BHV_TYPE_MUSHROOM_1UP },
    { bhv1upJumpOnApproach, BHV_TYPE_MUSHROOM_1UP },
    { bhv1upRunningAway, BHV_TYPE_MUSHROOM_1UP },
    { bhv1upSliding, BHV_TYPE_MUSHROOM_1UP },
    { bhv1upWalking, BHV_TYPE_MUSHROOM_1UP },
    { bhvActSelector, BHV_TYPE_STAR_MODEL },
    { bhvActSelectorStarType, 0 },
    { bhvActivatedBackAndForthPlatform, 0 },
    { bhvAirborneDeathWarp, 0 },
    { bhvAirborneStarCollectWarp, 0 },
    { bhvAirborneWarp, 0 },
    { bhvAmbientSounds, 0 },
    { bhvAnimatedTexture, 0 },
    { bhvAnimatesOnFloorSwitchPress, 0 },
    { bhvAnotherElavator, 0 },
    { bhvAnotherTiltingPlatform, 0 },
    { bhvArrowLift, 0 },
    { bhvBalconyBigBoo, BHV_TYPE_WEAK | BHV_TYPE_BOSS | BHV_TYPE_KNOCKABLE_1 },
    { bhvBbhTiltingTrapPlatform, 0 },
    { bhvBbhTumblingBridge, 0 },
    { bhvBeginningLakitu, 0 },
    { bhvBeginningPeach, 0 },
    { bhvBetaBooKey, 0 },
    { bhvBetaBowserAnchor, 0 },
    { bhvBetaChestBottom, 0 },
    { bhvBetaChestLid, 0 },
    { bhvBetaFishSplashSpawner, 0 },
    { bhvBetaHoldableObject, 0 },
    { bhvBetaMovingFlames, BHV_TYPE_FLAME },
    { bhvBetaMovingFlamesSpawn, BHV_TYPE_FLAME },
    { bhvBetaTrampolineSpring, 0 },
    { bhvBetaTrampolineTop, 0 },
    { bhvBigBoulder, BHV_TYPE_DESTRUCTIBLE },
    { bhvBigBoulderGenerator, 0 },
    { bhvBigBully, BHV_TYPE_BOSS | BHV_TYPE_BULLY },
    { bhvBigBullyWithMinions, BHV_TYPE_BOSS | BHV_TYPE_BULLY },
    { bhvBigChillBully, BHV_TYPE_BOSS | BHV_TYPE_BULLY },
    { bhvBigSnowmanWhole, BHV_TYPE_INVULNERABLE },
    { bhvBird, 0 },
    { bhvBirdsSoundLoop, 0 },
    { bhvBitfsSinkingCagePlatform, 0 },
    { bhvBitfsSinkingPlatforms, 0 },
    { bhvBitfsTiltingInvertedPyramid, 0 },
    { bhvBlackSmokeBowser, 0 },
    { bhvBlackSmokeMario, 0 },
    { bhvBlackSmokeUpward, 0 },
    { bhvBlueBowserFlame, BHV_TYPE_FLAME },
    { bhvBlueCoinJumping, BHV_TYPE_COIN },
    { bhvBlueCoinSliding, BHV_TYPE_COIN },
    { bhvBlueCoinSwitch, 0 },
    { bhvBlueFish, 0 },
    { bhvBlueFlamesGroup, BHV_TYPE_FLAME },
    { bhvBobBowlingBallSpawner, 0 },
    { bhvBobomb, BHV_TYPE_WEAK | BHV_TYPE_GRABBABLE | BHV_TYPE_KNOCKABLE_0 },
    { bhvBobombAnchorMario, 0 },
    { bhvBobombBuddy, 0 },
    { bhvBobombBuddyOpensCannon, 0 },
    { bhvBobombBullyDeathSmoke, 0 },
    { bhvBobombExplosionBubble, 0 },
    { bhvBobombExplosionBubble3600, 0 },
    { bhvBobombFuseSmoke, 0 },
    { bhvBoo, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvBooCage, 0 },
    { bhvBooInCastle, 0 },
    { bhvBooWithCage, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvBookSwitch, 0 },
    { bhvBookendSpawn, 0 },
    { bhvBouncingFireball, BHV_TYPE_FLAME },
    { bhvBouncingFireballFlame, BHV_TYPE_FLAME },
    { bhvBowlingBall, BHV_TYPE_DESTRUCTIBLE },
    { bhvBowser, BHV_TYPE_INVULNERABLE },
    { bhvBowserBodyAnchor, BHV_TYPE_BOSS | BHV_TYPE_INVULNERABLE },
    { bhvBowserBomb, 0 },
    { bhvBowserBombExplosion, 0 },
    { bhvBowserBombSmoke, 0 },
    { bhvBowserCourseRedCoinStar, 0 },
    { bhvBowserFlameSpawn, BHV_TYPE_FLAME },
    { bhvBowserKey, BHV_TYPE_STAR_OR_KEY },
    { bhvBowserKeyCourseExit, 0 },
    { bhvBowserKeyUnlockDoor, 0 },
    { bhvBowserShockWave, 0 },
    { bhvBowserSubDoor, 0 },
    { bhvBowserTailAnchor, BHV_TYPE_GRABBABLE | BHV_TYPE_NO_CAPPY },
    { bhvBowsersSub, 0 },
    { bhvBreakBoxTriangle, 0 },
    { bhvBreakableBox, BHV_TYPE_BREAKABLE },
    { bhvBreakableBoxSmall, BHV_TYPE_BREAKABLE | BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_GRABBABLE },
    { bhvBreathParticleSpawner, 0 },
    { bhvBub, 0 },
    { bhvBubba, 0 },
    { bhvBubbleMaybe, 0 },
    { bhvBubbleParticleSpawner, 0 },
    { bhvBubbleSplash, 0 },
    { bhvBulletBill, BHV_TYPE_INVULNERABLE },
    { bhvBulletBillCannon, 0 },
    { bhvButterfly, 0 },
    { bhvCameraLakitu, 0 },
    { bhvCannon, 0 },
    { bhvCannonBarrel, 0 },
    { bhvCannonBarrelBubbles, 0 },
    { bhvCannonBaseUnused, 0 },
    { bhvCannonClosed, 0 },
    { bhvCapSwitch, 0 },
    { bhvCapSwitchBase, 0 },
    { bhvCarrySomething1, 0 },
    { bhvCarrySomething2, 0 },
    { bhvCarrySomething3, 0 },
    { bhvCarrySomething4, 0 },
    { bhvCarrySomething5, 0 },
    { bhvCarrySomething6, 0 },
    { bhvCastleFlagWaving, 0 },
    { bhvCastleFloorTrap, 0 },
    { bhvCcmTouchedStarSpawn, 0 },
    { bhvCelebrationStar, BHV_TYPE_STAR_MODEL },
    { bhvCelebrationStarSparkle, 0 },
    { bhvChainChomp, BHV_TYPE_INVULNERABLE },
    { bhvChainChompChainPart, 0 },
    { bhvChainChompGate, 0 },
    { bhvCheckerboardElevatorGroup, 0 },
    { bhvCheckerboardPlatformSub, 0 },
    { bhvChirpChirp, 0 },
    { bhvChirpChirpUnused, 0 },
    { bhvChuckya, BHV_TYPE_STRONG | BHV_TYPE_GRABBABLE | BHV_TYPE_KNOCKABLE_0 },
    { bhvChuckyaAnchorMario, 0 },
    { bhvCirclingAmp, 0 },
    { bhvClamShell, 0 },
    { bhvClockHourHand, 0 },
    { bhvClockMinuteHand, 0 },
    { bhvCloud, 0 },
    { bhvCloudPart, 0 },
    { bhvCoffin, 0 },
    { bhvCoffinSpawner, 0 },
    { bhvCoinFormation, 0 },
    { bhvCoinFormationSpawn, BHV_TYPE_COIN },
    { bhvCoinInsideBoo, 0 },
    { bhvCoinSparkles, 0 },
    { bhvControllablePlatform, 0 },
    { bhvControllablePlatformSub, 0 },
    { bhvCourtyardBooTriplet, 0 },
    { bhvCutOutObject, 0 },
    { bhvDDDPole, 0 },
    { bhvDddMovingPole, 0 },
    { bhvDddWarp, 0 },
    { bhvDeathWarp, 0 },
    { bhvDecorativePendulum, 0 },
    { bhvDirtParticleSpawner, 0 },
    { bhvDonutPlatform, 0 },
    { bhvDonutPlatformSpawner, 0 },
    { bhvDoor, 0 },
    { bhvDoorWarp, 0 },
    { bhvDorrie, 0 },
    { bhvEndBirds1, 0 },
    { bhvEndBirds2, 0 },
    { bhvEndPeach, 0 },
    { bhvEndToad, 0 },
    { bhvEnemyLakitu, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvExclamationBox, BHV_TYPE_BREAKABLE | BHV_TYPE_EXCLAMATION_BOX },
    { bhvExitPodiumWarp, 0 },
    { bhvExplosion, 0 },
    { bhvEyerokBoss, 0 },
    { bhvEyerokHand, BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_BOSS },
    { bhvFadingWarp, 0 },
    { bhvFallingBowserPlatform, 0 },
    { bhvFallingPillar, 0 },
    { bhvFallingPillarHitbox, 0 },
    { bhvFerrisWheelAxle, 0 },
    { bhvFerrisWheelPlatform, 0 },
    { bhvFireParticleSpawner, 0 },
    { bhvFirePiranhaPlant, BHV_TYPE_WEAK },
    { bhvFireSpitter, BHV_TYPE_FLAME },
    { bhvFish, 0 },
    { bhvFishGroup, 0 },
    { bhvFlame, BHV_TYPE_FLAME },
    { bhvFlameBouncing, BHV_TYPE_FLAME },
    { bhvFlameBowser, BHV_TYPE_FLAME },
    { bhvFlameFloatingLanding, BHV_TYPE_FLAME },
    { bhvFlameLargeBurningOut, BHV_TYPE_FLAME },
    { bhvFlameMovingForwardGrowing, BHV_TYPE_FLAME },
    { bhvFlamethrower, BHV_TYPE_FLAME },
    { bhvFlamethrowerFlame, BHV_TYPE_FLAME },
    { bhvFloorSwitchAnimatesObject, 0 },
    { bhvFloorSwitchGrills, 0 },
    { bhvFloorSwitchHardcodedModel, 0 },
    { bhvFloorSwitchHiddenObjects, 0 },
    { bhvFloorTrapInCastle, 0 },
    { bhvFlyGuy, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvFlyguyFlame, BHV_TYPE_FLAME },
    { bhvFlyingBookend, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvFlyingWarp, 0 },
    { bhvFreeBowlingBall, BHV_TYPE_DESTRUCTIBLE },
    { bhvGhostHuntBigBoo, BHV_TYPE_WEAK | BHV_TYPE_BOSS | BHV_TYPE_KNOCKABLE_1 },
    { bhvGhostHuntBoo, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvGiantPole, 0 },
    { bhvGoldenCoinSparkles, 0 },
    { bhvGoomba, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_0 | BHV_TYPE_GOOMBA },
    { bhvGoombaTripletSpawner, 0 },
    { bhvGrandStar, BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL },
    { bhvGrindel, BHV_TYPE_DESTRUCTIBLE },
    { bhvHardAirKnockBackWarp, 0 },
    { bhvHauntedBookshelf, 0 },
    { bhvHauntedBookshelfManager, 0 },
    { bhvHauntedChair, BHV_TYPE_DESTRUCTIBLE },
    { bhvHeaveHo, BHV_TYPE_DESTRUCTIBLE },
    { bhvHeaveHoThrowMario, 0 },
    { bhvHidden1up, BHV_TYPE_MUSHROOM_1UP },
    { bhvHidden1upInPole, BHV_TYPE_MUSHROOM_1UP },
    { bhvHidden1upInPoleSpawner, BHV_TYPE_MUSHROOM_1UP },
    { bhvHidden1upInPoleTrigger, BHV_TYPE_MUSHROOM_1UP },
    { bhvHidden1upTrigger, BHV_TYPE_MUSHROOM_1UP },
    { bhvHiddenAt120Stars, 0 },
    { bhvHiddenBlueCoin, BHV_TYPE_COIN },
    { bhvHiddenObject, BHV_TYPE_BREAKABLE },
    { bhvHiddenRedCoinStar, 0 },
    { bhvHiddenStaircaseStep, 0 },
    { bhvHiddenStar, 0 },
    { bhvHiddenStarTrigger, BHV_TYPE_SECRET },
    { bhvHmcElevatorPlatform, 0 },
    { bhvHomingAmp, 0 },
    { bhvHoot, 0 },
    { bhvHorStarParticleSpawner, 0 },
    { bhvHorizontalGrindel, BHV_TYPE_DESTRUCTIBLE },
    { bhvIdleWaterWave, 0 },
    { bhvIgloo, 0 },
    { bhvInSunkenShip, 0 },
    { bhvInSunkenShip2, 0 },
    { bhvInSunkenShip3, 0 },
    { bhvInitializeChangingWaterLevel, 0 },
    { bhvInsideCannon, 0 },
    { bhvInstantActiveWarp, 0 },
    { bhvIntroScene, 0 },
    { bhvInvisibleObjectsUnderBridge, 0 },
    { bhvJetStream, 0 },
    { bhvJetStreamRingSpawner, 0 },
    { bhvJetStreamWaterRing, BHV_TYPE_WATER_RING },
    { bhvJrbFloatingBox, 0 },
    { bhvJrbFloatingPlatform, 0 },
    { bhvJrbSlidingBox, BHV_TYPE_DESTRUCTIBLE },
    { bhvJumpingBox, BHV_TYPE_BREAKABLE | BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_GRABBABLE },
    { bhvKickableBoard, BHV_TYPE_KICKABLE_BOARD },
    { bhvKingBobomb, BHV_TYPE_GRABBABLE | BHV_TYPE_BOSS },
    { bhvKlepto, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvKoopa, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_0 },
    { bhvKoopaFlag, 0 },
    { bhvKoopaRaceEndpoint, 0 },
    { bhvKoopaShell, 0 },
    { bhvKoopaShellFlame, 0 },
    { bhvKoopaShellUnderwater, 0 },
    { bhvLargeBomp, 0 },
    { bhvLaunchDeathWarp, 0 },
    { bhvLaunchStarCollectWarp, 0 },
    { bhvLeafParticleSpawner, 0 },
    { bhvLllBowserPuzzle, 0 },
    { bhvLllBowserPuzzlePiece, 0 },
    { bhvLllDrawbridge, 0 },
    { bhvLllDrawbridgeSpawner, 0 },
    { bhvLllFloatingWoodBridge, 0 },
    { bhvLllHexagonalMesh, 0 },
    { bhvLllMovingOctagonalMeshPlatform, 0 },
    { bhvLllRollingLog, 0 },
    { bhvLllRotatingBlockWithFireBars, 0 },
    { bhvLllRotatingHexFlame, BHV_TYPE_FLAME },
    { bhvLllRotatingHexagonalPlatform, 0 },
    { bhvLllRotatingHexagonalRing, 0 },
    { bhvLllSinkingRectangularPlatform, 0 },
    { bhvLllSinkingRockBlock, 0 },
    { bhvLllSinkingSquarePlatforms, 0 },
    { bhvLllTiltingInvertedPyramid, 0 },
    { bhvLllTumblingBridge, 0 },
    { bhvLllVolcanoFallingTrap, 0 },
    { bhvLllWoodPiece, 0 },
    { bhvMacroUkiki, BHV_TYPE_GRABBABLE },
    { bhvMadPiano, BHV_TYPE_DESTRUCTIBLE },
    { bhvMantaRay, 0 },
    { bhvMantaRayRingManager, 0 },
    { bhvMantaRayWaterRing, BHV_TYPE_WATER_RING },
    { bhvMario, 0 },
    { bhvMenuButton, 0 },
    { bhvMenuButtonManager, 0 },
    { bhvMerryGoRound, 0 },
    { bhvMerryGoRoundBigBoo, BHV_TYPE_WEAK | BHV_TYPE_BOSS | BHV_TYPE_KNOCKABLE_1 },
    { bhvMerryGoRoundBoo, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvMerryGoRoundBooManager, 0 },
    { bhvMeshElevator, 0 },
    { bhvMessagePanel, 0 },
    { bhvMetalCap, BHV_TYPE_CAP },
    { bhvMips, BHV_TYPE_GRABBABLE },
    { bhvMistCircParticleSpawner, 0 },
    { bhvMistParticleSpawner, 0 },
    { bhvMoatGrills, 0 },
    { bhvMoneybag, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_0 },
    { bhvMoneybagHidden, BHV_TYPE_INVULNERABLE },
    { bhvMontyMole, BHV_TYPE_WEAK },
    { bhvMontyMoleHole, 0 },
    { bhvMontyMoleRock, 0 },
    { bhvMovingBlueCoin, BHV_TYPE_COIN },
    { bhvMovingYellowCoin, BHV_TYPE_COIN },
    { bhvMrBlizzard, BHV_TYPE_STRONG },
    { bhvMrBlizzardSnowball, 0 },
    { bhvMrI, BHV_TYPE_STRONG | BHV_TYPE_KNOCKABLE_0 },
    { bhvMrIBlueCoin, BHV_TYPE_COIN },
    { bhvMrIBody, 0 },
    { bhvMrIParticle, 0 },
    { bhvNormalCap, BHV_TYPE_CAP },
    { bhvObjectBubble, 0 },
    { bhvObjectWaterSplash, 0 },
    { bhvObjectWaterWave, 0 },
    { bhvObjectWaveTrail, 0 },
    { bhvOctagonalPlatformRotating, 0 },
    { bhvOneCoin, BHV_TYPE_COIN },
    { bhvOpenableCageDoor, 0 },
    { bhvOpenableGrill, 0 },
    { bhvOrangeNumber, 0 },
    { bhvPaintingDeathWarp, 0 },
    { bhvPaintingStarCollectWarp, 0 },
    { bhvPenguinBaby, BHV_TYPE_GRABBABLE },
    { bhvPenguinRaceFinishLine, 0 },
    { bhvPenguinRaceShortcutCheck, 0 },
    { bhvPillarBase, 0 },
    { bhvPiranhaPlant, BHV_TYPE_WEAK },
    { bhvPiranhaPlantBubble, 0 },
    { bhvPiranhaPlantWakingBubbles, 0 },
    { bhvPitBowlingBall, BHV_TYPE_DESTRUCTIBLE },
    { bhvPlatformOnTrack, 0 },
    { bhvPlaysMusicTrackWhenTouched, 0 },
    { bhvPlungeBubble, 0 },
    { bhvPokey, BHV_TYPE_WEAK },
    { bhvPokeyBodyPart, BHV_TYPE_WEAK },
    { bhvPoleGrabbing, 0 },
    { bhvPoundTinyStarParticle, 0 },
    { bhvPunchTinyTriangle, 0 },
    { bhvPurpleParticle, 0 },
    { bhvPurpleSwitchHiddenBoxes, 0 },
    { bhvPushableMetalBox, 0 },
    { bhvPyramidElevator, 0 },
    { bhvPyramidElevatorTrajectoryMarkerBall, 0 },
    { bhvPyramidPillarTouchDetector, 0 },
    { bhvPyramidTop, 0 },
    { bhvPyramidTopFragment, 0 },
    { bhvRacingPenguin, 0 },
    { bhvRandomAnimatedTexture, 0 },
    { bhvRecoveryHeart, 0 },
    { bhvRedCoin, BHV_TYPE_COIN },
    { bhvRedCoinStarMarker, 0 },
    { bhvRespawner, 0 },
    { bhvRockSolid, 0 },
    { bhvRotatingCounterClockwise, 0 },
    { bhvRotatingExclamationMark, 0 },
    { bhvRotatingPlatform, 0 },
    { bhvRrCruiserWing, 0 },
    { bhvRrElevatorPlatform, 0 },
    { bhvRrRotatingBridgePlatform, 0 },
    { bhvSLSnowmanWind, 0 },
    { bhvSLWalkingPenguin, 0 },
    { bhvSandSoundLoop, 0 },
    { bhvScuttlebug, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_0 },
    { bhvScuttlebugSpawn, 0 },
    { bhvSeaweed, 0 },
    { bhvSeaweedBundle, 0 },
    { bhvSeesawPlatform, 0 },
    { bhvShallowWaterSplash, 0 },
    { bhvShallowWaterWave, 0 },
    { bhvShipPart3, 0 },
    { bhvSignOnWall, 0 },
    { bhvSingleCoinGetsSpawned, BHV_TYPE_COIN },
    { bhvSkeeter, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_0 },
    { bhvSkeeterWave, 0 },
    { bhvSlidingPlatform2, 0 },
    { bhvSlidingSnowMound, 0 },
    { bhvSmallBomp, 0 },
    { bhvSmallBully, BHV_TYPE_BULLY },
    { bhvSmallChillBully, BHV_TYPE_BULLY },
    { bhvSmallParticle, 0 },
    { bhvSmallParticleBubbles, 0 },
    { bhvSmallParticleSnow, 0 },
    { bhvSmallPenguin, BHV_TYPE_GRABBABLE },
    { bhvSmallPiranhaFlame, BHV_TYPE_FLAME },
    { bhvSmallWaterWave, 0 },
    { bhvSmallWaterWave398, 0 },
    { bhvSmallWhomp, BHV_TYPE_DESTRUCTIBLE },
    { bhvSmoke, 0 },
    { bhvSnowBall, 0 },
    { bhvSnowMoundSpawn, 0 },
    { bhvSnowParticleSpawner, 0 },
    { bhvSnowmansBodyCheckpoint, 0 },
    { bhvSnowmansBottom, BHV_TYPE_INVULNERABLE },
    { bhvSnowmansHead, BHV_TYPE_INVULNERABLE },
    { bhvSnufit, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvSnufitBalls, 0 },
    { bhvSoundSpawner, 0 },
    { bhvSparkle, 0 },
    { bhvSparkleParticleSpawner, 0 },
    { bhvSparkleSpawn, 0 },
    { bhvSpawnedStar, BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL },
    { bhvSpawnedStarNoLevelExit, BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL },
    { bhvSpinAirborneCircleWarp, 0 },
    { bhvSpinAirborneWarp, 0 },
    { bhvSpindel, BHV_TYPE_DESTRUCTIBLE },
    { bhvSpindrift, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_0 },
    { bhvSpiny, BHV_TYPE_STRONG | BHV_TYPE_KNOCKABLE_0 },
    { bhvSquarishPathMoving, 0 },
    { bhvSquishablePlatform, 0 },
    { bhvSslMovingPyramidWall, 0 },
    { bhvStar, BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL },
    { bhvStarDoor, 0 },
    { bhvStarKeyCollectionPuffSpawner, 0 },
    { bhvStarSpawnCoordinates, BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL },
    { bhvStaticCheckeredPlatform, 0 },
    { bhvStaticObject, 0 },
    { bhvStrongWindParticle, 0 },
    { bhvStub, 0 },
    { bhvStub1D0C, 0 },
    { bhvStub1D70, 0 },
    { bhvSunkenShipPart, 0 },
    { bhvSunkenShipPart2, 0 },
    { bhvSushiShark, BHV_TYPE_INVULNERABLE },
    { bhvSushiSharkCollisionChild, BHV_TYPE_INVULNERABLE },
    { bhvSwimmingWarp, 0 },
    { bhvSwingPlatform, 0 },
    { bhvSwoop, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvTTC2DRotator, 0 },
    { bhvTTCCog, 0 },
    { bhvTTCElevator, 0 },
    { bhvTTCMovingBar, 0 },
    { bhvTTCPendulum, 0 },
    { bhvTTCPitBlock, 0 },
    { bhvTTCRotatingSolid, 0 },
    { bhvTTCSpinner, 0 },
    { bhvTTCTreadmill, 0 },
    { bhvTankFishGroup, 0 },
    { bhvTemporaryYellowCoin, BHV_TYPE_COIN },
    { bhvTenCoinsSpawn, 0 },
    { bhvThiBowlingBallSpawner, 0 },
    { bhvThiHugeIslandTop, 0 },
    { bhvThiTinyIslandTop, 0 },
    { bhvThreeCoinsSpawn, 0 },
    { bhvThwomp, BHV_TYPE_DESTRUCTIBLE },
    { bhvThwomp2, BHV_TYPE_DESTRUCTIBLE },
    { bhvTiltingBowserLavaPlatform, 0 },
    { bhvTinyStrongWindParticle, 0 },
    { bhvToadMessage, 0 },
    { bhvTower, 0 },
    { bhvTowerDoor, BHV_TYPE_DESTRUCTIBLE },
    { bhvTowerPlatformGroup, 0 },
    { bhvToxBox, BHV_TYPE_DESTRUCTIBLE },
    { bhvTrackBall, 0 },
    { bhvTreasureChestBottom, 0 },
    { bhvTreasureChestTop, 0 },
    { bhvTreasureChests, 0 },
    { bhvTreasureChestsJrb, 0 },
    { bhvTreasureChestsShip, 0 },
    { bhvTree, 0 },
    { bhvTreeLeaf, 0 },
    { bhvTreeSnow, 0 },
    { bhvTriangleParticleSpawner, 0 },
    { bhvTripletButterfly, 0 },
    { bhvTtmBowlingBallSpawner, 0 },
    { bhvTtmRollingLog, 0 },
    { bhvTumblingBridgePlatform, 0 },
    { bhvTuxiesMother, 0 },
    { bhvTweester, 0 },
    { bhvTweesterSandParticle, 0 },
    { bhvUkiki, BHV_TYPE_GRABBABLE },
    { bhvUkikiCage, 0 },
    { bhvUkikiCageChild, 0 },
    { bhvUkikiCageStar, BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL },
    { bhvUnagi, 0 },
    { bhvUnagiSubobject, BHV_TYPE_UNAGIS_TAIL },
    { bhvUnlockDoorStar, BHV_TYPE_STAR_MODEL },
    { bhvUnused05A8, 0 },
    { bhvUnused0DFC, 0 },
    { bhvUnused1820, 0 },
    { bhvUnused20E0, BHV_TYPE_GRABBABLE },
    { bhvUnused2A10, 0 },
    { bhvUnused2A54, 0 },
    { bhvUnusedFakeStar, BHV_TYPE_STAR_MODEL },
    { bhvUnusedParticleSpawn, 0 },
    { bhvUnusedPoundablePlatform, 0 },
    { bhvVanishCap, BHV_TYPE_CAP },
    { bhvVertStarParticleSpawner, 0 },
    { bhvVolcanoFlames, 0 },
    { bhvVolcanoSoundLoop, 0 },
    { bhvWallTinyStarParticle, 0 },
    { bhvWarp, 0 },
    { bhvWarpPipe, 0 },
    { bhvWaterAirBubble, 0 },
    { bhvWaterBomb, BHV_TYPE_STRONG },
    { bhvWaterBombCannon, 0 },
    { bhvWaterBombShadow, 0 },
    { bhvWaterBombSpawner, 0 },
    { bhvWaterDroplet, 0 },
    { bhvWaterDropletSplash, 0 },
    { bhvWaterLevelDiamond, 0 },
    { bhvWaterLevelPillar, 0 },
    { bhvWaterMist, 0 },
    { bhvWaterMist2, 0 },
    { bhvWaterSplash, 0 },
    { bhvWaterfallSoundLoop, 0 },
    { bhvWaveTrail, 0 },
    { bhvWdwExpressElevator, 0 },
    { bhvWdwExpressElevatorPlatform, 0 },
    { bhvWdwRectangularFloatingPlatform, 0 },
    { bhvWdwSquareFloatingPlatform, 0 },
    { bhvWfBreakableWallLeft, BHV_TYPE_DESTRUCTIBLE },
    { bhvWfBreakableWallRight, BHV_TYPE_WF_STAR_WALL },
    { bhvWfElevatorTowerPlatform, 0 },
    { bhvWfRotatingWoodenPlatform, 0 },
    { bhvWfSlidingPlatform, 0 },
    { bhvWfSlidingTowerPlatform, 0 },
    { bhvWfSolidTowerPlatform, 0 },
    { bhvWfTumblingBridge, 0 },
    { bhvWhirlpool, 0 },
    { bhvWhitePuff1, 0 },
    { bhvWhitePuff2, 0 },
    { bhvWhitePuffExplosion, 0 },
    { bhvWhitePuffSmoke, 0 },
    { bhvWhitePuffSmoke2, 0 },
    { bhvWhompKingBoss, BHV_TYPE_DESTRUCTIBLE | BHV_TYPE_BOSS },
    { bhvWigglerBody, 0 },
    { bhvWigglerHead, BHV_TYPE_WEAK | BHV_TYPE_BOSS },
    { bhvWind, 0 },
    { bhvWingCap, BHV_TYPE_CAP },
    { bhvWoodenPost, 0 },
    { bhvYellowBackgroundInMenu, 0 },
    { bhvYellowBall, 0 },
    { bhvYellowCoin, BHV_TYPE_COIN },
    { bhvYoshi, 0 },
#if OMM_GAME_IS_SMMS
    { bhvBee, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvgoombone, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_0 | BHV_TYPE_GOOMBA },
#endif
#if OMM_GAME_IS_R96A
    { bhvBetaBooKey, BHV_TYPE_MUSHROOM_1UP },
    { bhvWarioCoin, BHV_TYPE_MUSHROOM_1UP },
    { bhvMovingYellowCoinWario, BHV_TYPE_COIN },
    { bhvBlueCoinMotos, BHV_TYPE_COIN },
    { bhvMotosHand, 0 },
    { bhvMotos, BHV_TYPE_BULLY | BHV_TYPE_GRABBABLE },
    { bhvYoshiEgg, BHV_TYPE_COIN },
    { bhvBlargg, BHV_TYPE_FLAME },
    { bhvFriendlyBlargg, 0 },
    { bhvCharacterSwitchPipe, 0 },
    { bhvCharacterSwitchPipeWarioLocked, 0 },
    { bhvCharacterSwitchPipeWarioUnlocked, 0 },
    { bhvCharacterSwitchPipeLuigiLocked, 0 },
    { bhvCharacterSwitchPipeLuigiUnlocked, 0 },
    { bhvMilk, BHV_TYPE_MUSHROOM_1UP },
    { bhvSpambaYellowCoin, BHV_TYPE_COIN },
    { bhvSpambaRedCoin, BHV_TYPE_COIN },
    { bhvSpambaBlueCoin, BHV_TYPE_COIN },
#endif
#if OMM_GAME_IS_SMSR
    { bhvCustomSMSRBreakableRock, BHV_TYPE_BREAKABLE },
    { bhvCustomSMSRWoodenOctagonalPlatform, 0 },
    { bhvCustomSMSRShyGuy, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_0 | BHV_TYPE_GOOMBA },
    { bhvCustomSMSRBreakableWindow, BHV_TYPE_BREAKABLE | BHV_TYPE_DESTRUCTIBLE },
    { bhvCustomSMSRStarReplica, BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL },
    { bhvCustomSMSRRedOctagonalPlatform, 0 },
    { bhvCustomSMSRRedSinkingPlatform, 0 },
    { bhvCustomSMSRRedWavePlatform, 0 },
    { bhvCustomSMSRPushableTomb, 0 },
    { bhvCustomSMSRLampPath, 0 },
    { bhvCustomSMSRRotatingDonutPlatform, 0 },
    { bhvCustomSMSRWindTurbine, 0 },
    { bhvCustomSMSRLightsOnSwitch, 0 },
    { bhvCustomSMSRRotatingWoodenGear, 0 },
    { bhvCustomSMSRPeachMessage, 0 },
    { bhvCustomSMSRYoshiMessage, 0 },
    { bhvCustomSMSRStaticMessage, 0 },
    { bhvCustomSMSRStoneCubePlatform, 0 },
    { bhvCustomSMSR30StarDoorWall, 0 },
    { bhvCustomSMSRSpecialBreakeableBox, BHV_TYPE_BREAKABLE },
    { bhvCustomSMSRRecoveryBubbleWater, 0 },
    { bhvCustomSMSRBreakableFloor, BHV_TYPE_BREAKABLE },
    { bhvCustomSMSRBoatOnTrack, 0 },
    { bhvCustomSMSRAngrySun, BHV_TYPE_FLAME },
    { bhvCustomSMSRYoshiCoin, BHV_TYPE_COIN },
    { bhvCustomSMSRPiranhaPlantWild, BHV_TYPE_WEAK },
    { bhvCustomSMSRMovingMushroom, 0 },
    { bhvCustomSMSRMipsMessage, 0 },
    { bhvCustomSMSRBigLeaves, 0 },
    { bhvCustomSMSRLilyPad, 0 },
    { bhvCustomSMSRInvisibleCannon, 0 },
    { bhvCustomSMSRTambourine, 0 },
    { bhvCustomSMSRSmallBee, BHV_TYPE_WEAK | BHV_TYPE_KNOCKABLE_1 },
    { bhvCustomSMSRDrumStick, 0 },
    { bhvCustomSMSRStarMoving, BHV_TYPE_STAR_OR_KEY | BHV_TYPE_STAR_MODEL },
    { bhvCustomSMSRFallingDomino, BHV_TYPE_DESTRUCTIBLE },
    { bhvCustomSMSRLavaLift, 0 },
    { bhvCustomSMSRRotatingLavaPlatform, 0 },
    { bhvCustomSMSRSmallSwingPlatform, 0 },
    { bhvCustomSMSRSinkingDonut, 0 },
    { bhvCustomSMSRFloatingThwomp, BHV_TYPE_DESTRUCTIBLE },
    { bhvCustomSMSRTiltingPyramid, 0 },
    { bhvCustomSMSRPlatformLift, 0 },
    { bhvCustomSMSRRisingLava, 0 },
    { bhvCustomSMSRRisingTallPlatform, 0 },
    { bhvCustomSMSRSinkingPlatform, 0 },
    { bhvCustomSMSRToxicWastePlatform, 0 },
    { bhvCustomSMSRAttractedSpaceBox, BHV_TYPE_DESTRUCTIBLE },
    { bhvCustomSMSRSpaceBox, BHV_TYPE_DESTRUCTIBLE },
    { bhvCustomSMSRSpaceOctagonPlatform, 0 },
    { bhvCustomSMSRSpaceRedPlatform, 0 },
    { bhvCustomSMSRBulletMine, BHV_TYPE_BREAKABLE | BHV_TYPE_DESTRUCTIBLE },
#endif
    { omm_bhv_grab, 0 },
    { omm_bhv_damage_mario, 0 },
    { omm_bhv_dummy, 0 },
    { omm_bhv_cappy, BHV_TYPE_CAP },
    { omm_bhv_possessed_object, 0 },
    { omm_bhv_possessed_object_cap, BHV_TYPE_CAP },
    { omm_bhv_possessed_koopa_shell, 0 },
    { omm_bhv_act_select_star, BHV_TYPE_STAR_MODEL },
    { omm_bhv_star_celebration, BHV_TYPE_STAR_MODEL },
    { omm_bhv_life_up, 0 },
    { omm_bhv_fire_smoke, 0 },
    { omm_bhv_wing_glow_left_hand, 0 },
    { omm_bhv_wing_glow_right_hand, 0 },
    { omm_bhv_wing_trail, 0 },
    { omm_bhv_metal_sparkle, 0 },
    { omm_bhv_vanish_mist, 0 },
    { omm_bhv_break_particle, 0 },
    { omm_bhv_snowball, 0 },
    { omm_bhv_mr_i_beam, 0 },
    { omm_bhv_snufit_ball, 0 },
    { omm_bhv_explosion, 0 },
    { omm_bhv_shockwave_whomp, 0 },
    { omm_bhv_shockwave_spindrift, 0 },
    { omm_bhv_shockwave_fire, 0 },
    { omm_bhv_rising_lava, 0 },
    { omm_bhv_bitfs_pillar, 0 },
    { omm_bhv_goomba_stack, 0 },
    { omm_bhv_goomba_stack_capture, 0 },
    { omm_bhv_chain_chomp_free, BHV_TYPE_INVULNERABLE },
    { omm_bhv_mips, BHV_TYPE_GRABBABLE | BHV_TYPE_NO_CAPPY },
    { omm_bhv_bowser, BHV_TYPE_INVULNERABLE },
    { omm_bhv_bowser_clone, BHV_TYPE_INVULNERABLE },
    { omm_bhv_bowser_mine, 0 },
    { omm_bhv_bowser_flame, 0 },
    { omm_bhv_bowser_fireball, 0 },
    { omm_bhv_bowser_fireball_flame, 0 },
    { omm_bhv_flaming_bobomb, BHV_TYPE_INVULNERABLE },
    { omm_bhv_flaming_bobomb_aura, 0 },
    { omm_bhv_flaming_bobomb_explosion, 0 },
    { omm_bhv_sparkly_star, BHV_TYPE_STAR_OR_KEY },
    { omm_bhv_sparkly_star_hint, 0 },
    { omm_bhv_sparkly_star_1_block, BHV_TYPE_BREAKABLE },
    { omm_bhv_sparkly_star_2_block, BHV_TYPE_BREAKABLE },
    { omm_bhv_sparkly_star_3_block, BHV_TYPE_BREAKABLE },
    { omm_bhv_sparkly_star_sparkle, 0 },
    { omm_bhv_sparkly_star_sparkle_mario, 0 },
    { omm_bhv_sparkly_star_celebration, 0 },
    { omm_bhv_perry, 0 },
    { omm_bhv_perry_trail, 0 },
    { omm_bhv_perry_shockwave, 0 },
    { omm_bhv_peach_vibe_aura, 0 },
    { omm_bhv_peach_vibe_sparkle, 0 },
    { omm_bhv_peach_vibe_joy_tornado, 0 },
    { omm_bhv_peach_vibe_joy_sparkle, 0 },
    { omm_bhv_peach_vibe_joy_gust, 0 },
    { omm_bhv_peach_vibe_rage_aura, 0 },
    { omm_bhv_peach_vibe_rage_shockwave, 0 },
    { omm_bhv_peach_vibe_gloom_aura, 0 },
    { omm_bhv_peach_vibe_gloom_tear, 0 },
    { omm_bhv_peach_vibe_gloom_tear_small, 0 },
    { omm_bhv_peach_vibe_calm_aura, 0 },
    { omm_bhv_peach_vibe_calm_sparkle, 0 },
    { omm_bhv_menu_character_select_button, 0 },
    { omm_bhv_problem, 0 },
};

static u32 omm_obj_get_behavior_types(struct Object *o) {
    static OmmHMap sOmmBhvTypes = omm_hmap_zero;

    // Load everything at once
    if (OMM_UNLIKELY(!sOmmBhvTypes.k)) {
        for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmBehaviorList); ++i) {
            if (sOmmBehaviorList[i].types) {
                omm_hmap_insert(sOmmBhvTypes, (uintptr_t) sOmmBehaviorList[i].behavior, &sOmmBehaviorList[i].types);
            }
        }
    }
    
    // Set the behavior types once and update only if the behavior changes
    if (OMM_UNLIKELY(o->oBehaviorRef != (u32) (uintptr_t) o->behavior)) {
        s32 i = omm_hmap_find(sOmmBhvTypes, (uintptr_t) o->behavior);
        o->oBehaviorTypes = (i != -1 ? *omm_hmap_get(sOmmBhvTypes, s32 *, i) : 0);
        o->oBehaviorRef = (u32) (uintptr_t) o->behavior;
    }
    return o->oBehaviorTypes;
}

//
// Lists
//

s32 gOmmCappyObjectLists[] = {
    OBJ_LIST_LEVEL,
    OBJ_LIST_SURFACE,
    OBJ_LIST_POLELIKE,
    OBJ_LIST_PUSHABLE,
    OBJ_LIST_GENACTOR,
    OBJ_LIST_DESTRUCTIVE,
    -1
};

s32 gOmmInteractionObjectLists[] = {
    OBJ_LIST_DESTRUCTIVE,
    OBJ_LIST_GENACTOR,
    OBJ_LIST_PUSHABLE,
    OBJ_LIST_LEVEL,
    OBJ_LIST_DEFAULT,
    OBJ_LIST_SURFACE,
    OBJ_LIST_POLELIKE,
    -1
};

//
// Types
//

bool omm_obj_is_interactible(struct Object *o) {
    return omm_obj_get_behavior_types(o) != 0;
}

bool omm_obj_is_coin(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_COIN) != 0;
}

bool omm_obj_is_water_ring(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_WATER_RING) != 0;
}

bool omm_obj_is_mushroom_1up(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_MUSHROOM_1UP) != 0 && !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE);
}

bool omm_obj_is_star_or_key(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_STAR_OR_KEY) != 0;
}

bool omm_obj_is_star_model(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_STAR_MODEL) != 0;
}

bool omm_obj_is_cap(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_CAP) != 0;
}

bool omm_obj_is_secret(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_SECRET) != 0;
}

bool omm_obj_is_weak(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_WEAK) != 0;
}

bool omm_obj_is_strong(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_STRONG) != 0;
}

bool omm_obj_is_destructible(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_DESTRUCTIBLE) != 0;
}

bool omm_obj_is_breakable(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_BREAKABLE) != 0;
}

bool omm_obj_is_boss(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_BOSS) != 0;
}

bool omm_obj_is_invulnerable(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_INVULNERABLE) != 0;
}

bool omm_obj_is_grabbable(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_GRABBABLE) != 0;
}

bool omm_obj_is_goomba(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_GOOMBA) != 0;
}

bool omm_obj_is_bully(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_BULLY) != 0;
}

bool omm_obj_is_flame(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_FLAME) != 0;
}

bool omm_obj_is_intangible_to_cappy(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_NO_CAPPY) != 0;
}

bool omm_obj_is_kickable_board(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_KICKABLE_BOARD) != 0;
}

bool omm_obj_is_wf_star_wall(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_WF_STAR_WALL) != 0;
}

bool omm_obj_is_exclamation_box(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_EXCLAMATION_BOX) != 0 && (o->oAction == 2);
}

bool omm_obj_is_unagis_tail(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_UNAGIS_TAIL) != 0 && (o->oBehParams2ndByte == -4);
}

bool omm_obj_is_collectible(struct Object *o) {
    return (omm_obj_get_behavior_types(o) & BHV_TYPE_COLLECTIBLE) != 0;
}

//
// Object behaviors
//

static void omm_obj_init_behaviors_array(OmmArray *arr, s32 type) {
    if (OMM_UNLIKELY(!arr->p)) {
        for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmBehaviorList); ++i) {
            if (sOmmBehaviorList[i].types & type) {
                omm_array_add(*arr, ptr, sOmmBehaviorList[i].behavior);
            }
        }
    }
}

OmmArray omm_obj_get_coin_behaviors() {
    static OmmArray sOmmCoinBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmCoinBehaviors, BHV_TYPE_COIN);
    return sOmmCoinBehaviors;
}

OmmArray omm_obj_get_star_or_key_behaviors() {
    static OmmArray sOmmStarOrKeyBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmStarOrKeyBehaviors, BHV_TYPE_STAR_OR_KEY);
    return sOmmStarOrKeyBehaviors;
}

OmmArray omm_obj_get_star_model_behaviors() {
    static OmmArray sOmmStarModelBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmStarModelBehaviors, BHV_TYPE_STAR_MODEL);
    return sOmmStarModelBehaviors;
}

OmmArray omm_obj_get_cap_behaviors() {
    static OmmArray sOmmCapBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmCapBehaviors, BHV_TYPE_CAP);
    return sOmmCapBehaviors;
}

OmmArray omm_obj_get_goomba_behaviors() {
    static OmmArray sOmmGoombaBehaviors = omm_array_zero;
    omm_obj_init_behaviors_array(&sOmmGoombaBehaviors, BHV_TYPE_GOOMBA);
    return sOmmGoombaBehaviors;
}

OmmArray omm_obj_get_player_behaviors() {
    static OmmArray sOmmPlayerBehaviors = omm_array_zero;
    if (OMM_UNLIKELY(!sOmmPlayerBehaviors.p)) {
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_cappy);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_possessed_object_cap);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_wing_glow_left_hand);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_wing_glow_right_hand);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_wing_trail);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_vanish_mist);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_metal_sparkle);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_perry);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_perry_trail);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_perry_charge);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_peach_vibe_joy_tornado);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_peach_vibe_joy_sparkle);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_peach_vibe_rage_aura);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_peach_vibe_gloom_aura);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_peach_vibe_gloom_tear);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_peach_vibe_gloom_tear_small);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_peach_vibe_calm_aura);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_peach_vibe_calm_sparkle);
        omm_array_add(sOmmPlayerBehaviors, ptr, omm_bhv_sparkly_star_sparkle_mario);
    }
    return sOmmPlayerBehaviors;
}

OmmArray omm_obj_get_bowser_behaviors() {
    static OmmArray sOmmBowserBehaviors = omm_array_zero;
    if (OMM_UNLIKELY(!sOmmBowserBehaviors.p)) {
        omm_array_add(sOmmBowserBehaviors, ptr, bhvBowser);
        omm_array_add(sOmmBowserBehaviors, ptr, bhvBowserBodyAnchor);
        omm_array_add(sOmmBowserBehaviors, ptr, bhvBowserTailAnchor);
        omm_array_add(sOmmBowserBehaviors, ptr, bhvBowserFlameSpawn);
        omm_array_add(sOmmBowserBehaviors, ptr, bhvBowserBomb);
    }
    return sOmmBowserBehaviors;
}

//
// Perry attacks (include Vibes)
//

static const struct {
    const BehaviorScript *behavior;
    s32 vibeSparkles;
    s32 deathAction;
} sOmmPerryAttackBehaviorList[] = {
    { bhvBalconyBigBoo, 8, 4 },
    { bhvBigBoulder, 4, -1 },
    { bhvBigBully, 8, -1 },
    { bhvBigBullyWithMinions, 8, -1 },
    { bhvBigChillBully, 8, -1 },
    { bhvBobomb, 2, -1 },
    { bhvBoo, 3, -1 },
    { bhvBooWithCage, 3, -1 },
    { bhvBowlingBall, 5, -1 },
    { bhvBreakableBox, 5, -1 },
    { bhvBreakableBoxSmall, 5, -1 },
    { bhvChuckya, 4, -1 },
    { bhvEnemyLakitu, 3, -1 },
    { bhvEyerokHand, 5, -1 },
    { bhvFirePiranhaPlant, 3, -1 },
    { bhvFlyGuy, 3, -1 },
    { bhvFlyingBookend, 3, -1 },
    { bhvFreeBowlingBall, 5, -1 },
    { bhvGhostHuntBigBoo, 8, 4 },
    { bhvGhostHuntBoo, 3, -1 },
    { bhvGoomba, 3, -1 },
    { bhvGrindel, 5, -1 },
    { bhvHauntedChair, 5, -1 },
    { bhvHeaveHo, 5, -1 },
    { bhvHiddenObject, 2, -1 },
    { bhvHorizontalGrindel, 5, -1 },
    { bhvJrbSlidingBox, 5, -1 },
    { bhvJumpingBox, 5, -1 },
    { bhvKingBobomb, 8, 8 },
    { bhvKoopa, 3, -1 },
    { bhvMadPiano, 5, -1 },
    { bhvMerryGoRoundBigBoo, 8, 4 },
    { bhvMerryGoRoundBoo, 3, -1 },
    { bhvMoneybag, 5, -1 },
    { bhvMrBlizzard, 4, -1 },
    { bhvMrI, 4, -1 },
    { bhvPiranhaPlant, 3, PIRANHA_PLANT_ACT_WAIT_TO_RESPAWN },
    { bhvPitBowlingBall, 5, -1 },
    { bhvPokey, 3, -1 },
    { bhvPokeyBodyPart, 1, -1 },
    { bhvScuttlebug, 3, -1 },
    { bhvSkeeter, 3, -1 },
    { bhvSmallBully, 3, -1 },
    { bhvSmallChillBully, 3, -1 },
    { bhvSmallWhomp, 5, -1 },
    { bhvSnufit, 3, -1 },
    { bhvSpindel, 5, -1 },
    { bhvSpindrift, 3, -1 },
    { bhvSpiny, 2, -1 },
    { bhvSwoop, 3, -1 },
    { bhvThwomp, 5, -1 },
    { bhvThwomp2, 5, -1 },
    { bhvTowerDoor, 5, -1 },
    { bhvToxBox, 5, -1 },
    { bhvWaterBomb, 3, -1 },
    { bhvWfBreakableWallLeft, 5, -1 },
    { bhvWfBreakableWallRight, 5, -1 },
    { bhvWhompKingBoss, 8, 9 },
    { bhvWigglerHead, 8, WIGGLER_ACT_FALL_THROUGH_FLOOR },
#if OMM_GAME_IS_SMMS
    { bhvBee, 3, -1 },
    { bhvgoombone, 3, -1 },
#endif
#if OMM_GAME_IS_R96A
    { bhvMotos, 4, -1 },
    { bhvBlargg, 4, -1 },
#endif
#if OMM_GAME_IS_SMSR
    { bhvCustomSMSRBreakableRock, 4, -1 },
    { bhvCustomSMSRShyGuy, 3, -1 },
    { bhvCustomSMSRBreakableWindow, 4, -1 },
    { bhvCustomSMSRSpecialBreakeableBox, 5, -1 },
    { bhvCustomSMSRBreakableFloor, 4, -1 },
    { bhvCustomSMSRPiranhaPlantWild, 3, -1 },
    { bhvCustomSMSRSmallBee, 3, -1 },
    { bhvCustomSMSRFallingDomino, 5, -1 },
    { bhvCustomSMSRFloatingThwomp, 5, -1 },
    { bhvCustomSMSRAttractedSpaceBox, 5, -1 },
    { bhvCustomSMSRSpaceBox, 5, -1 },
    { bhvCustomSMSRBulletMine, 2, -1 },
#endif
};

#define MAX_PERRY_ATTACKS 32
typedef struct {
    struct Object *obj;
    const BehaviorScript *bhv;
    Vec3f pos;
    s32 spk;
    s32 act;
} PerryAttack;
static PerryAttack sPerryAttacks[MAX_PERRY_ATTACKS];

OMM_ROUTINE_LEVEL_ENTRY(omm_obj_init_perry_attacks) {
    OMM_MEMSET(sPerryAttacks, 0, sizeof(sPerryAttacks));
}

OMM_ROUTINE_PRE_RENDER(omm_obj_update_perry_attacks) {
    if (OMM_LIKELY(gMarioObject)) {

        // Check objects attacked by Perry
        for (s32 i = 0; i != MAX_PERRY_ATTACKS; ++i) {
            PerryAttack *patk = &sPerryAttacks[i];
            if (patk->obj) {
                if (patk->obj->behavior == patk->bhv) {
                    if (!patk->obj->activeFlags || patk->obj->oAction == patk->act) {
                        patk->pos[0] = patk->obj->oPosX;
                        patk->pos[1] = patk->obj->oPosY + max_f(0.f, patk->obj->hitboxHeight - patk->obj->hitboxDownOffset);
                        patk->pos[2] = patk->obj->oPosZ;
                        patk->obj = NULL;
                    }
                } else {
                    patk->obj = NULL;
                    patk->spk = 0;
                }
            }
        }

        // Spawn sparkles from dead objects
        for (s32 i = 0; i != MAX_PERRY_ATTACKS; ++i) {
            PerryAttack *patk = &sPerryAttacks[i];
            if (!patk->obj && patk->spk) {
                for (s32 j = 0; j != patk->spk; ++j) {
                    omm_spawn_peach_vibe_sparkle(gMarioObject, patk->pos[0], patk->pos[1], patk->pos[2]);
                }
                patk->spk = 0;
            }
        }
    }
}

static void omm_obj_handle_perry_attack(struct Object *o, u32 interactionFlags) {
    if (interactionFlags & OBJ_INT_PERRY_ATTACK) {
        for (s32 i = 0; i != OMM_ARRAY_SIZE(sOmmPerryAttackBehaviorList); ++i) {
            if (sOmmPerryAttackBehaviorList[i].behavior == o->behavior) {
                s32 vibeSparkles = sOmmPerryAttackBehaviorList[i].vibeSparkles;
                if (vibeSparkles > 0) {
                    s32 j = -1;

                    // Look for an existing object
                    for (s32 k = 0; k != MAX_PERRY_ATTACKS; ++k) {
                        PerryAttack *patk = &sPerryAttacks[k];
                        if (!patk->obj) {
                            j = k;
                        } else if (patk->obj == o) {
                            patk->bhv = o->behavior;
                            patk->spk = vibeSparkles;
                            patk->act = sOmmPerryAttackBehaviorList[i].deathAction;
                            return;
                        }
                    }

                    // Not found, add it to the list
                    if (j != -1) {
                        PerryAttack *patk = &sPerryAttacks[j];
                        patk->obj = o;
                        patk->bhv = o->behavior;
                        patk->spk = vibeSparkles;
                        patk->act = sOmmPerryAttackBehaviorList[i].deathAction;
                    }
                }
                return;
            }
        }
    }
}

//
// Interactions
//

#define CAPPY_ONLY_CODE(...) if (isCappy) { __VA_ARGS__ }
static bool sStopAndReturn;

bool omm_obj_check_interaction(struct Object *o, struct MarioState *m, bool ignoreTangibility) {
    return !(o->oInteractStatus & INT_STATUS_INTERACTED) &&     // Not interacted
            (o->oIntangibleTimer == 0 || ignoreTangibility) &&  // Not intangible
            (o->activeFlags != ACTIVE_FLAG_DEACTIVATED) &&      // Not unloading/unloaded
            (o != gOmmCapture) &&                               // Not a capture
            (o != m->usedObj || !omm_mario_is_picking_up(m)) && // Not used or not picking up
            (o != m->heldObj) &&                                // Not held
            (o != m->riddenObj);                                // Not ridden
}

static void omm_obj_interact_attract_coin(struct Object *o, struct Object *target, u32 interactionFlags, bool strong) {
    f32 oCenterY = o->oPosY + o->hitboxHeight / 2.f - o->hitboxDownOffset;
    f32 dx = target->oPosX - o->oPosX;
    f32 dy = target->oPosY - oCenterY;
    f32 dz = target->oPosZ - o->oPosZ;
    f32 distX = max_f(0.f, abs_f(dx) - o->hitboxRadius);
    f32 distY = max_f(0.f, abs_f(dy) - o->hitboxHeight / 2.f);
    f32 distZ = max_f(0.f, abs_f(dz) - o->hitboxRadius);
    f32 distToObj = sqrtf(distX * distX + distY * distY + distZ * distZ);
    f32 distPullMax = OBJ_INT_ATTRACT_COINS_RANGE_GET(interactionFlags);
    if (distToObj < distPullMax) {
        f32 velPullMax = max_f(50.f, abs_f(o->oForwardVel) * 1.25f);
        f32 velPull = velPullMax * (strong ? 1.f : relerp_0_1_f(distToObj, 0.f, distPullMax, 1.f, 0.f));
        Vec3f dv = { dx, dy, dz };
        vec3f_normalize(dv);
        vec3f_mul(dv, velPull);
        Vec4f vel = { target->oVelX, target->oVelY, target->oVelZ, target->oForwardVel };
        target->oVelX = -dv[0];
        target->oVelY = -dv[1];
        target->oVelZ = -dv[2];
        target->oWallHitboxRadius = target->hitboxRadius / 2.f;
        perform_object_step(target, 0);
        target->oVelX = vel[0];
        target->oVelY = vel[1];
        target->oVelZ = vel[2];
        target->oForwardVel = vel[3];
    }
}

static void omm_obj_interact_heal_mario(struct Object *target, u32 interactionFlags) {
    if ((interactionFlags & OBJ_INT_HEAL_MARIO) && !omm_obj_is_collectible(target)) {
        omm_health_heal_mario(gMarioState);
        gMarioState->healCounter += 4;
    }
}

static bool omm_obj_interact_defeat_in_one_hit(struct Object *o, struct Object *target) {

    // King Bob-omb
    if (target->behavior == bhvKingBobomb && target->oHealth) {
        obj_play_sound(target, SOUND_OBJ_KING_BOBOMB);
        obj_spawn_white_puff(target, SOUND_OBJ2_KING_BOBOMB_DAMAGE);
        set_camera_shake_from_point(SHAKE_POS_SMALL, target->oPosX, target->oPosY, target->oPosZ);
        obj_set_forward_vel(target, 0, 0, 0);
        target->oAction = 7;
        target->oHealth = 0;
        return true;
    }

    // King Whomp
    if (target->behavior == bhvWhompKingBoss && target->oHealth) {
        set_camera_shake_from_point(SHAKE_POS_SMALL, target->oPosX, target->oPosY, target->oPosZ);
        obj_set_forward_vel(target, 0, 0, 0);
        obj_destroy(target);
        return true;
    }

    // Big Boos
    if (target->behavior == bhvBalconyBigBoo ||
        target->behavior == bhvGhostHuntBigBoo ||
        target->behavior == bhvMerryGoRoundBigBoo) {
        target->oHealth = 1;
        target->oInteractStatus = (ATTACK_GROUND_POUND_OR_TWIRL | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
        return true;
    }

    // Big Bullies
    if (target->behavior == bhvBigBully ||
        target->behavior == bhvBigBullyWithMinions ||
        target->behavior == bhvBigChillBully) {
        target->oForwardVel = 150.f;
        target->oMoveAngleYaw = obj_get_object1_angle_yaw_to_object2((o->parentObj != NULL) ? o->parentObj : o, target);
        target->oFaceAngleYaw = target->oMoveAngleYaw + 0x8000;
        target->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
        play_sound(SOUND_OBJ_BULLY_METAL, target->oCameraToObject);
        return true;
    }

    // Eyerock hands
    if (target->behavior == bhvEyerokHand) {
        set_camera_shake_from_point(SHAKE_POS_SMALL, target->oPosX, target->oPosY, target->oPosZ);
        obj_set_forward_vel(target, 0, 0, 0);
        obj_destroy(target);
        return true;
    }

    // Wiggler
    if (target->behavior == bhvWigglerHead) {
        target->oHealth = 2;
        target->oInteractStatus = (ATTACK_GROUND_POUND_OR_TWIRL | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
        return true;
    }

    // Bowser
    if (target->behavior == bhvBowserBodyAnchor) {
        struct Object *bowser = target->parentObj;
        if (bowser && bowser->behavior == bhvBowser && bowser->oHealth && gIsBowserInteractible[bowser->oAction]) {
#if OMM_GAME_IS_R96A
            // Spamba Bowser
            if (bowser->oInteractType == INTERACT_DAMAGE) {
                return false;
            }
#endif
            // Fake a Bowser throw, set his health to 1 and spawn a mine below him
            bowser->oAction = 1;
            bowser->oPrevAction = 1;
            bowser->oSubAction = 1;
            bowser->oTimer = 0;
            bowser->oHealth = 1;
            bowser->oBowserCameraState = 10;
            obj_anim_play(bowser, 2, 1.f);
            spawn_object(bowser, MODEL_NONE, bhvBowserBomb);
            return true;
        }
        return false;
    }

    return false;
}

bool omm_obj_process_one_surface_interaction(struct Object *o, struct Object *target, u32 interactionFlags) {
    struct MarioState *m = gMarioState;
    bool isCappy = (o == omm_cappy_get_object());
    bool interacted = false;

    // Target must not be the same as current object
    // Target must be tangible to Cappy if current object is Cappy
    // Target must be valid for interaction
    if (target != o && (!isCappy || !omm_obj_is_intangible_to_cappy(target)) && omm_obj_check_interaction(target, m, true)) {

CAPPY_ONLY_CODE(
        // Capture
        if (omm_cappy_is_mario_available(m, true) && omm_mario_possess_object(m, target, false)) {
            omm_cappy_unload();
            sStopAndReturn = true;
            return true;
        }
);
        // Interactible
        if (omm_obj_is_interactible(target)) {
            interacted = true;

            // Defeat enemies in one hit
            if (interactionFlags & OBJ_INT_ATTACK_ONE_HIT) {
                if (omm_obj_is_boss(target) && omm_obj_interact_defeat_in_one_hit(o, target)) {
                    omm_obj_handle_perry_attack(target, interactionFlags);
                    sStopAndReturn = true;
                    return true;
                }
            }

            // Attackable objects
            if (interactionFlags & (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG)) {

                // Kickable board
                if (omm_obj_is_kickable_board(target)) {
                    target->oInteractStatus = INT_STATUS_INTERACTED;
                    target->oAction = 2;
CAPPY_ONLY_CODE(
                    omm_cappy_bounce_back(o);
);
                }
            }

            // Destructible objects
            if (interactionFlags & OBJ_INT_ATTACK_DESTRUCTIBLE) {
                                
                // Destructible
                if (omm_obj_is_destructible(target)) {
                    omm_obj_handle_perry_attack(target, interactionFlags);
                    omm_obj_interact_heal_mario(target, interactionFlags);
                    target->oInteractStatus = INT_STATUS_INTERACTED;
                    obj_destroy(target);
CAPPY_ONLY_CODE(
                    omm_cappy_bounce_back(o);
);
                }

                // WF star wall
                if (omm_obj_is_wf_star_wall(target)) {
                    omm_obj_handle_perry_attack(target, interactionFlags);
                    omm_obj_interact_heal_mario(target, interactionFlags);
                    target->oInteractStatus = INT_STATUS_INTERACTED;
                    audio_play_puzzle_jingle();
                    obj_destroy(target);
CAPPY_ONLY_CODE(
                    omm_cappy_bounce_back(o);
);
                }
            }
        }
    }
    return interacted;
}

bool omm_obj_process_one_object_interaction(struct Object *o, struct Object *target, u32 interactionFlags) {
    struct MarioState *m = gMarioState;
    bool isCappy = (o == omm_cappy_get_object());
    bool interacted = false;

    // Target must not be the same as current object
    // Target must be valid for interaction
    if (target != o && omm_obj_check_interaction(target, m, false)) {

        // Attract coins
        if (omm_obj_is_coin(target)) {
            if (interactionFlags & OBJ_INT_ATTRACT_COINS_STRONG) {
                omm_obj_interact_attract_coin(o, target, interactionFlags, true);
            } else if (interactionFlags & OBJ_INT_ATTRACT_COINS_WEAK) {
                omm_obj_interact_attract_coin(o, target, interactionFlags, false);
            }
        }

        // Target must be tangible to Cappy if current object is Cappy
        // Target hitbox must overlap with current object hitbox
        if ((!isCappy || !omm_obj_is_intangible_to_cappy(target)) && obj_detect_hitbox_overlap(o, target, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX_HURTBOX_MAX)) {

CAPPY_ONLY_CODE(
            // Capture
            if (omm_cappy_is_mario_available(m, true) && omm_mario_possess_object(m, target, true)) {
                omm_cappy_unload();
                sStopAndReturn = true;
                return true;
            }
);
            // Interactible
            if (omm_obj_is_interactible(target)) {
                interacted = true;

                // Defeat enemies in one hit
                if (interactionFlags & OBJ_INT_ATTACK_ONE_HIT) {
                    if (omm_obj_is_boss(target) && omm_obj_interact_defeat_in_one_hit(o, target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        sStopAndReturn = true;
                        return true;
                    }
                }

                // Triggers
                if (interactionFlags & OBJ_INT_COLLECT_TRIGGERS) {
                        
                    // Water rings
                    if (omm_obj_is_water_ring(target)) {
                        target->oInteractStatus = INT_STATUS_INTERACTED;
                        m->healCounter += 4 * target->oDamageOrCoinValue;
                    }

                    // Mushrooms 1up
                    if (omm_obj_is_mushroom_1up(target)) {
                        target->oPosX = m->pos[0];
                        target->oPosY = m->pos[1] + 60.f;
                        target->oPosZ = m->pos[2];
                    }

                    // Secrets
                    if (omm_obj_is_secret(target)) {
                        target->oPosX = m->pos[0];
                        target->oPosY = m->pos[1] + 60.f;
                        target->oPosZ = m->pos[2];
                    }
                }

                // Coins
                if (interactionFlags & OBJ_INT_COLLECT_COINS) {
                    if (omm_obj_is_coin(target)) {
                        omm_mario_interact_coin(m, target);
CAPPY_ONLY_CODE(
                        omm_cappy_try_to_target_next_coin(o);
);
                    }
                }

                // Stars/Keys
                if (interactionFlags & OBJ_INT_COLLECT_STARS) {
                    if (omm_obj_is_star_or_key(target)) {
                        omm_mario_interact_star_or_key(m, target);
CAPPY_ONLY_CODE(
                        omm_cappy_return_to_mario(o);
);
                    }
                }

                // Caps
                if (interactionFlags & OBJ_INT_COLLECT_CAPS) {
                    if (omm_obj_is_cap(target)) {
                        if (!omm_mario_interact_cap(m, target)) {
                            interact_cap(m, INTERACT_CAP, target);
                        }
                    }
                }

CAPPY_ONLY_CODE(
                // Unagi's tail
                if (omm_obj_is_unagis_tail(target)) {
                    if (target->parentObj->oAnimState != 0) {
                        target->parentObj->oAnimState = 0;
                        target->oBehParams = target->parentObj->oBehParams;
                        obj_spawn_star(target, target->oPosX, target->oPosY, target->oPosZ, 6833.f, -3654.f, 2230.f, (target->oBehParams >> 24) & 0x1F, false);
                        omm_cappy_return_to_mario(o);
                        sStopAndReturn = true;
                        return true;
                    }
                }
);

                // Grabbable
                if (interactionFlags & OBJ_INT_GRAB_OBJECTS) {
                    if (omm_obj_is_grabbable(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        if (!isCappy || (omm_cappy_is_mario_available(m, false) && !(m->action & (ACT_FLAG_AIR | ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)))) {
                            if (omm_mario_check_grab(m, target, true)) {
                                vec3f_set(m->vel, 0, 0, 0);
CAPPY_ONLY_CODE(
                                omm_cappy_return_to_mario(o);
);
                                sStopAndReturn = true;
                                return true;
                            }
                        }
                    }
                }

                // Weak
                if (interactionFlags & OBJ_INT_ATTACK_WEAK) {
                    if (omm_obj_is_weak(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        if (interactionFlags & OBJ_INT_ATTACK_KNOCKBACK) {
                            switch (omm_obj_get_behavior_types(target) & BHV_TYPE_KNOCKABLE) {
                                case BHV_TYPE_KNOCKABLE_0: obj_set_knockback(target, o, 0, 20.f, 30.f); break;
                                case BHV_TYPE_KNOCKABLE_1: obj_set_knockback(target, o, 1, 30.f, 0.f); break;
                            }
                        } else {
                            if ((interactionFlags & OBJ_INT_ATTACK_STRONG) && (
                                (omm_obj_is_goomba(target) && (target->oGoombaSize & 1)) ||
                                (target->behavior == bhvWigglerHead))) {
                                target->oInteractStatus = (ATTACK_GROUND_POUND_OR_TWIRL | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                            } else {
                                target->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                            }
                        }
                        omm_obj_interact_heal_mario(target, interactionFlags);
CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                    }
                }

                // Strong
                if (interactionFlags & OBJ_INT_ATTACK_STRONG) {
                    if (omm_obj_is_strong(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        if (interactionFlags & OBJ_INT_ATTACK_KNOCKBACK) {
                            switch (omm_obj_get_behavior_types(target) & BHV_TYPE_KNOCKABLE) {
                                case BHV_TYPE_KNOCKABLE_0: obj_set_knockback(target, o, 0, 20.f, 30.f); break;
                                case BHV_TYPE_KNOCKABLE_1: obj_set_knockback(target, o, 1, 30.f, 0.f); break;
                            }
                        } else {
                            target->oInteractStatus = INT_STATUS_INTERACTED;
                            obj_destroy(target);
                        }
                        omm_obj_interact_heal_mario(target, interactionFlags);
CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                    }
                }

                // Destructible
                if (interactionFlags & OBJ_INT_ATTACK_DESTRUCTIBLE) {
                    if (omm_obj_is_destructible(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        omm_obj_interact_heal_mario(target, interactionFlags);
                        target->oInteractStatus = INT_STATUS_INTERACTED;
                        obj_destroy(target);
CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                    }
                }

                // Breakable
                if (interactionFlags & OBJ_INT_ATTACK_BREAKABLE) {
                    if (omm_obj_is_breakable(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        if (omm_obj_is_destructible(target)) {
                            target->oInteractStatus = INT_STATUS_INTERACTED;
                            obj_destroy(target);
                        } else {
                            target->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                            set_camera_shake_from_hit(SHAKE_ATTACK);
                        }
                        omm_obj_interact_heal_mario(target, interactionFlags);
CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                    }
                }

                // Bullies
                if (interactionFlags & (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG)) {
                    if (omm_obj_is_bully(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        omm_obj_interact_heal_mario(target, interactionFlags);
                        target->oForwardVel = ((interactionFlags & OBJ_INT_ATTACK_STRONG) != 0 ? 4800.f : 3600.f) / target->hitboxRadius;
                        target->oMoveAngleYaw = obj_get_object1_angle_yaw_to_object2((o->parentObj != NULL) ? o->parentObj : o, target);
                        target->oFaceAngleYaw = target->oMoveAngleYaw + 0x8000;
                        target->oInteractStatus = (ATTACK_KICK_OR_TRIP | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                        target->oInteractStatus |= (interactionFlags & (OBJ_INT_ATTACK_WEAK | OBJ_INT_ATTACK_STRONG)) << 24; // Attack type
                        play_sound(SOUND_OBJ_BULLY_METAL, target->oCameraToObject);
CAPPY_ONLY_CODE(
                        omm_cappy_bounce_back(o);
);
                    }
                }

                // Flames
                if (interactionFlags & OBJ_INT_ATTACK_FLAMES) {
                    if (omm_obj_is_flame(target)) {
                        omm_obj_handle_perry_attack(target, interactionFlags);
                        omm_obj_interact_heal_mario(target, interactionFlags);
                        obj_spawn_white_puff(target, SOUND_GENERAL_FLAME_OUT);
                        obj_mark_for_deletion(target);
                    }
                }
            }
        }
    }
    return interacted;
}

struct Object *omm_obj_process_interactions(struct Object *o, u32 interactionFlags) {
    struct Object *interacted = NULL;
    bool isCappy = (o == omm_cappy_get_object());
    bool notInteracted = ((interactionFlags & OBJ_INT_NOT_INTERACTED) != 0);
    sStopAndReturn = false;

    // Surfaces
    if (isCappy || (interactionFlags & OBJ_INT_PRESET_ATTACK_SURFACE)) {
        s32 rn = (isCappy ? 1 : min_s(1 + ((o->hitboxRadius - 1.f) / (CELL_SIZE >> 1)), 16));
        s32 yn = (isCappy ? 1 : min_s(1 + ((o->hitboxHeight - 1.f) / (CELL_SIZE >> 4)), 16));
        f32 dr = o->hitboxRadius / rn;
        f32 dy = o->hitboxHeight / yn;
        for (s32 yi = 0; yi <= yn; ++yi) {
            for (s32 ri = 0; ri != rn; ++ri) {
                s32 an = ((ri == 0) ? 1 : ((ri + 1) * 4));
                s32 da = 0x10000 / an;
                for (s32 ai = 0; ai != an; ++ai) {
                    struct WallCollisionData hitbox;
                    hitbox.x = o->oPosX + (dr * ri) * sins(da * ai);
                    hitbox.y = o->oPosY;
                    hitbox.z = o->oPosZ + (dr * ri) * coss(da * ai);
                    hitbox.offsetY = (dy * yi) - o->hitboxDownOffset;
                    hitbox.radius = dr;
                    if (find_wall_collisions(&hitbox) != 0) {
                        for (s32 k = 0; k != hitbox.numWalls; ++k) {
                            struct Object *obj = hitbox.walls[k]->object;
                            if (obj && omm_obj_process_one_surface_interaction(o, obj, interactionFlags)) {
                                interacted = obj;
                                if (sStopAndReturn) {
                                    return (notInteracted ? NULL : interacted);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Objects
    if (obj_check_hitbox(o, OBJ_OVERLAP_FLAG_HITBOX)) {
        for_each_object_in_interaction_lists(obj) {
            if (omm_obj_process_one_object_interaction(o, obj, interactionFlags)) {
                interacted = obj;
                if (sStopAndReturn) {
                    return (notInteracted ? NULL : interacted);
                }
            }
        }
    }

    return (notInteracted ? NULL : interacted);
}
