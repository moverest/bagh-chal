#ifndef __UI_H__
#define __UI_H__

#include "models.h"
#include "graphics.h"

// ui_main is the main function that manage the ui.
// It takes two arguments that are the callback to be used to interact with the
// user.
void ui_main(graphic_draw_callback_t       draw,
             graphic_wait_event_callback_t wait_event);

#endif
