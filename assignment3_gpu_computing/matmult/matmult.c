#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <cublas_v2.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <omp.h>
#include "matmult.h"

// define variables if they dont exist - this should not be the case when compiling with make

#define _TIMING


#ifndef _BLOCK_SIZE
#define _BLOCK_SIZE 14
#endif

#ifndef _TEAMS
#define _TEAMS 16384
#endif

#ifndef _THREADS
#define _THREADS 16
#endif

#ifndef _SLABS
#define _SLABS 4
#endif

// standard OpenMP versions for question 1
void matmult_mkn_omp(int m,int n,int k,double **A,double **B,double **C){
    C = init_C_omp(C,m,n); // initialize C as we are not interested in timing this
    
    #ifdef _TIMING // start timing
    double t1, t2;
    t1 = omp_get_wtime();
    #endif // end timing

    #pragma omp parallel shared(A,B,C)
    {
    #pragma omp for
    for(int i=0;i<m;i++){
        for(int l=0;l<k;l++){
            for(int j=0;j<n;j++){
                // #pragma omp atomic
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }

    #ifdef _TIMING // start timing
    t2 = omp_get_wtime();
    printf("Time: %lf\n", (t2-t1)*1e3);
    #endif // end timing

    } // end parallel
}

void matmult_blk_omp(int m,int n,int k,double **A,double **B,double **C, int bs){
    C = init_C_omp(C,m,n);

    #ifdef _TIMING // start timing
    double t1, t2;
    t1 = omp_get_wtime();
    #endif // end timing

    #pragma omp parallel shared(m,n,k,A,B,C,bs) 
    {
    #pragma omp for collapse(2)
    for(int i1=0;i1<m;i1+=bs){
        for(int j1=0; j1 < n; j1+=bs){
            for(int l1=0;l1<k;l1+=bs){  
                for(int i2=0; i2 < min(m-i1, bs); i2++){
                    for(int l2=0; l2 < min(k-l1, bs); l2++){
                        for(int j2=0; j2 < min(n-j1, bs); j2++){
                            // #pragma omp atomic
                            C[i1+i2][j1+j2] += A[i1+i2][l1+l2]*B[l1+l2][j1+j2];
                        }
                    }
                }
            }
        }
    }
    }

    #ifdef _TIMING // start timing
    t2 = omp_get_wtime();
    printf("Time: %lf\n", (t2-t1)*1e3);
    #endif // end timing
}

void matmult_lib(int m,int n,int k,double **A,double **B,double **C){
    C = init_C_omp(C,m,n);
     // C = A * B using cblas_dgemm see https://www.intel.com/content/www/us/en/develop/documentation/mkl-tutorial-c/top/multiplying-matrices-using-dgemm.html
    #ifdef _TIMING // start timing
    double t1, t2;
    t1 = omp_get_wtime();
    #endif // end timing

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

    #ifdef _TIMING // start timing
    t2 = omp_get_wtime();
    printf("Time: %lf\n", (t2-t1)*1e3);
    #endif // end timing
}

// OpenMP offload versions for question 2
void matmult_mkn_offload(int m,int n,int k,double **A,double **B, double **C){
    // printf("teams: %d, threads: %d, slabs: %d\n", _TEAMS, _THREADS, _SLABS);
    C = init_C_omp(C,m,n);
    
    // timings
    #ifdef _TIMING // TRANSER_TIMING start
    double t1, t2;
    t1 = omp_get_wtime();

    // transfer data to device
    #pragma omp target data map(to: A[0:m][0:k], B[0:k][0:n], m,k,n) map(tofrom: C[0:m][0:n])
    {
    t1 = omp_get_wtime();
    #else
    #pragma omp target data map(to: A[0:m][0:k], B[0:k][0:n], m,k,n) map(tofrom: C[0:m][0:n])
    {
    #endif // TRANSER_TIMING end
    #pragma omp target teams distribute parallel for \
    map(to: A[0:m][0:k], B[0:k][0:n], m,k,n) map(tofrom: C[0:m][0:n]) \
    num_teams(_TEAMS) thread_limit(_THREADS) 
    for(int i=0;i<m;i++){
        for(int l=0;l<k;l++){
            for(int j=0;j<n;j++){
                // #pragma omp atomic
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
    #ifdef _TIMING // TRANSER_TIMING start
    t2 = omp_get_wtime();
    printf("Time without transfer: %f\n", 1e3*(t2-t1));
    } // exit data
    t2 = omp_get_wtime();
    printf("Time with transfer: %f\n", 1e3*(t2-t1));
    #else
    } // exit data
    #endif // TRANSER_TIMING end
}

void matmult_mnk_offload(int m,int n,int k,double **A,double **B,double **C){
    C = init_C_omp(C,m,n);
    // timings
    #ifdef _TIMING // TRANSER_TIMING start
    double t1, t2;
    t1 = omp_get_wtime();
    
    // transfer data to device
    #pragma omp target data map(to: A[0:m][0:k], B[0:k][0:n], m,k,n) map(tofrom: C[0:m][0:n])
    {
    t1 = omp_get_wtime();
    #else
    #pragma omp target data map(to: A[0:m][0:k], B[0:k][0:n], m,k,n) map(tofrom: C[0:m][0:n])
    {
    #endif // TRANSER_TIMING end

    #pragma omp target teams distribute parallel for \
    map(to: A[0:m][0:k], B[0:k][0:n], m,k,n) map(tofrom: C[0:m][0:n]) \
    num_teams(_TEAMS) thread_limit(_THREADS) collapse(2)
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            for(int l=0;l<k;l++){
                // #pragma omp atomic
                C[i][j] += A[i][l]*B[l][j];
            }
        }
    }
    #ifdef _TIMING // TRANSER_TIMING start
    t2 = omp_get_wtime();
    printf("Time without transfer: %f\n", 1e3*(t2-t1));
    } // exit data
    t2 = omp_get_wtime();
    printf("Time with transfer: %f\n", 1e3*(t2-t1));
    #else
    } // exit data
    #endif // TRANSER_TIMING end
}

// OpenMP offload versions with blocking for question 3
void matmult_blk_offload(int m, int n, int k, double **A,double **B,double **C){
    C = init_C_omp(C,m,n);
    // timings
    #ifdef _TIMING // TRANSER_TIMING start
    double t1, t2;
    t1 = omp_get_wtime();
    #pragma omp target data map(to: A[:m][:k], B[:k][:n], m,k,n) map(tofrom: C[:m][:n])
    {
    t1 = omp_get_wtime();
    #else
    #pragma omp target data map(to: A[:m][:k], B[:k][:n], m,k,n) map(tofrom: C[:m][:n])
    {
    #endif // TRANSER_TIMING end
    #pragma omp target teams loop \
    map(to: A[:m][:k], B[:k][:n], m,k,n) map(tofrom: C[:m][:n]) \
    num_teams(m) thread_limit(_THREADS)\
    collapse(2)
    for(int i1=0;i1<m;i1+=_BLOCK_SIZE){
        for(int j=0;j<n;j++){
            int i2, l;
            double temp_sum[_BLOCK_SIZE] = {0};
            if (_BLOCK_SIZE -1 < (m-i1)){
                for(l=0;l<k;l++){   
                    for(i2=0; i2 < _BLOCK_SIZE; i2++){
                        temp_sum[i2] += A[i1+i2][l] * B[l][j];
                    }
                }

                for (int i=0; i < _BLOCK_SIZE; i++){
                    C[i + i1][j] = temp_sum[i];
                }

            }
            else { 
                for(l=0;l<k;l++){   
                    for(i2=0; i2 < (m-i1); i2++){
                        C[i1+i2][j] += A[i1+i2][l]*B[l][j];
                    }
                }
            }
        }
    }
    #ifdef _TIMING // TRANSER_TIMING start
    t2 = omp_get_wtime();
    printf("Time without transfer: %lf\n", (t2-t1)*1e3);
    }
    t2 = omp_get_wtime();
    printf("Time with transfer: %lf\n", (t2-t1)*1e3);
    #else
    }
    #endif // TRANSER_TIMING end
}   



// OpenMP offload versions with asynchronous data transfers for question 4
void matmult_asy_offload(int m, int n, int k, double **A,double **B,double **C){
    C = init_C_omp(C,m,n);

    // timings
    #ifdef _TIMING // TRANSER_TIMING start
    double t1, t2;
    t1 = omp_get_wtime();
    #endif // TRANSER_TIMING end

    #pragma omp target enter data map(alloc: A[0:m][0:k], B[0:k][0:n], C[0:m][0:n])
    #pragma omp target update to(B[0:k][0:n])

    #pragma omp parallel for
    for (int s = 0; s < _SLABS; ++s) {
        int length = m/_SLABS; // assumed to be divisible such that we get an integer
        int start = s*length;

        #pragma omp target update to(A[start:length][0:k]) depend(out:A) nowait

        #pragma omp target teams distribute parallel for \
        map(to: A[start:length][:k]) \
        num_teams(_TEAMS) thread_limit(_THREADS)\
        depend(in:A) depend(out:C) nowait \
        collapse(2)
        for(int i1=start; i1<start+length;i1+=_BLOCK_SIZE){
            for(int j=0;j<n;j++){
                if (_BLOCK_SIZE -1 < (m-i1)){
                    double temp_sum[_BLOCK_SIZE] = {0};
                    for(int l=0;l<k;l++){   
                        for(int i2=0; i2 < _BLOCK_SIZE; i2++){
                            temp_sum[i2] += A[i1+i2][l] * B[l][j];
                        }
                    }
                    for (int i=0; i < _BLOCK_SIZE; i++){
                        C[i + i1][j] = temp_sum[i];
                    }
                }
                else { 
                    for(int i2=0; i2 < (m-i1); i2++){
                        double sum= 0;
                        for(int l=0;l<k;l++){   
                            sum += A[i2+i1][l]*B[l][j];
                        }
                        C[i1+i2][j] = sum;
                    }
                }
            }
        } // end block
        #pragma omp target update from(C[start:length][:n]) depend(in:C) nowait
        // printf("s: %d, start: %d, length: %d\n", s, start, length);
    } // end async/parallel
    #pragma omp taskwait
    #pragma omp target exit data map(delete: A[0:m][0:k], B[0:k][0:n], C[0:m][0:n])

    #ifdef _TIMING // TRANSER_TIMING start
    t2 = omp_get_wtime();
    printf("Time: %f\n", 1e3*(t2-t1));
    #endif // TRANSER_TIMING end
}   

// OpenMP library version with offload for question 5
void matmult_lib_offload(int m, int n, int k, double **A, double **B, double **C) {

    #ifdef _TIMING // start timing
    double t1, t2;
    t1 = omp_get_wtime();
    #endif // end timing


    cublasHandle_t handle;
    cublasCreate(&handle);

    double *d_A, *d_B, *d_C;
    cudaMalloc((void **)&d_A, m * k * sizeof(double));
    cudaMalloc((void **)&d_B, k * n * sizeof(double));
    cudaMalloc((void **)&d_C, m * n * sizeof(double));
    // size_t pitch_C;
    // cudaMallocPitch((void**)&d_C, &pitch_C, n*sizeof(double), m);

    cudaMemcpy(d_A, *A, m * k * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, *B, k * n * sizeof(double), cudaMemcpyHostToDevice);

    double alpha = 1.0;
    double beta = 0.0;

    cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, &alpha, d_A, m, d_B, k, &beta, d_C, m);

    // double alpha_transpose = 1.0;
    // double beta_transpose = 0.0;
    // cublasDgeam(handle, CUBLAS_OP_T, CUBLAS_OP_N, n, m, &alpha_transpose, d_C, pitch_C, &beta_transpose, d_C, m, d_C, pitch_C);
    cudaMemcpy(*C, d_C, m * n * sizeof(double), cudaMemcpyDeviceToDevice);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cublasDestroy(handle);

    #ifdef _TIMING // start timing
    t2 = omp_get_wtime();
    printf("Time: %lf\n", (t2-t1)*1e3);
    #endif // end timing
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
    #pragma omp parallel for
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            C[i][j] = 0;
        }
    }
    return C;
}
