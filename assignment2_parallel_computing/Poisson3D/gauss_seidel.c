/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "helper.h"

void
gauss_seidel(int N, double threshold, int iter_max, double ***U, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;

    double diff_tmp;
    double diff = INFINITY;
    double diff_scale = 1./(N*N*N);
    int iteration = 1; // 1-indiced iterations

    while (diff > threshold && iteration <= iter_max) {
        diff = 0;
        for (i = 1; i <= N ; i++) {
            for (j = 1; j <= N; j++) {
                for (k = 1; k <= N; k++) {
                    diff_tmp = scale * ( // should this be initialised here when parallelising
                        U[i-1][j][k] + 
                        U[i+1][j][k] + 
                        U[i][j-1][k] + 
                        U[i][j+1][k] + 
                        U[i][j][k-1] + 
                        U[i][j][k+1] + 
                        delta * delta * F[i][j][k]);
                    diff += (diff_tmp - U[i][j][k])*(diff_tmp - U[i][j][k]);
                    U[i][j][k] = diff_tmp;
                }
            }
        }
        diff = sqrt(diff_scale *diff);
        iteration++;
    }

    printf("\tIterations: %d\n", iteration);
    printf("\tConvergence_difference: %lf\n", diff);
}

