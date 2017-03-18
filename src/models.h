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

typedef struct {
    int r;
    int c;
} position_t;

typedef struct {
    position_t from;
    position_t to;
} mvt_t;

typedef enum {
    GOAT_TURN,
    TIGER_TURN
} player_turn_t;

#endif
