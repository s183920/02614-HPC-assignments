#include <stdio.h>
#include <stdlib.h>
#include "cblas.h"

int min(int a, int b);
// helper function for creating the C matrix
double **init_C(double **C, int m, int n){
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            C[i][j] = 0;
        }
    }
    return C;
}


// Naive matrix multiplication
void matmult_nat(int m,int n,int k,double **A,double **B,double **C){
    C = init_C(C,m,n);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            for(int l=0;l<k;l++){
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}

// Matrix multiplication with library function
void matmult_lib(int m,int n,int k,double **A,double **B,double **C){
     // C = A * B using cblas_dgemm see https://www.intel.com/content/www/us/en/develop/documentation/mkl-tutorial-c/top/multiplying-matrices-using-dgemm.html
    cblas_dgemm(
        //Indicates that the matrices are stored in row major order, with the elements of each row of the matrix stored contiguously as shown in the figure above.
        CblasRowMajor, 
        // Enumeration type indicating that the matrices A and B should not be transposed or conjugate transposed before multiplication.
        CblasNoTrans, CblasNoTrans, 
        // Integers indicating the size of the matrices: A: m rows by k columns, B: k rows by n columns, C: m rows by n columns.
        m, n, k, 
        // alpha: Real value used to scale the product of matrices A and B.
        1.0, 
        // Array used to store matrix A/B and leading or the number of elements between successive rows (for row major storage) in memory. In the case of this exercise the leading dimension is the same as the number of columns.
        A[0], k, B[0], n, 
        // beta: Real value used to scale matrix C.
        0.0, 
         // Array used to store matrix C and leading or the number of elements between successive rows (for row major storage) in memory. In the case of this exercise the leading dimension is the same as the number of columns.
        C[0], n
        );
}


// Matrix multiplication with permutations
void matmult_mnk(int m,int n,int k,double **A,double **B,double **C){
    matmult_nat(m,n,k,A,B,C);
}
void matmult_mkn(int m,int n,int k,double **A,double **B,double **C){
    C = init_C(C,m,n);
    for(int i=0;i<m;i++){
        for(int l=0;l<k;l++){
            for(int j=0;j<n;j++){
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}

void matmult_nmk(int m,int n,int k,double **A,double **B,double **C){
    C = init_C(C,m,n);
    for(int j=0;j<n;j++){
        for(int i=0;i<m;i++){
            for(int l=0;l<k;l++){
                C[i][j] = C[i][j] + A[i][l]*B[l][j];
            }
        }
    }
}
void matmult_nkm(int m,int n,int k,double **A,double **B,double **C){
    C = init_C(C,m,n);
    for(int j=0;j<n;j++){
        for(int l=0;l<k;l++){
            for(int i=0;i<m;i++){
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}
void matmult_kmn(int m,int n,int k,double **A,double **B,double **C){
    C = init_C(C,m,n);
    for(int l=0;l<k;l++){
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}
void matmult_knm(int m,int n,int k,double **A,double **B,double **C){
    C = init_C(C,m,n);
    for(int l=0;l<k;l++){
        for(int j=0;j<n;j++){
            for(int i=0;i<m;i++){
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}

void matmult_blk(int m,int n,int k,double **A,double **B,double **C, int bs){
    C = init_C(C,m,n);
    
    for(int i1=0;i1<m;i1+=bs){
        for(int i2=0; i2 < min(m-i1, bs); i2++){
            for(int l=0;l<k;l++){
                for(int j=0;j<n;j++){
                    C[i1+i2][j] += A[i1+i2][l]*B[l][j];
                }
            }
        }
    }

}

int min(int a, int b)
{
    return (a > b) ? b : a;
}