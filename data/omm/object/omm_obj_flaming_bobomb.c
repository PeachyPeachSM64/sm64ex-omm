#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Geo layout
//

const GeoLayout omm_geo_flaming_bobomb[] = {
    OMM_GEO_BRANCH(black_bobomb_geo)
};

//
// Behavior
//

static void omm_bhv_flaming_bobomb_update() {
    struct Object *o = gCurrentObject;
    switch (o->oAction) {

        // Grow
        case 0: {
            f32 t = (o->oTimer / 20.f) - (o->oFlamingBobombCount - o->oFlamingBobombIndex) * clamp_f(1.f / o->oFlamingBobombCount, 0.f, 0.2f);
            obj_scale(o, 1.2f * clamp_0_1_f(t));
        } break;

        // Fly to the sky
        case 1: {
            obj_scale(o, 1.2f);
            o->oPosY += 3.f * o->oTimer;
            if (o->oPosY >= o->oFlamingBobombMaxHeight) {
                o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                o->oFlamingBobombAura->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                o->oAction = 2;
                o->oTimer = 0;
            }
        } break;

        // Focus Mario and start falling
        case 2: {
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            o->oFlamingBobombAura->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            o->oPosX = gMarioObject->oPosX;
            o->oPosY = o->oFlamingBobombMaxHeight;
            o->oPosZ = gMarioObject->oPosZ;
            o->oAction = 3;
            o->oTimer = 0;
        } break;

        // Fall
        case 3: {
            o->oPosY -= 75.f;
            if (o->oTimer >= 60) {
                obj_mark_for_deletion(o->oFlamingBobombAura);
                obj_destroy(o);
                return;
            }
            if (o->oPosY <= find_floor_height(o->oPosX, o->oPosY + 100.f, o->oPosZ)) {
                o->oAction = 4;
                o->oTimer = 0;
            }
        } break;

        // Boom
        case 4: {
            omm_spawn_flaming_bobomb_explosion(o);
            omm_spawn_shockwave_fire(o, 100, 80, 120, 40, 5000, OMM_TEXTURE_BOWSER_FIRE_BLUE_1, OMM_TEXTURE_BOWSER_FIRE_BLUE_2);
            obj_mark_for_deletion(o->oFlamingBobombAura);
            obj_mark_for_deletion(o);
        } return;

        // Fly to the sky (faster)
        case 5: {
            obj_scale(o, 1.2f);
            o->oPosY += 4.f * o->oTimer;
            if (o->oPosY >= o->oFlamingBobombMaxHeight) {
                o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                o->oFlamingBobombAura->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                o->oAction = 6;
                o->oTimer = 0;
            }
        } break;

        // Focus a random point and start falling
        case 6: {
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            o->oFlamingBobombAura->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            for (;;) {
                f32 r = random_float() * o->oFlamingBobombMaxRadius;
                s16 a = random_u16();
                o->oPosX = r * sins(a);
                o->oPosY = o->oFlamingBobombMaxHeight;
                o->oPosZ = r * coss(a);
                struct Surface *floor = NULL;
                find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
                if (floor && floor->type != SURFACE_BURNING && floor->type != SURFACE_DEATH_PLANE) {
                    break;
                }
            }
            o->oAction = 3;
            o->oTimer = 0;
        } break;
    }

    // Update Gfx
    o->oFaceAnglePitch += o->oAngleVelPitch;
    o->oFaceAngleYaw += o->oAngleVelYaw;
    o->oFaceAngleRoll += o->oAngleVelRoll;
    Vec3f offset = { 0, 49.f * o->oScaleY, 0 };
    vec3f_rotate_zxy(offset, offset, o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
    vec3f_set(o->oGfxPos, o->oPosX - offset[0], o->oPosY - offset[1], o->oPosZ - offset[2]);
    vec3s_set(o->oGfxAngle, o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
    obj_set_shadow_pos_to_object_pos(o);
    obj_set_params(o, INTERACT_FLAME, 1, 99, 0, true);
    obj_reset_hitbox(o, 50, 100, 0, 0, 0, 50);
    obj_anim_play_with_sound(o, 0, 1.f, 0, 0);

    // Update aura
    vec3f_copy(&o->oFlamingBobombAura->oPosX, &o->oPosX);
    vec3f_copy(&o->oFlamingBobombAura->oScaleX, &o->oScaleX);
    vec3f_mul(&o->oFlamingBobombAura->oScaleX, 6.f);
    o->oFlamingBobombAura->oGraphYOffset = o->oScaleY * 20.f;
}

const BehaviorScript omm_bhv_flaming_bobomb[] = {
    OBJ_TYPE_GENACTOR,
    0x27260000, (uintptr_t) bobomb_seg8_anims_0802396C,
    0x08000000,
    0x0C000000, (uintptr_t) omm_bhv_flaming_bobomb_update,
    0x09000000,
};

//
// Spawner
//

struct Object *omm_spawn_flaming_bobomb(struct Object *o, f32 x, f32 y, f32 z, s32 index, s32 count, f32 maxRadius, f32 maxHeight) {
    struct Object *bobomb           = obj_spawn_from_geo(o, omm_geo_flaming_bobomb, omm_bhv_flaming_bobomb);
    bobomb->oFlamingBobombAura      = obj_spawn_from_geo(bobomb, omm_geo_flaming_bobomb_aura, omm_bhv_flaming_bobomb_aura);
    bobomb->oFlamingBobombIndex     = index;
    bobomb->oFlamingBobombCount     = count;
    bobomb->oFlamingBobombMaxRadius = maxRadius;
    bobomb->oFlamingBobombMaxHeight = maxHeight;
    bobomb->oPosX                   = x;
    bobomb->oPosY                   = y;
    bobomb->oPosZ                   = z;
    bobomb->oFaceAnglePitch         = (s16) (random_u16() & 0xFFFF);
    bobomb->oFaceAngleYaw           = (s16) (random_u16() & 0xFFFF);
    bobomb->oFaceAngleRoll          = (s16) (random_u16() & 0xFFFF);
    bobomb->oAngleVelPitch          = (s16) (random_u16() & 0x0FFF) - 0x800;
    bobomb->oAngleVelYaw            = (s16) (random_u16() & 0x0FFF) - 0x800;
    bobomb->oAngleVelRoll           = (s16) (random_u16() & 0x0FFF) - 0x800;
    bobomb->oScaleX                 = 0;
    bobomb->oScaleY                 = 0;
    bobomb->oScaleZ                 = 0;
    bobomb->oOpacity                = 255;
    bobomb->oAction                 = 0;
    return bobomb;
}
