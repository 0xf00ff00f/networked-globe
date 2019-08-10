#include "rand.h"

#include <random>

static std::mt19937 mt;

float rand_float()
{
    return static_cast<float>(mt() - mt.min())/(mt.max() - mt.min());
}

int rand_int()
{
    return mt();
}
