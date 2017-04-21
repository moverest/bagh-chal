#include <stdbool.h>
#include <stdlib.h>

#include "models.h"

cell_state_t board_get_cell(board_t *board, position_t position) {
    return board->tab[position.r * 5 + position.c];
}


void board_set_cell(board_t *board, position_t position, cell_state_t state) {
    board->tab[position.r * 5 + position.c] = state;
}


bool is_position_possible(possible_positions_t *possible_pos,
                          position_t           position) {
    return possible_pos->ok[position.r * 5 + position.c];
}


void set_possible_position(possible_positions_t *possible_pos,
                           position_t position, bool ok) {
    possible_pos->ok[position.r * 5 + position.c] = ok;
}


void reset_possible_positions(possible_positions_t *possible_positions) {
    position_t pos;

    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            set_possible_position(possible_positions, pos, false);
        }
    }
}


bool position_is_valid(position_t pos) {
    return pos.r >= 0 && pos.c >= 0 && pos.r < 5 && pos.c < 5;
}


bool position_has_diagonal(position_t pos) {
    return (pos.r + pos.c) % 2 == 0;
}


bool position_equals(position_t pos1, position_t pos2) {
    return pos1.c == pos2.c && pos1.r == pos2.r;
}


bool mvt_is_diagonal(mvt_t mvt) {
    return abs(mvt.to.c - mvt.from.c) == abs(mvt.to.r - mvt.from.r);
}
