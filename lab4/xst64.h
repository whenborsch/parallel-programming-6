// xst64.h

#ifndef XST64_H
#define XST64_H

#include <vector>
#include <cstdint>
#include <fstream>
#include <thread>

class xst64 {
public:
    xst64(uint64_t M);
    void operator()(std::vector<uint64_t>& data);
    uint64_t operator()();

private:
    uint64_t seed;
    uint64_t M;
    void update_seed();
    uint64_t generate_single_number();
};

#endif // xst64