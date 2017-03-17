#include <stdio.h>
#include <stdlib.h>
#include "graphics_tb.h"

int main(int argc, char **argv) {
    graphics_tb_t *graphics = graphics_tb_init();

    if (graphics == NULL) {
        fprintf(stderr, "Could not initialize termbox");
        return 1;
    }

    graphics_tb_quit(graphics);
    return 0;
}
