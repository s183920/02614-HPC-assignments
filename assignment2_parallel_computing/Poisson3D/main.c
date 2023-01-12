/* main.c - Poisson problem in 3D
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "alloc3d.h"
#include "print.h"
#include "helper.h"
#include <math.h>

#ifdef _JACOBI
#include "jacobi.h"
#define solver jacobi
#endif

#ifdef _GAUSS_SEIDEL
#include "gauss_seidel.h"
#define solver gauss_seidel
#endif

#define N_DEFAULT 100

int
main(int argc, char *argv[]) {

    int 	N = N_DEFAULT;
    int 	iter_max = 1000;
    double	tolerance;
    double	start_T;
    int		output_type = 0;
    char	*output_prefix = "poisson_res";
    char        *output_ext    = "";
    char	output_filename[FILENAME_MAX];
    double 	***U = NULL;
    double ***F = NULL;
    // double ***U_true = NULL;


    /* get the paramters from the command line */
    N         = atoi(argv[1]);	// grid size
    iter_max  = atoi(argv[2]);  // max. no. of iterations
    tolerance = atof(argv[3]);  // tolerance
    start_T   = atof(argv[4]);  // start T for all inner grid points
    if (argc == 6) {
	output_type = atoi(argv[5]);  // ouput type
    }

    // allocate memory
    if ( (U = malloc_3d(N+2,N+2,N+2)) == NULL ) {
        perror("array U: allocation failed");
        exit(-1);
    }
    if ( (F = malloc_3d(N+2,N+2,N+2)) == NULL ) {
        perror("array F: allocation failed");
        exit(-1);
    }

    
    // double x,y,z;
    double step_size = 2.0 / ((double)N+1);
    init_grid_matrices(F, U, N);

    // for (int i = 0; i <= N+1; i++){
    //     z = -1.0 + step_size*i;
    //     for (int j = 0; j <= N+1; j++){
    //         y = -1.0 + step_size*j;
    //         for (int k = 0; k <= N+1; k++){
    //             x = -1.0 + step_size*k;
                
    //             F[i][j][k] = f(x,y,z);
    //             // u_true[i][j][k] = sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
    //             U[i][j][k] = 0;

    //             // printf("i: %d, j: %d, k:%d. x: %lf, y: %lf, z: %lf\n", i, j, k, x, y, z);
    //             // u[i+1][j+1][k+1] = start_T;
    //             // f[i][j][k] = 
    //         }
    //     }
    // }

    // solver(N, tolerance, iter_max, U, F, step_size);

    #ifdef _JACOBI
    printf("Using Jacobi\n");
    double ***U_new = NULL;
    if ( (U_new = malloc_3d(N+2,N+2,N+2)) == NULL ) {
        perror("array u: allocation failed");
        exit(-1);
    }
    jacobi(N, tolerance, iter_max, U, U_new, F, step_size);
    #endif

    #ifdef _GAUSS_SEIDEL
    printf("Using Gauss-Seidel\n");
    gauss_seidel(N, tolerance, iter_max, U, F, step_size);
    #endif


    #ifdef _TEST
    output_prefix = "../test_res/test";
    double err = test_case(N, U);
    printf("Set tolerance: %lf\n", tolerance);
    // printf("Obtained tolerance: %lf\n", tol);
    printf("Error: %lf\n", err);
    printf("Pct diff: %lf\n", (err - tolerance)/tolerance);
    #endif


    printf("End of running\n");

    // print_binary("f.txt", N+2, F);
    // print_binary("u_true.txt", N+2, U_true);
    // print_binary("u.txt", N+2, U);
    /*
     *
     * fill in your code here 
     *
     *
     */

    // dump  results if wanted 
    switch(output_type) {
	case 0:
	    // no output at all
	    break;
	case 3:
	    output_ext = ".bin";
	    sprintf(output_filename, "%s_%d%s", output_prefix, N, output_ext);
	    fprintf(stderr, "Write binary dump to %s: ", output_filename);
	    print_binary(output_filename, N+2, U);
	    break;
	case 4:
	    output_ext = ".vtk";
	    sprintf(output_filename, "%s_%d%s", output_prefix, N, output_ext);
	    fprintf(stderr, "Write VTK file to %s: ", output_filename);
	    print_vtk(output_filename, N+2, U);
	    break;
	default:
	    fprintf(stderr, "Non-supported output type!\n");
	    break;
    }

    // de-allocate memory
    free_3d(U);
    free_3d(F);

    return(0);
}



// number_of_iteration error wall_time cpu_time 