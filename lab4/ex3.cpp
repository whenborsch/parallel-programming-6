#include "kiss.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <fstream>

const int N_PROC = 8;
const int size = 1000000;

void norm_distr() {
    double mean = 0;
    double var = 0;

    const auto t1 = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(N_PROC);
    std::mutex mx;

    // normal distribution
    std::normal_distribution<double> normalDistribution(0.0, 1.0); // E[X], D[X]

    std::ofstream file;
    file.open("norm_dist.txt");

    for (int i = 0; i < N_PROC; ++i)
        threads.emplace_back([&, i]() {
            std::random_device r;
//            kiss::kiss rng(std::hash<std::thread::id>{}(std::this_thread::get_id()), r() + 1, r() + 2, r() + 3);
//            kiss::kiss rng(r(), r() + 1, r() + 2, r() + 3);
            for (int j = i; j < size; j += N_PROC) {
                mx.lock();
                auto x = normalDistribution(r);

                file << x << std::endl;

                mean += (x - mean) / (j + 1);
                if (j != 0) {
                    var += std::pow(x - mean, 2) / (j + 1) - var / j;
                }
                mx.unlock();
            }
        });


    for (auto& thread : threads) {
        thread.join();
    }

    file.close();

    const auto t2 = std::chrono::steady_clock::now();

    std::cout << "Normal Distribution -> mean: " << mean << "\tVar:   " << var<< std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
}

void exp_distr() {
    double mean = 0;
    double var = 0;

    const auto t1 = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(N_PROC);
    std::mutex mx;

    double const exp_dist_mean   = 2;
    double const exp_dist_lambda = 1 / exp_dist_mean;
    std::exponential_distribution<double> expDistribution(exp_dist_lambda); // E[X]=1/2,V[X]=1/4

    std::ofstream file;
    file.open("exp_dist.txt");

    for (int i = 0; i < N_PROC; ++i)
        threads.emplace_back([&, i]() {
            std::random_device r;
//            kiss::kiss rng(std::hash<std::thread::id>{}(std::this_thread::get_id()), r() + 1, r() + 2, r() + 3);
//            kiss::kiss rng(r(), r() + 1, r() + 2, r() + 3);
            for (int j = i; j < size; j += N_PROC) {
                mx.lock();
                auto x = expDistribution(r);

                file << x << std::endl;

                mean += (x - mean) / (j + 1);
                if (j != 0) {
                    var += std::pow(x - mean, 2) / (j + 1) - var / j;
                }
                mx.unlock();
            }
        });

    for (auto& thread : threads) {
        thread.join();
    }

    file.close();

    const auto t2 = std::chrono::steady_clock::now();

    std::cout << "Exponential Distribution -> mean: " << mean << "\tVar:   " << var<< std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
}

void poisson_distr() {
    double mean = 0;
    double var = 0;

    const auto t1 = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(N_PROC);
    std::mutex mx;

    int const poisson_mean = 4;
    std::poisson_distribution<int> poissonDistribution(poisson_mean); // E[X]=2,V[X]=4

    std::ofstream file;
    file.open("poisson_dist.txt");

    for (int i = 0; i < N_PROC; ++i)
        threads.emplace_back([&, i]() {
            std::random_device r;
//            kiss::kiss rng(std::hash<std::thread::id>{}(std::this_thread::get_id()), r() + 1, r() + 2, r() + 3);
//            kiss::kiss rng(r(), r() + 1, r() + 2, r() + 3);
            for (int j = i; j < size; j += N_PROC) {
                mx.lock();
                auto x = poissonDistribution(r);

                file << x << std::endl;

                mean += (x - mean) / (j + 1);
                if (j != 0) {
                    var += std::pow(x - mean, 2) / (j + 1) - var / j;
                }
                mx.unlock();
            }
        });


    for (auto& thread : threads) {
        thread.join();
    }

    file.close();

    const auto t2 = std::chrono::steady_clock::now();

    std::cout << "Poisson Distribution -> mean: " << mean << "\tVar:   " << var<< std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
}

void weibull_distr() {
    double mean = 0;
    double var = 0;

    const auto t1 = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(N_PROC);
    std::mutex mx;

    double const shape = 2;
    double const scale = 0.5;
    std::weibull_distribution<double> weibull_distribution(shape, scale); // E[X]=2,V[X]=4

    std::ofstream file;
    file.open("weibull_dist.txt");

    for (int i = 0; i < N_PROC; ++i)
        threads.emplace_back([&, i]() {
            std::random_device r;
//            kiss::kiss rng(std::hash<std::thread::id>{}(std::this_thread::get_id()), r() + 1, r() + 2, r() + 3);
//            kiss::kiss rng(r(), r() + 1, r() + 2, r() + 3);
            for (int j = i; j < size; j += N_PROC) {
                mx.lock();
                auto x = weibull_distribution(r);

                file << x << std::endl;

                mean += (x - mean) / (j + 1);
                if (j != 0) {
                    var += std::pow(x - mean, 2) / (j + 1) - var / j;
                }
                mx.unlock();
            }
        });


    for (auto& thread : threads) {
        thread.join();
    }

    file.close();

    const auto t2 = std::chrono::steady_clock::now();

    std::cout << "Weibull Distribution -> mean: " << mean << "\tVar:   " << var<< std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
}

int main() {
    norm_distr();
    exp_distr();
    poisson_distr();
    weibull_distr();
}