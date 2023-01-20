#ifndef _MATMULT_H
#define _MATMULT_H


// // helper functions 
// int min(int a, int b);
// double **init_C(double **C, int m, int n);
// double **init_C_omp(double **C, int m, int n);

// // standard OMP versions
// void matmult_mkn_omp(int m,int n,int k,double **A,double **B,double **C);
// void matmult_blk_omp(int m,int n,int k,double **A,double **B,double **C, int bs);
// void matmult_lib(int m,int n,int k,double **A,double **B,double **C);


// // offload versions
// void matmult_mkn_offload(int m,int n,int k,double **A,double **B,double **C);
// void matmult_mnk_offload(int m,int n,int k,double **A,double **B,double **C);

// // blocked offload
// void matmult_blk_offload(int m,int n,int k,double **A,double **B,double **C);

// // asynchroneous offload
// void matmult_asy_offload(int m,int n,int k,double **A,double **B,double **C);

// // library offload
// void matmult_lib_offload(int m,int n,int k,double **A,double **B,double **C);


// helper functions 
// extern "C"{
//     int min(int a, int b);
// }
// extern "C"{
//     double **init_C(double **C, int m, int n);
// }
// extern "C"{
//     double **init_C_omp(double **C, int m, int n);
// }

// // standard OMP versions
// extern "C"{
//     void matmult_mkn_omp(int m,int n,int k,double **A,double **B,double **C);
// }
// extern "C"{
//     void matmult_blk_omp(int m,int n,int k,double **A,double **B,double **C, int bs);
// }
// extern "C"{
//     void matmult_lib(int m,int n,int k,double **A,double **B,double **C);
// }

// // offload versions
// extern "C"{
//     void matmult_mkn_offload(int m,int n,int k,double **A,double **B,double **C);
// }
// extern "C"{
//     void matmult_mnk_offload(int m,int n,int k,double **A,double **B,double **C);
// }

// // blocked offload
// extern "C"{
//     void matmult_blk_offload(int m,int n,int k,double **A,double **B,double **C);
// }

// // asynchroneous offload
// extern "C"{
//     void matmult_asy_offload(int m,int n,int k,double **A,double **B,double **C);
// }

// // library offload
// extern "C"{
//     void matmult_lib_offload(int m,int n,int k,double **A,double **B,double **C);
// }



int min(int a, int b);
double **init_C(double **C, int m, int n);
double **init_C_omp(double **C, int m, int n);

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
void matmult_asy_offload(int m,int n,int k,double **A,double **B,double **C);

// library offload
void matmult_lib_offload(int m,int n,int k,double **A,double **B,double **C);

#endif