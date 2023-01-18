#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <omp.h>
#include "matmult.h"


#ifndef _BLOCK_SIZE
#define _BLOCK_SIZE 32
#endif

// standard OpenMP versions
void matmult_mkn_omp(int m,int n,int k,double **A,double **B,double **C){
    #pragma omp parallel shared(A,B,C)
    {
    C = init_C_omp(C,m,n);
    #pragma omp for
    for(int i=0;i<m;i++){
        for(int l=0;l<k;l++){
            for(int j=0;j<n;j++){
                #pragma omp atomic
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
    } // end parallel
}

void matmult_blk_omp(int m,int n,int k,double **A,double **B,double **C, int bs){
    C = init_C_omp(C,m,n);
    for(int i1=0;i1<m;i1+=bs){
        for(int l1=0;l1<k;l1+=bs){
            for(int i2=0; i2 < min(m-i1, bs); i2++){
                for(int l2=0; l2 < min(k-l1, bs); l2++){
                    for(int j=0; j < n; j++){
                        C[i1+i2][j] += A[i1+i2][l1+l2]*B[l1+l2][j];
                    }
                }
            }
        }
    }
}

// Matrix multiplication with library function
void matmult_lib(int m,int n,int k,double **A,double **B,double **C){
    #pragma omp parallel shared(A,B,C)
    {
    C = init_C_omp(C,m,n);
    } // end parallel
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

// offload versions
void matmult_mkn_offload(int m,int n,int k,double **A,double **B, double **C){
    C = init_C(C,m,n);
    #pragma omp target teams loop \
    map(to: A[0:m][0:k], B[0:k][0:n], m,k,n) map(tofrom: C[0:m][0:n]) \
    num_teams(64) thread_limit(32)
    for(int i=0;i<m;i++){
        for(int l=0;l<k;l++){
            for(int j=0;j<n;j++){
                #pragma omp atomic
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}

void matmult_mnk_offload(int m,int n,int k,double **A,double **B,double **C){
    C = init_C(C,m,n);
    #pragma omp target teams loop \
    map(to: A[0:m][0:k], B[0:k][0:n], m,k,n) map(tofrom: C[0:m][0:n]) \
    num_teams(32) thread_limit(32)
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            for(int l=0;l<k;l++){
                #pragma omp atomic
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}



// define helper functions
int min(int a, int b)
{
    return (a < b) ? a : b;
}

double **init_C(double **C, int m, int n){
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            C[i][j] = 0;
        }
    }
    return C;
}

double **init_C_omp(double **C, int m, int n){
    #pragma omp for
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            C[i][j] = 0;
        }
    }
    return C;
}
