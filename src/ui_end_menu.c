#include <stdio.h>

#include "ui_end_menu.h"
#include "ui_menu.h"
#include "graphics.h"
#include "menu.h"
#include "tools.h"

bool ui_end_menu(void                 *graphics_context,
                 graphics_callbacks_t graphics,
                 char                 *winner) {
    char str[256] = "";

    sprintf(str, "%s won", winner);

    menu_item_t winner_item = {
        .type  = MENU_ITEM_TEXT,
        .label = str
    };

    menu_item_t replay_item = {
        .type  = MENU_ITEM_BUTTON,
        .label = "Replay"
    };

    menu_item_t quit_item = {
        .type  = MENU_ITEM_BUTTON,
        .label = "Quit"
    };


    menu_item_t *items[] = {
        &winner_item,
        &EMPTY_MENU_ITEM,
        &replay_item,
        &quit_item
    };

    menu_t menu = {
        .cursor    =                2,
        .items     = items,
        .num_items = ARRAY_LEN(items),
        .title     = "Bagh-Chal game"
    };

    bool quit = ui_menu(graphics_context, graphics, &menu);
    quit |= menu.items[menu.cursor] == &quit_item;

    return quit;
}
