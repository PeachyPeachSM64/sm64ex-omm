#!/usr/bin/env python3

import os, sys

FILE_PATHS      = []
FILE_EXTENSIONS = [".c", ".h"]
EXCLUDE_FILES   = [".rej", ".porig"]
EXCLUDE_DIRS    = ["assets/", "bin/", "build/", "omm/", "tools/"]
COMMANDS        = {"-p": {"info": "Apply the patch.", "unpatch": False}, "-u": {"info": "Remove the patch", "unpatch": True}}
PATCH_BEFORE    = -1
PATCH_REPLACE   = 0
PATCH_AFTER     = +1

#
# Patch
#

def patch_string_in_file(filepath: str, location: str, where: str, what: str, how: int, unpatch: bool) -> bool:
    try:
        file = open(filepath, "r", newline="\n", encoding="utf-8", errors="ignore")
        data = file.read()
        file.close()

        i_location = data.find(location)
        if i_location == -1:
            print("[x] ERROR: File \"" + filepath + "\": Cannot find string \"" + location + "\".")
            return False

        i_what = data.find(what, i_location - len(what) if how == PATCH_BEFORE else 0)
        if unpatch:
            if i_what == -1:
                print("(!) Warning: File \"" + filepath + "\": " + location + " ... " + where + " has already been unpatched.")
                return True
        else:
            if i_what != -1:
                print("(!) Warning: File \"" + filepath + "\": " + location + " ... " + where + " has already been patched.")
                return True

        i_where = data.find(where, i_location)
        if i_where == -1 and (not unpatch or how != PATCH_REPLACE):
            print("[x] ERROR: File \"" + filepath + "\": Cannot find string \"" + where + "\".")
            return False

        if how == PATCH_BEFORE:
            if unpatch:
                new_data = data[:i_what] + data[i_where:]
            else:
                new_data = data[:i_where] + what + data[i_where:]
        elif how == PATCH_AFTER:
            if unpatch:
                new_data = data[:i_where + len(where)] + data[i_what + len(what):]
            else:
                new_data = data[:i_where + len(where)] + what + data[i_where + len(where):]
        elif how == PATCH_REPLACE:
            if unpatch:
                new_data = data[:i_what] + where + data[i_what + len(what):]
            else:
                new_data = data[:i_where] + what + data[i_where + len(where):]
        else:
            print("[x] ERROR: File \"" + filepath + "\": Argument 'how' must be either PATCH_BEFORE, PATCH_REPLACE or PATCH_AFTER.")
            return False

        try:
            new_file = open(filepath, "w", newline="\n", encoding="utf-8", errors="ignore")
            new_file.write(new_data)
            new_file.close()
            if unpatch:
                print("File \"" + filepath + "\": " + location + " ... " + where + " has successfully been unpatched.")
            else:
                print("File \"" + filepath + "\": " + location + " ... " + where + " has successfully been patched.")
            return True

        except IOError:
            print("[x] ERROR: File \"" + filepath + "\" cannot be modified.")
            return False

    except IOError:
        print("[x] ERROR: File \"" + filepath + "\" does not exist.")
        return False

#
# Delete
#

def delete_file(filepath: str, unpatch: bool) -> bool:
    if not unpatch:
        try:
            os.remove(filepath)
            print("File \"" + filepath + "\" has successfully been deleted.")
            return True
        except OSError:
            print("[x] ERROR: File \"" + filepath + "\" cannot be deleted.")
            return False
    return True

#
# Process
#

def process_command(filename: str, function, *args) -> bool:
    if len(sys.argv) < 2 or sys.argv[1] not in COMMANDS.keys():
        print("You must provide a valid command as first argument.")
        print("List of available commands:")
        for command, info in COMMANDS.items():
            print(command + " : " + info["info"])
        sys.exit(0)

    unpatch = COMMANDS[sys.argv[1]]["unpatch"]
    if filename in FILE_PATHS:
        return function(filename, *args, unpatch)
    for filepath in FILE_PATHS:
        if ("/" + filename) in filepath:
            return function(filepath, *args, unpatch)
    return False

#
# Do stuff
#

def do_patch_file(filename: str, location: str, where: str, what: str, how: int) -> bool:
    return process_command(filename, patch_string_in_file, location, where, what, how)

def do_undef_code(filename: str, start: str, end: str, index: int = 0) -> bool:
    if do_patch_file(filename, start, start, "#if 0 // {} [{}]\n".format(filename, index), PATCH_BEFORE):
        return do_patch_file(filename, end, end, "#endif // {} [{}]\n".format(filename, index), PATCH_BEFORE)
    return False

def do_delete_file(filename: str) -> bool:
    return process_command(filename, delete_file)

def do_delete_dir(dirname: str) -> bool:
    for filepath in FILE_PATHS:
        if ("/" + dirname + "/") in filepath:
            do_delete_file(filepath)

#
# Entry point
#

if __name__ == "__main__":

    # Gather all source filepaths
    for dirpath, _, filenames in os.walk("."):
        if not any([x in dirpath for x in EXCLUDE_DIRS]):
            for filename in filenames:
                if any([x in filename for x in FILE_EXTENSIONS]) and not any([x in filename for x in EXCLUDE_FILES]):
                    FILE_PATHS.append(dirpath + "/" + filename)

    # Dir deletion
    do_delete_dir("goddard")

    # File deletion
    do_delete_file("actors/mr_i_eyeball/model.inc.c")
    do_delete_file("behavior_script.c")
    do_delete_file("behavior_script.h")
    do_delete_file("blue_coin.inc.c")
    do_delete_file("cliopts.c")
    do_delete_file("cliopts.h")
    do_delete_file("config_camera.h")
    do_delete_file("config_collision.h")
    do_delete_file("config_graphics.h")
    do_delete_file("config_world.h")
    do_delete_file("controller_keyboard.c")
    do_delete_file("controller_sdl.c")
    do_delete_file("controller_sdl1.c")
    do_delete_file("controller_sdl2.c")
    do_delete_file("dirtree.c")
    do_delete_file("dirtree.h")
    do_delete_file("donut_platform.inc.c")
    do_delete_file("dorrie.inc.c")
    do_delete_file("dynos_c.cpp")
    do_delete_file("dynos_gfx_init.cpp")
    do_delete_file("dynos_gfx_load.cpp")
    do_delete_file("dynos_gfx_read.cpp")
    do_delete_file("dynos_gfx_texture.cpp")
    do_delete_file("dynos_gfx_update.cpp")
    do_delete_file("dynos_gfx_write.cpp")
    do_delete_file("dynos_level.cpp")
    do_delete_file("dynos_main.cpp")
    do_delete_file("dynos_misc.cpp")
    do_delete_file("dynos_opt_config.cpp")
    do_delete_file("dynos_opt_cont.cpp")
    do_delete_file("dynos_opt_render.cpp")
    do_delete_file("dynos_opt_vanilla_c.c")
    do_delete_file("dynos_opt_vanilla.cpp")
    do_delete_file("dynos_opt.cpp")
    do_delete_file("dynos_warps.cpp")
    do_delete_file("extended_bounds.h")
    do_delete_file("file_select.c")
    do_delete_file("file_select.h")
    do_delete_file("fs.c")
    do_delete_file("fs.h")
    do_delete_file("fs_packtype_dir.c")
    do_delete_file("fs_packtype_zip.c")
    do_delete_file("gbi.h")
    do_delete_file("geo_layout.c")
    do_delete_file("geo_layout.h")
    do_delete_file("gfx_pc.c")
    do_delete_file("gfx_sdl.c")
    do_delete_file("gfx_sdl1.c")
    do_delete_file("gfx_sdl2.c")
    do_delete_file("got_milk.inc.c")
    do_delete_file("graph_node.c")
    do_delete_file("graph_node.h")
    do_delete_file("graph_node_manager.c")
    do_delete_file("heave_ho.inc.c")
    do_delete_file("hidden_star.inc.c")
    do_delete_file("level_geo.c")
    do_delete_file("level_script.c")
    do_delete_file("level_script.h")
    do_delete_file("levels/ending/leveldata.c")
    do_delete_file("levels/menu/script.c")
    do_delete_file("levels/menu/custom.script.c")
    do_delete_file("mario_step.c")
    do_delete_file("math_util.c")
    do_delete_file("math_util.h")
    do_delete_file("moneybag.inc.c")
    do_delete_file("monty_mole.inc.c")
    do_delete_file("motos.inc.c")
    do_delete_file("mushroom_1up.inc.c")
    do_delete_file("qol_defines.h")
    do_delete_file("r96_character_swap.c")
    do_delete_file("red_coin.inc.c")
    do_delete_file("rendering_graph_node.c")
    do_delete_file("rendering_graph_node.h")
    do_delete_file("save_file.c")
    do_delete_file("save_file.h")
    do_delete_file("screen_transition.c")
    do_delete_file("shadow.c")
    do_delete_file("shadow.h")
    do_delete_file("skybox.c")
    do_delete_file("star_select.c")
    do_delete_file("surface_collision.c")
    do_delete_file("surface_collision.h")
    do_delete_file("surface_load.c")
    do_delete_file("surface_load.h")
    do_delete_file("surface_terrains.h")
    do_delete_file("text_save.inc.h")
    do_delete_file("ukiki.inc.c")

    # Code deletion
    do_undef_code("bettercamera.c",            "s32 ray_surface_intersect(",                          "static s32 puppycam_check_volume_bounds(")
    do_undef_code("ingame_menu.c",             "void render_dialog_box_type(",                        "void change_and_flash_dialog_text_color_lines(")
    do_undef_code("interaction.c",             "u32 determine_interaction(",                          "u32 attack_object(", 0)
    do_undef_code("interaction.c",             "void push_mario_out_of_object(",                      "void bounce_back_from_attack(", 1)
    do_undef_code("mario.c",                   "s32 is_anim_at_end(",                                 "void update_mario_pos_for_anim(", 0)
    do_undef_code("mario.c",                   "struct Surface *resolve_and_return_wall_collisions(", "s32 mario_facing_downhill(", 1)
    do_undef_code("mario.c",                   "#ifdef BETTER_WALL_COLLISION",                        "f32 vec3f_find_ceil(", 2)
    do_undef_code("mario_actions_submerged.c", "static f32 get_buoyancy(",                            "static BAD_RETURN(u32) update_water_pitch(")

    # Header patches
    do_patch_file("audio_sdl2.c",    "#include <SDL2/SDL.h>",              "#include <SDL2/SDL.h>",              "\n#include <stdio.h>",                                   PATCH_AFTER)
    do_patch_file("behavior_data.c", "const BehaviorScript bhvStarDoor[]", "const BehaviorScript bhvStarDoor[]", "#include \"data/omm/engine/behavior_commands.inl\"\n",   PATCH_BEFORE)
    do_patch_file("configfile.c",    "const char *configfile_name(",       "const char *configfile_name(",       "#include \"data/omm/engine/configfile.inl\"\n",          PATCH_BEFORE)
    do_patch_file("dynos_audio.cpp", "bool EndJingle = 0;",                "bool EndJingle = 0;",                "\n#include \"data/omm/peachy/omm_peach_vibes_r96.inl\"", PATCH_AFTER)
    do_patch_file("gfx_dxgi.cpp",    "static struct {",                    "} dxgi;",                            "\n#include \"data/omm/engine/gfx_dxgi.inl\"",            PATCH_AFTER)
    do_patch_file("options_menu.c",  "SubMenu *currentMenu = &menuMain",   ";",                                  "\n#include \"data/omm/system/omm_options_menu.inl\"",    PATCH_AFTER)
    do_patch_file("txtconv.c",       "struct Character charmap",           "{",                                  "\n#include \"data/omm/system/omm_text_charmap.inl\"",    PATCH_AFTER)
    do_patch_file("sm64.h",          "SM64_H",                             "#include \"macros.h\"",              "\n#include \"data/omm/omm_includes.h\"",                 PATCH_AFTER)
    do_patch_file("ultra64.h",       "_ULTRA64_H_",                        "#include <PR/libultra.h>",           "\n#include \"data/omm/omm_macros.h\"",                   PATCH_AFTER)
    do_patch_file("types.h",         "_SM64_TYPES_H_",                     "struct GraphNode *children;",        "\nomm_GraphNode_extra_fields",                           PATCH_AFTER)
    do_patch_file("types.h",         "_SM64_TYPES_H_",                     "s32 animAccel;",                     "\nomm_AnimInfo_extra_fields",                            PATCH_AFTER)
    do_patch_file("types.h",         "_SM64_TYPES_H_",                     "Vec3f cameraToObject;",              "\nomm_GraphNodeObject_extra_fields",                     PATCH_AFTER)

    # Actors patches
    do_patch_file("mario/model.inc.c",      "", "", "#define static\n#define const\n", PATCH_AFTER)
    do_patch_file("mario_cap/model.inc.c",  "", "", "#define static\n#define const\n", PATCH_AFTER)
    do_patch_file("text/define_text.inc.c", "", "", "#define const\n",                 PATCH_AFTER)

    # Game patches
    do_patch_file("camera.c",                 "s16 find_in_bounds_yaw_wdw_bob_thi(",       "{",                                              "\nomm_patch__find_in_bounds_yaw_wdw_bob_thi__return_yaw",                 PATCH_AFTER)
    do_patch_file("chuckya.inc.c",            "void common_anchor_mario_behavior(",        "{",                                              "\nomm_patch__common_anchor_mario_behavior__obj_update_held_mario",        PATCH_AFTER)
    do_patch_file("external.c",               "void play_sound(",                          "{",                                              "\nomm_patch__play_sound__play_character_sound",                           PATCH_AFTER)
    do_patch_file("external.c",               "void stop_background_music(",               "foundIndex = sBackgroundMusicQueueSize;",        "\nomm_patch__stop_background_music__fix_boss_music",                      PATCH_AFTER)
    do_patch_file("ingame_menu.c",            "void render_dialog_entries(",               "dialog->linesPerBox * 2;",                       "\nomm_patch__render_dialog_entries__fix_dialog_box_text_lower_bound",     PATCH_AFTER)
    do_patch_file("interaction.c",            "void check_death_barrier(",                 "{",                                              "\nomm_patch__check_death_barrier__cheat_walk_on_death_barrier",           PATCH_AFTER)
    do_patch_file("interaction.c",            "void check_lava_boost(",                    "{",                                              "\nomm_patch__check_lava_boost__cheat_walk_on_lava",                       PATCH_AFTER)
    do_patch_file("level_update.c",           "s16 level_trigger_warp(",                   "{",                                              "\nomm_patch__level_trigger_warp__check_death_warp",                       PATCH_AFTER)
    do_patch_file("mario.c",                  "s32 mario_get_floor_class(",                "{",                                              "\nomm_patch__mario_get_floor_class__cheat_walk_on_slope",                 PATCH_AFTER)
    do_patch_file("mario.c",                  "u32 mario_floor_is_slippery(",              "{",                                              "\nomm_patch__mario_floor_is_slippery__cheat_walk_on_slope",               PATCH_AFTER)
    do_patch_file("mario.c",                  "s32 set_water_plunge_action(",              "{",                                              "\nomm_patch__set_water_plunge_action__check_flooded",                     PATCH_AFTER)
    do_patch_file("mario_actions_airborne.c", "void update_air_with_turn(",                "{",                                              "\nomm_patch__update_air_with_turn__update_air_with_turn",                 PATCH_AFTER)
    do_patch_file("mario_actions_airborne.c", "void update_air_without_turn(",             "{",                                              "\nomm_patch__update_air_without_turn__update_air_without_turn",           PATCH_AFTER)
    do_patch_file("mario_actions_airborne.c", "u32 common_air_action_step(",               "case AIR_STEP_HIT_WALL:",                        "\nomm_patch__common_air_action_step__check_wall_slide",                   PATCH_AFTER)
    do_patch_file("mario_actions_airborne.c", "s32 act_lava_boost(",                       "if (m->floor->type == SURFACE_BURNING",          "  omm_patch__act_lava_boost__cheat_walk_on_lava",                         PATCH_AFTER)
    do_patch_file("mario_actions_cutscene.c", "s32 set_mario_npc_dialog(",                 ";",                                              "\nomm_patch__set_mario_npc_dialog__check_npc_dialog",                     PATCH_AFTER)
    do_patch_file("mario_actions_cutscene.c", "s32 check_for_instant_quicksand(",          "&& m->action != ACT_QUICKSAND_DEATH) {",         "\nomm_patch__check_for_instant_quicksand__fix_downwarp",                  PATCH_AFTER)
    do_patch_file("mario_actions_moving.c",   "void apply_slope_accel(",                   "} else",                                         "  omm_patch__apply_slope_accel__not_peach_vibe_gloom",                    PATCH_AFTER)
    do_patch_file("mario_actions_moving.c",   "void update_walking_speed(",                "{",                                              "\nomm_patch__update_walking_speed__update_walking_speed",                 PATCH_AFTER)
    do_patch_file("mario_cheats.c",           "s32 cheats_play_as(",                       "{",                                              "\nomm_patch__cheats_play_as__disable",                                    PATCH_AFTER)
    do_patch_file("obj_behaviors_2.c",        "static s32 obj_resolve_object_collisions(", "if (otherObject != gMarioObject",                "  omm_patch__obj_resolve_object_collisions__ignore_capture_goomba_stack", PATCH_AFTER)
    do_patch_file("object_helpers.c",         "void cur_obj_apply_drag_xz(",               "{",                                              "\nomm_patch__cur_obj_apply_drag_xz__fix_xz_vel",                          PATCH_AFTER)
    do_patch_file("object_helpers.c",         "void obj_set_hitbox(",                      "obj->header.gfx.scale[1] * hitbox->downOffset;", "\nomm_patch__obj_set_hitbox__obj_fix_hitbox",                             PATCH_AFTER)
    do_patch_file("object_helpers.c",         "s32 cur_obj_update_dialog(",                "{",                                              "\nomm_patch__cur_obj_update_dialog__skip_if_capture",                     PATCH_AFTER)
    do_patch_file("object_helpers.c",         "s32 cur_obj_update_dialog_with_cutscene(",  "{",                                              "\nomm_patch__cur_obj_update_dialog_with_cutscene__skip_if_capture",       PATCH_AFTER)
    do_patch_file("object_helpers.c",         "s32 cur_obj_has_model(",                    "} else",                                         "  omm_patch__cur_obj_has_model__check_georef",                            PATCH_AFTER)
    do_patch_file("object_list_processor.c",  "void unload_objects_from_area(",            "gObjectLists = gObjectListArray;",               "\nomm_patch__unload_objects_from_area__keep_held_ridden_capture_alive",   PATCH_AFTER)
    do_patch_file("level_update.c",           "void load_level_init_text(",                "if (!gotAchievement",                            "  omm_patch__load_level_init_text__skip_entry_dialog_if_non_stop",        PATCH_AFTER)
    do_patch_file("paintings.c",              "Gfx *render_painting(",                     "// Draw the triangles individually",             "\nomm_patch__render_painting__interpolate_painting",                      PATCH_AFTER)
    do_patch_file("paintings.c",              "Gfx *geo_painting_update(",                 "= gPaintingUpdateCounter;",                      "\nomm_patch__geo_painting_update__fix_floor_pointer",                     PATCH_AFTER)
    do_patch_file("r96_audio.c",              "const char *r96_get_intended_level_music(", "if (gCurrLevelNum == LEVEL_CASTLE_GROUNDS) {",   "\nomm_patch__r96_get_intended_level_music__bowser_4_music",               PATCH_AFTER)
    do_patch_file("r96_audio.c",              "const char *r96_get_intended_level_music(", "if (gCurrLevelNum == LEVEL_WDW",                 "  omm_patch__r96_get_intended_level_music__fix_milk_music",               PATCH_AFTER)
    do_patch_file("seqplayer.c",              "void sequence_channel_process_script(",     "case 0x00: // chan_testlayerfinished",           "\nif (loBits >= LAYERS_MAX) break;",                                      PATCH_AFTER)
