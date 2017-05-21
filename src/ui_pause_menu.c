#include "graphics.h"
#include "menu.h"
#include "ui_menu.h"
#include "tools.h"
#include "ui_pause_menu.h"

bool ui_pause_menu(void                 *graphics_context,
                   graphics_callbacks_t graphics) {
    menu_item_t quit_text_item = {
        .type  = MENU_ITEM_TEXT,
        .label = "Are you sure you want to quit ?"
    };

    menu_item_t continue_item = {
        .type  = MENU_ITEM_BUTTON,
        .label = "Continue"
    };

    menu_item_t quit_item = {
        .type  = MENU_ITEM_BUTTON,
        .label = "Quit"
    };

    menu_item_t *items[] = {
        &quit_text_item,
        &EMPTY_MENU_ITEM,
        &continue_item,
        &quit_item
    };

    menu_t menu = {
        .cursor   =                2,
        .items    = items,
        .num_item = ARRAY_LEN(items),
        .title    = "Bagh-Chal game"
    };

    bool quit = ui_menu(graphics_context, graphics, &menu);

    quit |= menu.items[menu.cursor] == &quit_item;

    return quit;
}
