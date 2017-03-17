#include <termbox.h>
#include <stdlib.h>

#include "graphics_tb.h"

graphics_tb_t *graphics_tb_init() {
    graphics_tb_t *tg = malloc(sizeof(graphics_tb_t));

    if (tg == NULL) {
        return NULL;
    }

    if (tb_init()) {
        return NULL;
    }

    return tg;
}


void graphics_tb_draw(void *context, state_to_draw_t *state) {
    graphics_tb_t *tg = (graphics_tb_t *)context;
}


void graphics_tb_quit(graphics_tb_t *tg) {
    tb_shutdown();
    free(tg);
}
