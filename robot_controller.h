#pragma once

#include <QObject>
#include <QVBoxLayout>
#include "canvas.h"
#include "gui_labeled_double_spinbox.h"
#include "robot_manager.h"

class RobotWindow;
class QTimer;

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
    void syncSegmentControls();
    void setAnimateTransitions(bool enabled);
    void updateAnimation();
    bool hasPendingAnimation() const;
    Point2D canvasStartPoint() const;

    RobotWindow* m_window = nullptr;
    RobotManager m_manager;
    Canvas* m_canvas = nullptr;
    QVBoxLayout* m_segmentsLayout = nullptr;
    LabeledDoubleSpinBox* m_currentEndPointX = nullptr;
    LabeledDoubleSpinBox* m_currentEndPointY = nullptr;
    QTimer* m_animationTimer = nullptr;
    bool m_animateTransitions = true;
};
