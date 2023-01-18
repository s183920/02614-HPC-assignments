

void jacobi_map(int N, double threshold, int iter_max, double ***U_new, double ***U_old, double ***F, double delta);
//void jacobi_para_simpel(int N, double threshold, int iter_max, double ***U_new, double ***U_old, double ***F, double delta);
void jacobi_para_opt(int N, double threshold, int iter_max, double ***U_new, double ***U_old, double ***F, double delta);