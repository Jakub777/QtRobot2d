#include "r_joint.h"

Joint::Joint()
    : angle(0.0)
    , minAngle(0.0)
    , maxAngle(0.0)
    , speed(90.0)
    , targetAngle(0.0)
{
}

Joint::Joint(double a)
    : angle(a)
    , speed(90.0)
    , targetAngle(a)
{
    minAngle = 10.0;
    maxAngle = 80.0;
}
