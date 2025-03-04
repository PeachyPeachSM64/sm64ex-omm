#ifndef EXTENDED_BOUNDS_H
#define EXTENDED_BOUNDS_H
                                                ///////////////////////////////
                                                // VANILLA //    OMM // DIFF //
#define LEVEL_BOUNDARY_MAX          (0x8000)    //    8192 //  32768 //   x4 //
#define CELL_SIZE                   (0x0400)    //    1024 //   1024 //    . //
#define NUM_CELLS                   (0x0040)    //      16 //     64 //   x4 //
#define NUM_CELLS_INDEX             (0x003F)    //      4b //     6b //   +2 //
#define CELL_AMOUNT                 (0x0040)    //      16 //     64 //   x4 //
#define CELL_AMOUNT_MINUS_ONE       (0x003F)    //      4b //     6b //   +2 //
#define CELL_HEIGHT_LIMIT           (+20000)    //  +20000 // +20000 //    . //
#define FLOOR_LOWER_LIMIT           (-11000)    //  -11000 // -11000 //    . //
#define FLOOR_LOWER_LIMIT_MISC      (-10000)    //  -10000 // -10000 //    . //
#define FLOOR_LOWER_LIMIT_SHADOW    (-10000)    //  -10000 // -10000 //    . //
                                                ///////////////////////////////
#endif // EXTENDED_BOUNDS_H
