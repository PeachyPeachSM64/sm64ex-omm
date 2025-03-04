#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

static void omm_mario_metal_destroy_or_push_away_object(struct MarioState *m, struct Object *o) {
    if (omm_obj_is_metal_destructible(o)) {
        obj_destroy(o);
    } else {
        f32 x = m->pos[0];
        f32 z = m->pos[2];
        push_mario_out_of_object(m, o, 0);
        o->oPosX -= (m->pos[0] - x);
        o->oPosZ -= (m->pos[2] - z);
        o->oInteractStatus = INT_STATUS_INTERACTED;
        m->pos[0] = x;
        m->pos[2] = z;
    }
}

void push_mario_out_of_object(struct MarioState *m, struct Object *o, f32 padding) {
    if (m->action != ACT_GETTING_BLOWN &&
        m->action != ACT_THROWN_FORWARD &&
        m->action != ACT_THROWN_BACKWARD) {
        f32 dx = m->pos[0] - o->oPosX;
        f32 dz = m->pos[2] - o->oPosZ;
        f32 dv = sqrtf(dx * dx + dz * dz);
        f32 dm = o->hitboxRadius + m->marioObj->hitboxRadius + padding;
        if (dv < dm) {
            s16 pushAngle = (dv ? atan2s(dz, dx) : m->faceAngle[1]);
            f32 mx = o->oPosX + dm * sins(pushAngle);
            f32 my = m->pos[1];
            f32 mz = o->oPosZ + dm * coss(pushAngle);
            f32_find_wall_collision(&mx, &my, &mz, 60.f, 50.f);
            struct Surface *floor = NULL;
            f32 fy = find_floor(mx, my, mz, &floor);
            if (floor) {
                vec3f_set(m->pos, mx, max_f(my, fy), mz);
                m->floor = floor;
                m->floorHeight = fy;
                if (my > fy + 100.f) {
                    m->input |= INPUT_OFF_FLOOR;
                }
            }
        }
    }
}

u32 determine_interaction(struct MarioState *m, struct Object *o) {
    u32 interaction = 0;

    // Bounces
    if (m->action & ACT_FLAG_AIR) {
        if (m->vel[1] < 0.f && m->pos[1] > o->oPosY) {
            interaction = INT_HIT_FROM_ABOVE;
        } else if (m->vel[1] > 0.f && m->pos[1] < o->oPosY) {
            interaction = INT_HIT_FROM_BELOW;
        }
    }

    // Attacks
    if (m->action & ACT_FLAG_ATTACKING) {

        // Punching / Kicking
        if (omm_mario_is_punching(m) || omm_mario_is_kicking(m)) {
            s16 dYawToObject = mario_obj_angle_to_object(m, o) - m->faceAngle[1];
            if ((m->flags & MARIO_PUNCHING) && abs_s(dYawToObject) <= 0x2AAA) {
                interaction = INT_PUNCH;
            }
            if ((m->flags & MARIO_KICKING) && abs_s(dYawToObject) <= 0x2AAA) {
                interaction = INT_KICK;
            }
            if ((m->flags & MARIO_TRIPPING) && abs_s(dYawToObject) <= 0x4000) {
                interaction = INT_TRIP;
            }
        }

        // Ground pounding
        else if (omm_mario_is_ground_pounding(m) || m->action == ACT_TWIRLING) {
            if (m->vel[1] < 0.f) {
                interaction = INT_GROUND_POUND_OR_TWIRL;
            }
        }

        // Landing
        else if (omm_mario_is_ground_pound_landing(m) || m->action == ACT_TWIRL_LAND) {
            if (m->vel[1] < 0.f && m->actionState == 0) {
                interaction = INT_GROUND_POUND_OR_TWIRL;
            }
        }

        // Slide kicking
        else if (m->action == ACT_SLIDE_KICK || m->action == ACT_SLIDE_KICK_SLIDE) {
            interaction = INT_SLIDE_KICK;
        }

        // Riding shell
        else if (m->action & ACT_FLAG_RIDING_SHELL) {
            interaction = INT_FAST_ATTACK_OR_SHELL;
        }

        // Going fast?
        else if (abs_f(m->forwardVel) >= 26.f) {
            interaction = INT_FAST_ATTACK_OR_SHELL;
        }
    }

    return interaction;
}

//
// Vanilla interactions
//

extern u8 sDelayInvincTimer;
extern s16 sInvulnerable;
extern u32 interact_hoot(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_grabbable(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_door(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_damage(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_coin(struct MarioState *m, u32 interactType, struct Object *o);
// extern u32 interact_cap(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_pole(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_unknown_08(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_breakable(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_strong_wind(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_warp_door(struct MarioState *m, u32 interactType, struct Object *o);
// extern u32 interact_star_or_key(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_warp(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_cannon_base(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_bounce_top(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_water_ring(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_bully(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_flame(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_koopa_shell(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_mr_blizzard(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_hit_from_below(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_text(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_tornado(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_whirlpool(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_clam_or_bubba(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_bbh_entrance(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_snufit_bullet(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_shock(struct MarioState *m, u32 interactType, struct Object *o);
extern u32 interact_igloo_barrier(struct MarioState *m, u32 interactType, struct Object *o);

//
// Interactions
//

bool omm_mario_interact_hoot(struct MarioState *m, struct Object *o) {
    if (omm_mario_is_milk(m)) {
        return false;
    }
    return interact_hoot(m, INTERACT_HOOT, o);
}

bool omm_mario_interact_grabbable(struct MarioState *m, struct Object *o) {
    if ((o->oInteractionSubtype & INT_SUBTYPE_GRABS_MARIO) && obj_is_object1_facing_object2(o, m->marioObj, 0x3000) && (omm_mario_has_metal_cap(m) || omm_mario_is_milk(m))) {
        return false;
    }
    return interact_grabbable(m, INTERACT_GRABBABLE, o);
}

bool omm_mario_interact_door(struct MarioState *m, struct Object *o) {
    return interact_door(m, INTERACT_DOOR, o);
}

bool omm_mario_interact_damage(struct MarioState *m, struct Object *o) {
    if (omm_mario_has_metal_cap(m) || omm_mario_is_milk(m)) {
        omm_mario_metal_destroy_or_push_away_object(m, o);
        return false;
    }
    return interact_damage(m, INTERACT_DAMAGE, o);
}

bool omm_mario_interact_coin(struct MarioState *m, struct Object *o) {

    // Track collected coins for Sparkly stars
    if (o->respawnInfoType == OMM_OBJECT_LOST_COIN) gOmmSparklyContext->coinsLost++;
    else if (o->oDamageOrCoinValue == 1) gOmmSparklyContext->coinsYellow++;
    else if (o->oDamageOrCoinValue == 2) gOmmSparklyContext->coinsRed++;
    else if (o->oDamageOrCoinValue == 5) gOmmSparklyContext->coinsBlue++;

    // All coins (except lost coins) are worth 2x with the Moneybag capture or if a Moneybag is inside Yoshi's mouth
    if (o->respawnInfoType != OMM_OBJECT_LOST_COIN && omm_mario_is_capture(m)) {
        struct Object *objInYoshisMouth = omm_cappy_yoshi_get_object_in_mouth(gOmmCapture);
        u64 captureType = objInYoshisMouth ? omm_behavior_data_get_capture_type(objInYoshisMouth) : omm_capture_get_type(gOmmCapture);
        if (captureType == OMM_CAPTURE_MONEY_BAG) {
            o->oDamageOrCoinValue *= 2;
        }
    }

    // Collect coins
    omm_stats_increase(coinsCollected, o->oDamageOrCoinValue);
    return interact_coin(m, INTERACT_COIN, o);
}

bool omm_mario_interact_cap(struct MarioState *m, struct Object *o) {
    u32 cap = (
        (o->behavior == bhvNormalCap) * MARIO_NORMAL_CAP |
        (o->behavior == bhvWingCap) * MARIO_WING_CAP |
        (o->behavior == bhvMetalCap) * MARIO_METAL_CAP |
        (o->behavior == bhvVanishCap) * MARIO_VANISH_CAP
    );

    // Cap must be valid
    // Mario isn't getting blown
    // Mario hasn't the milk power-up
    // No Vibe active
    // Not a capture or Yoshi capture with improved Power-ups
    if (cap && m->action != ACT_GETTING_BLOWN && !omm_mario_is_milk(m) && !omm_peach_vibe_is_active() && (!omm_mario_is_capture(m) || (omm_capture_get_type(gOmmCapture) == OMM_CAPTURE_YOSHI && OMM_POWER_UPS_IMPROVED))) {
        omm_stats_increase(capsCollected, 1);
        m->interactObj = o;
        o->oInteractStatus = INT_STATUS_INTERACTED;

        m->flags &= ~(MARIO_CAP_ON_HEAD | MARIO_CAP_IN_HAND);
        m->flags |= cap;

        switch (cap) {
            case MARIO_NORMAL_CAP: {
                audio_stop_cap_music();
                omm_mario_unset_cap(m);
            } break;

            case MARIO_WING_CAP: {
                audio_play_wing_cap_music();
                m->capTimer = max_s(m->capTimer, OMM_POWER_UPS_IMPROVED ? OMM_IMPROVED_WING_CAP_DURATION : 1800);
            } break;

            case MARIO_METAL_CAP: {
                audio_play_metal_cap_music();
                m->capTimer = max_s(m->capTimer, OMM_POWER_UPS_IMPROVED ? OMM_IMPROVED_METAL_CAP_DURATION : 600);
            } break;

            case MARIO_VANISH_CAP: {
                audio_play_vanish_cap_music();
                m->capTimer = max_s(m->capTimer, OMM_POWER_UPS_IMPROVED ? OMM_IMPROVED_VANISH_CAP_DURATION : 600);
            } break;
        }

        SFX(SOUND_MENU_STAR_SOUND);
        if (!omm_mario_is_capture(m)) {
            SFX(SOUND_MARIO_HERE_WE_GO);
        }

        if ((m->action & ACT_FLAG_IDLE) || m->action == ACT_WALKING) {
            m->flags |= MARIO_CAP_IN_HAND;
            omm_mario_set_action(m, ACT_PUTTING_ON_CAP, 0, 0);
        } else {
            m->flags |= MARIO_CAP_ON_HEAD;
        }
        return true;
    }
    return false;
}

bool omm_mario_interact_pole(struct MarioState *m, struct Object *o) {
    if ((m->action & ACT_FLAG_RIDING_SHELL) || o == gOmmMario->state.poleObject) {
        return false;
    }
    return interact_pole(m, INTERACT_POLE, o);
}

bool omm_mario_interact_koopa(struct MarioState *m, struct Object *o) {
    return interact_bounce_top(m, INTERACT_KOOPA, o);
}

bool omm_mario_interact_unknown_08(struct MarioState *m, struct Object *o) {
    if (omm_mario_has_metal_cap(m) || omm_mario_is_milk(m)) {
        omm_mario_metal_destroy_or_push_away_object(m, o);
        return false;
    }
    return interact_unknown_08(m, INTERACT_UNKNOWN_08, o);
}

bool omm_mario_interact_breakable(struct MarioState *m, struct Object *o) {
    return interact_breakable(m, INTERACT_BREAKABLE, o);
}

bool omm_mario_interact_strong_wind(struct MarioState *m, struct Object *o) {
    if (omm_mario_has_metal_cap(m) || omm_mario_is_milk(m)) {
        return false;
    }
    return interact_strong_wind(m, INTERACT_STRONG_WIND, o);
}

bool omm_mario_interact_warp_door(struct MarioState *m, struct Object *o) {
    return interact_warp_door(m, INTERACT_WARP_DOOR, o);
}

bool omm_mario_interact_star_or_key(struct MarioState *m, struct Object *o) {
    if (!omm_mario_is_dead(m) && o) {

        // Warp object
        // Some rom-hacks replace the Grand Star cutscene by a warp to an ending level
        if (o->oInteractType & INTERACT_WARP) {
            return omm_mario_interact_warp(m, o);
        }

        // Sparkly star
        if (omm_sparkly_interact_star(m, o)) {
            return true;
        }

        // Collect star or key
        omm_sparkly_interact_grand_star(m, o);
        omm_save_file_collect_star_or_key(gCurrSaveFileNum - 1, OMM_GAME_MODE, gCurrLevelNum - 1, (o->oBehParams >> 24) & 0x1F, m->numCoins);
        m->numStars = omm_save_file_get_total_star_count(gCurrSaveFileNum - 1, OMM_GAME_MODE);

        // Update Mario
        mario_stop_riding_and_holding(m);
        update_mario_sound_and_camera(m);
        SFX(SOUND_MENU_STAR_SOUND);
        spawn_object(o, MODEL_NONE, bhvStarKeyCollectionPuffSpawner);
        o->oInteractStatus = INT_STATUS_INTERACTED;
        m->interactObj = o;
        m->usedObj = o;

        // Grand star
        if (o->oInteractionSubtype & INT_SUBTYPE_GRAND_STAR) {
            omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
            if (gOmmGlobals->yoshiMode) {
                omm_obj_spawn_yoshi_mode_yoshi(m->marioObj);
            }
            omm_mario_set_action(m, ACT_JUMBO_STAR_CUTSCENE, 0, 0xFFFF);
            return true;
        }

        // Mario exits the course if:
        // - All stars have been collected, or
        // - It's not a noExit star and Non-Stop mode is disabled
        bool noExit = !OMM_ALL_STARS && (OMM_STARS_NON_STOP || (o->oInteractionSubtype & INT_SUBTYPE_NO_EXIT));
        if (!noExit) {
            m->hurtCounter = 0;
            m->healCounter = 0;
            omm_mario_unset_cap(m);
            drop_queued_background_music();
            fadeout_level_music(126);
            omm_render_course_complete_init();
        } else if (OMM_STARS_NON_STOP && omm_mario_is_capture(m)) {
            gOmmMario->capture.starDance = true;
            m->actionTimer = 0;
            return true;
        }

        // Action
        // Unpossess now, so the correct star grab action is selected
        omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
        u32 starGrabAction = (noExit ? ACT_STAR_DANCE_NO_EXIT : ACT_STAR_DANCE_EXIT);
        if (m->action & ACT_FLAG_SWIMMING) starGrabAction = ACT_STAR_DANCE_WATER;
        if (m->action & ACT_FLAG_METAL_WATER) starGrabAction = ACT_STAR_DANCE_WATER;
        if (m->action & ACT_FLAG_AIR) starGrabAction = ACT_FALL_AFTER_STAR_GRAB;
        if (gOmmGlobals->yoshiMode) {
            omm_obj_spawn_yoshi_mode_yoshi(m->marioObj);
        }
        omm_mario_set_action(m, starGrabAction, noExit, 0);
        vec3f_copy(&m->marioObj->oPosX, m->pos);
        return true;
    }
    return false;
}

bool omm_mario_interact_warp(struct MarioState *m, struct Object *o) {
#if OMM_GAME_IS_R96X
    if (!(o->oInteractionSubtype & INT_SUBTYPE_FADING_WARP) && !omm_mario_is_emerging_from_pipe(m) && (o->oObjectID == 1)) {
        o->oInteractStatus = INT_STATUS_INTERACTED;
        m->interactObj = o;
        m->usedObj = o;
        mario_stop_riding_and_holding(m);
        // TODO: YOSHIMODE
        // Try to find a way to keep the capture while changing the character
        omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
        SFX(SOUND_MENU_ENTER_PIPE);
        play_transition(WARP_TRANSITION_FADE_INTO_MARIO, 0x15, 0x00, 0x00, 0x00);

        // Left to right: Luigi, Mario, Wario
        s32 target = OMM_PLAYER_MARIO;
        switch (o->oBehParams) {
            case 0: target = OMM_PLAYER_LUIGI; break;
            case 1: target = OMM_PLAYER_MARIO; break;
            case 2: target = OMM_PLAYER_WARIO; break;
        }

        // If already selected, select Peach if unlocked
        // Otherwise, switch character if unlocked
        if (omm_player_get_selected_index() == target && OMM_REWARD_IS_PLAYABLE_PEACH_UNLOCKED) {
            omm_player_select(OMM_PLAYER_PEACH);
            omm_mario_set_action(m, ACT_CHARACTER_SWITCH, TRUE, 0);
        } else if (omm_player_is_unlocked(target)) {
            omm_player_select(target);
            omm_mario_set_action(m, ACT_CHARACTER_SWITCH, TRUE, 0);
        } else {
            omm_mario_set_action(m, ACT_CHARACTER_SWITCH, FALSE, 0);
        }
        return true;
    }
#endif

    // Grand star?
    // Some rom-hacks replace the jumbo star cutscene by a warp to a 'we saved the world' cutscene level
    if (o->behavior == bhvGrandStar) {
        omm_speedrun_split(OMM_SPLIT_BOWSER);
        mario_stop_riding_and_holding(m);
        omm_mario_unpossess_object_before_warp(m);
        SFX(SOUND_MENU_STAR_SOUND);
        spawn_object(o, MODEL_NONE, bhvStarKeyCollectionPuffSpawner);
    }
    return interact_warp(m, INTERACT_WARP, o);
}

bool omm_mario_interact_cannon_base(struct MarioState *m, struct Object *o) {
    return interact_cannon_base(m, INTERACT_CANNON_BASE, o);
}

bool omm_mario_interact_bounce_top(struct MarioState *m, struct Object *o) {
    return interact_bounce_top(m, INTERACT_BOUNCE_TOP, o);
}

bool omm_mario_interact_water_ring(struct MarioState *m, struct Object *o) {
    return interact_water_ring(m, INTERACT_WATER_RING, o);
}

bool omm_mario_interact_bully(struct MarioState *m, struct Object *o) {
    return interact_bully(m, INTERACT_BULLY, o);
}

bool omm_mario_interact_flame(struct MarioState *m, struct Object *o) {

    // Extinguish flames with Gloom Vibe
    if (omm_peach_vibe_is_gloom() && omm_obj_is_flame(o)) {
        obj_spawn_white_puff(o, SOUND_GENERAL_FLAME_OUT);
        obj_mark_for_deletion(o);
        return false;
    }

#if OMM_GAME_IS_R96X
    // Disable Wario charge invincibility during Bowser 4
    if (OMM_SPARKLY_MODE_IS_ENABLED && omm_sparkly_is_bowser_4_battle() && m->action == ACT_WARIO_CHARGE) {
        o->oInteractStatus = INT_STATUS_INTERACTED;
        m->interactObj = o;
        m->marioObj->oMarioBurnTimer = 0;
        update_mario_sound_and_camera(m);
        drop_and_set_mario_action(m, ACT_BURNING_JUMP, 1);
        SFX(SOUND_MARIO_ON_FIRE);
        return true;
    }
#endif
    return interact_flame(m, INTERACT_FLAME, o);
}

bool omm_mario_interact_koopa_shell(struct MarioState *m, struct Object *o) {
    if (!(m->action & ACT_FLAG_RIDING_SHELL) && !(o->oInteractStatus & INT_STATUS_STOP_RIDING) && !o->oSubAction) {
        u32 interaction = determine_interaction(m, o);
        if ((interaction & (INT_GROUND_POUND_OR_TWIRL | INT_KICK | INT_HIT_FROM_ABOVE)) || m->action == ACT_WALKING || m->action == ACT_HOLD_WALKING) {
            m->interactObj = o;
            m->usedObj = o;
            m->riddenObj = o;
            o->oInteractStatus = INT_STATUS_INTERACTED;
            mario_drop_held_object(m);
            update_mario_sound_and_camera(m);
            audio_play_shell_music();
            if (m->action & ACT_FLAG_AIR) {
                omm_mario_set_action(m, ACT_RIDING_SHELL_FALL, 0, Z_TRIG);
            } else {
                omm_mario_set_action(m, ACT_RIDING_SHELL_GROUND, 0, Z_TRIG);
            }
            return true;
        }
        push_mario_out_of_object(m, o, 2);
    }
    return interact_koopa_shell(m, INTERACT_KOOPA_SHELL, o);
}

bool omm_mario_interact_bounce_top_2(struct MarioState *m, struct Object *o) {
    return interact_bounce_top(m, INTERACT_BOUNCE_TOP2, o);
}

bool omm_mario_interact_mr_blizzard(struct MarioState *m, struct Object *o) {
    if (omm_mario_has_metal_cap(m) || omm_mario_is_milk(m)) {
        omm_mario_metal_destroy_or_push_away_object(m, o);
        return false;
    }
    return interact_mr_blizzard(m, INTERACT_MR_BLIZZARD, o);
}

bool omm_mario_interact_hit_from_below(struct MarioState *m, struct Object *o) {
    return interact_hit_from_below(m, INTERACT_HIT_FROM_BELOW, o);
}

bool omm_mario_interact_text(struct MarioState *m, struct Object *o) {
    return interact_text(m, INTERACT_TEXT, o);
}

bool omm_mario_interact_tornado(struct MarioState *m, struct Object *o) {
    return interact_tornado(m, INTERACT_TORNADO, o);
}

bool omm_mario_interact_whirlpool(struct MarioState *m, struct Object *o) {
    if (omm_mario_has_metal_cap(m)) {
        return false;
    }
    return interact_whirlpool(m, INTERACT_WHIRLPOOL, o);
}

bool omm_mario_interact_clam_or_bubba(struct MarioState *m, struct Object *o) {
    if (omm_mario_has_metal_cap(m)) {
        return false;
    }
    return interact_clam_or_bubba(m, INTERACT_CLAM_OR_BUBBA, o);
}

bool omm_mario_interact_bbh_entrance(struct MarioState *m, struct Object *o) {
    return interact_bbh_entrance(m, INTERACT_BBH_ENTRANCE, o);
}

bool omm_mario_interact_snufit_bullet(struct MarioState *m, struct Object *o) {
    return interact_snufit_bullet(m, INTERACT_SNUFIT_BULLET, o);
}

bool omm_mario_interact_shock(struct MarioState *m, struct Object *o) {
    if (omm_mario_has_metal_cap(m)) {
        return false;
    }
    return interact_shock(m, INTERACT_SHOCK, o);
}

bool omm_mario_interact_igloo_barrier(struct MarioState *m, struct Object *o) {
    return interact_igloo_barrier(m, INTERACT_IGLOO_BARRIER, o);
}

bool omm_mario_interact_unknown_31(UNUSED struct MarioState *m, struct Object *o) {
    if (o->behavior == bhvOmmPeach) {
        o->oAction = 8;
        return true;
    }
    return false;
}

//
// Interaction handlers
//

typedef struct OmmInteractionHandler {
    u32 type;
    bool (*interact)(struct MarioState *, struct Object *);
} OmmInteractionHandler;

static const OmmInteractionHandler sOmmInteractionHandlers[] = {
    { INTERACT_HOOT,            omm_mario_interact_hoot },
    { INTERACT_GRABBABLE,       omm_mario_interact_grabbable },
    { INTERACT_DOOR,            omm_mario_interact_door },
    { INTERACT_DAMAGE,          omm_mario_interact_damage },
    { INTERACT_COIN,            omm_mario_interact_coin },
    { INTERACT_CAP,             omm_mario_interact_cap },
    { INTERACT_POLE,            omm_mario_interact_pole },
    { INTERACT_KOOPA,           omm_mario_interact_koopa },
    { INTERACT_UNKNOWN_08,      omm_mario_interact_unknown_08 },
    { INTERACT_BREAKABLE,       omm_mario_interact_breakable },
    { INTERACT_STRONG_WIND,     omm_mario_interact_strong_wind },
    { INTERACT_WARP_DOOR,       omm_mario_interact_warp_door },
    { INTERACT_STAR_OR_KEY,     omm_mario_interact_star_or_key },
    { INTERACT_WARP,            omm_mario_interact_warp },
    { INTERACT_CANNON_BASE,     omm_mario_interact_cannon_base },
    { INTERACT_BOUNCE_TOP,      omm_mario_interact_bounce_top },
    { INTERACT_WATER_RING,      omm_mario_interact_water_ring },
    { INTERACT_BULLY,           omm_mario_interact_bully },
    { INTERACT_FLAME,           omm_mario_interact_flame },
    { INTERACT_KOOPA_SHELL,     omm_mario_interact_koopa_shell },
    { INTERACT_BOUNCE_TOP2,     omm_mario_interact_bounce_top_2 },
    { INTERACT_MR_BLIZZARD,     omm_mario_interact_mr_blizzard },
    { INTERACT_HIT_FROM_BELOW,  omm_mario_interact_hit_from_below },
    { INTERACT_TEXT,            omm_mario_interact_text },
    { INTERACT_TORNADO,         omm_mario_interact_tornado },
    { INTERACT_WHIRLPOOL,       omm_mario_interact_whirlpool },
    { INTERACT_CLAM_OR_BUBBA,   omm_mario_interact_clam_or_bubba },
    { INTERACT_BBH_ENTRANCE,    omm_mario_interact_bbh_entrance },
    { INTERACT_SNUFIT_BULLET,   omm_mario_interact_snufit_bullet },
    { INTERACT_SHOCK,           omm_mario_interact_shock },
    { INTERACT_IGLOO_BARRIER,   omm_mario_interact_igloo_barrier },
    { INTERACT_UNKNOWN_31,      omm_mario_interact_unknown_31 },
};

#define ACT_OMM_SHOCKED_FROM_HIGH_FALL_ALLOWED_INTERACTIONS ( \
    INTERACT_GRABBABLE | \
    INTERACT_DAMAGE | \
    INTERACT_COIN | \
    INTERACT_UNKNOWN_08 | \
    INTERACT_STRONG_WIND | \
    INTERACT_STAR_OR_KEY | \
    INTERACT_CANNON_BASE | \
    INTERACT_BOUNCE_TOP | \
    INTERACT_WATER_RING | \
    INTERACT_BULLY | \
    INTERACT_FLAME | \
    INTERACT_BOUNCE_TOP2 | \
    INTERACT_MR_BLIZZARD | \
    INTERACT_HIT_FROM_BELOW | \
    INTERACT_TORNADO | \
    INTERACT_WHIRLPOOL | \
    INTERACT_CLAM_OR_BUBBA | \
    INTERACT_SNUFIT_BULLET | \
    INTERACT_SHOCK | \
    INTERACT_IGLOO_BARRIER \
)

static void omm_mario_process_interaction_types(struct MarioState *m) {

    // Process interactions per type
    for (u32 i = 0; i != array_length(sOmmInteractionHandlers); ++i) {
        const OmmInteractionHandler *handler = &sOmmInteractionHandlers[i];
        if (m->collidedObjInteractTypes & handler->type) {

            // Process interactions for each object
            for (s32 j = 0; j != m->marioObj->numCollidedObjs; ++j) {
                struct Object *obj = m->marioObj->collidedObjs[j];
                if (obj->oInteractType == handler->type) {

                    // Reset state, cancel interactions if intangible
                    sInvulnerable = (m->action != ACT_OMM_SHOCKED_FROM_HIGH_FALL) && ((m->action & ACT_FLAG_INVULNERABLE) || m->invincTimer != 0);
                    if (m->action & ACT_FLAG_INTANGIBLE) {
                        return;
                    }

                    // Process interaction
                    if (!(obj->oInteractStatus & INT_STATUS_INTERACTED)) {
                        handler->interact(m, obj);
                    }
                }
            }

            // Mark as interacted
            m->collidedObjInteractTypes &= ~handler->type;
        }
    }
}

void omm_mario_process_interactions(struct MarioState *m) {
    sDelayInvincTimer = FALSE;
    sInvulnerable = (m->action != ACT_OMM_SHOCKED_FROM_HIGH_FALL) && ((m->action & ACT_FLAG_INVULNERABLE) || m->invincTimer != 0);

    // Unset pole object if no pole interaction
    if (!(m->collidedObjInteractTypes & INTERACT_POLE)) {
        gOmmData->mario->state.poleObject = NULL;
    }

    // Ignore all interactions while Mario is grabbed
    if (omm_mario_is_grabbed(m)) {
        m->collidedObjInteractTypes = 0;
    }

    // Process some interactions during ACT_OMM_SHOCKED_FROM_HIGH_FALL
    if (m->action == ACT_OMM_SHOCKED_FROM_HIGH_FALL) {
        m->collidedObjInteractTypes &= ACT_OMM_SHOCKED_FROM_HIGH_FALL_ALLOWED_INTERACTIONS;
    }

    // Process interactions per type
    omm_mario_process_interaction_types(m);

    // Finish interactions
    if ((OMM_MOVESET_ODYSSEY && omm_mario_is_kicking(m)) || omm_mario_is_milk(m)) {
        m->flags &= ~(MARIO_PUNCHING | MARIO_KICKING | MARIO_TRIPPING);
    }
    m->invincTimer += (m->invincTimer > 0 && sDelayInvincTimer);
    mario_process_interactions(m);
}
