#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

OMM_ROUTINE_UPDATE(omm_sparkly_update) {
    struct MarioState *m = gMarioState;
    
    // Check requirements
    if (omm_sparkly_check_requirements(m)) {
        return;
    }
    
    // Update game state
    if (omm_sparkly_check_game_state(m)) {
        return;
    }

    // Update star blocks
    if (omm_sparkly_check_star_block(m)) {
        return;
    }

    // Update the anti-cheat
    if (omm_sparkly_check_anti_cheat(m)) {
        return;
    }

    // Update Bowser 4
    if (omm_sparkly_check_bowser_4(m)) {
        return;
    }
    
    // Update context
    omm_sparkly_context_update(m);
    omm_sparkly_context_spawn_star(m);
    omm_sparkly_context_spawn_sign(m);
    gOmmSparklyContext->inited = true;
}
