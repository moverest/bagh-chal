#include "ui_menu.h"

bool ui_menu(void *graphics_context, graphics_callbacks_t graphics, menu_t *menu) {
    event_t event;

    bool quit = false;
    bool stop = false;

    while (!stop && !quit) {
        graphics.draw_menu(graphics_context, menu);
        graphics.wait_event_menu(graphics_context, &event, menu);
        switch (event.type) {
        case EVENT_KEY:
            switch (event.key) {
            case KEY_ENTER:
                stop = menu_enter(menu);
                break;

            case KEY_ARROW_UP:
                menu_move_up(menu);
                break;

            case KEY_ARROW_DOWN:
                menu_move_down(menu);
                break;

            case KEY_ARROW_LEFT:
                menu_switch_left(menu);
                break;

            case KEY_ARROW_RIGHT:
                menu_switch_right(menu);
                break;

            default:
                break;
            }

            break;

        case EVENT_QUIT:
            quit = true;
            break;

        case EVENT_MENU_ITEM_CLICKED:
            stop = menu_click(menu, event.menu_item);
            break;

        default:
            break;
        }
    }

    return quit;
}
