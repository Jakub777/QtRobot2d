#pragma once

#include <vector>

#include "point.h"

class Robot;

class RobotAlgorithm
{
public:
    virtual ~RobotAlgorithm() = default;

    virtual std::vector<double> calculateTargetAngles(
        const Robot& robot, const Point2D& target) const = 0;
};
