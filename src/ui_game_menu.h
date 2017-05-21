#ifndef __UI_GAME_MENU_H__
#define __UI_GAME_MENU_H__

#include "ai.h"
#include "graphics.h"

bool ui_game_menu(void                 *graphics_context,
                  graphics_callbacks_t graphics,
                  ai_callbacks_t       **tiger_ai,
                  ai_callbacks_t       **goat_ai);


#endif
