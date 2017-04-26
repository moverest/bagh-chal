#ifndef __UI_H__
#define __UI_H__

#include "models.h"
#include "graphics.h"
#include "game.h"
#include "ai.h"

// ui_main is the main function that manage the ui.
// It takes two arguments that are the callback to be used to interact with the
// user.
void ui_main(void                 *graphics_context,
             graphics_callbacks_t graphics,
             ai_callbacks_t       *tiger_ai,
             ai_callbacks_t       *goat_ai);

#endif
