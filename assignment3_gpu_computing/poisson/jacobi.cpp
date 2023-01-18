/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdio.h>

void swap_3d(double ****X, double ****Y){
    // printf("Adress of ***X: %p\nAdress of ***Y: %p\n",*X,*Y);
    double ***temp = *X;
    *X = *Y;
    *Y = temp;
    // printf("\nNew Adress of ***X: %p\nNew Adress of ***Y: %p\n",*X,*Y);


}

void
jacobi(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta) {
    int i, j, k;
    double scale = 1.0/6.0;

    double diff = INFINITY;
    double diff_scale = 1./(N*N*N);
    int iteration = 0;     
    

    while (diff > threshold && iteration < iter_max) {
        diff = 0;
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


        iteration++;
    }
    swap_3d(&U_old, &U_new);
    
    printf("\tIterations: %d\n", iteration);
    printf("\tConvergence_difference: %lf\n", diff);
}