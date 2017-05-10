#include "ui_game_menu.h"
#include "ai.h"
#include "graphics.h"
#include "graphics_tb.h"
#include "menu.h"
#include "ui_menu.h"
#include "ai_rand.h"
#include "tools.h"

bool ui_game_menu(void                 *graphics_context,
                  graphics_callbacks_t graphics,
                  ai_callbacks_t       **tiger_ai,
                  ai_callbacks_t       **goat_ai) {
    char           *ai_items[] = {
        "Human",
        "Random"
    };
    ai_callbacks_t *ai_item_callbacks[] = { NULL, &ai_rand_callbacks };

    menu_item_t player_goat_item = {
        .type        = MENU_ITEM_SELECT,
        .choices     = ai_items,
        .choice      =                   0,
        .num_choices = ARRAY_LEN(ai_items),
        .label       = "Goat"
    };

    menu_item_t player_tiger_item = {
        .type        = MENU_ITEM_SELECT,
        .choices     = ai_items,
        .choice      =                   0,
        .num_choices = ARRAY_LEN(ai_items),
        .label       = "Tiger"
    };

    menu_item_t play_button_item = {
        .type  = MENU_ITEM_BUTTON,
        .label = "Play"
    };

    menu_item_t empty_item = {
        .type = MENU_ITEM_EMPTY,
    };

    menu_item_t *items[] = {
        &player_goat_item,
        &player_tiger_item,
        &empty_item,
        &play_button_item
    };

    menu_t menu = {
        .cursor   =                0,
        .items    = items,
        .num_item = ARRAY_LEN(items),
        .title    = "Bagh-Chal game"
    };

    bool quit = ui_menu(graphics_context, graphics, &menu);

    *goat_ai  = ai_item_callbacks[player_goat_item.choice];
    *tiger_ai = ai_item_callbacks[player_tiger_item.choice];

    return quit;
}
