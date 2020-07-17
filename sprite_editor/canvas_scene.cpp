#include "canvas_scene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <cmath>

canvas_scene::canvas_scene(QObject* parent) : QGraphicsScene{parent} {}

canvas_scene::~canvas_scene() = default;

void canvas_scene::drawForeground(QPainter* painter, const QRectF& rect) {
    painter->setPen(QPen{Qt::black, 0.05, Qt::DotLine});
    for (auto x = 0; x <= sceneRect().width(); x++) {
        painter->drawLine(x, 0, x, sceneRect().height());
    }
    for (auto y = 0; y <= sceneRect().height(); y++) {
        painter->drawLine(0, y, sceneRect().width(), y);
    }
}

void canvas_scene::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->setPen(QColor{0, 0, 0, 0});
    painter->setBrush(Qt::lightGray);
    painter->drawRect(rect);

    auto light_square = false;
    auto square_size = 0.75;

    for (auto y = 0.0; y < sceneRect().height(); y += square_size) {
        auto start_light = light_square;
        for (auto x = 0.0; x < sceneRect().width(); x += square_size) {
            painter->setBrush(light_square ? QColor{0xFFFFFF}
                                           : QColor{0xCCCCCC});
            auto right_x = std::min(width(), x + square_size);
            auto bottom_y = std::min(height(), y + square_size);
            painter->drawRect(QRectF{x, y, right_x - x, bottom_y - y});
            light_square = !light_square;
        }
        light_square = !start_light;
    }
}

void canvas_scene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    auto x = event->scenePos().rx(), y = event->scenePos().ry();
    if (0 <= x && x < sceneRect().width() && 0 <= y &&
        y < sceneRect().height()) {
        emit mouse_down(x, y);
        _last_x = x;
        _last_y = y;
    }
}

void canvas_scene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    auto x = event->scenePos().rx(), y = event->scenePos().ry();
    if (0 <= x && x < sceneRect().width() && 0 <= y &&
        y < sceneRect().height()) {
        emit mouse_move(x, y);
        _last_x = x;
        _last_y = y;
    }
}

void canvas_scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    auto x = event->scenePos().rx(), y = event->scenePos().ry();
    if (0 <= x && x < sceneRect().width() && 0 <= y &&
        y < sceneRect().height()) {
        emit mouse_up(event->scenePos().rx(), event->scenePos().ry());
        _last_x = x;
        _last_y = y;
    } else {
        emit mouse_up(_last_x, _last_y);
    }
}
