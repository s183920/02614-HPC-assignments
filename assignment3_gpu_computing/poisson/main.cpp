/* main.c - Poisson problem in 3D
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "alloc3d.h"
#include "print.h"
#include "init.h"
#include <math.h>
#include <omp.h>

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


// get solver function
#include "jacobi.h"
// function for solving the Poisson problem
double solver(int version, int N, double tolerance, int iter_max, double ***U, double ***F,double ***U_d, 
              double ***F_d, double *data_u_d, double *data_f_d, double step_size){
    double ***U_new = NULL;
    if ( (U_new = malloc_3d(N+2,N+2,N+2)) == NULL ) {
        perror("array U_new: allocation failed");
        exit(-1);
    }
    
    #pragma omp parallel for shared(U_new, N)
    for (int i = 0; i <= N+1; i++){
        for (int j = 0; j <= N+1; j++){
            for (int k = 0; k <= N+1; k++){
                U_new[i][j][k] = U[i][j][k];
            }
        }
    }

    // SETUP TIMER
    #ifdef _OPENMP
    double t1, t2;
    fprintf(stderr, "OpenMP version: timing wallclock time (in ms)!\n");
    #else
    clock_t t1, t2;
    fprintf(stderr, "Serial version: timing CPU time (in ms)!\n");
    #endif
    
    t1 = mytimer();
    if (version == 0){
        jacobi_map(N, tolerance, iter_max, U, U_new, F, step_size);
    } else if (version == 1){
        jacobi_para_opt(N, tolerance, iter_max, U, U_new, F, step_size);
    } else if (version == 4){
        jacobi_map_norm(N, tolerance, iter_max, U, U_new, F, step_size);
    } else if (version == 2){
        double ***U_new_d = NULL;
        double *data_un_d = NULL;
        if ( (U_new_d = d_malloc_3d(N+2,N+2,N+2, &data_un_d)) == NULL ) {
            perror("array U_new_d: allocation failed");
            exit(-1);
        }

        //#pragma omp target data map(to: U_d[0:N+1][0:N+1][0:N+1]) map(tofrom: U_new_d[0:N+1][0:N+1][0:N+1])
        //{
        //#pragma omp target teams distribute parallel for collapse(3) 
    
        /* initialize U_new, U_old and F on host */
        omp_target_memcpy(data_un_d, U_new[0][0], (N+2) * (N+2) * (N+2) * sizeof(double), 0, 0, omp_get_default_device(), omp_get_initial_device());
        omp_target_memcpy(data_u_d, U[0][0], (N+2) * (N+2) * (N+2) * sizeof(double), 0, 0, omp_get_default_device(), omp_get_initial_device());
        omp_target_memcpy(data_f_d, F[0][0], (N+2) * (N+2) * (N+2) * sizeof(double), 0, 0, omp_get_default_device(), omp_get_initial_device());
        double t1_wot, t2_wot; 
        t1_wot = omp_get_wtime();
        jacobi_GPU(N, tolerance, iter_max, U_d, U_new_d, F_d, step_size);
        t2_wot = omp_get_wtime();
        omp_target_memcpy(U_new[0][0], data_un_d, (N+2) * (N+2) * (N+2) * sizeof(double), 0, 0, omp_get_initial_device(), omp_get_default_device());
        printf("\tTime w/o data transfer: %lf\n", delta_t(t1_wot, t2_wot));
        d_free_3d(U_d, data_u_d);
        d_free_3d(U_new_d, data_un_d);
        d_free_3d(F_d, data_f_d);
    }
    
    else {
        perror("Error: version not supported");
        exit(-1);
    }
    t2 = mytimer();
    return delta_t(t1,t2);
}

#define N_DEFAULT 100

int
main(int argc, char *argv[]) {

    int 	N = N_DEFAULT;
    int 	iter_max = 1000;
    double	tolerance;
    double	start_T;
    int		output_type = 0;
    char	*output_prefix = "poisson_res";
    char    *output_ext    = "";
    char	output_filename[FILENAME_MAX];
    double 	***U = NULL;
    double  ***F = NULL;
    double  ***U_d = NULL;
    double  ***F_d = NULL;
    double  *data_u_d = NULL;
    double  *data_f_d = NULL;
    int version = 0;
    // double ***U_true = NULL;


    /* get the paramters from the command line */
    N         = atoi(argv[1]);	// grid size
    iter_max  = atoi(argv[2]);  // max. no. of iterations
    tolerance = atof(argv[3]);  // tolerance
    start_T   = atof(argv[4]);  // start T for all inner grid points
    if (argc == 6) {
	output_type = atoi(argv[5]);  // ouput type
    }
    if (argc == 7) {
        version = atoi(argv[6]);
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
    if ( (U_d = d_malloc_3d(N+2,N+2,N+2, &data_u_d)) == NULL ) {
            perror("array U_d: allocation failed");
            exit(-1);
    }
    if ( (F_d = d_malloc_3d(N+2,N+2,N+2, &data_f_d)) == NULL ) {
        perror("array F_d: allocation failed");
        exit(-1);
    }

    // get step size
    double step_size = calc_step_size(N);

    // init matrices
    init_grid_matrices(F, U, N, start_T);

    // print setup
    printf("Setup \n");
    char *version_name;
    switch(version) {
    case 0:
        version_name = "Map";
        break;
    case 1:
        version_name = "Optimized parallel";
        break;
    case 2:
        version_name = "GPU";
        break;
    case 3:
        version_name = "Multiple GPU";
        break;
    case 4:
        version_name = "Map with norm calc";
        break;
    }
    printf("\tMethod: Jacobi (%s)\n", version_name);
    printf("\tN: %d\n", N);
    printf("\tIter_max: %d\n", iter_max);
    printf("\tTolerance: %lf\n", tolerance);
    printf("\tStart_T: %lf\n", start_T);
    printf("\tStep_size: %lf\n", step_size);

    // print results
    printf("Results\n");

    // solve 
    // t1 = mytimer();
    // solve
    double time = solver(version, N, tolerance, iter_max, U, F, U_d, F_d, data_u_d, data_f_d, step_size);
    // t2 = mytimer();

    // print time results
    printf("\tTime: %lf\n", time);

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
    // if (version > 1) {
    //     d_free_3d(U_d, data);
    //     d_free_3d(F_d, data);
    // }
    return(0);
}