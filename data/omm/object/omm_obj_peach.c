#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

#if OMM_GAME_IS_SM64

static Vec3f OMM_PEACH_SPAWN_POSITIONS[] = {
    {     0, -1074,  2000 },
    {  3000, -1074,  2000 },
    { -1435, -1279,  2365 },
    {  -330,  -767,   500 },
    {  -350,  -767,  -150 },
    { -3100, -1279,  1800 },
    { -3100, -1279,   650 },
    { -3850, -1279,   650 },
    { -3850, -1381, -1100 },
    { -1850, -1177,   650 },
    { -1850, -1177, -1500 },
    { -1430, -1177, -1500 },
    {  -200, -1279, -1500 },
    {  -200, -1279,  -870 },
    {   500, -1177,  -870 },
    {  1000, -1177,  -870 },
    {   900, -1074, -2650 },
    {  3500, -1279, -2650 },
    {  -200, -1279, -3100 },
    { -1430, -1279, -3100 },
    { -1400, -1177, -3600 },
    { -2900, -1279, -3100 },
    { -2900, -1177, -1500 },
    {  2800, -2508,  -500 },
    {  5600, -1177,  -500 },
};

//
// Behavior
//

static void bhv_omm_peach_update() {
    struct Object *o = gCurrentObject;
    struct MarioState *m = gMarioState;
    switch (o->oAction) {

        // Hide in the shadows
        case 0: {
            cur_obj_hide();
            cur_obj_become_intangible();
            if (obj_get_count_with_behavior(bhvFlame) == 0) {
                for_each_in_(const BehaviorScript *, bhv, {
                    bhvToadMessage,
                    bhvMips,
                    bhvStar,
                    bhvStarSpawnCoordinates,
                    bhvSpawnedStar,
                    bhvSpawnedStarNoLevelExit,
                    bhvDoor,
                    bhvStarDoor,
                    bhvMessagePanel,
                    bhvOmmSparklyStarHint,
                    bhvOmmSparklyStar,
                    bhvOmmMips,
                }) {
                    for_each_object_with_behavior(obj, *bhv) {
                        obj_set_dormant(obj, true);
                    }
                }
                stop_background_music(SEQ_LEVEL_INSIDE_CASTLE);
                if (gOmmGlobals->yoshiMode) {
                    o->oAction = 8;
                } else {
                    o->oAction = 1;
                    omm_secrets_unlock(OMM_SECRET_DARK_BASEMENT);
                }
            }
        } break;

        // Wait a bit before spawning
        case 1: {
            o->oOpacity = 0;
            if (o->oTimer >= 450) {
                o->oAction = 2;
            }
        } break;

        // Select a position to spawn
        case 2: {
            o->oOpacity = 0;
            obj_set_pos_vec3f(o, OMM_PEACH_SPAWN_POSITIONS[random_u16() % array_length(OMM_PEACH_SPAWN_POSITIONS)]);
            f32 distToMario = obj_get_horizontal_distance(o, m->marioObj);
            if (distToMario > 4000.f && distToMario < 5000.f) {
                o->oAction = 3;
                obj_anim_play_with_sound(o, 6, 1.f, NO_SOUND, true);
            }
        } break;

        // Fade-in, look at Mario
        case 3: {
            cur_obj_unhide();
            o->oOpacity = relerp_0_1_s(o->oTimer, 0, 30, 0x00, 0x40);
            if (o->oTimer % 300 == 0) {
                obj_play_sound(o, SOUND_OBJ_BOO_LAUGH_LONG);
                o->oSubAction++;
            }
            o->oFaceAngleYaw = obj_angle_to_object(o, m->marioObj);
            f32 distToMario = obj_get_horizontal_distance(o, m->marioObj);
            if (distToMario < 500.f) {
                o->oAction = 4;
                obj_anim_play_with_sound(o, 0, 1.f, NO_SOUND, true);
            } else if (distToMario > 6000.f) {
                o->oAction = 6;
            } else if (o->oSubAction >= 7) {
                o->oAction = 7;
                obj_anim_play_with_sound(o, 4, 1.f, NO_SOUND, true);
                set_background_music(0, SEQ_EVENT_ENDLESS_STAIRS, 0);
#if OMM_GAME_IS_R96X
                play_music(SEQ_PLAYER_LEVEL, SEQ_EVENT_ENDLESS_STAIRS, 0);
#endif
                fade_volume_scale(SEQ_PLAYER_LEVEL, 0, 1);
            }
        } break;

        // Turn around
        case 4: {
            if (obj_anim_is_at_end(o)) {
                o->oAction = 5;
                obj_anim_play_with_sound(o, 1, 1.f, NO_SOUND, true);
            }
        } break;

        // Fade-out
        case 5: {
            o->oOpacity = relerp_0_1_s(o->oAnimFrame, 0, o->oCurrAnim->mLoopEnd - 1, 0x40, 0x00);
            if (o->oOpacity == 0) {
                o->oAction = 2;
            }
        } break;

        // Fade-out, being too far from Mario
        case 6: {
            o->oOpacity -= 8;
            if (o->oOpacity <= 0) {
                o->oOpacity = 0;
                o->oAction = 2;
            }
        } break;

        // Chase Mario
        case 7: {
            Vec3f velToMario = {
                m->pos[0] - o->oPosX,
                m->pos[1] - o->oPosY,
                m->pos[2] - o->oPosZ,
            };
            vec3f_set_mag(velToMario, 20.f);
            o->oPosX += velToMario[0];
            o->oPosY += velToMario[1];
            o->oPosZ += velToMario[2];
            o->oFaceAngleYaw = atan2s(velToMario[2], velToMario[0]);
            cur_obj_become_tangible();
        } break;

        // Caught Mario
        case 8: {
            cur_obj_hide();
            cur_obj_become_intangible();
            stop_background_music(SEQ_EVENT_ENDLESS_STAIRS);
            play_sound(SOUND_MARIO_WAAAOOOW, gGlobalSoundArgs);
            initiate_warp(LEVEL_CASTLE_GROUNDS, 1, 0x03, 0);
            play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 1, 0, 0, 0);
            level_set_transition(90, NULL);
            warp_special(0);
            o->oAction = 9;
        } break;

        // Idle
        case 9: {
            stop_background_music(SEQ_EVENT_ENDLESS_STAIRS);
        } break;
    }
    if (o->oAnimID == 6) {
        obj_anim_clamp_frame(o, 0, 30);
    }
    if (o->oAction >= 1) {
        omm_mario_unset_cap(m);
        omm_peach_vibe_deactivate(m);
        omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
        if (m->heldObj || m->riddenObj) {
            omm_mario_set_action(m, ACT_FREEFALL, 0, 0);
        }
        mario_stop_riding_and_holding(m);
        gOmmAllow->captures = false;
        gOmmAllow->capModifier = false;
        gOmmAllow->yoshiSummon = false;
        gOmmAllow->vibes = false;
    }
}

extern void gfx_texture_set_level_palette_color(f32 r, f32 g, f32 b);

OMM_ROUTINE_UPDATE(bhv_omm_peach_update_basement_darkness) {
    struct Object *peach = obj_get_first_with_behavior(bhvOmmPeach);
    if (peach) {
        if (peach->oAction == 7) {
            f32 distToMario = obj_get_horizontal_distance(peach, gMarioObject);
            f32 t = invlerp_0_1_f(distToMario, 2000.f, 500.f);
            gfx_texture_set_level_palette_color(lerp_f(t, 0.02f, 0.5f), 0.02f, 0.02f);
            fade_volume_scale(SEQ_PLAYER_LEVEL, lerp_s(t, 0, 160), 1);
        } else if (peach->oAction >= 1) {
            gfx_texture_set_level_palette_color(0.02f, 0.02f, 0.02f);
        } else {
            s32 remainingFlames = obj_get_count_with_behavior(bhvFlame);
            f32 intensityModifier = clamp_f(remainingFlames / 10.f, 0.02f, 1.f);
            gfx_texture_set_level_palette_color(intensityModifier, intensityModifier, intensityModifier);
        }
    }
}

#if OMM_GAME_IS_R96X

OMM_ROUTINE_PRE_RENDER(bhv_omm_peach_stop_r96_music) {
    struct Object *peach = obj_get_first_with_behavior(bhvOmmPeach);
    if (peach && peach->oAction >= 1) {
        music_stop();
    }
}

#endif

#endif

const BehaviorScript bhvOmmPeach[] = {
    OBJ_TYPE_LEVEL,
#if OMM_GAME_IS_SM64
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_LOAD_ANIMATIONS(oAnimations, peach_seg5_anims_0501C41C),
    BHV_ANIMATE(6),
    BHV_SET_HITBOX(50, 200),
    BHV_SET_INTERACT_TYPE(INTERACT_UNKNOWN_31),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_peach_update),
    BHV_END_LOOP(),
#else
    BHV_DEACTIVATE(),
#endif
};

#if OMM_GAME_IS_SM64
OMM_ROUTINE_UPDATE(bhv_omm_peach_update_boo_laugh) {
    static uintptr_t sBooLaughLongLayerNote1Offset = 0;
    extern ALSeqFile *gSeqFileHeader;
    if (!sBooLaughLongLayerNote1Offset && gSeqFileHeader && gSeqFileHeader->seqArray[SEQ_SOUND_PLAYER].offset && gSeqFileHeader->seqArray[SEQ_SOUND_PLAYER].len) {
        for (s32 i = 0; i < gSeqFileHeader->seqArray[SEQ_SOUND_PLAYER].len - 3; ++i) {
            u32 *p = (u32 *) (gSeqFileHeader->seqArray[SEQ_SOUND_PLAYER].offset + i);
            if (*p == 0xFF7F3277) {
                sBooLaughLongLayerNote1Offset = (uintptr_t) i;
                break;
            }
        }
    }
    if (sBooLaughLongLayerNote1Offset && gSequencePlayers[SEQ_PLAYER_SFX].seqData) {
        u16 *offset = (u16 *) (gSequencePlayers[SEQ_PLAYER_SFX].seqData + sBooLaughLongLayerNote1Offset);
        if (!omm_is_main_menu() && obj_get_first_with_behavior(bhvOmmPeach)) {
            *offset = 0x6068;
        } else {
            *offset = 0x3277;
        }
    }
}
#endif

//
// Spawner
//

struct Object *omm_obj_spawn_peach(struct Object *o) {
    struct Object *peach = obj_spawn_from_geo(o, peach_geo, bhvOmmPeach);
    obj_set_always_rendered(peach, true);
    obj_scale(peach, 1.f);
    peach->oAction = 0;
    peach->oRoom = -1;
    peach->oOpacity = 0;
    return peach;
}
