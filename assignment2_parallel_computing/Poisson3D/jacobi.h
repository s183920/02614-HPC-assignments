/* jacobi.h - Poisson problem 
 *
 * $Id: jacobi.h,v 1.1 2006/09/28 10:12:58 bd Exp bd $
 */

#ifndef _JACOBI_H
#define _JACOBI_H

void jacobi(int N, double threshold, int iter_max, double ***U_new, double ***U_old, double ***F, double delta);
    
#endif
