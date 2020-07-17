#include "rectangle.h"

#include "canvas.h"
#include "draw_command.h"
#include "pixel_buffer.h"
#include <cmath>
#include <memory>
#include <tuple>

class rectangle_command : public draw_command {
public:
    rectangle_command(std::uint16_t x1,
                      std::uint16_t y1,
                      std::uint16_t x2,
                      std::uint16_t y2,
                      std::vector<color> overwritten,
                      color c)
        : _x1{x1}, _y1{y1}, _x2{x2}, _y2{y2},
          _overwritten{std::move(overwritten)}, _color{c} {}
    void draw(pixel_buffer& buffer) override {
        for (auto y = _y1; y <= _y2; y++) {
            for (auto x = _x1; x <= _x2; x++) {
                buffer(x, y) = _color;
            }
        }
    }

    void undraw(pixel_buffer& buffer) override {
        for (auto y = _y1; y <= _y2; y++) {
            for (auto x = _x1; x <= _x2; x++) {
                buffer(x, y) =
                        _overwritten[(y - _y1) * (_x2 - _x1) + (x - _x1)];
            }
        }
    }

private:
    std::uint16_t _x1, _y1, _x2, _y2;
    std::vector<color> _overwritten;
    color _color;
};

rectangle_tool::rectangle_tool(canvas& c, std::uint16_t x, std::uint16_t y)
    : _canvas(c), _x1{x}, _y1{y}, _x2{x}, _y2{y} {}

void rectangle_tool::move_to(std::uint16_t x, std::uint16_t y) {
    _x2 = x;
    _y2 = y;
}

std::unique_ptr<draw_command> rectangle_tool::finalize() {
    // normalize to top left to bottom right
    std::tie(_x1, _y1, _x2, _y2) = std::make_tuple(std::min(_x1, _x2),
                                                   std::min(_y1, _y2),
                                                   std::max(_x1, _x2),
                                                   std::max(_y1, _y2));
    auto overwritten = std::vector<color>{
            static_cast<std::vector<color>::size_type>(1 + _x2 - _x1) *
            (1 + _y2 - _y1)};
    for (auto y = _y1; y <= _y2; y++) {
        for (auto x = _x1; x <= _x2; x++) {
            overwritten[(y - _y1) * (_x2 - _x1) + (x - _x1)] =
                    _canvas.get_frame()(x, y);
        }
    }
    return std::make_unique<rectangle_command>(
            _x1, _y1, _x2, _y2, std::move(overwritten), _canvas.get_color());
}

void rectangle_tool::update_view(pixel_buffer& buffer) {
    auto[x1, y1, x2, y2] = std::make_tuple(std::min(_x1, _x2),
                                           std::min(_y1, _y2),
                                           std::max(_x1, _x2),
                                           std::max(_y1, _y2));
    for (auto y = y1; y <= y2; y++) {
        for (auto x = x1; x <= x2; x++) {
            buffer(x, y) = _canvas.get_color();
        }
    }
}

std::unique_ptr<draw_command> finalize(rectangle_tool&& to_reify) {
    return to_reify.finalize();
}
