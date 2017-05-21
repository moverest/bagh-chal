#include <stdbool.h>
#include <string.h>

#include "graphics.h"
#include "ai.h"
#include "ui_game.h"
#include "ui_end_menu.h"
#include "ui_game_menu.h"

void ui_main(void                 *graphics_context,
             graphics_callbacks_t graphics) {
    ai_callbacks_t *tiger_ai = NULL;
    ai_callbacks_t *goat_ai  = NULL;

    bool stop       = false;
    char winner[32] = "";

    while (!stop) {
        stop = ui_game_menu(graphics_context, graphics, &tiger_ai, &goat_ai);

        if (stop) {
            return;
        }

        stop = ui_game_main(graphics_context, graphics, tiger_ai, goat_ai, winner);

        if (stop) {
            return;
        }

        stop = ui_end_menu(graphics_context, graphics, winner);
    }
}
