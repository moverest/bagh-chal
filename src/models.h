#ifndef __MODELS_H__
#define __MODELS_H__

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
// the `to` field is marked as not set.
typedef struct {
    position_t from;
    position_t to;
} mvt_t;

typedef enum {
    GOAT_TURN,
    TIGER_TURN
} player_turn_t;


cell_state_t board_get_cell(board_t *board, int row, int col);
void board_set_cell(board_t *board, int row, int col, cell_state_t state);

int is_position_possible(possible_positions_t *possible_pos,
                         position_t           position);

int set_possible_position(possible_positions_t *possible_pos,
                          position_t position, int ok);

#endif
