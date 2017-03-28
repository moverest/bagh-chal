#include "models.h"

cell_state_t board_get_cell(board_t *board, position_t position) {
    return board->tab[position.r * 5 + position.c];
}


void board_set_cell(board_t *board, position_t position, cell_state_t state) {
    board->tab[position.r * 5 + position.c] = state;
}


int is_position_possible(possible_positions_t *possible_pos,
                         position_t           position) {
    return possible_pos->ok[position.r * 5 + position.c];
}


void set_possible_position(possible_positions_t *possible_pos,
                           position_t position, int ok) {
    possible_pos->ok[position.r * 5 + position.c] = ok;
}
