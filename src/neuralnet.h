#ifndef __NEURALNET_H__
#define __NEURALNET_H__

#include <stdbool.h>

// matrix_t represents a matrix.
// Use make_matrix to create one and free_matrix to distroy it.
typedef struct {
    int    num_cols;
    int    num_rows;
    int    capacity;
    double *values;
} matrix_t;

// make_matrix creates a new matrix with the given capacity and size.
// If the given capacity is not large enough for the give size, the capacity is
// recalculated.
matrix_t *make_matrix(int rows, int cols, int capacity);
void free_matrix(matrix_t *m);

// matrix_set_size sets the matrix size. If the current capacity is not big
// enough it allocates a new buffer.
// It does not initialize any values.
// It does not do anything if the size doesn't change.
// Returns only 0 on success.
int matrix_set_size(matrix_t *m, int rows, int cols);
void matrix_set_all_values(matrix_t *m, double value);
void matrix_initialize_from_values(matrix_t *m, int rows, int cols,
                                   double *values);

double matrix_get_value(matrix_t *m, int r, int c);
void matrix_set_value(matrix_t *m, int r, int c, double value);

// matrix_product computes the matrix product between m1 times m2 and store its
// result in dest.
int matrix_product(matrix_t *m1, matrix_t *m2, matrix_t *dest);

// matrix_add computes the matrix addition between m1 and m2 and store its
// result in dest. dest can be another matrix or m1 or m2.
// The size of dest is changed if necessary.
// Returns 0 on success.
int matrix_add(matrix_t *m1, matrix_t *m2, matrix_t *dest);


#endif
