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
    mvt_t           possible_mvts[3]; // TODO: Set possible movements
    state.num_possible_mvts = 3;
    board_t board;

    state.num_goats_to_put = 10;
    state.num_eaten_goats  = 10;
    state.turn             = TIGER_TURN;
    state.input.from.r     = 0;
    state.input.from.c     = 1;
    state.input.to.r       = 2;
    state.input.to.c       = 1;
    state.possible_mvts    = possible_mvts;
    state.board            = &board;

    graphics_tb_draw(graphics, &state);

    // Wait for an event before exiting.
    struct tb_event event;
    tb_poll_event(&event);

    graphics_tb_quit(graphics);
}


int main(int argc, char **argv) {
    test_graphics_tb();
    return 0;
}
