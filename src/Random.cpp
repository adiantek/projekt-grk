#include <Random.hpp>

int64_t Random::initialScramble(int64_t seed) {
    return (seed ^ Random::multiplier) & mask;
}

Random::Random(int64_t seed) {
    this->seed = this->initialScramble(seed);
    // this->haveNextNextGaussian = false;
}

void Random::setSeed(int64_t seed) {
    this->seed = this->initialScramble(seed);
    // this->haveNextNextGaussian = false;
}

int32_t Random::next(int32_t bits) {
    this->seed = (this->seed * Random::multiplier + Random::addend) & Random::mask;
    return (int32_t)(((uint64_t)this->seed) >> ((uint64_t)(48 - bits)));
}

int32_t Random::nextInt() {
    return this->next(32);
}

int32_t Random::nextInt(int32_t n) {
    if ((n & -n) == n)  // i.e., n is a power of 2
        return (int32_t)((n * (int64_t)(this->next(31))) >> 31);

    int32_t bits, val;
    do {
        bits = next(31);
        val = bits % n;
    } while (bits - val + (n - 1) < 0);
    return val;
}

int64_t Random::nextLong() {
    return ((int64_t)(this->next(32)) << 32L) + this->next(32);
}

bool Random::nextBoolean() {
    return this->next(1) != 0;
}

float Random::nextFloat() {
    return this->next(24) / ((float)(1 << 24));
}

double Random::nextDouble() {
    return (((int64_t)(next(26)) << 27) + next(27)) / ((double)(1LL << 53LL));
}
