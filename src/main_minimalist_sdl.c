#include <stdio.h>

#include "ui_game.h"
#include "graphics.h"
#include "graphics_minimalist_sdl.h"
#include "ai_rand.h"


int main(int argc, char **argv) {
    graphics_minimalist_sdl_t *tg = graphics_minimalist_sdl_init();


    ai_callbacks_t *ai_tiger = NULL;
    ai_callbacks_t *ai_goat  = NULL;

    if (argc > 1) {
        switch (argv[1][0]) {
        case 't':
            ai_tiger = &ai_rand_callbacks;
            break;

        case 'g':
            ai_goat = &ai_rand_callbacks;
        }
    }

    ui_game_main(tg, graphics_minimalist_sdl_callbacks, ai_tiger, ai_goat);

    graphics_minimalist_sdl_quit(tg);
    return 0;
}
