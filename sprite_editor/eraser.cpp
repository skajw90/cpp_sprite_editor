#include "eraser.h"
#include "canvas.h"
#include "pixel_buffer.h"
#include <cmath>
#include <memory>
#include <tuple>

class eraser_command : public draw_command {
public:
    eraser_command(std::vector<pixel> pixels) : _pixels(std::move(pixels)) {}

    void draw(pixel_buffer& f) override {
        for (auto p : _pixels) {
            f(p.x, p.y) = color{0};
        }
    }

    void undraw(pixel_buffer& f) override {
        for (auto p : _pixels) {
            f(p.x, p.y) = p.c;
        }
    }

private:
    std::vector<pixel> _pixels;
};

eraser_tool::eraser_tool(canvas& c, std::uint16_t x, std::uint16_t y)
    : _canvas(c), _pixels{pixel{x, y, c.get_frame()(x, y)}}, _x{x}, _y{y} {}

void eraser_tool::move_to(std::uint16_t x1, std::uint16_t y1) {
    auto x0 = _x, y0 = _y;
    _x = x1;
    _y = y1;
    auto x = std::uint16_t{}, y = std::uint16_t{};
    using std::abs;
    using std::tie;
    auto [a, b, a0, b0, a1, b1] =
            abs(y1 - y0) < abs(x1 - x0) ? (x0 > x1 ? tie(x, y, x1, y1, x0, y0)
                                                   : tie(x, y, x0, y0, x1, y1))
                                        : (y0 > y1 ? tie(y, x, y1, x1, y0, x0)
                                                   : tie(y, x, y0, x0, y1, x1));
    auto da = a1 - a0;
    auto db = abs(b1 - b0);
    auto inc = b1 < b0 ? -1 : 1;
    auto diff = (2 * db - da);
    a = a0;
    b = b0;

    for (a = a0; a <= a1; a++) {
        _pixels.push_back(pixel{x, y, _canvas.get_frame()(x, y)});
        _canvas.overlay()(x, y) = _white;
        if (diff > 0) {
            b += inc;
            diff -= 2 * da;
        }
        diff += 2 * db;
    }
}

std::unique_ptr<draw_command> eraser_tool::finalize() {
    return std::make_unique<eraser_command>(std::move(_pixels));
}

void eraser_tool::update_view(pixel_buffer& buffer) {
    for (auto p : _pixels) {
        buffer(p.x, p.y) = color{0};
    }
}

std::unique_ptr<draw_command> finalize(eraser_tool&& to_reify) {
    return to_reify.finalize();
}
