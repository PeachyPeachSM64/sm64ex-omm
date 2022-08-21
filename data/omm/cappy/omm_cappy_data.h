#ifndef OMM_CAPPY_DATA_H
#define OMM_CAPPY_DATA_H

enum OmmCappyBehavior {
    OMM_CAPPY_BHV_DEFAULT_GROUND,
    OMM_CAPPY_BHV_DEFAULT_AIR,
    OMM_CAPPY_BHV_UPWARDS_GROUND,
    OMM_CAPPY_BHV_UPWARDS_AIR,
    OMM_CAPPY_BHV_DOWNWARDS_GROUND,
    OMM_CAPPY_BHV_DOWNWARDS_AIR,
    OMM_CAPPY_BHV_SPIN_GROUND,
    OMM_CAPPY_BHV_SPIN_AIR,
    OMM_CAPPY_BHV_FLYING,
};

#define OMM_CAPPY_FLAG_START_ANIM                   (1 << 0)
#define OMM_CAPPY_FLAG_INTERACT_MARIO               (1 << 1)
#define OMM_CAPPY_FLAG_HOMING_ATTACK                (1 << 2)
#define OMM_CAPPY_FLAG_PLAY_AS                      (1 << 3)
#define OMM_CAPPY_FLAG_CAMERA                       (1 << 4)

#define OMM_CAPPY_LIFETIME                          (OMM_CHEAT_CAPPY_STAYS_FOREVER ? 0xFFFFFF : 120) // 4 seconds (6 days and a half for CSF cheat)
#define OMM_CAPPY_RETURN_VEL                        (120.f)
#define OMM_CAPPY_SLOWDOWN_FACTOR                   (1.15f)
#define OMM_CAPPY_HITBOX_RADIUS                     (88.f)
#define OMM_CAPPY_HITBOX_HEIGHT                     (128.f)
#define OMM_CAPPY_HITBOX_OFFSET                     (48.f)
#define OMM_CAPPY_WALL_RADIUS                       (56.f)
#define OMM_CAPPY_GFX_SCALE_X                       (1.2f)
#define OMM_CAPPY_GFX_SCALE_Y                       (1.1f)
#define OMM_CAPPY_GFX_SCALE_Z                       (1.2f)
#define OMM_CAPPY_GFX_ANGLE_VEL                     (0x2000)
#define OMM_CAPPY_GFX_VANISH_ALPHA                  (0x80)

#define OMM_CAPPY_BHV_DEFAULT_OFFSET                (50.f)
#define OMM_CAPPY_BHV_DEFAULT_VEL                   (144.f * (1.f + (0.4f * omm_mario_has_vanish_cap(gMarioState))) * omm_player_physics_get_selected_air() * omm_player_physics_get_selected_jump())
#define OMM_CAPPY_BHV_DEFAULT_CALL_BACK_START       (10)
#define OMM_CAPPY_BHV_SPIN_OFFSET                   (60.f)
#define OMM_CAPPY_BHV_SPIN_RADIUS_MAX               (240.f)
#define OMM_CAPPY_BHV_SPIN_RADIUS_GROWTH            (16.f)
#define OMM_CAPPY_BHV_SPIN_ANGLE_VEL                (0x1000)
#define OMM_CAPPY_BHV_SPIN_CALL_BACK_START          (16)
#define OMM_CAPPY_BHV_UPWARDS_OFFSET                (40.f)
#define OMM_CAPPY_BHV_UPWARDS_VEL                   (80.f)
#define OMM_CAPPY_BHV_UPWARDS_CALL_BACK_START       (10)
#define OMM_CAPPY_BHV_DOWNWARDS_OFFSET              (40.f)
#define OMM_CAPPY_BHV_DOWNWARDS_VEL                 (100.f)
#define OMM_CAPPY_BHV_DOWNWARDS_CALL_BACK_START     (10)
#define OMM_CAPPY_BHV_FLYING_OFFSET                 (30.f)
#define OMM_CAPPY_BHV_FLYING_RADIUS_MAX             (180.f)
#define OMM_CAPPY_BHV_FLYING_RADIUS_GROWTH          (12.f)
#define OMM_CAPPY_BHV_FLYING_ANGLE_VEL              (0x1000)
#define OMM_CAPPY_BHV_FLYING_CALL_BACK_START        (16)
#define OMM_CAPPY_HOMING_ATTACK_ORIGIN              (cappy->oCappyBehavior == OMM_CAPPY_BHV_FLYING ? m->marioObj : cappy)
#define OMM_CAPPY_HOMING_ATTACK_VEL                 (80.f)
#define OMM_CAPPY_HOMING_ATTACK_DURATION            (OMM_CHEAT_HOMING_ATTACK_GLOBAL_RANGE ? (OMM_CAPPY_LIFETIME - 20) : (cappy->oCappyBehavior == OMM_CAPPY_BHV_FLYING ? 6 : 5)) // Performs better with CSF cheat
#define OMM_CAPPY_SUPER_BOUNCE_FRAMES               (3)
#define OMM_CAPPY_PLAY_AS_H_VEL                     (60.f)
#define OMM_CAPPY_PLAY_AS_V_VEL                     (45.f)

#endif // OMM_CAPPY_DATA_H
