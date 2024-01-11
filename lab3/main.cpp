#include "xst64.h"
#include "statistics.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <cmath>
#include <numeric>
#include <chrono>

// task 1 demonstration
void print_first_10(std::vector<uint64_t> data) {
    std::cout << "First 10 elements: " << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << data[i] << "\t";
    }
    std::cout << std::endl;
}

std::vector<double> nomalize_data(std::vector<uint64_t> data, uint64_t M = UINT64_MAX) {
    std::vector<double> normalized_data(data.size());

    for (int i = 0; i < data.size(); i++) {
        normalized_data[i] = (double) data[i] / M;
    }

    return normalized_data;
}

void get_data_and_plot(std::vector<uint64_t> data, uint64_t M = UINT64_MAX) {
    std::vector<double> normalized_data = nomalize_data(data, M);

    for (int i = 1000; i <= 10000000; i *= 10) {
        auto filename = "xst64_" + std::to_string(i) + ".txt";
        std::ofstream of(filename);
        std::ostream_iterator<double> out_it(of, "\n");
        std::copy(normalized_data.begin(), normalized_data.begin() + i, out_it);
    }

    std::system("python3 graph.py");
}

void count_mean_variance(std::vector<uint64_t> data, uint64_t M = UINT64_MAX) {
    for (int i = 1000; i <= 10000000; i *= 10) {
        // auto start = std::chrono::high_resolution_clock::now();
        std::vector<uint64_t> sub_data(data.begin(), data.begin() + i);
        std::vector<double> norm_sub_data = nomalize_data(sub_data, M);
        std::cout << std::endl <<  i << " ELEMENTS" << std::endl;
        std::cout << "E[X]: " << mean(norm_sub_data) << " (diff: " << mean(norm_sub_data) -  1. / 2. << ")" << std::endl;
        std::cout << "D[X]: " << variance(norm_sub_data) << " (diff: " << variance(norm_sub_data) - 1. / 12. << ")" << std::endl;
    }
}

void welford(xst64 xst64_generator, uint64_t M = UINT64_MAX) {
    double mean = 0;
    double M2 = 0;
    double variance = 0;
    double x = 0;
    
    for (int j = 1000; j <= 1000000000; j *= 10) {
        for (int i = 0; i < j; i++) {
            x = (double) xst64_generator() / M;
            double delta = x - mean;
            mean += delta / (i + 1);
            M2 += delta * (x - mean);
            variance = M2 / (i + 1);
        }

        std::cout << std::endl <<  j << " ELEMENTS" << std::endl;
        std::cout << "E[X]: " << mean << " (diff: " << mean -  1. / 2. << ")" << std::endl;
        std::cout << "D[X]: " << variance << " (diff: " << variance - 1. / 12. << ")" << std::endl;
    }
} 

int main() {
   const int size = 10000000;
   const uint64_t M = UINT64_MAX;
   std::vector<uint64_t> data(size);
   xst64 xst_generator(M);
   xst_generator(data);

   print_first_10(data);
   std::cout << std::endl;
   get_data_and_plot(data);
   count_mean_variance(data);

   std::cout << std::endl << "WELFORD ALGORITHM" << std::endl; 
   welford(xst_generator);
}