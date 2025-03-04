#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layouts
//

const GeoLayout omm_geo_blargg_fireball[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(red_flame_geo),
};

//
// Behavior (fire drop)
//

static void bhv_omm_blargg_fire_drop_update() {
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

const BehaviorScript bhvOmmBlarggFireDrop[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_blargg_fire_drop_update),
    BHV_END_LOOP(),
};

//
// Behavior (fire trail)
//

static void bhv_omm_blargg_fire_trail_update() {
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

const BehaviorScript bhvOmmBlarggFireTrail[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_blargg_fire_trail_update),
    BHV_END_LOOP(),
};

//
// Behavior (fire ball)
//

static void bhv_omm_blargg_fireball_delete(struct Object *o) {
    for (s32 i = 0; i != 10; ++i) {
        struct Object *drop = obj_spawn_from_geo(o, omm_geo_fire_smoke_red, bhvOmmBlarggFireDrop);
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

static void bhv_omm_blargg_fireball_update() {
    struct Object *o = gCurrentObject;
    perform_object_step(o, OBJ_STEP_UPDATE_HOME);
    o->oVelY -= 2.f;

    // Out of bounds, or collided with a floor
    if (!o->oFloor || o->oDistToFloor <= 5.f) {
        bhv_omm_blargg_fireball_delete(o);
        return;
    }

    // Update
    obj_update_gfx(o);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 16, 24, 0, 0, 8, 12);
    omm_obj_process_interactions(o, OBJ_INT_PRESET_BLARGG_FIRE);
    o->oAnimState++;

    // Spawn fire trail
    struct Object *trail = obj_spawn_from_geo(o, omm_geo_fire_smoke_red, bhvOmmBlarggFireTrail);
    obj_scale(trail, o->oScaleX);
    trail->oAction = (s32) (100.f * trail->oScaleX);
    trail->oAnimState = (random_u16() & 7);
}

const BehaviorScript bhvOmmBlarggFireball[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_blargg_fireball_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_blargg_fireball(struct Object *o) {
    struct Object *fireball = obj_spawn_from_geo(o, omm_geo_blargg_fireball, bhvOmmBlarggFireball);
    f32 dx = 88.f * o->oScaleX * sins(o->oFaceAngleYaw);
    f32 dy = 96.f * o->oScaleY;
    f32 dz = 88.f * o->oScaleX * coss(o->oFaceAngleYaw);
    obj_set_xyz(fireball, o->oPosX + dx, o->oPosY + dy, o->oPosZ + dz);
    obj_set_angle(fireball, 0, o->oFaceAngleYaw, 0);
    obj_set_throw_vel(fireball, o, 32.f, 40.f);
    obj_scale(fireball, 5.f * o->oScaleX);
    obj_play_sound(o, SOUND_OBJ_FLAME_BLOWN);
    obj_set_interact_id(o, fireball);
    return fireball;
}
