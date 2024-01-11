// xst64.cpp

#include "xst64.h"
#include <chrono>
#include <stdexcept>
#include <random>

xst64::xst64(uint64_t M) : M(M) {
    // Для инициализации seed используем значение из /dev/random
    std::ifstream rand_file("/dev/random", std::ios::in | std::ios::binary);
    if (!rand_file) {
        // Не удается прочитать значение из /dev/random, используем std::random_device
        seed = std::random_device()();
    } else {
        rand_file.read(reinterpret_cast<char*>(&seed), sizeof(seed));
        rand_file.close();
    }
}

void xst64::update_seed() {
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
}

uint64_t xst64::generate_single_number() {
    update_seed();
    return static_cast<uint64_t>(seed % M);
}

void xst64::operator()(std::vector<uint64_t>& data) {
    for (int i = 0; i < data.size(); ++i) {
        data[i] = generate_single_number();
    }
}

uint64_t xst64::operator()() {
    return generate_single_number();
}