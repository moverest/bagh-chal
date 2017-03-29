#include "graphics.h"

void test_graphics(void                           *context,
                   graphics_draw_callback_t       draw,
                   graphics_wait_event_callback_t wait_event) {
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

    draw(context, &state);
    event_t event;
    wait_event(context, &event);
    if (event.type == EVENT_QUIT) {
        return;
    }

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
                                                      1, 0, 0, 0, 1,
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0,
                                                      1, 0, 0, 0, 1
                                                  } };
    state.possible_positions = possible_positions_2;


    draw(context, &state);
    wait_event(context, &event);
    if (event.type == EVENT_QUIT) {
        return;
    }

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

    draw(context, &state);
    wait_event(context, &event);
    if (event.type == EVENT_QUIT) {
        return;
    }

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

    draw(context, &state);
    wait_event(context, &event);
}
