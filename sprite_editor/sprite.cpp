#include "sprite.h"
#include "frame.h"
#include "pixel.h"
#include "sprite_dialog.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <cstdint>
#include <fstream>

sprite::sprite(std::uint16_t width, std::uint16_t height)
    : _frames{}, _unsaved_changes{false} {
    _frames.emplace_back(frame{width, height});
    _width = width;
    _height = height;
}

void sprite::load(QString filename) {
    _unsaved_changes = false;
    auto f = std::ifstream{filename.toStdString()};
    qDebug() << filename.toStdString().c_str();
    auto w = std::uint16_t{}, h = std::uint16_t{}, count = std::uint16_t{};
    f >> h;
    f >> w;
    f >> count;
    _frames.clear();
    for (auto i = 0; i < count; i++) {
        add_frame(frame{w, h});
        for (auto y = 0; y < h; y++) {
            for (auto x = 0; x < w; x++) {
                // int here, then cast, because f >> will give us ascii codes
                // otherwise
                auto r = int{}, g = int{}, b = int{}, a = int{};
                f >> r;
                f >> g;
                f >> b;
                f >> a;
                get_frame(i)(x, y) = color{static_cast<std::uint8_t>(r),
                                           static_cast<std::uint8_t>(g),
                                           static_cast<std::uint8_t>(b),
                                           static_cast<std::uint8_t>(a)};
            }
        }
    }
}

void sprite::save(QString filename) {
    // resize not applied in sprite width and height
    // need to fix that
    filename = filename + ".ssp";
    auto file = QFile{filename};
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    auto write = QTextStream{&file};
    write << _height << " " << _width << endl;
    write << num_frames() << endl;
    for (auto count_frame = 0; count_frame < num_frames(); count_frame++) {
        auto& current = get_frame(count_frame);
        for (auto y = 0; y < get_frame(0).height(); y++) {
            for (auto x = 0; x < get_frame(0).width() - 1; x++) {
                write << current(x, y).r << " " << current(x, y).g << " "
                      << current(x, y).b << " " << current(x, y).a << " ";
            }
            write << current(_width - 1, y).r << " " << current(_width - 1, y).g
                  << " " << current(_width - 1, y).b << " "
                  << current(_width - 1, y).a << "\n";
        }
    }
    write.flush();
}

frame& sprite::get_frame(int index) { return _frames[index]; }

void sprite::add_frame(frame f) { _frames.emplace_back(std::move(f)); }

int sprite::num_frames() { return _frames.size(); }

void sprite::remove_frame(int index) { _frames.erase(_frames.begin() + index); }

void sprite::insert_frame(int index, frame f) {
    _frames.insert(_frames.begin() + index, std::move(f));
}

void sprite::export_to_gif(std::string filename, int delay) {
    auto settings = GifskiSettings{};
    settings.once = false;
    settings.fast = false;
    settings.quality = 100;
    settings.width = get_frame(0).width();
    settings.height = get_frame(0).height();
    _g = gifski_new(&settings);

    // Start threads for decode and encode of gif
    auto encode_thread = std::thread{&sprite::gif_encoder, this, filename};
    auto decode_thread = std::thread{&sprite::gif_decoder, this, delay};

    decode_thread.join();
    encode_thread.join();
}

void sprite::gif_decoder(int delay) {
    auto width = get_frame(0).width();
    auto height = get_frame(0).height();

    // For each frame, get the data and set it to the gif buffer
    for (auto frame_index = 0; frame_index < num_frames(); frame_index++) {
        gifski_add_frame_rgba(_g,
                              frame_index,
                              width,
                              height,
                              reinterpret_cast<const unsigned char*>(
                                      get_frame(frame_index).data()),
                              delay);
    }
    auto empty_frame = std::vector<unsigned char>(width * height * 4);
    gifski_add_frame_rgba(
            _g, num_frames() + 1, width, height, empty_frame.data(), 0);
    gifski_end_adding_frames(_g);
}

void sprite::gif_encoder(std::string filename) {
    gifski_write(_g, filename.c_str());
    gifski_drop(_g);
}

void sprite::resize(std::uint16_t width, std::uint16_t height) {
    _width = width;
    _height = height;
    for (auto frame_idx = 0; frame_idx < _frames.size(); frame_idx++) {
        _frames[frame_idx].resize(width, height);
    }
}
