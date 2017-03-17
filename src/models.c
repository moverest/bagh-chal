#include "models.h"

cell_state_t board_get_cell(board_t *board, int row, int col) {
    return board->tab[row * 5 + col];
}


void board_set_cell(board_t *board, int row, int col, cell_state_t state) {
    board->tab[row * 5 + col] = state;
}
