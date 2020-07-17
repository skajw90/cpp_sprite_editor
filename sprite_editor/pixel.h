#ifndef PIXEL_H
#define PIXEL_H

#include "color.h"
#include <cstdint>

struct pixel {
    std::uint16_t x, y;
    color c;
};

#endif // PIXEL_H
