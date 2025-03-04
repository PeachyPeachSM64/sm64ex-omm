#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "data/omm/omm_constants.h"

//
// Mario object and Cappy gfx
//

typedef struct { const void *georef; s32 animParts; Vec3f translation; Vec3s rotation; f32 scale; } OmmCappyGfxParams;
static const OmmCappyGfxParams OMM_CAPPY_GFX_PARAMS[][40] = {

/* Super Mario 64 */
[OMM_GAME_SM64] = {
{ goomba_geo,               3, {  140,    0,   0 }, { 0xC000, 0x4000, 0x8000 },  3.00f },
{ koopa_with_shell_geo,     3, {  100,  -60,   0 }, { 0x0800, 0x4000, 0x8000 },  2.50f },
{ koopa_without_shell_geo,  3, {  100,  -60,   0 }, { 0x0800, 0x4000, 0x8000 },  2.50f },
{ black_bobomb_geo,        12, {    0,  -90,   0 }, { 0x8000, 0xC000, 0x0000 },  3.00f },
{ bobomb_buddy_geo,        12, {    0,  -90,   0 }, { 0x8000, 0xC000, 0x0000 },  3.00f },
{ hoot_geo,                 3, {  110,   15,   5 }, { 0xC000, 0xC000, 0x0000 },  2.50f },
{ whomp_geo,                2, { 1550,    0,  10 }, { 0x8000, 0x8000, 0x4000 },  8.00f },
{ spindrift_geo,           14, {  -75,  -15,   0 }, { 0x4800, 0xC000, 0x0000 },  2.50f },
{ penguin_geo,             11, {   90,   30,   0 }, { 0xC000, 0xC000, 0x0000 },  2.00f },
{ mr_blizzard_geo,          4, {  170,    0,   0 }, { 0xC000, 0xC000, 0x0000 },  4.00f },
{ mr_blizzard_hidden_geo,   4, {  170,    0,   0 }, { 0xC000, 0xC000, 0x0000 },  4.00f },
{ scuttlebug_geo,          17, {  100,    0,   0 }, { 0x0000, 0x0000, 0xC000 },  4.00f },
{ mad_piano_geo,            4, {  500,   10,   0 }, { 0xFC00, 0x4000, 0x0000 },  5.00f },
{ swoop_geo,                3, {   80,   20,   0 }, { 0xC000, 0x0000, 0xC000 },  2.00f },
{ monty_mole_geo,           3, {   85,   10,   0 }, { 0xC000, 0x0000, 0xC000 },  2.50f },
{ dorrie_geo,               5, {  770,  200,   0 }, { 0xC000, 0x0000, 0xC000 }, 10.00f },
{ bully_geo,               11, {  120,   10,   0 }, { 0xC000, 0x0000, 0xC000 },  2.50f },
{ bully_boss_geo,          11, {  120,   10,   0 }, { 0xC000, 0x0000, 0xC000 },  2.50f },
{ chilly_chief_geo,        11, {  120,   10,   0 }, { 0xC000, 0x0000, 0xC000 },  2.50f },
{ chilly_chief_big_geo,    11, {  120,   10,   0 }, { 0xC000, 0x0000, 0xC000 },  2.50f },
{ flyguy_geo,               2, {  216,   30,   0 }, { 0xC000, 0xC000, 0x0000 },  2.50f },
{ bub_geo,                  2, {  -50,    0,  70 }, { 0x0000, 0xCC00, 0xC000 },  2.50f },
{ sushi_geo,                6, { -400,    0, 140 }, { 0x0000, 0xC800, 0xC000 },  6.00f },
{ moneybag_geo,             2, {  230, -150,   0 }, { 0x7800, 0xC000, 0x0000 },  5.00f },
{ skeeter_geo,              2, {    0,  130,   0 }, { 0x0000, 0xC000, 0x0000 },  4.50f },
{ heave_ho_geo,             4, {   40,   85,   0 }, { 0xF400, 0xC000, 0x0000 },  2.50f },
{ chuckya_geo,             12, {  160,  -80,   0 }, { 0xAC00, 0xC000, 0x0000 },  4.00f },
{ ukiki_geo,                3, {  150,   20,   0 }, { 0xC000, 0x0000, 0xC000 },  3.00f },
{ lakitu_geo,               3, {   50, -100,   0 }, { 0x7C00, 0xC000, 0x0000 },  3.00f },
{ enemy_lakitu_geo,         3, {   50, -100,   0 }, { 0x7C00, 0xC000, 0x0000 },  3.00f },
{ spiny_geo,                3, {  140,    0,   0 }, { 0xC000, 0xC000, 0x0000 },  2.50f },
{ toad_geo,                 4, {    0,  130,   0 }, { 0xF800, 0x0000, 0x0000 },  3.00f },
{ mips_geo,                 3, {   65,  -20,   0 }, { 0xA400, 0xC000, 0x0000 },  2.00f },
{ yoshi_geo,                4, {  300,    0,   0 }, { 0xA000, 0xC000, 0x0000 },  3.50f },
},

/* Render96 */
[OMM_GAME_R96X] = {
#if OMM_GAME_IS_R96X
{ motos_geo,               17, {    0,   25,   0 }, { 0x0000, 0x0000, 0x0000 },  2.00f },
{ blargg_geo,               3, {  -40, -275,  10 }, { 0x7400, 0xC000, 0x0000 },  5.00f },
{ friendly_blargg_geo,      3, {  -40, -275,  10 }, { 0x7400, 0xC000, 0x0000 },  5.00f },
#endif
},

/* Super Mario 64 Moonshine */
[OMM_GAME_SMMS] = {
#if OMM_GAME_IS_SMMS
{ black_bobomb_geo,        12, {  -40,  -80,   0 }, { 0x8000, 0xC000, 0x0000 },  3.00f },
{ bobomb_buddy_geo,        12, {  -40,  -80,   0 }, { 0x8000, 0xC000, 0x0000 },  3.00f },
{ spindrift_geo,           14, {  -45,  -25,   0 }, { 0x4800, 0xC000, 0x0000 },  2.50f },
{ penguin_geo,             11, {  110,   30,   0 }, { 0xC000, 0xC000, 0x0000 },  2.00f },
{ mr_blizzard_geo,          4, {   25,    0,  70 }, { 0xC000, 0xC000, 0xF000 },  4.00f },
{ mr_blizzard_hidden_geo,   4, {   25,    0,  70 }, { 0xC000, 0xC000, 0xF000 },  4.00f },
{ scuttlebug_geo,          17, {  130,    0,   0 }, { 0x0000, 0x0000, 0xC000 },  4.00f },
{ bully_geo,               11, {  185,  -20,   0 }, { 0xC000, 0x0000, 0xC000 },  3.00f },
{ bully_boss_geo,          11, {  185,  -20,   0 }, { 0xC000, 0x0000, 0xC000 },  3.00f },
{ chilly_chief_geo,        11, {  185,  -20,   0 }, { 0xC000, 0x0000, 0xC000 },  3.00f },
{ chilly_chief_big_geo,    11, {  185,  -20,   0 }, { 0xC000, 0x0000, 0xC000 },  3.00f },
{ flyguy_geo,               2, {  200,   15,   6 }, { 0xC000, 0xC000, 0x0000 },  2.50f },
{ bub_geo,                  2, {    0,   -5,  40 }, { 0x0000, 0xC000, 0xC000 },  3.50f },
{ chuckya_geo,             12, {  130,  -80,   0 }, { 0xAC00, 0xC000, 0x0000 },  4.00f },
{ toad_geo,                 4, {    0,   95,   0 }, { 0xF800, 0x0000, 0x0000 },  3.00f },
{ goombone_geo,             3, {  160,    0,   0 }, { 0xBC00, 0x4000, 0x8000 },  3.00f },
{ Bee_geo,                  3, {  190,    0,   0 }, { 0xC400, 0xC000, 0x0000 },  4.50f },
#endif
},

/* Super Mario 74 */
[OMM_GAME_SM74] = {
#if OMM_GAME_IS_SM74
#endif
},

/* Super Mario Star Road */
[OMM_GAME_SMSR] = {
#if OMM_GAME_IS_SMSR
{ peach_geo,                4, {  145,   10,   0 }, { 0xBC00, 0xC000, 0x0000 },  3.00f },
{ custom_shyguy_geo,        2, {  216,   30,   0 }, { 0xC000, 0xC000, 0x0000 },  2.50f },
#endif
},

/* Super Mario 64: The Green Stars */
[OMM_GAME_SMGS] = {
#if OMM_GAME_IS_SMGS
#endif
},
};

/* Render96 Model Pack */
static const OmmCappyGfxParams OMM_CAPPY_GFX_PARAMS_MODEL_PACK[40] = {
{ koopa_with_shell_geo,     3, {   60,  -50,   0 }, { 0x0800, 0x4000, 0x8000 },  2.50f },
{ koopa_without_shell_geo,  3, {   60,  -50,   0 }, { 0x0800, 0x4000, 0x8000 },  2.50f },
{ spindrift_geo,           14, {  -52,    5,   0 }, { 0x4800, 0xC000, 0x0000 },  2.00f },
{ monty_mole_geo,           2, {  170,    5,   0 }, { 0xC000, 0x0000, 0xC000 },  2.00f },
{ dorrie_geo,               5, {  725,  200,   0 }, { 0xC000, 0x0000, 0xC000 }, 10.00f },
{ flyguy_geo,               2, {  200,   10,   0 }, { 0xBC00, 0xC000, 0x0000 },  2.50f },
{ sushi_geo,                6, { -400,    0, 300 }, { 0x0000, 0xC000, 0xC000 },  7.00f },
{ moneybag_geo,             2, {  160, -160,   0 }, { 0x7000, 0xC000, 0x0000 },  5.00f },
{ heave_ho_geo,             4, {  140,  160,   0 }, { 0xFC00, 0xC000, 0x0000 },  2.50f },
{ lakitu_geo,               3, {   60, -100,   0 }, { 0x7000, 0xC000, 0x0000 },  2.50f },
{ enemy_lakitu_geo,         3, {   60, -100,   0 }, { 0x7000, 0xC000, 0x0000 },  2.50f },
{ mips_geo,                 3, {   65,  -20,   5 }, { 0xA400, 0xC000, 0x0000 },  2.00f },
#if OMM_GAME_IS_R96X
{ blargg_geo,               3, {   30, -200,  25 }, { 0x6000, 0xC000, 0xF800 },  4.50f },
{ friendly_blargg_geo,      3, {   30, -200,  25 }, { 0x6000, 0xC000, 0xF800 },  4.50f },
#endif
};

static const OmmCappyGfxParams *omm_cappy_get_gfx_params_from_game(const OmmCappyGfxParams *params, const void *georef) {
    for (; params->georef; ++params) {
        if (params->georef == georef) {
            return params;
        }
    }
    return NULL;
}

static const OmmCappyGfxParams *omm_cappy_get_gfx_params(struct Object *o) {
    const OmmCappyGfxParams *params = NULL;
    if (o && o->oGraphNode) {
        const void *georef = o->oGraphNode->georef;

        // Check model pack
        if (omm_models_get_model_pack_index(georef) != -1) {
            params = omm_cappy_get_gfx_params_from_game(OMM_CAPPY_GFX_PARAMS_MODEL_PACK, georef);
        }

        // Check game
        if (!params) {
            params = omm_cappy_get_gfx_params_from_game(OMM_CAPPY_GFX_PARAMS[OMM_GAME_SAVE], georef);
        }

        // Check default
        if (!params) {
            params = omm_cappy_get_gfx_params_from_game(OMM_CAPPY_GFX_PARAMS[OMM_GAME_SM64], georef);
        }
    }
    return params;
}

void omm_mario_capture_update_cap_object(struct Object *o, struct Object *obj, struct Object *cap) {
    const OmmCappyGfxParams *params = omm_cappy_get_gfx_params(obj);
#if OMM_CODE_DEV
#define OMM_DEV_EXTERN_0
#include "data/omm/dev/omm_dev_extern.inl"
#undef OMM_DEV_EXTERN_0
#endif

    // Final transform
    Vec3f translation = { 0, 0, 0 };
    Vec3s rotation = { 0, 0, 0 };
    Vec3f scale = { 0, 0, 0 };
    Vec3f shear;

    // Dynamic transform
    if (params) {
        Mat4 objTransform, gfxTransform;
        if (geo_compute_capture_cappy_obj_transform(obj, params->animParts, objTransform)) {
            vec3f_sub(objTransform[3], obj->oGfxPos);
            mtxf_transform(gfxTransform, (f32 *) params->translation, (s16 *) params->rotation, gVec3fZero, (Vec3f) { params->scale, params->scale, params->scale });
            mtxf_mul(gfxTransform, gfxTransform, objTransform);
            mtxf_get_components(gfxTransform, translation, rotation, shear, scale);
            vec3f_add(translation, obj->oGfxPos);
        }
    }

    // Static transform
    else {
        if (!obj) { obj = o; }
        Vec3f dv = {
            -gOmmObject->cappy.tra_x * obj->oGfxScale[0],
            +gOmmObject->cappy.tra_y * obj->oGfxScale[1],
            +gOmmObject->cappy.tra_z * obj->oGfxScale[2]
        };

        // Object graph pos and angle
        if (gOmmObject->cappy.o_gfx) {
            vec3f_rotate_zxy(dv, dv, obj->oGfxAngle[0], obj->oGfxAngle[1], obj->oGfxAngle[2]);
            vec3f_sum(translation, obj->oGfxPos, dv);
            vec3s_sum(rotation, obj->oGfxAngle, gOmmObject->cappy.rot);
        }

        // Object pos and angle
        else {
            vec3f_rotate_zxy(dv, dv, obj->oFaceAnglePitch, obj->oFaceAngleYaw, obj->oFaceAngleRoll);
            vec3f_sum(translation, &obj->oPosX, dv);
            vec3s_set(rotation, obj->oFaceAnglePitch, obj->oFaceAngleYaw, obj->oFaceAngleRoll);
            vec3s_add(rotation, gOmmObject->cappy.rot);
        }

        // Object scale
        vec3f_copy(scale, obj->oGfxScale);
        vec3f_mul(scale, gOmmObject->cappy.scale);
    }

    // Update cap object and gfx
    obj_set_pos_vec3f(cap, translation);
    obj_set_angle_vec3s(cap, rotation);
    obj_set_scale_vec3f(cap, scale);
    obj_update_gfx(cap);
    cap->oFlags |= OBJ_FLAG_NO_SHADOW;
    cap->oOpacity = (0xFF - o->oTransparency) * !(o->oNodeFlags & GRAPH_RENDER_INVISIBLE);
    cap->oNodeFlags = GRAPH_RENDER_ACTIVE | (o->oNodeFlags & GRAPH_RENDER_INVISIBLE);
    cap->activeFlags = (cap->activeFlags & ~ACTIVE_FLAG_DITHERED_ALPHA) | (o->activeFlags & ACTIVE_FLAG_DITHERED_ALPHA);
}

static void omm_mario_capture_update_mario(struct MarioState *m, struct Object *o) {
    if (gOmmMario->capture.timer >= 20) {
        obj_pos_as_vec3f(o, m->pos);
        obj_vel_as_vec3f(o, m->vel);
        vec3s_set(m->faceAngle, -o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
        obj_scale(m->marioObj, 1.f);
        ANM(MARIO_ANIM_A_POSE, 1.f);
        m->forwardVel = o->oForwardVel;
        m->squishTimer = 0;
        m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    }
    obj_set_pos_vec3f(m->marioObj, m->pos);
    obj_set_angle_vec3s(m->marioObj, m->faceAngle);
    obj_update_gfx(m->marioObj);
}

static void omm_mario_capture_update_cappy(struct Object *o) {
    struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
    if (cap) {
        omm_mario_capture_update_cap_object(o, gOmmObject->cappy.object, cap);
    }
}

static void omm_mario_capture_update_transparency(struct Object *o, f32 factor) {
    o->oTransparency = clamp_s(0xFF - ((0xFF - o->oTransparency) * factor), 0x00, 0xFF);
}

static void omm_mario_capture_update_opacity(struct MarioState *m, struct Object *o) {

    // Invisible mode
    if (OMM_EXTRAS_INVISIBLE_MODE) {
        o->oFlags |= OBJ_FLAG_INVISIBLE_MODE;
    } else {
        o->oFlags &= ~OBJ_FLAG_INVISIBLE_MODE;
    }

    // Invincibility flicker
    if (gOmmObject->state._invincTimer > 0) {
        if (gOmmObject->state._invincTimer & 1) {
            o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
        } else {
            o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
        }
    }

    // Always visible when star dancing
    if (POBJ_IS_STAR_DANCING) {
        o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    }

    // Vanish cap transparency
    if (omm_mario_has_vanish_cap(m) && !omm_mario_cap_is_flickering(m)) {
        o->oTransparency = 0x80;
        o->activeFlags |= ACTIVE_FLAG_DITHERED_ALPHA;
    } else {
        o->activeFlags &= ~ACTIVE_FLAG_DITHERED_ALPHA;
    }

    // Fading opacity
    omm_mario_capture_update_transparency(o, m->fadeWarpOpacity / 255.f);

    // Camera proximity
    if (omm_camera_is_available(m)) {
        f32 dmax = omm_capture_get_hitbox_height(o) * 2.f;
        f32 dist = vec3f_dist(gLakituState.curFocus, gLakituState.curPos);
        if (dist < dmax) {
            f32 alpha = invlerp_0_1_f(dist, dmax / 2.f, dmax);
            omm_mario_capture_update_transparency(o, alpha);
        }
    }
}

static void omm_mario_capture_post_update(struct Object *o) {
    if (gOmmObject->cappy.post_update) {
        gOmmObject->cappy.post_update(o);
    }
}

//
// Star grab during a possession
//

static void omm_act_possession_update_star_dance(struct MarioState *m, struct Object *o) {
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;

    // Turn Mario (not the possessed object) to face the camera
    // so that the camera won't try to face the possessed object
    m->faceAngle[1] = m->area->camera->yaw;
    vec3s_set(m->marioObj->oGfxAngle, 0, m->area->camera->yaw, 0);
    omm_mario_lock_camera(m, true);

    // Enable time stop and spawn the celebration star
    if (m->actionTimer == 0) {
        m->faceAngle[1] = m->area->camera->yaw;
        vec3s_set(m->marioObj->oGfxAngle, 0, m->area->camera->yaw, 0);
        disable_background_sound();
        audio_play_course_clear();
        f32 radius = omm_capture_get_hitbox_radius(o);
        f32 height = omm_capture_get_top(o) + 40.f * o->oScaleY * gOmmObject->cappy.scale;
        omm_obj_spawn_star_celebration(m->marioObj, radius, height, m->interactObj->behavior);
        set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        m->marioObj->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    }

    // Display the text box "You got a star"
    else if (m->actionTimer == 39) {
#if OMM_GAME_IS_SMSR
#define gLastCompletedStarNum (gLastCompletedStarNum * (obj_get_first_with_behavior(bhvOmmStarCelebration)->oCelebStarBehavior != bhvCustomSMSRStarReplica))
#endif
        ustr_t courseName, actName;
        omm_render_effect_you_got_a_star_begin(
            OMM_TEXT_YOU_GOT_A_STAR,
            omm_level_get_course_name(courseName, gCurrLevelNum, OMM_GAME_MODE, false, false),
            omm_level_get_act_name(actName, gCurrLevelNum, gLastCompletedStarNum, OMM_GAME_MODE, false, false)
        );
    }

    // Here we go!
    else if (m->actionTimer == 40) {
        set_camera_shake_from_hit(SHAKE_GROUND_POUND);
    }

    // Resume action
    else if (m->actionTimer == 80) {
        obj_deactivate_all_with_behavior(bhvOmmStarCelebration);
        o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        m->marioObj->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        enable_background_sound();
        audio_stop_course_clear();
        omm_render_effect_you_got_a_star_end();
        omm_health_fully_heal_mario(m, true);
        m->healCounter = OMM_BREATH_REFILL;
        gOmmMario->capture.starDance = false;

#if OMM_GAME_IS_SM64
        // WF Tower transition
        if (gCurrCourseNum == COURSE_WF && gLastCompletedStarNum == 1) {
            omm_obj_spawn_wf_transition(m->marioObj);
            omm_mario_lock(m, -1);
        }
#endif
    }
}

//
// Warp
//

static s32 omm_act_possession_update_warp_timer() {
    s32 dir = sign_0_s(gOmmWarp->timer);
    gOmmWarp->timer -= dir;
    return dir * (gOmmWarp->timer == 0);
}

static void omm_act_possession_locate_nearest_warp(struct Object *o) {
    gOmmWarp->object = NULL;
    f32 nearestDist = LEVEL_BOUNDARY_MAX * 2;
    for_each_object_in_interaction_lists(obj) {
        if (obj != o && obj->activeFlags) {
            if (obj->oInteractType & (INTERACT_WARP | INTERACT_WARP_DOOR)) {
                f32 dist = obj_get_distance(o, obj);
                if (dist < nearestDist) {
                    gOmmWarp->object = obj;
                    nearestDist = dist;
                }
            }
        }
    }
}

static void omm_act_possession_check_nearest_warp(struct Object *o) {
    struct Object *warp = gOmmWarp->object;
    if (warp) {
        if (!vec3f_check_cylinder_overlap(
            &o->oPosX, omm_capture_get_hitbox_radius(o), omm_capture_get_top(o), 0.f,
            &warp->oPosX, warp->hitboxRadius + 50.f, warp->hitboxHeight + 200.f, warp->hitboxDownOffset + 100.f
        )) {
            gOmmWarp->object = NULL;
        }
    }
}

static bool omm_act_possession_check_warp(struct MarioState *m, struct Object *o) {
    switch (gOmmWarp->state) {

        case POBJ_WARP_STATE_NOT_WARPING: {
            omm_act_possession_check_nearest_warp(o);
        } break;

        case POBJ_WARP_STATE_WARPING: {
            if (omm_act_possession_update_warp_timer() != 0) {
                return true;
            }
        } break;

        case POBJ_WARP_STATE_OPENING_DOOR: {
            s32 res = omm_act_possession_update_warp_timer();
            if (res == -1) {
                m->interactObj = m->usedObj = gOmmWarp->object;
                m->actionArg = should_push_or_pull_door(m, gOmmWarp->object);
                gOmmWarp->timer = level_trigger_warp(m, WARP_OP_WARP_DOOR) - 1;
            } else if (res == 1) {
                return true;
            }
        } break;

        case POBJ_WARP_STATE_FADING_OUT: {
            m->fadeWarpOpacity = max_s(m->fadeWarpOpacity - 8, 0x00);
            s32 res = omm_act_possession_update_warp_timer();
            if (res == -1) {
                m->interactObj = m->usedObj = gOmmWarp->object;
                gOmmWarp->timer = level_trigger_warp(m, WARP_OP_TELEPORT) - 1;
            } else if (res == 1) {
                return true;
            }
        } break;

        case POBJ_WARP_STATE_FADING_IN: {
            m->fadeWarpOpacity = min_s(m->fadeWarpOpacity + 8, 0xFF);
            if (!omm_mario_is_locked(m)) {
                gOmmWarp->state = POBJ_WARP_STATE_NOT_WARPING;
                omm_act_possession_locate_nearest_warp(o);
            }
        } break;

        case POBJ_WARP_STATE_EMERGING_FROM_PIPE: {
            if (!omm_mario_is_locked(m) || (obj_is_on_ground(o) && o->oVelY <= 0.f)) {
                gOmmWarp->state = POBJ_WARP_STATE_NOT_WARPING;
                omm_act_possession_locate_nearest_warp(o);
                omm_mario_unlock(m);
            }
        } break;

        case POBJ_WARP_STATE_HAS_WARPED: {
            if (!omm_mario_is_locked(m)) {
                gOmmWarp->state = POBJ_WARP_STATE_NOT_WARPING;
                omm_act_possession_locate_nearest_warp(o);
            }
        } break;
    }
    return false;
}

//
// Act
//

bool omm_mario_lock(struct MarioState *m, s32 duration) {
    if (!omm_mario_is_capture(m)) {
        return false;
    }
    gOmmMario->capture.lockTimer = duration;
    return true;
}

bool omm_mario_lock_once(struct MarioState *m, s32 duration) {
    return !omm_mario_is_locked(m) && omm_mario_lock(m, duration);
}

bool omm_mario_unlock(struct MarioState *m) {
    if (!omm_mario_is_capture(m)) {
        return false;
    }
    gOmmMario->capture.lockTimer = 0;
    return true;
}

bool omm_mario_is_locked(struct MarioState *m) {
    return omm_mario_is_capture(m) && gOmmMario->capture.lockTimer != 0;
}

// Possession phases
// 10 frames / 1: Mario spins towards the object to possess
// 04 frames / 2: Mario stops, dive, rotates towards the ground and starts shrinking
// 06 frames / 3: while shrinking, Mario dives into the object

typedef struct { s32 from; s32 to; f32 t; s16 angle; f32 scale; s32 anim; } OmmCappyPossessAnimParams;
static const OmmCappyPossessAnimParams sOmmCappyPossessAnimParams[20] = {

    // Phase 1
    { 0, 1, 0.19f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 0.36f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 0.51f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 0.64f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 0.75f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 0.84f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 0.91f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 0.96f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 0.99f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 1.00f, 0x0000, 1.00f, MARIO_ANIM_FORWARD_SPINNING },

    // Phase 2
    { 1, 1, 0.00f, 0x0000, 1.00f, MARIO_ANIM_DIVE },
    { 1, 1, 0.00f, 0x1000, 0.95f, MARIO_ANIM_DIVE },
    { 1, 1, 0.00f, 0x2000, 0.90f, MARIO_ANIM_DIVE },
    { 1, 1, 0.00f, 0x3000, 0.85f, MARIO_ANIM_DIVE },

    // Phase 3
    { 1, 2, 0.05f, 0x4000, 0.80f, MARIO_ANIM_DIVE },
    { 1, 2, 0.15f, 0x4000, 0.70f, MARIO_ANIM_DIVE },
    { 1, 2, 0.30f, 0x4000, 0.60f, MARIO_ANIM_DIVE },
    { 1, 2, 0.50f, 0x4000, 0.50f, MARIO_ANIM_DIVE },
    { 1, 2, 0.75f, 0x4000, 0.40f, MARIO_ANIM_DIVE },
    { 1, 2, 1.00f, 0x4000, 0.30f, MARIO_ANIM_DIVE },
};

s32 omm_act_possession(struct MarioState *m) {
    struct Object *o = gOmmCapture;

    if (gOmmMario->capture.timer < 20) {

        // Play the possession animation for 20 frames
        const OmmCappyPossessAnimParams *params = sOmmCappyPossessAnimParams + gOmmMario->capture.timer;
        m->pos[0] = lerp_f(params->t, gOmmMario->capture.animPos[params->from][0], gOmmMario->capture.animPos[params->to][0]);
        m->pos[1] = lerp_f(params->t, gOmmMario->capture.animPos[params->from][1], gOmmMario->capture.animPos[params->to][1]);
        m->pos[2] = lerp_f(params->t, gOmmMario->capture.animPos[params->from][2], gOmmMario->capture.animPos[params->to][2]);
        m->floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &m->floor);
        m->faceAngle[0] = params->angle;
        m->squishTimer = 0xFF;
        obj_scale(m->marioObj, params->scale);
        ANM(params->anim, 1.f);
        PFX(PARTICLE_SPARKLES);
        gOmmMario->capture.timer++;
        gOmmObject->state._invincTimer = 15;
        gOmmWarp->state = POBJ_WARP_STATE_NOT_WARPING;

    } else {

        // Init capture
        if (gOmmMario->capture.timer == 20) {
            m->pos[1] -= omm_capture_get_hitbox_down_offset(o);
            m->controller->buttonPressed &= ~Z_TRIG;
            gOmmMario->capture.timer++;
        }

        // Spawn Cappy if not spawned
        if (!obj_get_first_with_behavior(bhvOmmPossessedObjectCap)) {
            struct Object *cap = spawn_object(o, omm_player_graphics_get_selected_normal_cap(), bhvOmmPossessedObjectCap);
            cap->oFlags = OBJ_FLAG_NO_SHADOW;
        }

        // If Mario is locked, don't check [Z] press and zero-init inputs
        if (omm_mario_is_locked(m) || POBJ_IS_STAR_DANCING || POBJ_IS_OPENING_DOORS || gOmmMario->capture.firstPerson) {
            gOmmMario->capture.stickX        = 0;
            gOmmMario->capture.stickY        = 0;
            gOmmMario->capture.stickMag      = 0;
            gOmmMario->capture.stickYaw      = 0;
            gOmmMario->capture.buttonPressed = 0;
            gOmmMario->capture.buttonDown    = 0;
            if (gOmmMario->capture.lockTimer > 0 && !POBJ_IS_STAR_DANCING) {
                gOmmMario->capture.lockTimer--;
            }

        } else {

            // Press [Z] to leave the object
            if (!gOmmGlobals->yoshiMode && (m->controller->buttonPressed & Z_TRIG) != 0) {
                omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, 6);
                return OMM_MARIO_ACTION_RESULT_CANCEL;
            }

            // Update inputs
            gOmmMario->capture.stickX        = m->controller->stickX / 64.f;
            gOmmMario->capture.stickY        = m->controller->stickY / 64.f;
            gOmmMario->capture.stickMag      = m->controller->stickMag / 64.f;
            gOmmMario->capture.stickYaw      = m->intendedYaw;
            gOmmMario->capture.buttonPressed = m->controller->buttonPressed;
            gOmmMario->capture.buttonDown    = m->controller->buttonDown;
        }

        // Check warp state
        if (omm_act_possession_check_warp(m, o)) {
            omm_mario_unpossess_object_before_warp(m);
            return OMM_MARIO_ACTION_RESULT_CANCEL;
        }

        // If Mario loses his cap, unpossess object
        if (!(m->flags & MARIO_CAP_ON_HEAD) && !POBJ_IS_TALKING) {
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB, 15);
            if (gOmmGlobals->yoshiMode) {
                m->health = OMM_HEALTH_DEAD;
                level_trigger_warp(m, WARP_OP_DEATH);
                obj_destroy(o);
            }
            return OMM_MARIO_ACTION_RESULT_CANCEL;
        }

        // Set possessed object position to Mario's position and update floor height
        bool isOnGround = obj_is_on_ground(o);
        o->oPosX = m->pos[0];
        o->oPosY = m->pos[1];
        o->oPosZ = m->pos[2];
        o->oFloorHeight = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
        o->hitboxDownOffset = 0;
        if (isOnGround && o->oDistToFloor < 60.f && o->oVelY <= 0.f) {
            o->oPosY = o->oFloorHeight;
        }

        // Do not update the possessed object during the star grab animation
        if (POBJ_IS_STAR_DANCING) {
            omm_capture_reset_squish(o);
            omm_capture_update_gfx(o);
            omm_act_possession_update_star_dance(m, o);
            m->controller->buttonPressed &= ~START_BUTTON;
            gPlayer1Controller->buttonPressed &= ~START_BUTTON;
            m->actionTimer++;
        }

        // Update possessed object
        else {
            mem_zero(&gOmmObject->cappy, sizeof(gOmmObject->cappy));
            gOmmObject->state.friction[0] = 0.80f;
            gOmmObject->state.friction[1] = 0.80f;
            gOmmObject->state.friction[2] = 0.95f;
            gOmmObject->state._properties = 0;
            gOmmObject->state._bullyTimer = max_s(gOmmObject->state._bullyTimer - 1, 0);
            gOmmObject->state._cannonTimer = max_s(gOmmObject->state._cannonTimer - 1, 0) * (o->oVelY > 0.f);
            while (true) {
                s32 res = omm_capture_update(o);
                if (res == 0) break;
                if (res == 1) return OMM_MARIO_ACTION_RESULT_CANCEL;
                if (res == 2) continue;
            }
            omm_capture_update_gfx(o);
            if (gOmmObject->state._cannonTimer) {
                struct Object *smoke = spawn_object(o, MODEL_SMOKE, bhvWhitePuff2);
                smoke->oFlags |= OBJ_FLAG_DONT_RENDER_ON_INIT;
                obj_scale(smoke, 2.f);
            }
        }

#if OMM_CODE_DEBUG
        gOmmMario->capture.hitboxRadius = o->hitboxRadius;
        gOmmMario->capture.hitboxHeight = o->hitboxHeight;
        gOmmMario->capture.hitboxWall   = o->oWallHitboxRadius;
#endif
        obj_reset_hitbox(o, 0, 0, 0, 0, 0, 0);

        // Post update
        omm_mario_capture_update_opacity(m, o);
        omm_mario_capture_post_update(o);
    }

    // Unpossess if Mario is dead
    if (omm_mario_is_dead(m)) {
        omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
        if (gOmmGlobals->yoshiMode) {
            obj_destroy(o);
        }
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Update Mario and Cappy objects and gfx
    omm_mario_capture_update_mario(m, o);
    omm_mario_capture_update_cappy(o);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

//
// Object copy
//

#define copy_object_field(dst, src, field) \
    mem_cpy(&((dst)->field), &((src)->field), sizeof((src)->field))

#define save_object_pos_and_angle(o) \
    Vec3f oPos = { (o)->oPosX, (o)->oPosY, (o)->oPosZ }; \
    Vec3s oAngle = { (o)->oFaceAnglePitch, (o)->oFaceAngleYaw, (o)->oFaceAngleRoll };

#define set_object_pos_and_angle(o) \
    obj_set_xyz(o, oPos[0], oPos[1], oPos[2]); \
    obj_set_home(o, oPos[0], oPos[1], oPos[2]); \
    obj_set_angle(o, oAngle[0], oAngle[1], oAngle[2]); \
    obj_set_vel(o, 0, 0, 0);

static void copy_object_data(struct Object *o, bool possess) {
    static struct Object sObjectData[1];
    struct Object *src = possess ? o : sObjectData;
    struct Object *dst = possess ? sObjectData : o;
    copy_object_field(dst, src, behavior);
    copy_object_field(dst, src, curBhvCommand);
    copy_object_field(dst, src, bhvStackIndex);
    copy_object_field(dst, src, bhvStack);
    copy_object_field(dst, src, bhvDelayTimer);
    copy_object_field(dst, src, respawnInfoType);
    copy_object_field(dst, src, hitboxRadius);
    copy_object_field(dst, src, hitboxHeight);
    copy_object_field(dst, src, hurtboxRadius);
    copy_object_field(dst, src, hurtboxHeight);
    copy_object_field(dst, src, hitboxDownOffset);
    copy_object_field(dst, src, oGfxScale);
    copy_object_field(dst, src, rawData);
#if IS_64_BIT
    copy_object_field(dst, src, ptrData);
#endif
}

//
// Possess
//

bool omm_mario_possess_object(struct MarioState *m, struct Object *o, u32 possessFlags) {
    if (!(possessFlags & OMM_MARIO_POSSESS_FORCE_CAPTURE)) {

        // Don't capture things
        if (!OMM_CAP_CAPPY_CAPTURE) {
            return false;
        }

        // 'Press' mode: Press X to capture, hold to attack
        // 'Hold' mode: Press X to attack, hold to capture
        // Note that flaming bob-ombs are always captured, never attacked
        if (o->behavior != bhvOmmFlamingBobomb && !(possessFlags & OMM_MARIO_POSSESS_IGNORE_PRESS_HOLD)) {
            if ((OMM_CAP_CAPPY_CAPTURE_PRESS && (m->controller->buttonDown & X_BUTTON)) ||
                (OMM_CAP_CAPPY_CAPTURE_HOLD && !(m->controller->buttonDown & X_BUTTON))) {
                return false;
            }
        }

        // Mario can't possess an object if he's holding or riding something
        if (m->heldObj || m->riddenObj) {
            return false;
        }

        // Mario can't possess the same object twice in a row
        if (gOmmMario->capture.prev == o) {
            return false;
        }
    }

    // No milk
    if (omm_mario_is_milk(m)) {
        return false;
    }

    // Not tangible
    if ((possessFlags & OMM_MARIO_POSSESS_CHECK_TANGIBILITY) && o->oIntangibleTimer != 0) {
        return false;
    }

    // Oof
    if (omm_mario_is_dead(m) || m->hurtCounter != 0) {
        return false;
    }

    // Mario is already possessing an object, duh
    if (omm_mario_is_capture(m)) {
        return false;
    }

    // Check out of bounds
    struct Surface *floor = NULL;
    find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
    if (!floor) {
        return false;
    }

    // Try to capture
    o->oCaptureData = omm_behavior_data_get_capture(o->behavior);
    if (!o->oCaptureData) {
        return false;
    }

    // Captures other than flaming bob-ombs cannot be possessed during the Bowser 4 fight if mode is not completed
    u64 captureType = omm_capture_get_type(o);
    if (captureType != OMM_CAPTURE_FLAMING_BOBOMB && !gOmmAllow->captures) {
        o->oCaptureData = NULL;
        return false;
    }

    // Try to init
    gOmmData->reset_object();
    if (!omm_capture_init(o)) {
        o->oCaptureData = NULL;
        return false;
    }

    // Register capture
    if (captureType) {
        omm_save_file_register_capture(gCurrSaveFileNum - 1, OMM_GAME_MODE, captureType);
    }

    // OK
    copy_object_data(o, true);
    obj_set_vel(o, 0, o->behavior == bhvChainChomp ? max_f(o->oVelY, 0) : 0, 0); // What a stupid glitch :(
    o->curBhvCommand = bhvOmmPossessedObject;
    o->bhvStackIndex = 0;
    o->oRoom = -1;
    o->oFlags &= (OBJ_FLAG_GFX_INITED | OBJ_FLAG_UPDATE_AREA_INDEX | OBJ_FLAG_MONEYBAG_COIN_INTERACTED | OBJ_FLAG_SPARKLY_NOT_ENEMY);
    o->oIntangibleTimer = 0;
    o->oTransparency = 0;
    o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    o->activeFlags &= ~(ACTIVE_FLAG_IN_DIFFERENT_ROOM | ACTIVE_FLAG_DITHERED_ALPHA);
    gOmmMario->capture.obj = o;
    gOmmMario->capture.timer = 0;
    gOmmMario->capture.lockTimer = 0;
    gOmmMario->capture.starDance = false;
    gOmmMario->capture.openDoors = false;
    gOmmMario->capture.firstPerson = false;
    gOmmObject->state._initialHome[0] = o->oHomeX;
    gOmmObject->state._initialHome[1] = o->oHomeY;
    gOmmObject->state._initialHome[2] = o->oHomeZ;
    struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
    if (cap) obj_mark_for_deletion(cap);

    // Init animation positions
    f32 x = o->oPosX;
    f32 y = o->oPosY + omm_capture_get_top(o);
    f32 z = o->oPosZ;
    vec3f_set(gOmmMario->capture.animPos[0], m->pos[0], m->pos[1], m->pos[2]);
    vec3f_set(gOmmMario->capture.animPos[1], x, y + 180.f, z);
    vec3f_set(gOmmMario->capture.animPos[2], x, y - 30.f, z);
    vec3s_set(m->faceAngle, 0, mario_obj_angle_to_object(m, o), 0);

    // Possess
    m->fadeWarpOpacity = 0xFF;
    omm_capture_reset_camera();
    mario_stop_riding_and_holding(m);
    omm_mario_set_action(m, ACT_OMM_POSSESSION, 0, Z_TRIG);
    omm_sound_play(OMM_SOUND_EVENT_CAPTURE, m->marioObj->oCameraToObject);
    omm_cappy_unload();
    omm_stats_increase(captures, 1);
    return true;
}

bool omm_mario_possess_object_after_warp(struct MarioState *m) {
    if (gOmmWarp->state != POBJ_WARP_STATE_WARPING) {
        return false;
    }

    // Don't capture things
    if (!gOmmGlobals->yoshiMode && !OMM_CAP_CAPPY_CAPTURE) {
        return false;
    }

    // Captures cannot enter the Bowser 4 fight if mode is not completed
    if (omm_sparkly_is_bowser_4_battle() && !OMM_SPARKLY_BYPASS_BOWSER_RULES) {
        return false;
    }

    // Spawn object and initialize it
    struct Object *o = obj_spawn_from_geo(m->marioObj, gOmmWarp->georef, gOmmWarp->behavior);
    o->oBehParams = gOmmWarp->behParams;
    o->oBehParams2ndByte = gOmmWarp->behParams2ndByte;
    o->oDistanceToMario = 0;
    o->oFlags |= OBJ_FLAG_CAPTURE_AFTER_WARP;
    o->parentObj = o;
    o->respawnInfoType = RESPAWN_INFO_TYPE_NULL;
    o->respawnInfo = NULL;
    obj_update(o);
    obj_set_pos_vec3f(o, m->pos);
    obj_set_angle(o, 0, m->faceAngle[1], 0);
    obj_update_gfx(o);

    // Try to capture
    o->oCaptureData = omm_behavior_data_get_capture(o->behavior);
    gOmmData->reset_object();
    switch (omm_capture_get_type(o)) {
        case OMM_CAPTURE_HOOT: { o->oHootAvailability = HOOT_AVAIL_READY_TO_FLY; } break;
        case OMM_CAPTURE_WHOMP: { o->oNumLootCoins = 0; } break;
        case OMM_CAPTURE_SNOWMAN: { o->oAction = 1; } break;
        case OMM_CAPTURE_BOO: { o->oAction = 1; o->oBehParams2ndByte = 1; gOmmObject->state.actionFlag = true; } break;
        case OMM_CAPTURE_MAD_PIANO: { o->oFaceAngleYaw -= 0x4000; } break;
        case OMM_CAPTURE_MONEY_BAG: { o->oAction = MONEYBAG_ACT_MOVE_AROUND; } break;
        case OMM_CAPTURE_MIPS: { o->oMipsStarStatus = MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR; } break;
        case OMM_CAPTURE_TOAD: { o->oToadMessageRecentlyTalked = TRUE;
            switch (o->oToadMessageDialogId) {
                case TOAD_STAR_1_DIALOG: o->oToadMessageDialogId = TOAD_STAR_1_DIALOG_AFTER; break;
                case TOAD_STAR_2_DIALOG: o->oToadMessageDialogId = TOAD_STAR_2_DIALOG_AFTER; break;
                case TOAD_STAR_3_DIALOG: o->oToadMessageDialogId = TOAD_STAR_3_DIALOG_AFTER; break;
            }
        } break;
    }
    if (!omm_capture_init(o)) {
        obj_mark_for_deletion(o);
        return false;
    }

    // OK
    copy_object_data(o, true);
    obj_set_vel(o, 0, 0, 0);
    o->curBhvCommand = bhvOmmPossessedObject;
    o->bhvStackIndex = 0;
    o->oRoom = -1;
    o->oFlags &= (OBJ_FLAG_GFX_INITED | OBJ_FLAG_UPDATE_AREA_INDEX | OBJ_FLAG_MONEYBAG_COIN_INTERACTED | OBJ_FLAG_SPARKLY_NOT_ENEMY);
    o->oIntangibleTimer = 0;
    o->oTransparency = 0;
    o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    o->activeFlags &= ~(ACTIVE_FLAG_IN_DIFFERENT_ROOM | ACTIVE_FLAG_DITHERED_ALPHA);
    gOmmMario->capture.obj = o;
    gOmmMario->capture.timer = 20;
    gOmmMario->capture.lockTimer = 0;
    gOmmMario->capture.starDance = false;
    gOmmMario->capture.openDoors = false;
    gOmmMario->capture.firstPerson = false;
    gOmmObject->state._initialHome[0] = o->oHomeX;
    gOmmObject->state._initialHome[1] = o->oHomeY;
    gOmmObject->state._initialHome[2] = o->oHomeZ;
    struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
    if (cap) obj_mark_for_deletion(cap);

    // Possess
    m->fadeWarpOpacity = 0xFF;
    omm_capture_reset_camera();
    omm_mario_set_action(m, ACT_OMM_POSSESSION, 0, Z_TRIG);
    return true;
}

//
// Unpossess
//

typedef struct { u32 action; u32 actionArg; f32 forwardVel; f32 yVel; bool fromTop; bool iframes; } OmmMarioUnpossessAction;
static const OmmMarioUnpossessAction sOmmMarioUnpossessActions[][3] = {
[OMM_MARIO_UNPOSSESS_ACT_NONE] = {
    { ACT_IDLE,                    0,   0,  0, false, true  },
    { ACT_FREEFALL,                0,   0,  0, false, true  },
    { ACT_WATER_IDLE,              0,   0,  0, false, true  },
},
[OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT] = {
    { ACT_OMM_LEAVE_OBJECT_JUMP,   0,  18, 42, true,  true  },
    { ACT_OMM_LEAVE_OBJECT_JUMP,   0,  18, 42, true,  true  },
    { ACT_OMM_LEAVE_OBJECT_WATER,  0,   0, 20, true,  true  },
},
[OMM_MARIO_UNPOSSESS_ACT_FORWARD_KB] = {
    { ACT_HARD_FORWARD_GROUND_KB,  1,  28,  0, false, false },
    { ACT_HARD_FORWARD_AIR_KB,     1,  28,  0, false, false },
    { ACT_FORWARD_WATER_KB,        1,  28,  0, false, false },
},
[OMM_MARIO_UNPOSSESS_ACT_BACKWARD_KB] = {
    { ACT_HARD_BACKWARD_GROUND_KB, 1, -28,  0, false, false },
    { ACT_HARD_BACKWARD_AIR_KB,    1, -28,  0, false, false },
    { ACT_BACKWARD_WATER_KB,       1, -28,  0, false, false },
},
[OMM_MARIO_UNPOSSESS_ACT_BURNT] = {
    { ACT_BURNING_JUMP,            1,  12, 34, true,  false },
    { ACT_BURNING_JUMP,            1,  12, 34, true,  false },
    { ACT_OMM_LEAVE_OBJECT_WATER,  0,   0, 20, true,  true  },
},
[OMM_MARIO_UNPOSSESS_ACT_GRABBED] = {
    { ACT_GRABBED,                 0,   0,  0, false, false },
    { ACT_GRABBED,                 0,   0,  0, false, false },
    { ACT_OMM_GRABBED_WATER,       0,   0,  0, false, false },
},
[OMM_MARIO_UNPOSSESS_ACT_EATEN] = {
    { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false },
    { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false },
    { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false },
},
[OMM_MARIO_UNPOSSESS_ACT_BLOWN] = {
    { ACT_GETTING_BLOWN,           0, -24, 12, false, false },
    { ACT_GETTING_BLOWN,           0, -24, 12, false, false },
    { ACT_BACKWARD_WATER_KB,       1, -28,  0, false, false },
},
[OMM_MARIO_UNPOSSESS_ACT_TORNADO] = {
    { ACT_TORNADO_TWIRLING,        0,   0,  0, false, false },
    { ACT_TORNADO_TWIRLING,        0,   0,  0, false, false },
    { ACT_BACKWARD_WATER_KB,       1, -28,  0, false, false },
},
};

static bool __omm_mario_unpossess_object(struct MarioState *m, s32 unpossessAct, u32 objIntangibleFrames, s16 *faceAngleYaw) {
    struct Object *o = gOmmCapture;
    if (!o) return false;

    // Unpossess action
    omm_capture_reset_squish(o);
    o->oFloorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &o->oFloor);
    bool isOnGround = obj_is_on_ground(o);
    f32 captureTopY = omm_capture_get_top(o);
    s32 terrainType = (((o->oPosY + captureTopY) < find_water_level(o->oPosX, o->oPosZ)) ? 2 : (isOnGround ? 0 : 1));
    const OmmMarioUnpossessAction *action = &sOmmMarioUnpossessActions[unpossessAct][terrainType];

    // Update Mario
    omm_mario_capture_update_mario(m, o);
    m->faceAngle[1] = (faceAngleYaw != NULL ? *faceAngleYaw : o->oFaceAngleYaw);
    mario_set_forward_vel(m, action->forwardVel);
    omm_mario_set_action(m, action->action, action->actionArg, 0);
    m->pos[1] += action->fromTop * captureTopY;
    m->vel[1] = action->yVel;
    m->input &= ~(INPUT_FIRST_PERSON | INPUT_A_PRESSED | INPUT_A_DOWN | INPUT_B_PRESSED | INPUT_Z_DOWN | INPUT_Z_PRESSED);
    m->squishTimer = 0;
    m->controller->buttonPressed = 0;
    m->marioObj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    m->marioObj->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    if (action->iframes) {
        m->invincTimer = max_s(15, m->invincTimer);
    }

    // Sfx, pfx, restore camera, unload cap
    omm_sound_play(OMM_SOUND_EVENT_UNCAPTURE, m->marioObj->oCameraToObject);
    obj_spawn_white_puff(m->marioObj, NO_SOUND);
    omm_capture_reset_camera();
    struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
    if (cap) obj_mark_for_deletion(cap);

    // End object possession
    bool shouldRefObject = omm_capture_should_reference_object(o);
    save_object_pos_and_angle(o);
    copy_object_data(o, false);
    set_object_pos_and_angle(o);
    omm_capture_end(o);
    o->oPosY += omm_capture_get_hitbox_down_offset(o);
    o->oRoom = -1;
    o->oMoveFlags = 0;
    o->oIntangibleTimer = objIntangibleFrames;
    o->oTransparency = 0;
    o->oFlags &= ~OBJ_FLAG_INVISIBLE_MODE;
    o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    o->activeFlags &= ~(ACTIVE_FLAG_IN_DIFFERENT_ROOM | ACTIVE_FLAG_DITHERED_ALPHA);
    o->oCaptureData = NULL;

    // If the release happens during an OMM Bowser fight, unload the capture
    if (omm_bowser_is_active() && omm_capture_get_type(o) != OMM_CAPTURE_FLAMING_BOBOMB) {
        obj_mark_for_deletion(o);
        shouldRefObject = false;
    }

    // Clear fields, reset Cappy bounce
    gOmmMario->capture.obj = NULL;
    gOmmMario->capture.prev = ((isOnGround || !shouldRefObject) ? NULL : o);
    gOmmMario->capture.timer = 0;
    gOmmMario->capture.lockTimer = 0;
    gOmmMario->capture.starDance = false;
    gOmmMario->capture.openDoors = false;
    gOmmMario->capture.firstPerson = false;
    gOmmMario->cappy.bounced = false;
    return true;
}

bool omm_mario_unpossess_object(struct MarioState *m, s32 unpossessAct, u32 objIntangibleFrames) {
    return __omm_mario_unpossess_object(m, unpossessAct, objIntangibleFrames, NULL);
}

bool omm_mario_unpossess_object_with_yaw(struct MarioState *m, s32 unpossessAct, u32 objIntangibleFrames, s16 faceAngleYaw) {
    return __omm_mario_unpossess_object(m, unpossessAct, objIntangibleFrames, &faceAngleYaw);
}

bool omm_mario_unpossess_object_before_warp(struct MarioState *m) {
    struct Object *o = gOmmCapture;
    if (!o) return false;

    // Unpossess normally if the capture cannot warp
    if (gOmmWarp->state == POBJ_WARP_STATE_CANNOT_WARP) {
        return omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
    }

    // Update Mario
    omm_mario_capture_update_mario(m, o);
    omm_mario_set_action(m, ACT_DISAPPEARED, 0, 0);

    // Restore camera, unload cap
    omm_capture_reset_camera();
    struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
    if (cap) obj_mark_for_deletion(cap);

    // End object possession
    save_object_pos_and_angle(o);
    copy_object_data(o, false);
    set_object_pos_and_angle(o);
    omm_capture_end(o);

    // Remember the capture, then unload the object
    gOmmWarp->object = NULL;
    gOmmWarp->behavior = (o->behavior == bhvChainChomp ? bhvOmmChainChompFree : o->behavior);
    gOmmWarp->georef = (o->oGraphNode ? o->oGraphNode->georef : NULL);
    gOmmWarp->behParams = o->oBehParams;
    gOmmWarp->behParams2ndByte = o->oBehParams2ndByte;
    gOmmWarp->state = POBJ_WARP_STATE_WARPING;
    gOmmWarp->timer = 0;
    obj_mark_for_deletion(o);

    // Clear fields
    gOmmMario->capture.obj = NULL;
    gOmmMario->capture.prev = NULL;
    gOmmMario->capture.timer = 0;
    gOmmMario->capture.lockTimer = 0;
    gOmmMario->capture.starDance = false;
    gOmmMario->capture.openDoors = false;
    gOmmMario->capture.firstPerson = false;
    return true;
}
