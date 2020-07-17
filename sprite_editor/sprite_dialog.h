#ifndef SPRITE_DIALOG_H
#define SPRITE_DIALOG_H

#include <QAbstractButton>
#include <QDialog>
#include <cstdint>

namespace Ui {
class sprite_dialog;
}

class sprite_dialog : public QDialog {
    Q_OBJECT

public:
    explicit sprite_dialog(QWidget* parent = nullptr);
    ~sprite_dialog();
    std::uint16_t get_width();
    std::uint16_t get_height();

private slots:
    void on_button_box_clicked(QAbstractButton* button);

private:
    Ui::sprite_dialog* ui;
};

#endif // SPRITE_DIALOG_H
