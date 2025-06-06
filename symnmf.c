# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "symnmf.h"
# define beta 0.5
# define max_iter  300
#define epsilon  0.0001

/* This function frees memory of a two-dimensional array */
void free_matrix(double **matrix, int rows){
    int i = 0;
    if (matrix != NULL) {
        for (; i < rows; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

/* This function prints a two-dimensional array (line by line) */
void printMatrix(double **matrix, int rows, int cols) {
    int i = 0;
    int j = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0 ; j < cols; j++) {
            if (j != cols-1){
                printf("%.4f,", matrix[i][j]);
            } else{
                printf("%.4f", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

/* This function calculates the number of rows and cols of a two-dimensional array */
int* getdAndn(char *filename){
    int n = 0;
    int d = 0;
    int ch = 0;
    int* res = NULL;
    FILE *file = NULL;

    res = (int *)calloc(2 , sizeof(int));
    file = fopen(filename, "r");

    if (res == NULL){
        return NULL;
    }
    if (file == NULL) {
        free(res);
        return NULL;
    }

    /* Count the number of vectors and their dimension */
    while ((ch = fgetc(file)) != EOF) {
        if (n == 0 && (ch == ',' || ch == '\n')) {
            d++;
        }
        if (ch == '\n') {
            n++;
        }
    }

    /* Reset file pointer to the beginning */
    fseek(file, 0, SEEK_SET);
    fclose(file);
    res[0] = n;
    res[1] = d;

    return res;
}

/* This function creates a two-dimensional array from data written in a text file */
double **readDatapointsFromFile(char *filename, int n, int d) {
    int i = 0;
    double **matrix = NULL;
    int j = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    /* Allocate memory for the matrix */
    matrix = (double **) calloc(n, sizeof(double *));
    if (matrix == NULL) {
        return NULL;
    }
    for (i = 0; i < n; i++) {
        matrix[i] = (double *) calloc(d, sizeof(double));
        if (matrix[i] == NULL) {
            free_matrix(matrix, i - 1);
            return NULL;
        }
    }

    /* Read and store the data in the matrix */
    for (i = 0; i<n;i++){
        for (j=0; j<d; j++){
            if (fscanf(file, "%lf,", &matrix[i][j]) != 1) {
                free_matrix(matrix,n);
                return NULL;
            }
        }
    }
    fclose(file);

    return matrix;
}

/* This function calculates the distance between two vectors */
double euclidianDistance(double vector1[], double vector2[], int d) {
    double result = 0.0;
    int i = 0;
    if ((vector1 != NULL) && (vector2 != NULL))
    {
        for (; i < d; i++)
        {
            result = result + pow((vector1[i] - vector2[i]), 2.0);
        }
        result = sqrt(result);
    }

    return result;
}

/* This function calculates the multiplication of two two-dimensional arrays */
double** multiply_matrices(int n1, int m1, int n2, int m2, double **A, double **B){
    double **result = NULL;
    int i = 0;
    int j = 0;
    int k = 0;

    if (m1 != n2) {
        return NULL;
    }

    /* Dynamically allocate memory for the result matrix */
    result = (double **)calloc(n1 , sizeof(double *));
    if (result == NULL){
        return NULL;
    }
    for (i = 0; i < n1; i++) {
        result[i] = (double *)calloc(m2 , sizeof(double));
        if (result[i] == NULL){
            free_matrix(result,i-1);
            return NULL;
        }
    }

    for (i = 0; i < n1; i++) {
        for (j = 0; j < m2; j++) {
            for (k = 0; k < m1; k++) {
                result[i][j] = result[i][j] + (A[i][k] * B[k][j]);

            }
        }
    }

    return result;
}

/* This function calculates C^(-0.5) of a two-dimensional array C */
double** power_of_minus_half(double **A, int rows, int cols) {
    int i = 0;
    int j = 0;
    double **result = NULL;

    /* Dynamically allocate memory for the result matrix */
    result = (double **) calloc(rows, sizeof(double *));
    if (result == NULL) {
        return NULL;
    }
    for (i = 0; i < rows; i++) {
        result[i] = (double *) calloc(cols, sizeof(double));
        if (result[i] == NULL) {
            free_matrix(result, i - 1);
            return NULL;
        }
    }

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            if (A[i][j] == 0) {
                result[i][j] = 0;
            } else if (A[i][j] < 0) {
                free_matrix(result,rows);
                return NULL;
            } else {
                result[i][j] = 1 / sqrt(A[i][j]);
            }
        }
    }

    return result;
}

/* This function calculates C transpose of a two-dimensional array C */
double** transpose_matrix(double **A, int rows, int cols){
    int i = 0;
    int j = 0;
    double **result = NULL;

    /* Dynamically allocate memory for the result matrix */
    result = (double **)calloc(cols , sizeof(double *));
    if (result == NULL) {
        return NULL;
    }
    for (i = 0; i < cols; i++) {
        result[i] = (double *) calloc(rows, sizeof(double));
        if (result[i] == NULL) {
            free_matrix(result, i - 1);
            return NULL;
        }
    }

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            result[j][i] = A[i][j];
        }
    }

    return result;
}

/* This function calculates the frobenius-norm of a two-dimensional array */
double frobenius_norm(double **A, int rows, int cols){
    double result = 0.0;
    int i = 0;
    int j = 0;

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            result += pow(A[i][j],2);
        }
    }

    return result;
}

/* This function calculates the subtraction of two two-dimensional arrays */
double** matrices_subtraction(double **A, double **B, int rows, int cols){
    int i = 0;
    int j = 0;
    double **result = NULL;

    /* Dynamically allocate memory for the result matrix */
    result = (double **)calloc(rows , sizeof(double *));
    if (result == NULL){
        return NULL;
    }
    for (i = 0; i < rows; i++) {
        result[i] = (double *)calloc(cols , sizeof(double));
        if (result[i] == NULL) {
            free_matrix(result, i - 1);
            return NULL;
        }
    }

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }

    return result;
}

/* This function creates a copy of a two-dimensional array */
double** matrix_copy(double **A, int rows, int cols){
    int i = 0;
    int j = 0;
    double **result = NULL;

    /* Dynamically allocate memory for the result matrix */
    result = (double **)calloc(rows , sizeof(double *));
    if (result == NULL) {
        return NULL;
    }
    for (i = 0; i < rows; i++) {
        result[i] = (double *)calloc(cols , sizeof(double ));
        if (result[i] == NULL) {
            free_matrix(result, i - 1);
            return NULL;
        }
    }
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            result[i][j] = A[i][j];
        }
    }
    return result;
}

/* This function calculates the Similarity Matrix */
double** sym_c(double **X, int n, int d){
    int i = 0;
    int j = 0;
    double **A = NULL;

    /* Dynamically allocate memory for the result matrix */
    A = (double **)calloc(n , sizeof(double *));
    if (A == NULL) {
        return NULL;
    }
    for (i = 0; i < n; i++) {
        A[i] = (double *)calloc(n , sizeof(double));
        if (A[i] == NULL) {
            free_matrix(A, i - 1);
            return NULL;
        }
    }
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            if (i != j){
                A[i][j] = exp( pow(euclidianDistance(X[i], X[j], d),2.0) / -2.0);
            }
            else {
                A[i][j] = 0.0;
            }
        }
    }

    return A;
}

/* This function calculates the Diagonal Degree Matrix */
double** ddg_c(double **X, int n, int d){
    int i = 0;
    int j = 0;
    int k = 0;
    double sum = 0;
    double **A = NULL;
    double **D = NULL;
    A = sym_c(X, n, d); /* Calculates the Similarity Matrix */
    if (A == NULL) return NULL;
    D = (double **)calloc(n , sizeof(double *));    /* Dynamically allocate memory for the result matrix */
    if (D == NULL) {
        free_matrix(A,n);
        return NULL;
    }
    for (i = 0; i < n; i++) {
        D[i] = (double *)calloc(n , sizeof(double));
        if (D[i] == NULL) {
            free_matrix(A,n);
            free_matrix(D, i - 1);
            return NULL;
        }
    }
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            if (i != j){
                D[i][j] = 0;
            }
            else {
                sum = 0;
                for (k = 0; k < n ; k++) sum += A[i][k];
                D[i][j] = sum;
            }
        }
    }
    free_matrix(A,n);
    A = NULL;
    return D;
}

/* This function calculates the Normalized Similarity Matrix */
double** norm_c(double **X, int n, int d){
    double **A = NULL;
    double **D = NULL;
    double  **D_half = NULL;
    double **first_mult = NULL;
    double **W = NULL;
    /* Calculates the Similarity Matrix */
    A = sym_c(X, n, d);
    if (A == NULL) return NULL;
    /* Calculates the Diagonal Degree Matrix */
    D = ddg_c(X,n,d);
    if (D == NULL){
        free_matrix(A,n);
        return NULL;
    }
    /* Calculates the D^(-0.5) Matrix */
    D_half = power_of_minus_half(D, n, n);
    if (D_half == NULL){
        free_matrix(A,n);
        free_matrix(D,n);
        return NULL;
    }
    /* Calculates the D^(-0.5)*A Matrix */
    first_mult = multiply_matrices(n,n,n,n,D_half,A);
    if (first_mult == NULL){
        free_matrix(A,n);
        free_matrix(D,n);
        free_matrix(D_half,n);
        return NULL;
    }
    /* Calculates the D^(-0.5)*A*D^(-0.5) Matrix W */
    W = multiply_matrices(n,n,n,n,first_mult,D_half);
    free_matrix(A,n);
    free_matrix(D,n);
    free_matrix(D_half,n);
    free_matrix(first_mult,n);
    return W;
}
/* Helper function to perform matrix operations */
double **perform_matrix_operations(double **H_COPY, double **W_COPY, int n, int k) {
    double **W_H = multiply_matrices(n, n, n, k, W_COPY, H_COPY);
    double **HT = transpose_matrix(H_COPY, n, k);
    double **H_HT = multiply_matrices(n, k, k, n, H_COPY, HT);
    double **H_HT_H = multiply_matrices(n, n, n, k, H_HT, H_COPY);
    
    if (!W_H || !HT || !H_HT || !H_HT_H) {
        free_matrix(W_H, n);
        free_matrix(HT, k);
        free_matrix(H_HT, n);
        free_matrix(H_HT_H, n);
        return NULL;
    }
    return H_HT_H;
}

/* Helper function to allocate new H matrix */
double **allocate_H_new(int n, int k) {
    double **H_NEW = (double **)calloc(n, sizeof(double *));
    int i;  
    if (!H_NEW) return NULL;
    
    for (i = 0; i < n; ++i) {
        H_NEW[i] = (double *)calloc(k, sizeof(double));
        if (!H_NEW[i]) {
            free_matrix(H_NEW, i);
            return NULL;
        }
    }
    return H_NEW;
}

/* Helper function to update H_NEW */
void update_H_new(double **H_NEW, double **H_COPY, double **H_HT_H, double **W_H, int n, int k) {
    int i, j; 
    for (i = 0; i < n; ++i) {
        for (j = 0; j < k; ++j) {
            H_NEW[i][j] = H_COPY[i][j] * ((1 - beta) + (beta * (W_H[i][j] / H_HT_H[i][j])));
        }
    }
}


double** symnmf_c(double **H, double **W, int n, int k) {
    int is_converge = 1, iter = 0;
    double frobenius = 0.0;
    double **H_COPY = matrix_copy(H, n, k), **W_COPY = matrix_copy(W, n, n), **H_NEW = NULL;
    
    if (!H_COPY || !W_COPY) {
        if (H_COPY) free_matrix(H_COPY, n);
        return NULL;
    }

    while (is_converge && iter < max_iter) {
        double **W_H = multiply_matrices(n, n, n, k, W_COPY, H_COPY);
        double **H_HT_H = perform_matrix_operations(H_COPY, W_COPY, n, k);
        if (!H_HT_H) {
            free_matrix(H_COPY, n);
            free_matrix(W_COPY, n);
            return NULL;
        }

        H_NEW = allocate_H_new(n, k);
        if (!H_NEW) {
            free_matrix(H_HT_H, n);
            free_matrix(H_COPY, n);
            free_matrix(W_COPY, n);
            return NULL;
        }

        update_H_new(H_NEW, H_COPY, H_HT_H, W_H, n, k);

        /* Frobenius norm calculation */
        frobenius = frobenius_norm(matrices_subtraction(H_NEW, H_COPY, n, k), n, k);
        is_converge = frobenius >= epsilon;
        
        /* Free old matrices and prepare for the next iteration */
        free_matrix(H_COPY, n);
        free_matrix(H_HT_H, n);
        free_matrix(W_H, n);
        if (is_converge) H_COPY = H_NEW;
        iter++;
    }
    free_matrix(W_COPY, n);
    return H_NEW;
}



/* Main function for running directly from C */
int main(int argc, char *argv[]) {
    double **result = NULL, **X = NULL;
    int n = 0, d = 0, *n_and_d = NULL;
    char *goal, *file_name;
    if (argc == 0){
        printf("An Error Has Occurred\n");
        return 1;
    }
    goal = argv[1];
    file_name = argv[2];
    n_and_d = getdAndn(file_name);/* Calculates n and d out of the data points file */
    if (n_and_d == NULL){
        printf("An Error Has Occurred\n");
        return 1;
    }
    n = n_and_d[0], d = n_and_d[1];
    free(n_and_d);
    X = readDatapointsFromFile(file_name,n,d);    /* Extracts the data points from the file and stores them in a two-dimensional array */
    if (X == NULL){
        printf("An Error Has Occurred\n");
        return 1;
    }
    if (strcmp(goal, "sym") == 0) result = sym_c(X,n,d); /* In case the 'sym' goal is chosen */
    else if (strcmp(goal, "ddg") == 0) result = ddg_c(X,n,d); /* In case the 'ddg' goal is chosen */
    else result = norm_c(X,n,d); /* the 'norm' goal is chosen */
    if (result == NULL){
        free_matrix(X,n);
        printf("An Error Has Occurred\n");
        return 1;
        }
    free_matrix(X,n);
    printMatrix(result,n,n);
    free_matrix(result,n);
    return 0;
}

