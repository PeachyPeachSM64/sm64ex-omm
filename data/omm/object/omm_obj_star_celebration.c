#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Behavior
//

static void omm_bhv_star_celebration_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    s32 frame = (m->actionTimer - 1);

    // Orbits around Mario
    if (frame <= 24) {
        struct Object *sparkle = obj_spawn_from_geo(o, sparkles_geo, bhvCelebrationStarSparkle);
        obj_scale(sparkle, o->oScaleX);
        f32 ratio = (frame / 24.f);
        f32 scale = 0.5f + 0.5f * ratio;
        f32 angle = sqr_f(ratio) * (3 * 0x10000);
        f32 radius = o->oCelebStarRadius * (1.f - sqr_f(ratio));
        f32 height = o->oCelebStarHeight * ratio;
        o->oPosX = o->oHomeX + radius * sins(m->faceAngle[1] - angle);
        o->oPosY = o->oHomeY + height;
        o->oPosZ = o->oHomeZ + radius * coss(m->faceAngle[1] - angle);
        o->oFaceAngleYaw = m->faceAngle[1] + frame * 0x3000;
        obj_scale(o, scale);
    }
    
    // Slows down
    else if (frame < 32) {
        o->oFaceAngleYaw += (0x8000 >> (frame - 24));
    }
    
    // Disappears
    else if (frame < 36) {
        o->oFaceAngleYaw = m->faceAngle[1];
        obj_scale(o, 1.f - ((frame - 31) / 4.f));
    }
    
    // Grows up to 1.25x and moves up
    else if (frame < 40) {
        obj_scale(o, 1.25f * ((frame - 35) / 4.f));
        o->oPosY = o->oHomeY + o->oCelebStarHeight + (m->marioObj->oGfxPos[1] - m->pos[1]);
    }
    
    // Goes back to 1x
    else if (frame < 44) {
        obj_scale(o, 1.f + 0.25f * (1.f - ((frame - 40) / 4.f)));
    }
    
    // End
    else {
        obj_scale(o, 1.f);
    }
}

const BehaviorScript omm_bhv_star_celebration[] = {
    OBJ_TYPE_LEVEL,
    0x11010001,
    0x10120000,
    0x10140000,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_star_celebration_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_star_celebration(struct Object *o, f32 radius, f32 height) {
    struct Object *star = spawn_object(o, MODEL_STAR, omm_bhv_star_celebration);
    star->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    star->oHomeX = o->oPosX;
    star->oHomeY = o->oPosY;
    star->oHomeZ = o->oPosZ;
    star->oCelebStarRadius = radius + 80.f;
    star->oCelebStarHeight = height + 80.f;
    obj_scale(star, 0);
    return star;
}
