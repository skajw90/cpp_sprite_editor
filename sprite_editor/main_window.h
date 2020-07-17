#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <QString>
#include <canvas_scene.h>
#include <sprite_dialog.h>

namespace Ui {
class main_window;
}

class main_window : public QMainWindow {
    Q_OBJECT

public:
    explicit main_window(QWidget* parent = nullptr);
    ~main_window();

    canvas_scene& get_edit_scene();
    void set_edit_pixmap(const QPixmap& pixmap);
    void set_preview_pixmap(const QPixmap& pixmap);
    void set_frame_preview_pixmap(const QPixmap& pixmap1,
                                  const QPixmap& pixmap2,
                                  const QPixmap& pixmap3);
    int get_top_frame_preview_pos();
    void set_file_name(const QString filename);
    void set_frame_spin_box_values(int max);
    QString get_file_name();
    int get_fps();

public slots:
    void on_action_new_sprite_triggered();
    void color_picker();
    void zoom();
signals:
    void enable_pencil();
    void disable_pencil();
    void new_frame();
    void change_frame(int val);
    void enable_fill();
    void disable_fill();
    void enable_eraser();
    void disable_eraser();
    void enable_rectangle();
    void disable_rectangle();
    void enable_dropper();
    void disable_dropper();
    void set_color(QColor q_color);
    void resize_frame(std::uint16_t width, std::uint16_t height);
    void new_sprite(std::uint16_t width, std::uint16_t height);
    void undo();
    void redo();
    void animate_sprite();
    void set_frame_previews(int top_frame_pos);
    void save_sprite();
    void load_sprite();
    void export_gif();

private slots:
    void on_pen_tool_button_toggled(bool checked);
    void on_fill_tool_button_toggled(bool checked);
    void on_eraser_tool_button_toggled(bool checked);
    void on_action_add_frame_triggered();
    void on_frame_spin_box_valueChanged(int arg1);
    void on_action_resize_triggered();
    void on_fps_selector_valueChanged(int fps);
    void resize_dialog_ok();
    void new_sprite_dialog_ok();
    void animate_timer();
    void on_frame_preview_scroll_sliderMoved(int position);

    void on_action_undo_triggered();

    void on_action_redo_triggered();

    void on_action_save_triggered();

    void on_action_open_triggered();

    void on_dropper_tool_button_toggled(bool checked);

    void on_rectangle_tool_button_toggled(bool checked);

    void on_animation_zoom_button_toggled(bool checked);

    void on_action_export_as_GIF_triggered();

private:
    Ui::main_window* _ui;
    QGraphicsPixmapItem* _pixmap_item;
    QGraphicsPixmapItem* _preview_pix;
    QGraphicsPixmapItem* _frame_1_pix;
    QGraphicsPixmapItem* _frame_2_pix;
    QGraphicsPixmapItem* _frame_3_pix;
    sprite_dialog* _resize_dialog;
    sprite_dialog* _new_sprite_dialog;
    QTimer* preview_timer;
    int _preview_animation_zoom;
    int _top_frame_pos;
    QString _filename;
    int _fps;
};

#endif // MAIN_WINDOW_H
