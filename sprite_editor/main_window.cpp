#include "main_window.h"
#include "canvas_scene.h"
#include "sprite_dialog.h"
#include "ui_main_window.h"
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QGraphicsItem>
#include <QTimer>
#include <QToolButton>

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include <sprite.h>

main_window::main_window(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::main_window) {
    _ui->setupUi(this);
    _ui->edit_view->setScene(new canvas_scene{this});
    _ui->edit_view->scale(_ui->zoom_slider->value(), _ui->zoom_slider->value());
    _pixmap_item = _ui->edit_view->scene()->addPixmap(QPixmap{});
    _ui->animation_preview->setScene(new canvas_scene{this});
    _preview_pix = _ui->animation_preview->scene()->addPixmap(QPixmap{});
    _preview_animation_zoom = 0;

    _top_frame_pos = 1;
    _ui->frame_preview_1->setScene(new canvas_scene{this});
    _frame_1_pix = _ui->frame_preview_1->scene()->addPixmap(QPixmap{});
    _ui->frame_preview_2->setScene(new canvas_scene{this});
    _frame_2_pix = _ui->frame_preview_2->scene()->addPixmap(QPixmap{});
    _ui->frame_preview_3->setScene(new canvas_scene{this});
    _frame_3_pix = _ui->frame_preview_3->scene()->addPixmap(QPixmap{});

    _resize_dialog = new sprite_dialog(this);
    _new_sprite_dialog = new sprite_dialog(this);
    connect(_ui->zoom_slider, &QSlider::valueChanged, this, &main_window::zoom);
    connect(_ui->color_picker_button,
            &QToolButton::clicked,
            this,
            &main_window::color_picker);
    connect(_resize_dialog,
            &sprite_dialog::accepted,
            this,
            &main_window::resize_dialog_ok);
    connect(_new_sprite_dialog,
            &sprite_dialog::accepted,
            this,
            &main_window::new_sprite_dialog_ok);
    auto fps = 1000 / _ui->fps_selector->value();
    _fps = _ui->fps_selector->value();
    preview_timer = new QTimer(this);
    connect(preview_timer, &QTimer::timeout, this, &main_window::animate_timer);
    preview_timer->start(fps);
    emit set_frame_previews(get_top_frame_preview_pos());
}

main_window::~main_window() { delete _ui; }

void main_window::on_action_new_sprite_triggered() {
    _new_sprite_dialog->show();
}

void main_window::on_action_add_frame_triggered() {
    emit new_frame();
    _ui->frame_spin_box->setMaximum((_ui->frame_spin_box->maximum()) + 1);
    _ui->frame_spin_box->setValue((_ui->frame_spin_box->value()) + 1);
    if (_ui->frame_spin_box->maximum() > 3)
        _ui->frame_preview_scroll->setMaximum(_ui->frame_spin_box->maximum() -
                                              2);
    emit set_frame_previews(_ui->frame_preview_scroll->value());
}

void main_window::set_frame_spin_box_values(int max) {
    _ui->frame_spin_box->setMaximum(max);
    if (_ui->frame_spin_box->maximum() > 3)
        _ui->frame_preview_scroll->setMaximum(_ui->frame_spin_box->maximum() -
                                              2);
    emit set_frame_previews(_ui->frame_preview_scroll->value());
}

void main_window::set_edit_pixmap(const QPixmap& pixmap) {
    _pixmap_item->setPixmap(pixmap);
    _ui->edit_view->scene()->setSceneRect(
            0, 0, pixmap.width(), pixmap.height());
}

void main_window::set_preview_pixmap(const QPixmap& pixmap) {
    _ui->animation_preview->scene()->setSceneRect(
            0, 0, pixmap.width(), pixmap.height());
    _preview_pix->setPixmap(pixmap);
}

void main_window::set_frame_preview_pixmap(const QPixmap& pixmap1,
                                           const QPixmap& pixmap2,
                                           const QPixmap& pixmap3) {
    _frame_1_pix->setPixmap(pixmap1);
    _frame_2_pix->setPixmap(pixmap2);
    _frame_3_pix->setPixmap(pixmap3);
}

int main_window::get_top_frame_preview_pos() { return _top_frame_pos; }

void main_window::zoom() {
    _ui->edit_view->resetMatrix();
    _ui->edit_view->scale(_ui->zoom_slider->value(), _ui->zoom_slider->value());
}

void main_window::color_picker() {
    auto q_color = QColorDialog::getColor(
            Qt::black, this, "Pick A Color", QColorDialog::ShowAlphaChannel);

    emit set_color(q_color);
}

canvas_scene& main_window::get_edit_scene() {
    return *dynamic_cast<canvas_scene*>(_ui->edit_view->scene());
}

void main_window::on_pen_tool_button_toggled(bool checked) {
    if (checked) {
        emit enable_pencil();
    } else {
        emit disable_pencil();
    }
}

void main_window::on_fill_tool_button_toggled(bool checked) {
    if (checked) {
        emit enable_fill();
    } else {
        emit disable_fill();
    }
}

void main_window::on_eraser_tool_button_toggled(bool checked) {
    if (checked) {
        emit enable_eraser();
    } else {
        emit disable_eraser();
    }
}

void main_window::on_dropper_tool_button_toggled(bool checked) {
    if (checked) {
        emit enable_dropper();
    } else {
        emit disable_dropper();
    }
}

void main_window::on_frame_spin_box_valueChanged(int frame_index) {
    emit change_frame(frame_index);
}

void main_window::on_action_save_triggered() {
    auto filename = QFileDialog::getSaveFileName(
            this,
            tr("Save Sprite Sheet Project"),
            "",
            tr("Sprite Sheet (*.ssp);;All Files (*)"));
    if (filename.isEmpty()) {
        return;
    }

    set_file_name(filename);
    emit save_sprite();
}

void main_window::set_file_name(const QString filename) {
    _filename = filename;
}

QString main_window::get_file_name() { return _filename; }

int main_window::get_fps() { return _fps; }

void main_window::on_action_open_triggered() {
    QString filename = QFileDialog::getOpenFileName(
            this,
            tr("Open Sprite Sheet"),
            "",
            tr("Sprite Sheet (*.ssp);;All Files (*)"));
    if (filename.isEmpty()) {
        return;
    }
    set_file_name(filename);
    emit load_sprite();
}

void main_window::on_action_resize_triggered() { _resize_dialog->show(); }

void main_window::resize_dialog_ok() {
    emit resize_frame(_resize_dialog->get_width(),
                      _resize_dialog->get_height());
}

void main_window::new_sprite_dialog_ok() {
    emit new_sprite(_new_sprite_dialog->get_width(),
                    _new_sprite_dialog->get_height());
}

void main_window::on_action_undo_triggered() { emit undo(); }

void main_window::on_action_redo_triggered() { emit redo(); }
void main_window::on_fps_selector_valueChanged(int fps) {
    preview_timer->setInterval(1000 / fps);
    _ui->fps_label->setText(QString("%1 FPS").arg(fps));
    _fps = fps;
}

void main_window::animate_timer() { emit animate_sprite(); }

void main_window::on_frame_preview_scroll_sliderMoved(int position) {
    _top_frame_pos = position;
    _ui->frame_label_1->setText(QString::number(position));
    _ui->frame_label_2->setText(QString::number(position + 1));
    _ui->frame_label_3->setText(QString::number(position + 2));
    emit set_frame_previews(position);
}

void main_window::on_rectangle_tool_button_toggled(bool checked) {
    if (checked) {
        emit enable_rectangle();
    } else {
        emit disable_rectangle();
    }
}

void main_window::on_animation_zoom_button_toggled(bool checked) {
    _ui->animation_preview->resetMatrix();
    if (checked) {
        auto rect = _preview_pix->scene()->sceneRect();
        auto factor =
                0.9 *
                std::min(_ui->animation_preview->width() / rect.width(),
                         _ui->animation_preview->height() / rect.height());
        _ui->animation_preview->scale(factor, factor);
    }
}

void main_window::on_action_export_as_GIF_triggered() {
    auto filename = QFileDialog::getSaveFileName(
            this, tr("Export GIF"), "", tr("GIF (*.gif);;All Files (*)"));

    if (filename.isEmpty()) {
        return;
    }
    filename += ".gif";
    set_file_name(filename);
    emit export_gif();
}
