#include "color.h"
#include "interval.h"
#include "material.h"
#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include "camera.h"
#include <memory>

color ray_color(const ray& r, const hittable& world) {

    hit_record rec;
    if(world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = .5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

#define sio                                                                                                            \
    std::ios::sync_with_stdio(false);                                                                                  \
    std::cin.tie(NULL);                                                                                                \
    std::cout.tie(NULL);

int main() {
    sio;

    // World

    hittable_list world;

    auto material_ground = std::make_shared<lambertian>(color(.8, .8, 0));
    auto material_center = make_shared<lambertian>(color(.1, .2, .5));
    auto material_left = make_shared<metal>(color(.8, .8, .8));
    auto material_right = make_shared<metal>(color(.8, .6, .2));

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(make_shared<sphere>(point3(0, 0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0, -1.2), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0, -1.2), 0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
    
    return 0;
}
