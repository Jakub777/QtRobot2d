#pragma once

#include "point.h"
#include <vector>

struct RobotSegmentViewData
{
    Point2D start;
    Point2D end;
    double width = 0.0;
    double length = 0.0;
    double angle = 0.0;
};

class RobotViewData
{
public:
    Point2D startPoint;
    Point2D endPoint;
    std::vector<RobotSegmentViewData> segments;
};
