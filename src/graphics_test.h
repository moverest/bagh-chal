#ifndef __GRAPHICS_TEST_H__
#define __GRAPHICS_TEST_H__

#include <stdbool.h>

#include "graphics.h"

#define GRAPHICS_TEST_ERR_MSG_SIZE    256

// test_graphics run a simple test with differents game situation drawn on the
// screen.
// Returns true if test succeeded and error message is set, false otherwhise.
bool test_graphics(void                 *context,
                   graphics_callbacks_t graphics,
                   char                 *err_msg);


#endif
