#include <stdio.h>

#include "ui.h"
#include "graphics.h"
#include "graphics_tb.h"
#include "ai_rand.h"


int main(int argc, char **argv) {
    graphics_tb_t *tg = graphics_tb_init();


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

    ui_main(tg, graphics_tb_callbacks, ai_tiger, ai_goat);

    graphics_tb_quit(tg);
    return 0;
}
