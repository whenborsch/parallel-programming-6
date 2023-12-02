#ifndef LAB3_KISS_H
#define LAB3_KISS_H

#include <cstdint>
#include <random>

namespace kiss {

    class kiss {
        uint64_t x;
        uint64_t y;
        uint64_t z;
        uint64_t w;

    public:
        kiss(uint64_t x, uint64_t y, uint64_t z, uint64_t w);

        uint64_t operator()();
    };
}


#endif //LAB3_KISS_H
