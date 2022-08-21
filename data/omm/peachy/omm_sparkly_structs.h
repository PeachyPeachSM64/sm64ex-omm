#ifndef OMM_SPARKLY_STRUCTS_H
#define OMM_SPARKLY_STRUCTS_H

#include "types.h"

typedef struct OmmSparklyData {
    s32 index, dialog, game, level, area;
    s32 starX, starY, starZ;
    s32 signX, signY, signZ, signA, hint;
    s32 timer;
    u64 flags;
} OmmSparklyData;
extern const OmmSparklyData gOmmSparklyData[4][OMM_SPARKLY_STARS_MAX];

typedef struct OmmSparklyContext {
    const OmmSparklyData *data;
    struct Object *star;
    s32 state;
    bool inited;
    bool successful;
    bool starSpawned;
    bool objectsSpawned;
    bool cheatDetected;
    bool entryDialog;
    bool timerStarted;
    bool eventStarted;
    bool zones[5];
    s32 coinsYellow, coinsYellowPrev, coinsYellowDiff;
    s32 coinsLost, coinsLostPrev, coinsLostDiff;
    s32 coinsRed, coinsRedPrev, coinsRedDiff;
    s32 coinsBlue, coinsBluePrev, coinsBlueDiff;
    s32 mushrooms, mushroomsPrev, mushroomsDiff;
    s32 secrets, secretsPrev, secretsDiff;
} OmmSparklyContext;
extern OmmSparklyContext gOmmSparklyContext[1];

#define gOmmSparkly         (&gOmmMario->sparkly)
#define gOmmSparklyMode     gOmmSparkly->mode
#define gOmmSparklyEnding   gOmmSparkly->ending
#define gOmmSparklyCheats   gOmmSparkly->cheats

#endif