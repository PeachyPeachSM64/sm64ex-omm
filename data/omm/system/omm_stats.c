#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define read_single_stat(x) \
if (strstr(name, #x)) { \
    if (!*value1) { \
        *invalid = true; \
        return false; \
    } \
    if (sscanf(value1, "%llu", &stats->x) != 1) { \
        *invalid = true; \
        return false; \
    } \
    return true; \
}

#define read_double_stat(x) \
if (strstr(name, #x)) { \
    if (!*value1 || !*value2) { \
        *invalid = true; \
        return false; \
    } \
    if (sscanf(value1, "%llu", &stats->x[0]) != 1) { \
        *invalid = true; \
        return false; \
    } \
    if (sscanf(value2, "%llu", &stats->x[1]) != 1) { \
        *invalid = true; \
        return false; \
    } \
    return true; \
}

bool omm_stats_read(OmmStats *stats, const char *prefix, const char *name, const char *value1, const char *value2, bool *invalid) {
    if (strstr(name, prefix) == name) {
        read_single_stat(starsCollected);
        read_single_stat(sparklyStarsCollected);
        read_single_stat(coinsCollected);
        read_single_stat(capsCollected);
        read_single_stat(mushrooms1upCollected);
        read_single_stat(secretsCollected);
        read_single_stat(exclamationBoxesBroken);
        read_single_stat(enemiesDefeated);
        read_single_stat(bowsersDefeated);
        read_single_stat(aPresses);
        read_single_stat(jumps);
        read_single_stat(attacks);
        read_single_stat(cappyThrows);
        read_single_stat(cappyBounces);
        read_single_stat(captures);
        read_single_stat(hitsTaken);
        read_single_stat(restarts);
        read_single_stat(deaths);
        read_double_stat(distanceTotal);
        read_double_stat(distanceOnGround);
        read_double_stat(distanceAirborne);
        read_double_stat(distanceUnderwater);
        read_double_stat(distanceWingCap);
        read_double_stat(distanceMetalCap);
        read_double_stat(distanceVanishCap);
        read_double_stat(timeTotal);
        read_double_stat(timeOnGround);
        read_double_stat(timeAirborne);
        read_double_stat(timeUnderwater);
        read_double_stat(timeWingCap);
        read_double_stat(timeMetalCap);
        read_double_stat(timeVanishCap);
    }
    return false;
}

#define write_single_stat(x) omm_save_file_write_buffer("%s" #x " = %llu\n", prefix, stats->x)
#define write_double_stat(x) omm_save_file_write_buffer("%s" #x " = %llu %llu\n", prefix, stats->x[0], stats->x[1])
void omm_stats_write(const OmmStats *stats, const char *prefix) {
    write_single_stat(starsCollected);
    write_single_stat(sparklyStarsCollected);
    write_single_stat(coinsCollected);
    write_single_stat(capsCollected);
    write_single_stat(mushrooms1upCollected);
    write_single_stat(secretsCollected);
    write_single_stat(exclamationBoxesBroken);
    write_single_stat(enemiesDefeated);
    write_single_stat(bowsersDefeated);
    write_single_stat(aPresses);
    write_single_stat(jumps);
    write_single_stat(attacks);
    write_single_stat(cappyThrows);
    write_single_stat(cappyBounces);
    write_single_stat(captures);
    write_single_stat(hitsTaken);
    write_single_stat(restarts);
    write_single_stat(deaths);
    write_double_stat(distanceTotal);
    write_double_stat(distanceOnGround);
    write_double_stat(distanceAirborne);
    write_double_stat(distanceUnderwater);
    write_double_stat(distanceWingCap);
    write_double_stat(distanceMetalCap);
    write_double_stat(distanceVanishCap);
    write_double_stat(timeTotal);
    write_double_stat(timeOnGround);
    write_double_stat(timeAirborne);
    write_double_stat(timeUnderwater);
    write_double_stat(timeWingCap);
    write_double_stat(timeMetalCap);
    write_double_stat(timeVanishCap);
}

void omm_stats_reset(OmmStats *stats) {
    mem_zero(stats, sizeof(OmmStats));
}
