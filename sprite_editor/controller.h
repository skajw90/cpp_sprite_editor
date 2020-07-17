#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "canvas.h"
#include "canvas_scene.h"
#include "main_window.h"
#include "sprite.h"
#include <QColor>

class controller : public QObject {
    Q_OBJECT
public:
    controller();
    ~controller();

    void do_new_sprite(std::uint16_t width, std::uint16_t height);
    void do_add_frame();
    void do_change_frame(int frame_index);
    void do_change_color(QColor q_color);
    void do_resize(std::uint16_t width, std::uint16_t height);
    void move_preview();
    void zoom_animation();
    void update_frame_previews(int top_frame_pos);
    void do_undo();
    void do_redo();
    void do_save();
    void do_load();
    void do_export();

private:
    template <class CanvasAction>
    void _click_listener(std::uint16_t x, std::uint16_t y) {
        auto action = std::make_shared<CanvasAction>(_canvas, x, y);
        auto drag_connection =
                _window.connect(&_window.get_edit_scene(),
                                &canvas_scene::mouse_move,
                                [this, action](uint16_t x, uint16_t y) {
                                    action->move_to(x, y);
                                    auto temp =
                                            pixel_buffer{_canvas.get_frame()};
                                    action->update_view(temp);
                                    _window.set_edit_pixmap(to_pixmap(temp));
                                });
        auto to_delete = std::make_shared<QMetaObject::Connection>();
        *to_delete = _window.connect(
                &_window.get_edit_scene(),
                &canvas_scene::mouse_up,
                [this, to_delete, action, drag_connection]() {
                    _window.disconnect(drag_connection);
                    _window.disconnect(*to_delete);
                    if (auto cmd = finalize(std::move(*action))) {
                        _canvas.get_frame().apply(std::move(cmd));
                    }
                    _window.set_edit_pixmap(to_pixmap(_canvas.get_frame()));
                    emit _window.set_frame_previews(
                            _window.get_top_frame_preview_pos());
                });
    }

    template <class CanvasAction, class Signal>
    void _connect_tool(Signal cs, Signal ds) {
        _window.connect(&_window, cs, [this, ds]() {
            auto to_disconnect =
                    _window.connect(&_window.get_edit_scene(),
                                    &canvas_scene::mouse_down,
                                    [this](auto x, auto y) {
                                        _click_listener<CanvasAction>(x, y);
                                    });
            _window.connect(&_window, ds, [this, to_disconnect]() {
                _window.disconnect(to_disconnect);
            });
        });
    }

private:
    int _preview_index;
    sprite _sprite;
    canvas _canvas;
    main_window _window;
};

#endif // CONTROLLER_H
