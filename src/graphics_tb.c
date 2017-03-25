#include <termbox.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graphics_tb.h"

#define MULT_COL                    3
#define MULT_ROW                    7
#define DECALAGE_COL                1
#define DECALAGE_ROW                2
#define GOAT_EATEN_ROW              40
#define GOAT_EATEN_COL              7
#define GOAT_LEFT_ROW               40
#define GOAT_LEFT_COL               6
#define TURN_ROW                    40
#define TURN_COL                    4
#define MOVE_ROW                    17
#define MOVE_COL                    1
#define COLOR_POSSIBLE_POSSITION    TB_CYAN


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


// 'placement_tb_draw' places tigers/goats which are already on the board before the beginning of the tour.
void placement_tb_draw(board_t *board) {
    struct tb_cell cell = { '0', TB_DEFAULT, TB_DEFAULT };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            switch (board_get_cell(board, i, j)) {
            case GOAT_CELL:
                cell.ch = 'G';
                cell.fg = TB_GREEN;
                break;

            case TIGER_CELL:
                cell.ch = 'T';
                cell.fg = TB_RED;
                break;

            case EMPTY_CELL:
                cell.ch = '+';
                cell.fg = TB_DEFAULT;
                break;
            }
            tb_put_cell(MULT_ROW * i + DECALAGE_ROW, MULT_COL * j + DECALAGE_COL, &cell);
        }
    }
}


// 'print_str' prints a string to given coordinate.
int print_str(char *str, int row, int col) {
    int i = 0;

    while (str[i] != '\0') {
        tb_change_cell(row + i, col, str[i], TB_DEFAULT, TB_DEFAULT);
        i++;
    }

    return i;
}


// 'board_tb_draw' draws a blanc board.
void board_tb_draw() {
    print_str("  a      b      c      d      e", 0, 0);
    print_str("0 + ---- + ---- + ---- + ---- +", 0, 1);
    print_str("  | \\    |    / | \\    |    / |", 0, 2);
    print_str("  |    \\ | /    |    \\ | /    |", 0, 3);
    print_str("1 + ---- + ---- + ---- + ---- +", 0, 4);
    print_str("  |    / | \\    |    / | \\    |", 0, 5);
    print_str("  | /    |    \\ | /    |    \\ |", 0, 6);
    print_str("2 + ---- + ---- + ---- + ---- +", 0, 7);
    print_str("  | \\    |    / | \\    |    / |", 0, 8);
    print_str("  |    \\ | /    |    \\ | /    |", 0, 9);
    print_str("3 + ---- + ---- + ---- + ---- +", 0, 10);
    print_str("  |    / | \\    |    / | \\    |", 0, 11);
    print_str("  | /    |    \\ | /    |    \\ |", 0, 12);
    print_str("4 + ---- + ---- + ---- + ---- +", 0, 13);
}


// 'goat_left_draw' writes the number of goats left.
void goat_left_draw(int goat_left) {
    char gl[32];

    sprintf(gl, goat_left <= 1 ? "%d Goat left" : "%d Goats left", goat_left);
    print_str(gl, GOAT_LEFT_ROW, GOAT_LEFT_COL);
}


// 'goat_eaten_draw' writes the number of goat eaten.
void goat_eaten_draw(int goat_eaten) {
    char ge[32];

    sprintf(ge, goat_eaten <= 1 ? "%d Goat eaten" : "%d Goats eaten", goat_eaten);
    print_str(ge, GOAT_EATEN_ROW, GOAT_EATEN_COL);
}


void turn_draw(player_turn_t turn) {
    print_str(turn == GOAT_TURN ? "Goat turn" : "Tiger turn", TURN_ROW, TURN_COL);
}


// 'gui_tb_draw' writes what is write in it.
void gui_tb_draw(state_to_draw_t *state) {
    turn_draw(state->turn);
    goat_left_draw(state->num_goats_to_put);
    goat_eaten_draw(state->num_eaten_goats);
}


// 'possible_positions_tb_draw' writes yellow '*' at possible positions.
void possible_positions_tb_draw(possible_positions_t *possible_positions) {
    position_t pos = { 0, 0 };

    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            if (is_position_possible(possible_positions, pos)) {
                tb_change_cell(MULT_ROW * pos.r + DECALAGE_ROW, MULT_COL * pos.c + DECALAGE_COL, 'a' + pos.r, COLOR_POSSIBLE_POSSITION, TB_DEFAULT);
                tb_change_cell(MULT_ROW * pos.r + DECALAGE_ROW + 1, MULT_COL * pos.c + DECALAGE_COL, pos.c + '0', COLOR_POSSIBLE_POSSITION, TB_DEFAULT);
                tb_change_cell(MULT_ROW * pos.r + DECALAGE_ROW + 2, MULT_COL * pos.c + DECALAGE_COL, ' ', COLOR_POSSIBLE_POSSITION, TB_DEFAULT);
            }
        }
    }
}


// 'input_tb_draw' writes the input.
void input_tb_draw(state_to_draw_t *state) {
    int  x = MOVE_COL;
    char inp[5];

    inp[0] = state->input.from.c == POSITION_NOT_SET ? ' ' : 'a' + state->input.from.c;
    inp[1] = state->input.from.r == POSITION_NOT_SET ? ' ' : state->input.from.r + '0';
    inp[2] = state->input.to.c == POSITION_NOT_SET ? ' ' : 'a' + state->input.to.c;
    inp[3] = state->input.to.r == POSITION_NOT_SET ? ' ' : state->input.to.r + '0';

    if (state->turn == GOAT_TURN && state->num_goats_to_put != 0) {
        x += print_str("Put ", x, MOVE_ROW);
        for (int i = 0; i < 2 && inp[i] != ' '; i++) {
            tb_change_cell(x, MOVE_ROW, inp[i], TB_DEFAULT, TB_DEFAULT);
            x++;
        }
    } else {
        x += print_str("Move ", x, MOVE_ROW);
        for (int i = 0; i < 4 && inp[i] != ' '; i++) {
            tb_change_cell(x, MOVE_ROW, inp[i], TB_DEFAULT, TB_DEFAULT);
            x++;
            if ((i == 1)) {
                x += print_str(" to ", x, MOVE_ROW);
            }
        }
    }
    tb_set_cursor(x, MOVE_ROW);
}


// 'graphics_tb_draw' draws all the graphics.
void graphics_tb_draw(void *context, state_to_draw_t *state) {
    tb_clear();
    board_tb_draw();
    gui_tb_draw(state);
    placement_tb_draw(state->board);
    possible_positions_tb_draw(&state->possible_positions);
    input_tb_draw(state);
    tb_present();
}


void graphics_tb_quit(graphics_tb_t *tg) {
    tb_shutdown();
    free(tg);
}
