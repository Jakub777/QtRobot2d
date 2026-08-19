#pragma once

#include <QObject>
#include <QTimer>
#include <QVBoxLayout>
#include <vector>
#include "canvas.h"
#include "gui_labeled_double_spinbox.h"
#include "gui_segment_widget.h"
#include "robot_manager.h"

class RobotWindow;

class RobotController : public QObject
{
    Q_OBJECT

public:
    explicit RobotController(RobotWindow* window = nullptr, QObject* parent = nullptr);

    void setup();
    void addRobot();

private:
    void bindRobotToView();
    void refreshCanvas();
    void syncSegmentWidgets();
    Point2D canvasStartPoint() const;

    RobotWindow* m_window = nullptr;
    RobotManager m_manager;
    Canvas* m_canvas = nullptr;
    QVBoxLayout* m_segmentsLayout = nullptr;
    QTimer* m_syncTimer = nullptr;
    LabeledDoubleSpinBox* m_currentEndPointX = nullptr;
    LabeledDoubleSpinBox* m_currentEndPointY = nullptr;
    std::vector<SegmentWidget*> m_segmentWidgets;
};
