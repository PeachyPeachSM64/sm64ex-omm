#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Behavior
//

static void omm_bhv_perry_update() {
    struct MarioState *m = gMarioState;
    struct Object *o = gCurrentObject;
    obj_set_params(o, 0, 0, 0, 0, 0);
    obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    o->oPerryType = omm_peach_get_perry_type(m);

    // Hitbox and interactions
    if (o->oPerryFlags & ~(OBJ_INT_PERRY_SWORD | OBJ_INT_PERRY_TRAIL)) {

        // Update Perry's hitbox
        Vec3f p = { o->oPosX, o->oPosY, o->oPosZ };
        Vec3f d = { 0, OMM_PERRY_SWORD_HITBOX, 0 };
        vec3f_rotate_zxy(d, d, o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
        obj_set_pos(o, o->oPosX + d[0] * o->oScaleX / 2.f, o->oPosY + d[1] * o->oScaleY / 2.f, o->oPosZ + d[2] * o->oScaleZ / 2.f);
        obj_set_params(o, 0, 0, 0, 0, true);
        obj_reset_hitbox(o, OMM_PERRY_SWORD_HITBOX / 2.f, OMM_PERRY_SWORD_HITBOX, 0, 0, 0, OMM_PERRY_SWORD_HITBOX / 2.f);

        // Process interactions
        struct Object *interacted = omm_obj_process_interactions(o, o->oPerryFlags);
        if (interacted && !omm_obj_is_collectible(interacted)) {
            play_sound(SOUND_ACTION_BOUNCE_OFF_OBJECT, interacted->oCameraToObject);
        }

        // Reset values
        obj_set_pos(o, p[0], p[1], p[2]);
        obj_set_params(o, 0, 0, 0, 0, false);
        obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);
    }

    // Preprocess
    geo_preprocess_object_graph_node(o);
}

const BehaviorScript omm_bhv_perry[] = {
    OBJ_TYPE_SPECIAL, // This object must be updated before omm_bhv_perry_trail
    0x11010001,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_perry_update,
    0x09000000,
};

//
// Spawner (auto)
//

OMM_ROUTINE_UPDATE(omm_spawn_perry) {
    if (gMarioObject) {
        if (OMM_PLAYER_IS_PEACH) {
            struct Object *perry = omm_peach_get_perry_object();
            if (!perry) {
                gOmmPerry = obj_spawn_from_geo(gMarioObject, omm_geo_perry, omm_bhv_perry);
            }
        } else {
            obj_deactivate_all_with_behavior(omm_bhv_perry);
            obj_deactivate_all_with_behavior(omm_bhv_perry_trail);
            obj_deactivate_all_with_behavior(omm_bhv_perry_shockwave);
            obj_deactivate_all_with_behavior(omm_bhv_perry_blast);
            obj_deactivate_all_with_behavior(omm_bhv_perry_charge);
        }
    }
}
