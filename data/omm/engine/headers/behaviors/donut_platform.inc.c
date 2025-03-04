#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif

static Vec3s sDonutPlatformPositions[] = {
    { 0x0B4C, 0xF7D7, 0x19A4 },
    { 0xF794, 0x08A3, 0xFFA9 },
    { 0x069C, 0x09D8, 0xFFE0 },
    { 0x05CF, 0x09D8, 0xFFE0 },
    { 0x0502, 0x09D8, 0xFFE0 },
    { 0x054C, 0xF7D7, 0x19A4 },
    { 0x0A7F, 0xF7D7, 0x19A4 },
    { 0x09B2, 0xF7D7, 0x19A4 },
    { 0x06E6, 0xF7D7, 0x19A4 },
    { 0x0619, 0xF7D7, 0x19A4 },
    { 0xEFB5, 0xF7D7, 0x19A4 },
    { 0x00E6, 0xF7D7, 0x19A4 },
    { 0x0019, 0xF7D7, 0x19A4 },
    { 0xFF4D, 0xF7D7, 0x19A4 },
    { 0xF081, 0xF7D7, 0x19A4 },
    { 0xE34F, 0xF671, 0x197A },
    { 0xEEE8, 0xF7D7, 0x19A4 },
    { 0xE74F, 0xF7D7, 0x197A },
    { 0xE683, 0xF7D7, 0x197A },
    { 0xE5B6, 0xF7D7, 0x197A },
    { 0xEE83, 0xF4A4, 0x19A4 },
    { 0xE41C, 0xF671, 0x197A },
    { 0xE4E9, 0xF671, 0x197A },
    { 0xECE9, 0xF4A4, 0x19A4 },
    { 0xEDB6, 0xF4A4, 0x19A4 },
    { 0xFC3F, 0x0A66, 0xFF45 },
    { 0x00EF, 0x04CD, 0xFF53 },
    { 0x0022, 0x04CD, 0xFF53 },
    { 0xFF57, 0x04CD, 0xFF53 },
    { 0xFB73, 0x0A66, 0xFF45 },
    { 0xFD0C, 0x0A66, 0xFF45 },
};

void bhv_donut_platform_spawner_update(void) {
    for (s32 i = 0; i != array_length(sDonutPlatformPositions); ++i) {
        struct Object *donut = spawn_object_relative(i, sDonutPlatformPositions[i][0], sDonutPlatformPositions[i][1], sDonutPlatformPositions[i][2], o, MODEL_RR_DONUT_PLATFORM, bhvDonutPlatform);
        if (donut) {
            obj_set_home(donut, donut->oPosX, donut->oPosY, donut->oPosZ);
        }
    }
    obj_mark_for_deletion(o);
}

void bhv_donut_platform_update(void) {
    switch (o->oAction) {

        // Ready
        case 0: {
            cur_obj_unhide();
            if (gMarioObject->platform == o) {
                cur_obj_shake_y(4.f);
                if (o->oTimer > 15) {
                    obj_set_vel(o, 0, 0, 0);
                    o->oAction = 1;
                }
            } else {
                cur_obj_set_pos_to_home();
                o->oTimer = 0;
            }
            load_object_collision_model();
        } break;

        // Falling
        case 1: {
            o->oGravity = -0.1f;
            cur_obj_unhide();
            cur_obj_update_floor_and_walls();
            cur_obj_move_standard(78);
            if ((o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND) || o->oTimer > 150) {
                spawn_mist_particles_variable(0, 0, 150);
                spawn_triangle_break_particles(OBJ_SPAWN_TRI_BREAK_PRESET_TRIANGLES_30);
                create_sound_spawner(SOUND_GENERAL_DONUT_PLATFORM_EXPLOSION);
                o->oAction = 2;
            }
            load_object_collision_model();
        } break;

        // Waiting to respawn
        case 2: {
            cur_obj_hide();
            cur_obj_set_pos_to_home();
            if (o->oTimer > 30) {
                o->oAction = 0;
            }
        } break;
    }
}
