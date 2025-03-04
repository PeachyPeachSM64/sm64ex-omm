#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

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

static void bhv_omm_sparkly_star_update() {
    struct Object *o = gCurrentObject;
    if (!OMM_SPARKLY_MODE_IS_ENABLED || !omm_sparkly_context_check_star(o)) {
        obj_mark_for_deletion(o);
        return;
    }

    // Check parent object
    if (!o->parentObj || !o->parentObj->activeFlags) {
        o->parentObj = gMarioObject;
    }

    // Actions
    switch (o->oAction) {

        // Invisible
        case 0: {

            // Move the star spawn point
            if (o->parentObj != gMarioObject && o->parentObj != gOmmCapture) {
                o->oPosX = o->parentObj->oPosX;
                o->oPosY = o->parentObj->oPosY + o->parentObj->hitboxHeight + o->parentObj->hitboxDownOffset + 150.f;
                o->oPosZ = o->parentObj->oPosZ;
            } else if (omm_mario_is_capture(gMarioState)) {
                o->oPosX = gMarioState->pos[0];
                o->oPosY = gMarioState->pos[1] + omm_capture_get_top(gOmmCapture) + 150.f;
                o->oPosZ = gMarioState->pos[2];
            } else {
                o->oPosX = gMarioState->pos[0];
                o->oPosY = gMarioState->pos[1] + 160.f * gMarioObject->oScaleY + 120.f;
                o->oPosZ = gMarioState->pos[2];
            }

            // Play jingle
            if (o->oTimer == 5) {
                audio_play_puzzle_jingle();
            }

            // Spawn sparkles that converge towards the star
            if (o->oTimer < 55) {
                omm_obj_spawn_sparkly_star_sparkle_star_spawn(o, o->oSparklyStarMode, min_s(15, 60 - o->oTimer), 0.5f, 800.f);
            }

            // Spawn star
            else if (o->oTimer >= 60) {
                o->oHomeX = o->oSparklyStarPosX;
                o->oHomeY = o->oSparklyStarPosY;
                o->oHomeZ = o->oSparklyStarPosZ;
                o->oMoveAngleYaw = atan2s(o->oHomeZ - o->oPosZ, o->oHomeX - o->oPosX);
                o->oStarSpawnDisFromHome = sqrtf(sqr_f(o->oHomeX - o->oPosX) + sqr_f(o->oHomeZ - o->oPosZ));
                o->oVelY = (o->oHomeY - o->oPosY) / 30.f;
                o->oForwardVel = o->oStarSpawnDisFromHome / 30.f;
                o->oStarSpawnUnkFC = o->oPosY;
                cutscene_object(CUTSCENE_RED_COIN_STAR_SPAWN, o);
                set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
                o->oFlags &= ~OBJ_FLAG_GFX_INITED;
                o->oAction = 1;
                o->oSubAction = 0;
                for (s32 i = 0; i != 20; ++i) {
                    omm_obj_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 80.f, 0.5f, 0);
                }
                for_each_object_with_behavior(obj, bhvOmmSparklyStarSparkle) {
                    obj->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
                    obj->parentObj = obj;
                }
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
                    o->oPosY = o->oStarSpawnUnkFC + sins((o->oTimer * 0x8000) / 30) * 400.f;
                    o->oPosZ += o->oVelZ;
                    o->oFaceAngleYaw += 0x0800;
                    obj_play_sound(o, SOUND_ENV_STAR);
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
                    obj_play_sound(o, SOUND_ENV_STAR);
                    if (o->oPosY < o->oHomeY) {
                        obj_play_sound(o, SOUND_GENERAL_STAR_APPEARS);
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
            obj_set_xyz(o, o->oSparklyStarPosX, o->oSparklyStarPosY, o->oSparklyStarPosZ);
            obj_set_home(o, o->oSparklyStarPosX, o->oSparklyStarPosY, o->oSparklyStarPosZ);
        } break;

        // In Mario's hands
        case 4: {
            Vec3f marioArmLeft; geo_get_marios_anim_part_pos(NULL, marioArmLeft, MARIO_ANIM_PART_LEFT_FOREARM);
            Vec3f marioArmRight; geo_get_marios_anim_part_pos(NULL, marioArmRight, MARIO_ANIM_PART_RIGHT_FOREARM);
            Vec3f marioHandLeft; geo_get_marios_anim_part_pos(NULL, marioHandLeft, MARIO_ANIM_PART_LEFT_HAND);
            Vec3f marioHandRight; geo_get_marios_anim_part_pos(NULL, marioHandRight, MARIO_ANIM_PART_RIGHT_HAND);
            Vec3f starPos = {
                ((2.f * marioHandLeft[0] - marioArmLeft[0]) + (2.f * marioHandRight[0] - marioArmRight[0])) / 2.f,
                ((2.f * marioHandLeft[1] - marioArmLeft[1]) + (2.f * marioHandRight[1] - marioArmRight[1])) / 2.f,
                ((2.f * marioHandLeft[2] - marioArmLeft[2]) + (2.f * marioHandRight[2] - marioArmRight[2])) / 2.f,
            };
            o->oSparklyStarPosX = starPos[0];
            o->oSparklyStarPosY = starPos[1];
            o->oSparklyStarPosZ = starPos[2];
            obj_set_xyz(o, o->oSparklyStarPosX, o->oSparklyStarPosY, o->oSparklyStarPosZ);
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
            obj_scale(o, o->oTimer / 20.f);
            o->oGraphNode = geo_layout_to_graph_node(NULL, OMM_SPARKLY_SPARKLE_GEO[o->oSparklyStarMode]);
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            o->oAnimState++;
        } break;

        // Visible, opaque
        case 1:
        case 2:
        case 4: {
            obj_scale(o, 1.f);
            o->oGraphNode = geo_layout_to_graph_node(NULL, OMM_SPARKLY_STAR_GEO_OPAQUE[o->oSparklyStarMode]);
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            if ((o->oTimer % (1 << o->oAction)) == 0) {
                omm_obj_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 8.f, 0.4f, 40.f);
            }
        } break;

        // Transparent
        case 3: {
            obj_scale(o, 1.f);
            o->oGraphNode = geo_layout_to_graph_node(NULL, OMM_SPARKLY_STAR_GEO_TRANSPARENT[o->oSparklyStarMode]);
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
        } break;
    }
}

const BehaviorScript bhvOmmSparklyStar[] = {
    OBJ_TYPE_LEVEL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_sparkly_star_update),
    BHV_END_LOOP(),
};

extern void bhv_omm_sparkly_grand_star_spawn_sparkles();
const BehaviorScript bhvOmmSparklyGrandStarEnding[] = {
    OBJ_TYPE_DEFAULT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_sparkly_grand_star_spawn_sparkles),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_sparkly_star(struct Object *o, s32 sparklyMode, f32 x, f32 y, f32 z, bool isCondStar) {
    struct Object *star = spawn_object(o, MODEL_NONE, bhvOmmSparklyStar);
    obj_set_angle(star, 0, 0, 0);
    star->oAction = 2 * !isCondStar;
    star->oTimer = 0;
    star->oRoom = -1;
    star->oSparklyStarMode = sparklyMode;
    star->oSparklyStarPosX = x;
    star->oSparklyStarPosY = y;
    star->oSparklyStarPosZ = z;
    return star;
}
