#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "draw_command.h"
#include "pixel.h"
#include "pixel_buffer.h"
#include <cstdint>
#include <memory>
#include <vector>

class canvas;

class rectangle_tool {
public:
    rectangle_tool(canvas& c, std::uint16_t x, std::uint16_t y);
    void move_to(std::uint16_t x, std::uint16_t y);

    void update_view(pixel_buffer& buffer);

private:
    std::unique_ptr<draw_command> finalize();

    canvas& _canvas;
    std::uint16_t _x1, _y1, _x2, _y2;

    friend std::unique_ptr<draw_command> finalize(rectangle_tool&&);
};

std::unique_ptr<draw_command> finalize(rectangle_tool&& to_reify);

#endif // RECTANGLE_H
