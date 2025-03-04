#ifndef OMM_SPARKLY_H
#define OMM_SPARKLY_H

#include "omm_sparkly_constants.h"
#include "omm_sparkly_structs.h"
#include "omm_sparkly_functions.h"

#define OMM_SPARKLY_BOWSER_4_LEVEL_ENTRY        level_castle_grounds_entry
#define OMM_SPARKLY_BOWSER_4_LEVEL              (LEVEL_CASTLE_GROUNDS)
#define OMM_SPARKLY_BOWSER_4_AREA               (2)

#define OMM_SPARKLY_IS_MODE_COMPLETED           (omm_sparkly_is_completed(gOmmSparklyMode))

#define OMM_SPARKLY_BYPASS_RESTRICTIONS         (OMM_SPARKLY_MODE_IS_NORMAL && gOmmSparklyContext->starSpawned)
#define OMM_SPARKLY_BYPASS_RULES                (OMM_SPARKLY_IS_MODE_COMPLETED)
#define OMM_SPARKLY_BYPASS_BOWSER_RULES         (OMM_SPARKLY_IS_MODE_COMPLETED)
#define OMM_SPARKLY_ENABLE_PLAYER_PHYSICS       (omm_sparkly_is_completed(OMM_SPARKLY_MODE_LUNATIC) || !OMM_SPARKLY_MODE_IS_LUNATIC)
#define OMM_SPARKLY_NEBULA_CURSE                (!omm_sparkly_is_completed(OMM_SPARKLY_MODE_LUNATIC) && OMM_SPARKLY_MODE_IS_LUNATIC)

#endif
