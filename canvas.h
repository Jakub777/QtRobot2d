#pragma once

#include <QWidget>
#include <QPainter>
#include <vector>
#include "r_robot.h"
#include "point.h"

class Canvas : public QWidget
{
public:
    Canvas(int x = 0, int y = 0, QWidget* parent = nullptr);
    Robot * my_robot;

    void addPoint(double x, double y);
    void initRobot();
    void randomizeLastAngle();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<Point2D> points;
    // Scene scene;
    // Camera camera;
    // Renderer renderer;
};
