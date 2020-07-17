#ifndef PENCIL_H
#define PENCIL_H

#include "draw_command.h"
#include "pixel.h"
#include "pixel_buffer.h"
#include <cstdint>
#include <memory>
#include <vector>

class canvas;

class pencil_tool {
public:
    pencil_tool(canvas& c, std::uint16_t x, std::uint16_t y);
    void move_to(std::uint16_t x, std::uint16_t y);
    std::unique_ptr<draw_command> finalize();

    void update_view(pixel_buffer& buffer);

private:
    canvas& _canvas;
    std::vector<pixel> _pixels;
    std::uint16_t _x, _y;
    friend std::unique_ptr<draw_command> finalize(pencil_tool&&);
};

std::unique_ptr<draw_command> finalize(pencil_tool&& to_reify);

#endif // PENCIL_H
