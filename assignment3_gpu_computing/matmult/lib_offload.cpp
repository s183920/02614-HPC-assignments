#include <cublas_v2.h>
#include <omp.h>
#include "lib_offload.h"

// OpenMP library version with offload for question 5
void matmult_lib_offload(int m,int n,int k,double **A,double **B,double **C){
    // C = init_C_omp(C,m,n);


    double *A_ptr = A[0];
    double *B_ptr = B[0];
    double *C_ptr = C[0];


    // Initialize CUBLAS
    cublasHandle_t handle;
    cublasCreate(&handle);

    #pragma target data \
        map(tofrom: A_ptr[0:m*k], B_ptr[0:k*n], C_ptr[0:m*n]) 
    {
    // parameters
    double alpha = 1.0;
    double beta = 0.0;
    

    cublasDgemm(
        handle, // handle to the CUBLAS library context
        CUBLAS_OP_N, CUBLAS_OP_N,  // _N means no transpose, _T means transpose and _C means conjugate transpose for the input matrices A and B
        m, n, k, // size of matrices
        &alpha, // pointer to a scalar that is multiplied by the result of the matrix multiplication.
        A_ptr, m, // matrix A (pointer to the memory location of the first matrix A. It should be of size m*k) and the leading dimension (nrows) of A
        B_ptr, k, // matrix B (pointer to the memory location of the first matrix B. It should be of size k*n) and the leading dimension (nrows) of B
        &beta, // pointer to a scalar that is multiplied by the matrix C
        C_ptr, m // matrix C (pointer to the memory location of the first matrix C. It should be of size m*k) and the leading dimension (nrows) of C
    ); 

    // Destroy the handle
    cublasDestroy(handle);
    }
}