#ifndef __UI_MAIN_H__
#define __UI_MAIN_H__


#include <stdbool.h>
#include <stdlib.h>

#include "ui_menu.h"
#include "ui_game_menu.h"
#include "menu.h"
#include "graphics.h"
#include "graphics_tb.h"
#include "ai_rand.h"
#include "ai.h"
#include "ui_game.h"

void ui_main(void                 *graphics_context,
             graphics_callbacks_t graphics);


#endif
