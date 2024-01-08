#pragma once

#include "rtweekend.hpp"

class interval {
   public:
    float min, max;

    interval() : min(+infinity), max(-infinity) {}

    interval(float _min, float _max) : min(_min), max(_max) {}

    auto contains(float x) const -> bool { return x >= min && x <= max; }

    auto surrounds(float x) const -> bool { return x > min && x < max; }

    static const interval empty, universe;
};

static const interval empty(+infinity, -infinity);
static const interval universe(-infinity, +infinity);
