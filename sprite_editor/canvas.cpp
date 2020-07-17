#include "canvas.h"

canvas::canvas(frame& f)
    : _selected_frame{&f}, _overlay{f.width(), f.height()}, _selected_color{
                                                                    0xFF} {}

frame& canvas::get_frame() { return *_selected_frame; }

void canvas::set_frame(frame& f) { _selected_frame = &f; }

color canvas::get_color() { return _selected_color; }

void canvas::set_color(color c) { _selected_color = c; }

pixel_buffer& canvas::overlay() { return _overlay; }
