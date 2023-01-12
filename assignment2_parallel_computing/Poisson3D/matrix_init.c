#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"


// #ifdef _TEST
// double f(double x, double y, double z) {
//     // printf("Testing program");
//     return 3.0*M_PI*M_PI*sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
// }
// double u(double x, double y, double z) {
//     return 0
// }

// void init_grid_matrices(double ***F, double ***U, int N){
//     printf("Initializing grid matrices for test\n");
//     double step_size = calc_step_size(N)
//     for (int i = 0; i <= N+1; i++){
//         z = -1.0 + step_size*i;
//         for (int j = 0; j <= N+1; j++){
//             y = -1.0 + step_size*j;
//             for (int k = 0; k <= N+1; k++){
//                 x = -1.0 + step_size*k;

//                 F[i][j][k] = f(x,y,z);
//                 U[i][j][k] = u(x,y,z);
//             }
//         }
//     }
// }

// #else

void init_F(double ***F, double N){
    printf("\t- init F\n");
    int i, j, k , i_upr, j_upr, k_lwr, k_upr;
    double step_size = calc_step_size(N);
    
    i_upr = (int)((1.0 - 3.0/8.0) / step_size);
    j_upr = (int)((1.0 - 1.0/2.0) / step_size);
    k_lwr = (int)ceil(((1.0 - 2.0/3.0) / step_size));
    k_upr = (int)(1.0 / step_size);
    
    for (i = 0; i <= i_upr; i++){
        for (j = 0; j <= j_upr; j++){
            for (k = k_lwr; k <= k_upr; k++){
                F[i][j][k] = 200;
            }
        }
    }
}

void init_U(double ***U, double N){
    int i, j, k;
    printf("\t- init U\n");
    for (i = 1; i <= N; i++){
        for (j = 1; j <= N; j++){
            for (k = 1; k <= N; k++){
                U[i][j][k] = 0;
            }
        }
    }

    for (i = 0; i <= N + 1; i++){
        for (k = 0; k <= N + 1; k++){
            U[i][0][k] = 0;
        }
    }
    
    // double step_size = calc_step_size(N);
    
    // i_upr = (int)((1.0 - 3.0/8.0) / step_size);
    // j_upr = (int)((1.0 - 1.0/2.0) / step_size);
    // k_lwr = (int)ceil(((1.0 - 2.0/3.0) / step_size));
    // k_upr = (int)(1.0 / step_size);
    
    // for (i = 0; i <= i_upr; i++){
    //     for (j = 0; j <= j_upr; j++){
    //         for (k = k_lwr; k <= k_upr; k++){
    //             U[i][j][k] = 200;
    //         }
    //     }
    // }
}

void init_constant(double ***F, double ***U, double N){
    int i, j, k;
    double x, y, z, step_size;
    x = y = z = 0;
    step_size = calc_step_size(N);
    printf("\t- Constant init\n");
    for (i = 0; i <= N+1; i++){
        z = -1.0 + step_size*i;
        for (j = 0; j <= N+1; j++){
            y = -1.0 + step_size*j;
            for (k = 0; k <= N+1; k++){
                x = -1.0 + step_size*k;

                F[i][j][k] = 0;
                U[i][j][k] = 20;
            }
        }
    }
}

void init_grid_matrices(double ***F, double ***U, int N){
    printf("Initializing grid matrices for program\n");
    init_constant(F, U, N);
    init_F(F, N);
    init_U(U, N);
}

// #endif

