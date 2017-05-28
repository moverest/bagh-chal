#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    size_t element_size;
    size_t capacity;
    int    num_elements;
    char   *data;
} stack_t;

// new_stack creates a new stack of element with size `element_size` and with a
// capacity of `capacity` elements.
stack_t *new_stack(size_t element_size, size_t capacity);
void free_stack(stack_t *stack);

// stack_change_capacity reallocate a new buffer of the given capacity of
// elements.
// You have to ensure that the capacity is enough to store all the values in
// the stack.
// Returns 0 on success.
int stack_change_capacity(stack_t *stack, size_t capacity);

// stack_push pushes a new element to the stack.
void stack_push(stack_t *stack, void *element);

// stack_pop copy the last element on the stack to dest.
// Returns 0 on success.
int stack_pop(stack_t *stack, void *dest);

// stack_is_empty returns true if the given stack is empty. False otherwhise.
bool stack_is_empty(stack_t *stack);

#endif
