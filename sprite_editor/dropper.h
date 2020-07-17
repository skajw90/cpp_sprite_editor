#ifndef DROPPER_H
#define DROPPER_H
#include "draw_command.h"
#include "pixel.h"
#include <cstdint>
#include <memory>
#include <vector>

class canvas;
class dropper_tool {
public:
    dropper_tool(canvas& c, std::uint16_t x, std::uint16_t y);
    void move_to(std::uint16_t x, std::uint16_t y);
    void update_view(pixel_buffer& buffer);

private:
    std::unique_ptr<draw_command> finalize();
    friend std::unique_ptr<draw_command> finalize(dropper_tool&&);
};

std::unique_ptr<draw_command> finalize(dropper_tool&& to_reify);

#endif // DROPPER_H
