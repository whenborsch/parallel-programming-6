#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>

const int a = 5;
const int num_points = 10000;
const int num_threads = std::thread::hardware_concurrency();

std::mutex vectorMutex;
std::mutex fileMutex;
std::vector<int> results(num_threads);
std::ofstream file("points.csv");

bool isInArea(double x, double y) {
    double r = sqrt(x*x + y*y);
    double phi = atan2(y, x);
    return r*r <= 2*a*a*std::cos(2*phi) && r >= a;
}

void generate(int id, int num_points) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribX(a*sqrt(3)/2, a*sqrt(2));
    std::uniform_real_distribution<> distribY(static_cast<double>(-a)/2, static_cast<double>(a)/2);

    int count = 0;
    for (int i = 0; i < (int) num_points / 2; ++i) {
        double x = distribX(gen);
        double y = distribY(gen);
        bool isInPositive = isInArea(x, y);
        if (isInPositive) {
            count++;
        }

        bool isInNegative = isInArea(-x, y);
        if (isInNegative) {
            count++;
        }

        std::lock_guard<std::mutex> lock(fileMutex);
        file << x << "," << y << "," << isInPositive << std::endl;
        file << -x << "," << y << "," << isInNegative << std::endl;
    }

    std::lock_guard<std::mutex> lock(vectorMutex);
    results[id] = count;
}

int main() {
    std::vector<std::thread> threads(num_threads);

    auto start = std::chrono::high_resolution_clock::now();

    file << "x,y,isInArea" << std::endl;
    for (int i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(generate, i, (int) num_points / num_threads);
    }

    for (auto& th : threads) {
        th.join();
    }

    int total = 0;
    for (int count : results) {
        total += count;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    double square = ((a*sqrt(2)) - ((sqrt(3)/2)*a)) * (static_cast<double>(a)/2);

    double area = static_cast<double>(total) / num_points * (4*square);
    std::cout << "Calculated area: " << area << std::endl;
    std::cout << "Theory area: " << (3*sqrt(3) - M_PI) * a * a / 3 << std::endl;
    std::cout << "Time taken: " << diff.count() << " s\n";

    std::system("python3 graph.py");

    return 0;
}