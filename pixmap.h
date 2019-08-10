#pragma once

#include <cstdint>
#include <memory>
#include <vector>

struct pixmap
{
    enum class pixel_type
    {
        GRAY,
        GRAY_ALPHA,
        RGB,
        RGB_ALPHA
    };

    pixmap(size_t width, size_t height, pixel_type type);

    size_t pixel_size() const;
    size_t row_stride() const;

    size_t width;
    size_t height;
    pixel_type type;
    std::vector<uint8_t> data;
};

std::unique_ptr<pixmap> load_pixmap_from_png(const char *path);
