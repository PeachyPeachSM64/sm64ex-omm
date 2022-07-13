#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layouts
//

const GeoLayout omm_geo_sparkly_star_1_opaque[] = {
    OMM_GEO_BRANCH(omm_geo_star_17_opaque),
};

const GeoLayout omm_geo_sparkly_star_1_transparent[] = {
    OMM_GEO_BRANCH(omm_geo_star_17_transparent),
};

const GeoLayout omm_geo_sparkly_star_2_opaque[] = {
    OMM_GEO_BRANCH(omm_geo_star_18_opaque),
};

const GeoLayout omm_geo_sparkly_star_2_transparent[] = {
    OMM_GEO_BRANCH(omm_geo_star_18_transparent),
};

const GeoLayout omm_geo_sparkly_star_3_opaque[] = {
    OMM_GEO_BRANCH(omm_geo_star_19_opaque),
};

const GeoLayout omm_geo_sparkly_star_3_transparent[] = {
    OMM_GEO_BRANCH(omm_geo_star_19_transparent),
};

//
// Behavior
//

static void omm_bhv_sparkly_star_update() {
    struct Object *o = gCurrentObject;
    if (!OMM_SSM_IS_ENABLED || !omm_ssc_check_star(o)) {
        obj_mark_for_deletion(o);
        return;
    }

    // Actions
    switch (o->oAction) {

        // Invisible
        case 0: {
            if (o->oTimer == 15) {
                audio_play_puzzle_jingle();
            } else if (o->oTimer >= 90) {
                o->oHomeX = o->oSparklyStarPosX;
                o->oHomeY = o->oSparklyStarPosY;
                o->oHomeZ = o->oSparklyStarPosZ;
                o->oPosX = gMarioState->pos[0];
                o->oPosY = gMarioState->pos[1] + 80;
                o->oPosZ = gMarioState->pos[2];
                o->oMoveAngleYaw = atan2s(o->oHomeZ - o->oPosZ, o->oHomeX - o->oPosX);
                o->oStarSpawnDisFromHome = sqrtf(sqr_f(o->oHomeX - o->oPosX) + sqr_f(o->oHomeZ - o->oPosZ));
                o->oVelY = (o->oHomeY - o->oPosY) / 30.0f;
                o->oForwardVel = o->oStarSpawnDisFromHome / 30.0f;
                o->oStarSpawnUnkFC = o->oPosY;
                cutscene_object(CUTSCENE_RED_COIN_STAR_SPAWN, o);
                set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
                o->oAction = 1;
                o->oSubAction = 0;
            }
        } break;

        // Spawning
        // During this phase, the context is not updated
        case 1: {
            switch (o->oSubAction) {

                // Rotating on place
                case 0: {
                    o->oFaceAngleYaw += 0x0800;
                    if (o->oTimer >= 20) {
                        o->oSubAction++;
                        o->oTimer = 0;
                    }
                } break;

                // Moving to its location
                case 1: {
                    o->oVelX = o->oForwardVel * sins(o->oMoveAngleYaw);
                    o->oStarSpawnUnkFC += o->oVelY;
                    o->oVelZ = o->oForwardVel * coss(o->oMoveAngleYaw);
                    o->oPosX += o->oVelX;
                    o->oPosY = o->oStarSpawnUnkFC + sins((o->oTimer * 0x8000) / 30) * 400.0f;
                    o->oPosZ += o->oVelZ;
                    o->oFaceAngleYaw += 0x0800;
                    play_sound(SOUND_ENV_STAR, o->oCameraToObject);
                    if (o->oTimer >= 30) {
                        audio_play_star_jingle();
                        o->oForwardVel = 0;
                        o->oSubAction++;
                        o->oTimer = 0;
                    }
                } break;

                // Bounce
                case 2: {
                    o->oVelX = o->oForwardVel * sins(o->oMoveAngleYaw);
                    o->oVelY = max_f(20 - o->oTimer, -10);
                    o->oVelZ = o->oForwardVel * coss(o->oMoveAngleYaw);
                    o->oPosX += o->oVelX;
                    o->oPosY += o->oVelY;
                    o->oPosZ += o->oVelZ;
                    o->oFaceAngleYaw += max_s(0, 0x0800 - o->oTimer * 0x10);
                    play_sound(SOUND_ENV_STAR, o->oCameraToObject);
                    if (o->oPosY < o->oHomeY) {
                        play_sound(SOUND_GENERAL_STAR_APPEARS, o->oCameraToObject);
                        o->oPosY = o->oHomeY;
                        o->oSubAction++;
                        o->oTimer = 0;
                    }
                } break;

                // Ready
                case 3: {
                    if (o->oTimer >= 20) {
                        gObjCutsceneDone = TRUE;
                        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                        o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
                        o->oAction = 2;
                    }
                } break;
            }
        } break;

        // Visible
        case 2:
        case 3: {
            obj_set_pos(o, o->oSparklyStarPosX, o->oSparklyStarPosY, o->oSparklyStarPosZ);
            obj_set_home(o, o->oSparklyStarPosX, o->oSparklyStarPosY, o->oSparklyStarPosZ);
        } break;

        // In Mario's hands
        case 4: {
            f32 *marioArmLeft = geo_get_marios_forearm_pos(1);
            f32 *marioArmRight = geo_get_marios_forearm_pos(0);
            f32 *marioHandLeft = geo_get_marios_hand_pos(1);
            f32 *marioHandRight = geo_get_marios_hand_pos(0);
            Vec3f starPos = {
                ((2.f * marioHandLeft[0] - marioArmLeft[0]) + (2.f * marioHandRight[0] - marioArmRight[0])) / 2.f,
                ((2.f * marioHandLeft[1] - marioArmLeft[1]) + (2.f * marioHandRight[1] - marioArmRight[1])) / 2.f,
                ((2.f * marioHandLeft[2] - marioArmLeft[2]) + (2.f * marioHandRight[2] - marioArmRight[2])) / 2.f,
            };
            o->oSparklyStarPosX = starPos[0];
            o->oSparklyStarPosY = starPos[1];
            o->oSparklyStarPosZ = starPos[2];
            obj_set_pos(o, o->oSparklyStarPosX, o->oSparklyStarPosY, o->oSparklyStarPosZ);
            obj_set_home(o, o->oSparklyStarPosX, o->oSparklyStarPosY, o->oSparklyStarPosZ);
        } break;
    }

    // Interaction
    bool tangible = (o->oAction == 2);
    s32 interactType = INTERACT_STAR_OR_KEY * tangible;
    f32 hitboxRadius = 50.f * tangible;
    f32 hitboxHeight = 100.f * tangible;
    obj_set_params(o, interactType, 0, 0, 0, tangible);
    obj_reset_hitbox(o, hitboxRadius, hitboxHeight, 0, 0, 0, hitboxHeight / 2.f);

    // Gfx
    o->oFaceAngleYaw += 0x0800;
    switch (o->oAction) {

        // Invisible
        case 0: {
            o->oGraphNode = NULL;
            o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        } break;

        // Visible, opaque
        case 1:
        case 2:
        case 4: {
            o->oGraphNode = geo_layout_to_graph_node(NULL, OMM_SSX_STAR_GEO_OPAQUE[o->oSparklyStarMode]);
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            if ((o->oTimer % (1 << o->oAction)) == 0) {
                omm_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 8.f, 0.4f, 40.f);
            }
        } break;

        // Transparent
        case 3: {
            o->oGraphNode = geo_layout_to_graph_node(NULL, OMM_SSX_STAR_GEO_TRANSPARENT[o->oSparklyStarMode]);
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
        } break;
    }
}

const BehaviorScript omm_bhv_sparkly_star[] = {
    OBJ_TYPE_LEVEL,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_sparkly_star_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_sparkly_star(struct Object *o, s32 mode, f32 x, f32 y, f32 z, bool isCondStar) {
    struct Object *star = spawn_object(o, MODEL_NONE, omm_bhv_sparkly_star);
    obj_set_angle(star, 0, 0, 0);
    star->oAction = 2 * !isCondStar;
    star->oTimer = 0;
    star->oRoom = -1;
    star->oSparklyStarMode = mode;
    star->oSparklyStarPosX = x;
    star->oSparklyStarPosY = y;
    star->oSparklyStarPosZ = z;
    return star;
}
