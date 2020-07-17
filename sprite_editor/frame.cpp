#include "frame.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <cstdint>

frame::frame(std::uint16_t width, std::uint16_t height)
    : pixel_buffer{width, height}, _commands{}, _commands_pos{
                                                        _commands.begin()} {}

void frame::apply(std::unique_ptr<draw_command> to_apply) {
    _commands.erase(_commands_pos, _commands.end());
    to_apply->draw(*this);
    _commands.push_back(std::move(to_apply));
    _commands_pos = _commands.end();
}

void frame::undo() { (*--_commands_pos)->undraw(*this); }

bool frame::can_undo() const { return _commands_pos != _commands.begin(); }

void frame::redo() {
    (*_commands_pos)->draw(*this);
    _commands_pos++;
}

bool frame::can_redo() const { return _commands_pos != _commands.end(); }
