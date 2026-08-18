#pragma once

#include "robot_algorithm.h"

class PointToPointAlgorithm final : public RobotAlgorithm
{
public:
    std::vector<double> calculateTargetAngles(
        const Robot& robot, const Point2D& target) const override;
};
