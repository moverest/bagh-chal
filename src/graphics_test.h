#ifndef __GRAPHICS_TEST_H__
#define __GRAPHICS_TEST_H__

#include "graphics.h"

// test_graphics run a simple test with differents game situation drawn on the
// screen.
void test_graphics(void                           *context,
                   graphics_draw_callback_t       draw,
                   graphics_wait_event_callback_t wait_event);


#endif
