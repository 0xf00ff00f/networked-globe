#include "pixmap.h"

#include "panic.h"
#include "noncopyable.h"

#include <algorithm>
#include <png.h>

class file : private noncopyable
{
public:
    file(const std::string &path)
        : fp_(fopen(path.c_str(), "rb"))
    {
    }

    ~file() { fclose(fp_); }

    operator FILE *() const { return fp_; }

    operator bool() const { return fp_; }

private:
    FILE *fp_;
};

pixmap::pixmap(size_t width, size_t height, pixel_type type)
    : width(width)
    , height(height)
    , type(type)
    , data(width * height * pixel_size())
{
}

size_t pixmap::pixel_size() const
{
    switch (type)
    {
    case pixmap::pixel_type::GRAY:
        return 1;

    case pixmap::pixel_type::GRAY_ALPHA:
        return 2;

    case pixmap::pixel_type::RGB:
        return 3;

    case pixmap::pixel_type::RGB_ALPHA:
        return 4;
    }
}

size_t pixmap::row_stride() const
{
    return pixel_size() * width;
}

namespace
{
png_byte to_png_color_type(pixmap::pixel_type type)
{
    switch (type)
    {
    case pixmap::pixel_type::GRAY:
        return PNG_COLOR_TYPE_GRAY;

    case pixmap::pixel_type::GRAY_ALPHA:
        return PNG_COLOR_TYPE_GRAY_ALPHA;

    case pixmap::pixel_type::RGB:
        return PNG_COLOR_TYPE_RGB;

    case pixmap::pixel_type::RGB_ALPHA:
        return PNG_COLOR_TYPE_RGBA;
    }
}

pixmap::pixel_type to_pixel_type(png_byte png_color_type)
{
    switch (png_color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
        return pixmap::pixel_type::GRAY;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
        return pixmap::pixel_type::GRAY_ALPHA;

    case PNG_COLOR_TYPE_RGB:
        return pixmap::pixel_type::RGB;

    case PNG_COLOR_TYPE_RGBA:
        return pixmap::pixel_type::RGB_ALPHA;

    default:
        panic("invalid PNG color type: %x\n", png_color_type);
    }
}
}

std::unique_ptr<pixmap> load_pixmap_from_png(const char *path)
{
    file in_file(path);
    if (!in_file)
        panic("failed to open %s\n", path);

    png_structp png_ptr;

    if (!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0)))
        panic("png_create_read_struct\n");

    png_infop info_ptr;

    if (!(info_ptr = png_create_info_struct(png_ptr)))
        panic("png_create_info_struct\n");

    if (setjmp(png_jmpbuf(png_ptr)))
        panic("png error?\n");

    png_init_io(png_ptr, in_file);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0);

    if (png_get_bit_depth(png_ptr, info_ptr) != 8)
        panic("invalid PNG bit depth\n");

    const size_t width = png_get_image_width(png_ptr, info_ptr);
    const size_t height = png_get_image_height(png_ptr, info_ptr);
    const png_byte type = png_get_color_type(png_ptr, info_ptr);

    auto pm = std::make_unique<pixmap>(width, height, to_pixel_type(type));

    const png_bytep *rows = png_get_rows(png_ptr, info_ptr);

    auto *dest = pm->data.data();
    const size_t stride = pm->row_stride();

    for (size_t i = 0; i < height; i++)
    {
        std::copy(rows[i], rows[i] + stride, dest);
        dest += stride;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, 0);

    return pm;
}
