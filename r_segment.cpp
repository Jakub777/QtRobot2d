#include "r_link.h"
#include "r_joint.h"
#include "r_segment.h"
#include <cmath>
#include <iostream>

#define PI 3.14159

Segment::Segment(double angle, double length, double width)
{
    link = Link(length, width);
    joint = Joint(angle);
}

void Segment::calculateAndOverwriteEnd(double baseAngle)
{
    double absoluteAngle = baseAngle + joint.angle;
    end.x = start.x + link.length * cos(absoluteAngle * PI / 180.0);
    end.y = start.y - link.length * sin(absoluteAngle * PI / 180.0);
}

Point2D Segment::getEnd()
{
    return end;
}

void Segment::debugPrint() const
{
    std::cout << "Segment\n";
    std::cout << "  Angle : " << joint.angle << '\n';
    std::cout << "  Start : (" << start.x << ", " << start.y << ")\n";
    std::cout << "  End   : (" << end.x << ", " << end.y << ")\n";
    std::cout << "  Length: " << link.length << '\n';
    std::cout << "  Width : " << link.width << '\n';
    std::cout << "-----------------------------\n";
}