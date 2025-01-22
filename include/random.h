#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <random>

namespace Random {
    std::mt19937_64 &gen();
    void randomize(uint64_t s = std::mt19937_64::default_seed); // Chama isso na main com uma seed aleatória
    int getInt();
    int getInt(int min, int max);
    double getReal(double min, double max);
} // namespace Random

#endif
// end of random.h