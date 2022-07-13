#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#if defined(DEBUG)
// VSCode + GDB being shit
static Gfx *_geo_switch_anim_state(s32 callContext, struct GraphNode *node, void *context) { return geo_switch_anim_state(callContext, node, context); }
static Gfx *_geo_switch_mario_cap_on_off(s32 callContext, struct GraphNode *node, void *context) { return geo_switch_mario_cap_on_off(callContext, node, context); }
#define geo_switch_anim_state _geo_switch_anim_state
#define geo_switch_mario_cap_on_off _geo_switch_mario_cap_on_off
#endif

//
// Peach
//

#include "peach/model.inc.c"
#include "peach/geo.inc.c"

//
// Tiara
//

#include "tiara/model.inc.c"
#include "tiara/geo.inc.c"

//
// Perry
//

#include "perry/model.inc.c"
#include "perry/geo.inc.c"
