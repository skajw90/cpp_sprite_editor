#include "pencil.h"

#include "canvas.h"
#include "draw_command.h"
#include "pixel_buffer.h"
#include <cmath>
#include <memory>
#include <tuple>

class pencil_command : public draw_command {
public:
    pencil_command(std::vector<pixel> pixels, color c)
        : _pixels(std::move(pixels)), _color(c) {}
    void draw(pixel_buffer& buffer) override {
        for (auto p : _pixels) {
            buffer(p.x, p.y) += _color;
        }
    }

    void undraw(pixel_buffer& buffer) override {
        for (auto p : _pixels) {
            buffer(p.x, p.y) = p.c;
        }
    }

private:
    std::vector<pixel> _pixels;
    color _color;
};

pencil_tool::pencil_tool(canvas& c, std::uint16_t x, std::uint16_t y)
    : _canvas(c), _pixels{pixel{x, y, c.get_frame()(x, y)}}, _x{x}, _y{y} {}

void pencil_tool::move_to(std::uint16_t x1, std::uint16_t y1) {
    auto x0 = _x, y0 = _y;
    _x = x1;
    _y = y1;
    auto x = std::uint16_t{}, y = std::uint16_t{};
    // based on pseudocode from
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    // a is increasing and changes faster than b
    // abstracting the actual axes allows us to use the same code for all cases
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
    // we need to skip the first step so we don't double-draw the first pixel,
    // so update diff as if we had
    if (diff > 0) {
        b += inc;
        diff -= 2 * da;
    }
    diff += 2 * db;

    // draw the last pixel iff it's different from the first one
    if (x1 != x0 || y1 != y0) {
        _pixels.push_back(pixel{x1, y1, _canvas.get_frame()(x1, y1)});
    }
    // skip the ends
    for (a = a0 + 1; a < a1; a++) {
        _pixels.push_back(pixel{x, y, _canvas.get_frame()(x, y)});
        if (diff > 0) {
            b += inc;
            diff -= 2 * da;
        }
        diff += 2 * db;
    }
}

std::unique_ptr<draw_command> pencil_tool::finalize() {
    return std::make_unique<pencil_command>(std::move(_pixels),
                                            _canvas.get_color());
}

void pencil_tool::update_view(pixel_buffer& buffer) {
    for (auto p : _pixels) {
        buffer(p.x, p.y) += _canvas.get_color();
    }
}

std::unique_ptr<draw_command> finalize(pencil_tool&& to_reify) {
    return to_reify.finalize();
}
