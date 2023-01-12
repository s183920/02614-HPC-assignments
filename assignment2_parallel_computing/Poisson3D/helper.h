#ifndef _HELPER_H
#define _HELPER_H


// function for f
double f(double x, double y, double z);

// swap tensors
void swap_3d(double ****X, double ****Y);

// Init special case for U (Cold wall)
void init_U(double*** F, double N);

// Init special case for F (radiator)
void init_F(double*** F, double N);

// Initializes F and U as NxNxN tensors
void init_grid_matrices(double*** F, double ***U, int N);

// Init F and U with constant according to assignment
void init_constant(double*** F, double*** U, double step_size);



double calc_step_size(int N);

// test case
double test_case(int N, double ***U);



#endif