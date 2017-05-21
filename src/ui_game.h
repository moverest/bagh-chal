#ifndef __UI_GAME_H__
#define __UI_GAME_H__

#include "graphics.h"
#include "ai.h"

// ui_main is the main function that manage the ui.
// It takes two arguments that are the callback to be used to interact with the
// user.
// It return the winner of the game if there is one.
bool ui_game_main(void                 *graphics_context,
                  graphics_callbacks_t graphics,
                  ai_callbacks_t       *tiger_ai,
                  ai_callbacks_t       *goat_ai,
                  char                 *winner);

#endif
