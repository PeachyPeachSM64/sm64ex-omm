#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static bool omm_cappy_pokey_can_run() {
    f32 fparts = (f32) gOmmObject->pokey.parts / (f32) OBJ_POKEY_BALLS_MAX;
    return 0.5f <= fparts && fparts <= 0.8f;
}

static bool omm_cappy_pokey_can_dash() {
    f32 fparts = (f32) gOmmObject->pokey.parts / (f32) OBJ_POKEY_BALLS_MAX;
    return fparts < 0.5f;
}

static f32 omm_cappy_pokey_get_jump_mult() {
    f32 fparts = (f32) gOmmObject->pokey.parts / (f32) OBJ_POKEY_BALLS_MAX;
    return lerp_f(fparts, 1.5f, 1.f);
}

static void omm_cappy_pokey_update_pokey_parts_gfx(struct Object *o) {
    gOmmObject->pokey.top = 0.f;
    for (s32 i = OBJ_POKEY_BALLS_MAX - 1; i >= 0; --i) {
        struct Object *body = gOmmObject->pokey.body[i];
        s16 offsetAngle = i * 0x4000 + o->oTimer * 0x800;
        body->oPosX = o->oPosX + coss(offsetAngle) * 6.f;
        body->oPosZ = o->oPosZ + sins(offsetAngle) * 6.f;
        body->oPosY = o->oPosY + gOmmObject->pokey.top;
        obj_set_angle(body, 0, o->oFaceAngleYaw, 0);
        obj_scale(body, i < gOmmObject->pokey.parts ? min_f(body->oScaleY + 0.3f, 3.f) : 0.f);
        body->oGraphYOffset = body->oScaleY * 22.f;
        obj_update_gfx(body);
        obj_update_blink_state(body, &body->oPokeyBodyPartBlinkTimer, 30, 60, 4);
        body->oGraphNode = geo_layout_to_graph_node(NULL, i == 0 ? pokey_head_geo : pokey_body_part_geo);
        obj_update_billboard(body);
        obj_copy_visibility_and_transparency(body, o);
        gOmmObject->pokey.top += omm_capture_get_hitbox_height(o) * body->oScaleY / 3.f;
    }
}

//
// Init
//

bool omm_cappy_pokey_init(struct Object *o) {
    struct Object *pokey = (o->behavior == bhvPokey ? o : o->parentObj);

    // Check Pokey state
    if (pokey->oAction != POKEY_ACT_WANDER ||
        pokey->oPokeyNumAliveBodyParts == 0 ||
        pokey->oPokeyHeadWasKilled) {
        return false;
    }

    // If it's a body part, capture Pokey
    if (o != pokey) {
        omm_mario_possess_object(gMarioState, pokey, 0);
        o->oInteractStatus = INT_STATUS_INTERACTED;
        o->oIntangibleTimer = -1;
        return false;
    }

    // Gather Pokey parts
    gOmmObject->pokey.parts = 0;
    mem_zero(gOmmObject->pokey.body, sizeof(gOmmObject->pokey.body));
    for_each_object_with_behavior(obj, bhvPokeyBodyPart) {
        if (obj->parentObj == o) {
            if (gOmmObject->pokey.parts < OBJ_POKEY_BALLS_MAX) {
                gOmmObject->pokey.body[gOmmObject->pokey.parts++] = obj;
                obj->curBhvCommand = bhvOmmDummy;
                obj->bhvStackIndex = 0;
                obj->oInteractStatus = INT_STATUS_INTERACTED;
                obj->oIntangibleTimer = -1;
            } else {
                obj_mark_for_deletion(obj);
            }
        }
    }

    // Create additional parts if needed
    for (s32 i = gOmmObject->pokey.parts; i != OBJ_POKEY_BALLS_MAX; ++i) {
        struct Object *body = spawn_object(o, MODEL_NONE, bhvOmmDummy);
        body->oNodeFlags = gOmmObject->pokey.body[0]->oNodeFlags;
        body->oInteractStatus = INT_STATUS_INTERACTED;
        body->oIntangibleTimer = -1;
        gOmmObject->pokey.body[i] = body;
    }

    // Init gfx
    omm_cappy_pokey_update_pokey_parts_gfx(o);
    return true;
}

void omm_cappy_pokey_end(struct Object *o) {

    // Destroy all dummy balls
    for (s32 i = 0; i != OBJ_POKEY_BALLS_MAX; ++i) {
        unload_object(gOmmObject->pokey.body[i]);
    }

    // Recreate the pokey
    s32 numParts = gOmmObject->pokey.parts;
    for (s32 i = 0; i != numParts; ++i) {
        struct Object *body = spawn_object_relative(i, 0, 120 * (numParts - 1 - i), 0, o, i == 0 ? MODEL_POKEY_HEAD : MODEL_POKEY_BODY_PART, bhvPokeyBodyPart);
        if (body) obj_scale(body, 3.f);
    }

    // Reinitialize it
    o->oPokeyAliveBodyPartFlags = (1 << numParts) - 1;
    o->oPokeyNumAliveBodyParts = numParts;
    o->oPokeyBottomBodyPartSize = 1.f;
    o->oPokeyHeadWasKilled = 0;
    o->oPokeyTargetYaw = o->oFaceAngleYaw;
    o->oPokeyChangeTargetTimer = 0;
    o->oPokeyTurningAwayFromWall = 0;
    o->oAction = POKEY_ACT_WANDER;
}

u64 omm_cappy_pokey_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_POKEY;
}

f32 omm_cappy_pokey_get_top(UNUSED struct Object *o) {
    return gOmmObject->pokey.top;
}

//
// Update
//

s32 omm_cappy_pokey_update(struct Object *o) {

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_cappy_pokey_get_top(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_IMMUNE_TO_QUICKSAND;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_OPEN_DOORS * (gOmmObject->pokey.parts <= 2);

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, omm_cappy_pokey_can_run(), omm_cappy_pokey_can_dash(), false);
        if (pobj_jump(o, 1) == POBJ_RESULT_JUMP_START) {
            obj_play_sound(o, POBJ_SOUND_JUMP_1);
            o->oVelY *= omm_cappy_pokey_get_jump_mult();
        }

        // Deez
        if (POBJ_B_BUTTON_PRESSED && gOmmObject->pokey.parts > 1) {
            omm_obj_spawn_pokey_ball(o);
            obj_spawn_white_puff(o, POBJ_SOUND_SHOOT);
            o->oPosY += omm_capture_get_hitbox_height(o);
            o->oVelY = max_f(o->oVelY, pobj_get_jump_velocity(o) * omm_cappy_pokey_get_jump_mult() / 2.f);
            gOmmObject->state.actionTimer = 30;
            gOmmObject->pokey.parts--;
        }
    }

    // Regenerate balls
    if (!POBJ_IS_OPENING_DOORS) {
        if (gOmmObject->state.actionTimer > 0) {
            gOmmObject->state.actionTimer--;
        } else if (gOmmObject->pokey.parts < OBJ_POKEY_BALLS_MAX) {
            obj_scale(gOmmObject->pokey.body[gOmmObject->pokey.parts], 0.f);
            gOmmObject->state.actionTimer = 30;
            gOmmObject->pokey.parts++;
        }
    }

    // Movement
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();

    // Interactions
    pobj_process_interactions();
    pobj_stop_if_unpossessed();

    // OK
    pobj_return_ok;
}

void omm_cappy_pokey_update_gfx(struct Object *o) {

    // Gfx
    obj_update_gfx(o);

    // Cappy transform
    struct Object *head = gOmmObject->pokey.body[0];
    bool modelPack = omm_models_get_model_pack_index(head->oGraphNode->georef) != -1;
    gOmmObject->cappy.tra_y = modelPack ? 42.f : 40.f;
    gOmmObject->cappy.tra_z = modelPack ? 4.f : 0.f;
    gOmmObject->cappy.scale = 0.5f;
    gOmmObject->cappy.object = head;
    gOmmObject->cappy.post_update = omm_cappy_pokey_update_pokey_parts_gfx;
}
