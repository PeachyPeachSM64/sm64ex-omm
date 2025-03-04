#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

void bhv_chain_chomp_update_chain_parts(struct Object *o, bool isFreed) {
    struct Object *pivot = o->parentObj;

    // Segment 0 connects the pivot to the chain chomp itself
    struct ChainSegment *seg0 = &o->oChainChompSegments[0];
    seg0->posX = o->oPosX - pivot->oPosX;
    seg0->posY = o->oPosY - pivot->oPosY;
    seg0->posZ = o->oPosZ - pivot->oPosZ;
    o->oChainChompDistToPivot = sqrtf(sqr_f(seg0->posX) + sqr_f(seg0->posY) + sqr_f(seg0->posZ));

    // If the chain is fully stretched
    f32 maxDistToPivot = o->oChainChompMaxDistFromPivotPerChainPart * 5.f;
    if (o->oChainChompDistToPivot > maxDistToPivot) {
        f32 ratio = maxDistToPivot / o->oChainChompDistToPivot;
        o->oChainChompDistToPivot = maxDistToPivot;
        seg0->posX *= ratio;
        seg0->posY *= ratio;
        seg0->posZ *= ratio;

        // If freed, move pivot like the chain chomp is pulling it along
        if (isFreed) {
            pivot->oVelY -= 4.f;
            pivot->oPosX = o->oPosX - seg0->posX;
            pivot->oPosZ = o->oPosZ - seg0->posZ;
            pivot->oPosY += pivot->oVelY;
            struct Surface *floor = NULL;
            f32 floorY = find_floor(pivot->oPosX, pivot->oPosY, pivot->oPosZ, &floor);
            if (floor) {
                if (pivot->oPosY <= floorY) {
                    pivot->oPosY = floorY;
                    pivot->oVelY = 0.f;
                }
            } else {
                pivot->oPosY = o->oPosY;
                pivot->oVelY = 0.f;
            }
        }
    }

    // Segments 1+ connect the pivot to chain part i
    // 1 is closest to the chain chomp
    // 4 is closest to the pivot
    for (s32 i = 1; i <= 4; i++) {
        struct ChainSegment *prevSegment = &o->oChainChompSegments[i - 1];
        struct ChainSegment *segment = &o->oChainChompSegments[i];

        // Apply gravity (using pitch for velY)
        segment->pitch -= 4.f;
        segment->posY += segment->pitch;
        f32 sx = segment->posX + pivot->oPosX;
        f32 sy = segment->posY + pivot->oPosY;
        f32 sz = segment->posZ + pivot->oPosZ;
        struct Surface *floor = NULL;
        f32 floorY = find_floor(sx, sy, sz, &floor);
        if (floor) {
            if (sy <= floorY) {
                segment->posY = floorY - pivot->oPosY;
                segment->pitch = 0;
            }
        } else {
            floorY = find_floor(sx, pivot->oPosY, sz, &floor);
            if (floor) {
                if (sy <= floorY) {
                    segment->posY = floorY - pivot->oPosY;
                    segment->pitch = 0;
                }
            } else {
                segment->posY = 0;
                segment->pitch = 0;
            }
        }

        // Distance to previous chain part (so that the tail follows the chomp)
        f32 offsetX = segment->posX - prevSegment->posX;
        f32 offsetY = segment->posY - prevSegment->posY;
        f32 offsetZ = segment->posZ - prevSegment->posZ;
        f32 offset = sqrtf(sqr_f(offsetX) + sqr_f(offsetY) + sqr_f(offsetZ));
        if (offset > o->oChainChompMaxDistFromPivotPerChainPart) {
            offset = o->oChainChompMaxDistFromPivotPerChainPart / offset;
            offsetX *= offset;
            offsetY *= offset;
            offsetZ *= offset;
        }

        // Distance to pivot (so that it stretches when the chomp moves far from the wooden post)
        offsetX += prevSegment->posX;
        offsetY += prevSegment->posY;
        offsetZ += prevSegment->posZ;
        offset = sqrtf(sqr_f(offsetX) + sqr_f(offsetY) + sqr_f(offsetZ));
        f32 maxTotalOffset = o->oChainChompMaxDistFromPivotPerChainPart * (5 - i);
        if (offset > maxTotalOffset) {
            offset = maxTotalOffset / offset;
            offsetX *= offset;
            offsetY *= offset;
            offsetZ *= offset;
        }

        // Segment final pos
        segment->posX = offsetX;
        segment->posY = offsetY;
        segment->posZ = offsetZ;
    }
}

//
// Behavior
//

static void bhv_omm_chain_chomp_free_init() {
    struct Object *o = gCurrentObject;
    o->oFlags &= (OBJ_FLAG_GFX_INITED | OBJ_FLAG_UPDATE_AREA_INDEX | OBJ_FLAG_MONEYBAG_COIN_INTERACTED | OBJ_FLAG_SPARKLY_NOT_ENEMY);
    o->oFlags |= (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE);
    o->oGraphYOffset = 240.f;
    o->oWallHitboxRadius = 120.f;
    o->oGravity = -4.f;
    o->oBounciness = -0.5f;
    o->oFriction = 10.f;
    o->oBuoyancy = 2.f;
    o->oChainChompMaxDistFromPivotPerChainPart = 750.f / 5.f;
    o->oChainChompFreeState = 0;
    o->oChainChompFreeTimer = 0;
    o->oChainChompFreeAngle = 0;
    obj_scale(o, 2.f);
    cur_obj_set_pos_to_home();
    obj_set_params(o, INTERACT_DAMAGE, 4, 99, 0, true);

    // Spawn the chain parts if they don't exist yet
    if (!o->oChainChompSegments) {
        struct ChainSegment *segments = mem_pool_alloc(gObjectMemoryPool, 5 * sizeof(struct ChainSegment));
        if (segments != NULL) {

            // Init segments
            o->oChainChompSegments = segments;
            for (s32 i = 0; i != 5; ++i) {
                chain_segment_init(&segments[i]);
            }

            // Spawn the pivot
            o->parentObj = spawn_object(o, MODEL_NONE, bhvChainChompChainPart);
            if (o->parentObj) {

                // Spawn the other chain parts
                for (s32 i = 1; i != 5; ++i) {
                    spawn_object_relative(i, 0, 0, 0, o, MODEL_METALLIC_BALL, bhvChainChompChainPart);
                }
                return;
            }
        }
        obj_mark_for_deletion(o);
    }
}

static void bhv_omm_chain_chomp_free_wander(struct Object* o) {
    if (o->oChainChompFreeTimer > 0) {
        o->oVelX *= 0.90f;
        o->oVelZ *= 0.90f;
        o->oChainChompFreeTimer--;
        return;
    }

    if (obj_is_on_ground(o)) {

        // Start chasing Mario
        if (o->oDistanceToMario <= 1000.f) {
            o->oVelX = 0;
            o->oVelY = 48.f;
            o->oVelZ = 0;
            o->oForwardVel = 0;
            o->oFaceAngleYaw = o->oAngleToMario;
            o->oMoveAngleYaw = o->oAngleToMario;
            o->oFloor = NULL;
            o->oChainChompFreeTimer = 45;
            o->oChainChompFreeState = 1;
            obj_play_sound(o, SOUND_GENERAL_CHAIN_CHOMP2);
        }

        // Make a small hop
        else {
            s16 nextAngle = (s16)(o->oChainChompFreeAngle + (random_u16() & 0x3FFF) - 0x2000);
            obj_set_forward_vel(o, nextAngle, 1.f, 12.f);
            o->oVelY = 36.f;
            o->oFloor = NULL;
            o->oFaceAngleYaw = nextAngle;
            o->oMoveAngleYaw = nextAngle;
            o->oChainChompFreeAngle = nextAngle;
            obj_play_sound(o, SOUND_GENERAL_CHAIN_CHOMP1);
        }
    }
}

static void bhv_omm_chain_chomp_free_chase(struct Object *o) {
    o->oFaceAngleYaw = o->oAngleToMario;
    o->oMoveAngleYaw = o->oAngleToMario;

    if (obj_is_on_ground(o)) {

        // Stop chasing Mario
        if (o->oDistanceToMario > 1500.f) {
            o->oChainChompFreeTimer = 30;
            o->oChainChompFreeState = 0;
        }

        // Make a fast hop
        else {
            obj_set_forward_vel(o, o->oAngleToMario, 1.f, 28.f);
            o->oVelY = 20.f;
            o->oFloor = NULL;
            o->oChainChompFreeAngle = o->oAngleToMario;
            obj_play_sound(o, SOUND_GENERAL_CHAIN_CHOMP1);
        }
    }
}

static void bhv_omm_chain_chomp_free_update() {
    struct Object *o = gCurrentObject;
    switch (o->oChainChompFreeState) {
        case 0: bhv_omm_chain_chomp_free_wander(o); break;
        case 1: bhv_omm_chain_chomp_free_chase(o); break;
    }

    if (o->oInteractStatus & (INT_STATUS_INTERACTED | INT_STATUS_ATTACKED_MARIO)) {
        o->oChainChompFreeTimer = 90;
        o->oChainChompFreeState = 0;
        o->oInteractStatus = 0;
    }

    o->oVelY = max_f(o->oVelY + o->oGravity, -75.f);
    perform_object_step(o, OBJ_STEP_UPDATE_HOME | OBJ_STEP_CHECK_ON_GROUND);
    obj_anim_play(o, 0, 1.f);
    bhv_chain_chomp_update_chain_parts(o, true);
    if (OMM_MOVESET_ODYSSEY) {
        obj_reset_hitbox(o, 100, 240, 80, 160, 120, 0);
    } else {
        obj_reset_hitbox(o, 80, 160, 80, 160, 120, 0);
    }
}

const BehaviorScript bhvOmmChainChompFree[] = {
    OBJ_TYPE_GENACTOR,
    BHV_DROP_TO_FLOOR(),
    BHV_LOAD_ANIMATIONS(oAnimations, chain_chomp_seg6_anims_06025178),
    BHV_ANIMATE(0),
    BHV_SET_HOME(),
    BHV_CALL_NATIVE(bhv_omm_chain_chomp_free_init),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_chain_chomp_free_update),
    BHV_END_LOOP(),
};
