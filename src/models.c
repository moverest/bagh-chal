#include "models.h"

cell_state_t board_get_cell(board_t *board, position_t position) {
    return board->tab[position.r * 5 + position.c];
}


void board_set_cell(board_t *board, position_t position, cell_state_t state) {
    board->tab[position.r * 5 + position.c] = state;
}


cell_state_t get_cell(board_t *board, position_t cell) {
    return board->tab[cell.r * 5 + cell.c];
}


void set_cell(board_t *board, position_t cell, cell_state_t state) {
    board->tab[cell.r * 5 + cell.c] = state;
}


int is_position_possible(possible_positions_t *possible_pos,
                         position_t           position) {
    return possible_pos->ok[position.r * 5 + position.c];
}


void set_possible_position(possible_positions_t *possible_pos,
                           position_t position, int ok) {
    possible_pos->ok[position.r * 5 + position.c] = ok;
}


void init_positions(possible_positions_t *pos, int ok) {
    position_t cell_id;

    for (cell_id.r = 0; cell_id.r < 5; cell_id.r++) {
        for (cell_id.c = 0; cell_id.c < 5; cell_id.c++) {
            set_possible_position(pos, cell_id, ok);
        }
    }
}


bool has_diagonal(position_t p) {
    return 1 - (p.r + p.c) % 2;
}


bool on_same_rowcol(position_t p1, position_t p2) {
    return (p1.r + p1.c + p2.r + p2.c) % 2;
}


bool in_board(position_t p) {
    return p.r >= 0 && p.c >= 0 && p.r < 5 && p.c < 5;
}


position_t top_left_cell(position_t p) {
    if (p.r > 0) {
        p.r--;
    }
    if (p.c > 0) {
        p.c--;
    }
    return p;
}


position_t bot_right_cell(position_t p) {
    if (p.r < 4) {
        p.r++;
    }
    if (p.c < 4) {
        p.c++;
    }
    return p;
}


bool equals_pos_t(position_t p1, position_t p2) {
    return p1.r == p2.r && p1.c == p2.c;
}


position_t jumpto_cell(position_t tiger, position_t goat) {
    return (position_t){
               2 * goat.r - tiger.r, 2 * goat.c - tiger.c
    };
}


bool tiger_eats(mvt_t *m) {
    return 2 == abs(m->from.r - m->to.r) + abs(m->from.c - m->to.c);
}


position_t eaten_goat_cell(mvt_t *m) {
    return (position_t){
               (m->to.r + m->from.r) / 2, (m->to.c + m->from.c) / 2
    };
}
