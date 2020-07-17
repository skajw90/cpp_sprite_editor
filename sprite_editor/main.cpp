#include "canvas.h"
#include "canvas_scene.h"
#include "controller.h"
#include "dropper.h"
#include "eraser.h"
#include "fill.h"
#include "main_window.h"
#include "pencil.h"
#include "pixel_buffer.h"
#include "rectangle.h"
#include "sprite.h"
#include "sprite_dialog.h"
#include <QApplication>
#include <QColorDialog>
#include <QDebug>
#include <memory>

int main(int argc, char* argv[]) {
    auto app = QApplication{argc, argv};
    auto control = controller{};

    return app.exec();
}
