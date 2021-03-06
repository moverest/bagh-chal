#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "test.h"
#include "tools.h"
#include "matrix.h"
#include "test_matrix.h"


static void test_matrix_creation(test_t *t) {
    matrix_t *m = make_matrix(4, 12, 0);

    CHECK_MATRIX_SIZE(m, 4, 12, 4 * 12, __FILE__, __LINE__);

    matrix_set_size(m, 2, 4);
    CHECK_MATRIX_SIZE(m, 2, 4, 4 * 12, __FILE__, __LINE__);

    matrix_set_size(m, 20, 40);
    CHECK_MATRIX_SIZE(m, 20, 40, 20 * 40, __FILE__, __LINE__);

    matrix_set_size(m, 2, 3);
    CHECK_MATRIX_SIZE(m, 2, 3, 20 * 40, __FILE__, __LINE__);

    matrix_set_all_values(m, 2);
    matrix_t *n = make_matrix(2, 3, 0);
    matrix_initialize_from_values(n, 2, 3, (double[]){2., 2., 2., 2., 2., 2. });
    CHECK_MATRIX_EQUAL(n, m, .0001, __FILE__, __LINE__);

    free_matrix(m);
    free_matrix(n);
}


static void test_matrix_add(test_t *t) {
    struct {
        int    rows, cols;
        double *a, *b, *expected;
    }
    tests[] = {
        { 1, 3,
                      (double[]){ 1, 3, 5 },
                      (double[]){ 4, 2, 1 },
                      (double[]){ 5, 5, 6 } }
    };

    matrix_t *a        = make_matrix(0, 0, 0);
    matrix_t *b        = make_matrix(0, 0, 0);
    matrix_t *expected = make_matrix(0, 0, 0);
    matrix_t *got      = make_matrix(0, 0, 0);

    for (int i = 0; i < ARRAY_LEN(tests); i++) {
        matrix_initialize_from_values(a, tests[i].rows,
                                      tests[i].cols, tests[i].a);

        matrix_initialize_from_values(b, tests[i].rows,
                                      tests[i].cols, tests[i].b);

        matrix_initialize_from_values(expected, tests[i].rows,
                                      tests[i].cols, tests[i].expected);

        matrix_add(a, b, got);
        CHECK_MATRIX_EQUAL(expected, got, .00001, __FILE__, __LINE__);
    }

    free_matrix(a);
    free_matrix(b);
    free_matrix(expected);
    free_matrix(got);
}


static void test_matrix_product(test_t *t) {
    struct {
        int    a_rows, a_cols, b_cols;
        double *a, *b, *expected;
    }
    tests[] = {
        {  1,   3,   1,
                                     (double[])              {  1,   2, 3 },
                                     (double[])              {  4,  -1, 3 },
                                     (double[])              {11 } },
        {  2,   3,   2,
                                     (double[])              {  1,   2,  -3,   4, -5, 6 },
                                     (double[])              {  7,   8, -10, -11, 12, 8 },
                                     (double[])              {-49, -38, 150,135 } },
    };

    matrix_t *a        = make_matrix(0, 0, 0);
    matrix_t *b        = make_matrix(0, 0, 0);
    matrix_t *expected = make_matrix(0, 0, 0);
    matrix_t *got      = make_matrix(0, 0, 0);

    for (int i = 0; i < ARRAY_LEN(tests); i++) {
        matrix_initialize_from_values(a, tests[i].a_rows,
                                      tests[i].a_cols, tests[i].a);

        matrix_initialize_from_values(b, tests[i].a_cols,
                                      tests[i].b_cols, tests[i].b);

        matrix_initialize_from_values(expected, tests[i].a_rows,
                                      tests[i].b_cols, tests[i].expected);

        matrix_product(a, b, got);
        CHECK_MATRIX_EQUAL(expected, got, .00001, __FILE__, __LINE__);
    }

    free_matrix(a);
    free_matrix(b);
    free_matrix(expected);
    free_matrix(got);
}


static void test_matrix_apply(test_t *t) {
    struct {
        int    rows, cols;
        double *in, *expected;
    }
    tests[] = {
        { 1, 3, (double[]){1, -1, -5 }, (double[]){1, 1, 5 } }
    };

    matrix_t *in       = make_matrix(0, 0, 0);
    matrix_t *expected = make_matrix(0, 0, 0);
    matrix_t *got      = make_matrix(0, 0, 0);

    for (int i = 0; i < ARRAY_LEN(tests); i++) {
        matrix_initialize_from_values(in, tests[i].rows,
                                      tests[i].cols, tests[i].in);
        matrix_initialize_from_values(expected, tests[i].rows,
                                      tests[i].cols, tests[i].expected);

        matrix_apply(in, got, fabs);
        CHECK_MATRIX_EQUAL(expected, got, .00001, __FILE__, __LINE__);
    }
}


static void test_matrix_copy(test_t *t) {
    struct {
        int    rows, cols;
        double *values;
    }
    tests[] = {
        { 1, 3, (double[]){1, 3, 5 } }
    };

    matrix_t *original = make_matrix(0, 0, 0);
    matrix_t *copy     = make_matrix(0, 0, 0);

    for (int i = 0; i < ARRAY_LEN(tests); i++) {
        matrix_initialize_from_values(original, tests[i].rows,
                                      tests[i].cols, tests[i].values);

        matrix_copy(original, copy);
        CHECK_MATRIX_EQUAL(original, copy, .00001, __FILE__, __LINE__);
    }

    free_matrix(original);
    free_matrix(copy);
}


int main(int argc, char **argv) {
    test_function_t tests[] = {
        TEST_FUNCTION(test_matrix_creation),
        TEST_FUNCTION(test_matrix_add),
        TEST_FUNCTION(test_matrix_product),
        TEST_FUNCTION(test_matrix_apply),
        TEST_FUNCTION(test_matrix_copy)
    };

    return test_run(tests, ARRAY_LEN(tests));
}
