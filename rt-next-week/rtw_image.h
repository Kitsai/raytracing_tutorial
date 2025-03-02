#pragma once

#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb/stb_image.h>

class rtw_image {
  public:
    rtw_image() = default;

    rtw_image(const std::string& image_filename) {
        auto imagedir = getenv("RTW_IMAGES");

        if(imagedir && load(std::string(imagedir) + "/" + image_filename))
            return;
        if(load(image_filename))
            return;
        if(load("images/" + image_filename))
            return;
        if(load("../images/" + image_filename))
            return;
        if(load("../../images/" + image_filename))
            return;
        if(load("../../../images/" + image_filename))
            return;
        if(load("../../../../images/" + image_filename))
            return;
        if(load("../../../../../images/" + image_filename))
            return;
        if(load("../../../../../../images/" + image_filename))
            return;

        std::cerr << "ERROR: Could not load image file '" << image_filename << "'.\n";
    }

    ~rtw_image() {
        delete[] bdata;
        STBI_FREE(fdata);
    }

    bool load(const std::string& filename) {
        auto n = bytes_per_pixel;
        fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
        if(fdata == nullptr)
            return false;

        bytes_per_scanline = image_width * bytes_per_pixel;
        convert_to_bytes();
        return true;
    }

    [[nodiscard]] int width() const { return (fdata == nullptr) ? 0 : image_width; }
    [[nodiscard]] int height() const { return (fdata == nullptr) ? 0 : image_height; }

    [[nodiscard]] const unsigned char* pixel_data(int x, int y) const {
        static std::array<unsigned char, 3> magenta = {255, 0, 255};
        if(bdata == nullptr)
            return magenta.data();

        x = clamp(x, 0, image_width);
        y = clamp(y, 0, image_height);

        return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
    }

  private:
    const int bytes_per_pixel = 3;
    float* fdata = nullptr;
    unsigned char* bdata = nullptr;
    int image_width = 0;
    int image_height = 0;
    int bytes_per_scanline = 0;

    static int clamp(int x, int low, int high) {
        if(x < low)
            return low;
        if(x < high)
            return x;
        return high - 1;
    }

    static unsigned char float_to_byte(float value) {
        if(value <= 0.0)
            return 0;
        if(1.0 <= value)
            return 255;
        return static_cast<unsigned char>(256.0 * value);
    }

    void convert_to_bytes() {
        int total_bytes = image_width * image_height * bytes_per_pixel;
        bdata = new unsigned char[total_bytes];

        auto* bptr = bdata;
        auto* fptr = fdata;
        for(auto i = 0; i < total_bytes; i++, fptr++, bptr++)
            *bptr = float_to_byte(*fptr);
    }
};
