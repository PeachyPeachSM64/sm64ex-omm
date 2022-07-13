#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Init
//

bool cappy_swoop_init(struct Object *o) {
    obj_scale(o, 1.f);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    gOmmData->object->swoop.ceilingType = 0;
    return true;
}

void cappy_swoop_end(struct Object *o) {
    obj_scale(o, 1.f);
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
}

f32 cappy_swoop_get_top(struct Object *o) {
    return 50.f * o->oScaleY;
}

//
// Update
//

s32 cappy_swoop_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->hitboxDownOffset = omm_capture_get_hitbox_down_offset(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;

    // Ceiling check
    if (!POBJ_A_BUTTON_DOWN || omm_mario_is_locked(gMarioState) ||
        find_ceil(o->oPosX, o->oPosY, o->oPosZ, NULL) - o->oPosY > 1.5f * (o->hitboxHeight - o->hitboxDownOffset) ||
        (gOmmData->object->swoop.ceilingType == 2 && gOmmData->object->swoop.ceiling.object.o->activeFlags == ACTIVE_FLAG_DEACTIVATED)) {
        gOmmData->object->swoop.ceilingType = 0;
    }

    // States
    if (gOmmData->object->swoop.ceilingType != 0) {

        // Movement
        o->oVelX = 0.f;
        o->oVelY = 0.f;
        o->oVelZ = 0.f;
        switch (gOmmData->object->swoop.ceilingType) {

            // Set y pos to ceiling height
            case 1: {
                o->oPosY = gOmmData->object->swoop.ceiling.surface.height;
                o->oFloorHeight = o->oPosY;
            } break;

            // Apply platform displacement
            case 2: {

                // Get displacement
                struct Object *ceilObj = gOmmData->object->swoop.ceiling.object.o;
                Vec3f ceilToObj = {
                    o->oPosX - gOmmData->object->swoop.ceiling.object.pos[0],
                    o->oPosY - gOmmData->object->swoop.ceiling.object.pos[1],
                    o->oPosZ - gOmmData->object->swoop.ceiling.object.pos[2],
                };
                Vec3f translation = {
                    ceilObj->oPosX - gOmmData->object->swoop.ceiling.object.pos[0],
                    ceilObj->oPosY - gOmmData->object->swoop.ceiling.object.pos[1],
                    ceilObj->oPosZ - gOmmData->object->swoop.ceiling.object.pos[2],
                };
                Vec3s rotation = {
                    (s16) ceilObj->oFaceAnglePitch - (s16) *((s32 *) &gOmmData->object->swoop.ceiling.object.angle[0]),
                    (s16) ceilObj->oFaceAngleYaw   - (s16) *((s32 *) &gOmmData->object->swoop.ceiling.object.angle[1]),
                    (s16) ceilObj->oFaceAngleRoll  - (s16) *((s32 *) &gOmmData->object->swoop.ceiling.object.angle[2]),
                };
                Vec3f scale = {
                    ceilObj->oScaleX / max_f(0.01f, gOmmData->object->swoop.ceiling.object.scale[0]),
                    ceilObj->oScaleY / max_f(0.01f, gOmmData->object->swoop.ceiling.object.scale[1]),
                    ceilObj->oScaleZ / max_f(0.01f, gOmmData->object->swoop.ceiling.object.scale[2]),
                };
                
                // Transform
                vec3f_transform(ceilToObj, ceilToObj, translation, rotation, scale);
                o->oPosX = gOmmData->object->swoop.ceiling.object.pos[0] + ceilToObj[0];
                o->oPosY = gOmmData->object->swoop.ceiling.object.pos[1] + ceilToObj[1];
                o->oPosZ = gOmmData->object->swoop.ceiling.object.pos[2] + ceilToObj[2];
                o->oFaceAngleYaw += rotation[1];
                o->oFloorHeight = o->oPosY;

                // Update values
                vec3f_copy(gOmmData->object->swoop.ceiling.object.pos,   (f32 *) &ceilObj->oPosX);
                vec3f_copy(gOmmData->object->swoop.ceiling.object.angle, (f32 *) &ceilObj->oFaceAnglePitch);
                vec3f_copy(gOmmData->object->swoop.ceiling.object.scale, (f32 *) &ceilObj->oScaleX);
            } break;
        }
    } else {

        // Inputs
        if (!omm_mario_is_locked(gMarioState)) {
            pobj_move(o, POBJ_B_BUTTON_DOWN, false, false);
            if (pobj_jump(o, 0, 6) == POBJ_RESULT_JUMP_START) {
                o->oCurrAnim = NULL;
                obj_play_sound(o, SOUND_OBJ2_SWOOP);
            }
            if (POBJ_B_BUTTON_DOWN && obj_anim_is_near_end(o)) {
                obj_play_sound(o, SOUND_OBJ_UNKNOWN6);
            }
        }

        // Movement
        perform_object_step(o, POBJ_STEP_FLAGS);
        pobj_decelerate(o, 0.80f, 0.80f);
        pobj_apply_gravity(o, 1.f);
        pobj_handle_special_floors(o);
        POBJ_STOP_IF_UNPOSSESSED;

        // Batman
        if (POBJ_A_BUTTON_DOWN && o->oCeil && o->oCeil->normal.y < -0.9f) {
            if (o->oCeil->object) {
                gOmmData->object->swoop.ceilingType = 2;
                gOmmData->object->swoop.ceiling.object.o = o->oCeil->object;
                vec3f_copy(gOmmData->object->swoop.ceiling.object.pos,   (f32 *) &o->oCeil->object->oPosX);
                vec3f_copy(gOmmData->object->swoop.ceiling.object.angle, (f32 *) &o->oCeil->object->oFaceAnglePitch);
                vec3f_copy(gOmmData->object->swoop.ceiling.object.scale, (f32 *) &o->oCeil->object->oScaleX);
            } else {
                gOmmData->object->swoop.ceilingType = 1;
                gOmmData->object->swoop.ceiling.surface.s = o->oCeil;
                gOmmData->object->swoop.ceiling.surface.height = o->oPosY;
            }
        }
    }
    
    // Interactions
    POBJ_INTERACTIONS();
    POBJ_STOP_IF_UNPOSSESSED;

    // Gfx
    obj_update_gfx(o);
    if (gOmmData->object->swoop.ceilingType != 0) {
        obj_anim_play(o, 1, 1.f);
        o->oGfxPos[1] += o->oScaleY * 60.f;
        o->oGfxAngle[2] = 0x8000;

        // Cappy values
        gOmmData->object->cappy.offset[1] = -50.f + o->oScaleY * 60.f;
        gOmmData->object->cappy.offset[2] = 6.f;
        gOmmData->object->cappy.angle[2]  = 0x8000;
        gOmmData->object->cappy.scale     = 0.5f;
    } else {
        obj_anim_play(o, 0, (o->oVelY > 0.f || POBJ_B_BUTTON_DOWN) ? 2.f : 1.f);
        
        // Cappy values
        gOmmData->object->cappy.offset[1] = 50.f;
        gOmmData->object->cappy.offset[2] = 12.f;
        gOmmData->object->cappy.scale     = 0.5f;
    }

    // OK
    POBJ_RETURN_OK;
}
