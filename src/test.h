// test is a really simple test module.
// See `test_test.c` for an example on how to use this module.

#ifndef __TEST_H__
#define __TEST_H__

#include <stdlib.h>
#include <stdio.h>

// test_t represents a test suite state.
typedef struct {
    int failed;
    int num_successes;
} test_t;

// test_function_t is structure that stores a pointer to a test function while
// retaining its name to be displayed.
#define TEST_FUNCTION_NAME_MAX_SIZE    256
typedef struct {
    void (*f)(test_t * t);
    char name[TEST_FUNCTION_NAME_MAX_SIZE];
} test_function_t;

// test_run runs all the given test. It stops at the first fail.
int test_run(test_function_t test_functions[], size_t num_tests);

// test_fail is a macro that mark the current test as failed.
// It should not be used in a nested function call.
#define test_fail(test)     test->failed = 1; return;

// TEST_FUNCTION defines the function structure.
#define TEST_FUNCTION(f)    { f, # f }

#endif
