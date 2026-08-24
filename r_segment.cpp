#include "r_link.h"
#include "r_joint.h"
#include "r_segment.h"
#include <cmath>

#define PI 3.14159

Segment::Segment(double angle, double length, double width)
{
    link = Link(length, width);
    joint = Joint(angle);
}

void Segment::calculateAndOverwriteEnd(double baseAngle)
{
    // double absoluteAngle = baseAngle + joint.angle; TO DO- check how many variables are needed
    double absoluteAngle = baseAngle;
    end.x = start.x + link.length * cos(absoluteAngle * PI / 180.0);
    end.y = start.y - link.length * sin(absoluteAngle * PI / 180.0);
}

Point2D Segment::getEnd()
{
    return end;
}
