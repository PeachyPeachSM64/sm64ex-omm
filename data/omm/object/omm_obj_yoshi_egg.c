#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#define YOSHI_EGG_LIFETIME          (o->oYoshiEggTarget ? 60 : 150)
#define YOSHI_EGG_POS_Y_NUM_FRAMES  (4)
#define YOSHI_EGG_MAX_TARGETS       (10)
#define YOSHI_EGG_TARGET_MAX_DIST   (20 * vec3f_length(&o->oVelX)) // 1200 ~ 2400

//
// Gfx data
//

static const Vtx omm_yoshi_egg_vtx[] = {
    {{{ -40, -40, 0 }, 0, {   0, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  40, -40, 0 }, 0, { 992, 992 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{  40,  40, 0 }, 0, { 992,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
    {{{ -40,  40, 0 }, 0, {   0,   0 }, { 0xFF, 0xFF, 0xFF, 0xFF }}},
};

static const Gfx omm_yoshi_egg_gfx[] = {
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(omm_yoshi_egg_vtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPEndDisplayList(),
};

static const Gfx omm_yoshi_egg_gfx_0[] = {
    gsDPLoadTextureBlock(OMM_ASSET_YOSHI_EGG_0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPBranchList(omm_yoshi_egg_gfx),
};

static const Gfx omm_yoshi_egg_gfx_1[] = {
    gsDPLoadTextureBlock(OMM_ASSET_YOSHI_EGG_1, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPBranchList(omm_yoshi_egg_gfx),
};

static const Gfx omm_yoshi_egg_gfx_2[] = {
    gsDPLoadTextureBlock(OMM_ASSET_YOSHI_EGG_2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPBranchList(omm_yoshi_egg_gfx),
};

static const Gfx omm_yoshi_egg_gfx_3[] = {
    gsDPLoadTextureBlock(OMM_ASSET_YOSHI_EGG_3, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPBranchList(omm_yoshi_egg_gfx),
};

static const Gfx omm_yoshi_egg_gfx_4[] = {
    gsDPLoadTextureBlock(OMM_ASSET_YOSHI_EGG_4, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPBranchList(omm_yoshi_egg_gfx),
};

static const Gfx omm_yoshi_egg_gfx_5[] = {
    gsDPLoadTextureBlock(OMM_ASSET_YOSHI_EGG_5, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPBranchList(omm_yoshi_egg_gfx),
};

static const Gfx omm_yoshi_egg_gfx_6[] = {
    gsDPLoadTextureBlock(OMM_ASSET_YOSHI_EGG_6, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPBranchList(omm_yoshi_egg_gfx),
};

static const Gfx omm_yoshi_egg_gfx_7[] = {
    gsDPLoadTextureBlock(OMM_ASSET_YOSHI_EGG_7, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPBranchList(omm_yoshi_egg_gfx),
};

//
// Geo layout
//

const GeoLayout omm_geo_yoshi_egg[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SWITCH_CASE(8, geo_switch_anim_state),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_egg_gfx_0),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_egg_gfx_1),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_egg_gfx_2),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_egg_gfx_3),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_egg_gfx_4),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_egg_gfx_5),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_egg_gfx_6),
            GEO_DISPLAY_LIST(LAYER_ALPHA, omm_yoshi_egg_gfx_7),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_yoshi_egg_get_target_pos(struct Object *o, struct Object *p, Vec3f dest) {
    s32 index = o->oSubAction;
    s32 eggs = obj_get_count_with_behavior_and_field_s32(bhvOmmYoshiEgg, _FIELD(oAction), 0);
    s16 angle = ((((index - 1) | 1) - eggs) * ((index & 1) ? 1 : -1)) * 0xC00;
    Vec3f pos = { 0, 0, 200 };
    Vec3s rot = { 0, p->oFaceAngleYaw + 0x8000 + angle, 0 };
    vec3f_transform(dest, pos, &p->oPosX, rot, p->oGfxScale);
}

static bool bhv_omm_yoshi_egg_should_target_nearest_object(struct Object *obj, u32 interactionFlags) {
    if (interactionFlags & OBJ_INT_ATTACK_WEAK) {
        if (omm_obj_is_weak(obj)) return true;
        if (omm_obj_is_bully(obj)) return true;
        if (omm_obj_is_kickable_board(obj)) return true;
    }
    if (interactionFlags & OBJ_INT_ATTACK_STRONG) {
        if (omm_obj_is_strong(obj)) return true;
        if (omm_obj_is_bully(obj)) return true;
        if (omm_obj_is_kickable_board(obj)) return true;
    }
    if (interactionFlags & OBJ_INT_ATTACK_DESTRUCTIBLE) {
        if (omm_obj_is_destructible(obj)) return true;
        if (omm_obj_is_wf_star_wall(obj)) return true;
    }
    if (interactionFlags & OBJ_INT_ATTACK_BREAKABLE) {
        if (omm_obj_is_breakable(obj)) return true;
    }
    return false;
}

static bool bhv_omm_yoshi_egg_is_obj_targetable(struct Object *obj, u32 interactionFlags) {

    // Koopa the Quick
    if (obj->behavior == bhvKoopa && obj->oKoopaMovementType >= KOOPA_BP_KOOPA_THE_QUICK_BASE) {
        return false;
    }

    // Exclamation box
    if (obj->behavior == bhvExclamationBox && obj->oAction != 2) {
        return false;
    }

    // Death actions
    if (obj->oAction >= 100) {
        return false;
    }

    // Object in Yoshi's mouth
    if (obj == omm_cappy_yoshi_get_object_in_mouth(gOmmCapture)) {
        return false;
    }

    // Check interaction
    if (!omm_obj_check_interaction(obj, gMarioState, obj_get_list_index(obj) != OBJ_LIST_SURFACE)) {
        return false;
    }

    // Check type
    if (!bhv_omm_yoshi_egg_should_target_nearest_object(obj, interactionFlags)) {
        return false;
    }

    // OK
    return true;
}

static bool bhv_omm_yoshi_egg_try_to_target_nearest_object(struct Object *o, struct Object *interacted, u32 interactionFlags) {
    if (o->oYoshiEggLastInteracted != interacted) {
        o->oYoshiEggLastInteracted = interacted;
        if (o->oNumLootCoins < YOSHI_EGG_MAX_TARGETS && bhv_omm_yoshi_egg_should_target_nearest_object(interacted, interactionFlags)) {
            o->oNumLootCoins++;
            struct Object *target = NULL;
            f32 distMin = YOSHI_EGG_TARGET_MAX_DIST;
            for_each_object_in_interaction_lists(obj) {
                if (obj != o && obj != interacted && bhv_omm_yoshi_egg_is_obj_targetable(obj, interactionFlags)) {
                    f32 dist = obj_get_distance(o, obj);
                    if (dist < distMin) {
                        distMin = dist;
                        target = obj;
                    }
                }
            }
            if (target) {
                o->oYoshiEggTarget = target;
                o->oTimer = 0;
                obj_spawn_particle_preset(o, PARTICLE_HORIZONTAL_STAR, false);
                return true;
            }
        }
        return false;
    }
    return true;
}

static void bhv_omm_yoshi_egg_delete(struct Object *o) {
    obj_spawn_loot_yellow_coins(o, o->oNumLootCoins, 10.f);
    obj_spawn_triangle_break_particles(o, OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_20);
    obj_create_sound_spawner(o, SOUND_GENERAL_BREAK_BOX);
    obj_mark_for_deletion(o);
}

static void bhv_omm_yoshi_egg_update() {
    struct Object *o = gCurrentObject;
    switch (o->oAction) {

        // Follow Yoshi
        case 0: {
            struct Object *p = o->parentObj;
            if (!p || p != gOmmCapture || omm_capture_get_type(p) != OMM_CAPTURE_YOSHI) {
                obj_spawn_white_puff(o, NO_SOUND);
                obj_mark_for_deletion(o);
                return;
            }

            // Get target pos
            Vec3f target;
            bhv_omm_yoshi_egg_get_target_pos(o, p, target);
            target[1] = max_f(target[1], find_floor_height(target[0], target[1] + 100.f, target[2]));
            Vec2f posToTarget = { target[0] - o->oPosX, target[2] - o->oPosZ };
            Vec2f yoshiDelta = { p->oPosX - p->oPrevPosX, p->oPosZ - p->oPrevPosZ };

            // Horizontal
            f32 distToTarget = vec2f_length(posToTarget);
            f32 yoshiVel = vec2f_length(yoshiDelta);
            o->oForwardVel = min_3_f(o->oForwardVel + 3.f, max_f(30.f, yoshiVel * 1.1f), distToTarget);
            vec2f_normalize(posToTarget);
            o->oPosX += o->oForwardVel * posToTarget[0];
            o->oPosZ += o->oForwardVel * posToTarget[1];

            // Vertical
            s32 frame = o->oTimer % YOSHI_EGG_POS_Y_NUM_FRAMES;
            o->oPosY = o->oYoshiEggPosY(frame);
            o->oPosY = max_f(o->oPosY, find_floor_height(o->oPosX, o->oPosY + 100.f, o->oPosZ));
            o->oYoshiEggPosY(frame) = target[1];

            // Gfx
            o->oAnimState++;
            o->oGraphYOffset = 60;
            obj_update_billboard(o);
            obj_set_angle_vec3s(o, gVec3sZero);
            obj_scale(o, 1.f);
        } break;

        // Projectile
        case 1: {

            // Update velocity
            struct Object *target = o->oYoshiEggTarget;
            if (target) {
                if (target->activeFlags) {
                    Vec3f posEggToTarget = {
                        target->oPosX - o->oPosX,
                        target->oPosY - o->oPosY + target->hitboxHeight / 2.f - target->hitboxDownOffset,
                        target->oPosZ - o->oPosZ,
                    };
                    f32 velocity = vec3f_length(&o->oVelX);
                    vec3f_set_mag(posEggToTarget, velocity);
                    vec3f_copy(&o->oVelX, posEggToTarget);
                } else {
                    bhv_omm_yoshi_egg_delete(o);
                }
            }

            // Step
            obj_reset_hitbox(o, 50, 100, 0, 0, 50, 0);
            s32 step = perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_STOP_IF_OUT_OF_BOUNDS);

            // Out of time/bounds
            if (o->oTimer > YOSHI_EGG_LIFETIME || check_object_step(o, step, OBJECT_STEP_CHECK_OUT_OF_BOUNDS)) {
                bhv_omm_yoshi_egg_delete(o);
                return;
            }

            // Collided with a wall (or a slanted floor)
            if (check_object_step(o, step, OBJECT_STEP_CHECK_HIT_WALL)) {
                obj_bounce_on_wall(o, o->oWall ? o->oWall : o->oFloor, false, false);
            }

            // Collided with a ceiling
            if (check_object_step(o, step, OBJECT_STEP_CHECK_HIT_CEILING) && o->oCeil && o->oCeil->normal.y > -0.9f) {
                obj_bounce_on_wall(o, o->oCeil, false, false);
            }

            // Update
            obj_scale(o, 1.f);
            obj_set_params(o, 0, 0, 0, o->oNumLootCoins, true);
            obj_reset_hitbox(o, 80, 160, 0, 0, 50, 30);
            u32 interactionFlags = obj_get_interaction_flags_from_power(o->oObjectPower, OBJ_INT_PRESET_YOSHI_EGG);
            struct Object *interacted = omm_obj_process_interactions(o, interactionFlags);
            if (interacted && !omm_obj_is_collectible(interacted) && !bhv_omm_yoshi_egg_try_to_target_nearest_object(o, interacted, interactionFlags)) {
                bhv_omm_yoshi_egg_delete(o);
                return;
            }

            // Gfx
            o->oAnimState = 0;
            o->oGraphYOffset = 50;
            o->oFaceAngleYaw = 0;
            obj_rotate_billboard(o, 0x2000);
            obj_scale(o, 1.25f);

            // Pfx
            struct Object *smoke = spawn_object(o, MODEL_SMOKE, bhvWhitePuff2);
            smoke->oFlags |= OBJ_FLAG_DONT_RENDER_ON_INIT;
            smoke->oPosY += 10;
            smoke->oTimer = 1;
            obj_scale(smoke, 1.25f);
        } break;
    }
}

const BehaviorScript bhvOmmYoshiEgg[] = {
    OBJ_TYPE_SPECIAL,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_yoshi_egg_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_yoshi_egg(struct Object *o, s32 index) {
    struct Object *egg = obj_spawn_from_geo(o, omm_geo_yoshi_egg, bhvOmmYoshiEgg);
    egg->parentObj = o;
    egg->oAction = 0;
    egg->oSubAction = index;
    egg->oGraphYOffset = 60;
    bhv_omm_yoshi_egg_get_target_pos(egg, o, &egg->oPosX);
    obj_set_angle_vec3s(egg, gVec3sZero);
    obj_scale(egg, 1.f);
    obj_update_gfx(egg);
    obj_set_interact_id(o, egg);
    egg->oYoshiEggTarget = NULL;
    egg->oYoshiEggLastInteracted = NULL;
    for (s32 i = 0; i != YOSHI_EGG_POS_Y_NUM_FRAMES; ++i) {
        egg->oYoshiEggPosY(i) = egg->oPosY;
    }
    return egg;
}
