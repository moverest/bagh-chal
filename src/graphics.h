#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "models.h"

typedef struct {
    int           num_goats_to_put;
    int           num_eaten_goats;
    player_turn_t turn;
    mvt_t         input;
    mvt_t         *possible_mvts;
    size_t        num_possible_mvts;
    board_t       *board;
} state_to_draw_t;

#endif
