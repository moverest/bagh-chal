#include "ai_simple_heuristic.h"
#include "ai_heuristic.h"
#include "models.h"
#include "game.h"

// DEPTH defines the number of movements to look ahead.
#define DEPTH    8

void *ai_simple_heuristic_new() {
    return malloc(sizeof(ai_simple_heuristic_t));
}


void ai_simple_heuristic_free(void *context) {
    free(context);
}


static int get_num_tiger_on_diag(game_t *game) {
    int num = 0;

    for (int i = 0; i < 5 * 5; i += 2) {
        if (game->board.tab[i] == TIGER_CELL) {
            num += 1;
        }
    }

    return num;
}


static double tiger_winning(void *context, game_t *game, int num_turns) {
    double score = 0;

    score += game->num_eaten_goats * 20 / num_turns;
    score += get_num_tiger_on_diag(game) * 3;

    if (game_is_done(game)) {
        score -= 50;
    }
}


mvt_t ai_simple_heuristic_get_mvt(void *context, game_t *game) {
    return ai_heuristic_get_mvt(game, tiger_winning,
                                context, DEPTH);
}


ai_callbacks_t ai_simple_heuristic_callbacks = {
    .new           = ai_simple_heuristic_new,
    .free          = ai_simple_heuristic_free,
    .get_goat_mvt  = ai_simple_heuristic_get_mvt,
    .get_tiger_mvt = ai_simple_heuristic_get_mvt
};
