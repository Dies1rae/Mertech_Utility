#ifndef COMMON_H
#define COMMON_H

#include <iostream>

struct serialCFG {
    uint64_t baud_rate_ = 115200;
    uint8_t databits_ = 8;
    bool parity_ = 0;
    uint8_t stop_bits_ = 1;
};

#endif // COMMON_H
