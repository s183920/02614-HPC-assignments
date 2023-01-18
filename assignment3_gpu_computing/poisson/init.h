#ifndef _INIT_H_
#define _INIT_H_

// helper functions 
void swap_3d(double ****X, double ****Y);
double calc_step_size(int N);

// init functions
void init_F(double ***F, double N);
void init_U(double ***U, double N, double start_T);
void init_constant(double ***F, double ***U, int N);
void init_grid_matrices(double ***F, double ***U, int N, double start_T);

// init functions for testing
double f(double x, double y, double z);
double u(double x, double y, double z);
void init_grid_matrices_test(double ***F, double ***U, int N, double start_T);

#endif