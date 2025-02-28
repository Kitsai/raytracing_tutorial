#pragma once

#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"
#include <cmath>
#include <memory>
#include <utility>

class sphere : public hittable {
  public:
    sphere(const point3& center, double radius, std::shared_ptr<material> mat)
        : center(center, vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(std::move(mat)) {}

    sphere(const point3& center1, const point3& center2, double radius, std::shared_ptr<material> mat)
        : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(std::move(mat)) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        point3 current_center = center.at(r.time());
        vec3 oc = current_center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;

        if(discriminant < 0.0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if(root <= ray_t.min || ray_t.max <= root) {
            root = (h + sqrtd) / a;
            if(root <= ray_t.min || ray_t.max <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - current_center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

  public:
    ray center;
    double radius;
    shared_ptr<material> mat;
};
