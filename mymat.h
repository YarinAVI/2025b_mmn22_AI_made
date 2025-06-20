#ifndef MYMAT_H
#define MYMAT_H

/* Matrix structure definition */
typedef struct {
    double data[4][4];
} mat;

/* Function declarations */

/* Read values into a matrix */
void read_mat(mat *m, double values[], int count);

/* Print a matrix */
void print_mat(const mat *m);

/* Add two matrices: result = a + b */
void add_mat(const mat *a, const mat *b, mat *result);

/* Subtract two matrices: result = a - b */
void sub_mat(const mat *a, const mat *b, mat *result);

/* Multiply two matrices: result = a * b */
void mul_mat(const mat *a, const mat *b, mat *result);

/* Multiply a matrix by a scalar: result = m * scalar */
void mul_scalar(const mat *m, double scalar, mat *result);

/* Transpose a matrix: result = m^T */
void trans_mat(const mat *m, mat *result);

#endif /* MYMAT_H */