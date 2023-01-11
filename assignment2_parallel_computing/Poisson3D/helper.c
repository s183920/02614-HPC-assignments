#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _TEST
double f(double x, double y, double z) {
    // printf("Testing program");
    return -3.0*M_PI*M_PI*sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
}
#else
double f(double x, double y, double z) {
    // printf("Running program");
    return -3.0*M_PI*M_PI*sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
}
#endif

// swap function
void swap_3d(double ****X, double ****Y){
    double ***temp = *X;
    *X = *Y;
    *Y = temp;
}

// test case
double test_case(int N, double ***U){
    double u_true;
    double diff = 0;
    double x,y,z;
    double step_size = 2.0 / ((double)N+1);

    for (int i = 1; i <= N; i++){
        z = -1.0 + step_size*i;
        for (int j = 1; j <= N; j++){
            y = -1.0 + step_size*j;
            for (int k = 1; k <= N; k++){
                x = -1.0 + step_size*k;

                u_true = sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
                diff += (U[i][j][k] - u_true)*(U[i][j][k] - u_true);
            }
        }
    }
    return sqrt(diff/(N*N*N));
}
