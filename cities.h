#pragma once

#include <vector>

struct city
{
    const char *name;
    const char *country;
    float latitude;  // radians, N:+, S:-
    float longitude; // radians, E:+, W:-
};

const std::vector<city> &get_cities();
