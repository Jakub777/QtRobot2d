#pragma once

#include "point.h"
#include <vector>

class LabeledDoubleSpinBox;
class RobotManager;
class QWidget;

struct RobotSegmentViewControls
{
    QWidget* widget = nullptr;
    LabeledDoubleSpinBox* angle = nullptr;
    LabeledDoubleSpinBox* length = nullptr;
    LabeledDoubleSpinBox* width = nullptr;
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
    std::vector<RobotSegmentViewData> segments;
};

void connectSegmentControls(RobotSegmentViewControls& controls,
                            RobotManager* manager, int segmentIndex);
void setSegmentControls(const RobotSegmentViewControls& controls,
                        const RobotSegmentViewData& data, bool moving);
