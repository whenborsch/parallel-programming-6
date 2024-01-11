#include "xst64.h"
#include "distrib.h"
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <fstream>
#include <iterator>

struct WelfordData {
    int count;
    double mean;
    double M2;
};

std::mutex mtx, numbers_mtx;

void generate_and_print_numbers(int thread_id, uint64_t M, int num_numbers) {
    xst64 generator(M);
    for (int i = 0; i < num_numbers; ++i) {
        std::cout << "Thread " << thread_id << ": " << generator() << std::endl;
    }
}

void parallel_generate_and_print(int num_threads, uint64_t M, int total_numbers) {
    std::vector<std::thread> threads;
    int numbers_per_thread = total_numbers / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int num_numbers = (i == num_threads - 1) ? total_numbers - numbers_per_thread * i : numbers_per_thread;
        threads.push_back(std::thread(generate_and_print_numbers, i, M, num_numbers));
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void print_results(WelfordData& data) {
    if (data.count < 2) {
        std::cout << "Insufficient data" << std::endl;
    } else {
        double mean = data.mean;
        if (data.count - 1 == 0) {
            std::cout << "Variance calculation would cause division by zero" << std::endl;
        } else {
            double variance = data.M2 / (data.count - 1);
            std::cout << "Mean: " << mean << std::endl;
            std::cout << "Variance: " << variance << std::endl;
        }
    }
}

// --- xst uniform distribution ---

void welford_online_algorithm(int thread_id, uint64_t M, int num_numbers, WelfordData& data) {
    xst64 generator(M);
    for (int i = 1; i <= num_numbers; ++i) {
        double number = static_cast<double>(generator()) / M;
        double delta, delta2;
        {
            std::lock_guard<std::mutex> lock(mtx);
            int count = ++data.count;
            delta = number - data.mean;
            data.mean += delta / i;
            delta2 = number - data.mean;
            data.M2 += delta * delta2;
        }
    }
}

void parallel_welford(int num_threads, uint64_t M, int total_numbers) {
    std::vector<std::thread> threads;
    int numbers_per_thread = total_numbers / num_threads;
    WelfordData data = {0, 0.0, 0.0};

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << std::endl << "[UNIFORM] Starting Welford's online algorithm with " << num_threads << " threads and " << total_numbers << " numbers" << std::endl;

    for (int i = 0; i < num_threads; ++i) {
        int num_numbers = (i == num_threads - 1) ? total_numbers - numbers_per_thread * i : numbers_per_thread;
        threads.push_back(std::thread(welford_online_algorithm, i, M, num_numbers, std::ref(data)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    print_results(data);

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

// --- xst normal distribution ---
void welford_online_algorithm_normal(int thread_id, uint64_t M, int num_numbers, WelfordData& data, std::vector<double>& numbers) {
    xst64 generator(M);
    for (int i = 1; i <= num_numbers; ++i) {
        double number = xst64_normal(generator, 0.0, 1.0);
        {
            std::lock_guard<std::mutex> lock(numbers_mtx);
            numbers.push_back(number);
        }
        double delta, delta2;
        {
            std::lock_guard<std::mutex> lock(mtx);
            int count = ++data.count;
            delta = number - data.mean;
            data.mean += delta / i;
            delta2 = number - data.mean;
            data.M2 += delta * delta2;
        }
    }
}

void parallel_welford_normal(int num_threads, uint64_t M, int total_numbers) {
    std::vector<std::thread> threads;
    int numbers_per_thread = total_numbers / num_threads;
    WelfordData data = {0, 0.0, 0.0};

    std::vector<double> numbers;

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << std::endl << "[NORM] Starting Welford's online algorithm with " << num_threads << " threads and " << total_numbers << " numbers" << std::endl;

    for (int i = 0; i < num_threads; ++i) {
        int num_numbers = (i == num_threads - 1) ? total_numbers - numbers_per_thread * i : numbers_per_thread;
        threads.push_back(std::thread(welford_online_algorithm_normal, i, M, num_numbers, std::ref(data), std::ref(numbers)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::ofstream of("norm.txt");
    std::ostream_iterator<double> out_it(of, "\n");
    std::copy(numbers.begin(), numbers.end(), out_it);

    print_results(data);

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

// --- xst exponential distribution ---
void welford_online_algorithm_exponential(int thread_id, uint64_t M, int num_numbers, WelfordData& data, std::vector<double>& numbers) {
    xst64 generator(M);
    for (int i = 1; i <= num_numbers; ++i) {
        double number = xst64_exp(generator, 2.0);
        {
            std::lock_guard<std::mutex> lock(numbers_mtx);
            numbers.push_back(number);
        }
        double delta, delta2;
        {
            std::lock_guard<std::mutex> lock(mtx);
            int count = ++data.count;
            delta = number - data.mean;
            data.mean += delta / i;
            delta2 = number - data.mean;
            data.M2 += delta * delta2;
        }
    }
}

void parallel_welford_exponential(int num_threads, uint64_t M, int total_numbers) {
    std::vector<std::thread> threads;
    int numbers_per_thread = total_numbers / num_threads;
    WelfordData data = {0, 0.0, 0.0};

    std::vector<double> numbers;

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << std::endl << "[EXP] Starting Welford's online algorithm with " << num_threads << " threads and " << total_numbers << " numbers" << std::endl;

    for (int i = 0; i < num_threads; ++i) {
        int num_numbers = (i == num_threads - 1) ? total_numbers - numbers_per_thread * i : numbers_per_thread;
        threads.push_back(std::thread(welford_online_algorithm_exponential, i, M, num_numbers, std::ref(data), std::ref(numbers)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::ofstream of("exp.txt");
    std::ostream_iterator<double> out_it(of, "\n");
    std::copy(numbers.begin(), numbers.end(), out_it);

    print_results(data);

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

// --- xst poisson distribution ---
void welford_online_algorithm_poisson(int thread_id, uint64_t M, int num_numbers, WelfordData& data, std::vector<int>& numbers) {
    xst64 generator(M);
    for (int i = 1; i <= num_numbers; ++i) {
        int number = xst64_poisson(generator, 4.0);
        {
            std::lock_guard<std::mutex> lock(numbers_mtx);
            numbers.push_back(number);
        }
        double delta, delta2;
        {
            std::lock_guard<std::mutex> lock(mtx);
            int count = ++data.count;
            delta = number - data.mean;
            data.mean += delta / i;
            delta2 = number - data.mean;
            data.M2 += delta * delta2;
        }
    }
}

void parallel_welford_poisson(int num_threads, uint64_t M, int total_numbers) {
    std::vector<std::thread> threads;
    int numbers_per_thread = total_numbers / num_threads;
    WelfordData data = {0, 0.0, 0.0};

    std::vector<int> numbers;

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << std::endl << "[POISSON] Starting Welford's online algorithm with " << num_threads << " threads and " << total_numbers << " numbers" << std::endl;

    for (int i = 0; i < num_threads; ++i) {
        int num_numbers = (i == num_threads - 1) ? total_numbers - numbers_per_thread * i : numbers_per_thread;
        threads.push_back(std::thread(welford_online_algorithm_poisson, i, M, num_numbers, std::ref(data), std::ref(numbers)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    print_results(data);

    std::ofstream of("poisson.txt");
    std::ostream_iterator<int> out_it(of, "\n");
    std::copy(numbers.begin(), numbers.end(), out_it);

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

// --- xst weibull distribution ---
void welford_online_algorithm_weibull(int thread_id, uint64_t M, int num_numbers, WelfordData& data, std::vector<double>& numbers) {
    xst64 generator(M);
    for (int i = 1; i <= num_numbers; ++i) {
        double number = xst64_weibull(generator, 2.0, 1.0);
        {
            std::lock_guard<std::mutex> lock(numbers_mtx);
            numbers.push_back(number);
        }
        double delta, delta2;
        {
            std::lock_guard<std::mutex> lock(mtx);
            int count = ++data.count;
            delta = number - data.mean;
            data.mean += delta / i;
            delta2 = number - data.mean;
            data.M2 += delta * delta2;
        }
    }
}

void parallel_welford_weibull(int num_threads, uint64_t M, int total_numbers) {
    std::vector<std::thread> threads;
    int numbers_per_thread = total_numbers / num_threads;
    WelfordData data = {0, 0.0, 0.0};

    std::vector<double> numbers;

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << std::endl << "[WEIBULL] Starting Welford's online algorithm with " << num_threads << " threads and " << total_numbers << " numbers" << std::endl;

    for (int i = 0; i < num_threads; ++i) {
        int num_numbers = (i == num_threads - 1) ? total_numbers - numbers_per_thread * i : numbers_per_thread;
        threads.push_back(std::thread(welford_online_algorithm_weibull, i, M, num_numbers, std::ref(data), std::ref(numbers)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    print_results(data);

    std::ofstream of("weibull.txt");
    std::ostream_iterator<double> out_it(of, "\n");
    std::copy(numbers.begin(), numbers.end(), out_it);

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

int main() {
    // Выясняем количество потоков выполнения, поддерживаемых процессором
    auto n_threads = std::thread::hardware_concurrency();
    const int size = 10000000;

    std::vector<uint64_t> data(size);

    auto start = std::chrono::high_resolution_clock::now(); // Start time measurement

    parallel_generate_and_print(n_threads, 10, 100);

    auto end = std::chrono::high_resolution_clock::now(); // End time measurement
    std::chrono::duration<double> duration = end - start; // Calculate duration

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl << std::endl;

    for (int i = 1000; i <= size; i *= 10) {
        parallel_welford(n_threads, UINT64_MAX, i);
    }

    parallel_welford_normal(n_threads, RAND_MAX, size);
    parallel_welford_exponential(n_threads, RAND_MAX, size);
    parallel_welford_poisson(n_threads, RAND_MAX, size);
    parallel_welford_weibull(n_threads, RAND_MAX, size);

    std::system("python3 graph.py");

    return 0;
}