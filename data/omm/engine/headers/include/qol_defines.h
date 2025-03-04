#ifndef QOL_DEFINES_H
#define QOL_DEFINES_H

#include "qol_omm.h"
#include "config/config_camera.h"
#include "config/config_collision.h"
#include "config/config_graphics.h"
#include "config/config_world.h"

#undef  ZELDA_STYLE_LEVEL_SELECT                        //QOL_DISABLE//
#undef  PLATFORM_DISPLACEMENT_2                         //QOL_DISABLE//
#undef  OPTIMIZED_SHADOWS                               //QOL_FIXED_BY_OMM//

#undef  QOL_FEATURE_Z_BUTTON_EXTRA_OPTION               //QOL_DISABLE//
#undef  QOL_FEATURE_MARIO_HEAD_EASTER_EGG               //QOL_DISABLE//
#undef  QOL_FEATURE_SPECIAL_TRIPLE_JUMP_AIR_STEPS       //QOL_DISABLE//
#undef  QOL_FEATURE_SSL_CRACKED_ICE                     //QOL_DISABLE//
#undef  QOL_FEATURE_BETTER_WF_BREAKEABLE_WALL           //QOL_FIXED_BY_OMM//
#define QOL_FEATURE_STAR_GRAB_NO_FALL_DEATH             QOL_ENABLE
#undef  QOL_FEATURE_HANG_AIR_ANYWHERE                   //QOL_DISABLE//
#undef  QOL_FEATURE_ROOM_OBJECT_CAMERA_FOCUS            //QOL_FIXED_BY_OMM//
#undef  QOL_FEATURE_BETTER_REDS_STAR_MARKER             //QOL_DISABLE//
#undef  QOL_FEATURE_RESPAWN_BLUE_COIN_SWITCH            //QOL_FIXED_BY_OMM//
#undef  QOL_FEATURE_BETTER_HANGING                      //QOL_FIXED_BY_OMM//
#define QOL_FEATURE_STAR_GRAB_NO_FALL_HEIGHT            QOL_ENABLE
#undef  QOL_FEATURE_BETTER_OBJ_COLLISIONS               //QOL_DISABLE//
#undef  QOL_FEATURE_SMOOTH_SQUISH                       //QOL_DISABLE//
#undef  QOL_FEATURE_SMOOTH_WATER_FLOOR_PITCH            //QOL_DISABLE//
#undef  QOL_FEATURE_BOUNCE_BOOST                        //QOL_FIXED_BY_OMM//
#undef  QOL_FEATURE_HOOT_TREE_PARTICLES                 //QOL_DISABLE//
#undef  QOL_FEATURE_TINY_GOOMBA_DROP_COIN               //QOL_DISABLE//
#define QOL_FEATURE_CLAMS_ONLY_HURT_WHEN_MOVING         QOL_ENABLE
#undef  QOL_FEATURE_CONTROLLABLE_PLATFORM_SPEED         //QOL_DISABLE//
#define QOL_FEATURE_BETTER_HOOT_YAW_CONTROL             QOL_ENABLE
#define QOL_FEATURE_LEDGE_PROTECTION                    QOL_ENABLE
#define QOL_FEATURE_KOOPA_SHELL_BOXES_RESPAWN           QOL_ENABLE
#undef  QOL_FEATURE_BETTER_ROOM_CHECKS                  //QOL_DISABLE//
#undef  QOL_FEATURE_VELOCITY_BASED_TURN_SPEED           //QOL_FIXED_BY_OMM//

#define QOL_FIX_CASTLE_TRAP_SOUND                       QOL_ENABLE
#undef  QOL_FIX_WATER_RING                              //QOL_FIXED_BY_OMM//
#define QOL_FIX_MISSING_SOUNDS_KEY_EXIT                 QOL_ENABLE
#define QOL_FIX_TUXIE_HELD_STATE                        QOL_ENABLE
#define QOL_FIX_BULLY_KNOCKBACK_TIMER                   QOL_ENABLE
#define QOL_FIX_CAMERA_BOSS_FIGHT_HEIGHT                QOL_ENABLE
#define QOL_FIX_AUDIO_LOAD_BANK_NOT_SEQUENCE            QOL_ENABLE
#define QOL_FIX_DROWING_SOUND_METAL                     QOL_ENABLE
#undef  QOL_FIX_COMPLETE_COURSE_50_COINS                //QOL_DISABLE//
#undef  QOL_FIX_HAT_CLONE_FADE                          //QOL_DISABLE//
#define QOL_FIX_DOOR_KEY_CUTSCENE                       QOL_ENABLE
#define QOL_FIX_YELLOW_SPARKLES_OFFSET                  QOL_ENABLE
#define QOL_FIX_OOB_OBJ_CRASH_OVERFLOW                  QOL_ENABLE
#define QOL_FIX_MISSING_SEAWEED_ANIMFRAME               QOL_ENABLE
#define QOL_FIX_TILTING_PYRAMID_Y_DISTANCE              QOL_ENABLE
#define QOL_FIX_SURFACE_WIND_DETECTION                  QOL_ENABLE
#define QOL_FIX_KING_BOBOMB_MUSIC_THROWN_OFF            QOL_ENABLE
#define QOL_FIX_INSTANT_WARP_OFFSET                     QOL_ENABLE
#define QOL_FIX_OBJ_FLOOR_WIND_DEATH                    QOL_ENABLE
#define QOL_FIX_HARDCODED_TREE_PARTICLES                QOL_ENABLE
#define QOL_FIX_HIDE_IN_HOLE_NULL_CHECK                 QOL_ENABLE
#define QOL_FIX_SPAWN_OBJECT_ANGLE_ROLL                 QOL_ENABLE
#undef  QOL_FIX_BOO_MOVE_HIT_VELOCITY                   //QOL_DISABLE//
#define QOL_FIX_BOWSER_TRANSPARENT_HELD                 QOL_ENABLE
#define QOL_FIX_LANDING_CANCEL_OFF_FLOOR                QOL_ENABLE
#define QOL_FIX_JUMP_KICK_NOT_SLIPPERY                  QOL_ENABLE
#define QOL_FIX_BULLY_BACK_UP_TIMER                     QOL_ENABLE
#define QOL_FIX_MARIO_LOOK_HEAD_BOSSES                  QOL_ENABLE
#define QOL_FIX_SHORT_HITBOX_SLIDE_ACTS                 QOL_ENABLE
#define QOL_FIX_RESET_PSS_SLIDE_STARTED                 QOL_ENABLE
#define QOL_FIX_DROPPED_UNDERWATER_SHELL                QOL_ENABLE
#define QOL_FIX_PLATFORM_TRACK_CHECKERED                QOL_ENABLE
#define QOL_FIX_CAMERA_CUTSCENE_MOVING_PLATFORMS        QOL_ENABLE
#define QOL_FIX_HORIZONTAL_WIND_SPEED                   QOL_ENABLE
#undef  QOL_FIX_POLE_BOTTOM_GRAB                        //QOL_FIXED_BY_OMM//
#define QOL_FIX_HIT_WALL_ACTION                         QOL_ENABLE
#undef  QOL_FIX_END_CUTSCENE_MARIO_FLOOR                //QOL_DISABLE//
#undef  QOL_FIX_GROUND_POUND_WALL                       //QOL_FIXED_BY_OMM//
#define QOL_FIX_SPAWNED_STAR_SOFTLOCK                   QOL_ENABLE
#undef  QOL_FIX_GOOMBA_JUMP_AIR                         //QOL_DISABLE//
#undef  QOL_FIX_GOOMBA_DEAD_ATTACKED_MARIO              //QOL_FIXED_BY_OMM//
#define QOL_FIX_PUSH_MARIO_OUT_OF_OBJECT_FLOOR          QOL_ENABLE
#define QOL_FIX_MARIO_KOOPA_SHELL_ACTION                QOL_ENABLE
#undef  QOL_FIX_WATER_GRAB_NOT_GRABBABLE                //QOL_DISABLE//
#undef  QOL_FIX_STATIONARY_GROUND_STEPS                 //QOL_FIXED_BY_OMM//
#undef  QOL_FIX_LAVA_INTERACTION                        //QOL_DISABLE//
#define QOL_FIX_PALM_TREE_LEAF_HEIGHT                   QOL_ENABLE
#define QOL_FIX_CEILING_NULL_CRASH                      QOL_ENABLE
#define QOL_FIX_SHELL_SPEED_NEGATIVE_OFFSET             QOL_ENABLE
#define QOL_FIX_LESS_GROUND_BONKS                       QOL_ENABLE
#define QOL_FIX_SPINY_CHECK_ACTIVE_PARENT               QOL_ENABLE
#define QOL_FIX_SIDE_FLIP_VISUAL_LOOK                   QOL_ENABLE
#undef  QOL_FIX_NO_DISAPPEARANCE_SQUISH                 //QOL_DISABLE//
#define QOL_FIX_MR_I_EYEBALL_POSITION                   QOL_ENABLE
#define QOL_FIX_MR_I_DELETED_PARENT                     QOL_ENABLE
#define QOL_FIX_FLOOR_SLOPE_OOB                         QOL_ENABLE
#undef  QOL_FIX_STAR_DOOR_ROOM_RENDER                   //QOL_DISABLE//
#undef  QOL_FIX_UNUSED_BETA_CHEST_YAW                   //QOL_DISABLE//
#undef  QOL_FIX_UNUSED_PLAY_MODE                        //QOL_DISABLE//
#undef  QOL_FIX_ASSET_IMPOSSIBLE_COINS                  //QOL_DISABLE//
#undef  QOL_FIX_ASSET_CCM_SLIDE_1UPS                    //QOL_DISABLE//
#define QOL_FIX_OBJ_COUNT_LIST                          QOL_ENABLE
#define QOL_FIX_MARIO_IS_IN_ROOM                        QOL_ENABLE
#define QOL_FIX_FILE_SELECT_SCORE_COURSE_LIST           QOL_ENABLE
#define QOL_FIX_MENU_SCROLLING                          QOL_ENABLE
#undef  QOL_FIX_RED_COIN_STAR_MARKER_POSITION           //QOL_DISABLE//
#define QOL_FIX_CUTSCENE_DOOR_CHECK                     QOL_ENABLE
#undef  QOL_FIX_MATH_UTIL_RETURN_VALUES                 //QOL_FIXED_BY_OMM//
#undef  QOL_FIX_GROUND_TURN_RADIUS                      //QOL_FIXED_BY_OMM//
#undef  QOL_FIX_UNUSED_LOOP_AUDIO_EU_SH                 //QOL_DISABLE//
#define QOL_FIX_UNUSED_FREE_BOWLING_BALL_COL_FLAGS      QOL_ENABLE
#undef  QOL_FIX_ASSET_DUP_STAR_GD                       //QOL_DISABLE//
#define QOL_FIX_ASSET_YOSHI_DIALOG                      QOL_ENABLE

#endif // QOL_DEFINES_H
