#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 

#define NPOINTS 2000
#define MAXITER 2000

struct complex {
  double real;
  double imag;
};

int main(){
  int i, j, iter;
  int numoutside = 0;
  double area, error;
  double start_time, end_time;

  start_time = omp_get_wtime();

  /* 
   * Parallelize the outer loop. 
   * i, j, iter are automatically private if declared in the loop or clauses.
   * numoutside uses reduction to safely sum up counts across threads.
   */
  #pragma omp parallel for default(none) private(i, j, iter) reduction(+:numoutside) schedule(static)
  for (i = 0; i < NPOINTS; i++) {
    for (j = 0; j < NPOINTS; j++) {
      struct complex z, c;
      double ztemp;
      
      c.real = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + 1.0e-7;
      c.imag = 1.125 * (double)(j) / (double)(NPOINTS) + 1.0e-7;
      z = c;
      
      for (iter = 0; iter < MAXITER; iter++) {
        ztemp = (z.real * z.real) - (z.imag * z.imag) + c.real;
        z.imag = z.real * z.imag * 2 + c.imag; 
        z.real = ztemp; 
        if ((z.real * z.real + z.imag * z.imag) > 4.0e0) {
          numoutside++; 
          break;
        }
      }
    }
  }

  end_time = omp_get_wtime();

  /* Calculate area and error and output the results */
  area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
  error = area / (double)NPOINTS;

  printf("Area of Mandelbrot set = %12.8f +/- %12.8f\n", area, error);
  printf("Execution Time = %f seconds\n", end_time - start_time);

  return 0;
}