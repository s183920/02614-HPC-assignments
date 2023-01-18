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
    
    #pragma omp parallel shared(U_old, U_new, F, delta, scale) private(i, j, k, iteration)
    while (iteration < iter_max) {
        #pragma omp target map(to: U_old[0:N+1][0:N+1][0:N+1], F[0:N+1][0:N+1][0:N+1], delta, scale) map(tofrom: U_new[0:N+1][0:N+1][0:N+1])
        #pragma omp for
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
        swap_3d(&U_old, &U_new);
        iteration++;
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

    #pragma omp parallel shared(U_old, U_new, F, delta, scale) private(i, j, k, iteration)
    {
    while(iteration < iter_max) {
        #pragma omp for
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
        swap_3d(&U_old, &U_new);
        iteration++;
    }
    }
    swap_3d(&U_old, &U_new);
    printf("\tMax iterations: %d\n", iter_max);
    printf("\tIterations: %d\n", iteration);
}

void
jacobi_GPU(int N, double threshold, int iter_max, double ***U_old, double ***U_new, 
            double ***F, double ***U_d, double ***U_new_d, double ***F_d, double *data, double delta) {
    
    /* initialize U_new, U_old and F on host */
    omp_target_memcpy(data, U_new[0], N * N * N * sizeof(double), 0, 0, omp_get_default_device(), omp_get_initial_device());
    omp_target_memcpy(data, U_old[0], N * N * N * sizeof(double), 0, 0, omp_get_default_device(), omp_get_initial_device());
    omp_target_memcpy(data, F[0], N * N * N * sizeof(double), 0, 0, omp_get_default_device(), omp_get_initial_device());

    int i, j, k;
    double scale = 1.0/6.0;
    int iteration = 0;     


    //#pragma omp parallel shared(U_old, U_new, F, delta, scale) private(i, j, k, iteration)
    while (iteration < iter_max) {
        #pragma omp target teams distribute parallel for is_device_ptr(U_d, U_new_d, F_d)
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
        swap_3d(&U_d, &U_new_d);
        iteration++;
        }
    swap_3d(&U_d, &U_new_d);
    
    printf("\tIterations: %d\n", iteration);
}