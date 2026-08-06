#pragma once
#include "r_segment.h"
#include <QPainter> 
#include <vector>
#include "point.h"

class Robot
{
public:
    std::vector<Segment> segments;
    Point2D startPoint;
    Robot();
    void addStartingPoint(int x, int y);
    void addSegment(double angle, double length, double width);
    void draw(QPainter& painter);
    void calculatePosition();

};