#include <termbox.h>

typedef struct {}   termbox_graphics_t;

termbox_graphics_t *termbox_graphics_init();
void termbox_graphics_draw(termbox_graphics_t *tg);
void termbox_graphics_quit(termbox_graphics_t *tg);
