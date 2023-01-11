/* main.c - Poisson problem in 3D
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "alloc3d.h"
#include "print.h"
#include <math.h>

#ifdef _JACOBI
#include "jacobi.h"
#endif

#ifdef _GAUSS_SEIDEL
#include "gauss_seidel.h"
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
    double 	***u = NULL;
    double ***f = NULL;
    double ***u_true = NULL;


    /* get the paramters from the command line */
    N         = atoi(argv[1]);	// grid size
    iter_max  = atoi(argv[2]);  // max. no. of iterations
    tolerance = atof(argv[3]);  // tolerance
    start_T   = atof(argv[4]);  // start T for all inner grid points
    if (argc == 6) {
	output_type = atoi(argv[5]);  // ouput type
    }

    // allocate memory
    if ( (u = malloc_3d(N+2,N+2,N+2)) == NULL ) {
        perror("array u: allocation failed");
        exit(-1);
    }
    if ( (f = malloc_3d(N+2,N+2,N+2)) == NULL ) {
        perror("array f: allocation failed");
        exit(-1);
    }
    if ( (u_true = malloc_3d(N+2,N+2,N+2)) == NULL ) {
        perror("array u: allocation failed");
        exit(-1);
    }

    
    double x,y,z;
    double step_size = 2.0 / ((double)N+1);

    for (int i = 0; i <= N+1; i++){
        z = -1.0 + step_size*i;
        for (int j = 0; j <= N+1; j++){
            y = -1.0 + step_size*j;
            for (int k = 0; k <= N+1; k++){
                x = -1.0 + step_size*k;
                
                f[i][j][k] = -3.0*M_PI*M_PI*sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
                u_true[i][j][k] = sin(M_PI*x)*sin(M_PI*y)*sin(M_PI*z);
                u[i][j][k] = 0;

                // printf("i: %d, j: %d, k:%d. x: %lf, y: %lf, z: %lf\n", i, j, k, x, y, z);
                // u[i+1][j+1][k+1] = start_T;
                // f[i][j][k] = 
            }
        }
    }

    #ifdef _GAUSS_SEIDEL
    gauss_seidel(N, tolerance, iter_max, u, f, step_size);
    #endif

    print_binary("f.txt", N+2, f);
    print_binary("u_true.txt", N+2, u_true);
    print_binary("u.txt", N+2, u);
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
	    print_binary(output_filename, N, u);
	    break;
	case 4:
	    output_ext = ".vtk";
	    sprintf(output_filename, "%s_%d%s", output_prefix, N, output_ext);
	    fprintf(stderr, "Write VTK file to %s: ", output_filename);
	    print_vtk(output_filename, N, u);
	    break;
	default:
	    fprintf(stderr, "Non-supported output type!\n");
	    break;
    }

    // de-allocate memory
    free_3d(u);
    free_3d(f);

    return(0);
}




