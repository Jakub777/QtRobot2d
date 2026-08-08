#pragma once

#include <QObject>
#include <vector>
#include "r_robot.h"

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

    Robot* robot();
    const Robot& robot() const;

signals:
    void robotChanged();

private:
    std::vector<Robot> m_robots;
    int m_currentRobotIndex = 0;
};
