#include <termbox.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graphics_tb.h"
#include "menu.h"


/*
 *
 *      a      b      c      d      e
 *    0 + ---- + ---- + ---- + ---- +       Quit            < PADDING_ROW
 *      | \    |    / | \    |    / |                         QUIT_ROW
 *      |    \ | /    |    \ | /    |       Tiger turn      < TURN_ROW
 *    1 + ---- + ---- + ---- + ---- +
 *      |    / | \    |    / | \    |       19 Goats left   < GOAT_LEFT_ROW
 *      | /    |    \ | /    |    \ |       0 Goat eaten    < GOAT_EATEN_ROW
 *    2 + ---- + ---- + ---- + ---- +
 *      | \    |    / | \    |    / |       Message         < MSG_ROW
 *      |    \ | /    |    \ | /    |
 *    3 + ---- + ---- + ---- + ---- +                       <-+ SPACING_ROW
 *      |    / | \    |    / | \    |                         |
 *      | /    |    \ | /    |    \ |                         |
 *    4 + ---- + ---- + ---- + ---- +                       <-+
 *
 * Move a1 to b1                                            < MOVE_ROW
 *
 *      ^                    ^      ^       ^
 *      PADDING_COL          + ---- +       TURN_COL
 * ^                         SPACING_COL    GOAT_EATEN_COL
 * MOVE_COL                                 GOAT_LEFT_COL
 *                                          MSG_COL
 *                                          QUIT_COL
 */

#define PADDING_COL                            2
#define PADDING_ROW                            1
#define GOAT_EATEN_ROW                         40
#define GOAT_EATEN_COL                         7
#define GOAT_LEFT_ROW                          40
#define GOAT_LEFT_COL                          6
#define TURN_ROW                               40
#define TURN_COL                               4
#define MOVE_ROW                               17
#define MOVE_COL                               1
#define MSG_ROW                                9
#define MSG_COL                                40
#define QUIT_ROW                               2
#define QUIT_COL                               40
#define QUIT_LEN                               4


#define POSSIBLE_POSSITION_TIGER_TURN_COLOR    TB_MAGENTA
#define POSSIBLE_POSSITION_GOAT_TURN_COLOR     TB_GREEN
#define GOAT_COLOR                             TB_CYAN
#define TIGER_COLOR                            TB_RED
#define EMPTY_COLOR                            TB_DEFAULT
#define INPUT_POS_COLOR                        TB_YELLOW
#define SELECTION_COLOR                        TB_YELLOW


#define SPACING_COL                            7
#define SPACING_ROW                            3


// Menu
#define TITLE_COL        10
#define TITLE_ROW        5
#define MENU_ITEM_COL    10
#define MENU_ITEM_ROW    7


graphics_tb_t *graphics_tb_init() {
    graphics_tb_t *tg = malloc(sizeof(graphics_tb_t));

    if (tg == NULL) {
        return NULL;
    }

    if (tb_init()) {
        return NULL;
    }

    tb_select_input_mode(TB_INPUT_ESC | TB_INPUT_MOUSE);

    return tg;
}


// draw_cells places tigers and goats which are already on the board before the
// beginning of the tour.
static void draw_cells(board_t *board) {
    struct tb_cell cell = { '0', TB_DEFAULT, TB_DEFAULT };

    position_t pos;

    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            switch (board_get_cell(board, pos)) {
            case GOAT_CELL:
                cell.ch = 'G';
                cell.fg = GOAT_COLOR;
                break;

            case TIGER_CELL:
                cell.ch = 'T';
                cell.fg = TIGER_COLOR;
                break;

            case EMPTY_CELL:
                cell.ch = '+';
                cell.fg = EMPTY_COLOR;
                break;
            }
            tb_put_cell(SPACING_COL * pos.c + PADDING_COL, SPACING_ROW * pos.r + PADDING_ROW, &cell);
        }
    }
}


// print_str prints a string at the given position.
static int print_str(char *str, int col, int row) {
    int i = 0;

    while (str[i] != '\0') {
        tb_change_cell(col + i, row, str[i], TB_DEFAULT, TB_DEFAULT);
        i++;
    }

    return i;
}


#define BOARD_TXT_PADDING_COL    PADDING_COL

// draw_board draws a blanc board.
static void draw_board() {
    int i = PADDING_ROW;

    print_str("+ ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
    print_str("| \\    |    / | \\    |    / |", BOARD_TXT_PADDING_COL, i++);
    print_str("|    \\ | /    |    \\ | /    |", BOARD_TXT_PADDING_COL, i++);
    print_str("+ ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
    print_str("|    / | \\    |    / | \\    |", BOARD_TXT_PADDING_COL, i++);
    print_str("| /    |    \\ | /    |    \\ |", BOARD_TXT_PADDING_COL, i++);
    print_str("+ ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
    print_str("| \\    |    / | \\    |    / |", BOARD_TXT_PADDING_COL, i++);
    print_str("|    \\ | /    |    \\ | /    |", BOARD_TXT_PADDING_COL, i++);
    print_str("+ ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
    print_str("|    / | \\    |    / | \\    |", BOARD_TXT_PADDING_COL, i++);
    print_str("| /    |    \\ | /    |    \\ |", BOARD_TXT_PADDING_COL, i++);
    print_str("+ ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
}


// draw_goat_left writes the number of goats left to be put on the board.
static void draw_goat_left(int goat_left) {
    char gl[32];

    sprintf(gl, goat_left <= 1 ? "%d Goat left" : "%d Goats left", goat_left);
    print_str(gl, GOAT_LEFT_ROW, GOAT_LEFT_COL);
}


// draw_goat_eaten writes the number of goat eaten.
static void draw_goat_eaten(int goat_eaten) {
    char ge[32];

    sprintf(ge, goat_eaten <= 1 ? "%d Goat eaten" : "%d Goats eaten", goat_eaten);
    print_str(ge, GOAT_EATEN_ROW, GOAT_EATEN_COL);
}


// draw_turn draws the player turn on the screen.
static void draw_turn(player_turn_t turn) {
    print_str(turn == GOAT_TURN ? "Goat turn" : "Tiger turn", TURN_ROW, TURN_COL);
}


// draw_quit draws the "Quit" button on the screen.
static void draw_quit() {
    tb_change_cell(QUIT_COL, QUIT_ROW, 'Q', TB_DEFAULT | TB_UNDERLINE, TB_DEFAULT);
    print_str("uit", QUIT_COL + 1, QUIT_ROW);
}


// draw_gui writes what is write in it.
static void draw_gui(game_state_to_draw_t *state) {
    draw_quit();
    draw_turn(state->game->turn);
    draw_goat_left(state->game->num_goats_to_put);
    draw_goat_eaten(state->game->num_eaten_goats);
}


// draw_possible_positions writes the possible possible_positions on the board
// with their label (their coordinates).
static void draw_possible_positions(possible_positions_t *possible_positions,
                                    mvt_t                input,
                                    player_turn_t        turn) {
    int selected_col = POSITION_NOT_SET;

    if ((input.from.c != POSITION_NOT_SET) && (input.from.r == POSITION_NOT_SET)) {
        selected_col = input.from.c;
    } else if ((input.to.c != POSITION_NOT_SET) && (input.to.r == POSITION_NOT_SET)) {
        selected_col = input.to.c;
    }

    uint16_t possible_position_color = turn == TIGER_TURN ?
                                       POSSIBLE_POSSITION_TIGER_TURN_COLOR : POSSIBLE_POSSITION_GOAT_TURN_COLOR;

    position_t pos;
    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            if ((input.from.c == pos.c) && (input.from.r == pos.r)) {
                tb_change_cell(SPACING_COL * pos.c + PADDING_COL,
                               SPACING_ROW * pos.r + PADDING_ROW,
                               'O',
                               SELECTION_COLOR,
                               TB_DEFAULT);
            } else if (((selected_col == POSITION_NOT_SET) || (selected_col == pos.c)) &&
                       is_position_possible(possible_positions, pos)) {
                tb_change_cell(SPACING_COL * pos.c + PADDING_COL,
                               SPACING_ROW * pos.r + PADDING_ROW,
                               position_get_tag(pos),
                               possible_position_color,
                               TB_DEFAULT);
            }
        }
    }
}


// draw_input draws the input.
static void draw_input(game_state_to_draw_t *state) {
    int  x             = MOVE_COL;
    bool placing_goats = (state->game->turn == GOAT_TURN) &&
                         (state->game->num_goats_to_put != 0);

    x += print_str(placing_goats ? "Put to " : "Move ", x, MOVE_ROW);

    if (position_is_set(state->input.from)) {
        tb_change_cell(x, MOVE_ROW, position_get_tag(state->input.from),
                       INPUT_POS_COLOR, TB_DEFAULT);
        x++;

        if (!placing_goats) {
            x += print_str(" to ", x, MOVE_ROW);
        }
    }

    if (position_is_set(state->input.to)) {
        tb_change_cell(x, MOVE_ROW, position_get_tag(state->input.to),
                       INPUT_POS_COLOR, TB_DEFAULT);
    }

    tb_set_cursor(x, MOVE_ROW);
}


void graphics_tb_draw_menu(void *context, menu_t *menu) {
    tb_clear();
    print_str(menu->title, TITLE_COL, TITLE_ROW);

    for (int i = 0; i < menu->num_item; i++) {
        int x = 0;
        switch (menu->items[i]->type) {
        case MENU_ITEM_SELECT:
            x = print_str(menu->items[i]->label, MENU_ITEM_COL, MENU_ITEM_ROW + i);
            x = x + print_str(" : ", MENU_ITEM_COL + x, MENU_ITEM_ROW + i);
            print_str(menu->items[i]->choices[menu->items[i]->choice], MENU_ITEM_COL + x, MENU_ITEM_ROW + i);
            break;

        case MENU_ITEM_BUTTON:
        case MENU_ITEM_TEXT:
            print_str(menu->items[i]->label, MENU_ITEM_COL, MENU_ITEM_ROW + i);
            break;

        case MENU_ITEM_EMPTY:
            break;
        }
    }

    tb_set_cursor(MENU_ITEM_COL, MENU_ITEM_ROW + menu->cursor);
    tb_present();
}


// graphics_tb_draw_game draws the whole screen.
void graphics_tb_draw_game(void *context, game_state_to_draw_t *state) {
    tb_clear();
    draw_board();
    draw_gui(state);
    draw_cells(&state->game->board);
    draw_possible_positions(&state->possible_positions, state->input,
                            state->game->turn);
    draw_input(state);
    print_str(state->msg, MSG_COL, MSG_ROW);
    tb_present();
}


static int get_label_clickable_length(menu_t *menu, int selected_menu_item) {
    switch (menu->items[selected_menu_item]->type) {
    case MENU_ITEM_SELECT:
        return strlen(menu->items[selected_menu_item]->label)
               + strlen(menu->items[selected_menu_item]->choices[menu->items[selected_menu_item]->choice])
               + 3;

    case MENU_ITEM_BUTTON:
        return strlen(menu->items[selected_menu_item]->label);

    default:
        return 0;
    }
}


void graphics_tb_wait_game_event(void *context, event_t *event) {
    graphics_tb_wait_menu_event(context, event, NULL);
}


void graphics_tb_wait_menu_event(void *context, event_t *event, menu_t *menu) {
    struct tb_event tevent;
    int             stop = 0;

    do {
        tb_poll_event(&tevent);
        switch (tevent.type) {
        case TB_EVENT_KEY:
            event->type = EVENT_KEY;
            stop        = 1;
            switch (tevent.key) {
            case TB_KEY_ENTER:
                event->key = KEY_ENTER;
                break;

            case TB_KEY_ESC:
                event->type = EVENT_QUIT;
                event->key  = KEY_ESC;
                break;

            case TB_KEY_ARROW_UP:
                event->key = KEY_ARROW_UP;
                break;

            case TB_KEY_ARROW_DOWN:
                event->key = KEY_ARROW_DOWN;
                break;

            case TB_KEY_ARROW_LEFT:
                event->key = KEY_ARROW_LEFT;
                break;

            case TB_KEY_ARROW_RIGHT:
                event->key = KEY_ARROW_RIGHT;
                break;

            case TB_KEY_BACKSPACE:
            case TB_KEY_BACKSPACE2:
                event->key = KEY_BACKSPACE;
                break;

            default:
                if (!tevent.key) {
                    tb_utf8_unicode_to_char(&event->ch, tevent.ch);
                    event->key = KEY_CH;
                }
            }
            break;

        case TB_EVENT_RESIZE:
            event->type = EVENT_REDRAW;
            stop        = 1;
            break;

        case TB_EVENT_MOUSE:
            if (menu != NULL) {
                int selected_menu_item;
                selected_menu_item = tevent.y - MENU_ITEM_ROW;
                if ((selected_menu_item < menu->num_item) &&
                    (selected_menu_item >= 0) &&
                    (tevent.x >= MENU_ITEM_COL) &&
                    (tevent.x < MENU_ITEM_COL +
                     get_label_clickable_length(menu, selected_menu_item)) &&
                    (tevent.key == TB_KEY_MOUSE_LEFT)) {
                    event->type      = EVENT_MENU_ITEM_CLICKED;
                    event->menu_item = selected_menu_item;
                    stop             = 1;
                }
            } else { // The user clicked on the board
                if ((tevent.x <= SPACING_COL * 4 + PADDING_COL) &&
                    (tevent.x >= PADDING_COL) &&
                    (tevent.y <= SPACING_ROW * 4 + PADDING_ROW) &&
                    (tevent.y >= PADDING_ROW) &&
                    (tevent.key == TB_KEY_MOUSE_LEFT)) {
                    int x0 = tevent.x - PADDING_COL;
                    int y0 = tevent.y - PADDING_ROW;
                    if (((x0 % SPACING_COL == 0)) && (y0 % SPACING_ROW == 0)) {
                        // The user clicked on piece placement on the board
                        event->type       = EVENT_POSITION;
                        event->position.c = x0 / SPACING_COL;
                        event->position.r = y0 / SPACING_ROW;
                        stop = 1;
                    }
                } else if ((tevent.x >= QUIT_COL) &&
                           (tevent.x < QUIT_COL + QUIT_LEN) &&
                           (tevent.y == QUIT_ROW)) {
                    event->type = EVENT_QUIT;
                    stop        = 1;
                }
            }
            break;
        }
    } while (!stop);
}


void graphics_tb_quit(graphics_tb_t *tg) {
    tb_shutdown();
    free(tg);
}


graphics_callbacks_t graphics_tb_callbacks = {
    .draw_game       = graphics_tb_draw_game,
    .wait_event_game = graphics_tb_wait_game_event,
    .wait_event_menu = graphics_tb_wait_menu_event,
    .draw_menu       = graphics_tb_draw_menu
};
