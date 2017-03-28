#include <termbox.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graphics_tb.h"


/*
 *
 *       a      b      c      d      e
 *    0 + ---- + ---- + ---- + ---- +                       < PADDING_ROW
 *      | \    |    / | \    |    / |
 *      |    \ | /    |    \ | /    |       Tiger turn      < TURN_ROW
 *    1 + ---- + ---- + ---- + ---- +
 *      |    / | \    |    / | \    |       19 Goats left   < GOAT_LEFT_ROW
 *      | /    |    \ | /    |    \ |       0 Goat eaten    < GOAT_EATEN_ROW
 *    2 + ---- + ---- + ---- + ---- +
 *      | \    |    / | \    |    / |
 *      |    \ | /    |    \ | /    |
 *    3 + ---- + ---- + ---- + ---- +                       <-+ SPACING_ROW
 *      |    / | \    |    / | \    |                         |
 *      | /    |    \ | /    |    \ |                         |
 *    4 + ---- + ---- + ---- + ---- +                       <-+
 *
 *      ^                    ^      ^       ^
 *      PADDING_COL          +------+       TURN_COL
 *                           SPACING_ROW    GOAT_EATEN_COL
 *                                          GOAT_LEFT_COL
 */

#define PADDING_COL                 4
#define PADDING_ROW                 2
#define GOAT_EATEN_ROW              40
#define GOAT_EATEN_COL              7
#define GOAT_LEFT_ROW               40
#define GOAT_LEFT_COL               6
#define TURN_ROW                    40
#define TURN_COL                    4
#define MOVE_ROW                    17
#define MOVE_COL                    1

#define POSSIBLE_POSSITION_COLOR    TB_GREEN
#define GOAT_COLOR                  TB_CYAN
#define TIGER_COLOR                 TB_RED
#define EMPTY_COLOR                 TB_DEFAULT
#define INPUT_POS_COLOR             TB_YELLOW
#define SELECTION_COLOR             TB_YELLOW


#define SPACING_COL                 7
#define SPACING_ROW                 3

graphics_tb_t *graphics_tb_init() {
    graphics_tb_t *tg = malloc(sizeof(graphics_tb_t));

    if (tg == NULL) {
        return NULL;
    }

    if (tb_init()) {
        return NULL;
    }

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


#define BOARD_TXT_PADDING_COL    PADDING_COL - 2

// draw_board draws a blanc board.
static void draw_board() {
    int i = PADDING_ROW - 1;

    print_str("  a      b      c      d      e", BOARD_TXT_PADDING_COL, i++);
    print_str("0 + ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
    print_str("  | \\    |    / | \\    |    / |", BOARD_TXT_PADDING_COL, i++);
    print_str("  |    \\ | /    |    \\ | /    |", BOARD_TXT_PADDING_COL, i++);
    print_str("1 + ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
    print_str("  |    / | \\    |    / | \\    |", BOARD_TXT_PADDING_COL, i++);
    print_str("  | /    |    \\ | /    |    \\ |", BOARD_TXT_PADDING_COL, i++);
    print_str("2 + ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
    print_str("  | \\    |    / | \\    |    / |", BOARD_TXT_PADDING_COL, i++);
    print_str("  |    \\ | /    |    \\ | /    |", BOARD_TXT_PADDING_COL, i++);
    print_str("3 + ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
    print_str("  |    / | \\    |    / | \\    |", BOARD_TXT_PADDING_COL, i++);
    print_str("  | /    |    \\ | /    |    \\ |", BOARD_TXT_PADDING_COL, i++);
    print_str("4 + ---- + ---- + ---- + ---- +", BOARD_TXT_PADDING_COL, i++);
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


// draw_gui writes what is write in it.
static void draw_gui(state_to_draw_t *state) {
    draw_turn(state->turn);
    draw_goat_left(state->num_goats_to_put);
    draw_goat_eaten(state->num_eaten_goats);
}


// draw_possible_positions writes the possible possible_positions on the board
// with their label (their coordinates).
static void draw_possible_positions(possible_positions_t *possible_positions,
                                    mvt_t                input) {
    int selected_col = POSITION_NOT_SET;

    if ((input.from.c != POSITION_NOT_SET) && (input.from.r == POSITION_NOT_SET)) {
        selected_col = input.from.c;
    } else if ((input.to.c != POSITION_NOT_SET) && (input.to.r == POSITION_NOT_SET)) {
        selected_col = input.to.c;
    }

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
                               'a' + pos.c,
                               POSSIBLE_POSSITION_COLOR,
                               TB_DEFAULT);

                tb_change_cell(SPACING_COL * pos.c + PADDING_COL + 1,
                               SPACING_ROW * pos.r + PADDING_ROW,
                               pos.r + '0',
                               POSSIBLE_POSSITION_COLOR,
                               TB_DEFAULT);

                tb_change_cell(SPACING_COL * pos.c + PADDING_COL + 2,
                               SPACING_ROW * pos.r + PADDING_ROW,
                               ' ',
                               POSSIBLE_POSSITION_COLOR,
                               TB_DEFAULT);
            }
        }
    }
}


// draw_input draws the input.
static void draw_input(state_to_draw_t *state) {
    int  x = MOVE_COL;
    char inp[4];

    inp[0] = state->input.from.c == POSITION_NOT_SET ? ' ' : 'a' + state->input.from.c;
    inp[1] = state->input.from.r == POSITION_NOT_SET ? ' ' : '0' + state->input.from.r;
    inp[2] = state->input.to.c == POSITION_NOT_SET ? ' ' : 'a' + state->input.to.c;
    inp[3] = state->input.to.r == POSITION_NOT_SET ? ' ' : '0' + state->input.to.r;

    if ((state->turn == GOAT_TURN) && (state->num_goats_to_put != 0)) {
        x += print_str("Put ", x, MOVE_ROW);
        for (int i = 0; i < 2 && inp[i] != ' '; i++) {
            tb_change_cell(x, MOVE_ROW, inp[i], INPUT_POS_COLOR, TB_DEFAULT);
            x++;
        }
    } else {
        x += print_str("Move ", x, MOVE_ROW);
        for (int i = 0; i < 4 && inp[i] != ' '; i++) {
            tb_change_cell(x, MOVE_ROW, inp[i], INPUT_POS_COLOR, TB_DEFAULT);
            x++;
            if ((i == 1)) {
                x += print_str(" to ", x, MOVE_ROW);
            }
        }
    }
    tb_set_cursor(x, MOVE_ROW);
}


// graphics_tb_draw draws the whole screen.
void graphics_tb_draw(void *context, state_to_draw_t *state) {
    tb_clear();
    draw_board();
    draw_gui(state);
    draw_cells(state->board);
    draw_possible_positions(&state->possible_positions, state->input);
    draw_input(state);
    tb_present();
}


void graphics_tb_wait_event(void *context, event_t *event) {
    //TODO: Emplement.

    struct tb_event tevent;

    tb_poll_event(&tevent);

    if (tevent.key == TB_KEY_ESC) {
        event->type = EVENT_QUIT;
        return;
    }

    event->type = EVENT_KEY;
    event->key  = KEY_BACKSPACE;
}


void graphics_tb_quit(graphics_tb_t *tg) {
    tb_shutdown();
    free(tg);
}
