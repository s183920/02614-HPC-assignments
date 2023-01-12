#ifndef _MATRIX_INIT_H
#define _MATRIX_INIT_H

// #ifndef _TEST


// // double f(double x, double y, double z);

// // #else

// // Init special case for U (Cold wall)
// void init_U(double*** F, double N);

// // Init special case for F (radiator)
// void init_F(double*** F, double N);

// // Init F and U with constant according to assignment
// void init_constant(double*** F, double*** U, double step_size);



// #endif

// Initializes F and U as NxNxN tensors
void init_grid_matrices(double ***F, double ***U, int N);

#endif