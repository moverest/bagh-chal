#ifndef __GRAPHICS_TB_H__
#define __GRAPHICS_TB_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "models.h"
#include "graphics.h"

typedef struct {
    SDL_Window   *win;
    SDL_Renderer *renderer;
    SDL_Texture  **tags_textures;
    TTF_Font     *font;
    SDL_Texture  *prompt_put_texture;
    SDL_Texture  *prompt_move_texture;
    SDL_Texture  *prompt_to_texture;
    SDL_Texture  *num_goat_to_put_texture;
    SDL_Texture  *num_eaten_goats_texture;
    int          last_num_goat_to_put;
    int          last_num_eaten_goats;
} graphics_minimalist_sdl_t;

// Implement graphics. See graphics.h for details.
graphics_minimalist_sdl_t *graphics_minimalist_sdl_init();
void graphics_minimalist_sdl_draw_game(void *context, game_state_to_draw_t *state);
void graphics_minimalist_sdl_wait_game_event(void *context, event_t *event);
void graphics_minimalist_sdl_wait_menu_event(void *context, event_t *event, menu_t *menu);
void graphics_minimalist_sdl_quit(graphics_minimalist_sdl_t *tg);

extern graphics_callbacks_t graphics_minimalist_sdl_callbacks;

#endif
