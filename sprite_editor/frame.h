#ifndef FRAME_H
#define FRAME_H

#include "color.h"
#include "draw_command.h"
#include "pixel_buffer.h"
#include <cstdint>
#include <memory>
#include <vector>

class frame : public pixel_buffer {
public:
    frame(std::uint16_t width, std::uint16_t height);

    void apply(std::unique_ptr<draw_command> to_apply);

    void undo();
    bool can_undo() const;

    void redo();
    bool can_redo() const;

private:
    std::vector<std::unique_ptr<draw_command>> _commands;
    std::vector<std::unique_ptr<draw_command>>::iterator _commands_pos;
};

#endif // FRAME_H
