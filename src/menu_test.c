#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "graphics.h"
#include "graphics_tb.h"

#include "termbox.h"

#define WAIT_BEFORE_NEXT_TEST()                                  \
    do {                                                         \
        graphics.draw_menu(context, &menu);                      \
        graphics.wait_event(context, &event);                    \
        if (event.type == EVENT_QUIT) {                          \
            return true;                                         \
        }                                                        \
    } while (event.type != EVENT_KEY || event.key != KEY_ENTER); \

bool test_menu(void                 *context,
               graphics_callbacks_t graphics,
               char                 *err_msg) {
    char *ai_items[] = {
        "Human",
        "Random"
    };

    menu_item_t player_goat_item = {
        .type        = MENU_ITEM_SELECT,
        .choices     = ai_items,
        .choice      =                                      0,
        .num_choices = sizeof(ai_items) / sizeof(ai_items[0]),
        .label       = "Goat"
    };

    menu_item_t player_tiger_item = {
        .type        = MENU_ITEM_SELECT,
        .choices     = ai_items,
        .choice      =                                      0,
        .num_choices = sizeof(ai_items) / sizeof(ai_items[0]),
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
        .cursor   =                                0,
        .items    = items,
        .num_item = sizeof(items) / sizeof(items[0]),
        .title    = "Bagh-Chal game"
    };

    event_t event;

    WAIT_BEFORE_NEXT_TEST();

    menu_switch_right(&menu);

    WAIT_BEFORE_NEXT_TEST();

    menu_move_down(&menu);

    WAIT_BEFORE_NEXT_TEST();

    menu_switch_right(&menu);

    WAIT_BEFORE_NEXT_TEST();

    menu_switch_right(&menu);

    WAIT_BEFORE_NEXT_TEST();

    menu_move_up(&menu);

    WAIT_BEFORE_NEXT_TEST();

    menu_move_down(&menu);
    menu_move_down(&menu);

    WAIT_BEFORE_NEXT_TEST();

    return true;
}
