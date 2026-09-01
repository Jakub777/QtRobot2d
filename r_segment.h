#pragma once
#include "r_link.h"
#include "r_joint.h"
#include "point.h"

class Segment
{
public:
    Joint joint;
    Link link;
    Point2D start;
    Point2D end;
    Segment(double angle = 0.0, double length = 40.0, double width = 20.0);
    void calculateAndOverwriteEnd(double baseAngle);
    Point2D getEnd();
};