#ifndef ERASER_H
#define ERASER_H

#include "draw_command.h"
#include "pixel.h"
#include <cstdint>
#include <memory>
#include <vector>

class canvas;

class eraser_tool {
public:
    eraser_tool(canvas& c, std::uint16_t x, std::uint16_t y);
    void move_to(std::uint16_t x, std::uint16_t y);
    std::unique_ptr<draw_command> finalize();

    void update_view(pixel_buffer& buffer);

private:
    canvas& _canvas;
    std::vector<pixel> _pixels;
    std::uint16_t _x, _y;
    color _white;

    friend std::unique_ptr<draw_command> finalize(eraser_tool&&);
};

std::unique_ptr<draw_command> finalize(eraser_tool&& to_reify);

#endif // ERASER_H
