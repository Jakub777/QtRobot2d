#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include "r_robot.h"
#include "robot_view_data.h"
#include "point.h"
#include "robot_algorithm.h"

class QTimer;

class RobotManager : public QObject
{
    Q_OBJECT

public:
    explicit RobotManager(QObject* parent = nullptr);

    void createDefaultRobot(const Point2D& startPoint);
    void addRobot(const Point2D& startPoint);
    void setCurrentRobot(int index);
    int robotCount() const;
    bool hasRobots() const;

    void setSegmentCount(int count);
    void setSegmentAngle(int index, double angle);
    void setSegmentLength(int index, double length);
    void setSegmentWidth(int index, double width);
    void moveCurrentRobotTo(const Point2D& target);
    bool isPointReachable(const Point2D& target) const;

    void setGlobalJointSpeed(double speed);

    std::vector<Robot>& robots();
    const std::vector<Robot>& robots() const;
    Robot* robot();
    const Robot& robot() const;
    RobotViewData robotViewData() const;
    

signals:
    void robotChanged();

private:
    std::vector<Robot> m_robots;
    int m_currentRobotIndex = 0;
    double m_globalJointSpeed = 20.0;
    std::unique_ptr<RobotAlgorithm> m_algorithm;
};
