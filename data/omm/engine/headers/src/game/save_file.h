#ifndef SAVE_FILE_H
#define SAVE_FILE_H

#include "types.h"
#include "level_table.h"
#include "course_table.h"

#define EEPROM_SIZE (0x200)
#define NUM_SAVE_FILES (4)
#define NUM_SAVE_MODES (2)

// -------- Render96 -------- //
#define NUM_KEYS (10)
#define NUM_WARIO_COINS (6)
// -------------------------- //

#define OMM_SAVE_FILE_SECTION_GAME_DATA     "[%s:%c%d]"
#define OMM_SAVE_FILE_SECTION_SPARKLY_STARS "[sparkly_stars]"
#define OMM_SAVE_FILE_SECTION_STATS         "[stats]"
#define OMM_SAVE_FILE_SECTION_SECRETS       "[secrets]"
#define OMM_SAVE_FILE_SECTION_MARIO_COLORS  "[mario_colors]"
#define OMM_SAVE_FILE_SECTION_PEACH_COLORS  "[peach_colors]"

struct WarpNode;
struct SaveBuffer { bool unused; };

extern s8 gLevelToCourseNumTable[];
extern s8 gCourseNumToLevelNumTable[];
extern u8 gLastCompletedCourseNum;
extern u8 gLastCompletedLevelNum;
extern u8 gLastCompletedStarNum;
extern u8 gCurrCourseStarFlags;
extern u8 gGotFileCoinHiScore;
extern u8 gSpecialTripleJump;

#define SAVE_FLAG_FILE_EXISTS               (1 <<  0)
#define SAVE_FLAG_HAVE_WING_CAP             (1 <<  1)
#define SAVE_FLAG_HAVE_METAL_CAP            (1 <<  2)
#define SAVE_FLAG_HAVE_VANISH_CAP           (1 <<  3)
#define SAVE_FLAG_HAVE_KEY_1                (1 <<  4)
#define SAVE_FLAG_HAVE_KEY_2                (1 <<  5)
#define SAVE_FLAG_UNLOCKED_BASEMENT_DOOR    (1 <<  6)
#define SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR    (1 <<  7)
#define SAVE_FLAG_DDD_MOVED_BACK            (1 <<  8)
#define SAVE_FLAG_MOAT_DRAINED              (1 <<  9)
#define SAVE_FLAG_UNLOCKED_PSS_DOOR         (1 << 10)
#define SAVE_FLAG_UNLOCKED_WF_DOOR          (1 << 11)
#define SAVE_FLAG_UNLOCKED_CCM_DOOR         (1 << 12)
#define SAVE_FLAG_UNLOCKED_JRB_DOOR         (1 << 13)
#define SAVE_FLAG_UNLOCKED_BITDW_DOOR       (1 << 14)
#define SAVE_FLAG_UNLOCKED_BITFS_DOOR       (1 << 15)
#define SAVE_FLAG_UNLOCKED_50_STAR_DOOR     (1 << 16)
#define SAVE_FLAG_ALL_FLAGS                 ((1 << 17) - 1)
#define SAVE_FLAG_CAP_ON_GROUND             (0)
#define SAVE_FLAG_CAP_ON_KLEPTO             (0)
#define SAVE_FLAG_CAP_ON_UKIKI              (0)
#define SAVE_FLAG_CAP_ON_MR_BLIZZARD        (0)

// Not flags, but needs to be defined to fix compiling issues
#define SAVE_FLAG_COLLECTED_TOAD_STAR_1     (1 << 24)
#define SAVE_FLAG_COLLECTED_TOAD_STAR_2     (1 << 25)
#define SAVE_FLAG_COLLECTED_TOAD_STAR_3     (1 << 26)
#define SAVE_FLAG_COLLECTED_MIPS_STAR_1     (1 << 27)
#define SAVE_FLAG_COLLECTED_MIPS_STAR_2     (1 << 28)
#define SAVE_FLAG_TO_STAR_FLAG(cmd)         (((cmd) >> 24) & 0x7F)
#define STAR_FLAG_TO_SAVE_FLAG(cmd)         (((cmd) & 0x7F) << 24)

bool omm_save_file_exists                (s32 fileIndex, s32 modeIndex);
u32  omm_save_file_get_flags             (s32 fileIndex, s32 modeIndex);
u32  omm_save_file_get_star_flags        (s32 fileIndex, s32 modeIndex, s32 courseIndex);
u32  omm_save_file_get_cannon_unlocked   (s32 fileIndex, s32 modeIndex, s32 courseIndex);
s32  omm_save_file_get_course_star_count (s32 fileIndex, s32 modeIndex, s32 courseIndex);
s32  omm_save_file_get_course_coin_score (s32 fileIndex, s32 modeIndex, s32 courseIndex);
s32  omm_save_file_get_total_star_count  (s32 fileIndex, s32 modeIndex);
s32  omm_save_file_get_total_coin_score  (s32 fileIndex, s32 modeIndex);
s32  omm_save_file_get_last_file_index   ();
s32  omm_save_file_get_last_mode_index   ();
s32  omm_save_file_get_last_course_num   (s32 fileIndex, s32 modeIndex);
u64  omm_save_file_get_capture_flags     (s32 fileIndex, s32 modeIndex);
s32  omm_save_file_get_capture_count     (s32 fileIndex, s32 modeIndex);
u32  omm_save_file_get_taken_luigi_key   (s32 fileIndex, s32 modeIndex, s32 keyIndex);
u32  omm_save_file_get_taken_wario_coin  (s32 fileIndex, s32 modeIndex, s32 coinIndex);
s32  omm_save_file_get_luigi_keys_count  (s32 fileIndex, s32 modeIndex);
s32  omm_save_file_get_wario_coins_count (s32 fileIndex, s32 modeIndex);

void omm_save_file_do_save               ();
void omm_save_file_load_all              ();
void omm_save_file_set_flags             (s32 fileIndex, s32 modeIndex, u32 flags);
void omm_save_file_set_star_flags        (s32 fileIndex, s32 modeIndex, s32 courseIndex, u32 starFlags);
void omm_save_file_set_cannon_unlocked   (s32 fileIndex, s32 modeIndex, s32 courseIndex);
void omm_save_file_set_last_course       (s32 fileIndex, s32 modeIndex, s32 courseIndex);
void omm_save_file_register_capture      (s32 fileIndex, s32 modeIndex, u64 captureFlag);
void omm_save_file_set_taken_luigi_key   (s32 fileIndex, s32 modeIndex, s32 keyIndex);
void omm_save_file_set_taken_wario_coin  (s32 fileIndex, s32 modeIndex, s32 coinIndex);
void omm_save_file_collect_star_or_key   (s32 fileIndex, s32 modeIndex, s32 levelIndex, s32 starIndex, s32 numCoins);
void omm_save_file_clear_flags           (s32 fileIndex, s32 modeIndex, u32 flags);
void omm_save_file_clear_star_flags      (s32 fileIndex, s32 modeIndex, s32 courseIndex, u32 starFlags);
void omm_save_file_erase                 (s32 fileIndex, s32 modeIndex);
void omm_save_file_copy                  (s32 fileIndex, s32 modeIndex, s32 destIndex);

bool warp_checkpoint_check               (struct WarpNode *warpNode, s32 actIndex);
void warp_checkpoint_check_if_should_set (struct WarpNode *warpNode, s32 courseIndex, s32 actIndex);
void warp_checkpoint_disable             ();

struct OmmStats;
struct OmmStats *omm_save_file_get_stats (s32 fileIndex, s32 modeIndex);

#define OMM_SAVE_FILE_WRITE_BUFFER_LENGTH (0x20000)
#define omm_save_file_write_buffer(...) { \
    extern char *gOmmSaveFileWriteBuffer; \
    extern s32 gOmmSaveFileWrittenLength; \
    if (gOmmSaveFileWrittenLength < OMM_SAVE_FILE_WRITE_BUFFER_LENGTH) { \
        s32 written = snprintf(gOmmSaveFileWriteBuffer, OMM_SAVE_FILE_WRITE_BUFFER_LENGTH - gOmmSaveFileWrittenLength, __VA_ARGS__); \
        gOmmSaveFileWriteBuffer += written; \
        gOmmSaveFileWrittenLength += written; \
    } \
}

#define save_file_exists(fileIndex)                                                 omm_save_file_exists(fileIndex, OMM_GAME_MODE)
#define save_file_get_flags()                                                       omm_save_file_get_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE)
#define save_file_get_star_flags(fileIndex, courseIndex)                            omm_save_file_get_star_flags(fileIndex, OMM_GAME_MODE, courseIndex)
#define save_file_is_cannon_unlocked()                                              omm_save_file_get_cannon_unlocked(gCurrSaveFileNum - 1, OMM_GAME_MODE, gCurrCourseNum - 1)
#define save_file_get_cannon_flags(fileIndex, courseIndex)                          omm_save_file_get_cannon_unlocked(fileIndex, OMM_GAME_MODE, courseIndex)
#define save_file_get_course_star_count(fileIndex, courseIndex)                     omm_save_file_get_course_star_count(fileIndex, OMM_GAME_MODE, courseIndex)
#define save_file_get_course_coin_score(fileIndex, courseIndex)                     omm_save_file_get_course_coin_score(fileIndex, OMM_GAME_MODE, courseIndex)
#define save_file_get_total_star_count(fileIndex, courseIndexMin, courseIndexMax)   omm_save_file_get_total_star_count(fileIndex, OMM_GAME_MODE)
#define save_file_taken_key(fileIndex, keyIndex)                                    omm_save_file_get_taken_luigi_key(fileIndex, OMM_GAME_MODE, keyIndex)
#define save_file_taken_wario_coin(fileIndex, coinIndex)                            omm_save_file_get_taken_wario_coin(fileIndex, OMM_GAME_MODE, coinIndex)
#define save_file_get_keys(fileIndex)                                               omm_save_file_get_luigi_keys_count(fileIndex, OMM_GAME_MODE)
#define save_file_get_wario_coins(fileIndex)                                        omm_save_file_get_wario_coins_count(fileIndex, OMM_GAME_MODE)
#define save_file_get_player_model(...)                                             0
#define save_file_get_max_coin_score(...)                                           0
#define save_file_get_cap_pos(...)                                                  0
#define save_file_get_sound_mode()                                                  SOUND_MODE_STEREO

#define save_file_do_save(...)                                                      omm_save_file_do_save()
#define save_file_load_all()                                                        omm_save_file_load_all()
#define save_file_reload()                                                          {}
#define save_file_set_flags(flags)                                                  omm_save_file_set_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, flags)
#define save_file_set_star_flags(fileIndex, courseIndex, starFlags)                 omm_save_file_set_star_flags(fileIndex, OMM_GAME_MODE, courseIndex, starFlags)
#define save_file_set_cannon_unlocked()                                             omm_save_file_set_cannon_unlocked(gCurrSaveFileNum - 1, OMM_GAME_MODE, gCurrCourseNum - 1)
#define save_file_clear_flags(flags)                                                omm_save_file_clear_flags(gCurrSaveFileNum - 1, OMM_GAME_MODE, flags)
#define save_file_collect_star_or_key(numCoins, starIndex)                          omm_save_file_collect_star_or_key(gCurrSaveFileNum - 1, OMM_GAME_MODE, gCurrLevelNum - 1, starIndex, numCoins)
#define save_file_register_key(fileIndex, keyIndex)                                 omm_save_file_set_taken_luigi_key(fileIndex, OMM_GAME_MODE, keyIndex)
#define save_file_register_wario_coin(fileIndex, coinIndex)                         omm_save_file_set_taken_wario_coin(fileIndex, OMM_GAME_MODE, coinIndex)
#define save_file_update_player_model(...)                                          {}
#define save_file_set_cap_pos(...)                                                  {}
#define save_file_set_sound_mode(...)                                               {}
#define save_file_move_cap_to_default_location()                                    {}
#define save_file_erase(fileIndex)                                                  omm_save_file_erase(fileIndex, OMM_GAME_MODE)
#define save_file_copy(fileIndex, destIndex)                                        omm_save_file_copy(fileIndex, OMM_GAME_MODE, destIndex)

#define check_warp_checkpoint(warpNode)                                             warp_checkpoint_check(warpNode, gCurrActNum - 1)
#define check_if_should_set_warp_checkpoint(warpNode)                               warp_checkpoint_check_if_should_set(warpNode, gCurrCourseNum - 1, gCurrActNum - 1)
#define disable_warp_checkpoint()                                                   warp_checkpoint_disable()

#endif // SAVE_FILE_H
