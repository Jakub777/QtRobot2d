#include "r_segment.h"
#include "r_robot.h"
#include "point.h"
#include <QPainter>

Robot::Robot()
{

}

void Robot::addStartingPoint(int x, int y)
{
    startPoint = Point2D(x, y);
}

 void Robot::calculatePosition()
 {
    double cumulativeAngle = 0.0;
    for (size_t i = 0; i < segments.size(); ++i)
    {
        if (i == 0)
        {
            segments[0].start = startPoint;
        }
        else
        {
            segments[i].start = segments[i - 1].getEnd();
        }
        cumulativeAngle += segments[i].joint.angle;
        segments[i].calculateAndOverwriteEnd(cumulativeAngle);
        segments[i].debugPrint(i);
    }
}

void Robot::addSegment(double angle, double length, double width = 20.0)
{
    Segment new_segment = Segment(angle, length, width);
    segments.emplace_back(new_segment);
}

void Robot::draw(QPainter& painter)
{   
    // painter.setPen(Qt::red);

    for(const auto& segment : segments)
    {
        painter.drawLine(segment.start.x,
                         segment.start.y,
                         segment.end.x,
                         segment.end.y);
    }
}