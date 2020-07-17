#ifndef DRAW_COMMAND_H
#define DRAW_COMMAND_H

class pixel_buffer;

class draw_command {
public:
    virtual ~draw_command() = default;
    virtual void draw(pixel_buffer& c) = 0;
    virtual void undraw(pixel_buffer& c) = 0;
};

#endif // DRAW_COMMAND_H
