#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "game/paintings.h"
#include "game/object_list_processor.h"

//
// Geo
//

static void *geo_nop(UNUSED s32 callContext, UNUSED void *node, UNUSED void *c) {
    return NULL;
}

static void *geo_rotate_3d_coin(s32 callContext, void *node, UNUSED void *c) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct Object *obj = (struct Object *) gCurGraphNodeObject;
        obj->oFaceAnglePitch = obj->oFaceAngleRoll = 0;
        obj->oMoveAnglePitch = obj->oMoveAngleRoll = 0;
        struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) ((struct GraphNode *) node)->next;
        vec3s_set(gCurGraphNodeObject->angle, 0, 0, 0);
        vec3s_set(rotNode->rotation, 0, obj->oTimer * 0x800, 0);
    }
    return NULL;
}

//
// Actors
//

#define define_actor(geo, ...) { (const void *) geo, { #geo, ##__VA_ARGS__, NULL } }
static const struct { const void *layout; const char *names[8]; } ACTOR_LAYOUT_REFS[] = {
define_actor(amp_geo),
#if OMM_GAME_IS_SMMS
define_actor(Bee_geo),
#endif
define_actor(birds_geo),
define_actor(blargg_geo),
define_actor(blue_coin_switch_geo),
define_actor(black_bobomb_geo),
define_actor(bobomb_buddy_geo),
define_actor(boo_geo),
define_actor(boo_castle_geo),
#if OMM_GAME_IS_R96X
define_actor(boo_key_geo),
#endif
define_actor(bookend_geo),
define_actor(bookend_part_geo),
define_actor(bowling_ball_geo),
define_actor(bowling_ball_track_geo),
define_actor(bowser_geo),
define_actor(bowser2_geo),
define_actor(bowser_bomb_geo),
define_actor(bowser_flames_geo),
define_actor(bowser_impact_smoke_geo),
define_actor(bowser_1_yellow_sphere_geo),
define_actor(invisible_bowser_accessory_geo),
define_actor(bowser_key_geo),
define_actor(bowser_key_cutscene_geo),
define_actor(breakable_box_geo),
#if !OMM_GAME_IS_R96X
define_actor(breakable_box_small_geo),
#endif
define_actor(bub_geo),
define_actor(bubba_geo),
define_actor(bubble_geo),
define_actor(bullet_bill_geo),
define_actor(bully_geo),
define_actor(bully_boss_geo),
define_actor(burn_smoke_geo),
define_actor(butterfly_geo),
define_actor(cannon_barrel_geo),
define_actor(cannon_base_geo),
define_actor(cap_switch_geo),
#if OMM_GAME_IS_R96X
define_actor(cap_switch_base_geo),
#endif
define_actor(cartoon_star_geo),
define_actor(chain_chomp_geo),
define_actor(checkerboard_platform_geo),
define_actor(chilly_chief_geo),
define_actor(chilly_chief_big_geo),
define_actor(chuckya_geo),
define_actor(clam_shell_geo),
define_actor(yellow_coin_geo),
define_actor(yellow_coin_no_shadow_geo),
define_actor(blue_coin_geo),
define_actor(blue_coin_no_shadow_geo),
define_actor(red_coin_geo),
define_actor(red_coin_no_shadow_geo),
define_actor(dirt_animation_geo),
define_actor(dorrie_geo),
define_actor(cabin_door_geo),
define_actor(castle_door_geo),
define_actor(castle_door_0_star_geo),
define_actor(castle_door_1_star_geo),
define_actor(castle_door_3_stars_geo),
define_actor(haunted_door_geo),
define_actor(hazy_maze_door_geo),
define_actor(metal_door_geo),
define_actor(key_door_geo),
define_actor(wooden_door_geo),
define_actor(enemy_lakitu_geo),
define_actor(exclamation_box_geo),
define_actor(exclamation_box_outline_geo),
define_actor(explosion_geo),
define_actor(eyerok_left_hand_geo),
define_actor(eyerok_right_hand_geo),
define_actor(fish_geo),
define_actor(cyan_fish_geo),
define_actor(flyguy_geo),
define_actor(red_flame_geo),
define_actor(red_flame_shadow_geo),
define_actor(blue_flame_geo),
#if OMM_GAME_IS_R96X
define_actor(friendly_blargg_geo),
#endif
define_actor(fwoosh_geo),
define_actor(goomba_geo),
#if OMM_GAME_IS_SMMS
define_actor(goombone_geo),
#endif
define_actor(haunted_cage_geo),
define_actor(haunted_chair_geo),
define_actor(heart_geo),
define_actor(heave_ho_geo),
define_actor(hoot_geo),
define_actor(king_bobomb_geo),
define_actor(klepto_geo),
define_actor(koopa_with_shell_geo),
define_actor(koopa_without_shell_geo),
define_actor(koopa_flag_geo),
define_actor(koopa_shell_geo),
define_actor(lakitu_geo),
#if OMM_GAME_IS_R96X
define_actor(luigi_geo),
define_actor(luigis_cap_geo),
define_actor(luigis_metal_cap_geo),
define_actor(luigis_wing_cap_geo),
define_actor(luigis_winged_metal_cap_geo),
#endif
define_actor(mad_piano_geo),
define_actor(manta_seg5_geo_05008D14),
#if OMM_GAME_IS_R96X
define_actor(mario_geo, "waluigi_geo", "toad_player_geo"),
define_actor(marios_cap_geo, "waluigis_cap_geo", "toads_cap_geo"),
define_actor(marios_metal_cap_geo, "waluigis_metal_cap_geo", "toads_metal_cap_geo"),
define_actor(marios_wing_cap_geo, "waluigis_wing_cap_geo", "toads_wing_cap_geo"),
define_actor(marios_winged_metal_cap_geo, "waluigis_winged_metal_cap_geo"),
#else
define_actor(mario_geo, "luigi_geo", "wario_geo", "waluigi_geo", "toad_player_geo"),
define_actor(marios_cap_geo, "luigis_cap_geo", "warios_cap_geo", "waluigis_cap_geo", "toads_cap_geo"),
define_actor(marios_metal_cap_geo, "luigis_metal_cap_geo", "warios_metal_cap_geo", "waluigis_metal_cap_geo", "toads_metal_cap_geo"),
define_actor(marios_wing_cap_geo, "luigis_wing_cap_geo", "warios_wing_cap_geo", "waluigis_wing_cap_geo", "toads_wing_cap_geo"),
define_actor(marios_winged_metal_cap_geo, "luigis_winged_metal_cap_geo", "warios_winged_metal_cap_geo", "waluigis_winged_metal_cap_geo"),
#endif
define_actor(metal_box_geo),
define_actor(metallic_ball_geo),
define_actor(mips_geo),
define_actor(mist_geo),
define_actor(moneybag_geo),
define_actor(monty_mole_geo),
#if OMM_GAME_IS_R96X
define_actor(motos_geo),
#endif
define_actor(mr_blizzard_geo),
define_actor(mr_blizzard_hidden_geo),
define_actor(mr_i_geo),
define_actor(mr_i_iris_geo),
define_actor(mushroom_1up_geo),
define_actor(number_geo),
define_actor(peach_geo),
define_actor(penguin_geo),
define_actor(piranha_plant_geo),
define_actor(pokey_head_geo),
define_actor(pokey_body_part_geo),
define_actor(purple_marble_geo),
define_actor(purple_switch_geo),
define_actor(scuttlebug_geo),
define_actor(seaweed_geo),
define_actor(skeeter_geo),
define_actor(small_key_geo),
define_actor(small_water_splash_geo),
define_actor(smoke_geo),
define_actor(snufit_geo),
define_actor(sparkles_geo),
define_actor(sparkles_animation_geo),
define_actor(spindrift_geo),
define_actor(spiny_geo),
define_actor(spiny_ball_geo),
define_actor(star_geo),
define_actor(transparent_star_geo),
define_actor(sushi_geo),
define_actor(swoop_geo),
define_actor(thwomp_geo),
define_actor(toad_geo),
define_actor(treasure_chest_base_geo),
define_actor(treasure_chest_lid_geo),
define_actor(bubbly_tree_geo),
define_actor(spiky_tree_geo),
define_actor(snow_tree_geo),
define_actor(palm_tree_geo),
define_actor(leaves_geo),
define_actor(tweester_geo),
define_actor(ukiki_geo),
define_actor(unagi_geo),
#if OMM_GAME_IS_R96X
define_actor(wario_geo),
define_actor(wario_coin_tree_geo),
define_actor(wario_coin_house_geo),
define_actor(wario_coin_koopa_geo),
define_actor(wario_coin_lunar_geo),
define_actor(wario_coin_mario_geo),
define_actor(wario_coin_pumpkin_geo),
define_actor(warios_cap_geo),
define_actor(warios_metal_cap_geo),
define_actor(warios_wing_cap_geo),
define_actor(warios_winged_metal_cap_geo),
define_actor(warp_pipe_boo_geo),
define_actor(warp_pipe_boo_red_geo),
define_actor(warp_pipe_boo_green_locked_geo),
define_actor(warp_pipe_boo_green_unlocked_geo),
define_actor(warp_pipe_boo_yellow_locked_geo),
define_actor(warp_pipe_boo_yellow_unlocked_geo),
#endif
define_actor(warp_pipe_geo),
define_actor(water_bomb_geo),
define_actor(water_bomb_shadow_geo),
define_actor(water_ring_geo),
define_actor(water_splash_geo),
define_actor(idle_water_wave_geo),
define_actor(wave_trail_geo),
define_actor(white_particle_geo),
define_actor(white_puff_geo),
define_actor(whomp_geo),
define_actor(wiggler_head_geo),
define_actor(wiggler_body_geo),
define_actor(wooden_post_geo),
define_actor(wooden_signpost_geo),
define_actor(yellow_sphere_geo),
define_actor(yoshi_geo),
define_actor(yoshi_egg_geo),
#if OMM_GAME_IS_SMSR
define_actor(custom_shyguy_geo),
#endif
#define OMM_GEO_(geo, ...) { (const void *) geo, { #geo, ##__VA_ARGS__, NULL } },
#include "data/omm/object/omm_object_data_geo.inl"
#undef OMM_GEO
};

s32 omm_models_get_actor_count() {
    return sizeof(ACTOR_LAYOUT_REFS) / sizeof(ACTOR_LAYOUT_REFS[0]);
}

const char **omm_models_get_actor_names(s32 index) {
    return (const char **) ACTOR_LAYOUT_REFS[index].names;
}

const void *omm_models_get_actor_layout(s32 index) {
    return (const void *) ACTOR_LAYOUT_REFS[index].layout;
}

s32 omm_models_get_actor_index(const void *geo_layout) {
    for (s32 i = 0; i != omm_models_get_actor_count(); ++i) {
        if (ACTOR_LAYOUT_REFS[i].layout == geo_layout) {
            return i;
        }
    }
    return -1;
}

//
// Geo Functions
//

#define define_geo_function(func)       { (const void *) func, #func }
#define rename_geo_function(func, name) { (const void *) func, name }
static const struct { const void *func; const char *name; } GEO_FUNCTIONS[] = {
define_geo_function(geo_mirror_mario_set_alpha),
define_geo_function(geo_switch_mario_stand_run),
define_geo_function(geo_switch_mario_eyes),
define_geo_function(geo_mario_tilt_torso),
define_geo_function(geo_mario_head_rotation),
define_geo_function(geo_switch_mario_hand),
define_geo_function(geo_mario_hand_foot_scaler),
define_geo_function(geo_switch_mario_cap_effect),
define_geo_function(geo_switch_mario_cap_on_off),
define_geo_function(geo_mario_rotate_wing_cap_wings),
define_geo_function(geo_switch_mario_hand_grab_pos),
define_geo_function(geo_render_mirror_mario),
define_geo_function(geo_mirror_mario_backface_culling),
define_geo_function(geo_update_projectile_pos_from_parent),
define_geo_function(geo_update_layer_transparency),
define_geo_function(geo_switch_anim_state),
define_geo_function(geo_switch_area),
define_geo_function(geo_camera_main),
define_geo_function(geo_camera_fov),
define_geo_function(geo_envfx_main),
define_geo_function(geo_skybox_main),
define_geo_function(geo_wdw_set_initial_water_level),
define_geo_function(geo_movtex_pause_control),
define_geo_function(geo_movtex_draw_water_regions),
define_geo_function(geo_movtex_draw_nocolor),
define_geo_function(geo_movtex_draw_colored),
define_geo_function(geo_movtex_draw_colored_no_update),
define_geo_function(geo_movtex_draw_colored_2_no_update),
define_geo_function(geo_movtex_update_horizontal),
define_geo_function(geo_movtex_draw_colored_no_update),
define_geo_function(geo_painting_draw),
define_geo_function(geo_painting_update),
define_geo_function(geo_exec_inside_castle_light),
define_geo_function(geo_exec_flying_carpet_timer_update),
define_geo_function(geo_exec_flying_carpet_create),
define_geo_function(geo_exec_cake_end_screen),
define_geo_function(geo_cannon_circle_base),
define_geo_function(geo_move_mario_part_from_parent),
define_geo_function(geo_bits_bowser_coloring),
define_geo_function(geo_update_body_rot_from_parent),
define_geo_function(geo_switch_bowser_eyes),
define_geo_function(geo_switch_tuxie_mother_eyes),
define_geo_function(geo_update_held_mario_pos),
define_geo_function(geo_snufit_move_mask),
define_geo_function(geo_snufit_scale_body),
define_geo_function(geo_scale_bowser_key),
rename_geo_function(geo_rotate_3d_coin, "geo_rotate_coin"),
define_geo_function(geo_offset_klepto_held_object),
define_geo_function(geo_switch_peach_eyes),
};

void *omm_models_get_func_pointer(s32 index) {
    if (index < 0 || index >= (s32) array_length(GEO_FUNCTIONS)) return (void *) geo_nop;
    return (void *) GEO_FUNCTIONS[index].func;
}
