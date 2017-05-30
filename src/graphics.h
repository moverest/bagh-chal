#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdlib.h>

#include "models.h"
#include "game.h"
#include "menu.h"

typedef struct {
    game_t               *game;
    mvt_t                input;
    possible_positions_t possible_positions;
    char                 *msg;
} game_state_to_draw_t;

typedef    enum {
    EVENT_KEY,
    EVENT_QUIT,
    EVENT_POSITION,
    EVENT_REDRAW,
    EVENT_MENU_ITEM_CLICKED
} event_type_t;

typedef enum {
    KEY_CH,
    KEY_ESC,
    KEY_ENTER,
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,
    KEY_BACKSPACE,
} keypress_t;

typedef struct {
    event_type_t type;
    keypress_t   key;
    char         ch;
    position_t   position;
    int          menu_item;
} event_t;

// graphics_draw_callback_t defines the function to be used as a callback to
// draw the current state of the game on the screen.
typedef void (*graphics_draw_game_callback_t)(void *context, game_state_to_draw_t *state);

// graphics_wait_event_callback_t defines the function to be used as a callback
// to wait for events.
typedef void (*graphics_wait_event_game_callback_t)(void *context, event_t *event);

typedef void (*graphics_wait_event_menu_callback_t)(void *context, event_t *event, menu_t *menu);

typedef void (*graphics_draw_menu_callback_t)(void *context, menu_t *menu);

typedef struct {
    graphics_draw_game_callback_t       draw_game;
    graphics_wait_event_game_callback_t wait_event_game;
    graphics_wait_event_menu_callback_t wait_event_menu;
    graphics_draw_menu_callback_t       draw_menu;
} graphics_callbacks_t;

#endif
