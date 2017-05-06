#ifndef __MENU_TEST_H__
#define __MENU_TEST_H__

#include <stdbool.h>

#include "menu.h"

#define MENU_TEST_ERR_MSG_SIZE    256

// test_graphics run a simple test with differents game situation drawn on the
// screen.
// Returns true if test succeeded and error message is set, false otherwhise.
bool test_menu(void *context, graphics_callbacks_t graphics_tb_callbacks, char *err_msg);


#endif
