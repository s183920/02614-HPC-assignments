#ifndef _MATMULT_H
#define _MATMULT_H

// helper functions 
int min(int a, int b);
double **init_C(double **C, int m, int n);

// standard OMP versions
void matmult_mkn_omp(int m,int n,int k,double **A,double **B,double **C);
void matmult_blk_omp(int m,int n,int k,double **A,double **B,double **C, int bs);
void matmult_lib(int m,int n,int k,double **A,double **B,double **C);


// offload versions
void matmult_mkn_offload(int m,int n,int k,double **A,double **B,double **C);
void matmult_mnk_offload(int m,int n,int k,double **A,double **B,double **C);

// blocked offload
void matmult_blk_offload(int m,int n,int k,double **A,double **B,double **C);

// asynchroneous offload
void matmult_mkn_asy_offload(int m,int n,int k,double **A,double **B,double **C);
#endif