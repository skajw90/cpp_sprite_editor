#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

struct color {
    color();
    explicit color(std::uint32_t raw);
    explicit color(std::uint8_t red,
                   std::uint8_t green,
                   std::uint8_t blue,
                   std::uint8_t alpha);

    std::uint8_t r, g, b, a;

    color operator+(color rhs);
    color& operator+=(color rhs);
    bool operator==(color rhs);
};

#endif // COLOR_H
