#ifndef CANVAS_VIEW_H
#define CANVAS_VIEW_H

#include <QGraphicsScene>
#include <QPainter>
#include <cstdint>

class canvas_scene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit canvas_scene(QObject* parent = nullptr);
    ~canvas_scene() override;

signals:
    void mouse_down(std::uint16_t x, std::uint16_t y);
    void mouse_move(std::uint16_t x, std::uint16_t y);
    void mouse_up(std::uint16_t x, std::uint16_t y);

protected:
    void drawForeground(QPainter* painter, const QRectF& rect) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    std::uint16_t _last_x, _last_y;
};

#endif // CANVAS_VIEW_H
