/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include <omp.h>

void
jacobi_map(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;
    int iteration = 0;     
    
    #pragma omp parallel private(i, j, k)
    while (iteration < iter_max) {
        #pragma omp target map(to: U_old[0:N+1][0:N+1][0:N+1], F[0:N+1][0:N+1][0:N+1], delta, scale) map(tofrom: U_new[0:N+1][0:N+1][0:N+1])
        #pragma omp for schedule(static) private(i, j, k)
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

    #pragma omp parallel private(i, j, k) firstprivate(iteration)
    {
    while(iteration < iter_max) {
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
                }
            }
        }
        double ***tmp = U_old;
        U_old = U_new;
        U_new = tmp;
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
        #pragma omp target teams distribute parallel for collapse(3) is_device_ptr(U_d, U_new_d, F_d)
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
        double ***tmp = U_d;
        U_d = U_new_d;
        U_new_d = tmp;
        iteration++;
        }
    // swap_3d(&U_d, &U_new_d);
    
    printf("\tIterations: %d\n", iteration);
}

// void copy(float *dst, float *src, int n) {
//     #pragma omp target teams loop is_device_ptr(dst, src)
//     for (int i = 0; i < n; i++)
//         dst[i] = src[i];
//     }