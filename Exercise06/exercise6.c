#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000
#define STRIP_SIZE 256

int main()
{
    double *A = malloc(N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double *C = malloc(N * sizeof(double));

    for (int i = 0; i < N; i++) {
        A[i] = i * 0.5;
        B[i] = i * 0.25;
    }

    double start = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int start = 0; start < N; start += STRIP_SIZE) {

        int end = start + STRIP_SIZE;

        if (end > N)
            end = N;

        for (int i = start; i < end; i++) {
            C[i] = A[i] * B[i];
        }
    }

    double end = omp_get_wtime();

    printf("C[0] = %f\n", C[0]);
    printf("C[N-1] = %f\n", C[N - 1]);
    printf("Time = %f seconds\n", end - start);

    free(A);
    free(B);
    free(C);

    return 0;
}