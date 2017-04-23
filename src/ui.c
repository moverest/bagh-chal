#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "ui.h"
#include "graphics.h"

static void input_append_position(mvt_t                *input,
                                  possible_positions_t *possible_positions,
                                  position_t           position) {
    if (!is_position_possible(possible_positions, position)) {
        return;
    }
    if (input->from.r == POSITION_NOT_SET) {
        input->from = position;
        return;
    }

    if (input->to.r == POSITION_NOT_SET) {
        input->to = position;
        return;
    }
}


static void input_append_from_tag(mvt_t                *input,
                                  possible_positions_t *possible_positions,
                                  char                 tag) {
    position_t pos = position_from_tag(tag);

    if (!position_is_valid(pos)) {
        return;
    }

    input_append_position(input, possible_positions, pos);
}


static void input_backspace(mvt_t *input) {
    int *coordinates_p[] = {
        &input->from.r,
        &input->to.c,
        &input->to.r
    };

    int i;

    for (i = 0;
         i < (sizeof(coordinates_p) / sizeof(coordinates_p[0])) &&
         *coordinates_p[i] != POSITION_NOT_SET;
         i++) {
    }

    *coordinates_p[i - 1] = POSITION_NOT_SET;
}


static bool input_is_complete(game_state_to_draw_t *state) {
    if ((state->game->turn == GOAT_TURN) && (state->game->num_goats_to_put > 0)) {
        return state->input.from.r != POSITION_NOT_SET;
    }

    return state->input.to.r != POSITION_NOT_SET;
}


static void input_reset(mvt_t *input) {
    input->from.c = POSITION_NOT_SET;
    input->from.r = POSITION_NOT_SET;
    input->to.c   = POSITION_NOT_SET;
    input->to.r   = POSITION_NOT_SET;
}


static void update_possible_positions(game_state_to_draw_t *state) {
    if (state->input.from.r == POSITION_NOT_SET) {
        game_get_possible_from_positions(state->game,
                                         &state->possible_positions);
    } else {
        game_get_possible_to_positions(state->game, state->input.from,
                                       &state->possible_positions);
    }
}


void ui_main(void                 *graphics_context,
             graphics_callbacks_t graphics) {
    char                 msg[256] = "";
    game_state_to_draw_t state    = {
        .game = game_new(),
        .msg  = msg
    };

    event_t event;

    input_reset(&state.input);

    bool stop = false;

    while (!game_is_done(state.game) && !stop) {
        update_possible_positions(&state);
        graphics.draw_game(graphics_context, &state);

        graphics.wait_event(graphics_context, &event);
        switch (event.type) {
        case EVENT_QUIT:
            stop = true;
            break;

        case EVENT_REDRAW:
            break;

        case EVENT_POSITION:
            input_append_position(&state.input,
                                  &state.possible_positions,
                                  event.position);
            break;

        case EVENT_KEY:
            switch (event.key) {
            case KEY_CH:
                if (event.ch == 'Q') {
                    stop = true;
                } else {
                    input_append_from_tag(&state.input,
                                          &state.possible_positions,
                                          event.ch);
                    break;
                }

            case KEY_BACKSPACE:
                input_backspace(&state.input);
                break;

            case KEY_ESC:
                stop = true;
                break;

            default:
                // To remove gcc warning telling that other cases are not
                // being handled.
                break;
            }
            break;
        }

        if (input_is_complete(&state)) {
            if (!game_do_mvt(state.game, state.input)) {
                strcpy(msg, "Invalid movement");
            }
            input_reset(&state.input);
        }
    }

    if (!stop) {
        reset_possible_positions(&state.possible_positions);
        sprintf(msg, "%s wins", state.game->turn == TIGER_TURN ? "Goat" : "Tiger");
        graphics.draw_game(graphics_context, &state);
        graphics.wait_event(graphics_context, &event);
    }

    game_free(state.game);
}
