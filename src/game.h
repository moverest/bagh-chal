#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

#include "models.h"
#include "stack.h"

typedef struct {
    board_t       board;
    player_turn_t turn;
    int           num_goats_to_put;
    int           num_eaten_goats;
    stack_t       *history;
} game_t;

// game_reset resets the game.
void game_reset(game_t *g);

// game_new creates a new game object.
game_t *game_new();

// game_free free the game object from memory.
void game_free(game_t *g);



// game_get_possible_to_positions updates the `pos` structure with the
// valid to positions given a from position.
void game_get_possible_to_positions(game_t *g, position_t from_pos,
                                    possible_positions_t *pos);

// game_possible_from_positions updates the `pos` structure with the
// valid from positions.
void game_get_possible_from_positions(game_t *g, possible_positions_t *pos);


// game_do_mvt does the given movement for the current player if valid.
// Returns 0 if the movement is not feasible. 1 otherwhise.
bool game_do_mvt(game_t *g, mvt_t mvt);

// game_is_done returns 0 if the game is still on. 1 if the game is done.
// The looser can be retrived by looking at `game.turn`.
// If `game.turn == TIGER_TURN`, tigers have lost the game and goats won.
bool game_is_done(game_t *g);

// game_undo undoes the last movement.
// Returns 0 on success.
int game_undo(game_t *g);


// game_count_num_movable_tigers returns the number of tigers that can be moved.
int game_count_num_movable_tigers(game_t *game);

#endif
