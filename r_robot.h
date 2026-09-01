#pragma once
#include "r_segment.h"
#include <QPainter> 
#include <vector>
#include "point.h"
#include "robot_view_data.h"

class Robot
{
public:
    std::vector<Segment> segments;
    Point2D startPoint;
    Point2D endPoint;
    bool moving = false;
    Robot();
    void addStartingPoint(int x, int y);
    void addSegment(double angle = 0.0, double length = 40.0, double width = 20.0);
    void draw(QPainter& painter);
    void calculatePosition();
    RobotViewData createViewData() const;

};