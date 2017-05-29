#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define GROW_FACTOR    2

stack_t *new_stack(size_t element_size, size_t capacity) {
    stack_t *stack = malloc(sizeof(stack_t));

    if (stack == NULL) {
        return NULL;
    }

    stack->element_size = element_size;
    stack->capacity     = capacity;
    stack->num_elements = 0;

    stack->data = malloc(element_size * capacity);
    if (stack->data == NULL) {
        free(stack);
        return NULL;
    }

    return stack;
}


void free_stack(stack_t *stack) {
    free(stack->data);
    free(stack);
}


int stack_change_capacity(stack_t *stack, size_t capacity) {
    char *buffer = malloc(stack->element_size * capacity);

    if (buffer == NULL) {
        return 1;
    }

    memcpy(buffer, stack->data, stack->num_elements * stack->element_size);
    free(stack->data);
    stack->data     = buffer;
    stack->capacity = capacity;

    return 0;
}


void stack_push(stack_t *stack, void *element) {
    if (stack->capacity < stack->num_elements + 1) {
        stack_change_capacity(stack, stack->capacity * GROW_FACTOR);
    }

    memcpy(stack->data + stack->num_elements * stack->element_size,
           element, stack->element_size);

    stack->num_elements++;
}


int stack_pop(stack_t *stack, void *dest) {
    if (stack->num_elements <= 0) {
        return 1;
    }

    stack->num_elements--;
    memcpy(dest, stack->data + stack->num_elements * stack->element_size,
           stack->element_size);
    return 0;
}


bool stack_is_empty(stack_t *stack) {
    return stack->num_elements <= 0;
}


void stack_reset(stack_t *stack) {
    stack->num_elements = 0;
}
