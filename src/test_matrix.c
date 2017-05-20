#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "test.h"
#include "tools.h"
#include "matrix.h"

static void print_matrix(matrix_t *m) {
    if (m == NULL) {
        puts("null matrix");
        return;
    }

    printf("{r: %d, c: %d, cap: %d, v: ", m->num_rows, m->num_cols, m->capacity);
    int max = m->num_rows * m->num_cols;
    for (int i = 0; i < max; i++) {
        printf("%f ", m->values[i]);
    }
    puts("}\n");
}


static bool matrix_equals(matrix_t *m1, matrix_t *m2, double error) {
    if ((m1 == NULL) || (m2 == NULL)) {
        return false;
    }

    if ((m1->num_cols != m2->num_cols) ||
        (m1->num_rows != m2->num_rows)) {
        return false;
    }

    int max = m1->num_rows * m2->num_cols;
    for (int i = 0; i < max; i++) {
        if (fabs(m1->values[i] - m2->values[i]) > error) {
            return false;
        }
    }

    return true;
}


#define CHECK_MATRIX_SIZE(m, rows, cols, cap, file, line)         \
    if (m == NULL) {                                              \
        printf("%s:%d: Got a NULL matrix.\n", file, line);        \
        test_fail(t);                                             \
    }                                                             \
    if (m->num_rows != rows ||                                    \
        m->num_cols != cols ||                                    \
        (m->capacity != cap && cap != -1)) {                      \
        printf("%s:%d: Matrix sizes don't match.\n", file, line); \
        printf("Expected: {r: %d, c: %d, cap: %d}\n",             \
               rows, cols, cap);                                  \
        printf("Got: {r: %d, c: %d, cap: %d}\n",                  \
               m->num_rows, m->num_cols, m->capacity);            \
        test_fail(t);                                             \
    }

#define CHECK_MATRIX_EQUAL(expected, got, error, file, line) \
    if (!matrix_equals(expected, got, error)) {              \
        printf("Expected: ");                                \
        print_matrix(expected);                              \
        printf("Got:      ");                                \
        print_matrix(got);                                   \
        test_fail(t);                                        \
    }

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


int main(int argc, char **argv) {
    test_function_t tests[] = {
        TEST_FUNCTION(test_matrix_creation),
        TEST_FUNCTION(test_matrix_add),
        TEST_FUNCTION(test_matrix_product),
        TEST_FUNCTION(test_matrix_apply)
    };

    return test_run(tests, ARRAY_LEN(tests));
}
