#pragma once

class Joint 
{
public:
    double angle;
    double minAngle;
    double maxAngle;
    double speed;
    double targetAngle;
    Joint();
    Joint(double a);
};