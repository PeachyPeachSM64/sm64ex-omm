#ifndef CONFIG_GRAPHICS_H
#define CONFIG_GRAPHICS_H

#include "qol_omm.h"

#define FIX_KOOPA_SHORTS                                QOL_ENABLE
#define QOL_FIX_ASSET_KOOPA_SHORTS                      QOL_ENABLE
#define FIX_BURN_SMOKE                                  QOL_ENABLE
#define QOL_FIX_ASSET_BURN_SMOKE                        QOL_ENABLE
#define UKIKI_BLINK_EYES                                QOL_ENABLE
#define KING_BOBOMB_BLINK_EYES                          QOL_ENABLE
#define QOL_FEATURE_KING_BOBOMB_BLINK                   QOL_ENABLE
#define TUXIES_MOTHER_SAD_EYES                          QOL_ENABLE
#define QOL_FEATURE_TUXIES_MOTHER_SAD_EYES              QOL_ENABLE
#undef  INTRO_JAPANESE_GAME_TEXT                        //QOL_DISABLE//
#undef  CREDITS_TEXT_STRING_FONT                        //QOL_DISABLE//
#define OBJ_OPACITY_BY_CAM_DIST                         QOL_ENABLE
#define PROPER_TREE_SHADOWS                             QOL_ENABLE
#undef  SHOW_INTERACT_OBJ_COLLECTABLE                   //QOL_DISABLE//
#undef  QOL_FEATURE_PROPER_SHOW_COLLECTABLE             //QOL_DISABLE//
#define OBJ_HOLD_TRANSPARENT_STAR                       QOL_ENABLE
#define QOL_FEATURE_BEH_HELD_TRANSPARENT_STAR           QOL_ENABLE

#endif
