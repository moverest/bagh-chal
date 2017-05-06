#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graphics.h"
#include "graphics_minimalist_sdl.h"
#include "graphics_test.h"

static void test_graphics_tb() {
    graphics_minimalist_sdl_t *graphics      = graphics_minimalist_sdl_init();
    char err_msg[GRAPHICS_TEST_ERR_MSG_SIZE] = "";

    if (graphics == NULL) {
        fprintf(stderr, "Could not initialize SDL module");
        return;
    }

    bool success = test_graphics(graphics, graphics_minimalist_sdl_callbacks, err_msg);
    graphics_minimalist_sdl_quit(graphics);
    if (!success) {
        puts(err_msg);
    }
}


int main(int argc, char **argv) {
    test_graphics_tb();
    return 0;
}
