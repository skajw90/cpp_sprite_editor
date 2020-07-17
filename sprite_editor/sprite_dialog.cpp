#include "sprite_dialog.h"
#include "ui_sprite_dialog.h"
#include <QDebug>

sprite_dialog::sprite_dialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::sprite_dialog) {
    ui->setupUi(this);
}

sprite_dialog::~sprite_dialog() { delete ui; }

std::uint16_t sprite_dialog::get_width() { return ui->widthSpinBox->value(); }

std::uint16_t sprite_dialog::get_height() { return ui->heightSpinBox->value(); }

void sprite_dialog::on_button_box_clicked(QAbstractButton* button) {
    if (get_width() != 0 && get_height() != 0) {
        this->accept();
    }
}
