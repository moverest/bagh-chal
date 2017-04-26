#ifndef __AI_H__
#define __AI_H__

#include "game.h"
#include "models.h"

typedef mvt_t (*ai_get_mvt_callback_t)(void *context, game_t *game);
typedef void * (*ai_new_callback_t)();
typedef void (*ai_free_callback_t)(void *context);

typedef struct {
    ai_new_callback_t     new;
    ai_free_callback_t    free;
    ai_get_mvt_callback_t get_goat_mvt;
    ai_get_mvt_callback_t get_tiger_mvt;
} ai_callbacks_t;

#endif
