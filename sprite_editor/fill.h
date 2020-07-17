#ifndef FILL_H
#define FILL_H

#include "draw_command.h"
#include "pixel.h"
#include <cstdint>
#include <memory>
#include <vector>

class canvas;
class pixel_buffer;

class fill_tool {
public:
    fill_tool(canvas& c, std::uint16_t x, std::uint16_t y);
    void move_to(std::uint16_t x, std::uint16_t y);
    std::unique_ptr<draw_command> finalize();

    void update_view(pixel_buffer& buffer);

private:
    canvas& _canvas;
    std::uint16_t _x, _y;
    color _fill, _old;
    std::vector<pixel> _pixels;
    friend std::unique_ptr<draw_command> finalize(fill_tool&&);
};

std::unique_ptr<draw_command> finalize(fill_tool&& to_reify);

#endif // FILL_H
