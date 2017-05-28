#include "ai.h"
#include "ai_rand.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void *ai_rand_new() {
    srand(time(NULL));
    return malloc(sizeof(ai_rand_t));
}


void ai_rand_free(void *context) {
    free(context);
}


static position_t get_random_position(possible_positions_t *possible_pos) {
    int        num_possible_pos = possible_positions_count(possible_pos);
    int        selection        = rand() % num_possible_pos;
    position_t pos;

    for (pos.c = 0; pos.c < 5; pos.c++) {
        for (pos.r = 0; pos.r < 5; pos.r++) {
            if (is_position_possible(possible_pos, pos)) {
                if (selection == 0) {
                    return pos;
                }

                selection--;
            }
        }
    }

    return pos;
}


mvt_t ai_rand_get_mvt(void *context, game_t *game) {
    possible_positions_t possible_pos;
    mvt_t                mvt;

    game_get_possible_from_positions(game, &possible_pos);
    mvt.from = get_random_position(&possible_pos);

    if ((game->turn == GOAT_TURN) && (game->num_goats_to_put > 0)) {
        mvt.to.c = POSITION_NOT_SET;
        mvt.to.r = POSITION_NOT_SET;
        return mvt;
    }

    game_get_possible_to_positions(game, mvt.from, &possible_pos);
    mvt.to = get_random_position(&possible_pos);

    return mvt;
}


ai_callbacks_t ai_rand_callbacks = {
    .new           = ai_rand_new,
    .free          = ai_rand_free,
    .get_goat_mvt  = ai_rand_get_mvt,
    .get_tiger_mvt = ai_rand_get_mvt
};
