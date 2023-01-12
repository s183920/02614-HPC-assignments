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
    int iteration = 0;

    while (diff > threshold && iteration < iter_max) {
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


// parallel version
void
gauss_seidel_para_simpel(int N, double threshold, int iter_max, double ***U, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;

    double diff_tmp;
    double diff = INFINITY;
    double diff_scale = 1./(N*N*N);
    int iteration = 0;

    #pragma omp parallel shared(iteration,diff,delta, scale, diff_scale) private(diff_tmp)
    {
    while (diff > threshold && iteration < iter_max) {
        diff = 0;
        #pragma omp for ordered(2) schedule(static,1)
        for (i = 1; i <= N ; i++) {
            for (j = 1; j <= N; j++) {
                #pragma omp ordered depend(sink: i-1,j) depend(sink: i,j-1) 
                for (k = 1; k <= N; k++) {
                    // double tmp1 = (U[i-1][j-1][k-1] + U[i-1][j-1][k] + U[i-1][j-1][k+1]
                    //     + U[i-1][j][k-1] + U[i-1][j][k] + U[i-1][j][k+1]
                    //     + U[i-1][j+1][k-1] + U[i-1][j+1][k] + U[i-1][j+1][k+1]);
                    // double tmp2 = (U[i][j-1][k-1] + U[i][j-1][k] + U[i][j-1][k+1]
                    //     + U[i][j][k-1] + U[i][j][k] + U[i][j][k+1]
                    //     + U[i][j+1][k-1] + U[i][j+1][k] + U[i][j+1][k+1]);
                    // double tmp3 = (U[i+1][j-1][k-1] + U[i+1][j-1][k] + U[i+1][j-1][k+1]
                    //     + U[i+1][j][k-1] + U[i+1][j][k] + U[i+1][j][k+1]
                    //     + U[i+1][j+1][k-1] + U[i+1][j+1][k] + U[i+1][j+1][k+1]);
                    // diff_tmp = (tmp1 + tmp2 + tmp3) / 27.0;
                    diff_tmp = scale * ( // should this be initialised here when parallelising
                        U[i-1][j][k] + 
                        U[i+1][j][k] + 
                        U[i][j-1][k] + 
                        U[i][j+1][k] + 
                        U[i][j][k-1] + 
                        U[i][j][k+1] + 
                        delta * delta * F[i][j][k]);
                    #pragma omp atomic
                    diff += (diff_tmp - U[i][j][k])*(diff_tmp - U[i][j][k]);
                    U[i][j][k] = diff_tmp;
                }
                #pragma omp ordered depend(source)
            }
        } // end ordered
        #pragma omp barrier
        #pragma omp master
        {
        diff = sqrt(diff_scale *diff);
        iteration++;
        }
        #pragma omp barrier
        // diff = sqrt(diff_scale *diff);
        // iteration++;
    }
    } // end parallel

    printf("\tIterations: %d\n", iteration);
    printf("\tConvergence_difference: %lf\n", diff);
}


// void gauss_seidel_para_simpel(int N, double threshold, int iter_max, double ***U, double ***F, double delta) {
//     int i, j, k;
//     double scale = 1.0/6.0;

//     double diff_tmp;
//     double diff = INFINITY;
//     double diff_scale = 1./(N*N*N);
//     int iteration = 1; // 1-indiced iterations

//     #pragma omp for ordered(2) private(i,j,k) schedule(static,1)
//     for (i = 1; i < N-1; ++i) {
//         for (j = 1; j < N-1; ++j) {
//             #pragma omp ordered depend(sink: i-1,j-1) depend(sink: i-1,j) \
//             depend(sink: i-1,j+1) depend(sink: i,j-1)
//             for (k = 1; k < N-1; ++k) {
//                 double tmp1 = (U[i-1][j-1][k-1] + U[i-1][j-1][k] + U[i-1][j-1][k+1]
//                 + U[i-1][j][k-1] + U[i-1][j][k] + U[i-1][j][k+1]
//                 + U[i-1][j+1][k-1] + U[i-1][j+1][k] + U[i-1][j+1][k+1]);
//                 double tmp2 = (U[i][j-1][k-1] + U[i][j-1][k] + U[i][j-1][k+1]
//                 + U[i][j][k-1] + U[i][j][k] + U[i][j][k+1]
//                 + U[i][j+1][k-1] + U[i][j+1][k] + U[i][j+1][k+1]);
//                 double tmp3 = (U[i+1][j-1][k-1] + U[i+1][j-1][k] + U[i+1][j-1][k+1]
//                 + U[i+1][j][k-1] + U[i+1][j][k] + U[i+1][j][k+1]
//                 + U[i+1][j+1][k-1] + U[i+1][j+1][k] + U[i+1][j+1][k+1]);

//                 diff_tmp = (tmp1 + tmp2 + tmp3) / 27.0;
//                 diff_tmp = (diff_tmp + delta * delta * F[i][j][k]) * scale;


//                 diff += (diff_tmp - U[i][j][k])*(diff_tmp - U[i][j][k]);
//                 U[i][j][k] = diff_tmp;
//             }
//             #pragma omp ordered depend(source)
//         }
//     }
// }



// void gauss_seidel_para_simpel(int N, double threshold, int iter_max, double ***u, double ***F, double delta){
//     int n = 0;
//     int i, j, k;
//     // double delta = 2.0/((double)N-1.0);
//     double delta2 = delta*delta;
//     double div = 1.0/6.0;
//     int iterations = 267;
//     double tolerance = threshold;
    
//     #pragma omp parallel default(none) private(n) \
//          shared(delta2, u, N, tolerance, F, iterations, div)
//     {
//     for (n = 0; n < iterations; n++){
//         //Default schedule would be (static, N/P), with N work and P threads
//         #pragma omp for private(i,j,k) ordered(2) schedule(static,1)
//         for(i = 1; i < (N - 1); i++){
//             for(j = 1; j < (N - 1); j++){
//                 #pragma omp ordered depend(sink:i-1,j) depend(sink:i,j-1)
//                 for(k = 1; k < (N - 1); k++){
//                     u[i][j][k] = div * (
//                         u[i-1][j][k] + u[i+1][j][k] + 
//                         u[i][j-1][k] + u[i][j+1][k] + 
//                         u[i][j][k-1] + u[i][j][k+1] + 
//                         delta2 * F[i][j][k]
//                     );
//                 }
//                 #pragma omp ordered depend(source) /*Ending ordered*/
//             }
//         }/*End of for ordered*/
//     }
//     }/*End of parallel*/
// }

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
