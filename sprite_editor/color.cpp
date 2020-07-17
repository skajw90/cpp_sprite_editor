#include "color.h"

#include <QDebug>
#include <cmath>
#include <cstdint>

float blend_alpha(float under, float over) { return over + under * (1 - over); }

float blend(float under_val,
            float over_val,
            float under_alpha,
            float over_alpha) {
    return (over_val * over_alpha +
            under_val * under_alpha * (1 - over_alpha)) /
           blend_alpha(under_alpha, over_alpha);
}

// byte color space value to float color space after accounting for gamma
// correction
float vtofs(std::uint8_t v) { return std::pow(v / 255.f, 2.2f); }

// converting back from above
std::uint8_t vtobs(float v) {
    return static_cast<std::uint8_t>(std::pow(v, 1.0f / 2.2f) * 255.f);
}

// byte color space alpha to float color space - no gamma correction
float atofs(std::uint8_t v) { return v / 255.f; }

// converting back from above
std::uint8_t atobs(float v) { return static_cast<std::uint8_t>(v * 255.f); }

std::uint8_t blend_alpha(std::uint8_t under, std::uint8_t over) {
    return atobs(blend_alpha(atofs(under), atofs(over)));
}

std::uint8_t blend(std::uint8_t under_val,
                   std::uint8_t over_val,
                   std::uint8_t under_alpha,
                   std::uint8_t over_alpha) {
    return vtobs(blend(vtofs(under_val),
                       vtofs(over_val),
                       atofs(under_alpha),
                       atofs(over_alpha)));
}

std::uint8_t get_byte(std::uint32_t from, int byte) {
    return (from >> (byte * 8)) & 0xFF;
}

color::color() : color{0} {}

color::color(std::uint32_t raw)
    : r{get_byte(raw, 3)}, g{get_byte(raw, 2)}, b{get_byte(raw, 1)},
      a{get_byte(raw, 0)} {}

color::color(std::uint8_t red,
             std::uint8_t green,
             std::uint8_t blue,
             std::uint8_t alpha)
    : r{red}, g{green}, b{blue}, a{alpha} {}

color color::operator+(color rhs) {
    return color{blend(r, rhs.r, a, rhs.a),
                 blend(g, rhs.g, a, rhs.a),
                 blend(b, rhs.b, a, rhs.a),
                 blend_alpha(a, rhs.a)};
}

color& color::operator+=(color rhs) { return *this = *this + rhs; }

bool color::operator==(color rhs) {
    return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
}
