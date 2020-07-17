#include "pixel_buffer.h"

#include "color.h"
#include <QImage>
#include <QPixmap>
#include <algorithm>
#include <cstdint>

pixel_buffer::pixel_buffer(std::uint16_t w, std::uint16_t h)
    : _width{w}, _height{h}, _data{static_cast<decltype(_data)::size_type>(w *
                                                                           h)} {
    for (auto& p : _data) {
        p = color{0};
    }
}

color& pixel_buffer::operator()(std::uint16_t x, std::uint16_t y) {
    return _data[y * _width + x];
}

color* pixel_buffer::data() { return _data.data(); }

const color* pixel_buffer::data() const { return _data.data(); }

pixel_buffer pixel_buffer::operator+(const pixel_buffer& rhs) const {
    return pixel_buffer{*this} += rhs;
}

pixel_buffer& pixel_buffer::operator+=(const pixel_buffer& rhs) {
    for (auto i = decltype(_data)::size_type{0}; i < _data.size(); i++) {
        _data[i] += rhs._data[i];
    }
    return *this;
}

std::uint16_t pixel_buffer::width() const { return _width; }

std::uint16_t pixel_buffer::height() const { return _height; }

void pixel_buffer::resize(std::uint16_t width, std::uint16_t height) {
    auto temp = pixel_buffer{width, height};
    for (auto y = 0; y < std::min(height, _height); y++) {
        for (auto x = 0; x < std::min(width, _width); x++) {
            temp(x, y) = (*this)(x, y);
        }
    }
    *this = temp;
}

QPixmap to_pixmap(const pixel_buffer& f) {
    // not undefined behavior - the standard permits reinterpret cast to
    // unsigned char* to examine underlying representation
    auto img = QImage{reinterpret_cast<const uchar*>(f.data()),
                      f.width(),
                      f.height(),
                      QImage::Format_RGBA8888};
    auto pm = QPixmap{f.width(), f.height()};
    pm.convertFromImage(img);
    return pm;
}
