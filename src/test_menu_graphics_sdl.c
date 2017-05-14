#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graphics_minimalist_sdl.h"
#include "menu.h"
#include "ui_menu.h"
#include "menu_test.h"

static void test_graphics_minimalist_sdl() {
    graphics_minimalist_sdl_t *graphics  = graphics_minimalist_sdl_init();
    char err_msg[MENU_TEST_ERR_MSG_SIZE] = "";

    if (graphics == NULL) {
        fprintf(stderr, "Could not initialize sdl");
        return;
    }

    bool success = test_menu(graphics, graphics_minimalist_sdl_callbacks, err_msg);
    graphics_minimalist_sdl_quit(graphics);
    if (!success) {
        puts(err_msg);
    }
}


int main(int argc, char **argv) {
    test_graphics_minimalist_sdl();
    return 0;
}
