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
    for (size_t i = 0; i < segments.size(); ++i)
    {
        if (i == 0)
        {
            segments[0].start = startPoint;
            segments[0].calculateAndOverwriteEnd();
        }
        else
        {
            segments[i].start = segments[i - 1].getEnd();
            segments[i].calculateAndOverwriteEnd();
        }
        segments[i].debugPrint();
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