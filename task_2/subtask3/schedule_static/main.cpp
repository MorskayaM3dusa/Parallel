#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>

void solving(double *A, double *x_previous, double *b, long long n, double tau, double epsilon, int num_of_threads)
{
  double *middle_res = (double *)malloc(sizeof(double) * n);
  double error = 1e10;
  double b_norm = 0.0;
  double tmp;
#pragma omp parallel for schedule(static, n / num_of_threads) num_threads(num_of_threads)

  for (long long i = 0; i < n; ++i)
    b_norm += b[i] * b[i];
  b_norm = std::sqrt(b_norm);
  while (error > epsilon)
  {
    double middle_norm = 0.0;

#pragma omp parallel for schedule(static, n / num_of_threads) num_threads(num_of_threads)

    for (long long i = 0; i < n; i++)
    {
      middle_res[i] = 0.0;
      for (long long j = 0; j < n; j++)
        middle_res[i] += A[i * n + j] * x_previous[j];
      middle_res[i] -= b[i];

      tmp = middle_res[i] * middle_res[i];
#pragma omp atomic
      middle_norm += tmp;

      middle_res[i] *= tau;
      x_previous[i] -= middle_res[i];
    }
    middle_norm = std::sqrt(middle_norm);
    error = middle_norm / b_norm;
  }
  free(middle_res);
}

void one_example(int num_of_threads)
{
  double *A, *x_previous, *b;
  long long n = 10000;
  double tau = 0.00001;
  double epsilon = 1e-5;
  A = (double *)malloc(sizeof(double) * n * n);
  b = (double *)malloc(sizeof(double) * n);
  x_previous = (double *)malloc(sizeof(double) * n);

#pragma omp parallel for num_threads(num_of_threads)
  for (long long i = 0; i < n; i++)
  {
    for (long long j = 0; j < n; j++)
    {
      if (i == j)
        A[i * n + j] = 2.0;
      else
        A[i * n + j] = 1.0;
    }
  }
#pragma omp parallel for num_threads(num_of_threads)
  for (int i = 0; i < n; ++i)
  {
    b[i] = n + 1;
    x_previous[i] = 0.0;
  }

  const auto start{std::chrono::steady_clock::now()};
  solving(A, x_previous, b, n, tau, epsilon, num_of_threads);
  const auto end{std::chrono::steady_clock::now()};
  const std::chrono::duration<double> elapsed_seconds{end - start};
  printf("Elapsed time (parallel, %d threads): %.6f sec.\n", num_of_threads, elapsed_seconds.count());

  free(A);
  free(b);
  free(x_previous);
}

int main()
{
  one_example(1);
  one_example(2);
  one_example(4);
  one_example(7);
  one_example(8);
  one_example(16);
  one_example(20);
  one_example(40);
  return 0;
}
