#ifndef TIME_TRIALS_H
#define TIME_TRIALS_H

#include <stdbool.h>
#include "types.h"

bool time_trials_is_enabled();
void time_trials_render_hud_timer();
bool time_trials_render_pause_castle(u8 alpha);
void time_trials_render_star_select_time(s32 starIndex, u8 alpha);

#endif // TIME_TRIALS_H
