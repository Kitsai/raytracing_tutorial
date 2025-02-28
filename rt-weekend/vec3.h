#pragma once

#include "rtweekend.h"
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

class vec3 {
  public:
    std::array<double, 3> e;

    vec3(const vec3&) = default;
    vec3& operator=(const vec3& v) = default;
    vec3(vec3&& other) noexcept = default;
    vec3& operator=(vec3&& other) noexcept = default;

    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    [[nodiscard]] double x() const { return e[0]; }
    [[nodiscard]] double y() const { return e[1]; }
    [[nodiscard]] double z() const { return e[2]; }

    vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        if(t == 0.0)
            throw std::invalid_argument("Division by zero");
        return *this *= 1 / t;
    }

    [[nodiscard]] double length() const { return std::sqrt(length_squared()); }

    [[nodiscard]] double length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    static vec3 random() {
        return {random_double(), random_double(), random_double()};
    }

    static vec3 random(double min, double max) {
        return {random_double(min,max), random_double(min,max), random_double(min,max)};
    }

    bool near_zero() const {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) { return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]}; }

inline vec3 operator-(const vec3& u, const vec3& v) { return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]}; }

inline vec3 operator*(const vec3& u, const vec3& v) { return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]}; }

inline vec3 operator*(double t, const vec3& v) { return {t * v.e[0], t * v.e[1], t * v.e[2]}; }

inline vec3 operator*(const vec3& u, double t) { return t * u; }

inline vec3 operator/(const vec3& u, double t) { return (1 / t) * u; }

inline double dot(const vec3& u, const vec3& v) { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }

inline vec3 cross(const vec3& u, const vec3& v) {
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline vec3 unit_vector(const vec3& v) {
    double len = v.length();
    if(len == 0.0)
        throw std::invalid_argument("Cannot compute unit vector of zero length vector");
    return v / v.length();
}

inline vec3 random_unit_vector() {
    while(true) {
        auto p = vec3::random(-1, 1);
        auto lensq = p.length_squared();
        if(1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}