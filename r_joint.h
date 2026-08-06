#pragma once

class Joint 
{
public:
    double angle;
    double minAngle;
    double maxAngle;
    Joint();
    Joint(double a);
};