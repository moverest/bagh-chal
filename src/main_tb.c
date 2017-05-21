#include <stdio.h>

#include "menu.h"
#include "graphics.h"
#include "graphics_tb.h"
#include "ui_main.h"


int main(int argc, char **argv) {
    graphics_tb_t *tg = graphics_tb_init();

    if (tg == NULL) {
        fprintf(stderr, "An error happend while loading the graphics module.\n");
        return -1;
    }

    ui_main(tg, graphics_tb_callbacks);

    graphics_tb_quit(tg);
    return 0;
}
