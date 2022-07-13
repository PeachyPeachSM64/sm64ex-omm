#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Behavior
//

static void omm_bhv_sparkly_star_celebration_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;

    // Rotates and moves up
    if (o->oTimer < 29) {
        o->oFaceAngleYaw = m->faceAngle[1] + o->oTimer * (0x10000 / 6);
        o->oPosY += 4.f * sins(o->oTimer * (0x8000 / 25));
        if ((o->oTimer % 2) == 0) {
            omm_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 8.f, 0.4f, 40.f);
        }
    }
    
    // Sparkles
    else if (o->oTimer == 29) {
        o->oFaceAngleYaw = m->faceAngle[1];
        for (s32 i = 0; i != 32; ++i) {
            omm_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 32.f, 0.75f, 80.f);
        }
    }
    
    // Small sparkles
    else {
        if ((o->oTimer % 4) == 0) {
            omm_spawn_sparkly_star_sparkle(o, o->oSparklyStarMode, 0, 8.f, 0.4f, 40.f);
        }
    }
}

const BehaviorScript omm_bhv_sparkly_star_celebration[] = {
    OBJ_TYPE_LEVEL,
    0x11010001,
    0x0D0700C0,
    0x10120000,
    0x10140000,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_sparkly_star_celebration_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_sparkly_star_celebration(struct Object *o, s32 mode) {
    struct Object *star = obj_spawn_from_geo(o, OMM_SSX_STAR_GEO_OPAQUE[mode], omm_bhv_sparkly_star_celebration);
    star->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    star->oSparklyStarMode = mode;
    return star;
}
