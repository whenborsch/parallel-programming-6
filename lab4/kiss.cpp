#include "kiss.h"

namespace kiss {

    kiss::kiss(uint64_t x, uint64_t y, uint64_t z, uint64_t w) :
            x(x), y(y), z(z), w(w) {}

    uint64_t kiss::operator()() {
        x = 69069 * x + 123456;

        y ^= y << 13;
        y ^= y >> 17;
        y ^= y << 5;

        z = 698769069 * z + w;
        w = z >> 32;

        return x + y + z;
    }
}