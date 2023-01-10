/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <stdio.h>
#include <math.h>

void
gauss_seidel(int N, double threshold, int iter_max, double ***U, double ***f, double delta) {
    // fill in your code here
    int i, j, k;
    int M = N + 2;
    double d = 1.0/6.0;

    double tmp;
    double diff = 100000000000000;
    int iteration = 0;
    double threshold_cubed = threshold * threshold * threshold;

    while (diff > threshold_cubed && iteration < iter_max) {
        diff = 0;
        for (i = 1; i < M - 1; i++) {
            for (j = 1; j < M - 1; j++) {
                for (k = 1; k < M - 1; k++) {
                    tmp = d * (U[i - 1][j][k] + U[i + 1][j][k] + 
                                    U[i][j - 1][k] + U[i][j + 1][k] + 
                                    U[i][j][k - 1] + U[i][j][k + 1] + 
                                    delta * delta * f[i][j][k]);
                    //printf("f: %lf\n", d);
                    diff += (tmp - U[i][j][k]) * (tmp - U[i][j][k]) * (tmp - U[i][j][k]);
                    U[i][j][k] = tmp;
                }
            }
        }
        iteration++;
        
    }
    printf("Number of iterations: %d with diff: %f", iteration, diff); 
}

