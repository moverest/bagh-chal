#ifndef __GRAPHICS_TB_H__
#define __GRAPHICS_TB_H__

#include <termbox.h>

#include "models.h"
#include "graphics.h"

typedef struct {}   graphics_tb_t;

// graphics_tb initializes the graphic module.
graphics_tb_t *graphics_tb_init();

// graphics_tb_draw draws the given state on the screen.
//
// We do not use `graphics_tb_t *tg` but `void *context` because this function
// is used as a callback and the caller shouldn't have to know about
// graphics_tb. This way, this module is pluggable.
void graphics_tb_draw(void *context, state_to_draw_t *state);

// graphics_tb_quit terminates the graphic module.
void graphics_tb_quit(graphics_tb_t *tg);


#endif
