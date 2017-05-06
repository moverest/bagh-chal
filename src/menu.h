#ifndef __MENU_H__
#define __MENU_H__

#include <stdbool.h>

#include "models.h"

typedef enum {
    MENU_ITEM_SELECT,
    MENU_ITEM_BUTTON,
    MENU_ITEM_EMPTY
} item_type_t;

typedef struct {
    item_type_t type;
    char        **choices;
    int         num_choices;
    int         choice;
    char        *label;
} menu_item_t;

typedef struct {
    int         cursor;
    menu_item_t **items;
    int         num_item;
    char        *title;
    bool        quit;
} menu_t;

void menu_move_up(menu_t *menu);
void menu_move_down(menu_t *menu);
void menu_switch_left(menu_t *menu);
void menu_switch_right(menu_t *menu);

bool menu_enter(menu_t *menu);
bool menu_esc(menu_t *menu);

#endif
