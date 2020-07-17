#ifndef PIXEL_BUFFER_H
#define PIXEL_BUFFER_H

#include "color.h"
#include <cstdint>
#include <vector>

class pixel_buffer {
public:
    pixel_buffer(std::uint16_t width, std::uint16_t height);

    color& operator()(std::uint16_t x, std::uint16_t y);

    color* data();
    const color* data() const;

    pixel_buffer operator+(const pixel_buffer& over) const;
    pixel_buffer& operator+=(const pixel_buffer& over);
    std::uint16_t width() const;
    std::uint16_t height() const;
    void resize(std::uint16_t width, std::uint16_t height);

private:
    std::uint16_t _width, _height;

    std::vector<color> _data;
};

class QPixmap;

QPixmap to_pixmap(const pixel_buffer& f);

#endif // PIXEL_BUFFER_H
