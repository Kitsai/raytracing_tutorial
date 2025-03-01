#pragma once

#include "rtweekend.h"
class interval {
  public:
    double min, max;
    interval() : min(+infinity), max(-infinity) {}
    interval(double min, double max) : min(min), max(max) {}
    interval(const interval& a, const interval& b) {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    [[nodiscard]] double size() const { return max - min; }

    [[nodiscard]] bool contains(double x) const { return min <= x && x <= max; }

    [[nodiscard]] bool surrounds(double x) const { return min < x && x < max; }

    [[nodiscard]] double clamp(double x) const {
        if(x < min)
            return min;
        if(x > max)
            return max;

        return x;
    }

    [[nodiscard]] interval expand(double delta) const {
        auto padding = delta / 2;
        return {min - padding, max + padding};
    }

    static const interval empty, universe;
};

const interval interval::empty = {+infinity, -infinity};
const interval interval::universe = {-infinity, +infinity};
