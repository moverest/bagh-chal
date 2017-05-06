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
             graphics_callbacks_t graphics) {
    ai_callbacks_t *tiger_ai = NULL;
    ai_callbacks_t *goat_ai  = NULL;

    bool quit = ui_game_menu(graphics_context, graphics, &tiger_ai, &goat_ai);

    if (quit) {
        return;
    }

    ui_game_main(graphics_context, graphics, tiger_ai, goat_ai);
}
