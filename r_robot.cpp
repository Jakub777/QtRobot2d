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
     endPoint = startPoint;
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
        endPoint = segments[i].end;
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

RobotViewData Robot::createViewData() const
{
    RobotViewData viewData;
    viewData.moving = moving;
    viewData.startPoint = startPoint;
    viewData.endPoint = endPoint;
    viewData.segments.reserve(segments.size());

    for (const auto& segment : segments)
    {
        viewData.segments.push_back({
            segment.start,
            segment.end,
            segment.link.width,
            segment.link.length,
            segment.joint.angle
        });
    }

    return viewData;
}