#pragma once

#include <QWidget>
#include <QPainter>
#include <vector>
#include "r_robot.h"
#include "point.h"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(int x = 0, int y = 0, QWidget* parent = nullptr);

    void setRobot(Robot* robot);
    void addPoint(double x, double y);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Robot* my_robot = nullptr;
    std::vector<Point2D> points;
};
