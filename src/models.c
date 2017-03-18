#include "models.h"

cell_state_t board_get_cell(board_t *board, int row, int col) {
    return board->tab[row * 5 + col];
}


void board_set_cell(board_t *board, int row, int col, cell_state_t state) {
    board->tab[row * 5 + col] = state;
}


int is_position_possible(possible_positions_t *possible_pos,
                         position_t           position) {
    return possible_pos->ok[position.r * 5 + position.c];
}


int set_possible_position(possible_positions_t *possible_pos,
                          position_t position, int ok) {
    possible_pos->ok[position.r * 5 + position.c] = ok;
}
