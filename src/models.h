#ifndef __MODELS_H__
#define __MODELS_H__

typedef enum {
    GOAT,
    TIGER,
    EMPTY
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
    GOAT,
    TIGER
} player_turn_t;

#endif
