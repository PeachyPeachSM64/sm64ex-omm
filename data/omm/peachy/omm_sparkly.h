#ifndef OMM_SPARKLY_H
#define OMM_SPARKLY_H

#include "omm_sparkly_constants.h"
#include "omm_sparkly_structs.h"
#include "omm_sparkly_functions.h"

#define OMM_SPARKLY_IS_GAMEMODE_UNLOCKED        omm_sparkly_is_unlocked(OMM_SPARKLY_MODE_NORMAL)
#define OMM_SPARKLY_IS_PEACH_UNLOCKED           omm_sparkly_is_completed(OMM_SPARKLY_MODE_NORMAL)
#define OMM_SPARKLY_IS_PERRY_CHARGE_UNLOCKED    omm_sparkly_is_completed(OMM_SPARKLY_MODE_HARD)
#define OMM_SPARKLY_IS_NEBULA_FORM_UNLOCKED     omm_sparkly_is_completed(OMM_SPARKLY_MODE_LUNATIC)

#endif
