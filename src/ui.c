#include <stdbool.h>
#include <string.h>

#include "ui.h"
#include "graphics.h"

static void input_append_col(position_t *pos, char ch,
                             possible_positions_t *possible_pos) {
    //ch &= ~32; // Put in lower case
    ch -= 'a';

    if ((ch < 0) || (ch >= 5)) {
        return;
    }

    bool valid = false;
    for (int i = 0; i < 5; i++) {
        if (is_position_possible(possible_pos, (position_t){ch, i })) {
            valid = true;
            continue;
        }
    }

    if (!valid) {
        return;
    }

    pos->c = ch;
}


static void input_append_row(position_t *pos, char ch,
                             possible_positions_t *possible_pos) {
    ch -= '0';

    if ((ch < 0) || (ch >= 5)) {
        return;
    }

    if (!is_position_possible(possible_pos, (position_t){pos->c, ch })) {
        return;
    }

    pos->r = ch;
}


static bool input_append_to_position(position_t *pos, char ch,
                                     possible_positions_t *possible_pos) {
    if (pos->c == POSITION_NOT_SET) {
        input_append_col(pos, ch, possible_pos);
        return true;
    }

    if (pos->r == POSITION_NOT_SET) {
        input_append_row(pos, ch, possible_pos);
        return true;
    }

    return false;
}


static void input_append(mvt_t *input, char ch,
                         possible_positions_t *possible_pos) {
    if (input_append_to_position(&input->from, ch, possible_pos)) {
        return;
    }
    input_append_to_position(&input->to, ch, possible_pos);
}


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


void ui_main(void                           *graphics_context,
             graphics_draw_callback_t       draw_game,
             graphics_wait_event_callback_t wait_event) {
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
        draw_game(graphics_context, &state);

        wait_event(graphics_context, &event);
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
                input_append(&state.input, event.ch,
                             &state.possible_positions);
                break;

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
        sprintf(msg, "%s wins", state.game->turn == TIGER_TURN ? "Goat" : "Tiger");
        draw_game(graphics_context, &state);
        wait_event(graphics_context, &event);
    }

    game_free(state.game);
}
