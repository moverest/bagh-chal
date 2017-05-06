#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"


void menu_move_up(menu_t *menu) {
    if (menu->cursor <= 0) {
        return;
    }

    do {
        menu->cursor--;
    } while (menu->cursor > 0 &&
             menu->items[menu->cursor]->type == MENU_ITEM_EMPTY);
}


void menu_move_down(menu_t *menu) {
    if (menu->cursor >= menu->num_item - 1) {
        return;
    }

    do {
        menu->cursor++;
    } while (menu->cursor < menu->num_item - 1 &&
             menu->items[menu->cursor]->type == MENU_ITEM_EMPTY);
}


void menu_switch_left(menu_t *menu) {
    if (menu->items[menu->cursor]->type == MENU_ITEM_SELECT) {
        menu->items[menu->cursor]->choice =
            (menu->items[menu->cursor]->choice - 1 +
             (menu->items[menu->cursor]->num_choices)) % (menu->items[menu->cursor]->num_choices);
    }
}


void menu_switch_right(menu_t *menu) {
    if (menu->items[menu->cursor]->type == MENU_ITEM_SELECT) {
        menu->items[menu->cursor]->choice =
            (menu->items[menu->cursor]->choice + 1) %
            (menu->items[menu->cursor]->num_choices);
    }
}


bool menu_enter(menu_t *menu) {
    return menu->items[menu->cursor]->type == MENU_ITEM_BUTTON;
}
