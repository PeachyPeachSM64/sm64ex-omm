#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_rock[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_ALPHA, pebble_seg3_dl_0301CB00),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END()
};

//
// Behavior
//

static void omm_bhv_rock_delete(struct Object *o) {
    obj_spawn_white_puff(o, -1);
    obj_mark_for_deletion(o);
}

static void omm_bhv_rock_update() {
    struct Object *o = gCurrentObject;
    perform_object_step(o, OBJ_STEP_UPDATE_HOME);
    o->oVelY -= 2.f;

    // Collided with a wall/floor
    if (o->oWall || o->oCeil || o->oDistToFloor <= 0.f) {
        omm_bhv_rock_delete(o);
        return;
    }

    // Update
    obj_scale(o, 1.5f);
    obj_update_gfx(o);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 30, 50, 0, 0, 15, 25);
    struct Object *interacted = omm_obj_process_interactions(o, OBJ_INT_PRESET_ROCK);
    if (interacted && !omm_obj_is_collectible(interacted)) {
        omm_bhv_rock_delete(o);
    }
}

const BehaviorScript omm_bhv_rock[] = {
    OBJ_TYPE_SPECIAL,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_rock_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_rock(struct Object *o) {
    struct Object *rock = obj_spawn_from_geo(o, omm_geo_rock, omm_bhv_rock);
    s16 da = o->oFaceAngleYaw + 0x4000;
    f32 dx = -40.f * o->oScaleX * sins(da);
    f32 dy = +25.f * o->oScaleY;
    f32 dz = -40.f * o->oScaleX * coss(da);
    rock->oPosX = o->oPosX + dx;
    rock->oPosY = o->oPosY + dy;
    rock->oPosZ = o->oPosZ + dz;
    rock->oFaceAngleYaw = o->oFaceAngleYaw;
    rock->oVelX = 42.f * sins(rock->oFaceAngleYaw);
    rock->oVelY = 24.f;
    rock->oVelZ = 42.f * coss(rock->oFaceAngleYaw);
    return rock;
}
