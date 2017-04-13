#include "game.h"

game_t *game_new() {
    game_t *new_game = malloc(sizeof(game_t));

    game_reset(new_game);
    return new_game;
}


void game_free(game_t *g) {
    free(g);
}


void game_reset(game_t *g) {
    g->num_goats_to_put = 20;
    g->num_eaten_goats  = 0;

    // Goats begin the game
    g->turn = GOAT_TURN;

    position_t cell_id;
    for (cell_id.r = 0; cell_id.r < 5; cell_id.r++) {
        for (cell_id.c = 0; cell_id.c < 5; cell_id.c++) {
            board_set_cell(&(g->board), cell_id, EMPTY_CELL);
        }
    }

    // Put tigers at the corners
    board_set_cell(&(g->board), (position_t){0, 0 }, TIGER_CELL);
    board_set_cell(&(g->board), (position_t){0, 4 }, TIGER_CELL);
    board_set_cell(&(g->board), (position_t){4, 0 }, TIGER_CELL);
    board_set_cell(&(g->board), (position_t){4, 4 }, TIGER_CELL);
}


bool game_possible_movement(game_t *g, position_t from_pos,
                            possible_positions_t *pos, bool erase) {
    if (erase) {
        init_positions(pos, false);
    }

    bool movement_possible = false;

    if (g->turn == GOAT_TURN) {
        if (g->num_goats_to_put > 0) {
            position_t cell_id;

            for (cell_id.r = 0; cell_id.r < 5; cell_id.r++) {
                for (cell_id.c = 0; cell_id.c < 5; cell_id.c++) {
                    if (board_get_cell(&(g->board), cell_id) == EMPTY_CELL) {
                        set_possible_position(pos, cell_id, true);
                        movement_possible = true;
                    }
                }
            }
            return movement_possible;
        } else {
            if (get_cell(&(g->board), from_pos) != GOAT_CELL) {
                return false;
            } else {
                position_t top_left  = top_left_cell(from_pos);
                position_t bot_right = bot_right_cell(from_pos);

                position_t cell_id;

                //looks in the square around from_pos
                for (cell_id.r = top_left.r; cell_id.r <= bot_right.r; cell_id.r++) {
                    for (cell_id.c = top_left.c; cell_id.c <= bot_right.c; cell_id.c++) {
                        if (!equals_pos_t(cell_id, from_pos)) {
                            if ((get_cell(&(g->board), cell_id) == EMPTY_CELL) && (on_same_rowcol(cell_id, from_pos) || has_diagonal(from_pos))) {
                                set_possible_position(pos, cell_id, true);
                                movement_possible = true;
                            }
                        }
                    }
                }

                return movement_possible;
            }
        }
    } else if (g->turn == TIGER_TURN) {
        if (get_cell(&(g->board), from_pos) != TIGER_CELL) {
            return false;
        } else {
            position_t top_left  = top_left_cell(from_pos);
            position_t bot_right = bot_right_cell(from_pos);

            position_t   cell_id;
            cell_state_t target_cell;

            //looks in the square around from_pos
            for (cell_id.r = top_left.r; cell_id.r <= bot_right.r; cell_id.r++) {
                for (cell_id.c = top_left.c; cell_id.c <= bot_right.c; cell_id.c++) {
                    if (!equals_pos_t(cell_id, from_pos)) {
                        target_cell = get_cell(&(g->board), cell_id);
                        if ((target_cell == EMPTY_CELL) && (on_same_rowcol(cell_id, from_pos) || has_diagonal(from_pos))) {
                            set_possible_position(pos, cell_id, true);
                            movement_possible = true;
                        } else if (target_cell == GOAT_CELL) {
                            position_t jump_to = jumpto_cell(from_pos, cell_id);

                            //predicate returns false if jump_to is not in board, before calling get_cell
                            if (in_board(jump_to) && (get_cell(&(g->board), jump_to) == EMPTY_CELL)) {
                                set_possible_position(pos, jump_to, true);
                                movement_possible = true;
                            }
                        }
                    }
                }
            }
            return movement_possible;
        }
    }

    //default value in case algorithm doesn't work
    return false;
}


int game_find_movable_pawns(game_t *g, possible_positions_t *pos) {
    int movable_pawns = 0;
    possible_positions_t mute_pos;

    init_positions(pos, false);

    position_t cell_id;
    for (cell_id.r = 0; cell_id.r < 5; cell_id.r++) {
        for (cell_id.c = 0; cell_id.c < 5; cell_id.c++) {
            if (game_possible_movement(g, cell_id, &mute_pos, false)) {
                set_possible_position(pos, cell_id, true);
                movable_pawns++;
            }
        }
    }

    if ((g->turn == GOAT_TURN) && (g->num_goats_to_put > 0)) {
        return 1;
    }
    return movable_pawns;
}


bool game_do_movement(game_t *g, mvt_t *mvt) {
    possible_positions_t target_cells;

    if (game_possible_movement(g, mvt->from, &target_cells, true)) {
        if (is_position_possible(&target_cells, mvt->to)) {
            if (tiger_eats(mvt)) {
                set_cell(&(g->board), eaten_goat_cell(mvt), EMPTY_CELL);
            }
            set_cell(&(g->board), mvt->to, get_cell(&(g->board), mvt->from));
            set_cell(&(g->board), mvt->from, EMPTY_CELL);
        }
    }
    return false;
}


bool game_is_done(game_t *g) {
    possible_positions_t board;

    return (g->turn == TIGER_TURN && game_find_movable_pawns(g, &board) == 0) || (g->num_eaten_goats >= 5);
}
