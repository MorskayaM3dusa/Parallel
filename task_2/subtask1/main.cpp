#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <omp.h>
#include <chrono>

int n = 20000;
int m = 20000;

void matrix_vector_product_omp(double *a, double *b, double *c, int m, int n)
{
#pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = m / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
        for (int i = lb; i <= ub; i++)
        {
            c[i] = 0.0;
            for (int j = 0; j < n; j++)
                c[i] += a[i * n + j] * b[j];
        }
    }
}

void matrix_vector_product(double *a, double *b, double *c, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        c[i] = 0.0;
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
    }
}

void run_serial()
{
    double *a, *b, *c;
    a = (double *)malloc(sizeof(*a) * m * n);
    b = (double *)malloc(sizeof(*b) * n);
    c = (double *)malloc(sizeof(*c) * m);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + j;
    }
    for (int j = 0; j < n; j++)
        b[j] = j;
    auto start = std::chrono::high_resolution_clock::now();
    matrix_vector_product(a, b, c, m, n);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    printf("Elapsed time (serial): %.6f sec.\n", duration.count());
    free(a);
    free(b);
    free(c);
}

void run_parallel(int num_threads)
{
    omp_set_num_threads(num_threads);
    double *a, *b, *c;
    a = (double *)malloc(sizeof(*a) * m * n);
    b = (double *)malloc(sizeof(*b) * n);
    c = (double *)malloc(sizeof(*c) * m);
#pragma omp parallel
    {
        int nthreads = num_threads;
        int threadid = omp_get_thread_num();
        int items_per_thread = m / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
        for (int i = lb; i <= ub; i++)
        {
            for (int j = 0; j < n; j++)
                a[i * n + j] = i + j;
            c[i] = 0.0;
        }
    }
    for (int j = 0; j < n; j++)
        b[j] = j;

    const auto start{std::chrono::steady_clock::now()};
    matrix_vector_product_omp(a, b, c, m, n);
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    printf("Elapsed time (parallel, %d threads): %.6f sec.\n", num_threads, elapsed_seconds.count());
    free(a);
    free(b);
    free(c);
}

int main(int argc, char **argv)
{
    printf("Matrix-vector product (c[m] = a[m, n] * b[n]; m = %d, n = %d)\n", m, n);
    printf("Memory used: %" PRIu64 " MiB\n", ((m * n + m + n) * sizeof(double)) >> 20);
    run_serial();
    run_parallel(1);
    run_parallel(2);
    run_parallel(4);
    run_parallel(7);
    run_parallel(8);
    run_parallel(16);
    run_parallel(20);
    run_parallel(40);
    return 0;
}
