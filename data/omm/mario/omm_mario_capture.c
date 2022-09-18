#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Object copy
//

#define copy_object_field(field) omm_copy(&dst->field, &src->field, sizeof(src->field))
static void copy_object_data(struct Object *o, bool revert) {
    static struct Object sObjectCopy;
    struct Object *src = (revert ? &sObjectCopy : o);
    struct Object *dst = (revert ? o : &sObjectCopy);

    // Save position and angle
    Vec3f pos = { o->oPosX, o->oPosY, o->oPosZ };
    Vec3s angle = { o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll };

    // Copy data
    copy_object_field(behavior);
    copy_object_field(curBhvCommand);
    copy_object_field(bhvStackIndex);
    copy_object_field(bhvStack);
    copy_object_field(bhvDelayTimer);
    copy_object_field(respawnInfoType);
    copy_object_field(hitboxRadius);
    copy_object_field(hitboxHeight);
    copy_object_field(hurtboxRadius);
    copy_object_field(hurtboxHeight);
    copy_object_field(hitboxDownOffset);
    copy_object_field(oGfxScale);
    copy_object_field(rawData);
#if IS_64_BIT
    copy_object_field(ptrData);
#endif

    // Restore saved data
    obj_set_pos(o, pos[0], pos[1], pos[2]);
    obj_set_home(o, pos[0], pos[1], pos[2]);
    obj_set_angle(o, angle[0], angle[1], angle[2]);
    obj_set_vel(o, 0.f, 0.f, 0.f);
}
#undef copy_object_field

//
// Star grab during a possession
//

static void omm_act_possession_update_star_dance(struct MarioState *m) {
    static const BehaviorScript *starBehavior = NULL;
    static struct Object *celebStar = NULL;
    struct Object *o = gOmmCapture;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;

    // Turn Mario (not the possessed object) to face the camera
    // so that the camera won't try to face the possessed object
    m->faceAngle[1] = m->area->camera->yaw;
    vec3s_set(m->marioObj->oGfxAngle, 0, m->area->camera->yaw, 0);
    omm_mario_lock_camera(m, true);

    // Enable time stop and spawn the celebration star
    if (m->actionTimer == 0) {
        starBehavior = m->interactObj->behavior;
        m->faceAngle[1] = m->area->camera->yaw;
        vec3s_set(m->marioObj->oGfxAngle, 0, m->area->camera->yaw, 0);
        disable_background_sound();
        audio_play_course_clear();
        f32 radius = omm_capture_get_hitbox_radius(o);
        f32 height = omm_capture_get_top(o) + 40.f * o->oScaleY * (gOmmObject->cappy.scale / 100.f);
        celebStar = omm_spawn_star_celebration(m->marioObj, radius, height);
        set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        m->marioObj->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        if (o->behavior != bhvToadMessage) {
            o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        }
    }

    // Display the text box "You got a star"
    else if (m->actionTimer == 39) {
#if OMM_GAME_IS_SMSR
#define gLastCompletedStarNum (gLastCompletedStarNum * (starBehavior != bhvCustomSMSRStarReplica))
#endif
        omm_render_effect_you_got_a_star_begin(OMM_TEXT_YOU_GOT_A_STAR, omm_level_get_name(gCurrLevelNum, false, false), omm_level_get_act_name(gCurrLevelNum, gLastCompletedStarNum, false, false));
    }
    
    // Here we go!
    else if (m->actionTimer == 40) {
        set_camera_shake_from_hit(SHAKE_GROUND_POUND);
    }
    
    // Resume action
    else if (m->actionTimer == 80) {
        obj_mark_for_deletion(celebStar);
        o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        m->marioObj->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
        clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        enable_background_sound();
        audio_stop_course_clear();
        omm_render_effect_you_got_a_star_end();
        omm_health_fully_heal_mario(m);
        m->healCounter = OMM_O2_REFILL;
        starBehavior = NULL;
        omm_mario_unlock(m);
        gOmmMario->capture.timer = 20;
    }
}

//
// Act
//

bool omm_mario_lock(struct MarioState *m, s32 duration) {
    if (m->action != ACT_OMM_POSSESSION) {
        return false;
    }
    gOmmMario->capture.lockTimer = duration;
    return true;
}

bool omm_mario_lock_star_grab(struct MarioState *m) {
    if (m->action != ACT_OMM_POSSESSION) {
        return false;
    }
    gOmmMario->capture.lockTimer = -1;
    gOmmMario->capture.timer = 0xFF;
    m->actionTimer = 0;
    return true;
}

bool omm_mario_unlock(struct MarioState *m) {
    if (m->action != ACT_OMM_POSSESSION) {
        return false;
    }
    gOmmMario->capture.lockTimer = 0;
    return true;
}

bool omm_mario_is_locked(struct MarioState *m) {
    if (m->action != ACT_OMM_POSSESSION) {
        gOmmMario->capture.lockTimer = 0;
        return false;
    }
    return (gOmmMario->capture.lockTimer != 0);
}

static void omm_mario_cappy_update_object_and_gfx(struct MarioState *m) {
    struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
    struct Object *o = gOmmCapture;

    // Mario
    if (gOmmMario->capture.timer >= 20) {
        vec3f_copy(m->pos, &o->oPosX);
        vec3f_copy(m->vel, &o->oVelX);
        vec3s_set(m->faceAngle, -o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
        obj_scale(m->marioObj, 1.f);
        obj_anim_play(m->marioObj, MARIO_ANIM_A_POSE, 1.f);
        m->forwardVel = o->oForwardVel;
        m->squishTimer = 0;
        m->marioObj->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
    }
    obj_set_pos(m->marioObj, m->pos[0], m->pos[1], m->pos[2]);
    obj_set_angle(m->marioObj, m->faceAngle[0], m->faceAngle[1], m->faceAngle[2]);
    obj_update_gfx(m->marioObj);

    // Cappy
    if (cap) {
        Vec3f dv = {
            -gOmmObject->cappy.offset[0] * o->oGfxScale[0],
            +gOmmObject->cappy.offset[1] * o->oGfxScale[1],
            +gOmmObject->cappy.offset[2] * o->oGfxScale[0]
        };
        if (gOmmObject->cappy.copyGfx) {
            vec3f_rotate_zxy(dv, dv, o->oGfxAngle[0], o->oGfxAngle[1], o->oGfxAngle[2]);
            cap->oPosX           = o->oGfxPos[0] + dv[0];
            cap->oPosY           = o->oGfxPos[1] + dv[1];
            cap->oPosZ           = o->oGfxPos[2] + dv[2];
            cap->oFaceAnglePitch = o->oGfxAngle[0] + gOmmObject->cappy.angle[0];
            cap->oFaceAngleYaw   = o->oGfxAngle[1] + gOmmObject->cappy.angle[1];
            cap->oFaceAngleRoll  = o->oGfxAngle[2] + gOmmObject->cappy.angle[2];
        } else {
            vec3f_rotate_zxy(dv, dv, o->oFaceAnglePitch, o->oFaceAngleYaw, o->oFaceAngleRoll);
            cap->oPosX           = o->oPosX + dv[0];
            cap->oPosY           = o->oPosY + dv[1];
            cap->oPosZ           = o->oPosZ + dv[2];
            cap->oFaceAnglePitch = o->oFaceAnglePitch + gOmmObject->cappy.angle[0];
            cap->oFaceAngleYaw   = o->oFaceAngleYaw   + gOmmObject->cappy.angle[1];
            cap->oFaceAngleRoll  = o->oFaceAngleRoll  + gOmmObject->cappy.angle[2];
        }
        cap->oOpacity     = 0xFF;
        cap->oGfxPos[0]   = cap->oPosX;
        cap->oGfxPos[1]   = cap->oPosY + cap->oGraphYOffset;
        cap->oGfxPos[2]   = cap->oPosZ;
        cap->oGfxAngle[0] = cap->oFaceAnglePitch & 0xFFFF;
        cap->oGfxAngle[1] = cap->oFaceAngleYaw   & 0xFFFF;
        cap->oGfxAngle[2] = cap->oFaceAngleRoll  & 0xFFFF;
        cap->oGfxScale[0] = gOmmObject->cappy.scale * o->oGfxScale[0];
        cap->oGfxScale[1] = gOmmObject->cappy.scale * o->oGfxScale[1];
        cap->oGfxScale[2] = gOmmObject->cappy.scale * o->oGfxScale[2];
        cap->oNodeFlags   = GRAPH_RENDER_ACTIVE;
        cap->oNodeFlags  &= ~GRAPH_RENDER_INVISIBLE;
        cap->oNodeFlags  |= GRAPH_RENDER_INVISIBLE * ((o->oNodeFlags & GRAPH_RENDER_INVISIBLE) != 0);
    }
}

// Possession phases
// 10 frames / 1: Mario spins towards the object to possess
// 04 frames / 2: Mario stops, dive, rotates towards the ground and starts shrinking
// 06 frames / 3: while shrinking, Mario dives into the object

/*From, To, Coeff, Angle, Scale, Animation */
static const s32 sOmmCappyPossessAnimParams[20][6] = {

    // Phase 1
    { 0, 1,  19, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1,  36, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1,  51, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1,  64, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1,  75, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1,  84, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1,  91, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1,  96, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1,  99, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },
    { 0, 1, 100, 0x0000, 100, MARIO_ANIM_FORWARD_SPINNING },

    // Phase 2
    { 1, 1,   0, 0x0000, 100, MARIO_ANIM_DIVE },
    { 1, 1,   0, 0x1000,  95, MARIO_ANIM_DIVE },
    { 1, 1,   0, 0x2000,  90, MARIO_ANIM_DIVE },
    { 1, 1,   0, 0x3000,  85, MARIO_ANIM_DIVE },

    // Phase 3
    { 1, 2,   5, 0x4000,  80, MARIO_ANIM_DIVE },
    { 1, 2,  15, 0x4000,  70, MARIO_ANIM_DIVE },
    { 1, 2,  30, 0x4000,  60, MARIO_ANIM_DIVE },
    { 1, 2,  50, 0x4000,  50, MARIO_ANIM_DIVE },
    { 1, 2,  75, 0x4000,  40, MARIO_ANIM_DIVE },
    { 1, 2, 100, 0x4000,  30, MARIO_ANIM_DIVE },
};

#define xFrom(t)        (gOmmMario->capture.animPos[sOmmCappyPossessAnimParams[t][0]][0])
#define yFrom(t)        (gOmmMario->capture.animPos[sOmmCappyPossessAnimParams[t][0]][1])
#define zFrom(t)        (gOmmMario->capture.animPos[sOmmCappyPossessAnimParams[t][0]][2])
#define xTo(t)          (gOmmMario->capture.animPos[sOmmCappyPossessAnimParams[t][1]][0])
#define yTo(t)          (gOmmMario->capture.animPos[sOmmCappyPossessAnimParams[t][1]][1])
#define zTo(t)          (gOmmMario->capture.animPos[sOmmCappyPossessAnimParams[t][1]][2])
#define coeff(t)        ((f32) (sOmmCappyPossessAnimParams[t][2]) / 100.f)
#define angle(t)        ((s16) (sOmmCappyPossessAnimParams[t][3]))
#define scale(t)        ((f32) (sOmmCappyPossessAnimParams[t][4]) / 100.f)
#define anim(t)         (sOmmCappyPossessAnimParams[t][5])
#define lerp(t, a, b)   ((a) + ((b) - (a)) * (t));

s32 omm_act_possession(struct MarioState *m) {
    if (gOmmMario->capture.timer < 20) {

        // Play the possession animation for 20 frames
        if (gOmmMario->capture.timer == 0) {
            omm_sound_play(OMM_SOUND_EVENT_CAPTURE, m->marioObj->oCameraToObject);
            omm_cappy_unload();
        }
        s32 t = gOmmMario->capture.timer;
        m->pos[0] = lerp(coeff(t), xFrom(t), xTo(t));
        m->pos[1] = lerp(coeff(t), yFrom(t), yTo(t));
        m->pos[2] = lerp(coeff(t), zFrom(t), zTo(t));
        m->faceAngle[0] = angle(t);
        m->squishTimer = 0xFF;
        vec3f_set(m->marioObj->oGfxScale, scale(t), scale(t), scale(t));
        obj_anim_play(m->marioObj, anim(t), 1.f);
        m->particleFlags |= PARTICLE_SPARKLES;
        gOmmMario->capture.timer++;

    } else {

        // Spawn Cappy if not spawned
        if (!obj_get_first_with_behavior(bhvOmmPossessedObjectCap)) {
            spawn_object(m->marioObj, omm_player_graphics_get_selected_normal_cap(), bhvOmmPossessedObjectCap)->oFlags = 0;
            gOmmObject->state.invincTimer = 15;
        }

        // If Mario is locked, don't check [Z] press and zero-init inputs
        if (omm_mario_is_locked(m)) {
            gOmmMario->capture.stickX        = 0;
            gOmmMario->capture.stickY        = 0;
            gOmmMario->capture.stickMag      = 0;
            gOmmMario->capture.stickYaw      = 0;
            gOmmMario->capture.buttonPressed = 0;
            gOmmMario->capture.buttonDown    = 0;
            gOmmMario->capture.lockTimer    -= 1;

        } else {

            // Press [Z] to leave the object
            if (m->controller->buttonPressed & Z_TRIG) {
                if (omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_JUMP_OUT, false, 6)) {
                    return OMM_MARIO_ACTION_RESULT_CANCEL;
                }
            }

            // Update inputs
            gOmmMario->capture.stickX        = (m->controller->stickX / 64.f);
            gOmmMario->capture.stickY        = (m->controller->stickY / 64.f);
            gOmmMario->capture.stickMag      = (m->controller->stickMag / 64.f);
            gOmmMario->capture.stickYaw      = (m->intendedYaw);
            gOmmMario->capture.buttonPressed = (m->controller->buttonPressed);
            gOmmMario->capture.buttonDown    = (m->controller->buttonDown);
        }

        // Set possessed object position to Mario's position
        struct Object *o = gOmmCapture;
        o->oPosX = m->pos[0];
        o->oPosY = m->pos[1];
        o->oPosZ = m->pos[2];

        // Do not update the possessed object during the star grab animation
        if (gOmmMario->capture.timer == 0xFF) {
            omm_act_possession_update_star_dance(m);
            m->controller->buttonPressed &= ~START_BUTTON;
            gPlayer1Controller->buttonPressed &= ~START_BUTTON;
            m->actionTimer++;
        }
        
        // Update possessed object
        else {
            gOmmObject->cappy.copyGfx    = false;
            gOmmObject->cappy.offset[2]  = 0.f;
            gOmmObject->cappy.offset[0]  = 0.f;
            gOmmObject->cappy.offset[1]  = 0.f;
            gOmmObject->cappy.angle[0]   = 0;
            gOmmObject->cappy.angle[1]   = 0;
            gOmmObject->cappy.angle[2]   = 0;
            gOmmObject->cappy.scale      = 1.f;
            gOmmObject->state.properties = 0;
            gOmmObject->state.bullyTimer = max_s(gOmmObject->state.bullyTimer - 1, 0);
            while (true) {
                s32 res = omm_capture_update(o);
                if (res == 0) break;
                if (res == 1) return OMM_MARIO_ACTION_RESULT_CANCEL;
                if (res == 2) continue;
            }
            if (gOmmObject->state.invincTimer > 0) {
                if (gOmmObject->state.invincTimer & 1) {
                    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
                } else {
                    o->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                }
            }
        }

#if OMM_CODE_DEBUG
        gOmmMario->capture.hitboxRadius = o->hitboxRadius;
        gOmmMario->capture.hitboxHeight = o->hitboxHeight;
        gOmmMario->capture.hitboxOffset = o->hitboxDownOffset;
        gOmmMario->capture.hitboxWall   = o->oWallHitboxRadius;
#endif

        // Unset possessed object's hitbox to avoid undesirable interactions
        o->hitboxRadius      = 0;
        o->hitboxHeight      = 0;
        o->hurtboxRadius     = 0;
        o->hurtboxHeight     = 0;
        o->hitboxDownOffset  = 0;
        o->oWallHitboxRadius = 0;
    }

    // Unpossess if Mario is dead
    if (omm_mario_is_dead(m)) {
        omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, false, 0);
        return OMM_MARIO_ACTION_RESULT_CANCEL;
    }

    // Update Mario and Cappy's objects and graphics
    omm_mario_cappy_update_object_and_gfx(m);
    return OMM_MARIO_ACTION_RESULT_BREAK;
}

#undef xFrom
#undef yFrom
#undef zFrom
#undef xTo
#undef yTo
#undef zTo
#undef coeff
#undef angle
#undef scale
#undef anim
#undef lerp

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

    // Not tangible
    if ((possessFlags & OMM_MARIO_POSSESS_CHECK_TANGIBILITY) && o->oIntangibleTimer != 0) {
        return false;
    }

    // Oof
    if (omm_mario_is_dead(m) || m->hurtCounter != 0) {
        return false;
    }

    // Mario is already possessing an object, duh
    if (m->action == ACT_OMM_POSSESSION) {
        return false;
    }

    // Try to capture
    const void *data = omm_behavior_data_get_capture(o->behavior);
    if (!data) {
        return false;
    }

    // Try to init
    gOmmData->resetObject();
    gOmmMario->capture.data = data;
    if (!omm_capture_init(o)) {
        return false;
    }

    // OK
    copy_object_data(o, false);
    o->curBhvCommand = bhvOmmPossessedObject;
    o->oRoom = -1;
    o->oFlags = 0;
    o->oIntangibleTimer = 0;
    o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    gOmmMario->capture.obj = o;
    gOmmMario->capture.timer = 0;
    gOmmMario->capture.lockTimer = 0;
    struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
    if (cap) obj_mark_for_deletion(cap);

    // Init animation positions
    f32 x = o->oPosX;
    f32 y = o->oPosY + omm_capture_get_top(o);
    f32 z = o->oPosZ;
    gOmmMario->capture.animPos[0][0] = m->pos[0];
    gOmmMario->capture.animPos[0][1] = m->pos[1];
    gOmmMario->capture.animPos[0][2] = m->pos[2];
    gOmmMario->capture.animPos[1][0] = x;
    gOmmMario->capture.animPos[1][1] = y + 180.f;
    gOmmMario->capture.animPos[1][2] = z;
    gOmmMario->capture.animPos[2][0] = x;
    gOmmMario->capture.animPos[2][1] = y - 30.f;
    gOmmMario->capture.animPos[2][2] = z;

    // Init face angles
    m->faceAngle[0] = 0;
    m->faceAngle[1] = mario_obj_angle_to_object(m, o);
    m->faceAngle[2] = 0;

    // Possess
    omm_capture_reset_camera();
    mario_stop_riding_and_holding(m);
    omm_mario_set_action(m, ACT_OMM_POSSESSION, 0, Z_TRIG);
    return true;
}

//
// Unpossess
//

/* action, actionArg, forward vel, non-zero vertical vel, come from top, invert face yaw, i-frames */
static const s32 sOmmMarioUnpossessActions[OMM_MARIO_UNPOSSESS_ACT_COUNT * 6][7] = {

    /* MARIO_UNPOSSESS_ACT_NONE */
    /* GF */ { ACT_IDLE,                    0,   0,  0, false, false, true },
    /* GB */ { ACT_IDLE,                    0,   0,  0, false, false, true },
    /* AF */ { ACT_FREEFALL,                0,   0,  0, false, false, true },
    /* AB */ { ACT_FREEFALL,                0,   0,  0, false, false, true },
    /* WF */ { ACT_WATER_IDLE,              0,   0,  0, false, false, true },
    /* WB */ { ACT_WATER_IDLE,              0,   0,  0, false, false, true },

    /* MARIO_UNPOSSESS_ACT_JUMP_OUT */
    /* GF */ { ACT_OMM_LEAVE_OBJECT_JUMP,   0,  18, 42, true,  false, true },
    /* GB */ { ACT_OMM_LEAVE_OBJECT_JUMP,   0,  18, 42, true,  true,  true },
    /* AF */ { ACT_OMM_LEAVE_OBJECT_JUMP,   0,  18, 42, true,  false, true },
    /* AB */ { ACT_OMM_LEAVE_OBJECT_JUMP,   0,  18, 42, true,  true,  true },
    /* WF */ { ACT_OMM_LEAVE_OBJECT_WATER,  0,   0, 20, true,  false, true },
    /* WB */ { ACT_OMM_LEAVE_OBJECT_WATER,  0,   0, 20, true,  true,  true },

    /* MARIO_UNPOSSESS_ACT_KNOCKED_BACK */
    /* GF */ { ACT_HARD_FORWARD_GROUND_KB,  1,  28,  0, false, true,  false },
    /* GB */ { ACT_HARD_BACKWARD_GROUND_KB, 1, -28,  0, false, false, false },
    /* AF */ { ACT_HARD_FORWARD_AIR_KB,     1,  28,  0, false, true,  false },
    /* AB */ { ACT_HARD_BACKWARD_AIR_KB,    1, -28,  0, false, false, false },
    /* WF */ { ACT_FORWARD_WATER_KB,        1,  28,  0, false, true,  false },
    /* WB */ { ACT_BACKWARD_WATER_KB,       1, -28,  0, false, false, false },

    /* MARIO_UNPOSSESS_ACT_BURNT */
    /* GF */ { ACT_BURNING_JUMP,            1,  12, 34, true,  false, false },
    /* GB */ { ACT_BURNING_JUMP,            1,  12, 34, true,  false, false },
    /* AF */ { ACT_BURNING_JUMP,            1,  12, 34, true,  false, false },
    /* AB */ { ACT_BURNING_JUMP,            1,  12, 34, true,  false, false },
    /* WF */ { ACT_BURNING_JUMP,            1,  12, 34, true,  false, false },
    /* WB */ { ACT_BURNING_JUMP,            1,  12, 34, true,  false, false },

    /* MARIO_UNPOSSESS_ACT_GRABBED */
    /* GF */ { ACT_GRABBED,                 0,   0,  0, false, false, false },
    /* GB */ { ACT_GRABBED,                 0,   0,  0, false, false, false },
    /* AF */ { ACT_GRABBED,                 0,   0,  0, false, false, false },
    /* AB */ { ACT_GRABBED,                 0,   0,  0, false, false, false },
    /* WF */ { ACT_GRABBED,                 0,   0,  0, false, false, false },
    /* WB */ { ACT_GRABBED,                 0,   0,  0, false, false, false },

    /* MARIO_UNPOSSESS_ACT_EATEN */
    /* GF */ { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false, false },
    /* GB */ { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false, false },
    /* AF */ { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false, false },
    /* AB */ { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false, false },
    /* WF */ { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false, false },
    /* WB */ { ACT_EATEN_BY_BUBBA,          0,   0,  0, false, false, false },

    /* MARIO_UNPOSSESS_ACT_BLOWN */
    /* GF */ { ACT_GETTING_BLOWN,           0, -24, 12, false, false, false },
    /* GB */ { ACT_GETTING_BLOWN,           0, -24, 12, false, false, false },
    /* AF */ { ACT_GETTING_BLOWN,           0, -24, 12, false, false, false },
    /* AB */ { ACT_GETTING_BLOWN,           0, -24, 12, false, false, false },
    /* WF */ { ACT_GETTING_BLOWN,           0, -24, 12, false, false, false },
    /* WB */ { ACT_GETTING_BLOWN,           0, -24, 12, false, false, false },

    /* MARIO_UNPOSSESS_ACT_TORNADO */
    /* GF */ { ACT_TORNADO_TWIRLING,        0,   0,  0, false, false, false },
    /* GB */ { ACT_TORNADO_TWIRLING,        0,   0,  0, false, false, false },
    /* AF */ { ACT_TORNADO_TWIRLING,        0,   0,  0, false, false, false },
    /* AB */ { ACT_TORNADO_TWIRLING,        0,   0,  0, false, false, false },
    /* WF */ { ACT_TORNADO_TWIRLING,        0,   0,  0, false, false, false },
    /* WB */ { ACT_TORNADO_TWIRLING,        0,   0,  0, false, false, false },
};

bool omm_mario_unpossess_object(struct MarioState *m, u8 unpossessAct, bool isBackwards, u32 objIntangibleFrames) {
    struct Object *o = gOmmCapture;
    if (!o) {
        return false;
    }

    // Unpossess action
    o->oFloorHeight  = find_floor(o->oPosX, o->oPosY, o->oPosZ, &o->oFloor);
    bool onGround    = obj_is_on_ground(o);
    f32 objectTop    = omm_capture_get_top(o);
    s32 terrainType  = (((o->oPosY + objectTop) < find_water_level(o->oPosX, o->oPosZ)) ? 2 : (onGround ? 0 : 1));
    s32 actionIndex  = (unpossessAct * 6) + (terrainType * 2) + isBackwards;
    u32 action       = (u32)  sOmmMarioUnpossessActions[actionIndex][0];
    u32 actionArg    = (u32)  sOmmMarioUnpossessActions[actionIndex][1];
    f32 forwardVel   = (f32)  sOmmMarioUnpossessActions[actionIndex][2];
    f32 verticalVel  = (f32)  sOmmMarioUnpossessActions[actionIndex][3];
    bool comeFromTop = (bool) sOmmMarioUnpossessActions[actionIndex][4];
    bool invFaceYaw  = (bool) sOmmMarioUnpossessActions[actionIndex][5];
    bool invFrames   = (bool) sOmmMarioUnpossessActions[actionIndex][6];

    omm_mario_cappy_update_object_and_gfx(m);
    mario_set_forward_vel(m, forwardVel);
    if (verticalVel != 0.f) m->vel[1] = verticalVel;
    m->pos[1] += (comeFromTop * objectTop);
    m->faceAngle[1] += (invFaceYaw * 0x8000);
    m->input &= ~(INPUT_FIRST_PERSON | INPUT_A_PRESSED | INPUT_A_DOWN | INPUT_B_PRESSED | INPUT_Z_DOWN | INPUT_Z_PRESSED);
    m->controller->buttonPressed = 0;
    m->marioObj->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
    omm_mario_set_action(m, action, actionArg, 0);
    if (invFrames) m->invincTimer = max_s(15, m->invincTimer);

    // Unload cap, restore camera
    omm_sound_play(OMM_SOUND_EVENT_UNCAPTURE, m->marioObj->oCameraToObject);
    obj_spawn_white_puff(m->marioObj, -1);
    omm_capture_reset_camera();
    struct Object *cap = obj_get_first_with_behavior(bhvOmmPossessedObjectCap);
    if (cap) obj_mark_for_deletion(cap);

    // End object possession
    copy_object_data(o, true);
    omm_capture_end(o);
    o->oRoom = -1;
    o->oIntangibleTimer = objIntangibleFrames;
    o->oNodeFlags |= GRAPH_RENDER_ACTIVE;
    o->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;

    // Clear fields, reset Cappy bounce
    gOmmMario->capture.obj       = NULL;
    gOmmMario->capture.prev      = ((onGround || !omm_capture_should_reference_object(o)) ? NULL : o);
    gOmmMario->capture.timer     = 0;
    gOmmMario->capture.lockTimer = 0;
    gOmmMario->cappy.bounced     = false;
    return true;
}
