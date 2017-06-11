#include <math.h>

#include "ai_heuristic.h"

// go_through_all_mvt calls the `action` function with each possible move from
// the given board state (and the given context, and game).
// Before every `action` call, the movement is done and undone afterward.
// If the `action` function returns true, the function stops.
static void go_through_all_mvt(game_t *game,
                               void   *context,
                               bool (*action)(void *context,
                                              game_t *game,
                                              mvt_t mvt)) {
    possible_positions_t possible_from_pos;
    possible_positions_t possible_to_pos;
    mvt_t                mvt;
    bool                 done = false;

    game_get_possible_from_positions(game, &possible_from_pos);

    if ((game->turn == GOAT_TURN) && (game->num_goats_to_put > 0)) {
        mvt.to.c = POSITION_NOT_SET;
        mvt.to.r = POSITION_NOT_SET;

        for (mvt.from.c = 0; mvt.from.c < 5; mvt.from.c++) {
            for (mvt.from.r = 0; mvt.from.r < 5; mvt.from.r++) {
                if (is_position_possible(&possible_from_pos, mvt.from)) {
                    game_do_mvt(game, mvt);
                    done = action(context, game, mvt);
                    game_undo(game);

                    if (done) {
                        return;
                    }
                }
            }
        }
    } else {
        for (mvt.from.c = 0; mvt.from.c < 5; mvt.from.c++) {
            for (mvt.from.r = 0; mvt.from.r < 5; mvt.from.r++) {
                if (is_position_possible(&possible_from_pos, mvt.from)) {
                    game_get_possible_to_positions(game, mvt.from,
                                                   &possible_to_pos);

                    for (mvt.to.c = 0; mvt.to.c < 5; mvt.to.c++) {
                        for (mvt.to.r = 0; mvt.to.r < 5; mvt.to.r++) {
                            if (is_position_possible(&possible_to_pos,
                                                     mvt.to)) {
                                game_do_mvt(game, mvt);
                                done = action(context, game, mvt);
                                game_undo(game);

                                if (done) {
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


// ai_heuristic_alphabeta_context defines a context for a
// `ai_heuristic_alphabeta` function call. This context is used so that we
// can iterate over movements with the `go_through_all_mvt` function.
struct ai_heuristic_alphabeta_context {
    double                  value;
    double                  alpha;
    double                  beta;
    int                     depth;
    bool                    maximizing;
    ai_heuristic_callback_t heuristic;
    void                    *heuristic_context;
    double                  heuristic_coeff;
    mvt_t                   best_mvt;
    int                     num_turns;
};

// See below.
static double ai_heuristic_alphabeta(struct ai_heuristic_alphabeta_context
                                            *context,
                                     game_t *game);


static double max(double x, double y) {
    return x > y ? x : y;
}


static double min(double x, double y) {
    return x < y ? x : y;
}


static bool ai_heuristic_alphabeta_maximizing(void   *c,
                                              game_t *game,
                                              mvt_t  mvt) {
    struct ai_heuristic_alphabeta_context *context = c;

    struct ai_heuristic_alphabeta_context call_context = {
        .value             = context->value,
        .depth             = context->depth - 1,
        .alpha             = context->alpha,
        .beta              = context->beta,
        .maximizing        = false,
        .heuristic         = context->heuristic,
        .heuristic_context = context->heuristic_context,
        .heuristic_coeff   = context->heuristic_coeff,
        .num_turns         = context->num_turns + 1,
    };

    double child_value = ai_heuristic_alphabeta(&call_context, game);

    if (context->value < child_value) {
        context->best_mvt = mvt;
        context->value    = child_value;
    }

    context->alpha = max(context->alpha, context->value);

    if (context->beta <= context->alpha) {
        return true; // Stops going througth the other movements.
    }

    return false;
}


static bool ai_heuristic_alphabeta_minimizing(void   *c,
                                              game_t *game,
                                              mvt_t  mvt) {
    struct ai_heuristic_alphabeta_context *context = c;

    struct ai_heuristic_alphabeta_context call_context = {
        .value             = context->value,
        .depth             = context->depth - 1,
        .alpha             = context->alpha,
        .beta              = context->beta,
        .maximizing        = true,
        .heuristic         = context->heuristic,
        .heuristic_context = context->heuristic_context,
        .heuristic_coeff   = context->heuristic_coeff,
        .num_turns         = context->num_turns + 1,
    };

    context->value = min(context->value,
                         ai_heuristic_alphabeta(&call_context, game));
    context->beta = min(context->beta, context->value);

    if (context->beta <= context->alpha) {
        return true; // Stops going througth the other movements.
    }

    return false;
}


// ai_heuristic_alphabeta implements a Alpa-Beta Pruning algorithm.
// See: https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
static double ai_heuristic_alphabeta(struct ai_heuristic_alphabeta_context
                                            *context,
                                     game_t *game) {
    if (context->depth == 0) {
        context->value = context->heuristic(context->heuristic_context,
                                            game,
                                            context->num_turns) *
                         context->heuristic_coeff;
    } else if (context->maximizing) {
        context->value = -INFINITY;
        go_through_all_mvt(game, context, ai_heuristic_alphabeta_maximizing);
    } else {
        context->value = +INFINITY;
        go_through_all_mvt(game, context, ai_heuristic_alphabeta_minimizing);
    }

    return context->value;
}


mvt_t ai_heuristic_get_mvt(game_t                  *game,
                           ai_heuristic_callback_t tiger_winning,
                           void                    *heuristic_context,
                           int                     depth) {
    struct ai_heuristic_alphabeta_context context = {
        .depth             = depth,
        .alpha             =                         -INFINITY,
        .beta              =                         +INFINITY,
        .maximizing        = true,
        .heuristic         = tiger_winning,
        .heuristic_context = heuristic_context,
        .heuristic_coeff   = game->turn == TIGER_TURN ? 1 : -1,
        .num_turns         =                                 0,
    };

    ai_heuristic_alphabeta(&context, game);
    return context.best_mvt;
}
