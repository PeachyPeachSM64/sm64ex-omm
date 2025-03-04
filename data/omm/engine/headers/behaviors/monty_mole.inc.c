#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

s32 sMontyMoleKillStreak;

//
// Monty mole hole
//

static struct Object *monty_mole_hole_get_mole(struct Object *hole) {
    for_each_object_with_behavior(mole, bhvMontyMole) {
        if (mole->oMontyMoleCurrentHole == hole) {
            return mole;
        }
    }
    return NULL;
}

static struct Object *monty_mole_select_available_hole(f32 minDistToMario, f32 maxDistToMario) {
    for (s32 selectedHole = random_u16() % obj_get_count_with_behavior(bhvMontyMoleHole), noAvailableHole = FALSE; !noAvailableHole;) {
        noAvailableHole = TRUE;
        for_each_object_with_behavior(hole, bhvMontyMoleHole) {
            if (hole->oMontyMoleHoleCooldown == 0 && monty_mole_hole_get_mole(hole) == NULL && minDistToMario < hole->oDistanceToMario && hole->oDistanceToMario < maxDistToMario) {
                noAvailableHole = FALSE;
                if (selectedHole-- == 0) {
                    return hole;
                }
            }
        }
    }
    return NULL;
}

void bhv_monty_mole_hole_init(void) {
    sMontyMoleKillStreak = 0;
}

void bhv_monty_mole_hole_update(void) {
    if (o->oTimer == 0) {
        bhv_monty_mole_hole_init();
    }
    o->oMontyMoleHoleCooldown = max_s(o->oMontyMoleHoleCooldown - 1, 0);
}

//
// Monty mole
//

void monty_mole_spawn_dirt_particles(s8 offsetY, s8 velYBase) {
    static struct SpawnParticlesInfo sMontyMoleRiseFromGroundParticles = { 0, 3, MODEL_SAND_DUST, 0, 4, 4, 10, 15, -4, 0, 10, 7 };
    sMontyMoleRiseFromGroundParticles.offsetY = offsetY;
    sMontyMoleRiseFromGroundParticles.velYBase = velYBase;
    cur_obj_spawn_particles(&sMontyMoleRiseFromGroundParticles);
}

static void monty_mole_act_select_hole(void) {
    cur_obj_hide();
    o->oInteractStatus = 0;

    // Select a hole to pop out of
    f32 minDistToMario = (o->oBehParams2ndByte != MONTY_MOLE_BP_NO_ROCK ? 200 : (gMarioState->forwardVel < 8 ? 100 : 500));
    o->oMontyMoleCurrentHole = monty_mole_select_available_hole(minDistToMario, 1500);
    if (o->oMontyMoleCurrentHole) {
        cur_obj_play_sound_2(SOUND_OBJ2_MONTY_MOLE_APPEAR);

        // Position at hole
        o->oPosX = o->oMontyMoleCurrentHole->oPosX;
        o->oPosY = o->oFloorHeight = o->oMontyMoleCurrentHole->oPosY;
        o->oPosZ = o->oMontyMoleCurrentHole->oPosZ;
        o->oFaceAnglePitch = 0;
        o->oMoveAngleYaw = o->oMontyMoleCurrentHole->oAngleToMario;

        // Select next action
        if (o->oDistanceToMario > 500 || minDistToMario > 100 || random_sign() < 0) {
            o->oAction = MONTY_MOLE_ACT_RISE_FROM_HOLE;
            o->oVelY = 3;
            o->oGravity = 0;
            monty_mole_spawn_dirt_particles(0, 10);
        } else {
            o->oAction = MONTY_MOLE_ACT_JUMP_OUT_OF_HOLE;
            o->oVelY = 50;
            o->oGravity = -4;
            monty_mole_spawn_dirt_particles(0, 20);
        }
        cur_obj_unhide();
        cur_obj_become_tangible();
    }
}

static void monty_mole_act_rise_from_hole(void) {
    cur_obj_init_animation_with_sound(1);
    if (o->oMontyMoleHeightRelativeToFloor >= 49) {
        o->oPosY = o->oFloorHeight + 50;
        o->oVelY = 0;
        if (cur_obj_check_if_near_animation_end()) {
            o->oAction = MONTY_MOLE_ACT_SPAWN_ROCK;
        }
    }
}

static void monty_mole_act_spawn_rock(void) {
    struct Object *rock;
    if (cur_obj_init_anim_and_check_if_end(2)) {
        if (o->oBehParams2ndByte != MONTY_MOLE_BP_NO_ROCK && abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw) < 0x4000 && (rock = spawn_object(o, MODEL_PEBBLE, bhvMontyMoleRock)) != NULL) {
            o->prevObj = rock;
            o->oAction = MONTY_MOLE_ACT_THROW_ROCK;
        } else {
            o->oAction = MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE;
        }
    }
}

static void monty_mole_act_begin_jump_into_hole(void) {
    if (cur_obj_init_anim_and_check_if_end(3) || obj_is_near_to_and_facing_mario(1000, 0x4000)) {
        o->oAction = MONTY_MOLE_ACT_JUMP_INTO_HOLE;
        o->oVelY = 40;
        o->oGravity = -6;
    }
}

static void monty_mole_act_throw_rock(void) {
    if (cur_obj_init_anim_check_frame(8, 10)) {
        cur_obj_play_sound_2(SOUND_OBJ_MONTY_MOLE_ATTACK);
        o->prevObj = NULL;
    }
    if (cur_obj_check_if_near_animation_end()) {
        o->oAction = MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE;
    }
}

static void monty_mole_act_jump_into_hole(void) {
    cur_obj_init_anim_extend(0);
    o->oFaceAnglePitch = -atan2s(o->oVelY, -4);
    if (o->oVelY < 0 && o->oMontyMoleHeightRelativeToFloor < 120) {
        o->oAction = MONTY_MOLE_ACT_HIDE;
        o->oGravity = 0;
        monty_mole_spawn_dirt_particles(-80, 15);
    }
}

static void monty_mole_hide_in_hole(void) {
    if (o->oMontyMoleCurrentHole) {
        o->oMontyMoleCurrentHole->oMontyMoleHoleCooldown = 30;
    }
    o->oAction = MONTY_MOLE_ACT_SELECT_HOLE;
    o->oVelY = 0;
    cur_obj_become_intangible();
}

static void monty_mole_act_hide(void) {
    cur_obj_init_animation_with_sound(1);
    if (o->oPosY <= o->oFloorHeight) {
        cur_obj_hide();
        monty_mole_hide_in_hole();
    } else {
        approach_f32_ptr(&o->oVelY, -4, 0.5f);
    }
}

static void monty_mole_act_jump_out_of_hole(void) {
    if (o->oVelY > 0) {
        cur_obj_init_animation_with_sound(9);
    } else {
        cur_obj_init_anim_extend(4);
        if (o->oMontyMoleHeightRelativeToFloor < 50) {
            o->oPosY = o->oFloorHeight + 50;
            o->oVelY = o->oGravity = 0;
            o->oAction = MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE;
        }
    }
}

void bhv_monty_mole_init(void) {
    o->oMontyMoleCurrentHole = NULL;
}

void bhv_monty_mole_update(void) {
    o->oDeathSound = SOUND_OBJ_DYING_ENEMY1;
    cur_obj_update_floor_and_walls();
    o->oMontyMoleHeightRelativeToFloor = o->oPosY - o->oFloorHeight;

    // Actions
    switch (o->oAction) {
        case MONTY_MOLE_ACT_SELECT_HOLE: monty_mole_act_select_hole(); break;
        case MONTY_MOLE_ACT_RISE_FROM_HOLE: monty_mole_act_rise_from_hole(); break;
        case MONTY_MOLE_ACT_SPAWN_ROCK: monty_mole_act_spawn_rock(); break;
        case MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE: monty_mole_act_begin_jump_into_hole(); break;
        case MONTY_MOLE_ACT_THROW_ROCK: monty_mole_act_throw_rock(); break;
        case MONTY_MOLE_ACT_JUMP_INTO_HOLE: monty_mole_act_jump_into_hole(); break;
        case MONTY_MOLE_ACT_HIDE: monty_mole_act_hide(); break;
        case MONTY_MOLE_ACT_JUMP_OUT_OF_HOLE: monty_mole_act_jump_out_of_hole(); break;
    }

    // Spawn a 1-up if Mario kills 8 monty moles
    static struct ObjectHitbox sMontyMoleHitbox = { INTERACT_BOUNCE_TOP, 0, 2, -1, 0, 70, 50, 30, 40 };
    if (obj_check_attacks(&sMontyMoleHitbox, o->oAction)) {
        if (++sMontyMoleKillStreak == 8) {
            audio_play_puzzle_jingle();
            spawn_object(o, MODEL_1UP, bhv1upWalking);
        }
        monty_mole_hide_in_hole();
        o->prevObj = NULL;
    }

    // Move mole, ignore water
    o->oMoveFlags &= ~OBJ_MOVE_MASK_IN_WATER;
    cur_obj_move_standard(78);
    o->oMoveFlags &= ~OBJ_MOVE_MASK_IN_WATER;
}

//
// Monty mole rock
//

static void monty_mole_rock_act_held(void) {
    o->oParentRelativePosX = 80;
    o->oParentRelativePosY = -50;
    o->oParentRelativePosZ = 0;
    if (!o->parentObj->prevObj) {
        f32 distToMario = min_f(o->oDistanceToMario, 600);
        o->oAction = MONTY_MOLE_ROCK_ACT_MOVE;
        o->oMoveAngleYaw = (s32) (o->parentObj->oMoveAngleYaw + 0x1F4 - distToMario * 0.1f);
        o->oForwardVel = 40;
        o->oVelY = 8 + distToMario * 0.08f;
        o->oMoveFlags = 0;
    }
}

static void monty_mole_rock_act_move(void) {
    static struct SpawnParticlesInfo sMontyMoleRockBreakParticles = { 0, 2, MODEL_PEBBLE, 10, 4, 4, 10, 15, -4, 0, 8, 4 };
    cur_obj_update_floor_and_walls();
    cur_obj_move_standard(78);
    if (o->oTimer > 150 || o->oPosY <= o->oFloorHeight) {
        cur_obj_spawn_particles(&sMontyMoleRockBreakParticles);
        obj_mark_for_deletion(o);
    }
}

void bhv_monty_mole_rock_update(void) {
    static struct ObjectHitbox sMontyMoleRockHitbox = { INTERACT_MR_BLIZZARD, 15, 1, 99, 0, 30, 15, 30, 15 };
    obj_check_attacks(&sMontyMoleRockHitbox, o->oAction);
    switch (o->oAction) {
        case MONTY_MOLE_ROCK_ACT_HELD: monty_mole_rock_act_held(); break;
        case MONTY_MOLE_ROCK_ACT_MOVE: monty_mole_rock_act_move(); break;
    }
}
