#include "test.h"
#include "tools.h"
#include "stack.h"

void test_stack_creation(test_t *t) {
    stack_t *stack = new_stack(sizeof(int), 10);

    if (!stack_is_empty(stack)) {
        printf("%s:%d: Stack should be empty\n", __FILE__, __LINE__);
        test_fail(t);
    }

    int value = 5;
    stack_push(stack, &value);
    if (stack_is_empty(stack)) {
        printf("%s:%d: Stack should not be empty\n", __FILE__, __LINE__);
        test_fail(t);
    }

    value = -1;
    stack_pop(stack, &value);
    if (value != 5) {
        printf("%s:%d: Value should be 5 not %d\n", __FILE__, __LINE__, value);
        test_fail(t);
    }

    free_stack(stack);
}


void test_stack_insert(test_t *t) {
    stack_t *stack     = new_stack(sizeof(int), 1);
    int     num_values = 4096;

    for (int i = 0; i < num_values; i++) {
        stack_push(stack, &i);
    }

    for (int i = num_values - 1; i >= 0; i--) {
        int value;
        stack_pop(stack, &value);
        if (value != i) {
            printf("%s:%d: Value should be %d, not %d\n", __FILE__, __LINE__,
                   i, value);
            test_fail(t);
        }
    }

    free_stack(stack);
}


int main(int argc, char **argv) {
    test_function_t tests[] = {
        TEST_FUNCTION(test_stack_creation),
        TEST_FUNCTION(test_stack_insert),
    };

    return test_run(tests, ARRAY_LEN(tests));
}
