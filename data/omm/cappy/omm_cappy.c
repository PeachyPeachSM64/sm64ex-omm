#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Get
//

struct Object *omm_cappy_get_object() {
    if (gOmmCappy && gOmmCappy->activeFlags && gOmmCappy->behavior == omm_bhv_cappy) {
        return gOmmCappy;
    }
    return NULL;
}

struct Object *omm_cappy_get_object_play_as() {
    struct Object *cappy = omm_cappy_get_object();
    if (cappy && (cappy->oCappyFlags & CAPPY_FLAG_PLAY_AS)) {
        return cappy;
    }
    return NULL;
}

//
// Spawn
//

static s16 omm_cappy_get_behavior(struct MarioState *m) {
    bool air = (m->action & (ACT_FLAG_AIR | ACT_FLAG_SWIMMING)) != 0;
    if (m->action == ACT_FLYING)                 return OMM_CAPPY_BHV_FLYING;
    if (m->controller->buttonDown & U_JPAD)      return OMM_CAPPY_BHV_UPWARDS_GROUND + air;
    if (m->controller->buttonDown & D_JPAD)      return OMM_CAPPY_BHV_DOWNWARDS_GROUND + air;
    if (m->controller->buttonDown & L_JPAD)      return OMM_CAPPY_BHV_SPIN_GROUND + air;
    if (m->controller->buttonDown & R_JPAD)      return OMM_CAPPY_BHV_SPIN_GROUND + air;
    if (m->controller->buttonPressed & A_BUTTON) return OMM_CAPPY_BHV_UPWARDS_GROUND + air;
    if (omm_mario_is_ground_pound_landing(m))    return OMM_CAPPY_BHV_DOWNWARDS_GROUND;
    if (gOmmData->mario->spin.timer != 0)        return OMM_CAPPY_BHV_SPIN_GROUND + air;
    else                                         return OMM_CAPPY_BHV_DEFAULT_GROUND + air;
}

struct Object *omm_cappy_spawn(struct MarioState *m) {

    // Cappy must be available
    if (OMM_CAP_CLASSIC || !(m->flags & MARIO_CAP_ON_HEAD) || omm_cappy_get_object()) {
        return NULL;
    }

    // Cap flags
    bool wing = ((m->flags & MARIO_WING_CAP) != 0);
    bool metal = ((m->flags & MARIO_METAL_CAP) != 0);

    // Spawn Cappy, but set it dormant
    struct Object *cappy       = spawn_object(m->marioObj, omm_player_get_selected_cap(wing, metal), omm_bhv_cappy);
    cappy->oCappyBehavior      = omm_cappy_get_behavior(m);
    cappy->oCappyLifeTimer     = -255;
    cappy->oCappyFlags         = CAPPY_FLAG_START_ANIM;
    cappy->oCappyThrowStrength = 0;
    cappy->oIntangibleTimer    = 0;
    obj_set_dormant(cappy, true);
    gOmmCappy = cappy;
    return cappy;
}

//
// Update
//

extern void omm_cappy_update_mario_anim(struct Object *cappy, struct MarioState *m);
extern void omm_cappy_init_behavior(struct Object *cappy, struct MarioState *m);
extern void omm_cappy_update_behavior(struct Object *cappy, struct MarioState *m);
extern void omm_cappy_process_interactions(struct Object *cappy, struct MarioState *m);
extern bool omm_cappy_perform_step_return_to_mario(struct Object *cappy, struct MarioState *m);

void omm_cappy_update(struct MarioState *m) {
    static bool sReturnedToMario = false;
    struct Object *cappy = omm_cappy_get_object();

    // Update Mario's throw animation (the Cappy object can be NULL)
    omm_cappy_update_mario_anim(cappy, m);

    // Unloads Cappy if he returned to Mario the previous frame
    if (sReturnedToMario) {
        sReturnedToMario = false;
        omm_cappy_unload();
        return;
    }

    // Update Cappy if it exists
    if (cappy) {

        // Unload Cappy if Mario loses his cap
        if (!(m->flags & MARIO_CAP_ON_HEAD)) {
            omm_cappy_unload();
            return;
        }

        // Set Cappy in a dormant state if its timer is below 0
        bool awaken = (cappy->oCappyLifeTimer >= 0);
        obj_set_dormant(cappy, !awaken);

        // Init Cappy
        if (cappy->oCappyLifeTimer == 0) {
            omm_cappy_init_behavior(cappy, m);
            cappy->oCappyYaw = m->faceAngle[1];
        }

        // Update Cappy's hitbox
        if (awaken) {
            cappy->hitboxRadius = CAPPY_HITBOX_RADIUS;
            cappy->hitboxHeight = CAPPY_HITBOX_HEIGHT;
            cappy->hitboxDownOffset = CAPPY_HITBOX_OFFSET;
            cappy->oWallHitboxRadius = CAPPY_WALL_RADIUS;
        }

        // Update Cappy's behavior
        // Does not update during Time Stop
        bool advance = !(gTimeStopState & TIME_STOP_ENABLED);
        if (awaken && advance) {
            if (cappy->oCappyLifeTimer < CAPPY_LIFETIME) {
                omm_cappy_update_behavior(cappy, m);
                omm_cappy_process_interactions(cappy, m);
            } else if (omm_cappy_perform_step_return_to_mario(cappy, m)) {
                sReturnedToMario = true;
            }
            cappy->oGfxAngle[1] += CAPPY_GFX_ANGLE_VEL;
        }

        // Update Cappy's Gfx
        if (awaken) {
            cappy->oGfxPos[0] = cappy->oPosX;
            cappy->oGfxPos[1] = cappy->oPosY;
            cappy->oGfxPos[2] = cappy->oPosZ;
            cappy->oGfxAngle[0] = 0;
            cappy->oGfxAngle[2] = 0;
            cappy->oGfxScale[0] = CAPPY_GFX_SCALE_X;
            cappy->oGfxScale[1] = CAPPY_GFX_SCALE_Y;
            cappy->oGfxScale[2] = CAPPY_GFX_SCALE_Z;
            cappy->oNodeFlags |= GRAPH_RENDER_ACTIVE;
            cappy->oNodeFlags &= ~GRAPH_RENDER_INVISIBLE;
            cappy->oOpacity = (m->flags & MARIO_VANISH_CAP ? 0x80 : 0xFF);
            spawn_object(cappy, MODEL_NONE, bhvSparkleSpawn);
        }

        // Update Mario's cap state
        if (awaken) {
            m->marioBodyState->capState = MARIO_HAS_DEFAULT_CAP_OFF;
        }

        // Advance Cappy's life timer
        cappy->oCappyThrowStrength += advance * ((cappy->oCappyLifeTimer < 0) && (m->controller->buttonDown & X_BUTTON));
        cappy->oCappyThrowStrength = clamp_s(cappy->oCappyThrowStrength, 4 * (m->controller->stickMag > 60.f), 4);
        cappy->oCappyLifeTimer += advance;
    }
}

//
// Unload
//

void omm_cappy_unload() {
    struct Object *cappy = omm_cappy_get_object();
    if (cappy) {
        obj_mark_for_deletion(cappy);
    }
}
