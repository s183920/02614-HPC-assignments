#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// helper functions
void swap_3d(double ****X, double ****Y){
    double ***temp = *X;
    *X = *Y;
    *Y = temp;
}

double calc_step_size(int N){
    return 2.0 / (N+1);
}

// init functions
void init_F(double ***F, double N){
    printf("\t- init F\n");
    int i, j, k , k_upr, j_upr, i_lwr, i_upr;
    double step_size = calc_step_size(N);
    
    k_upr = (int)((1.0 - 3.0/8.0) / step_size);
    j_upr = (int)((1.0 - 1.0/2.0) / step_size);
    i_lwr = (int)ceil(((1.0 - 2.0/3.0) / step_size));
    i_upr = (int)(1.0 / step_size);
    

    for (i = i_lwr; i <= i_upr; i++){
        for (j = 0; j <= j_upr; j++){
            for (k = 0; k <= k_upr; k++){
                F[i][j][k] = 200;
            }
        }
    }
}

void init_U(double ***U, double N, double start_T){
    int i, j, k;
    printf("\t- init U\n");
    for (i = 1; i <= N; i++){
        for (j = 1; j <= N; j++){
            for (k = 1; k <= N; k++){
                U[i][j][k] = start_T;
            }
        }
    }

    for (i = 0; i <= N + 1; i++){
        for (k = 0; k <= N + 1; k++){
            U[i][0][k] = 0;
        }
    }
}

void init_constant(double ***F, double ***U, int N){
    // int i, j, k;
    double x, y, z, step_size;
    x = y = z = 0;
    step_size = calc_step_size(N);
    printf("\t- Constant init\n");
    #pragma omp parallel for \
            private(x, y, z) \
            shared(F, U, N, step_size)
    for (int i = 0; i < N+2; i++){
        z = -1.0 + step_size*i;
        for (int j = 0; j < N+2; j++){
            y = -1.0 + step_size*j;
            for (int k = 0; k < N+2; k++){
                x = -1.0 + step_size*k;

                F[i][j][k] = 0;
                U[i][j][k] = 20;
            }
        }
    }
}

void init_grid_matrices(double ***F, double ***U, int N, double start_T){
    printf("Initializing grid matrices for program\n");
    init_constant(F, U, N);
    init_F(F, N);
    init_U(U, N, start_T);
}


// init functions for test case
double f(double x, double y, double z) {
    // printf("Testing program");
    return 3.0*M_PI*M_PI*sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
}
double u(double x, double y, double z) {
    return 0.0;
}

void init_grid_matrices_test(double ***F, double ***U, int N, double start_T){
    printf("Initializing grid matrices for test\n");
    // double step_size = calc_step_size(N)
    double x, y, z, step_size;
    x = y = z = 0;    // double step_size = calc_step_size(N)
    step_size = calc_step_size(N);
    for (int i = 0; i <= N+1; i++){
        z = -1.0 + step_size*i;
        for (int j = 0; j <= N+1; j++){
            y = -1.0 + step_size*j;
            for (int k = 0; k <= N+1; k++){
                x = -1.0 + step_size*k;

                F[i][j][k] = f(x,y,z);
                U[i][j][k] = u(x,y,z);
            }
        }
    }
}