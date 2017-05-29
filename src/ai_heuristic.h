#ifndef __AI_HEURISTIC_H__
#define __AI_HEURISTIC_H__

#include "game.h"

// ai_heuristic_callback_t is a callback to a function that returns double.
// Given a game, the heigher the value, the more likly tigers are going to win.
typedef double (*ai_heuristic_callback_t)(void *context, game_t *game,
                                          int num_turns);

// ai_heuristic_get_mvt returns the best movement possible looking `depth`
// movements ahead with the given `tiger_winning` heuristic.
// `heuristic_context` is passed to `tiger_winning` when called.
mvt_t ai_heuristic_get_mvt(game_t                  *game,
                           ai_heuristic_callback_t tiger_winning,
                           void                    *heuristic_context,
                           int                     depth);


#endif
