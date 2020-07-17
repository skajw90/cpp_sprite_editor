#ifndef CANVAS_H
#define CANVAS_H

#include "color.h"
#include "frame.h"

class canvas {
public:
    canvas(frame& f);
    frame& get_frame();
    void set_frame(frame& f);

    color get_color();
    void set_color(color c);

    pixel_buffer& overlay();

private:
    frame* _selected_frame;
    pixel_buffer _overlay;
    color _selected_color;
};

#endif // CANVAS_H
