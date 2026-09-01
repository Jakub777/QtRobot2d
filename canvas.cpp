#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include "point.h"
#include "canvas.h"

Canvas::Canvas(int width, int height, QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(width, height);
}

void Canvas::setRobotData(const RobotViewData& robotData)
{
    my_robot = robotData;
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
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.fillRect(rect(), Qt::white);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    for (const auto& point : points)
    {
        painter.drawEllipse(QPointF(point.x, point.y), 3, 3);
    }
    for (const auto& segment : my_robot.segments)
    {
        painter.drawLine(segment.start.x,
                         segment.start.y,
                         segment.end.x,
                         segment.end.y);
    }

    if (my_robot.hasUserPoint)
    {
        const QColor pointColor = my_robot.userPointReachable
            ? Qt::green
            : Qt::red;
        painter.setPen(pointColor);
        painter.setBrush(pointColor);
        painter.drawEllipse(QPointF(my_robot.userPoint.x,
                                    my_robot.userPoint.y), 4, 4);
    }
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit pointClicked(event->position().x(), event->position().y());

    QWidget::mousePressEvent(event);
}