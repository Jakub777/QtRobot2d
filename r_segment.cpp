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
    // double absoluteAngle = baseAngle + joint.angle; TO DO- check how many variables are needed
    double absoluteAngle = baseAngle;
    end.x = start.x + link.length * cos(absoluteAngle * PI / 180.0);
    std::cout << "kat: " << absoluteAngle << " cosinus: " << cos(absoluteAngle * PI / 180.0) << std::endl ;
    end.y = start.y - link.length * sin(absoluteAngle * PI / 180.0);
    std::cout << "kat: " << absoluteAngle << " sinus: " << cos(absoluteAngle * PI / 180.0) << std::endl ;
}

Point2D Segment::getEnd()
{
    return end;
}

void Segment::debugPrint(int i) const
{
    std::cout << "Segment " << i << "\n";
    std::cout << "  Angle : " << joint.angle << '\n';
    std::cout << "  Start : (" << start.x << ", " << start.y << ")\n";
    std::cout << "  End   : (" << end.x << ", " << end.y << ")\n";
    std::cout << "  Length: " << link.length << '\n';
    std::cout << "  Width : " << link.width << '\n';
    std::cout << "-----------------------------\n";
}