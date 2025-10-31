#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
   int num_rows;
   int num_cols;
   double** data;
} Matrix;

typedef Matrix Vector;

Matrix* matrix_create(int num_rows, int num_cols);

void matrix_free(Matrix* m);

void matrix_randomize(Matrix* m, int seed);

void matrix_print(Matrix* m);

Matrix* matrix_copy(const Matrix* m);

Matrix* matrix_transpose(const Matrix* m);

Matrix* matrix_multiply(const Matrix* a, const Matrix* b);

Matrix* matrix_add(const Matrix* a, const Matrix* b);

Matrix* matrix_subtract(const Matrix* a, const Matrix* b);

Matrix* matrix_elementwise_multiply(const Matrix* a, const Matrix* b);

Matrix* matrix_scale(const Matrix* m, double scalar);

Matrix* matrix_add_scalar(const Matrix* m, double scalar);

Matrix* matrix_apply_function(const Matrix* m, double (*func)(double));

#endif
