#pragma once

#include <QWidget>
#include <QPainter>
#include <vector>
#include "point.h"
#include "robot_view_data.h"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(int x = 0, int y = 0, QWidget* parent = nullptr);

    void setRobotData(const RobotViewData& robotData);
    void addPoint(double x, double y);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    RobotViewData my_robot;
    std::vector<Point2D> points;
};
