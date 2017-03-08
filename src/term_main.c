#include <stdio.h>
#include <stdlib.h>
#include "termbox_graphics.h"

int main(int argc, char **argv) {
    termbox_graphics_t *graphics = termbox_graphics_init();

    if (graphics == NULL) {
        fprintf(stderr, "Could not initialize termbox");
        return 1;
    }

    termbox_graphics_quit(graphics);
    return 0;
}
