#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graphics.h"
#include "graphics_tb.h"
#include "graphics_test.h"

static void test_graphics_tb() {
    graphics_tb_t *graphics = graphics_tb_init();
    char          err_msg[GRAPHICS_TEST_ERR_MSG_SIZE] = "";

    if (graphics == NULL) {
        fprintf(stderr, "Could not initialize termbox");
        return;
    }

    bool success = test_graphics(graphics, graphics_tb_draw, graphics_tb_wait_event, err_msg);
    graphics_tb_quit(graphics);
    if (!success) {
        puts(err_msg);
    }
}


int main(int argc, char **argv) {
    test_graphics_tb();
    return 0;
}
