#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES

// double M_PI = 3.14159265359;
void init_F(double*** F, double step_size);
double calc_step_size(int N);

int main(int argc, char const *argv[])
{
    int N;
    double ***F = NULL;
    N         = atoi(argv[1]);
    init_F(F, N);
    return 0;
}


void init_F(double*** F, double N){
    int i_upr, j_upr, k_lwr, k_upr;
    double step = calc_step_size(N);
    
    i_upr = (int)((1.0 - 3.0/8.0) / step);
    j_upr = (int)((1.0 - 1.0/2.0) / step);
    k_lwr = (int)ceil(((1.0 - 2.0/3.0) / step));
    k_upr = (int)(1.0 / step);
    
    for (int i = 0; i <= i_upr; i++){
        for (int j = 0; j <= j_upr; j++){
            for (int k = k_lwr; k <= k_upr; k++){
                F[i][j][k] = 200
            }
        }
        printf("%lf ", -1.0 + step*i );
    }
    printf("\n\n i: \n");
    printf("%d\n", i_upr);
    printf("%lf\n", -3.0/8.0);
        printf("%d\n", j_upr);
    printf("%lf\n", -1.0/2.0);
    printf("%d\n", k_lwr);
    printf("%lf\n", -2.0/3.0);
    printf("\n\n k_upr:\n");
    printf("%d\n", k_upr);
    printf("%lf\n", 0.0);
    
}


double calc_step_size(int N){
    return 2.0 / (N+1);
}