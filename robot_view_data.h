#pragma once

#include "point.h"
#include <vector>

struct RobotSegmentViewControls
{
    class QWidget* widget = nullptr;
    class LabeledDoubleSpinBox* angle = nullptr;
    class LabeledDoubleSpinBox* length = nullptr;
    class LabeledDoubleSpinBox* width = nullptr;
    bool signalsConnected = false;
};

struct RobotSegmentViewData
{
    Point2D start;
    Point2D end;
    double width = 0.0;
    double length = 0.0;
    double angle = 0.0;
};

class RobotViewData
{
public:
    bool moving = false;
    Point2D startPoint;
    Point2D endPoint;
    Point2D targetPoint;
    Point2D userPoint;
    bool hasUserPoint = false;
    std::vector<RobotSegmentViewData> segments;
};
