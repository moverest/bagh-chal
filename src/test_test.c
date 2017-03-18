#include "test.h"

static void test2(test_t *t) {
    puts("This test should fail.");
    test_fail(t);
    printf("%s (%d): This should not be displayed.", __FILE__, __LINE__);
}


static void test1(test_t *t) {
    puts("This test should pass.");
}


static void test3(test_t *t) {
    printf("%s (%d): This test should never have been run.", __FILE__, __LINE__);
}


int main(int argc, char **argv) {
    test_function_t tests[] = {
        TEST_FUNCTION(test1),
        TEST_FUNCTION(test2),
        TEST_FUNCTION(test3)
    };

    return test_run(tests, sizeof(tests) / sizeof(tests[0]));
}
