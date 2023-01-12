/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include "helper.h"
#include <stdio.h>

void
jacobi(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta) {
    int i, j, k;
    i=j=k=0;
    double scale = 1.0/6.0;

    double diff_tmp;
    double diff = INFINITY;
    double diff_scale = 1./(N*N*N);
    int iteration = 1; // 1-indiced iterations

    while (diff > threshold && iteration <= iter_max) {
        diff = 0;
        // printf("\t In iter %d\n", iteration);
        for (i = 1; i <= N ; i++) {
            // printf("In outer loop i=%d\n", i);
            for (j = 1; j <= N; j++) {
                for (k = 1; k <= N; k++) {

                    // printf("\t value of U_old=%lf", U_old[i+1][j][k]);
                    
                    U_new[i][j][k] = scale * ( // should this be initialised here when parallelising
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

        iteration++;
    }
    swap_3d(&U_old, &U_new);
    printf("Number of iterations: %d with diff: %lf\n", iteration, diff); 

    // return diff;
}

