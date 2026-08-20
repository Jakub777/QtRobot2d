#include "point_to_point_algorithm.h"

#include "r_robot.h"
#include "robot_tolerances.h"

#include <algorithm>
#include <cmath>

namespace
{
constexpr double pi = 3.14159265358979323846;
constexpr int maxIterations = 30;

double robotAngle(double x, double y)
{
    return std::atan2(-y, x) * 180.0 / pi;
}

double normalizeAngle(double angle)
{
    while (angle > 180.0)
        angle -= 360.0;
    while (angle < -180.0)
        angle += 360.0;
    return angle;
}

double distance(const Point2D& first, const Point2D& second)
{
    const double dx = second.x - first.x;
    const double dy = second.y - first.y;
    return std::sqrt(dx * dx + dy * dy);
}
}

std::vector<double> PointToPointAlgorithm::calculateTargetAngles(
    const Robot& robot, const Point2D& target) const
{
    Robot workingRobot = robot;

    for (int iteration = 0; iteration < maxIterations; ++iteration)
    {
        workingRobot.calculatePosition();
        if (workingRobot.segments.empty())
            break;

        if (distance(workingRobot.segments.back().end, target) <= RobotTolerances::position)
            break;

        for (int index = static_cast<int>(workingRobot.segments.size()) - 1;
             index >= 0;
             --index)
        {
            workingRobot.calculatePosition();

            const Point2D jointPosition = workingRobot.segments[index].start;
            const Point2D endEffector = workingRobot.segments.back().end;

            const double currentAngle = robotAngle(
                endEffector.x - jointPosition.x,
                endEffector.y - jointPosition.y);
            const double desiredAngle = robotAngle(
                target.x - jointPosition.x,
                target.y - jointPosition.y);
            const double rotation = normalizeAngle(desiredAngle - currentAngle);

            auto& joint = workingRobot.segments[index].joint;
            joint.angle = std::clamp(
                joint.angle + rotation, joint.minAngle, joint.maxAngle);
        }
    }

    std::vector<double> targetAngles;
    targetAngles.reserve(workingRobot.segments.size());
    for (const auto& segment : workingRobot.segments)
        targetAngles.push_back(segment.joint.angle);

    return targetAngles;
}
