#include "test.h"

#include <stdio.h>

static void test_run_individual(test_t *t, test_function_t test_function) {
    test_function.f(t);

    if (!t->failed) {
        printf("\x1b[1;32m[PASSED]\x1b[0m %s\n", test_function.name);
        t->num_successes++;
    } else {
        printf("\x1b[1;31m[FAILED]\x1b[0m %s\n", test_function.name);
    }
}


int test_run(test_function_t test_functions[], size_t num_tests) {
    test_t test;

    test.failed        = 0;
    test.num_successes = 0;

    for (int i = 0; !test.failed && i < num_tests; i++) {
        test_run_individual(&test, test_functions[i]);
    }

    printf("\n\x1b[32m%d succeeded.\x1b[0m\n", test.num_successes);

    if (test.failed) {
        puts("\x1b[31m1 failed.\x1b[0m");
        printf("\x1b[33m%d left to be tested.\x1b[0m\n",
               (int)num_tests - test.num_successes - 1);
    }

    return test.failed;
}
