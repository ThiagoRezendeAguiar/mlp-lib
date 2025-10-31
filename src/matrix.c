#include "matrix.h"

typedef double (*op_func)(double, double);

static double op_add(double a, double b) {
    return a + b;
}

static double op_subtract(double a, double b) {
    return a - b;
}

static double op_multiply(double a, double b) {
    return a * b;
}

static Matrix* apply_op(const Matrix* a, const Matrix* b, op_func op) {
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL 
        || a->num_rows != b->num_rows || a->num_cols != b->num_cols) {
        return NULL;
    }

    Matrix* c = matrix_create(a->num_rows, a->num_cols);

    if (c == NULL) return NULL;

    for (int i = 0; i < c->num_rows; i++) {
        for (int j = 0; j < c->num_cols; j++) {
            c->data[i][j] = op(a->data[i][j], b->data[i][j]);
        }
    }
    
    return c;
}

static Matrix* apply_scalar_op(const Matrix* m, double scalar, op_func op) {
    if (m == NULL ||m->data == NULL) return NULL;

    Matrix* c = matrix_create(m->num_rows, m->num_cols);

    if (c == NULL) return NULL;

    for (int i = 0; i < c->num_rows; i++) {
        for (int j = 0; j < c->num_cols; j++) {
            c->data[i][j] = op(m->data[i][j], scalar);
        }
    }
    
    return c;
}

Matrix* matrix_create(int num_rows, int num_cols) {
    Matrix* m = (Matrix *)malloc(sizeof(Matrix));

    if (m == NULL) return NULL;

    m->num_rows = num_rows;
    m->num_cols = num_cols;
    m->data = (double**)malloc(num_rows * sizeof(double*));

    if (m->data == NULL) {
        free(m);
        return NULL;
    }

    for (int i = 0; i < num_rows; i++) {
        m->data[i] = (double*)malloc(num_cols * sizeof(double));
        
        if (m->data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(m->data[j]);
            }
            free(m->data);
            free(m);
            return NULL;
        }
    }

    return m;
}

void matrix_free(Matrix* m) {
    if (m == NULL || m->data == NULL) return;

    for (int i = 0; i < m->num_rows; i++) {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

void matrix_randomize(Matrix* m, int seed) {
    if (m == NULL || m->data == NULL) return;

    srand(time(NULL) + seed);
    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
            m->data[i][j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        }
    }
}

void matrix_print(Matrix* m) {
    if (m == NULL || m->data == NULL) return;

    printf("\n");
    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
            printf("%.2f\t", m->data[i][j]);
        } 
        printf("\n");
    } 
}

Matrix* matrix_copy(const Matrix* m) {
    if (m == NULL || m->data == NULL) return NULL;

    Matrix* new_m = matrix_create(m->num_rows, m->num_cols);

    if (new_m == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para a cópia da matriz.\n");
        return NULL;
    }

    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
           new_m->data[i][j] = m->data[i][j];
        } 
    } 

    return new_m;
}

Matrix* matrix_transpose(const Matrix* m) {
    if (m == NULL || m->data == NULL) return NULL;

    Matrix* mt = matrix_create(m->num_cols, m->num_rows);

    if (mt == NULL) return NULL;

    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
           mt->data[j][i] = m->data[i][j];
        } 
    } 

    return mt;
}

Matrix* matrix_multiply(const Matrix* a, const Matrix* b) {
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL || a->num_cols != b->num_rows) {
        return NULL; 
    } 

    Matrix* c = matrix_create(a->num_rows, b->num_cols);

    if (c == NULL || c->data == NULL) return NULL;

    for (int i = 0; i < c->num_rows; i++) {
        for (int j = 0; j < c->num_cols; j++) {
            double sum = 0;
            for (int k = 0; k < a->num_cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            c->data[i][j] = sum;
        }
    }

    return c;
}

Matrix* matrix_add(const Matrix* a, const Matrix* b) {
    return apply_op(a, b, op_add);
}

Matrix* matrix_subtract(const Matrix* a, const Matrix* b) {
    return apply_op(a, b, op_subtract);
}

Matrix* matrix_elementwise_multiply(const Matrix* a, const Matrix* b) {
    return apply_op(a, b, op_multiply);
}

Matrix* matrix_scale(const Matrix* m, double scalar) {
    return apply_scalar_op(m, scalar, op_multiply);
}

Matrix* matrix_add_scalar(const Matrix* m, double scalar) {
    return apply_scalar_op(m, scalar, op_add);
}

Matrix* matrix_apply_function(const Matrix* m, double (*func)(double)) {
    if (m == NULL || m->data == NULL || func == NULL) return NULL;

    Matrix* new_m = matrix_create(m->num_rows, m->num_cols);

    if (new_m == NULL) return NULL;

    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
            new_m->data[i][j] = func(m->data[i][j]);
        }
    }

    return new_m;
}