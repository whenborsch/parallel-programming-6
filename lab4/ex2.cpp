#include "kiss.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

const int N_PROC = 1;

int main2() {
    const int size = 1000000;
    double mean = 0;
    double var = 0;

    const auto t1 = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(N_PROC);
    std::mutex mx;

    for (int i = 0; i < N_PROC; ++i)
        threads.emplace_back([&, i]() {
            std::random_device r;
//            kiss::kiss rng(std::hash<std::thread::id>{}(std::this_thread::get_id()), r() + 1, r() + 2, r() + 3);
            kiss::kiss rng(r(), r() + 1, r() + 2, r() + 3);
            for (int j = i; j < size; j += N_PROC) {
                mx.lock();
                auto x = double(rng() % 1000) / 100.;
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

    const auto t2 = std::chrono::steady_clock::now();

    std::cout << "mean: " << mean << "\tVar:   " << var<< std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;

    return 0;
}