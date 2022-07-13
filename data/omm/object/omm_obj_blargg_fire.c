#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layouts
//

const GeoLayout omm_geo_blargg_fire_ball[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(red_flame_geo),
};

//
// Behavior (fire drop)
//

static void omm_bhv_blargg_fire_drop_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= 90) {
        obj_mark_for_deletion(o);
        return;
    }

    f32 velY = o->oVelY;
    perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STICKY_FEET);
    if (obj_is_on_ground(o)) {
        o->oVelY = abs_f(velY) / 2.f;
    } else {
        o->oVelY -= 2.f;
    }

    f32 maxScale = o->oAction / 100.f;
    obj_scale(o, relerp_0_1_f(o->oTimer, 60, 90, maxScale, 0.f));
    obj_update_gfx(o);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 16, 24, 0, 0, 8, 12);
    omm_obj_process_interactions(o, OBJ_INT_PRESET_BLARGG_FIRE);
    o->oAnimState++;
}

const BehaviorScript omm_bhv_blargg_fire_drop[] = {
    OBJ_TYPE_SPECIAL,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_blargg_fire_drop_update,
    0x09000000,
};

//
// Behavior (fire trail)
//

static void omm_bhv_blargg_fire_trail_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= 15) {
        obj_mark_for_deletion(o);
        return;
    }

    f32 maxScale = o->oAction / 100.f;
    obj_scale(o, relerp_0_1_f(o->oTimer, 0, 15, maxScale, 0.f));
    obj_update_gfx(o);
    o->oAnimState++;
}

const BehaviorScript omm_bhv_blargg_fire_trail[] = {
    OBJ_TYPE_UNIMPORTANT,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_blargg_fire_trail_update,
    0x09000000,
};

//
// Behavior (fire ball)
//

static void omm_bhv_blargg_fire_ball_delete(struct Object *o) {
    for (s32 i = 0; i != 10; ++i) {
        struct Object *drop = obj_spawn_from_geo(o, omm_geo_fire_smoke_red, omm_bhv_blargg_fire_drop);
        s16 angle = (s16) random_u16();
        f32 velXZ = lerp_f(random_float(), 0.f, 8.f);
        f32 velY = lerp_f(random_float(), 30.f, 40.f);
        drop->oVelX = velXZ * sins(angle);
        drop->oVelY = velY;
        drop->oVelZ = velXZ * coss(angle);
        drop->oAction = (s32) (100.f * lerp_f(random_float(), 3.f, 4.f));
    }
    obj_spawn_white_puff(o, SOUND_GENERAL_FLAME_OUT);
    obj_mark_for_deletion(o);
}

static void omm_bhv_blargg_fire_ball_update() {
    struct Object *o = gCurrentObject;
    perform_object_step(o, OBJ_STEP_UPDATE_HOME);
    o->oVelY -= 1.f;

    // Collided with a floor
    if (o->oDistToFloor <= 0.f) {
        omm_bhv_blargg_fire_ball_delete(o);
        return;
    }

    // Update
    obj_update_gfx(o);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 16, 24, 0, 0, 8, 12);
    omm_obj_process_interactions(o, OBJ_INT_PRESET_BLARGG_FIRE);
    o->oAnimState++;

    // Spawn fire trail
    struct Object *trail = obj_spawn_from_geo(o, omm_geo_fire_smoke_red, omm_bhv_blargg_fire_trail);
    obj_scale(trail, o->oScaleX);
    trail->oAction = (s32) (100.f * trail->oScaleX);
    trail->oAnimState = (random_u16() & 7);
}

const BehaviorScript omm_bhv_blargg_fire_ball[] = {
    OBJ_TYPE_SPECIAL,
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_blargg_fire_ball_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_blargg_fire_ball(struct Object *o) {
    struct Object *fireball = obj_spawn_from_geo(o, omm_geo_blargg_fire_ball, omm_bhv_blargg_fire_ball);
    f32 dx = 88.f * o->oScaleX * sins(o->oFaceAngleYaw);
    f32 dy = 96.f * o->oScaleY;
    f32 dz = 88.f * o->oScaleX * coss(o->oFaceAngleYaw);
    fireball->oPosX = o->oPosX + dx;
    fireball->oPosY = o->oPosY + dy;
    fireball->oPosZ = o->oPosZ + dz;
    fireball->oFaceAngleYaw = o->oFaceAngleYaw;
    fireball->oVelX = 24.f * sins(fireball->oFaceAngleYaw);
    fireball->oVelY = 24.f;
    fireball->oVelZ = 24.f * coss(fireball->oFaceAngleYaw);
    obj_scale(fireball, 5.f * o->oScaleX);
    return fireball;
}
