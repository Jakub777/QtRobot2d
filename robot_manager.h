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

    void createDefaultRobot();
    void addRobot();
    void setCurrentRobot(int index);
    int robotCount() const;
    bool hasRobots() const;

    void setSegmentCount(int count);
    void setSegmentAngle(int index, double angle);
    void setSegmentLength(int index, double length);
    void setSegmentWidth(int index, double width);
    void randomizeLastAngle();
    void moveCurrentRobotTo(const Point2D& target);

    void setAnimateTransitions(bool enabled);
    void setGlobalJointSpeed(double speed);

    Robot* robot();
    const Robot& robot() const;
    RobotViewData robotViewData() const;
    

signals:
    void robotChanged();

private:
    void updateAnimation();

    std::vector<Robot> m_robots;
    int m_currentRobotIndex = 0;
    bool m_animateTransitions = true;
    double m_globalJointSpeed = 20.0;
    QTimer* m_animationTimer = nullptr;
    std::unique_ptr<RobotAlgorithm> m_algorithm;
};
