#ifndef CONFIG_CAMERA_H
#define CONFIG_CAMERA_H

#include "qol_omm.h"

#undef  CAMERA_SPECIFIC_FUNCTION_DEFINES                //QOL_DISABLE//
#define FAST_VERTICAL_CAMERA_MOVEMENT                   QOL_ENABLE
#define QOL_FIX_CAMERA_VERTICAL_MOVEMENT                QOL_ENABLE
#define QOL_FIX_CAMERA_WATER_HEIGHT                     QOL_ENABLE
#define CORRECT_ROTATE_CAMERA_AROUND_WALLS              QOL_ENABLE
#define CORRECT_COLLIDE_WITH_WALLS                      QOL_ENABLE
#define FIX_CUTSCENE_FOCUS_DEACTIVE                     QOL_ENABLE
#define QOL_FIX_OBJ_CUTSCENE_FOCUS_SOFTLOCK             QOL_ENABLE
#define QOL_FIX_CUTSCENE_LOOP                           QOL_ENABLE
#define FIX_CAMERA_BOSS_FIGHT_POS                       QOL_ENABLE
#define DS_CAM_MOVEMENT_C_UP                            QOL_ENABLE
#define QOL_FEATURE_C_UP_DS                             QOL_ENABLE
#undef  SSL_PYRAMID_CUTSCENE                            //QOL_DISABLE//
#undef  QOL_FEATURE_SSL_PYRAMID_CUTSCENE                //QOL_DISABLE//
#define CAMERA_NUM_OBSTRUCTION_CHECKS                   8

#endif
