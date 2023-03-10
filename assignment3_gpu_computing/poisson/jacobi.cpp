/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include <omp.h>

#ifndef _THREADS
#define _THREADS 16
#endif

#ifndef _TEAMS
#define _TEAMS 500
#endif

void
jacobi_map(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;
    int iteration = 0;     

    #pragma omp target data map(to: U_old[0:N+2][0:N+2][0:N+2], F[0:N+2][0:N+2][0:N+2]) map(tofrom: U_new[0:N+2][0:N+2][0:N+2])
    while (iteration < iter_max) {
        #pragma omp target teams distribute parallel for num_teams((N*N*N)/64) thread_limit(64) collapse(3)
        for (i = 1; i <= N ; i++) {
            for (j = 1; j <= N; j++) {
                for (k = 1; k <= N; k++) {                    
                    U_new[i][j][k] = scale * ( 
                        U_old[i-1][j][k] + 
                        U_old[i+1][j][k] + 
                        U_old[i][j-1][k] + 
                        U_old[i][j+1][k] + 
                        U_old[i][j][k-1] + 
                        U_old[i][j][k+1] + 
                        delta * delta * F[i][j][k]);
                }
            }
        }
        iteration++;
        double ***tmp = U_old;
        U_old = U_new;
        U_new = tmp;
        }
    swap_3d(&U_old, &U_new);
    
    printf("\tIterations: %d\n", iteration);
}

// parallel version optimized
void
jacobi_para_opt(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;
    int iteration = 0; 
    double ***tmp;

    //double diff = INFINITY; //outcomment for 6 and 7
    //double diff_scale = 1./(N*N*N); //outcomment for 6 and 7

    #pragma omp parallel private(i, j, k) firstprivate(iteration)
    {
    while(iteration < iter_max) { //change for 6 and 7
        //diff = 0.0; //outcomment for 6 and 7
        #pragma omp for schedule(static) private(i, j, k)
        for (i = 1; i <= N ; i++) {
            for (j = 1; j <= N; j++) {
                for (k = 1; k <= N; k++) {
                    U_new[i][j][k] = scale * ( // should this be initialised here when parallelising
                        U_old[i-1][j][k] + 
                        U_old[i+1][j][k] + 
                        U_old[i][j-1][k] + 
                        U_old[i][j+1][k] + 
                        U_old[i][j][k-1] + 
                        U_old[i][j][k+1] + 
                        delta * delta * F[i][j][k]);
                        //diff += (U_old[i][j][k] - U_new[i][j][k])*(U_old[i][j][k] - U_new[i][j][k]);
                }
            }
        }
        swap_3d(&U_old, &U_new);
        iteration++;  
    }
    }
    swap_3d(&U_old, &U_new);
    printf("\tMax iterations: %d\n", iter_max);
    printf("\tIterations: %d\n", iteration);
}

// GPU versin TODO: Segmentation fault fix
void
jacobi_GPU(int N, double threshold, int iter_max, double ***U_d, double ***U_new_d, double ***F_d, double delta) {
    
    int i, j, k;
    double scale = 1.0/6.0;
    int iteration = 0;     

    //#pragma omp parallel shared(U_d, U_new, F, delta, scale) private(i, j, k, iteration)
    while (iteration < iter_max) {
        #pragma omp target teams distribute parallel for collapse(3) is_device_ptr(U_d, U_new_d, F_d) thread_limit(_THREADS) num_teams(_TEAMS)
        for (i = 1; i <= N ; i++) {
            for (j = 1; j <= N; j++) {
                for (k = 1; k <= N; k++) {                    
                    U_new_d[i][j][k] = scale * ( 
                        U_d[i-1][j][k] + 
                        U_d[i+1][j][k] + 
                        U_d[i][j-1][k] + 
                        U_d[i][j+1][k] + 
                        U_d[i][j][k-1] + 
                        U_d[i][j][k+1] + 
                        delta * delta * F_d[i][j][k]);
                }
            }
        }
        //swap_3d(&U_d, &U_new_d);
        iteration++;
        }
    swap_3d(&U_d, &U_new_d);
    
    printf("\tIterations: %d\n", iteration);
}

// void copy(float *dst, float *src, int n) {
//     #pragma omp target teams loop is_device_ptr(dst, src)
//     for (int i = 0; i < n; i++)
//         dst[i] = src[i];
//     }

void
jacobi_dual_GPU(int N, double threshold, int iter_max, double ***U_d, double ***U_new_d, double ***F_d, double ***U_d1, double ***U_new_d1, double ***F_d1, double delta) {
    
    double scale = 1.0/6.0;
    int iteration = 0;     

    cudaSetDevice(0);
    cudaDeviceEnablePeerAccess(1, 0); // (dev 1, future flag)
    cudaSetDevice(1);
    cudaDeviceEnablePeerAccess(0, 0); // (dev 0, future flag)
    //#pragma omp parallel shared(U_d, U_new, F, delta, scale) private(i, j, k, iteration)
    while (iteration < iter_max) {
        cudaSetDevice(0);
        #pragma omp target teams distribute parallel for collapse(3) is_device_ptr(U_d, U_d1, U_new_d, U_new_d1, F_d, F_d1) nowait \
        thread_limit(_THREADS) num_teams(_TEAMS) device(0)
        for (int i = 1; i <= N/2 ; i++) {
            for (int j = 1; j <= N; j++) {
                for (int k = 1; k <= N; k++) {                    
                    U_new_d[i][j][k] = scale * ( 
                        U_d[i-1][j][k] + 
                        U_d[i+1][j][k] + 
                        U_d[i][j-1][k] + 
                        U_d[i][j+1][k] + 
                        U_d[i][j][k-1] + 
                        U_d[i][j][k+1] + 
                        delta * delta * F_d[i][j][k]);
                    //U_new_d1[i][j][k] = U_new_d[i][j][k];
                }
            }
        }
        cudaSetDevice(1);
        #pragma omp target teams distribute parallel for collapse(3) is_device_ptr(U_d, U_d1, U_new_d, U_new_d1, F_d, F_d1) nowait \
        thread_limit(_THREADS) num_teams(_TEAMS) device(1)
        for (int i = N; i > N/2 ; i--) {
            for (int j = 1; j <= N; j++) {
                for (int k = 1; k <= N; k++) {                    
                    U_new_d1[i][j][k] = scale * ( 
                        U_d1[i-1][j][k] + 
                        U_d1[i+1][j][k] + 
                        U_d1[i][j-1][k] + 
                        U_d1[i][j+1][k] + 
                        U_d1[i][j][k-1] + 
                        U_d1[i][j][k+1] + 
                        delta * delta * F_d1[i][j][k]);
                    //U_new_d[i][j][k] = U_new_d1[i][j][k];
                }
            }
        }
        
        //#pragma omp taskwait
        swap_3d(&U_d, &U_new_d);
        swap_3d(&U_d1, &U_new_d1);
        iteration++;
        }
    //swap_3d(&U_d, &U_new_d);
    //swap_3d(&U_d1, &U_new_d1);
    
    printf("\tIterations: %d\n", iteration);
}

void
jacobi_map_norm(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;
    double ***tmp;    

    double diff = INFINITY;
    double diff_scale = 1./(N*N*N);
    int iteration = 0; 
    
    #pragma omp target data map(to: U_old[0:N+2][0:N+2][0:N+2], F[0:N+2][0:N+2][0:N+2]) map(tofrom: U_new[0:N+2][0:N+2][0:N+2])
    while (diff > threshold && iteration < iter_max) {
        diff = 0.0;
        #pragma omp target teams distribute parallel for reduction(+:diff) num_teams(_TEAMS) thread_limit(_THREADS) collapse(3)
        for (i = 1; i <= N ; i++) {
            for (j = 1; j <= N; j++) {
                for (k = 1; k <= N; k++) {                    
                    U_new[i][j][k] = scale * ( 
                        U_old[i-1][j][k] + 
                        U_old[i+1][j][k] + 
                        U_old[i][j-1][k] + 
                        U_old[i][j+1][k] + 
                        U_old[i][j][k-1] + 
                        U_old[i][j][k+1] + 
                        delta * delta * F[i][j][k]);
                    diff += (U_old[i][j][k] - U_new[i][j][k])*(U_old[i][j][k] - U_new[i][j][k]);
                }
            }
        }
        iteration += 1;
        #pragma omp single
            {
                diff = sqrt(diff_scale *diff);
                tmp = U_new;
                U_new = U_old;
                U_old = tmp;
            }
        }
    swap_3d(&U_old, &U_new);
    //why does iteration not work here?
    printf("\tIterations: %d\n", iteration);
    printf("\tDiff: %f\n", diff);
}
