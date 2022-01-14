#pragma once

#include <stdbool.h>
#include <stdint.h>

class Random {
   private:
    int64_t seed;
    static const int64_t multiplier = 0x5DEECE66DLL;
    static const int64_t addend = 0xBLL;
    static const int64_t mask = (1LL << 48LL) - 1;
    // double nextNextGaussian = 0.0;
    // bool haveNextNextGaussian = false;
    static int64_t initialScramble(int64_t seed);

   public:
    Random(int64_t seed);
    void setSeed(int64_t seed);
    int32_t next(int32_t bits);
    int32_t nextInt();
    int32_t nextInt(int32_t n);
    int32_t nextInt(int32_t begin, int32_t end);
    int64_t nextLong();
    int64_t nextLong(int64_t begin, int64_t end);
    bool nextBoolean();
    float nextFloat();
    float nextFloat(float begin, float end);
    double nextDouble();
    double nextDouble(double begin, double end);
};
