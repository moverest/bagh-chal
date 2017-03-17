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

typedef struct {
    int l;
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
