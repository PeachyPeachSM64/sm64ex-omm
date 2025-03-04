#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

static void bhv_omm_sparkly_star_celebration_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;

    // Rotates and moves up
    if (o->oTimer < 29) {
        o->oFaceAngleYaw = m->faceAngle[1] + o->oTimer * (0x10000 / 6);
        o->oPosY += 4.f * sins(o->oTimer * (0x8000 / 25));
        if ((o->oTimer % 2) == 0) {
            omm_obj_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 8.f, 0.4f, 40.f);
        }
    }

    // Sparkles
    else if (o->oTimer == 29) {
        o->oFaceAngleYaw = m->faceAngle[1];
        for (s32 i = 0; i != 32; ++i) {
            omm_obj_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 32.f, 0.75f, 80.f);
        }
    }

    // Small sparkles
    else {
        if ((o->oTimer % 4) == 0) {
            omm_obj_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 8.f, 0.4f, 40.f);
        }
    }
}

const BehaviorScript bhvOmmSparklyStarCelebration[] = {
    OBJ_TYPE_LEVEL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_ADD_FLOAT(oPosY, 192),
    BHV_SET_INT(oFaceAnglePitch, 0),
    BHV_SET_INT(oFaceAngleRoll, 0),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_sparkly_star_celebration_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_sparkly_star_celebration(struct Object *o, s32 sparklyMode) {
    struct Object *star = obj_spawn_from_geo(o, OMM_SPARKLY_STAR_GEO_OPAQUE[sparklyMode], bhvOmmSparklyStarCelebration);
    star->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    star->oSparklyStarMode = sparklyMode;
    return star;
}
