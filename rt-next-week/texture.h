#pragma once

#include "color.h"
#include "interval.h"
#include "rtw_image.h"
#include "rtweekend.h"
#include "vec3.h"
#include <memory>
#include <utility>

class texture {
  public:
    virtual ~texture() = default;

    [[nodiscard]] virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
  public:
    solid_color(const color& albedo) : albedo(albedo) {}

    solid_color(double red, double green, double blue) : solid_color(color(red, blue, green)) {}

    [[nodiscard]] color value(double u, double v, const point3& p) const override { return albedo; }

  private:
    color albedo;
};

class checker_texture : public texture {
  public:
    checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
        : inv_scale(1.0 / scale), even(std::move(even)), odd(std::move(odd)) {}

    checker_texture(double scale, const color& c1, const color& c2)
        : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

    [[nodiscard]] color value(double u, double v, const point3& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.x()));
        auto yInteger = int(std::floor(inv_scale * p.y()));
        auto zInteger = int(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class image_texture : public texture {
  public:
    image_texture(const std::string& filename) : image(filename) {}

    [[nodiscard]] color value(double u, double v, const point3& p) const override {
        if(image.height() <= 0)
            return {0, 1, 1};

        u = interval(0, 1).clamp(u);
        v = 1.0 - interval(0, 1).clamp(v);

        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
    }

  private:
    rtw_image image;
};
