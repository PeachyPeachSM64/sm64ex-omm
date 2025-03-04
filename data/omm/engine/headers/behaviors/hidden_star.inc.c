#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

#include "actors/group0.h" // purple_marble_geo

//
// Hidden star
//

void bhv_hidden_star_init(void) {
}

void bhv_hidden_star_loop(void) {
    switch (o->oAction) {

        // Init
        case 0: {
            s32 numSecrets = count_objects_with_behavior(bhvHiddenStarTrigger);
            if (numSecrets == 0) {
                struct Object *star = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
                star->oBehParams = o->oBehParams;
                obj_mark_for_deletion(o);
            }
            if (gOmmArea->secrets == 0) {
                gOmmArea->secrets = numSecrets;
            }
            if (gOmmArea->secretStarIndex == -1) {
                gOmmArea->secretStarIndex = (o->oBehParams >> 24);
            }
            o->oHiddenStarTriggerCollected = gOmmArea->secrets - numSecrets;
            o->oAction = 1;
        } break;

        // Update
        case 1: {
            if (o->oHiddenStarTriggerCollected >= gOmmArea->secrets) {
                o->oAction = 2;
            }
        } break;

        // Spawn star
        case 2: {
            if (o->oTimer > 2) {
                spawn_red_coin_cutscene_star(o->oPosX, o->oPosY, o->oPosZ);
                spawn_mist_particles();
                obj_mark_for_deletion(o);
            }
        } break;
    }
}

//
// Hidden star trigger (secret)
//

void bhv_hidden_star_trigger_loop(void) {
#if !OMM_GAME_IS_SMGS
    if (OMM_EXTRAS_REVEAL_SECRETS) {
        o->oGraphNode = geo_layout_to_graph_node(NULL, purple_marble_geo);
        o->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
    } else {
        o->oGraphNode = NULL;
        o->oNodeFlags &= ~GRAPH_RENDER_BILLBOARD;
    }
#endif

    if (obj_check_if_collided_with_object(o, gMarioObject)) {
        struct Object *hiddenStar = obj_get_first_with_behavior(bhvHiddenStar);
        if (hiddenStar) {
            hiddenStar->oHiddenStarTriggerCollected++;
            s32 maxVariations = min_s(7, gOmmArea->secrets - 1);
            s32 soundVariation = clamp_s(maxVariations - (gOmmArea->secrets - hiddenStar->oHiddenStarTriggerCollected), 0, maxVariations);
            omm_obj_spawn_orange_number(gMarioObject,
                hiddenStar->oHiddenStarTriggerCollected, 80,
                SOUND_MENU_COLLECT_SECRET, soundVariation,
                omm_geo_number
            );
        }
        obj_mark_for_deletion(o);
        omm_stats_increase(secretsCollected, 1);
    }
}
