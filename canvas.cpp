#include <QWidget>
#include <QPainter>
#include <vector>
#include "point.h"
#include "canvas.h"

Canvas::Canvas(int width, int height, QWidget* parent)
    : QWidget(parent)
{
    setMinimumHeight(height);
    setMinimumWidth(width);
    my_robot = nullptr;
}

void Canvas::setRobot(Robot* robot)
{
    my_robot = robot;
    update();
}

void Canvas::addPoint(double x, double y)
{
    points.emplace_back(x, y);
    update();
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.fillRect(rect(), Qt::white);
    painter.setPen(Qt::black);

    for (const auto& point : points)
    {
        painter.drawEllipse(QPointF(point.x, point.y), 3, 3);
    }
    if (my_robot != nullptr)
        my_robot->draw(painter);
}