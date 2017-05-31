#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graphics_minimalist_sdl.h"
#include "tools.h"
#include "menu.h"

#define SCALE                      1

#define WIN_TITLE                  "Bagh Chal"
#define WIN_HEIGHT                 800 * SCALE
#define WIN_WIDTH                  700 * SCALE
#define BOARD_X                    50 * SCALE
#define BOARD_Y                    50 * SCALE
#define POSITION_SPACING_WIDTH     150 * SCALE
#define POSITION_SPACING_HEIGHT    150 * SCALE
#define POSITION_WIDTH             50 * SCALE
#define POSITION_HEIGHT            50 * SCALE
#define PROMPT_X                   100 * SCALE
#define PROMPT_Y                   700 * SCALE
#define TURN_X                     40 * SCALE
#define TURN_Y                     700 * SCALE
#define GOAT_INFO_ICON_X           460 * SCALE
#define GOAT_INFO_ICON_Y           715 * SCALE
#define GOAT_INFO_ICON_WIDTH       50 * SCALE
#define GOAT_INFO_ICON_HEIGHT      50 * SCALE
#define NUM_GOATS_TO_PUT_X         520 * SCALE
#define NUM_GOATS_TO_PUT_Y         700 * SCALE
#define NUM_EATEN_GOATS_X          520 * SCALE
#define NUM_EATEN_GOATS_Y          740 * SCALE
#define MSG_X                      40 * SCALE
#define MSG_Y                      740 * SCALE
#define MENU_TITLE_Y               100 * SCALE
#define MENU_ITEM_ROW              200 * SCALE
#define MENU_LINE_ITEMS_SPACING    50 * SCALE
#define MENU_CURSOR_COL            200 * SCALE

#define BG_COLOR_R                 0
#define BG_COLOR_G                 0
#define BG_COLOR_B                 0

#define BOARD_COLOR_R              255
#define BOARD_COLOR_G              255
#define BOARD_COLOR_B              255

#define TIGER_COLOR_R              255
#define TIGER_COLOR_G              0
#define TIGER_COLOR_B              0

#define GOAT_COLOR_R               0
#define GOAT_COLOR_G               255
#define GOAT_COLOR_B               0

#define TAG_COLOR_R                255
#define TAG_COLOR_G                127
#define TAG_COLOR_B                0

#define SELECT_COLOR_R             255
#define SELECT_COLOR_G             0
#define SELECT_COLOR_B             255

#define TXT_COLOR_R                255
#define TXT_COLOR_G                255
#define TXT_COLOR_B                255


#define FONT_PATH                  "./res/FiraSans-Regular.ttf"
#define FONT_SIZE                  30 * SCALE


static int init_tags(graphics_minimalist_sdl_t *sg) {
    sg->tags_textures = malloc(sizeof(SDL_Texture *) * 25);
    for (int i = 0; i < 25; i++) {
        position_t  pos          = { i % 5, i / 5 };
        char        tag_string[] = { position_get_tag(pos), 0 };
        SDL_Surface *tag_surface = TTF_RenderText_Blended(sg->font,
                                                          tag_string,
                                                          (SDL_Color){TAG_COLOR_R,
                                                                      TAG_COLOR_G,
                                                                      TAG_COLOR_B,
                                                                      255 });
        if (tag_surface == NULL) {
            sg->tags_textures[i] = NULL;
            return 1;
        }
        sg->tags_textures[i] = SDL_CreateTextureFromSurface(sg->renderer,
                                                            tag_surface);
        SDL_FreeSurface(tag_surface);
        if (sg->tags_textures[i] == NULL) {
            return -1;
        }
    }

    return 0;
}


static SDL_Texture *get_tag_texture_from_pos(graphics_minimalist_sdl_t *sg,
                                             position_t                pos) {
    return sg->tags_textures[pos.c + pos.r * 5];
}


static void free_tags(graphics_minimalist_sdl_t *sg) {
    if (sg->tags_textures == NULL) {
        return;
    }

    for (int i = 0; i < 25 && sg->tags_textures[i] != NULL; i++) {
        SDL_DestroyTexture(sg->tags_textures[i]);
    }

    free(sg->tags_textures);
}


static SDL_Texture *str_to_texture(graphics_minimalist_sdl_t *sg,
                                   char                      *s) {
    SDL_Surface *surface = TTF_RenderText_Blended(sg->font,
                                                  s,
                                                  (SDL_Color){TXT_COLOR_R,
                                                              TXT_COLOR_G,
                                                              TXT_COLOR_B,
                                                              255 });

    if (surface == NULL) {
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(sg->renderer,
                                                        surface);

    SDL_FreeSurface(surface);
    return texture;
}


// create_text_textures creates the needed textures for the prompt.
// Returns true if an error happend.
static bool create_text_textures(graphics_minimalist_sdl_t *sg) {
    struct {
        char        *text;
        SDL_Texture **texture;
    }
    to_create[] = {
        { " to ",  &sg->prompt_to_texture   },
        { "Put ",  &sg->prompt_put_texture  },
        { "Move ", &sg->prompt_move_texture }
    };

    for (int i = 0; i < ARRAY_LEN(to_create); i++) {
        *to_create[i].texture = str_to_texture(sg, to_create[i].text);
        if (to_create[i].texture == NULL) {
            return true;
        }
    }

    return false;
}


graphics_minimalist_sdl_t *graphics_minimalist_sdl_init() {
    graphics_minimalist_sdl_t *sg = malloc(sizeof(graphics_minimalist_sdl_t));

    if (sg == NULL) {
        return NULL;
    }

    sg->win                     = NULL;
    sg->renderer                = NULL;
    sg->tags_textures           = NULL;
    sg->font                    = NULL;
    sg->prompt_to_texture       = NULL;
    sg->prompt_put_texture      = NULL;
    sg->prompt_move_texture     = NULL;
    sg->num_goat_to_put_texture = NULL;
    sg->num_eaten_goats_texture = NULL;


    int err = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    if (err) {
        fprintf(stderr, "Error while intializing SDL: %s\n", SDL_GetError());
        graphics_minimalist_sdl_quit(sg);
        return NULL;
    }

    err = TTF_Init();
    if (err) {
        fprintf(stderr, "Error while intializing SDL_ttf: %s\n", TTF_GetError());
        graphics_minimalist_sdl_quit(sg);
        return NULL;
    }

    sg->font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (sg->font == NULL) {
        fprintf(stderr, "Error while loading font: %s\n", TTF_GetError());
        graphics_minimalist_sdl_quit(sg);
        return NULL;
    }

    sg->win = SDL_CreateWindow(WIN_TITLE,
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               WIN_WIDTH, WIN_HEIGHT,
                               SDL_WINDOW_SHOWN);
    if (sg->win == NULL) {
        fprintf(stderr, "Error while creating window: %s\n", SDL_GetError());
        graphics_minimalist_sdl_quit(sg);
        return NULL;
    }


    sg->renderer = SDL_CreateRenderer(sg->win, -1, SDL_RENDERER_ACCELERATED);
    if (sg->renderer == NULL) {
        fprintf(stderr, "Error while creating renderer: %s\n", SDL_GetError());
        graphics_minimalist_sdl_quit(sg);
        return NULL;
    }

    err = init_tags(sg);
    if (err) {
        fprintf(stderr, "Error while initializing tags\n");
        graphics_minimalist_sdl_quit(sg);
        return NULL;
    }

    err = create_text_textures(sg);
    if (err) {
        fprintf(stderr, "Error while creating text textures.\n");
        graphics_minimalist_sdl_quit(sg);
        return NULL;
    }

    sg->last_num_goat_to_put = -1;
    sg->last_num_eaten_goats = -1;

    SDL_StartTextInput();

    return sg;
}


static void draw_empty_board(graphics_minimalist_sdl_t *sg) {
    SDL_SetRenderDrawColor(sg->renderer,
                           BOARD_COLOR_R,
                           BOARD_COLOR_G,
                           BOARD_COLOR_B,
                           255);
    int x1 = BOARD_X;
    int y1 = BOARD_Y;
    int x2 = BOARD_X + POSITION_SPACING_WIDTH * 4;
    int y2 = BOARD_Y;

    for (int i = 0; i < 5; i++) {
        SDL_RenderDrawLine(sg->renderer, x1, y1, x2, y2);
        y1 += POSITION_SPACING_HEIGHT;
        y2 += POSITION_SPACING_HEIGHT;
    }


    x1 = BOARD_X;
    y1 = BOARD_Y;
    x2 = BOARD_X;
    y2 = BOARD_Y + POSITION_SPACING_HEIGHT * 4;

    for (int i = 0; i < 5; i++) {
        SDL_RenderDrawLine(sg->renderer, x1, y1, x2, y2);
        x1 += POSITION_SPACING_WIDTH;
        x2 += POSITION_SPACING_WIDTH;
    }

    SDL_RenderDrawLine(sg->renderer,
                       BOARD_X,
                       BOARD_Y,
                       BOARD_X + POSITION_SPACING_WIDTH * 4,
                       BOARD_Y + POSITION_SPACING_HEIGHT * 4);



    SDL_RenderDrawLine(sg->renderer,
                       BOARD_X,
                       BOARD_Y + POSITION_SPACING_HEIGHT * 4,
                       BOARD_X + POSITION_SPACING_WIDTH * 4,
                       BOARD_Y);

    SDL_Point middle_square[] = {
        { BOARD_X + 2 * POSITION_SPACING_WIDTH,
          BOARD_Y },
        { BOARD_X + 4 * POSITION_SPACING_WIDTH,
          BOARD_Y + 2 * POSITION_SPACING_HEIGHT },

        { BOARD_X + 2 * POSITION_SPACING_WIDTH,
          BOARD_Y + 4 * POSITION_SPACING_HEIGHT },

        { BOARD_X,
          BOARD_Y + 2 * POSITION_SPACING_HEIGHT },

        { BOARD_X + 2 * POSITION_SPACING_WIDTH,
          BOARD_Y }
    };
    SDL_RenderDrawLines(sg->renderer, middle_square, ARRAY_LEN(middle_square));
}


#define DRAWING_POINT(x1, y1) \
    { x + ((float)x1) / 10. * w, y + ((float)y1) / 10. * h }

static void draw_goat_icon(graphics_minimalist_sdl_t *sg,
                           int x, int y, int w, int h) {
    SDL_Point head[] = {
        DRAWING_POINT(2, 3),
        DRAWING_POINT(8, 3),
        DRAWING_POINT(5, 9),
        DRAWING_POINT(2, 3)
    };

    SDL_Point left_horn[] = {
        DRAWING_POINT(4, 3),
        DRAWING_POINT(2, 1),
        DRAWING_POINT(0, 3),
        DRAWING_POINT(2, 2),
        DRAWING_POINT(3, 3)
    };

    SDL_Point right_horn[] = {
        DRAWING_POINT(6,  3),
        DRAWING_POINT(8,  1),
        DRAWING_POINT(10, 3),
        DRAWING_POINT(8,  2),
        DRAWING_POINT(7, 3)
    };

    SDL_RenderDrawLines(sg->renderer, head, ARRAY_LEN(head));
    SDL_RenderDrawLines(sg->renderer, left_horn, ARRAY_LEN(left_horn));
    SDL_RenderDrawLines(sg->renderer, right_horn, ARRAY_LEN(right_horn));
}


static void draw_tiger_icon(graphics_minimalist_sdl_t *sg,
                            int x, int y, int w, int h) {
    SDL_Point head[] = {
        DRAWING_POINT(2, 4),
        DRAWING_POINT(3, 3),
        DRAWING_POINT(7, 3),
        DRAWING_POINT(8, 4),
        DRAWING_POINT(8, 6),
        DRAWING_POINT(6, 8),
        DRAWING_POINT(4, 8),
        DRAWING_POINT(2, 6),
        DRAWING_POINT(2, 4)
    };

    SDL_Point mouth[] = {
        DRAWING_POINT(4, 7),
        DRAWING_POINT(6, 7)
    };


    SDL_RenderDrawLines(sg->renderer, head, ARRAY_LEN(head));
    SDL_RenderDrawLines(sg->renderer, mouth, ARRAY_LEN(mouth));
}


static void draw_select_icon(graphics_minimalist_sdl_t *sg,
                             int x, int y, int w, int h) {
    SDL_Point points[] = {
        DRAWING_POINT(3, 3),
        DRAWING_POINT(3, 7),
        DRAWING_POINT(7, 7),
        DRAWING_POINT(7, 3),
        DRAWING_POINT(3, 3)
    };

    SDL_RenderDrawLines(sg->renderer, points, ARRAY_LEN(points));
}


static void draw_blank_spot(graphics_minimalist_sdl_t *sg,
                            int x, int y, int w, int h) {
    SDL_Rect rect = {
        x, y, w, h
    };

    SDL_RenderFillRect(sg->renderer, &rect);
}


static void draw_board(graphics_minimalist_sdl_t *sg, game_state_to_draw_t *state) {
    draw_empty_board(sg);
    position_t pos;

    for (pos.c = 0; pos.c < 5; pos.c++) {
        for (pos.r = 0; pos.r < 5; pos.r++) {
            cell_state_t cell     = board_get_cell(&state->game->board, pos);
            bool         possible = is_position_possible(&state->possible_positions, pos);


            int cell_x      = BOARD_X + pos.c * POSITION_SPACING_WIDTH - POSITION_WIDTH / 2;
            int cell_y      = BOARD_Y + pos.r * POSITION_SPACING_HEIGHT - POSITION_WIDTH / 2;
            int cell_width  = POSITION_WIDTH;
            int cell_height = POSITION_HEIGHT;

            bool position_selected = state->input.from.c == pos.c &&
                                     state->input.from.r == pos.r;

            if ((cell != EMPTY_CELL) || possible || position_selected) {
                SDL_SetRenderDrawColor(sg->renderer,
                                       BG_COLOR_R,
                                       BG_COLOR_G,
                                       BG_COLOR_B,
                                       255);


                draw_blank_spot(sg, cell_x, cell_y, cell_width, cell_height);
            }
            if (position_selected) {
                SDL_SetRenderDrawColor(sg->renderer,
                                       SELECT_COLOR_R,
                                       SELECT_COLOR_G,
                                       SELECT_COLOR_B,
                                       255);
                draw_select_icon(sg, cell_x, cell_y,
                                 cell_width, cell_height);
            } else if (cell != EMPTY_CELL) {
                switch (cell) {
                case TIGER_CELL:
                    SDL_SetRenderDrawColor(sg->renderer,
                                           TIGER_COLOR_R,
                                           TIGER_COLOR_G,
                                           TIGER_COLOR_B,
                                           255);
                    draw_tiger_icon(sg, cell_x, cell_y, cell_width, cell_height);
                    break;

                case GOAT_CELL:
                    SDL_SetRenderDrawColor(sg->renderer,
                                           GOAT_COLOR_R,
                                           GOAT_COLOR_G,
                                           GOAT_COLOR_B,
                                           255);
                    draw_goat_icon(sg, cell_x, cell_y, cell_width, cell_height);
                    break;

                default:
                    break;
                }
            }

            if (possible) {
                SDL_Rect    tag_texture_rect;
                SDL_Texture *tag_texture = get_tag_texture_from_pos(sg, pos);
                SDL_QueryTexture(tag_texture, NULL, NULL, &tag_texture_rect.w, &tag_texture_rect.h);
                tag_texture_rect.w /= 1.5;
                tag_texture_rect.h /= 1.5;
                tag_texture_rect.x  = BOARD_X + pos.c * POSITION_SPACING_WIDTH - tag_texture_rect.w / 2;
                tag_texture_rect.y  = BOARD_Y + pos.r * POSITION_SPACING_HEIGHT - tag_texture_rect.h / 2;
                SDL_RenderCopy(sg->renderer, tag_texture, NULL, &tag_texture_rect);
            }
        }
    }
}


static int draw_input_draw_texture(graphics_minimalist_sdl_t *sg,
                                   SDL_Texture               *texture,
                                   int                       x) {
    SDL_Rect rect = {
        .x = x,
        .y = PROMPT_Y
    };

    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(sg->renderer, texture, NULL, &rect);

    return rect.w;
}


static void draw_input(graphics_minimalist_sdl_t *sg, game_state_to_draw_t *state) {
    int  x             = PROMPT_X;
    bool placing_goats = (state->game->turn == GOAT_TURN) &&
                         (state->game->num_goats_to_put != 0);



    x += draw_input_draw_texture(sg,
                                 placing_goats ? sg->prompt_put_texture : sg->prompt_move_texture,
                                 x);



    if (position_is_set(state->input.from)) {
        x += draw_input_draw_texture(sg,
                                     get_tag_texture_from_pos(sg, state->input.from),
                                     x);

        if (!placing_goats) {
            x += draw_input_draw_texture(sg,
                                         sg->prompt_to_texture,
                                         x);
        }
    }

    if (position_is_set(state->input.to)) {
        x += draw_input_draw_texture(sg,
                                     get_tag_texture_from_pos(sg, state->input.to),
                                     x);
    }


    SDL_SetRenderDrawColor(sg->renderer,
                           TXT_COLOR_R,
                           TXT_COLOR_G,
                           TXT_COLOR_B,
                           255);
    SDL_RenderDrawLine(sg->renderer, x, PROMPT_Y, x, PROMPT_Y + FONT_SIZE);
}


static void update_num_texture(graphics_minimalist_sdl_t *sg,
                               int                       *last,
                               int                       current,
                               char                      *prefix,
                               char                      *suffix,
                               SDL_Texture               **texture) {
    if (*last != current) {
        *last = current;
        if (*texture != NULL) {
            SDL_DestroyTexture(*texture);
        }

        char s[256];
        sprintf(s, "%s%d%s", prefix, current, suffix);
        *texture = str_to_texture(sg, s);
    }
}


static void draw_texture(graphics_minimalist_sdl_t *sg,
                         SDL_Texture *texture, int x, int y) {
    SDL_Rect rect = {
        .x = x,
        .y = y
    };


    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(sg->renderer, texture, NULL, &rect);
}


static void draw_num_goats_to_put(graphics_minimalist_sdl_t *sg,
                                  game_state_to_draw_t      *state) {
    update_num_texture(sg, &sg->last_num_goat_to_put,
                       state->game->num_goats_to_put,
                       " to place: ",
                       "",
                       &sg->num_goat_to_put_texture);
    draw_texture(sg, sg->num_goat_to_put_texture,
                 NUM_GOATS_TO_PUT_X, NUM_GOATS_TO_PUT_Y);
}


static void draw_num_eaten_goats(graphics_minimalist_sdl_t *sg,
                                 game_state_to_draw_t      *state) {
    update_num_texture(sg, &sg->last_num_eaten_goats,
                       state->game->num_eaten_goats,
                       " eaten: ",
                       "",
                       &sg->num_eaten_goats_texture);
    draw_texture(sg, sg->num_eaten_goats_texture,
                 NUM_EATEN_GOATS_X, NUM_EATEN_GOATS_Y);
}


static void draw_turn(graphics_minimalist_sdl_t *sg,
                      game_state_to_draw_t      *state) {
    switch (state->game->turn) {
    case TIGER_TURN:
        SDL_SetRenderDrawColor(sg->renderer,
                               TIGER_COLOR_R,
                               TIGER_COLOR_G,
                               TIGER_COLOR_B,
                               255);
        draw_tiger_icon(sg, TURN_X, TURN_Y, FONT_SIZE, FONT_SIZE);
        break;

    case GOAT_TURN:
        SDL_SetRenderDrawColor(sg->renderer,
                               GOAT_COLOR_R,
                               GOAT_COLOR_G,
                               GOAT_COLOR_B,
                               255);
        draw_goat_icon(sg, TURN_X, TURN_Y, FONT_SIZE, FONT_SIZE);
        break;

    default:
        break;
    }
}


static void draw_message(graphics_minimalist_sdl_t *sg, game_state_to_draw_t *state) {
    if (state->msg[0] == '\0') {
        return;
    }
    SDL_Texture *texture = str_to_texture(sg, state->msg);
    draw_texture(sg, texture, MSG_X, MSG_Y);
    SDL_DestroyTexture(texture);
}


static void draw_goat_info_icon(graphics_minimalist_sdl_t *sg) {
    SDL_SetRenderDrawColor(sg->renderer,
                           GOAT_COLOR_R,
                           GOAT_COLOR_G,
                           GOAT_COLOR_B,
                           255);
    draw_goat_icon(sg, GOAT_INFO_ICON_X, GOAT_INFO_ICON_Y,
                   GOAT_INFO_ICON_WIDTH, GOAT_INFO_ICON_HEIGHT);
}


void graphics_minimalist_sdl_draw_game(void *context, game_state_to_draw_t *state) {
    graphics_minimalist_sdl_t *sg = context;

    SDL_SetRenderDrawColor(sg->renderer,
                           BG_COLOR_R,
                           BG_COLOR_G,
                           BG_COLOR_B,
                           255);

    SDL_RenderClear(sg->renderer);
    draw_board(sg, state);
    draw_input(sg, state);
    draw_turn(sg, state);
    draw_num_goats_to_put(sg, state);
    draw_num_eaten_goats(sg, state);
    draw_goat_info_icon(sg);
    draw_message(sg, state);
    SDL_RenderPresent(sg->renderer);
}


void draw_menu_string(graphics_minimalist_sdl_t *sg, char *str,
                      int y, bool selected) {
    if (str[0] == '\0') {
        return;
    }

    SDL_Texture *texture = str_to_texture(sg, str);

    // Draws the texture horizontally centered
    SDL_Rect texture_rect;
    SDL_QueryTexture(texture, NULL, NULL, &texture_rect.w, &texture_rect.h);
    texture_rect.y = y;
    texture_rect.x = WIN_WIDTH / 2 - texture_rect.w / 2;
    draw_texture(sg, texture, texture_rect.x, texture_rect.y);


    if (selected) {
        // Draws the border around the item.
        texture_rect.x -= FONT_SIZE / 5;
        texture_rect.y -= FONT_SIZE / 5;

        texture_rect.w += FONT_SIZE / 5 * 2;
        texture_rect.h += FONT_SIZE / 5 * 2;

        SDL_SetRenderDrawColor(sg->renderer,
                               TXT_COLOR_R, TXT_COLOR_G, TXT_COLOR_B, 255);

        SDL_RenderDrawRect(sg->renderer, &texture_rect);
    }

    SDL_DestroyTexture(texture);
}

void menu_item_to_texture(void *context, menu_t *menu, char *str, int i) {
    switch (menu->items[i]->type) {
    case MENU_ITEM_SELECT:
        sprintf(str, "%s : %s", menu->items[i]->label, menu->items[i]->choices[menu->items[i]->choice]);

        break;

    case MENU_ITEM_TEXT:
    case MENU_ITEM_BUTTON:
        sprintf(str, "%s", menu->items[i]->label);

        break;

    default:
        sprintf(str, "");
        break;
    }
}

void graphics_draw_menu(void *context, menu_t *menu) {
    graphics_minimalist_sdl_t *sg = context;

    char str[256] = "";

    SDL_SetRenderDrawColor(sg->renderer,
                           BG_COLOR_R,
                           BG_COLOR_G,
                           BG_COLOR_B,
                           255);

    SDL_RenderClear(sg->renderer);

    draw_menu_string(sg, menu->title, MENU_TITLE_Y, false);

    for (int i = 0; i < menu->num_item; i++) {
        bool selected = i == menu->cursor;
        menu_item_to_texture(sg, menu, str, i);
        draw_menu_string(sg, str,
                         MENU_ITEM_ROW + i * MENU_LINE_ITEMS_SPACING,
                         selected);
    }

    SDL_RenderPresent(sg->renderer);
}


static int get_label_clickable_length(void *context, menu_t *menu, int i) {
    graphics_minimalist_sdl_t *sg = context;

    char str[256] = "";
    SDL_Texture *texture;

    menu_item_to_texture(sg, menu, str, i);

    texture = str_to_texture(sg, str);
    int w;
    SDL_QueryTexture(texture, NULL, NULL, &w, NULL);

    SDL_DestroyTexture(texture);

    return w;

}


void graphics_minimalist_sdl_wait_game_event(void *context, event_t *event) {
    graphics_minimalist_sdl_wait_menu_event(context, event, NULL);
}


void graphics_minimalist_sdl_wait_menu_event(void *context, event_t *event, menu_t *menu) {
    graphics_minimalist_sdl_t *sg = context;

    while (1) {
        SDL_Event sdl_event;
        SDL_WaitEvent(&sdl_event);
        switch (sdl_event.type) {
        case SDL_TEXTINPUT:
            // We only want ASCII caracters.
            if ((sdl_event.text.text[0] != '\0') &&
                (sdl_event.text.text[1] == '\0')) {
                event->type = EVENT_KEY;
                event->key  = KEY_CH;
                event->ch   = sdl_event.text.text[0];
                return;
            }

        case SDL_KEYDOWN:
            event->type = EVENT_KEY;
            switch (sdl_event.key.keysym.sym) {
            case SDLK_ESCAPE:
                event->type = EVENT_QUIT;
                event->key  = KEY_ESC;
                return;

            case SDLK_q:
                if (sdl_event.key.keysym.mod & KMOD_SHIFT) {
                    event->type = EVENT_QUIT;
                    return;
                }
                break;

            case SDLK_BACKSPACE:
                event->key = KEY_BACKSPACE;
                return;

            case SDLK_RETURN:
                event->key = KEY_ENTER;
                return;

            case SDLK_LEFT:
                event->key = KEY_ARROW_LEFT;
                return;

            case SDLK_RIGHT:
                event->key = KEY_ARROW_RIGHT;
                return;

            case SDLK_UP:
                event->key = KEY_ARROW_UP;
                return;

            case SDLK_DOWN:
                event->key = KEY_ARROW_DOWN;
                return;
            }
            break;

        case SDL_QUIT:
            event->type = EVENT_QUIT;
            return;

        case SDL_WINDOWEVENT:
            event->type = EVENT_REDRAW;
            return;

        case SDL_MOUSEBUTTONDOWN:
            ;
            if (menu != NULL) {
                int selected_menu_item = -1;
                int label_length = 0;
                if (sdl_event.button.y - MENU_ITEM_ROW >= 0) {
                    selected_menu_item = (sdl_event.button.y - MENU_ITEM_ROW) / MENU_LINE_ITEMS_SPACING;
                }
                if ((selected_menu_item < menu->num_item) &&
                    (selected_menu_item >= 0)) {
                    label_length = get_label_clickable_length(sg, menu, selected_menu_item);
                    if ((sdl_event.button.x >= WIN_WIDTH / 2 - label_length / 2) &&
                       (sdl_event.button.x <= WIN_WIDTH / 2 + label_length / 2)) {
                        event->type      = EVENT_MENU_ITEM_CLICKED;
                        event->menu_item = selected_menu_item;
                        return;
                    }
                }
            } else {
                // Positions relative to the board.
                int x = sdl_event.button.x - (BOARD_X - POSITION_WIDTH / 2);
                int y = sdl_event.button.y - (BOARD_Y - POSITION_HEIGHT / 2);

                if ((x > 0) && ((x < (4 * POSITION_SPACING_WIDTH + POSITION_WIDTH)) &&
                                (y > 0) && (x < 4 * POSITION_SPACING_HEIGHT + POSITION_HEIGHT))) {
                    // We are in the board.

                    position_t pos = {
                        x / POSITION_SPACING_WIDTH,
                        y / POSITION_SPACING_HEIGHT
                    };

                    // Positions relative to the placement.
                    int x_placement = x % 5;
                    int y_placement = y % 5;
                    if ((x_placement < POSITION_SPACING_WIDTH) &&
                        (y_placement < POSITION_SPACING_HEIGHT)) {
                        event->type     = EVENT_POSITION;
                        event->position = pos;
                        return;
                    }
                }
            }
        }
    }
}


void graphics_minimalist_sdl_quit(graphics_minimalist_sdl_t *sg) {
    if (sg != NULL) {
        if (sg->font) {
            TTF_CloseFont(sg->font);
        }

        SDL_Texture *textures_to_destroy[] = {
            sg->prompt_to_texture,
            sg->prompt_put_texture,
            sg->prompt_move_texture,
            sg->num_goat_to_put_texture,
            sg->num_eaten_goats_texture
        };

        for (int i = 0; i < ARRAY_LEN(textures_to_destroy); i++) {
            if (textures_to_destroy[i]) {
                SDL_DestroyTexture(textures_to_destroy[i]);
            }
        }

        free_tags(sg);

        if (sg->renderer) {
            SDL_DestroyRenderer(sg->renderer);
        }

        if (sg->win) {
            SDL_DestroyWindow(sg->win);
        }

        SDL_StopTextInput();
    }

    if (TTF_WasInit()) {
        TTF_Quit();
    }

    if (SDL_WasInit(0)) {
        SDL_Quit();
    }
}


graphics_callbacks_t graphics_minimalist_sdl_callbacks = {
    .draw_game       = graphics_minimalist_sdl_draw_game,
    .wait_event_game = graphics_minimalist_sdl_wait_game_event,
    .wait_event_menu = graphics_minimalist_sdl_wait_menu_event,
    .draw_menu       = graphics_draw_menu
};
