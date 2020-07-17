#include "dropper.h"
#include "canvas.h"
#include "pixel_buffer.h"
#include <QDebug>
#include <cmath>
#include <memory>
#include <tuple>

dropper_tool::dropper_tool(canvas& c, std::uint16_t x, std::uint16_t y) {
    c.set_color(c.get_frame()(x, y));
}

void dropper_tool::move_to(std::uint16_t x1, std::uint16_t y1) {}

std::unique_ptr<draw_command> dropper_tool::finalize() {
    return std::unique_ptr<draw_command>(nullptr);
}

void dropper_tool::update_view(pixel_buffer&) {}

std::unique_ptr<draw_command> finalize(dropper_tool&& to_reify) {
    return to_reify.finalize();
}
