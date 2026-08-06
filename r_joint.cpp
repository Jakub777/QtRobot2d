#include "r_joint.h"

Joint::Joint()
{
}

Joint::Joint(double a)
    : angle(a)
{
    minAngle = 10.0;
    maxAngle = 80.0;
}
