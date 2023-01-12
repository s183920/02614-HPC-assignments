/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "helper.h"

double gauss_seidel_update(double ***U, double ***F, int i, int j, int k, double gauss_scale, double delta2){
    return gauss_scale * (
        U[i-1][j][k] + U[i+1][j][k] +  U[i][j-1][k] + U[i][j+1][k] + U[i][j][k-1] + U[i][j][k+1] + 
        delta2* F[i][j][k]
    );
}

void gauss_seidel(int N, double threshold, int iter_max, double ***U, double ***F, double delta) {
    // constants
    double gauss_scale = 1.0/6.0; // d in slides
    double norm_scale = 1./(N*N*N); // normalisation
    double delta2 = delta*delta;

    // loop vars
    int i, j, k;
    int iteration = 0;

    // convergence
    double convergence = INFINITY;
    while (convergence > threshold && iteration < iter_max) {
        convergence = 0;
        for (i = 1; i <= N ; i++) {
            for (j = 1; j <= N; j++) {
                for (k = 1; k <= N; k++) {
                    double new_u = gauss_seidel_update(U, F, i, j, k, gauss_scale, delta2);
                    convergence += (new_u - U[i][j][k])*(new_u - U[i][j][k]);
                    U[i][j][k] = new_u;
                }
            }
        }
        convergence = sqrt(norm_scale * convergence);
        iteration++;
    }

    // print results
    printf("\tIterations: %d\n", iteration);
    printf("\tConvergence_difference: %lf\n", convergence);
}


// parallel

// obtained error not the same, convergence difference ignored as this is a bottleneck
// error seems to be influenced by calculation of convergence difference
void gauss_seidel_para_simpel(int N, double threshold, int iter_max, double ***U, double ***F, double delta) {
    // constants
    double gauss_scale = 1.0/6.0; // d in slides
    double norm_scale = 1./(N*N*N); // normalisation
    double delta2 = delta*delta;

    // loop vars
    int i, j, k;
    int iteration;

    // convergence
    double convergence = INFINITY;
    
    for (iteration = 0; iteration < iter_max; iteration++) {
        convergence = 0;
        #pragma omp parallel for schedule(static,1) ordered(2) private(i,j,k)
        for (i = 1; i <= N ; i++) {
            for (j = 1; j <= N; j++) {
                #pragma omp ordered depend(sink: i-1,j) depend(sink: i,j-1) 
                for (k = 1; k <= N; k++) {
                    double new_u = gauss_seidel_update(U, F, i, j, k, gauss_scale, delta2);
                    // #pragma omp atomic
                    // convergence += (new_u - U[i][j][k])*(new_u - U[i][j][k]);
                    U[i][j][k] = new_u;
                }
                #pragma omp ordered depend(source)
            }
        } // end parallel
        // convergence = sqrt(norm_scale * convergence);
        iteration++;
    }

    // print results
    printf("\tIterations: %d\n", iteration);
    printf("\tConvergence_difference: %lf\n", convergence);
}


// parallel opt
void
gauss_seidel_para_opt(int N, double threshold, int iter_max, double ***U, double ***F, double delta) {
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
