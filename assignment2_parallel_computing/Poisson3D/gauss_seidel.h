/* gauss_seidel.h - Poisson problem
 *
 */
#ifndef _GAUSS_SEIDEL_H
#define _GAUSS_SEIDEL_H

// define your function prototype here
// gauss_seidel(...);
void
gauss_seidel(int N, double threshold, int iter_max, double ***U, double ***f, double delta);

void
gauss_seidel_para_simpel(int N, double threshold, int iter_max, double ***U, double ***f, double delta);

void
gauss_seidel_para_opt(int N, double threshold, int iter_max, double ***U, double ***f, double delta);

#endif
