#include <stdio.h>

#include "menu.h"
#include "graphics.h"
#include "graphics_tb.h"
#include "ui_main.h"


int main(int argc, char **argv) {
    graphics_tb_t *tg = graphics_tb_init();

    ui_main(tg, graphics_tb_callbacks);

    graphics_tb_quit(tg);
    return 0;
}
