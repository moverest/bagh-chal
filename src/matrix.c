#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

matrix_t *make_matrix(int rows, int cols, int capacity) {
    matrix_t *m = malloc(sizeof(matrix_t));

    if (m == NULL) {
        return NULL;
    }


    int min_capacity = rows * cols;
    if (capacity < min_capacity) {
        capacity = min_capacity;
    }


    m->num_rows = rows;
    m->num_cols = cols;
    m->capacity = capacity;

    m->values = malloc(sizeof(double) * capacity);
    if (m->values == NULL) {
        free(m);
        return NULL;
    }

    return m;
}


void free_matrix(matrix_t *m) {
    if (m == NULL) {
        return;
    }

    free(m->values);
    free(m);
}


int matrix_set_size(matrix_t *m, int rows, int cols) {
    int min_capacity = rows * cols;

    if (m->capacity < min_capacity) {
        free(m->values);
        m->values = malloc(sizeof(double) * min_capacity);
        if (m->values == NULL) {
            return 1;
        }
        m->capacity = min_capacity;
    }

    m->num_rows = rows;
    m->num_cols = cols;

    return 0;
}


void matrix_set_all_values(matrix_t *m, double value) {
    int max = m->num_cols * m->num_rows;

    for (int i = 0; i < max; i++) {
        m->values[i] = value;
    }
}


double matrix_get_value(matrix_t *m, int r, int c) {
    return m->values[r * m->num_cols + c];
}


void matrix_set_value(matrix_t *m, int r, int c, double value) {
    m->values[r * m->num_cols + c] = value;
}


void matrix_initialize_from_values(matrix_t *m, int rows, int cols,
                                   double *values) {
    matrix_set_size(m, rows, cols);
    memcpy(m->values, values, sizeof(double) * rows * cols);
}


int matrix_product(matrix_t *m1, matrix_t *m2, matrix_t *dest) {
    if (m1->num_cols != m2->num_rows) {
        return -1;
    }

    matrix_set_size(dest, m1->num_rows, m2->num_cols);
    for (int i = 0; i < dest->num_rows; i++) {
        for (int j = 0; j < dest->num_cols; j++) {
            double sum = 0;
            for (int k = 0; k < m1->num_cols; k++) {
                sum += matrix_get_value(m1, i, k) * matrix_get_value(m2, k, j);
            }

            matrix_set_value(dest, i, j, sum);
        }
    }

    return 0;
}


int matrix_add(matrix_t *m1, matrix_t *m2, matrix_t *dest) {
    if ((m1->num_cols != m2->num_cols) || (m1->num_rows != m2->num_rows)) {
        return -1;
    }

    matrix_set_size(dest, m1->num_rows, m1->num_cols);
    int max = m1->num_rows * m1->num_cols;
    for (int i = 0; i < max; i++) {
        dest->values[i] = m1->values[i] + m2->values[i];
    }

    return 0;
}


void matrix_apply(matrix_t *m, matrix_t *dest, double (*f)(double)) {
    matrix_set_size(dest, m->num_rows, m->num_cols);
    int max = m->num_rows * m->num_cols;
    for (int i = 0; i < max; i++) {
        dest->values[i] = f(m->values[i]);
    }
}


int matrix_copy(matrix_t *src, matrix_t *dest) {
    int err = matrix_set_size(dest, src->num_rows, src->num_cols);

    if (err != 0) {
        return -1;
    }

    size_t size = sizeof(double) * src->num_rows * src->num_cols;
    memcpy(dest->values, src->values, size);
    return 0;
}
