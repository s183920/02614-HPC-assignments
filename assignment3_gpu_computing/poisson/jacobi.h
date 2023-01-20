#ifndef _JACOBI_H
#define _JACOBI_H

void jacobi_map(int N, double threshold, int iter_max, double ***U_new, double ***U_old, double ***F, double delta);
//void jacobi_para_simpel(int N, double threshold, int iter_max, double ***U_new, double ***U_old, double ***F, double delta);
void jacobi_para_opt(int N, double threshold, int iter_max, double ***U_new, double ***U_old, double ***F, double delta);
void jacobi_GPU(int N, double threshold, int iter_max, double ***U_d, double ***U_new_d, double ***F_d, double delta);
void jacobi_dual_GPU(int N, double threshold, int iter_max, double ***U_d, double ***U_new_d, double ***F_d, 
                     double ***U_d1, double ***U_new_d1, double ***F_d1,double delta);
void jacobi_map_norm(int N, double threshold, int iter_max, double ***U_old, double ***U_new, double ***F, double delta);
#endif