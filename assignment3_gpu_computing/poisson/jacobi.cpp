/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "init.h"

void
jacobi_map(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;

    double diff = INFINITY;
    double diff_scale = 1./(N*N*N);
    int iteration;     
    
    for (iteration = 0; iteration < iter_max; iteration++) {
        //#pragma omp target teams loop map(to: U_old) map(from: U_new)
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
        diff = sqrt(diff_scale *diff);
        swap_3d(&U_old, &U_new);
    }
    swap_3d(&U_old, &U_new);
    
    printf("\tIterations: %d\n", iteration);
    printf("\tConvergence_difference: %lf\n", diff);
}

// parallel version optimized
void
jacobi_para_opt(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;

    double diff = INFINITY;
    double diff_scale = 1./(N*N*N);
    int iteration; 

    for (iteration = 0; iteration < iter_max; iteration++) {
        #pragma omp parallel for private(i,j,k) reduction(+:diff) shared(U_old, U_new, F, scale, diff_scale) schedule(runtime)
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
    }
    swap_3d(&U_old, &U_new);
    
    printf("\tIterations: %d\n", iteration);
    printf("\tConvergence_difference: %lf\n", 0.0);
}