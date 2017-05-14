#include <stdio.h>

#include "ui_menu.h"
#include "menu.h"
#include "graphics.h"
#include "graphics_minimalist_sdl.h"
#include "ai_rand.h"
#include "ui_main.h"


int main(int argc, char **argv) {
    graphics_minimalist_sdl_t *sg = graphics_minimalist_sdl_init();

    ui_main(sg, graphics_minimalist_sdl_callbacks);

    graphics_minimalist_sdl_quit(sg);
    return 0;
}
