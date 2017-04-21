#include <stdio.h>

#include "ui.h"
#include "graphics.h"
#include "graphics_tb.h"


int main(int argc, char **argv) {
    graphics_tb_t *tg = graphics_tb_init();

    ui_main(tg, graphics_tb_draw, graphics_tb_wait_event);

    graphics_tb_quit(tg);
    return 0;
}
