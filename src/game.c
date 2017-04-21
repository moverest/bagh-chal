#include <stdlib.h>

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

    // Clear the board
    position_t pos;
    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            board_set_cell(&(g->board), pos, EMPTY_CELL);
        }
    }

    // Put tigers in the corners
    board_set_cell(&g->board, (position_t){0, 0 }, TIGER_CELL);
    board_set_cell(&g->board, (position_t){0, 4 }, TIGER_CELL);
    board_set_cell(&g->board, (position_t){4, 0 }, TIGER_CELL);
    board_set_cell(&g->board, (position_t){4, 4 }, TIGER_CELL);
}


// test_possible_position tests from a given position if a move can be done.
// If possible_dest is set, then if the token can be moved to the given postion
// then the destination is maked as possible.
//
// If test_diagonals is set to true, we only care about the diagonals. Meaning
// top-left, top-right, bottom_left and bottom-right.
// Otherwhise, we only look at the top, bottom, left and right positions.
//
// Returns true a movement is possible.
static bool test_possible_position(board_t *board, position_t pos, bool test_diagonals,
                                   possible_positions_t *possible_dest) {
    int mvt_possible = false;


    /*   not test_diagonals     |      test_diagonals
     *     .    +    .          |        +    .    +
     *          |               |          \     /
     *     + -- O -- +          |        .    O    .
     *          |               |          /     \
     *     .    +   .           |        +         +
     *
     *  . not tested
     *  + tested
     */

    int        diag_offset = test_diagonals ? 1 : 0;
    position_t to_check[]  = {
        { pos.c + 1,           pos.r + diag_offset },
        { pos.c - 1,           pos.r - diag_offset },
        { pos.c + diag_offset, pos.r + 1           },
        { pos.c - diag_offset, pos.r - 1           }
    };


    for (int i = 0; i < sizeof(to_check) / sizeof(to_check[0]); i++) {
        if (position_is_valid(to_check[i]) &&
            (board_get_cell(board, to_check[i]) == EMPTY_CELL)) {
            if (possible_dest == NULL) {
                return true;
            } else {
                set_possible_position(possible_dest, to_check[i], true);
                mvt_possible = true;
            }
        }
    }

    if (board_get_cell(board, pos) == TIGER_CELL) {
        // We have a tiger, so we can jump and eat other goats.

        int        diag_offset     = test_diagonals ? 2 : 0;
        position_t jump_to_check[] = {
            { pos.c + 2,           pos.r + diag_offset },
            { pos.c - 2,           pos.r - diag_offset },
            { pos.c + diag_offset, pos.r + 2           },
            { pos.c - diag_offset, pos.r - 2           }
        };


        for (int i = 0; i < sizeof(to_check) / sizeof(to_check[0]); i++) {
            if (position_is_valid(to_check[i]) &&
                (board_get_cell(board, jump_to_check[i]) == EMPTY_CELL) &&
                (board_get_cell(board, to_check[i]) == GOAT_CELL)) {
                if (possible_dest == NULL) {
                    return true;
                } else {
                    set_possible_position(possible_dest, jump_to_check[i], true);
                    mvt_possible = true;
                }
            }
        }
    }

    return mvt_possible;
}


static void mark_possible_movable_from_positions(board_t *board,
                                                 possible_positions_t *possible_positions, player_turn_t turn) {
    cell_state_t movable_cell = turn == TIGER_TURN ? TIGER_CELL : GOAT_CELL;

    position_t pos;

    for (pos.c = 0; pos.c < 5; pos.c++) {
        for (pos.r = 0; pos.r < 5; pos.r++) {
            if (board_get_cell(board, pos) == movable_cell) {
                bool movable = test_possible_position(board, pos, false, NULL);
                if (position_has_diagonal(pos)) {
                    movable |= test_possible_position(board, pos, true, NULL);
                }

                set_possible_position(possible_positions, pos, movable);
            }
        }
    }
}


static void mark_possible_movable_to_positons(board_t *board, position_t from_pos,
                                              possible_positions_t *possible_positions, player_turn_t turn) {
    cell_state_t movable_cell = turn == TIGER_TURN ? TIGER_CELL : GOAT_CELL;

    if (board_get_cell(board, from_pos) == movable_cell) {
        test_possible_position(board, from_pos, false, possible_positions);
        if (position_has_diagonal(from_pos)) {
            test_possible_position(board, from_pos, true, possible_positions);
        }
    }
}


static void mark_empty_positions(board_t *board, possible_positions_t *possible_pos) {
    position_t pos;

    for (pos.c = 0; pos.c < 5; pos.c++) {
        for (pos.r = 0; pos.r < 5; pos.r++) {
            if (board_get_cell(board, pos) == EMPTY_CELL) {
                set_possible_position(possible_pos, pos, true);
            }
        }
    }
}


void game_get_possible_from_positions(game_t               *game,
                                      possible_positions_t *possible_pos) {
    reset_possible_positions(possible_pos);
    switch (game->turn) {
    case TIGER_TURN:
        mark_possible_movable_from_positions(&game->board,
                                             possible_pos,
                                             game->turn);
        break;

    case GOAT_TURN:
        if (game->num_goats_to_put == 0) {
            mark_possible_movable_from_positions(&game->board,
                                                 possible_pos,
                                                 game->turn);
        } else {
            mark_empty_positions(&game->board,
                                 possible_pos);
        }
    }
}


void game_get_possible_to_positions(game_t *game, position_t from_pos,
                                    possible_positions_t *possible_pos) {
    reset_possible_positions(possible_pos);
    mark_possible_movable_to_positons(&game->board, from_pos,
                                      possible_pos, game->turn);
}


#define MAX(x, y)    x > y ? x : y

bool game_do_mvt(game_t *game, mvt_t mvt) {
    if (!position_is_valid(mvt.from)) {
        return false;
    }

    if ((game->turn == GOAT_TURN) && (game->num_goats_to_put > 0)) {
        // We are placing a goat.

        if (board_get_cell(&game->board, mvt.from) != EMPTY_CELL) {
            return false;
        }

        board_set_cell(&game->board, mvt.from, GOAT_CELL);
        game->num_goats_to_put--;
        game->turn = TIGER_TURN;
        return true;
    }

    // From this point on, we are moving a token.

    if (!position_is_valid(mvt.to) || position_equals(mvt.from, mvt.to)) {
        return false;
    }

    if ((board_get_cell(&game->board, mvt.from) == EMPTY_CELL) ||
        (board_get_cell(&game->board, mvt.to) != EMPTY_CELL)) {
        return false;
    }

    if (!position_has_diagonal(mvt.to) && mvt_is_diagonal(mvt)) {
        return false;
    }

    switch (MAX(abs(mvt.to.c - mvt.from.c), abs(mvt.to.r - mvt.from.r))) {
    case 1:
        // We are not eating a goat, just moving a token to the nearest
        // position.
        ;

        cell_state_t cell_state_to_move = board_get_cell(&game->board, mvt.from);
        if (((cell_state_to_move == TIGER_CELL) && (game->turn != TIGER_TURN)) ||
            ((cell_state_to_move == GOAT_CELL) && (game->turn != GOAT_TURN))) {
            return false;
        }

        board_set_cell(&game->board, mvt.from, EMPTY_CELL);
        board_set_cell(&game->board, mvt.to, cell_state_to_move);
        game->turn = game->turn == GOAT_TURN ? TIGER_TURN : GOAT_TURN;
        return true;

    case 2:     // We are eating a goat.
        if ((game->turn != TIGER_TURN) ||
            (board_get_cell(&game->board, mvt.from) != TIGER_CELL)) {
            return false;
        }

        position_t eaten_goat_pos = {
            (mvt.to.c + mvt.from.c) / 2,
            (mvt.to.r + mvt.from.r) / 2
        };

        if (board_get_cell(&game->board, eaten_goat_pos) != GOAT_CELL) {
            return false;
        }

        board_set_cell(&game->board, mvt.from, EMPTY_CELL);
        board_set_cell(&game->board, eaten_goat_pos, EMPTY_CELL);
        board_set_cell(&game->board, mvt.to, TIGER_CELL);
        game->num_eaten_goats++;
        game->turn = GOAT_TURN;
        return true;
    }

    return false;
}


static bool is_blocked(board_t *board, player_turn_t turn) {
    cell_state_t movable_cell = turn == TIGER_TURN ? TIGER_CELL : GOAT_CELL;

    position_t pos;

    for (pos.c = 0; pos.c < 5; pos.c++) {
        for (pos.r = 0; pos.r < 5; pos.r++) {
            if (board_get_cell(board, pos) == movable_cell) {
                if (test_possible_position(board, pos, false, NULL)) {
                    return false;
                }
                if (position_has_diagonal(pos)) {
                    if (test_possible_position(board, pos, true, NULL)) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}


bool game_is_done(game_t *game) {
    if (game->num_eaten_goats > 4) {
        return true;
    }

    if (is_blocked(&game->board, game->turn)) {
        return true;
    }

    return false;
}
