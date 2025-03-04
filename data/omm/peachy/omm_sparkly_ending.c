#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Ending cutscene
//

#if OMM_GAME_IS_SM64
static BAD_RETURN(s32) omm_sparkly_ending_cutscene_mario_and_toads(struct Camera *c) {
    cutscene_event(cutscene_ending_reset_spline, c, 0, 0);
    cutscene_event(cutscene_ending_look_at_sky, c, 0, 0);
    cutscene_event(cutscene_ending_zoom_fov, c, 0, 1049);
    cutscene_event(cutscene_ending_look_at_sky, c, 1050, -1);
    cutscene_spawn_obj(8, 1150);
    cutscene_spawn_obj(8, 1158);
    cutscene_spawn_obj(8, 1174);
    cutscene_spawn_obj(8, 1260);
}

struct Cutscene sOmmSparklyEndingCutscene[] = {
    { cutscene_ending_mario_fall, 170 },
    { cutscene_ending_mario_land, 70 },
    { cutscene_ending_mario_land_closeup, 75 },
    { cutscene_ending_stars_free_peach, 386 },
    { omm_sparkly_ending_cutscene_mario_and_toads, CUTSCENE_LOOP },
    { cutscene_ending_stop, 0 }
};
#endif

void omm_sparkly_ending_cutscene() {
#if OMM_GAME_IS_SM64
    static struct Cutscene *sCutsceneGoodEnding = NULL;
    extern struct Cutscene sCutsceneEnding[];
    switch (gOmmSparklyEnding) {
        case OMM_SPARKLY_ENDING_REGULAR:
        case OMM_SPARKLY_ENDING_PEACH: {
            if (sCutsceneGoodEnding) {
                mem_cpy(sCutsceneEnding, sCutsceneGoodEnding, sizeof(sOmmSparklyEndingCutscene));
                mem_del(sCutsceneGoodEnding);
                sCutsceneGoodEnding = NULL;
            }
        } break;

        case OMM_SPARKLY_ENDING_BAD: {
            if (!sCutsceneGoodEnding) {
                sCutsceneGoodEnding = mem_dup(sCutsceneEnding, sizeof(sOmmSparklyEndingCutscene));
                mem_cpy(sCutsceneEnding, sOmmSparklyEndingCutscene, sizeof(sOmmSparklyEndingCutscene));
            }
        } break;
    }
#endif
}
