#include "robot_manager.h"

#include <cstdlib>

RobotManager::RobotManager(QObject* parent)
    : QObject(parent)
{
}

void RobotManager::createDefaultRobot()
{
    m_robots.clear();
    m_robots.emplace_back(Robot());
    m_currentRobotIndex = 0;

    Robot& robot = m_robots.front();
    robot.addStartingPoint(150, 150);
    robot.segments.clear();

    robot.addSegment(0, 40, 20);
    robot.addSegment(45, 60, 20);
    robot.addSegment(90, 70, 20);

    robot.calculatePosition();
    emit robotChanged();
}

void RobotManager::addRobot()
{
    m_robots.emplace_back(Robot());
    m_currentRobotIndex = static_cast<int>(m_robots.size()) - 1;

    Robot& robot = m_robots.back();
    robot.addStartingPoint(150, 150);
    robot.segments.clear();

    robot.addSegment(0, 40, 20);
    robot.addSegment(45, 60, 20);
    robot.addSegment(90, 70, 20);
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

    m_robots[m_currentRobotIndex].segments[index].joint.angle = angle;
    m_robots[m_currentRobotIndex].calculatePosition();
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

void RobotManager::randomizeLastAngle()
{
    if (m_robots.empty())
        return;

    Robot& robot = m_robots[m_currentRobotIndex];
    if (robot.segments.empty())
        return;

    const int lastIndex = static_cast<int>(robot.segments.size()) - 1;
    robot.segments[lastIndex].joint.angle = std::rand();
    robot.calculatePosition();
    emit robotChanged();
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
