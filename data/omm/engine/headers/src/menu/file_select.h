#ifndef FILE_SELECT_H
#define FILE_SELECT_H

#include "types.h"

void beh_yellow_background_menu_init(void);
void beh_yellow_background_menu_loop(void);
void bhv_menu_button_init(void);
void bhv_menu_button_loop(void);
void bhv_menu_button_manager_init(void);
void bhv_menu_button_manager_loop(void);
Gfx *geo_file_select_strings_and_menu_cursor(s32 callContext, struct GraphNode *node, Mat4 mtx);

#endif // FILE_SELECT_H
