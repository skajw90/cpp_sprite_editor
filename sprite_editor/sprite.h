#ifndef SPRITE_H
#define SPRITE_H

#include "gifski.h"
#include <QString>
#include <QString>
#include <cstdint>
#include <frame.h>
#include <thread>
#include <vector>

class sprite {
public:
    sprite(std::uint16_t width, std::uint16_t height);
    void load(QString filename);
    frame& get_frame(int index);
    int num_frames();
    void add_frame(frame f);
    void remove_frame(int index);
    void insert_frame(int index, frame f);
    void save(QString filename);
    void export_to_gif(std::string filename, int delay);
    void resize(std::uint16_t width, std::uint16_t height);
    void gif_encoder(std::string filename);
    void gif_decoder(int delay);

private:
    std::vector<frame> _frames;
    bool _unsaved_changes;
    std::uint16_t _width;
    std::uint16_t _height;
    gifski* _g;
};

#endif // SPRITE_H
