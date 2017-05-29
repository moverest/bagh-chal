#ifndef __AI_SIMPLE_HEURISTIC_AI_H__
#define __AI_SIMPLE_HEURISTIC_AI_H__

#include "ai.h"

typedef struct {} ai_simple_heuristic_t;

void *ai_simple_heuristic_new();
void ai_simple_heuristic_free(void *context);
mvt_t ai_simple_heuristic_get_mvt(void *context, game_t *game);

extern ai_callbacks_t ai_simple_heuristic_callbacks;

#endif
