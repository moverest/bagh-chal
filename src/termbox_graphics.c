#include <termbox.h>
#include <stdlib.h>
#include "termbox_graphics.h"

termbox_graphics_t* termbox_graphics_init() {
        termbox_graphics_t *tg = malloc(sizeof(termbox_graphics_t));
        if (tg == NULL) {
                return NULL;
        }

        if (tb_init()) {
                return NULL;
        }

        return tg;
}

void termbox_graphics_draw(termbox_graphics_t *tg) {

}

void termbox_graphics_quit(termbox_graphics_t *tg) {
        tb_shutdown();
        free(tg);
}
