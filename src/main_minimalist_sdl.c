#include <stdio.h>

#include "graphics.h"
#include "graphics_minimalist_sdl.h"
#include "ui_main.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <font file>\n", argv[0]);
        return 1;
    }

    char *font_filename = argv[1];

    graphics_minimalist_sdl_t *sg = graphics_minimalist_sdl_init(font_filename);

    if (sg == NULL) {
        fprintf(stderr, "An error happend while loading the graphics module.\n");
        return -1;
    }

    ui_main(sg, graphics_minimalist_sdl_callbacks);

    graphics_minimalist_sdl_quit(sg);
    return 0;
}
