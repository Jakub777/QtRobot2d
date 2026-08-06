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
    Segment(double angle, double length, double width);
    void calculateAndOverwriteEnd();
    Point2D getEnd();
    void debugPrint() const;
};