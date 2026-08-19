#include "robot_manager.h"
#include "point_to_point_algorithm.h"

#include <QTimer>
#include <cstdlib>
#include <cmath>

RobotManager::RobotManager(QObject* parent)
    : QObject(parent)
    , m_animationTimer(new QTimer(this))
    , m_algorithm(std::make_unique<PointToPointAlgorithm>())
{
    m_animationTimer->setInterval(16);
    connect(m_animationTimer, &QTimer::timeout,
            this, &RobotManager::updateAnimation);
}

void RobotManager::createDefaultRobot()
{
    m_robots.clear();
    m_robots.emplace_back(Robot());
    m_currentRobotIndex = 0;

    Robot& robot = m_robots.front();
    robot.addStartingPoint(150, 150);
    robot.segments.clear();

    robot.addSegment(90, 40, 20);
    robot.addSegment(15, 60, 20);
    robot.addSegment(15, 70, 20);

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

    if (!m_animateTransitions)
    {
        joint.angle = angle;
        robot.calculatePosition();
        emit robotChanged();
        return;
    }

    robot.moving = true;
    if (!m_animationTimer->isActive())
        m_animationTimer->start();
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
    robot.segments[lastIndex].joint.targetAngle = std::rand();
    if (!m_animateTransitions)
    {
        robot.segments[lastIndex].joint.angle = robot.segments[lastIndex].joint.targetAngle;
        robot.calculatePosition();
        emit robotChanged();
        return;
    }

    robot.moving = true;
    if (!m_animationTimer->isActive())
        m_animationTimer->start();
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

    if (!m_animateTransitions)
    {
        for (size_t index = 0; index < targetAngles.size(); ++index)
            m_robots[m_currentRobotIndex].segments[index].joint.angle = targetAngles[index];

        m_robots[m_currentRobotIndex].calculatePosition();
        emit robotChanged();
        return;
    }

    m_robots[m_currentRobotIndex].moving = true;
    if (!m_animationTimer->isActive())
        m_animationTimer->start();
}

void RobotManager::setAnimateTransitions(bool enabled)
{
    m_animateTransitions = enabled;
    if (!m_animateTransitions && m_animationTimer->isActive())
        m_animationTimer->stop();

    if (!m_animateTransitions && !m_robots.empty())
        m_robots[m_currentRobotIndex].moving = false;
}

void RobotManager::setGlobalJointSpeed(double speed)
{
    if (speed <= 0.0 || m_robots.empty())
        return;

    m_globalJointSpeed = speed;
    for (auto& segment : m_robots[m_currentRobotIndex].segments)
        segment.joint.speed = speed;
}

void RobotManager::updateAnimation()
{
    if (m_robots.empty())
    {
        if (m_animationTimer->isActive())
            m_animationTimer->stop();
        return;
    }

    Robot& robot = m_robots[m_currentRobotIndex];
    bool didUpdate = false;
    double deltaSeconds = m_animationTimer->interval() / 1000.0;

    for (auto& segment : robot.segments)
    {
        auto& joint = segment.joint;
        double diff = joint.targetAngle - joint.angle;
        if (std::abs(diff) < 0.01)
        {
            joint.angle = joint.targetAngle;
            continue;
        }

        didUpdate = true;
        double maxStep = joint.speed * deltaSeconds;
        if (std::abs(diff) <= maxStep)
            joint.angle = joint.targetAngle;
        else
            joint.angle += diff > 0 ? maxStep : -maxStep;
    }

    if (didUpdate)
    {
        robot.calculatePosition();
        emit robotChanged();
    }
    else if (m_animationTimer->isActive())
    {
        m_animationTimer->stop();
        robot.moving = false;
        emit robotChanged();
    }
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
