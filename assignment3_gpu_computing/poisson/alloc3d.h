#ifndef __ALLOC_3D
#define __ALLOC_3D

double ***malloc_3d(int m, int n, int k);
double ***d_malloc_3d(int m, int n, int k);

#define HAS_FREE_3D
void free_3d(double ***array3D);
void d_free_3d(double ***array3D);

#endif /* __ALLOC_3D */