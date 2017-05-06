#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "menu.h"
#include "ui_menu.h"
#include "menu_test.h"
#include "graphics_tb.h"

static void test_menu_tb() {
    graphics_tb_t *graphics = graphics_tb_init();
    char          err_msg[MENU_TEST_ERR_MSG_SIZE] = "";

    if (graphics == NULL) {
        fprintf(stderr, "Could not initialize termbox");
        return;
    }

    bool success = test_menu(graphics, graphics_tb_callbacks, err_msg);
    graphics_tb_quit(graphics);

    if (!success) {
        puts(err_msg);
    }
}


int main(int argc, char **argv) {
    test_menu_tb();
    return 0;
}
