#ifndef __TEST_MATRIIX_H__
#define __TEST_MATRIIX_H__

#include <math.h>

#include "test.h"

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


#define CHECK_MATRIX_EQUAL(expected, got, error, file, line) \
    if (!matrix_equals(expected, got, error)) {              \
        printf("Expected: ");                                \
        print_matrix(expected);                              \
        printf("Got:      ");                                \
        print_matrix(got);                                   \
        test_fail(t);                                        \
    }


#endif
