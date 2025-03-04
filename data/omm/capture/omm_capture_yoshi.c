#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#if OMM_GAME_IS_SMSR
#define zeroLifeYoshi (gOmmGlobals->yoshiMode && gOmmGlobals->booZeroLife)
#else
#define zeroLifeYoshi false
#endif

#define OMM_CAPPY_YOSHI_FLUTTER_DURATION    (24)
#define OMM_CAPPY_YOSHI_TONGUE_DURATION     (16)
#define OMM_CAPPY_YOSHI_THROW_DURATION      (-8)
#define OMM_CAPPY_YOSHI_THROW_FVEL          (48)
#define OMM_CAPPY_YOSHI_THROW_YVEL          (16)
#define OMM_CAPPY_YOSHI_THROW_POS           0, -40, 10
#define OMM_CAPPY_YOSHI_FIRE_POS            0, 0, 10
#define OMM_CAPPY_YOSHI_BUBBLE_POS          0, 360, 0
#define OMM_CAPPY_YOSHI_EGG_THROW_POS       0, o->hitboxHeight / 3.f, 60

#define oScale_                             OBJECT_FIELD_F32(0x02)
#define oGraphYOffset_                      OBJECT_FIELD_S16(0x03, 0)
#define oNodeFlags_                         OBJECT_FIELD_S16(0x03, 1)
#define oNextObj_                           OBJECT_FIELD_OBJ(0x04)

typedef struct { const void *georef; f32 offset; f32 scale; bool hideChildren; } OmmCappyYoshiBubbleParams;
static const OmmCappyYoshiBubbleParams OMM_CAPPY_YOSHI_BUBBLE_PARAMS[] = {
    { amp_geo,                       0, 0.75f, false },
#if OMM_GAME_IS_SMMS
    { Bee_geo,                      40, 0.75f, false },
#endif
    { blue_coin_switch_geo,         25, 1.50f, false },
    { black_bobomb_geo,             40, 0.70f, false },
    { bobomb_buddy_geo,             40, 0.70f, false },
    { boo_geo,                       0, 0.60f, true  },
    { boo_castle_geo,                0, 0.60f, false },
    { bookend_geo,                   0, 1.00f, false },
    { bookend_part_geo,              0, 1.00f, false },
    { bowling_ball_geo,              0, 0.35f, false },
    { bowser_geo,                   40, 0.20f, false },
    { breakable_box_geo,            35, 0.35f, false },
#if !OMM_GAME_IS_R96X
    { breakable_box_small_geo,      35, 0.35f, false },
#endif
    { bully_geo,                    40, 0.60f, false },
    { bully_boss_geo,               40, 0.30f, false },
    { chilly_chief_geo,             45, 0.60f, false },
    { chilly_chief_big_geo,         45, 0.30f, false },
    { chuckya_geo,                  40, 0.60f, false },
    { clam_shell_geo,               10, 0.35f, false },
    { enemy_lakitu_geo,             30, 0.80f, false },
    { flyguy_geo,                   30, 0.90f, false },
    { goomba_geo,                   35, 1.00f, false },
#if OMM_GAME_IS_SMMS
    { goombone_geo,                 35, 1.00f, false },
#endif
    { haunted_chair_geo,            35, 0.65f, false },
    { heave_ho_geo,                 30, 1.00f, false },
    { hoot_geo,                     65, 0.80f, false },
    { king_bobomb_geo,              50, 0.30f, false },
    { klepto_geo,                   45, 0.30f, false },
    { koopa_with_shell_geo,         35, 1.25f, false },
    { koopa_without_shell_geo,      35, 1.25f, false },
    { koopa_shell_geo,              25, 1.00f, false },
    { metal_box_geo,                40, 0.25f, false },
    { omm_geo_sparkly_star_1_box,   40, 0.25f, false },
    { omm_geo_sparkly_star_2_box,   40, 0.25f, false },
    { omm_geo_sparkly_star_3_box,   40, 0.25f, false },
    { omm_geo_sparkly_star_1_hint,  35, 0.60f, false },
    { omm_geo_sparkly_star_2_hint,  35, 0.60f, false },
    { omm_geo_sparkly_star_3_hint,  35, 0.60f, false },
    { mips_geo,                      0, 0.80f, false },
    { moneybag_geo,                 30, 0.55f, false },
    { monty_mole_geo,                5, 0.80f, false },
#if OMM_GAME_IS_R96X
    { motos_geo,                    30, 0.75f, false },
#endif
    { mr_blizzard_geo,              30, 0.40f, false },
    { mr_blizzard_hidden_geo,       30, 0.40f, false },
    { mr_i_geo,                      0, 0.40f, false },
    { mr_i_iris_geo,                 0, 0.40f, false },
    { peach_geo,                    40, 0.45f, false },
    { penguin_geo,                  35, 1.00f, false },
    { piranha_plant_geo,            40, 0.35f, false },
    { red_flame_geo,                 0, 3.00f, false },
    { blue_flame_geo,                0, 3.00f, false },
    { omm_geo_yoshi_fire_red,        0, 3.00f, false },
    { omm_geo_yoshi_fire_blue,       0, 3.00f, false },
    { scuttlebug_geo,               35, 0.70f, false },
    { skeeter_geo,                  35, 0.45f, false },
    { snufit_geo,                    0, 0.90f, false },
    { spindrift_geo,                45, 0.45f, false },
    { spiny_geo,                    45, 1.00f, false },
    { spiny_ball_geo,               45, 1.00f, false },
    { swoop_geo,                    25, 0.80f, false },
    { toad_geo,                     40, 0.80f, false },
    { ukiki_geo,                    10, 0.75f, false },
    { wooden_signpost_geo,          35, 0.60f, false },
    { yoshi_geo,                    45, 0.40f, false },
    { yoshi_egg_geo,                30, 1.00f, false },
#if OMM_GAME_IS_SMSR
    { custom_shyguy_geo,            30, 0.90f, false },
#endif
    { NULL },
};

//
// Utils
//

static void omm_cappy_yoshi_store_object_values(struct Object *obj) {
    obj->oScale_ = obj->oScaleY;
    obj->oGraphYOffset_ = obj->oGraphYOffset;
    obj->oNodeFlags_ = obj->oNodeFlags;
    obj->oFlags &= ~OBJ_FLAG_GFX_INITED;
    struct Object *curr = obj;
    for_each_object_in_all_lists(child) {
        if (child->activeFlags && child->parentObj == obj) {
            child->oScale_ = child->oScaleY;
            child->oGraphYOffset_ = child->oGraphYOffset;
            child->oNodeFlags_ = child->oNodeFlags;
            child->oFlags &= ~OBJ_FLAG_GFX_INITED;
            curr->oNextObj_ = child;
            curr = child;
        }
    }
    curr->oNextObj_ = NULL;
}

static void omm_cappy_yoshi_restore_object_values(struct Object *obj) {
    for (struct Object *curr = obj; curr; curr = curr->oNextObj_) {
        curr->oScaleX = curr->oScale_;
        curr->oScaleY = curr->oScale_;
        curr->oScaleZ = curr->oScale_;
        curr->oGraphYOffset = curr->oGraphYOffset_;
        curr->oNodeFlags = curr->oNodeFlags_;
        curr->oScale_ = 0;
        curr->oGraphYOffset_ = 0;
        curr->oNodeFlags_ = 0;
    }
}

static void omm_cappy_yoshi_throw_set_pos(struct Object *o, struct Object *obj, f32 dx, f32 dy, f32 dz) {
    Mat4 objTransform;
    Vec3f throwPos = { dx, dy, dz };
    Vec3s throwRot = { 0, o->oFaceAngleYaw, 0 };
    geo_compute_capture_cappy_obj_transform(o, 4, objTransform);
    vec3f_transform(&obj->oPosX, throwPos, objTransform[3], throwRot, o->oGfxScale);
    obj_set_angle(obj, 0, o->oFaceAngleYaw, 0);
    obj_update_gfx(obj);
    obj->oFlags &= ~OBJ_FLAG_GFX_INITED;
}

//
// Eggs
//

static s32 omm_cappy_yoshi_get_num_eggs() {
    return obj_get_count_with_behavior_and_field_s32(bhvOmmYoshiEgg, _FIELD(oAction), 0);
}

static void omm_cappy_yoshi_spawn_egg(struct Object *o) {
    s32 numEggs = omm_cappy_yoshi_get_num_eggs();
    struct Object *egg = NULL;
    if (numEggs < OBJ_YOSHI_EGGS_MAX) {
        egg = omm_obj_spawn_yoshi_egg(o, numEggs + 1);
    } else {
        egg = obj_get_first_with_behavior_and_field_s32(bhvOmmYoshiEgg, _FIELD(oSubAction), OBJ_YOSHI_EGGS_MAX);
    }
    obj_spawn_white_puff(egg, POBJ_SOUND_YOSHI_EGG);
}

static void omm_cappy_yoshi_throw_egg(struct Object *o, f32 power) {
    s32 numEggs = omm_cappy_yoshi_get_num_eggs();
    struct Object *egg = obj_get_first_with_behavior_and_field_s32(bhvOmmYoshiEgg, _FIELD(oSubAction), numEggs);
    if (egg) {
        egg->oAction = 1;
        egg->oSubAction = 0;
        egg->oTimer = 0;
        egg->oNumLootCoins = 0;
        egg->oObjectPower = power;
        egg->oFlags &= ~OBJ_FLAG_GFX_INITED;
        Vec3f throwPos = { OMM_CAPPY_YOSHI_EGG_THROW_POS };
        Vec3s throwRot = { 0, o->oFaceAngleYaw, 0 };
        vec3f_transform(&egg->oPosX, throwPos, &o->oPosX, throwRot, o->oGfxScale);
        obj_set_angle(egg, 0, o->oFaceAngleYaw, 0);
        obj_update_gfx(egg);
        obj_set_throw_vel(egg, o, lerp_f(power, 60.f, 80.f), 0.f);
        obj_play_sound(o, POBJ_SOUND_YOSHI_EGG_THROW);
    }
}

//
// Consume, throw, eat
//

static void omm_cappy_yoshi_spit_fireball(struct Object *o) {
    struct Object *fireball = omm_obj_spawn_yoshi_fireball(o);
    omm_cappy_yoshi_throw_set_pos(o, fireball, OMM_CAPPY_YOSHI_FIRE_POS);
    gOmmObject->yoshi.tongueTimer = OMM_CAPPY_YOSHI_THROW_DURATION;
    gOmmObject->yoshi.tongued = NULL;
    gOmmObject->state.heldObj = NULL;
}

static void omm_cappy_yoshi_try_to_consume_object(struct Object *o, struct Object *obj) {
    const OmmBhvDataYoshiTongue *yt = omm_behavior_data_get_yoshi_tongue(obj->behavior);
    switch (yt->type) {

        // Store values for the object and its children, disable interpolation for one frame
        case YOSHI_TONGUE_TYPE_DEFAULT:
        case YOSHI_TONGUE_TYPE_EATABLE:
        case YOSHI_TONGUE_TYPE_THROWABLE: {
            omm_cappy_yoshi_store_object_values(obj);
        } break;

        // Destroy object, replace by a flame instead
        case YOSHI_TONGUE_TYPE_FIRE: {
            struct Object *fire = omm_obj_spawn_yoshi_fire(o);
            gOmmObject->yoshi.tongued = fire;
            gOmmObject->state.heldObj = fire;
            obj->oFlags |= OBJ_FLAG_YOSHI_DESTROY;
            obj_destroy(obj);
        } break;

        // Collect object
        case YOSHI_TONGUE_TYPE_MUSHROOM_1UP:
        case YOSHI_TONGUE_TYPE_STAR_OR_KEY:
        case YOSHI_TONGUE_TYPE_CAP: {
            obj->oPosX = o->oPosX;
            obj->oPosY = o->oPosY + 60.f;
            obj->oPosZ = o->oPosZ;
            obj->oIntangibleTimer = 0;
            gOmmObject->yoshi.tongued = NULL;
        } break;

        // Destroy object
        case YOSHI_TONGUE_TYPE_DESTROY: {
            gOmmObject->yoshi.tongued = NULL;
            obj->oFlags |= OBJ_FLAG_YOSHI_DESTROY;
            obj_destroy(obj);
        } break;
    }
}

static void omm_cappy_yoshi_try_to_throw_object(struct Object *o, struct Object *obj) {
    const OmmBhvDataYoshiTongue *yt = omm_behavior_data_get_yoshi_tongue(obj->behavior);
    switch (yt->type) {

        // Throw held object
        case YOSHI_TONGUE_TYPE_DEFAULT:
        case YOSHI_TONGUE_TYPE_THROWABLE: {
            omm_cappy_yoshi_restore_object_values(obj);
            omm_cappy_yoshi_throw_set_pos(o, obj, OMM_CAPPY_YOSHI_THROW_POS);
            pobj_throw_held_object(o, OMM_CAPPY_YOSHI_THROW_FVEL, OMM_CAPPY_YOSHI_THROW_YVEL);
            obj_play_sound(o, POBJ_SOUND_YOSHI_THROW);
            gOmmObject->yoshi.tongueTimer = OMM_CAPPY_YOSHI_THROW_DURATION;
            gOmmObject->yoshi.tongued = NULL;
        } break;

        // Spit fireball
        case YOSHI_TONGUE_TYPE_FIRE: {
            omm_cappy_yoshi_spit_fireball(o);
            obj_mark_for_deletion(obj);
        } break;
    }
}

static void omm_cappy_yoshi_try_to_eat_object(struct Object *o, struct Object *obj) {
    const OmmBhvDataYoshiTongue *yt = omm_behavior_data_get_yoshi_tongue(obj->behavior);
    switch (yt->type) {

        // Eat held object, spawn egg
        case YOSHI_TONGUE_TYPE_DEFAULT:
        case YOSHI_TONGUE_TYPE_EATABLE: {

            // Don't eat Koopa the Quick
            if (omm_obj_is_koopa_the_quick(obj)) {
                break;
            }

            // Eat the whole Goomba stack
            omm_array_for_each(omm_obj_get_goomba_behaviors(), p_bhv) {
                const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
                for_each_object_with_behavior(goomba, bhv) {
                    if (goomba != obj && omm_obj_is_goomba_stack(goomba) && goomba->oGoombaStackParent == obj) {
                        goomba->oFlags |= OBJ_FLAG_YOSHI_DESTROY;
                        obj_destroy(goomba);
                    }
                }
            }

            omm_cappy_yoshi_restore_object_values(obj);
            pobj_release_held_object();
            omm_cappy_yoshi_spawn_egg(o);
            gOmmObject->yoshi.tongued = NULL;
            gOmmObject->state.heldObj = NULL;
            obj->oFlags |= OBJ_FLAG_YOSHI_DESTROY;
            obj_destroy(obj);
        } break;

        // Spit fireball
        case YOSHI_TONGUE_TYPE_FIRE: {
            omm_cappy_yoshi_spit_fireball(o);
            obj_mark_for_deletion(obj);
        } break;
    }
}

static void omm_cappy_yoshi_release_object(struct Object *o, struct Object *obj) {
    const OmmBhvDataYoshiTongue *yt = omm_behavior_data_get_yoshi_tongue(obj->behavior);
    switch (yt->type) {

        // Throw it
        case YOSHI_TONGUE_TYPE_DEFAULT:
        case YOSHI_TONGUE_TYPE_THROWABLE: {
            if (obj_get_first_with_behavior(bhvOmmYoshiTongue)) {
                pobj_release_held_object();
            } else {
                omm_cappy_yoshi_try_to_throw_object(o, obj);
            }
        } break;

        // Eat it
        case YOSHI_TONGUE_TYPE_EATABLE: {
            if (obj_get_first_with_behavior(bhvOmmYoshiTongue)) {
                pobj_release_held_object();
            } else {
                omm_cappy_yoshi_try_to_eat_object(o, obj);
            }
        } break;

        // Destroy it
        case YOSHI_TONGUE_TYPE_FIRE:
        case YOSHI_TONGUE_TYPE_DESTROY: {
            obj->oFlags |= OBJ_FLAG_YOSHI_DESTROY;
            obj_destroy(obj);
        } break;

        // Make it tangible again
        case YOSHI_TONGUE_TYPE_MUSHROOM_1UP:
        case YOSHI_TONGUE_TYPE_STAR_OR_KEY:
        case YOSHI_TONGUE_TYPE_CAP: {
            obj->oIntangibleTimer = 0;
        } break;
    }
}

struct Object *omm_cappy_yoshi_get_object_in_mouth(struct Object *o) {
    if (omm_mario_is_capture(gMarioState) && omm_capture_get_type(o) == OMM_CAPTURE_YOSHI && gOmmObject->yoshi.tongueTimer == 0) {
        return gOmmObject->yoshi.tongued;
    }
    return NULL;
}

//
// Init
//

bool omm_cappy_yoshi_init(struct Object *o) {

    // Cannot capture Yoshi if all other captures haven't been registered
    if (!gOmmGlobals->yoshiMode && (omm_save_file_get_capture_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE) & OMM_ALL_CAPTURES_BUT_YOSHI) != OMM_ALL_CAPTURES_BUT_YOSHI) {
        return false;
    }

    // Can capture Yoshi only if he's idling or moving
    if (o->oAction != YOSHI_ACT_IDLE &&
        o->oAction != YOSHI_ACT_WALK) {
        return false;
    }

#if OMM_GAME_IS_SM64
    // Castle roof Yoshi
    if (o->behavior == bhvYoshi) {
        obj_anim_play(o, 0, 1.f);
        gOmmObject->state.actionTimer = 5;
    } else {
        obj_unload_all_with_behavior(bhvYoshi);
    }
#endif

    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    gOmmObject->yoshi.flutterTimer = -1;
    gOmmObject->yoshi.tongueTimer = 0;
    gOmmObject->yoshi.tongueSine = 0;
    gOmmObject->yoshi.tongued = NULL;
    gOmmObject->yoshi.lavaBoost = false;
    return true;
}

void omm_cappy_yoshi_end(struct Object *o) {
    obj_set_angle(o, 0, o->oFaceAngleYaw, 0);
    if (gOmmObject->yoshi.tongued) {
        omm_cappy_yoshi_release_object(o, gOmmObject->yoshi.tongued);
    }
    if (o->behavior == bhvYoshi) {
        obj_anim_play_with_sound(o, 2, 1.f, SOUND_GENERAL_ENEMY_ALERT1, true);
        o->oForwardVel = 50.f;
        o->oVelY = 40.f;
        o->oMoveAngleYaw = -0x3FFF;
        o->oAction = YOSHI_ACT_FINISH_JUMPING_AND_DESPAWN;
    }
}

u64 omm_cappy_yoshi_get_type(UNUSED struct Object *o) {
    return OMM_CAPTURE_YOSHI;
}

f32 omm_cappy_yoshi_get_top(struct Object *o) {
    return omm_capture_get_hitbox_height(o);
}

//
// Update
//

s32 omm_cappy_yoshi_update(struct Object *o) {
#if OMM_GAME_IS_SM64
    // Castle roof Yoshi, act as an NPC with a special dialog
    if (o->behavior == bhvYoshi) {
        if (gOmmObject->state.actionState == 1) audio_play_puzzle_jingle();
        o->oBehParams = 0;
        o->oBehParams2ndByte = OMM_DIALOG_YOSHI_CAPTURE;
        omm_mario_lock(gMarioState, -1);
        return omm_cappy_npc_message_update(o);
    }
#endif

    bool wasUnderwater = obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ));
    gOmmObject->state.actionTimer--;

    // Caps
    bool hasWingCap = omm_mario_has_wing_cap(gMarioState);
    bool hasMetalCap = omm_mario_has_metal_cap(gMarioState);
    bool hasVanishCap = omm_mario_has_vanish_cap(gMarioState);

    // Abilities
    struct Object *obj = omm_cappy_yoshi_get_object_in_mouth(o);
    bool abilitySpinJump = obj && (
        omm_obj_is_goomba(obj)
    );
    bool abilitySpeedBoost = obj && (
        omm_behavior_data_get_capture_type(obj) == OMM_CAPTURE_LAKITU ||
        omm_behavior_data_get_capture_type(obj) == OMM_CAPTURE_MIPS
    );
    bool abilityAttractCoins = obj && (
        omm_behavior_data_get_capture_type(obj) == OMM_CAPTURE_AMP
    );
    bool abilityMoveThroughWalls = obj && (
        omm_obj_is_boo(obj)
    );
    bool abilityGroundPoundLanding = obj && (
        omm_obj_is_bully(obj) ||
        omm_obj_is_metal_ball(obj)
    );

    // Hitbox
    o->hitboxRadius = omm_capture_get_hitbox_radius(o);
    o->hitboxHeight = omm_capture_get_hitbox_height(o);
    o->oWallHitboxRadius = omm_capture_get_wall_hitbox_radius(o);

    // Properties
    POBJ_SET_ABOVE_WATER;
    POBJ_SET_UNDER_WATER;
    POBJ_SET_AFFECTED_BY_WATER;
    POBJ_SET_AFFECTED_BY_VERTICAL_WIND;
    POBJ_SET_AFFECTED_BY_CANNON;
    POBJ_SET_INVULNERABLE * hasMetalCap;
    POBJ_SET_INTANGIBLE * hasVanishCap;
    POBJ_SET_IMMUNE_TO_FIRE * (hasMetalCap || hasVanishCap);
    POBJ_SET_IMMUNE_TO_LAVA * hasMetalCap;
    POBJ_SET_IMMUNE_TO_QUICKSAND * hasMetalCap;
    POBJ_SET_IMMUNE_TO_STRONG_WINDS * hasMetalCap;
    POBJ_SET_ABLE_TO_MOVE_ON_SLOPES;
    POBJ_SET_ABLE_TO_MOVE_THROUGH_WALLS * (hasVanishCap || abilityMoveThroughWalls || zeroLifeYoshi);
    POBJ_SET_ABLE_TO_OPEN_DOORS;
    POBJ_SET_ATTACKING * hasMetalCap;

    // Inputs
    if (pobj_process_inputs(o)) {
        pobj_move(o, false, hasVanishCap || abilitySpeedBoost, false);
        s32 jumpResult = pobj_jump(o, 1);
        if (!gOmmObject->yoshi.lavaBoost) {

            // Single jump
            if (jumpResult == POBJ_RESULT_JUMP_START) {
                if (!hasMetalCap) {
                    obj_play_sound(o, POBJ_SOUND_JUMP_1);
                } else if (wasUnderwater) {
                    obj_play_sound(o, POBJ_SOUND_JUMP_METAL_WATER);
                } else {
                    obj_play_sound(o, POBJ_SOUND_JUMP_METAL);
                }
                if (abilitySpinJump && gOmmMario->spin.timer) {
                    gOmmObject->state.actionTimer = 8;
                    o->oVelY = max_f(o->oVelY, 1.35f * pobj_get_jump_velocity(o));
                    omm_secrets_unlock(OMM_SECRET_YOSHI_SECRET);
                }
            }

            // Spin jump
            else if (jumpResult == POBJ_RESULT_JUMP_HOLD && gOmmObject->state.actionTimer > 0) {
                o->oVelY = max_f(o->oVelY, 1.35f * pobj_get_jump_velocity(o));
            }

            // Flutter jump
            else if (!obj_is_on_ground(o) && gOmmObject->yoshi.flutterTimer < 0 && POBJ_A_BUTTON_PRESSED) {
                gOmmObject->yoshi.flutterTimer = 0;
                o->oVelY = min_f(o->oVelY / 2.f, 0.f);
            }

            // Actions
            if (gOmmObject->yoshi.tongueTimer == 0) {
                if (!gOmmObject->yoshi.tongued) {

                    // Tongue/Fireball
                    if (POBJ_B_BUTTON_PRESSED) {
                        if (hasWingCap) {
                            omm_cappy_yoshi_spit_fireball(o);
                        } else {
                            omm_obj_spawn_yoshi_tongue(o);
                            omm_sound_play(OMM_SOUND_EFFECT_YOSHI_TONGUE, o->oCameraToObject);
                            gOmmObject->yoshi.tongueTimer = OMM_CAPPY_YOSHI_TONGUE_DURATION;
                        }
                    }

                    // Egg throw
                    if (omm_cappy_yoshi_get_num_eggs() > 0) {
                        pobj_charge_attack(15, o, 200, 100);
                        pobj_release_attack(15,
                            omm_cappy_yoshi_throw_egg(o, _power_);
                        );
                    } else {
                        gOmmObject->state._powerTimer = 0;
                    }

                } else {

                    // Throw
                    if (POBJ_B_BUTTON_PRESSED) {
                        omm_cappy_yoshi_try_to_throw_object(o, gOmmObject->yoshi.tongued);
                    }

                    // Eat
                    else if (POBJ_X_BUTTON_PRESSED) {
                        omm_cappy_yoshi_try_to_eat_object(o, gOmmObject->yoshi.tongued);
                    }
                }
            }
        }
    }

    // Flutter jump
    if (obj_is_on_ground(o)) {
        gOmmObject->yoshi.flutterTimer = -1;
    } else if (gOmmObject->yoshi.flutterTimer >= 0) {

        // Update flutter timer
        bool updateVelocity = false;
        if (wasUnderwater || hasWingCap) {
            if (gOmmObject->yoshi.flutterTimer >= OMM_CAPPY_YOSHI_FLUTTER_DURATION) {
                if (POBJ_A_BUTTON_DOWN) {
                    gOmmObject->yoshi.flutterTimer++;
                } else {
                    gOmmObject->yoshi.flutterTimer = -1;
                }
            } else if (!POBJ_A_BUTTON_DOWN) {
                gOmmObject->yoshi.flutterTimer = OMM_CAPPY_YOSHI_FLUTTER_DURATION;
            } else {
                gOmmObject->yoshi.flutterTimer = (gOmmObject->yoshi.flutterTimer + 1) % 3;
                updateVelocity = true;
            }
        } else if (++gOmmObject->yoshi.flutterTimer <= OMM_CAPPY_YOSHI_FLUTTER_DURATION) {
            if (!POBJ_A_BUTTON_DOWN) {
                gOmmObject->yoshi.flutterTimer = OMM_CAPPY_YOSHI_FLUTTER_DURATION;
            } else {
                updateVelocity = true;
            }
        }

        // Update velocity
        if (updateVelocity) {
            if (wasUnderwater) {
                o->oVelY = min_f(o->oVelY - pobj_get_gravity(o) * 1.5f, pobj_get_jump_velocity(o));
            } else if (o->oVelY < 0.f) {
                o->oVelY -= pobj_get_gravity(o) * relerp_0_1_f(o->oVelY, pobj_get_terminal_velocity(o) / 2.f, 0.f, 2.f, 1.5f);
            } else {
                o->oVelY = min_f(o->oVelY - pobj_get_gravity(o) * relerp_0_1_f(o->oVelY, 0.f, pobj_get_jump_velocity(o) / 2.f, 1.5f, 1.25f), pobj_get_jump_velocity(o));
            }
        }
    }

    // Tongue
    if (gOmmObject->yoshi.tongueTimer > 0) {
        gOmmObject->yoshi.tongueTimer--;
        if (gOmmObject->yoshi.tongueTimer == 0 && gOmmObject->yoshi.tongued) {
            omm_cappy_yoshi_try_to_consume_object(o, gOmmObject->yoshi.tongued);
            struct Object *tongue = obj_get_first_with_behavior(bhvOmmYoshiTongue);
            if (tongue) {
                obj_mark_for_deletion(tongue);
            }
        }
    } else if (gOmmObject->yoshi.tongueTimer < 0) {
        gOmmObject->yoshi.tongueTimer++;
    }

    // Movement
    f32 prevVelY = o->oVelY;
    bool wasOnGround = obj_is_on_ground(o);
    perform_object_step(o, POBJ_STEP_FLAGS);
    pobj_decelerate(o);
    pobj_apply_gravity(o, hasVanishCap || abilitySpeedBoost ? 0.8f : 1.f);
    pobj_handle_special_floors(o);
    pobj_stop_if_unpossessed();
    bool isOnGround = obj_is_on_ground(o);

    // Transition under/above water
    bool isUnderwater = obj_is_underwater(o, find_water_level(o->oPosX, o->oPosZ));
    if (gOmmObject->yoshi.flutterTimer >= 0) {

        // If entering water during a flutter jump, cancel it until A is released
        if (isUnderwater && !wasUnderwater) {
            gOmmObject->yoshi.flutterTimer = OMM_CAPPY_YOSHI_FLUTTER_DURATION + 1;
        }

        // If exiting water during a flutter jump without Wing cap, reduce its duration to half the max
        else if (!isUnderwater && wasUnderwater && !hasWingCap) {
            gOmmObject->yoshi.flutterTimer = max_s(gOmmObject->yoshi.flutterTimer, OMM_CAPPY_YOSHI_FLUTTER_DURATION / 2);
        }
    }

    // Attract coins if Yoshi has an amp in his mouth
    if (abilityAttractCoins) {
        omm_obj_process_interactions(o, POBJ_INT_PRESET_AMP);
    }

    // Ground pound land if Yoshi has a metal ball in his mouth
    if (abilityGroundPoundLanding && !wasOnGround && isOnGround) {
        POBJ_SET_GROUND_POUNDING;
    }

    // Interactions
    pobj_process_interactions(
        if (obj == gOmmObject->yoshi.tongued) {
            continue;
        }
    );
    pobj_stop_if_unpossessed();
    if (POBJ_IS_BOUNCING || POBJ_IS_STAR_DANCING) {
        gOmmObject->yoshi.flutterTimer = -1;
    }

    // Lava boost
    if (gOmmObject->yoshi.lavaBoost && o->oVelY <= 0.f && isOnGround) {
        if (prevVelY < -24.f) {
            o->oVelY = -0.5f * prevVelY;
            obj_set_forward_vel(o, o->oFaceAngleYaw, 1.f, o->oForwardVel * 0.5f);
        } else {
            gOmmObject->yoshi.lavaBoost = false;
        }
    }

    // Release tongued object if lava-boosting or a warp is triggered
    if (gOmmObject->yoshi.tongued && (gOmmObject->yoshi.lavaBoost || gOmmWarp->state > POBJ_WARP_STATE_NOT_WARPING)) {
        omm_cappy_yoshi_release_object(o, gOmmObject->yoshi.tongued);
        gOmmObject->yoshi.tongueTimer = 0;
        gOmmObject->yoshi.tongued = NULL;
        gOmmObject->state.heldObj = NULL;
    }

    // Animation, sound and particles
    bool isFlutterJumping = (
        gOmmObject->yoshi.flutterTimer >= 0 &&
        gOmmObject->yoshi.flutterTimer <= OMM_CAPPY_YOSHI_FLUTTER_DURATION
    );
    if (gOmmObject->yoshi.lavaBoost) {
        obj_anim_play(o, 1, 8.f);
        if ((gMarioState->area->terrainType & TERRAIN_MASK) != TERRAIN_SNOW && !hasMetalCap && o->oVelY > 0.f) {
            spawn_object(o, MODEL_BURN_SMOKE, bhvBlackSmokeMario);
            obj_play_sound(o, POBJ_SOUND_LAVA_BURN);
        }
    } else if (isOnGround) {
        if (POBJ_IS_WALKING) {
            obj_anim_play(o, 1, max_f(1.f, 1.5f * POBJ_ABS_FORWARD_VEL / pobj_get_walk_speed(o)));
        } else {
            obj_anim_play(o, 0, 1.f);
        }
        if (wasOnGround) {
            s32 stepSound;
            if (zeroLifeYoshi) {
                stepSound = NO_SOUND;
            } else if (!hasMetalCap) {
                stepSound = POBJ_SOUND_WALK_YOSHI;
            } else if (isUnderwater) {
                stepSound = POBJ_SOUND_WALK_METAL_WATER;
            } else {
                stepSound = POBJ_SOUND_WALK_METAL;
            }
            obj_make_step_sound_and_particle(o, &gOmmObject->state.walkDistance, pobj_get_walk_speed(o) * 10.f, POBJ_ABS_FORWARD_VEL, stepSound, OBJ_PARTICLE_NONE);
        } else {
            s32 landingSound;
            if (zeroLifeYoshi) {
                landingSound = NO_SOUND;
            } else if (!hasMetalCap) {
                landingSound = POBJ_SOUND_WALK_YOSHI;
            } else if (isUnderwater) {
                landingSound = POBJ_SOUND_LANDING_METAL_WATER;
            } else {
                landingSound = POBJ_SOUND_LANDING_METAL;
            }
            obj_play_sound(o, landingSound);
            if (POBJ_IS_GROUND_POUNDING) {
                obj_spawn_particle_preset(o, PARTICLE_MIST_CIRCLE, false);
                obj_spawn_particle_preset(o, PARTICLE_HORIZONTAL_STAR, false);
                obj_play_sound(o, SOUND_ACTION_TERRAIN_HEAVY_LANDING);
                set_camera_shake_from_hit(SHAKE_GROUND_POUND);
            }
        }
    } else if (isFlutterJumping) {
        obj_anim_play(o, 1, 5.f);
        if (gOmmObject->yoshi.flutterTimer == OMM_CAPPY_YOSHI_FLUTTER_DURATION) {
            omm_sound_play(OMM_SOUND_EFFECT_YOSHI_FLUTTER_END, o->oCameraToObject);
        } else if ((gOmmObject->yoshi.flutterTimer % 3) == 0) {
            omm_sound_play(OMM_SOUND_EFFECT_YOSHI_FLUTTER, o->oCameraToObject);
        }
        if (isUnderwater && (gGlobalTimer & 1)) {
            spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvPlungeBubble);
        } else if (hasWingCap && (gGlobalTimer & 1)) {
            struct Object *smoke = spawn_object(o, MODEL_SMOKE, bhvWhitePuff2);
            smoke->oFlags |= OBJ_FLAG_DONT_RENDER_ON_INIT;
            obj_scale(smoke, 1.5f);
        }
    } else {
        obj_anim_play(o, 2, 1.f);
        obj_anim_extend(o);
    }
    if (abilitySpinJump && isOnGround) {
        obj_spawn_particle_preset(o, PARTICLE_DUST * (gOmmMario->spin.timer != 0), false);
    } else {
        gOmmMario->spin.timer = 0;
    }

    // Wings
    if (hasWingCap) {
        struct Object *wings = omm_obj_spawn_yoshi_wings(o);
        if (wings) {
            wings->oAnimState = isFlutterJumping;
        }
    }

    // OK
    pobj_return_ok;
}

void omm_cappy_yoshi_update_gfx(struct Object *o) {
#if OMM_GAME_IS_SM64
    // Castle roof Yoshi, act as an NPC with a special dialog
    if (o->behavior == bhvYoshi) {
        return omm_cappy_npc_message_update_gfx(o);
    }
#endif

    gOmmObject->yoshi.tongueSine = (
        gOmmObject->yoshi.tongueTimer > 0 ?
        sins((0x8000 * gOmmObject->yoshi.tongueTimer) / OMM_CAPPY_YOSHI_TONGUE_DURATION) :
        sins((0x8000 * gOmmObject->yoshi.tongueTimer) / OMM_CAPPY_YOSHI_THROW_DURATION)
    );

    // Gfx
    obj_update_gfx(o);
    obj_random_blink(o, &o->oYoshiBlinkTimer);
    if (gOmmObject->state.actionTimer > 0) {
        o->oGfxAngle[1] += 0x2000 * gOmmObject->state.actionTimer;
        obj_spawn_particle_preset(o, !POBJ_IS_STAR_DANCING * PARTICLE_SPARKLES, false);
    }
    if (POBJ_IS_ABLE_TO_MOVE_THROUGH_WALLS && !zeroLifeYoshi) {
        o->oTransparency = 0x80;
    }

    // Update held object
    if (!obj_get_first_with_behavior(bhvOmmYoshiTongue) && pobj_update_held_object(o, OMM_CAPPY_YOSHI_BUBBLE_POS, 0, 1)) {
        struct Object *heldObj = gOmmObject->state.heldObj;

        // Update bubble
        struct Object *bubble = omm_obj_spawn_yoshi_bubble(o);
        obj_copy_pos(bubble, heldObj);
        obj_update_gfx(bubble);

        // Update held object
        heldObj->oNodeFlags |= GRAPH_RENDER_BILLBOARD;
        heldObj->oGraphYOffset = 0;
        heldObj->oFaceAngleYaw = heldObj->oMoveAngleYaw = atan2s(gCamera->pos[2] - gCamera->focus[2], gCamera->pos[0] - gCamera->focus[0]);
        if (heldObj->oGraphNode) {
            for (const OmmCappyYoshiBubbleParams *params = OMM_CAPPY_YOSHI_BUBBLE_PARAMS; params->georef; ++params) {
                if (heldObj->oGraphNode->georef == params->georef) {
                    heldObj->oPosY -= params->offset;
                    obj_scale(heldObj, params->scale);
                    obj_update_gfx(heldObj);
                    f32 scaleFactor = params->scale / heldObj->oScale_;
                    for (struct Object *child = heldObj->oNextObj_; child; child = child->oNextObj_) {
                        if (params->hideChildren) {
                            child->oNodeFlags |= GRAPH_RENDER_INVISIBLE;
                        } else {
                            child->oGraphYOffset = 0;
                            obj_scale(child, child->oScale_ * scaleFactor);
                        }
                        obj_update_gfx(child);
                    }
                    break;
                }
            }
        }

        // Display the size of the Goomba stack
        bubble->oBehParams2ndByte = 1;
        omm_array_for_each(omm_obj_get_goomba_behaviors(), p_bhv) {
            const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
            for_each_object_with_behavior(goomba, bhv) {
                bubble->oBehParams2ndByte += (goomba != heldObj && omm_obj_is_goomba_stack(goomba) && goomba->oGoombaStackParent == heldObj);
            }
        }
    }

    // Cappy transform
    gOmmObject->cappy.object = o;
}
