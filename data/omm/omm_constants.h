#ifndef OMM_CONSTANTS_H
#define OMM_CONSTANTS_H

// Define commonly used values in rom-hacks needed by the OMM engine
// Must be included after sm64.h, which can include tweaks.inc.c

//
// Toad stars
//

#ifndef TOAD_STAR_1_REQUIREMENT
#define TOAD_STAR_1_REQUIREMENT 12
#endif
#ifndef TOAD_STAR_2_REQUIREMENT
#define TOAD_STAR_2_REQUIREMENT 25
#endif
#ifndef TOAD_STAR_3_REQUIREMENT
#define TOAD_STAR_3_REQUIREMENT 35
#endif

#ifndef TOAD_STAR_1_DIALOG
#define TOAD_STAR_1_DIALOG DIALOG_082
#endif
#ifndef TOAD_STAR_2_DIALOG
#define TOAD_STAR_2_DIALOG DIALOG_076
#endif
#ifndef TOAD_STAR_3_DIALOG
#define TOAD_STAR_3_DIALOG DIALOG_083
#endif

#ifndef TOAD_STAR_1_DIALOG_AFTER
#define TOAD_STAR_1_DIALOG_AFTER DIALOG_154
#endif
#ifndef TOAD_STAR_2_DIALOG_AFTER
#define TOAD_STAR_2_DIALOG_AFTER DIALOG_155
#endif
#ifndef TOAD_STAR_3_DIALOG_AFTER
#define TOAD_STAR_3_DIALOG_AFTER DIALOG_156
#endif

//
// Play modes
//

#ifndef PLAY_MODE_NORMAL
#define PLAY_MODE_NORMAL 0
#endif
#ifndef PLAY_MODE_PAUSED
#define PLAY_MODE_PAUSED 2
#endif
#ifndef PLAY_MODE_CHANGE_AREA
#define PLAY_MODE_CHANGE_AREA 3
#endif
#ifndef PLAY_MODE_CHANGE_LEVEL
#define PLAY_MODE_CHANGE_LEVEL 4
#endif
#ifndef PLAY_MODE_FRAME_ADVANCE
#define PLAY_MODE_FRAME_ADVANCE 5
#endif

//
// Warp types
//

#ifndef WARP_TYPE_NOT_WARPING
#define WARP_TYPE_NOT_WARPING 0
#endif
#ifndef WARP_TYPE_CHANGE_LEVEL
#define WARP_TYPE_CHANGE_LEVEL 1
#endif
#ifndef WARP_TYPE_CHANGE_AREA
#define WARP_TYPE_CHANGE_AREA 2
#endif
#ifndef WARP_TYPE_SAME_AREA
#define WARP_TYPE_SAME_AREA 3
#endif

//
// Warp nodes
//

#ifndef WARP_NODE_STAR_EXIT
#define WARP_NODE_STAR_EXIT 0xF0
#endif
#ifndef WARP_NODE_DEATH
#define WARP_NODE_DEATH 0xF1
#endif
#ifndef WARP_NODE_TOTWC_ENTRANCE
#define WARP_NODE_TOTWC_ENTRANCE 0xF2
#endif
#ifndef WARP_NODE_WARP_FLOOR
#define WARP_NODE_WARP_FLOOR 0xF3
#endif

#endif
