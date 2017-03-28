#include <stdio.h>
#include <stdlib.h>

#include "models.h"
#include "graphics.h"
#include "graphics_tb.h"

static void test_graphics_tb() {
    graphics_tb_t *graphics = graphics_tb_init();

    if (graphics == NULL) {
        fprintf(stderr, "Could not initialize termbox");
        return;
    }

    state_to_draw_t state;

    state.num_goats_to_put = 20;
    state.num_eaten_goats  = 0;
    state.turn             = GOAT_TURN;
    state.input.from.c     = 2;
    state.input.from.r     = POSITION_NOT_SET;
    state.input.to.c       = POSITION_NOT_SET;
    state.input.to.r       = POSITION_NOT_SET;

    board_t board = { {
                          TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL,
                          EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                          EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                          EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                          TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL
                      } };
    state.board = &board;

    possible_positions_t possible_positions_1 = { {
                                                      0, 1, 1, 1, 0,
                                                      1, 1, 1, 1, 1,
                                                      1, 1, 1, 1, 1,
                                                      1, 1, 1, 1, 1,
                                                      0, 1, 1, 1, 0
                                                  } };
    state.possible_positions = possible_positions_1;

    graphics_tb_draw(graphics, &state);
    struct tb_event event;
    tb_poll_event(&event);

    state.num_goats_to_put = 19;
    state.num_eaten_goats  = 0;
    state.turn             = TIGER_TURN;
    state.input.from.c     = 0;
    state.input.from.r     = POSITION_NOT_SET;
    state.input.to.c       = POSITION_NOT_SET;
    state.input.to.r       = POSITION_NOT_SET;

    board_t board1 = { {
                           TIGER_CELL, EMPTY_CELL, GOAT_CELL, EMPTY_CELL, TIGER_CELL,
                           EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                           EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                           EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                           TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL
                       } };
    state.board = &board1;

    possible_positions_t possible_positions_2 = { {
                                                      1, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0,
                                                      1, 0, 0, 0, 0
                                                  } };
    state.possible_positions = possible_positions_2;


    graphics_tb_draw(graphics, &state);
    tb_poll_event(&event);

    state.num_goats_to_put = 0;
    state.num_eaten_goats  = 0;
    state.turn             = TIGER_TURN;
    state.input.from.c     = 1;
    state.input.from.r     = 3;
    state.input.to.c       = 1;
    state.input.to.r       = POSITION_NOT_SET;

    board_t board2 = { {
                           TIGER_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL,
                           GOAT_CELL, GOAT_CELL, EMPTY_CELL, TIGER_CELL, GOAT_CELL,
                           GOAT_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL,
                           GOAT_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL,
                           TIGER_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL, TIGER_CELL
                       } };
    state.board = &board2;

    possible_positions_t possible_positions_3 = { {
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0,
                                                      0, 1, 0, 0, 0,
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0
                                                  } };
    state.possible_positions = possible_positions_3;

    graphics_tb_draw(graphics, &state);
    tb_poll_event(&event);

    state.num_goats_to_put = 0;
    state.num_eaten_goats  = 4;
    state.turn             = GOAT_TURN;
    state.input.from.c     = 3;
    state.input.from.r     = 2;
    state.input.to.c       = 3;
    state.input.to.r       = POSITION_NOT_SET;

    board_t board3 = { {
                           TIGER_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL, TIGER_CELL,
                           GOAT_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                           GOAT_CELL, EMPTY_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL,
                           GOAT_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL,
                           TIGER_CELL, GOAT_CELL, GOAT_CELL, GOAT_CELL, TIGER_CELL
                       } };
    state.board = &board3;

    possible_positions_t possible_positions_4 = { {
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 1, 0,
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0
                                                  } };
    state.possible_positions = possible_positions_4;

    graphics_tb_draw(graphics, &state);
    tb_poll_event(&event);

    graphics_tb_quit(graphics);
}


int main(int argc, char **argv) {
    test_graphics_tb();
    return 0;
}
