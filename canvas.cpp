#include <QWidget>
#include <QPainter>
#include <vector>
#include "point.h"
#include "canvas.h"

Canvas::Canvas(int width, int height, QWidget* parent)
    : QWidget(parent)
{
    // resize(width, height);
    setMinimumHeight(height);
    setMinimumWidth(width);
    my_robot = new Robot();
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

void Canvas::initRobot()
{
    // my_robot->addStartingPoint(width() / 2,height()- 1);
    my_robot->addStartingPoint(149,149);
    my_robot->addSegment(0, 20, 20);
    my_robot->addSegment(45, 40, 20);
    my_robot->addSegment(90, 70, 20);
    my_robot->calculatePosition();
}


void Canvas::randomizeLastAngle()
{
    int last_index =  my_robot->segments.size() -1;
    my_robot->segments[last_index].joint.angle = rand();
    my_robot->calculatePosition();
    update();
}