#ifndef LAB3_STAT_H
#define LAB3_STAT_H

#include <vector>
#include <algorithm>

template<typename T>
T mean(std::vector<T> x) {
    return std::accumulate(x.begin(), x.end(), T()) / x.size();
}

template<typename T>
T var(std::vector<T> x) {
    auto m = mean(x);
    T sum = 0;

    std::for_each(x.begin(), x.end(), [&](T val) {
        sum += (val - m) * (val - m);
    });

    return sum / (x.size() - 1);
}

#endif //LAB3_STAT_H
