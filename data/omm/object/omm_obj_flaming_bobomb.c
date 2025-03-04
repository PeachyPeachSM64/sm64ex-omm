#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

static void bhv_omm_flaming_bobomb_update() {
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
            omm_obj_spawn_flaming_bobomb_explosion(o);
            omm_obj_spawn_bowser_shockwave_fire(o, 100, 80, 120, 40, 5000, OMM_TEXTURE_BOWSER_FIRE_BLUE_1, OMM_TEXTURE_BOWSER_FIRE_BLUE_2);
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
                if (floor && !SURFACE_IS_LETHAL(floor->type)) {
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
    o->oFlags |= OBJ_FLAG_SHADOW_COPY_OBJ_POS;
    Vec3f offset = { 0, 49.f * o->oScaleY, 0 };
    vec3f_rotate_zxy(offset, offset, o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
    vec3f_set(o->oGfxPos, o->oPosX - offset[0], o->oPosY - offset[1], o->oPosZ - offset[2]);
    vec3s_set(o->oGfxAngle, o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
    obj_set_params(o, INTERACT_FLAME, 1, 99, 0, o->oAction == 1 || o->oAction == 3 || o->oAction == 5);
    obj_reset_hitbox(o, 50, 100, 0, 0, 0, 50);
    obj_anim_play_with_sound(o, 0, 1.f, 0, 0);

    // Update aura
    vec3f_copy(&o->oFlamingBobombAura->oPosX, &o->oPosX);
    vec3f_copy(&o->oFlamingBobombAura->oScaleX, &o->oScaleX);
    vec3f_mul(&o->oFlamingBobombAura->oScaleX, 6.f);
    o->oFlamingBobombAura->oGraphYOffset = o->oScaleY * 20.f;
}

const BehaviorScript bhvOmmFlamingBobomb[] = {
    OBJ_TYPE_GENACTOR,
    BHV_LOAD_ANIMATIONS(oAnimations, bobomb_seg8_anims_0802396C),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_flaming_bobomb_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_flaming_bobomb(struct Object *o, f32 x, f32 y, f32 z, s32 index, s32 count, f32 maxRadius, f32 maxHeight) {
    struct Object *bobomb           = obj_spawn_from_geo(o, black_bobomb_geo, bhvOmmFlamingBobomb);
    bobomb->oFlamingBobombAura      = obj_spawn_from_geo(bobomb, omm_geo_flaming_bobomb_aura, bhvOmmFlamingBobombAura);
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
