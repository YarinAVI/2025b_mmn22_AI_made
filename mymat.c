#include "mymat.h"
#include <stdio.h>

/* Read values into a matrix row by row, setting remaining cells to 0 */
void read_mat(mat *m, double values[], int count) {
    int i, j, index = 0;
    
    /* Initialize all elements to 0 */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m->data[i][j] = 0.0;
        }
    }
    
    /* Fill matrix row by row with provided values */
    for (i = 0; i < 4 && index < count; i++) {
        for (j = 0; j < 4 && index < count; j++) {
            m->data[i][j] = values[index];
            index++;
        }
    }
}

/* Print a matrix with proper formatting */
void print_mat(const mat *m) {
    int i, j;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%8.2f", m->data[i][j]);
            if (j < 3) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/* Add two matrices: result = a + b */
void add_mat(const mat *a, const mat *b, mat *result) {
    int i, j;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
}

/* Subtract two matrices: result = a - b */
void sub_mat(const mat *a, const mat *b, mat *result) {
    int i, j;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
}

/* Multiply two matrices: result = a * b */
void mul_mat(const mat *a, const mat *b, mat *result) {
    mat temp;
    int i, j, k;
    
    /* Use temporary matrix to handle aliasing */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp.data[i][j] = 0.0;
            for (k = 0; k < 4; k++) {
                temp.data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    
    /* Copy result from temporary matrix */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result->data[i][j] = temp.data[i][j];
        }
    }
}

/* Multiply a matrix by a scalar: result = m * scalar */
void mul_scalar(const mat *m, double scalar, mat *result) {
    int i, j;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result->data[i][j] = m->data[i][j] * scalar;
        }
    }
}

/* Transpose a matrix: result = m^T */
void trans_mat(const mat *m, mat *result) {
    mat temp;
    int i, j;
    
    /* Use temporary matrix to handle aliasing */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp.data[i][j] = m->data[j][i];
        }
    }
    
    /* Copy result from temporary matrix */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            result->data[i][j] = temp.data[i][j];
        }
    }
}