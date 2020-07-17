#include "fill.h"
#include "canvas.h"
#include "draw_command.h"
#include "pixel_buffer.h"
#include <QDebug>
#include <cmath>
#include <memory>
#include <pixel.h>
#include <queue>
#include <tuple>

class fill_command : public draw_command {
public:
    fill_command(color fill, color old, std::uint16_t x, std::uint16_t y)
        : _x{x}, _y{y}, _fill_color{fill}, _original_color{old}, _pixels{} {}

    void draw(pixel_buffer& f) override {
        color _current = f(_x, _y);
        _pixels.push_back(pixel{_x, _y, _current});
        flood_fill(f, _fill_color, _current, _x, _y);
    }

    void undraw(pixel_buffer& f) override {
        for (auto p : _pixels) {
            f(p.x, p.y) = p.c;
        }
    }

    void flood_fill(pixel_buffer& f,
                    color fill,
                    color old,
                    std::uint16_t x,
                    std::uint16_t y) {
        if (fill == old) {
            return;
        }
        if (!(f(x, y) == old)) {
            return;
        }
        auto pixq = std::queue<std::pair<std::uint16_t, std::uint16_t>>{};
        auto width = f.width();
        auto height = f.height();
        f(x, y) = fill;
        pixq.push(std::pair(x, y));
        while (!pixq.empty()) {
            auto n = std::pair<std::uint16_t, std::uint16_t>{};
            n.first = pixq.front().first;
            n.second = pixq.front().second;
            pixq.pop();

            if (check_bounds(n.first - 1, n.second, width, height)) {
                if (old == f(n.first - 1, n.second)) {
                    _pixels.push_back(
                            pixel{static_cast<std::uint16_t>(n.first - 1),
                                  n.second,
                                  old});
                    pixq.push(std::pair(n.first - 1, n.second));
                    f(n.first - 1, n.second) = fill;
                }
            }

            if (check_bounds(n.first + 1, n.second, width, height)) {
                if (old == f(n.first + 1, n.second)) {
                    _pixels.push_back(
                            pixel{static_cast<std::uint16_t>(n.first + 1),
                                  n.second,
                                  old});

                    pixq.push(std::pair(n.first + 1, n.second));
                    f(n.first + 1, n.second) = fill;
                }
            }

            if (check_bounds(n.first, n.second - 1, width, height)) {
                if (old == f(n.first, n.second - 1)) {
                    _pixels.push_back(
                            pixel{n.first,
                                  static_cast<std::uint16_t>(n.second - 1),
                                  old});

                    pixq.push(std::pair(n.first, n.second - 1));
                    f(n.first, n.second - 1) = fill;
                }
            }

            if (check_bounds(n.first, n.second + 1, width, height)) {
                if (old == f(n.first, n.second + 1)) {
                    _pixels.push_back(
                            pixel{n.first,
                                  static_cast<std::uint16_t>(n.second + 1),
                                  old});

                    pixq.push(std::pair(n.first, n.second + 1));
                    f(n.first, n.second + 1) = fill;
                }
            }
        }

        //        if(x < f.width()  && y<f.height() ){
        //        if(old == f(x,y)){
        //        qDebug () <<x << " "<< y;
        //        f(x, y) += fill;
        //        flood_fill(f,fill, old, x+1, y);
        //        flood_fill(f,fill, old, x-1, y);
        //        flood_fill(f,fill, old, x, y+1);
        //        flood_fill(f,fill, old, x, y-1);
        //            }
        //        }
    }

    bool check_bounds(int x, int y, int width, int height) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

private:
    std::uint16_t _x, _y;
    color _fill_color, _original_color;
    std::vector<pixel> _pixels;
};

fill_tool::fill_tool(canvas& c, std::uint16_t x, std::uint16_t y)
    : _canvas(c), _x{x}, _y{y} {
    _old = _canvas.get_color();
    _fill = _canvas.get_color();
}

void fill_tool::move_to(std::uint16_t, std::uint16_t) {}

std::unique_ptr<draw_command> fill_tool::finalize() {
    return std::make_unique<fill_command>(_fill, _old, _x, _y);
}

void fill_tool::update_view(pixel_buffer& buffer) {
    for (auto p : _pixels) {
        buffer(p.x, p.y) = p.c;
    }
}

std::unique_ptr<draw_command> finalize(fill_tool&& to_reify) {
    return to_reify.finalize();
}
