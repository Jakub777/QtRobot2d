#include "robot_manager.h"
#include "point_to_point_algorithm.h"
#include "robot_tolerances.h"

#include <cmath>

RobotManager::RobotManager(QObject* parent)
    : QObject(parent)
    , m_algorithm(std::make_unique<PointToPointAlgorithm>())
{
}

void RobotManager::createDefaultRobot(const Point2D& startPoint)
{
    m_robots.clear();
    m_robots.emplace_back(Robot());
    m_currentRobotIndex = 0;

    Robot& robot = m_robots.front();
    robot.addStartingPoint(startPoint.x, startPoint.y);
    robot.segments.clear();

    robot.addSegment(90, 40, 20);
    robot.addSegment(15, 60, 20);
    robot.addSegment(15, 70, 20);

    robot.calculatePosition();
    emit robotChanged();
}

void RobotManager::addRobot(const Point2D& startPoint)
{
    m_robots.emplace_back(Robot());
    m_currentRobotIndex = static_cast<int>(m_robots.size()) - 1;

    Robot& robot = m_robots.back();
    robot.addStartingPoint(startPoint.x, startPoint.y);
    robot.segments.clear();

    robot.addSegment(15, 40, 20);
    robot.addSegment(15, 60, 20);
    robot.addSegment(15, 70, 20);
    robot.calculatePosition();

    emit robotChanged();
}

void RobotManager::setCurrentRobot(int index)
{
    if (index < 0 || index >= static_cast<int>(m_robots.size()))
        return;

    m_currentRobotIndex = index;
    emit robotChanged();
}

int RobotManager::robotCount() const
{
    return static_cast<int>(m_robots.size());
}

bool RobotManager::hasRobots() const
{
    return !m_robots.empty();
}

void RobotManager::setSegmentCount(int count)
{
    if (count < 0 || m_robots.empty())
        return;

    Robot& robot = m_robots[m_currentRobotIndex];

    while (static_cast<int>(robot.segments.size()) < count)
        robot.addSegment(0, 40, 20);

    while (static_cast<int>(robot.segments.size()) > count)
        robot.segments.pop_back();

    robot.calculatePosition();
    emit robotChanged();
}

void RobotManager::setSegmentAngle(int index, double angle)
{
    if (m_robots.empty() || index < 0 || index >= static_cast<int>(m_robots[m_currentRobotIndex].segments.size()))
        return;

    Robot& robot = m_robots[m_currentRobotIndex];
    auto& joint = robot.segments[index].joint;
    joint.targetAngle = angle;
    joint.speed = m_globalJointSpeed;

    emit robotChanged();
}

void RobotManager::setSegmentLength(int index, double length)
{
    if (m_robots.empty() || index < 0 || index >= static_cast<int>(m_robots[m_currentRobotIndex].segments.size()))
        return;

    m_robots[m_currentRobotIndex].segments[index].link.length = length;
    m_robots[m_currentRobotIndex].calculatePosition();
    emit robotChanged();
}

void RobotManager::setSegmentWidth(int index, double width)
{
    if (m_robots.empty() || index < 0 || index >= static_cast<int>(m_robots[m_currentRobotIndex].segments.size()))
        return;

    m_robots[m_currentRobotIndex].segments[index].link.width = width;
    m_robots[m_currentRobotIndex].calculatePosition();
    emit robotChanged();
}

void RobotManager::moveCurrentRobotTo(const Point2D& target)
{
    if (m_robots.empty() || !m_algorithm)
        return;

    const std::vector<double> targetAngles =
        m_algorithm->calculateTargetAngles(m_robots[m_currentRobotIndex], target);

    for (size_t index = 0; index < targetAngles.size(); ++index)
    {
        auto& joint = m_robots[m_currentRobotIndex].segments[index].joint;
        joint.targetAngle = targetAngles[index];
        joint.speed = m_globalJointSpeed;
    }

    emit robotChanged();
}

bool RobotManager::isPointReachable(const Point2D& target) const
{
    if (m_robots.empty() || !m_algorithm)
        return false;

    const Robot& currentRobot = m_robots[m_currentRobotIndex];
    Robot candidate = currentRobot;
    const std::vector<double> targetAngles =
        m_algorithm->calculateTargetAngles(currentRobot, target);

    if (targetAngles.size() != candidate.segments.size())
        return false;

    for (size_t index = 0; index < targetAngles.size(); ++index)
        candidate.segments[index].joint.angle = targetAngles[index];

    candidate.calculatePosition();
    const double dx = candidate.endPoint.x - target.x;
    const double dy = candidate.endPoint.y - target.y;
    return std::sqrt(dx * dx + dy * dy) <= RobotTolerances::position;
}

void RobotManager::setGlobalJointSpeed(double speed)
{
    if (speed <= 0.0 || m_robots.empty())
        return;

    m_globalJointSpeed = speed;
    for (auto& robot : m_robots)
        for (auto& segment : robot.segments)
            segment.joint.speed = speed;
}

std::vector<Robot>& RobotManager::robots()
{
    return m_robots;
}

const std::vector<Robot>& RobotManager::robots() const
{
    return m_robots;
}

Robot* RobotManager::robot()
{
    if (m_robots.empty())
        return nullptr;

    return &m_robots[m_currentRobotIndex];
}

const Robot& RobotManager::robot() const
{
    return m_robots[m_currentRobotIndex];
}

RobotViewData RobotManager::robotViewData() const
{
    if (m_robots.empty())
        return {};

    return m_robots[m_currentRobotIndex].createViewData();
}
