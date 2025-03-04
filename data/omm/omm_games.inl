//
// Games data
// Everything that needs to be defined no matter the game is here
//
// #define OMM_GAME(code, suffix, modes, stars, collectibles)
// #include "data/omm/omm_games.inl"
// #undef OMM_GAME
//

#ifndef OMM_GAME
#define OMM_GAME_SMEX__CODE         "smex"
#define OMM_GAME_SMEX__SUFFIX       SMEX
#define OMM_GAME_SMEX__MODES        1
#define OMM_GAME_SMEX__STARS        7
#define OMM_GAME_SMEX__COLLECTS     NULL
#else
OMM_GAME(OMM_GAME_SMEX__CODE, OMM_GAME_SMEX__SUFFIX, OMM_GAME_SMEX__MODES, OMM_GAME_SMEX__STARS, OMM_GAME_SMEX__COLLECTS)
#endif

#ifndef OMM_GAME
#define OMM_GAME_R96X__CODE         "r96x"
#define OMM_GAME_R96X__SUFFIX       R96X
#define OMM_GAME_R96X__MODES        1
#define OMM_GAME_R96X__STARS        7
#define OMM_GAME_R96X__COLLECTS     "b0010b1006"
#else
OMM_GAME(OMM_GAME_R96X__CODE, OMM_GAME_R96X__SUFFIX, OMM_GAME_R96X__MODES, OMM_GAME_R96X__STARS, OMM_GAME_R96X__COLLECTS)
#endif

#ifndef OMM_GAME
#define OMM_GAME_SMMS__CODE         "smms"
#define OMM_GAME_SMMS__SUFFIX       SMMS
#define OMM_GAME_SMMS__MODES        1
#define OMM_GAME_SMMS__STARS        7
#define OMM_GAME_SMMS__COLLECTS     NULL
#else
OMM_GAME(OMM_GAME_SMMS__CODE, OMM_GAME_SMMS__SUFFIX, OMM_GAME_SMMS__MODES, OMM_GAME_SMMS__STARS, OMM_GAME_SMMS__COLLECTS)
#endif

#ifndef OMM_GAME
#define OMM_GAME_SM74__CODE         "sm74"
#define OMM_GAME_SM74__SUFFIX       SM74
#define OMM_GAME_SM74__MODES        2
#define OMM_GAME_SM74__STARS        7
#define OMM_GAME_SM74__COLLECTS     NULL
#else
OMM_GAME(OMM_GAME_SM74__CODE, OMM_GAME_SM74__SUFFIX, OMM_GAME_SM74__MODES, OMM_GAME_SM74__STARS, OMM_GAME_SM74__COLLECTS)
#endif

#ifndef OMM_GAME
#define OMM_GAME_SMSR__CODE         "smsr"
#define OMM_GAME_SMSR__SUFFIX       SMSR
#define OMM_GAME_SMSR__MODES        1
#define OMM_GAME_SMSR__STARS        7
#define OMM_GAME_SMSR__COLLECTS     NULL
#else
OMM_GAME(OMM_GAME_SMSR__CODE, OMM_GAME_SMSR__SUFFIX, OMM_GAME_SMSR__MODES, OMM_GAME_SMSR__STARS, OMM_GAME_SMSR__COLLECTS)
#endif

#ifndef OMM_GAME
#define OMM_GAME_SMGS__CODE         "smgs"
#define OMM_GAME_SMGS__SUFFIX       SMGS
#define OMM_GAME_SMGS__MODES        1
#define OMM_GAME_SMGS__STARS        7
#define OMM_GAME_SMGS__COLLECTS     NULL
#else
OMM_GAME(OMM_GAME_SMGS__CODE, OMM_GAME_SMGS__SUFFIX, OMM_GAME_SMGS__MODES, OMM_GAME_SMGS__STARS, OMM_GAME_SMGS__COLLECTS)
#endif
