#include "kiss.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "stat.h"

const int N_PROC = 8;

void generateParallel(std::vector<uint64_t>& result, int size) {
    std::vector<std::thread> threads;
    threads.reserve(N_PROC);
    result.resize(size);

    for (int i = 0; i < N_PROC; ++i)
        threads.emplace_back([&, i]() {
            std::random_device r;
            kiss::kiss rng(r(), r() + i, r() + i, r() + i);
            for (int j = i; j < size; j += N_PROC) {
                result[j] = rng();
            }
        });


    for (auto& thread : threads) {
        thread.join();
    }
}


int main() {
    const int size = 10000000;
    std::vector<uint64_t> randomNumbers;

    const auto t1 = std::chrono::steady_clock::now();

    generateParallel(randomNumbers, size);

    const auto t2 = std::chrono::steady_clock::now();

    for (const auto& number : randomNumbers) {
        std::cout << number << " ";
    }

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;


    return 0;
}
