#include <vector>
#include <numeric>
#include <cmath>

template <typename T>
double mean(const std::vector<T>& data) {
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

template <typename T>
double variance(const std::vector<T>& data) {
    double mean_value = mean(data);
    double sq_sum = std::accumulate(data.begin(), data.end(), 0.0, 
                                    [mean_value](double acc, T i) {
                                        return acc + std::pow(i - mean_value, 2);
                                    });
    return sq_sum / (data.size() - 1);
}

template <typename T>
double standard_deviation(const std::vector<T>& data) {
    return std::sqrt(variance(data));
}

template <typename T>
double z_score(const T& x, double mean_value, double standard_deviation_value) {
    return (x - mean_value) / standard_deviation_value;
}