#ifndef __MODELS_H__
#define __MODELS_H__

#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    GOAT_CELL,
    TIGER_CELL,
    EMPTY_CELL
} cell_state_t;

typedef struct {
    cell_state_t tab[5 * 5];
} board_t;

// possible_positions_t represent the possible position that the player can
// select. Each position is represented by a boolean. Position r, c is at
// index r*5+c. A position can be selected if its value is 1. 0 otherwhise.
typedef struct {
    char ok[5 * 5];
} possible_positions_t;

// POSITION_NOT_SET is used when a coordinate is not yet set.
// This is used when using the `mvt_t` structure as to store the input.
// It is also used to place goats. The `to` field is then marked as not set.
#define POSITION_NOT_SET    -1

typedef struct {
    int r;
    int c;
} position_t;

// mvt_t represents a movement in the game. When placing a goat on the board,
// the `from` field is marked as not set.
typedef struct {
    position_t from;
    position_t to;
} mvt_t;

typedef enum {
    GOAT_TURN,
    TIGER_TURN
} player_turn_t;


cell_state_t board_get_cell(board_t *board, position_t position);
void board_set_cell(board_t *board, position_t position, cell_state_t state);

//same as the 2 previous functions but they use position_t type declared above
cell_state_t get_cell(board_t *board, position_t cell);
void set_cell(board_t *board, position_t cell, cell_state_t state);

//a board file might help models being lighter

int is_position_possible(possible_positions_t *possible_pos,
                         position_t           position);


void set_possible_position(possible_positions_t *possible_pos,
                           position_t position, int ok);

//sets every value of pos (char[5][5]) to ok value
void init_positions(possible_positions_t *pos, int ok);

//1 if a diagonal goes through p, 0 otherwise
bool has_diagonal(position_t p);

//only for the square around a cell
bool on_same_rowcol(position_t p1, position_t p2);

//top left corner of the square around a cell
position_t top_left_cell(position_t p);

//bottom right corner of the square around a cell
position_t bot_right_cell(position_t p);

//1 if the cell is in the board, 0 otherwise
bool in_board(position_t p);

//comparator for position_t
bool equals_pos_t(position_t p1, position_t p2);

//'jump to cell' would mean an action while jump-to cell refers to the cell we want to jump to
//returns the symetric cell of tiger with respect to goat
position_t jumpto_cell(position_t tiger, position_t goat);

//1 if the tiger is going to eat the goat 0 otherwise
bool tiger_eats(mvt_t *m);

//position of the eaten goat
position_t eaten_goat_cell(mvt_t *m);

#endif
