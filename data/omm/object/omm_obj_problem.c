#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//                                    *&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&%#(*.
//                                /@@@@@@@@@@@@@@&&&&&&&&&&%%%%%%%%%%%%%%&&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&%#(,
//                             *&@@@@@&/                         .*/##%##(///*///((###%%##(/*,..              ..,*/#&@@@@@@@@@@@@@@@@%/.
//                           %@@@@@&.                     ,/%&(,                                       ...,,,,,,,...      ..,*/#%@@@@@@@@@@(.
//                         %@@@@@,                   *@@(          .*(#&@@&&%##(((#%&@@@%(,                                 ....     .#@@@@@@@@#
//                       /@@@@@,                .#&/.     .(&%/,.                                ....,,,,,,,,,,,.....             *#      ,&@@@@@@%.
//                      %@@@@/              .(#,     *%%/     .*(######((((//(((((((*                                                         *&@@@@@(
//                    *@@@@@.            /%,     *%#.   *%&#,                     (&.                    ./%&&%#(*,..      ./%%*                 *@@@@@,
//                   (@@@@(           ((      (@,   *@#                             %/                                          .%&.               &@@@@
//                  %@@@@*                 .&/   *&/                                 &                     .&.                     ,&              .@@@@,
//                /@@@@@,                 .    ##             .,**/////**,.          .                     &,                       ,(              &@@@#
//              *@@@@@%                              ./&@@@@@@@@@@@@@@@@@@@@@@@(.                          #                                        *@@@@&
//            #@@@@@@.                            ,@@@@@@@@@@@@@@@@@@@&    .(@@@@@@%.                      .     ,#&@@@@@@@@@@@@%,                   .@@@@@@,
//         *@@@@@@@@%#(((/*,              .,*   /@@@&, .%@@@@@@@@@@@@@(         .%@@@@&.                    .%@@@@@@@@@@&#((#&@@@@@@*                  .&@@@@@&,
//      .%@@@@@#*/#%%%#((/**,..       ./%#*,   .@@@@@@@@@@@@@@@@@@@@@@@@@@@@%*     ,@@@@*            .,,/#@@@@@@@@@@@@@@@@@@@@@@@@@&,  *///(((/////(#(.   *@@@@@@,
//    ,@@@@@&./&,        .,//(((/*,..      *%,     ..          .//      ,#&@@@@@@@@@@@@#            #@@@@@@@@@@@@@@&%(/,.                             ,&#.  .&@@@@#
//  .@@@@@% *@,      *@@@@@@@@@@@@@@@@@@&*                  .%@@@@,           ,&@@@@#                   #@@@@/                                     (@@/   (@, *@@@@
// %@@@@&  (#      %@@@@/           .(@@@@@@@@&#/.    .*#&@@@@@#.                                        &@@#                         .(&@@@@@@@@&(.   &/   (/ &@@@
// @@@@/   @.    ,@@@@,        #@@.       ,/#&@@@@@@@@@@@@@#*                                            %@@&              #%#*    /&@@@@&#///(%@@@@@,  %    %*(@@@
// @@@(   .@    *@@@(         /@@@@%,                                                                    ,@@@@*            (@@@@@@@@@@@(            .   (,   *//@@@
// @@@,   .@    %@@#         %@@@@@@@@@@*                                 *     .*/*,                      (@@@@@&,             .,***.    #@#           //   &*#@@@
// @@@*    @.   &@@/   *#&@@@@@@&  .*%@@@@@@&(.                         ,(% *@@@@@@@@&.                        (@@@@@(                   .@@@,         #%   *&.@@@@
// @@@%    %*   (@@@, ,(#%%%&@@@@,       *#@@@@@@@%/.        ,,..           %@@#                                #@@@@@@@*                #@@@@&     .&(    .&.%@@@%
// @@@@%   ,#    &@@&         %@@@/           /@@@@@@@@@@#*.                %@@&.   &@@@@@@@@@@/              .@@@@@@@@(*(%#/,          /@@@@@@@        ,%&*.@@@@%.
// %@@@@&   (%    (@@(        /@@@@@#           (@@@##@@@@@@@@@@@%*         ,@@@@%  /@(                     ,@@@@%            .%%     ,@@@@&@@@@/  ,&@/    ,@@@@#
//  .@@@@@(   (@(              *@@@@@@@@%*      *@@@,      .,(&@@@@@@@@@&%/.   .,                   (@@@%&@@@@@,                   (@@@@@@@@(@@@#         (@@@@/
//    *@@@@@%.    *((,           /@@@@@@@@@@@%/./@@@@(              ,/%@@@@@@@@@@&#/*.               .(&@@@&/                ,(&@@@@@@@@.%@@@@@@&        (@@@@*
//      *@@@@@@@%*                 %@@@# .(&@@@@@@@@@@@%.               #@@@%#%&@@@@@@@@@@@@@@&%#(/*,,.....    ....,*/#&@@@@@@@@@&/.&@@@  #@@@@@@       ,@@@@,
//        ,@@@@@@*                  .@@@@*     #@@@@@@@@@@@@@#,        .@@@(            ,(&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@(     *@@@, .@@@@@@.      /@@@%
//           #@@@@@/                  *@@@@,    .@@@@@@@@@@@@@@@@@@%/..@@@&                 (@@@,         ,@@@@,          .@@@&     (@@@/ *@@@@@@.      /@@@(
//             #@@@@&.                  /@@@&,   &@@#    *#@@@@@@@@@@@@@@@@%/.              *@@@,          (@@&            ,@@@,   *@@@@@@@@@@@@@.      /@@@(
//              .@@@@@/                   /@@@@#%@@@*          *#&@@@@@@@@@@@@@@@@@@@&%((//(@@@@@%((////((#@@@@@#((##%&@@@@@@@@@@@@@@@@@@@@@@@@@@       /@@@(
//                (@@@@%                    ,@@@@@&                 .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&       /@@@(
//                 *@@@@&.                     /@@@@&,              .@@@#     .*%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#       /@@@(
//                  .&@@@@%                       /@@@@@(.          #@@@*             .*%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@((@@@*       /@@@(
//                    *@@@@@%.                       *&@@@@@*      &@@@.                 %@@&      .,*/(#@@@@@@@@@@@@@@@@@@@@&#&@@@@* &@@@*#@@@/        /@@@(
//                      ,@@@@@@/                         #@@@@@@((@@@@.                  %@@#            .@@@%        @@@&    /@@@#  #@@@,#@@@*         /@@@#
//                        .#@@@@@&,   *#,      ,(*          .(@@@@@@@@(.                 &@@(            %@@&       ,@@@%    #@@@(    .(@@@@%           /@@@#
//                           ,&@@@@@%,   .(&#,     .(%(.         .(%@@@@@@@@&%(*.       .@@@(           ,@@@/      /@@@@.   ,@@@&#%@@@@@@@/             *@@@%
//                              /@@@@@@@#.    ,#&(.     .#&#,           .*#&@@@@@@@@@@@@@@@@@&%#(/////(%@@@@&//(#&@@@@@@@@@@@@@@@@@@&(,                 *@@@&
//                                 .&@@@@@@@&.     .#@(       *&@(                   .,*(#%&&@@@@@@@@@@@@@@@@@@@@@@@&&%#/*.                             ,@@@&
//                                     ,%@@@@@@@&(.     ./%%*      .*%&(.                                                               /&,             .@@@@
//                                         .(@@@@@@@@&(.      ,/##/.      *(%#/.                  .....,,,,,,....                    /&(.        /.      @@@@,
//                                              ,%@@@@@@@@@&/.       ./%%(*..    ./#%%#/*,,....                                .*%&(.           (&       #@@@/
//                                                   .#@@@@@@@@@@#.            ./%@@%/,                   ...,,****//***,..                 ,&@,         *@@@#
//                                                         ,#@@@@@@@@@%,                  .,*/#%%%#/*..                                 /&&*             #@@@#
//                                                              ,(&@@@@@@@@%*                                   ...,,,,,******/////**,.                 #@@@&,
//                                                                    (&@@@@@@@@&(,                                                                   *@@@@&,
//                                                                         *&@@@@@@@@@@@&#/.                                                        /@@@@@(
//                                                                              ./&@@@@@@@@@@@@@@@@%#*.                                         .#@@@@@&(
//                                                                                       ,*(#&@@@@@@@@@@@@@@&%(/,.                        .*#@@@@@@@&(,
//                                                                                                  ./#&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%/.

//
// Gfx data
//

static const Vtx omm_problem_vertices[] = {
    { { { -128, -128, 0 }, 0, { 0, 992 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 128, -128, 0 }, 0, { 992, 992 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 128, 128, 0 }, 0, { 992, 0 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { -128, 128, 0 }, 0, { 0, 0 }, { 0xff, 0xff, 0xff, 0xff } } },
};

static const Gfx omm_problem_triangles[] = {
    gsSPVertex(omm_problem_vertices, 4, 0),
    gsSP2Triangles(0, 1, 2, 0x0, 0, 2, 3, 0x0),
    gsSPEndDisplayList(),
};

static const Gfx omm_problem_gfx[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, ENVIRONMENT, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(OMM_TEXTURE_EFFECT_PROBLEM, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, 0, 0, 0, 0, 0, 0),
    gsSPDisplayList(omm_problem_triangles),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_problem[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_BILLBOARD(),
        GEO_OPEN_NODE(),
            GEO_ASM(0, geo_update_layer_transparency),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_TRANSPARENT, omm_problem_gfx),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static void bhv_omm_problem_update() {
    struct Object *o = gCurrentObject;
    f32 t = clamp_0_1_f(o->oTimer / 20.f);
    o->oOpacity = 255.f * sqr_f(1.f - t);
    obj_scale(o, 2.5f * sqrt(t));
    if (t == 1.f) {
        obj_mark_for_deletion(o);
    }
}

const BehaviorScript bhvOmmProblem[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_problem_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

static const u32 sOmmProblemMarioActions[] = {
    ACT_HARD_FORWARD_GROUND_KB, ACT_HARD_BACKWARD_GROUND_KB,
    ACT_HARD_FORWARD_AIR_KB, ACT_HARD_BACKWARD_AIR_KB,
    ACT_FORWARD_WATER_KB, ACT_BACKWARD_WATER_KB,
    ACT_FORWARD_WATER_KB, ACT_BACKWARD_WATER_KB,
};

struct Object *omm_obj_spawn_problem(struct Object *o) {
    struct Object *problem = obj_spawn_from_geo(o, omm_geo_problem, bhvOmmProblem);
    problem->oGraphYOffset = 100.f;
    struct Object *explosion = obj_spawn_from_geo(problem, explosion_geo, bhvExplosion);
    explosion->oGraphYOffset = 100.f;
    play_sound(SOUND_GENERAL2_BOBOMB_EXPLOSION | 0xFF00, gGlobalSoundArgs);
    set_environmental_camera_shake(SHAKE_ENV_EXPLOSION);

    // Get trolled
    struct MarioState *m = gMarioState;
    m->interactObj = problem;
    obj_play_sound(o, SOUND_MARIO_ATTACKED);
    update_mario_sound_and_camera(m);
    omm_mario_unpossess_object(m, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
    drop_and_set_mario_action(m, sOmmProblemMarioActions[(((m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)) != 0) << 2) | (((m->action & ACT_FLAG_AIR) != 0) << 1) | (random_u16() & 1)], 1);
    gOmmMario->state.health.state = OMM_HEALTH_STATE_DAMAGE;
    gOmmMario->state.health.timer = 0;
    omm_stats_increase(hitsTaken, m->health > OMM_HEALTH_DEAD);
    gOmmGlobals->marioTimer = gGlobalTimer;
    return problem;
}
