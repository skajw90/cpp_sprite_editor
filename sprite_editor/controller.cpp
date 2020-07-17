#include "controller.h"

#include "dropper.h"
#include "eraser.h"
#include "fill.h"
#include "frame.h"
#include "pencil.h"
#include "rectangle.h"
#include <QColorDialog>
#include <QDebug>

controller::controller()
    : _preview_index{0}, _sprite{32, 32}, _canvas{_sprite.get_frame(0)},
      _window{} {
    {
        auto init_edit = QPixmap{32, 32};
        init_edit.fill(QColor{0, 0, 0, 0});
        _window.set_edit_pixmap(init_edit);
    }

    QObject::connect(&_window,
                     &main_window::new_sprite,
                     this,
                     &controller::do_new_sprite);

    QObject::connect(
            &_window, &main_window::new_frame, this, &controller::do_add_frame);

    QObject::connect(&_window,
                     &main_window::change_frame,
                     this,
                     &controller::do_change_frame);

    QObject::connect(&_window,
                     &main_window::set_color,
                     this,
                     &controller::do_change_color);

    QObject::connect(
            &_window, &main_window::resize_frame, this, &controller::do_resize);

    QObject::connect(&_window,
                     &main_window::animate_sprite,
                     this,
                     &controller::move_preview);

    QObject::connect(&_window,
                     &main_window::set_frame_previews,
                     this,
                     &controller::update_frame_previews);

    QObject::connect(&_window, &main_window::undo, this, &controller::do_undo);

    QObject::connect(&_window, &main_window::redo, this, &controller::do_redo);

    QObject::connect(
            &_window, &main_window::save_sprite, this, &controller::do_save);

    QObject::connect(
            &_window, &main_window::load_sprite, this, &controller::do_load);

    QObject::connect(
            &_window, &main_window::export_gif, this, &controller::do_export);

    _connect_tool<pencil_tool>(&main_window::enable_pencil,
                               &main_window::disable_pencil);
    _connect_tool<fill_tool>(&main_window::enable_fill,
                             &main_window::disable_fill);
    _connect_tool<eraser_tool>(&main_window::enable_eraser,
                               &main_window::disable_eraser);
    _connect_tool<dropper_tool>(&main_window::enable_dropper,
                                &main_window::disable_dropper);
    _connect_tool<rectangle_tool>(&main_window::enable_rectangle,
                                  &main_window::disable_rectangle);

    _window.show();
}

controller::~controller(){};

void controller::do_new_sprite(std::uint16_t width, std::uint16_t height) {
    _sprite = sprite{width, height};
    _window.set_edit_pixmap(to_pixmap(_sprite.get_frame(0)));
    _canvas = canvas{_sprite.get_frame(0)};
}

void controller::do_add_frame() {
    auto new_frame =
            frame{_canvas.get_frame().width(), _canvas.get_frame().height()};
    _sprite.add_frame(std::move(new_frame));
    _canvas.set_frame(_sprite.get_frame(_sprite.num_frames() - 1));

    auto init_edit =
            QPixmap{_canvas.get_frame().width(), _canvas.get_frame().height()};
    init_edit.fill(QColor{0, 0, 0, 0});
    _window.set_edit_pixmap(init_edit);
}

void controller::do_change_frame(int frame_index) {
    _canvas.set_frame(_sprite.get_frame(frame_index - 1));
    _window.set_edit_pixmap(to_pixmap(_canvas.get_frame()));
}

void controller::do_change_color(QColor q_color) {
    auto selected_color = color{static_cast<std::uint8_t>(q_color.red()),
                                static_cast<std::uint8_t>(q_color.green()),
                                static_cast<std::uint8_t>(q_color.blue()),
                                static_cast<std::uint8_t>(q_color.alpha())};
    _canvas.set_color(selected_color);
}

void controller::do_resize(std::uint16_t width, std::uint16_t height) {
    _sprite.resize(width, height);
    _window.set_edit_pixmap(to_pixmap(_canvas.get_frame()));
}

void controller::move_preview() {
    _window.set_preview_pixmap(to_pixmap(_sprite.get_frame(_preview_index)));
    _preview_index = (_preview_index + 1) % _sprite.num_frames();
}

void controller::update_frame_previews(int top_frame_pos) {
    auto num_frames = _sprite.num_frames();
    auto empty_pix = QPixmap{0, 0};
    if (num_frames == 0) {
        _window.set_frame_preview_pixmap(empty_pix, empty_pix, empty_pix);
    } else if (num_frames == 1) {
        _window.set_frame_preview_pixmap(
                to_pixmap(_sprite.get_frame(top_frame_pos - 1)),
                empty_pix,
                empty_pix);
    } else if (num_frames == 2) {
        _window.set_frame_preview_pixmap(
                to_pixmap(_sprite.get_frame(top_frame_pos - 1)),
                to_pixmap(_sprite.get_frame(top_frame_pos)),
                empty_pix);
    } else {
        _window.set_frame_preview_pixmap(
                to_pixmap(_sprite.get_frame(top_frame_pos - 1)),
                to_pixmap(_sprite.get_frame(top_frame_pos)),
                to_pixmap(_sprite.get_frame(top_frame_pos + 1)));
    }
}

void controller::do_undo() {
    if (_canvas.get_frame().can_undo()) {
        _canvas.get_frame().undo();
        _window.set_edit_pixmap(to_pixmap(_canvas.get_frame()));
    }
}

void controller::do_redo() {
    if (_canvas.get_frame().can_redo()) {
        _canvas.get_frame().redo();
        _window.set_edit_pixmap(to_pixmap(_canvas.get_frame()));
    }
}

void controller::do_save() { _sprite.save(_window.get_file_name()); }

void controller::do_load() {
    _sprite.load(_window.get_file_name());
    _canvas.set_frame(_sprite.get_frame(0));
    _window.set_edit_pixmap(to_pixmap(_canvas.get_frame()));
    _window.set_frame_spin_box_values(_sprite.num_frames());
}

void controller::do_export() {
    _sprite.export_to_gif(_window.get_file_name().toUtf8().constData(),
                          100 / _window.get_fps());
}
