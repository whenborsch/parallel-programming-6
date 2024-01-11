#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <thread>
#include <algorithm>
#include <chrono>

double mean(const std::vector<double>& v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

double variance(const std::vector<double>& v, double m) {
    double sum = std::accumulate(v.begin(), v.end(), 0.0, [&](double acc, double val) {
        return acc + (val - m) * (val - m);
    });
    return sum / (v.size() - 1);
}

void generate_data(std::vector<double>& v) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1.0, 2.0);
    for (auto& i : v) {
        i = dis(gen);
    }
}

int main() {
    const int size = 10000000;
    const int num_threads = std::thread::hardware_concurrency();
    const int num_trials = 10;

    // task 2, 3
    std::vector<std::vector<double>> data(num_threads, std::vector<double>(size / num_threads));
    std::vector<std::thread> threads(num_threads);
    std::vector<double> means(num_threads);
    std::vector<double> variances(num_threads);
    std::vector<double> times(num_trials);

    for (auto& v : data) {
        generate_data(v);
    }

    for (int trial = 0; trial < num_trials; ++trial) {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < num_threads; ++i) {
            threads[i] = std::thread([&](int idx) {
                means[idx] = mean(data[idx]);
            }, i);
        }

        for (auto& t : threads) {
            t.join();
        }

        double total_mean = std::accumulate(means.begin(), means.end(), 0.0) / num_threads;

        for (int i = 0; i < num_threads; ++i) {
            threads[i] = std::thread([&](int idx) {
                variances[idx] = variance(data[idx], total_mean);
            }, i);
        }

        for (auto& t : threads) {
            t.join();
        }

        double total_variance = std::accumulate(variances.begin(), variances.end(), 0.0) / num_threads;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << "Trial " << trial+1 << std::endl;
        std::cout << "Mean: " << total_mean << ", Variance: " << total_variance << std::endl;
        std::cout << "Time elapsed: " << elapsed.count() << " seconds" << std::endl;

        times[trial] = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    }

    double mean_time = mean(times);
    double std_dev_time = std::sqrt(variance(times, mean_time));

    std::cout << std::endl << "Mean time: " << mean_time << " +- " << std_dev_time << " ms" << std::endl;

    return 0;
}
