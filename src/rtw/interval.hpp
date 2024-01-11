#pragma once

#include "rtweekend.hpp"

class interval {
   public:
    float min, max;

    interval() : min(+infinity), max(-infinity) {}

    interval(float _min, float _max) : min(_min), max(_max) {}

    interval(const interval& a, const interval& b)
        : min(std::min(a.min, b.min)), max(std::max(a.max, b.max)) {}

    auto contains(float x) const -> bool { return x >= min && x <= max; }

    auto surrounds(float x) const -> bool { return x > min && x < max; }

    auto clamp(float x) const -> float {
        if (x < min)
            return min;
        else if (x > max)
            return max;
        else
            return x;
    }

    auto size() const -> float { return max - min; }

    auto expand(float delta) const -> interval {
        auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }

    static const interval empty, universe;
};

static const interval empty(+infinity, -infinity);
static const interval universe(-infinity, +infinity);
