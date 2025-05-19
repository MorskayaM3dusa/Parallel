#include <iostream> 
#include <thread>
#include <chrono>
#include <vector> 

void initialize_arrays(double* matrix, double* vec, double* ans, int size, int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < size; j++)
            matrix[i * size + j] = i + j;
        vec[i] = i;
        ans[i] = 0.0;
    }
}

void initialize_parallel(double* matrix, double* vec, double* ans, int size, int num_threads) {
    std::vector<std::thread> threads;
    int chunk_size = size / num_threads;
    int mod = size % num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = start + chunk_size;
        if (i == num_threads - 1) 
            end += mod;
        threads.emplace_back(initialize_arrays, matrix, vec, ans, size, start, end);
    }
    for (auto& thread : threads)
        thread.join();
}

void matrix_mult(double* matrix, double* vec, double* ans, int size, int start, int end) {
    for (int i = start; i < end; i++) {
        ans[i] = 0.0;
        for (int j = 0; j < size; j++) {
            ans[i] += matrix[i * size + j] * vec[j];
        }
    }
}

void matrix_mult_parallel(double* matrix, double* vec, double* ans, int size, int num_threads) {
    std::vector<std::thread> threads;
    int chunk_size = size / num_threads;
    int mod = size % num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = start + chunk_size;
        if (i == num_threads - 1) 
            end += mod;
        threads.emplace_back(matrix_mult, matrix, vec, ans, size, start, end);
    }
    for (auto& thread : threads)
        thread.join();
}

double run_parallel(int N, int num_threads) {
    double *matrix, *vect, *ans;
    matrix = (double*)malloc(sizeof(*matrix) * N * N);
    vect = (double*)malloc(sizeof(*vect) * N);
    ans = (double*)malloc(sizeof(*ans) * N);
    const auto start{std::chrono::steady_clock::now()};
    initialize_parallel(matrix, vect, ans, N, num_threads);
    matrix_mult_parallel(matrix, vect, ans, N, num_threads);
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    free(matrix); 
    free(vect); 
    free(ans);
    return elapsed_seconds.count();
}

int main() {
    int size = 20000;
    int threads[8] = {1, 2, 4, 7, 8, 16, 20, 40};
    for (auto& n_threads : threads) {
        double parallel_time = run_parallel(size, n_threads);
        std::cout << n_threads << " threads\n";
        std::cout << "Time: " << parallel_time << "s\n";
    }
    size = 40000;
    for (auto& n_threads : threads) {
        double parallel_time = run_parallel(size, n_threads);
        std::cout << n_threads << " threads\n";
        std::cout << "Time: " << parallel_time << "s\n";
    }
    return 0;
}