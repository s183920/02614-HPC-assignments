/* main.c - Poisson problem in 3D
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "alloc3d.h"
#include "print.h"
#include "helper.h"
#include <math.h>

// get matrix init
#ifdef _TEST
#include "matrix_init_test.h"
#else
#include "matrix_init.h"
#endif

// get solver function
#ifdef _JACOBI
#include "jacobi.h"
// function for solving the Poisson problem
void solver(int N, double tolerance, int iter_max, double ***U, double ***F, double step_size){
    double ***U_new = NULL;
    if ( (U_new = malloc_3d(N+2,N+2,N+2)) == NULL ) {
        perror("array u: allocation failed");
        exit(-1);
    }
    jacobi(N, tolerance, iter_max, U, U_new, F, step_size);
}
#endif

#ifdef _GAUSS_SEIDEL
#include "gauss_seidel.h"
// function for solving the Poisson problem
void solver(int N, double tolerance, int iter_max, double ***U, double ***F, double step_size){
    gauss_seidel(N, tolerance, iter_max, U, F, step_size);
}
#endif

// timer function
#ifdef _OPENMP
#include <omp.h>
#define mytimer omp_get_wtime
#define delta_t(a,b) (1e3 * ((b)-(a)))
#else
#include <time.h>
#define mytimer clock
#define delta_t(a,b) (1e3 * ((b) - (a)) / CLOCKS_PER_SEC)
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

    // SETUP TIMER
    #ifdef _OPENMP
    double t1, t2;
    fprintf(stderr, "OpenMP version: timing wallclock time (in ms)!\n");
    #else
    clock_t t1, t2;
    fprintf(stderr, "Serial version: timing CPU time (in ms)!\n");
    #endif

    
    // get step size
    double step_size = calc_step_size(N);

    // init matrices
    init_grid_matrices(F, U, N);

    // print setup
    printf("Setup \n");
    printf("\tN: %d\n", N);
    printf("\tIter_max: %d\n", iter_max);
    printf("\tTolerance: %lf\n", tolerance);
    printf("\tStart_T: %lf\n", start_T);
    printf("\tStep_size: %lf\n", step_size);

    // print results
    printf("Results\n");

    // solve 
    t1 = mytimer();
    solver(N, tolerance, iter_max, U, F, step_size);
    t2 = mytimer();

    // print time results
    printf("\tTime: %lf\n", delta_t(t1,t2));

    // print error if testing
    #ifdef _TEST
    output_prefix = "../test_res/test";
    double err = test_case(N, U);
    // printf("Obtained tolerance: %lf\n", tol);
    printf("\tError: %lf\n", err);
    // printf("Pct diff: %lf\n", (err - tolerance)/tolerance);
    #endif

    



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