#ifndef __AI_RAND_H__
#define __AI_RAND_H__

#include "ai.h"

typedef struct {}   ai_rand_t;

void *ai_rand_new();
void ai_rand_free(void *context);
mvt_t ai_rand_get_mvt(void *context, game_t *game);

extern ai_callbacks_t ai_rand_callbacks;

#endif
