import os
import sys


def patch_string_in_file(filepath, location, where, what, how, unpatch):
    try:
        file = open(filepath, "r", newline="\n", encoding="utf-8", errors="ignore")
        data = file.read()
        file.close()
        
        i_location = data.find(location)
        if i_location == -1:
            print("[x] ERROR: File \"" + filepath + "\": Cannot find string \"" + location + "\".")
            return 0
            
        i_what = data.find(what, i_location - len(what) if how == -1 else 0)
        if unpatch:
            if i_what == -1:
                print("(!) Warning: File \"" + filepath + "\": " + location + " ... " + where + " has already been unpatched.")
                return 1
        else:
            if i_what != -1:
                print("(!) Warning: File \"" + filepath + "\": " + location + " ... " + where + " has already been patched.")
                return 1
            
        i_where = data.find(where, i_location)
        if i_where == -1 and (not unpatch or how != 0):
            print("[x] ERROR: File \"" + filepath + "\": Cannot find string \"" + where + "\".")
            return 0
            
        if how == -1:
            if unpatch:
                new_data = data[:i_what] + data[i_where:]
            else:
                new_data = data[:i_where] + what + data[i_where:]
        elif how == +1:
            if unpatch:
                new_data = data[:i_where + len(where)] + data[i_what + len(what):]
            else:
                new_data = data[:i_where + len(where)] + what + data[i_where + len(where):]
        elif how == 0:
            if unpatch:
                new_data = data[:i_what] + where + data[i_what + len(what):]
            else:
                new_data = data[:i_where] + what + data[i_where + len(where):]
        else:
            print("[x] ERROR: File \"" + filepath + "\": Argument 'how' must be either 0, -1 or +1.")
            return 0
            
        try:
            new_file = open(filepath, "w", newline="\n", encoding="utf-8", errors="ignore")
            new_file.write(new_data)
            new_file.close()
            if unpatch:
                print("File \"" + filepath + "\": " + location + " ... " + where + " has successfully been unpatched.")
            else:
                print("File \"" + filepath + "\": " + location + " ... " + where + " has successfully been patched.")
            return 1
            
        except IOError:
            print("[x] ERROR: File \"" + filepath + "\" cannot be modified.")
            return 0
            
    except IOError:
        print("[x] ERROR: File \"" + filepath + "\" does not exist.")
        return 0
        
        
def delete_file(filepath, unpatch):
    if not unpatch:
        try:
            os.remove(filepath)
            print("File \"" + filepath + "\" has successfully been deleted.")
            return 1
        except OSError:
            print("[x] ERROR: File \"" + filepath + "\" cannot be deleted.")
            return 0
    return 1
            
            
def scan_directories(filename, function, *args):
    unpatch = None
    if len(sys.argv) < 2 or sys.argv[1] not in ["-p", "-u"]:
        print("You must provide a valid command as first argument.")
        print("List of available commands:")
        print("-p : Apply the patch")
        print("-u : Remove the patch")
        sys.exit(0)
        
    if sys.argv[1] == "-p":
        unpatch = False
    elif sys.argv[1] == "-u":
        unpatch = True
        
    filename.replace('/', os.sep)
    for root, _, files in os.walk("."):
        if not any([pattern in root for pattern in ["build", "omm"]]):
            for file in files:
                if not any([pattern in file for pattern in [".rej", ".porig"]]):
                    filepath = root + os.sep + file
                    if (os.sep + filename) in filepath:
                        return function(filepath, *args, unpatch)
                
                
def do_patch_file(filename, location, where, what, how):
    return scan_directories(filename, patch_string_in_file, location, where, what, how)
    
    
def do_undef_code(filename, start, end, index=0):
    if (do_patch_file(filename, start, start, "#if 0 // {} [{}]\n".format(filename, index), -1) != 0):
        do_patch_file(filename, end, end, "#endif // {} [{}]\n".format(filename, index), -1)
    
    
def do_delete_file(filename):
    scan_directories(filename, delete_file)
    
    
if __name__ == "__main__":

    # File deletion
    do_delete_file("behavior_script.c")
    do_delete_file("behavior_script.h")
    do_delete_file("gbi.h")
    do_delete_file("geo_layout.c")
    do_delete_file("geo_layout.h")
    do_delete_file("gfx_pc.c")
    do_delete_file("graph_node.c")
    do_delete_file("graph_node.h")
    do_delete_file("graph_node_manager.c")
    do_delete_file("level_script.c")
    do_delete_file("level_script.h")
    do_delete_file("mario_step.c")
    do_delete_file("math_util.c")
    do_delete_file("math_util.h")
    do_delete_file("rendering_graph_node.c")
    do_delete_file("rendering_graph_node.h")
    do_delete_file("save_file.c")
    do_delete_file("screen_transition.c")
    do_delete_file("shadow.c")
    do_delete_file("shadow.h")
    do_delete_file("star_select.c")
    do_delete_file("surface_collision.c")
    do_delete_file("surface_collision.h")
    do_delete_file("surface_load.c")
    do_delete_file("surface_load.h")
    do_delete_file("gfx_sdl.c")
    do_delete_file("gfx_sdl1.c")
    do_delete_file("gfx_sdl2.c")
    
    # Code deletion
    do_undef_code("ingame_menu.c", "void render_dialog_box_type", "void change_and_flash_dialog_text_color_lines")
    do_undef_code("interaction.c", "u32 determine_interaction", "u32 attack_object")
    do_undef_code("mario.c", "struct Surface *resolve_and_return_wall_collisions", "s32 mario_facing_downhill", 0)
    do_undef_code("mario_actions_submerged.c", "static f32 get_buoyancy", "static BAD_RETURN(u32) update_water_pitch")

    # Header patches
    do_patch_file("sm64.h", "SM64_H", "#include \"macros.h\"", "\n#include \"data/omm/omm_includes.h\"", +1)
    do_patch_file("types.h", "_SM64_TYPES_H_", "s32 animAccel;", "\n    s16_ts _animID;\n    ptr_ts _curAnim;\n    s16_ts _animFrame;", +1)
    do_patch_file("types.h", "_SM64_TYPES_H_", "Vec3f cameraToObject;", "\n    Vec3s_ts _angle;\n    Vec3f_ts _pos;\n    Vec3f_ts _scale;\n    Vec3f_ts _shadowPos;\n    f32_ts _shadowScale;\n    Mat4_ts _throwMatrix;", +1)
    do_patch_file("ultra64.h", "_ULTRA64_H_", "#include <PR/libultra.h>", "\n#include \"data/omm/omm_macros.h\"", +1)
    
    # Actors patches
    do_patch_file("mario/model.inc.c", "", "", "#define static\n#define const\n", +1)
    do_patch_file("mario_cap/model.inc.c", "", "", "#define static\n#define const\n", +1)

    # Game patches
    do_patch_file("camera.c", "find_in_bounds_yaw_wdw_bob_thi(Vec3f pos, Vec3f origin, s16 yaw)", "{", "\n    return (yaw);", +1)
    do_patch_file("interaction.c", "void check_death_barrier(struct MarioState *m)", "{", "\n    OMM_RETURN_IF_TRUE(OMM_CHEAT_WALK_ON_DEATH_BARRIER,,);", +1)
    do_patch_file("interaction.c", "void check_lava_boost(struct MarioState *m)", "{", "\n    OMM_RETURN_IF_TRUE(OMM_CHEAT_WALK_ON_LAVA,,);", +1)
    do_patch_file("level_update.c", "level_trigger_warp(struct MarioState *m, s32 warpOp)", "{", "\n    OMM_RETURN_IF_TRUE(omm_mario_check_death_warp(m, warpOp), 0,);", +1)
    do_patch_file("mario.c", "s32 mario_get_floor_class(struct MarioState *m)", "{", "\n    OMM_RETURN_IF_TRUE(OMM_CHEAT_WALK_ON_SLOPE, SURFACE_CLASS_NOT_SLIPPERY,);", +1)
    do_patch_file("mario.c", "u32 mario_floor_is_slippery(struct MarioState *m)", "{", "\n    OMM_RETURN_IF_TRUE(OMM_CHEAT_WALK_ON_SLOPE, FALSE,);", +1)
    do_patch_file("mario_actions_airborne.c", "update_air_with_turn(struct MarioState *m)", "{", "\n    OMM_RETURN_IF_TRUE(OMM_LIKELY(omm_mario_update_air_with_turn(m)),,);", +1)
    do_patch_file("mario_actions_airborne.c", "update_air_without_turn(struct MarioState *m)", "{", "\n    OMM_RETURN_IF_TRUE(OMM_LIKELY(omm_mario_update_air_without_turn(m)),,);", +1)
    do_patch_file("mario_actions_airborne.c", "common_air_action_step(struct MarioState *m, u32 landAction, s32 animation, u32 stepArg)", "case AIR_STEP_HIT_WALL:", "\n            OMM_RETURN_IF_TRUE(omm_mario_try_to_perform_wall_slide(m), AIR_STEP_NONE, set_mario_animation(m, animation););", +1)
    do_patch_file("mario_actions_cutscene.c", "check_for_instant_quicksand(struct MarioState *m)", "&& m->action != ACT_QUICKSAND_DEATH) {", "\n        OMM_RETURN_IF_TRUE(OMM_MOVESET_ODYSSEY, (find_floor_height(m->pos[0], m->pos[1], m->pos[2]) >= m->pos[1]) && mario_update_quicksand(m, 0.f),);", +1)
    do_patch_file("mario_actions_moving.c", "apply_slope_accel(struct MarioState *m)", "} else ", "if (!omm_peach_vibe_is_gloom()) ", +1)
    do_patch_file("mario_actions_moving.c", "update_walking_speed(struct MarioState *m)", "{", "\n    OMM_RETURN_IF_TRUE(OMM_LIKELY(omm_mario_update_walking_speed(m)),,);", +1)
    do_patch_file("object_helpers.c", "geo_switch_area(s32 callContext, struct GraphNode *node", "if (gMarioObject == NULL) {", "gFindFloorForCutsceneStar = TRUE;\n        ", -1)
    do_patch_file("options_menu.c", "static struct SubMenu *currentMenu = &menuMain", ";", "\n#include \"data/omm/system/omm_options_menu.inl\"", +1)
    do_patch_file("paintings.c", "Gfx *render_painting", "// Draw the triangles individually", "\n    extern void gfx_interpolate_painting(Vtx *, s32);\n    gfx_interpolate_painting(verts, numVtx);", +1)
    do_patch_file("paintings.c", "Gfx *geo_painting_update(s32 callContext,", "gLastPaintingUpdateCounter = gPaintingUpdateCounter;", "geo_painting_update_fix_floor();\n        ", -1)
    
    # Other patches
    do_patch_file("audio/external.c", "play_sound(s32 soundBits, f32 *pos)", "{", "\n    OMM_RETURN_IF_TRUE(omm_sound_play_character_sound_n64(soundBits, pos),,);", +1)
    do_patch_file("audio/external.c", "stop_background_music(u16 seqId)", "foundIndex = sBackgroundMusicQueueSize;", "\n    stop_background_music_fix_boss_music();", +1)
    do_patch_file("audio/seqplayer.c", "sequence_channel_process_script(struct SequenceChannel *seqChannel)", "case 0x00: // chan_testlayerfinished", "\n                        if (loBits >= LAYERS_MAX) break;", +1)
    do_patch_file("fs_packtype_dir.c", "fs_walk_result_t pack_dir_walk", "return fs_sys_walk(path, packdir_walkfn, &walkdata, recur)", " ? FS_WALK_SUCCESS : FS_WALK_INTERRUPTED", +1)
    do_patch_file("gfx_dxgi.cpp", "static struct {", "} dxgi;", "\n#include \"data/omm/engine/gfx_dxgi.inl\"", +1)
    do_patch_file("text/define_text.inc.c", "", "", "#define const\n", +1)

    # SM64 patches
    if any([x in sys.argv for x in ["-u", "smex", "r96a", "xalo"]]):
        print("---- SM64 patches ----")

    # Refresh 14+ patches (ex-alo based repositories)
    if any([x in sys.argv for x in ["-u", "xalo", "sm74", "smsr"]]):
        print("---- Refresh 14+ patches ----")
        do_undef_code("bettercamera.c", "s32 ray_surface_intersect", "static s32 puppycam_check_volume_bounds")
        do_undef_code("mario.c", "#ifdef BETTER_WALL_COLLISION", "s32 mario_facing_downhill", 1)
        do_patch_file("gd_memory.c", "#ifndef USE_SYSTEM_MALLOC", "#ifndef USE_SYSTEM_MALLOC", "#undef USE_SYSTEM_MALLOC\n", -1)
    
    # Render96 patches
    if any([x in sys.argv for x in ["-u", "r96a"]]):
        print("---- Render96 patches ----")
        do_delete_file("r96_character_swap.c")
        do_patch_file("r96_audio.c", "const char *r96_get_intended_level_music()", "if (gCurrLevelNum == LEVEL_CASTLE_GROUNDS) {", "\n        OMM_RETURN_IF_TRUE(omm_ssd_is_bowser_4(), R96_LEVEL_BOWSER_3,);", +1)
    
    # DynOS patches
    if any([x in sys.argv for x in ["-u", "dynos"]]):
        print("---- DynOS patches ----")
        do_patch_file("dynos_audio.cpp", "static SDL_AudioDeviceID DynOS_Music_GetDevice()", "static SDL_AudioDeviceID DynOS_Music_GetDevice()", "#include \"data/omm/peachy/omm_peach_vibes_r96.inl\"\n", -1)
        do_patch_file("dynos_audio.cpp", "static SDL_AudioDeviceID DynOS_Jingle_GetDevice()", "static SDL_AudioDeviceID DynOS_Jingle_GetDevice()", "#include  \"data/omm/peachy/omm_peach_vibes_r96.inl\"\n", -1)
        do_patch_file("dynos_c.cpp", "dynos_sound_play(const char *name, float *pos)", "{", "\n    OMM_RETURN_IF_TRUE(omm_sound_play_character_sound_r96(name, pos),,);", +1)
        do_patch_file("dynos_misc.cpp", "&__Actors()", "define_actor(yoshi_egg_geo),", "\nOMM_DYNOS_ACTORS,", +1)
    
