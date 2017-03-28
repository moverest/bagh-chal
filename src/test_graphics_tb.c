#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "graphics_tb.h"
#include "graphics_test.h"

static void test_graphics_tb() {
    graphics_tb_t *graphics = graphics_tb_init();

    if (graphics == NULL) {
        fprintf(stderr, "Could not initialize termbox");
        return;
    }

    test_graphics(graphics, graphics_tb_draw, graphics_tb_wait_event);

    graphics_tb_quit(graphics);
}


int main(int argc, char **argv) {
    test_graphics_tb();
    return 0;
}
