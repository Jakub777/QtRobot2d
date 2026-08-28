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
    void createGuiConnections();
    void connectMoveButton();
    void connectAnimationToggle();
    void connectSpeedControl();
    void connectCanvasMouse();
    void connectManagerSignals();
    void connectSegmentControls(RobotSegmentViewControls& controls, int segmentIndex);
    void updateSegmentControls(const RobotSegmentViewControls& controls,
                               const RobotSegmentViewData& data, bool moving);
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
    Point2D m_targetPoint{250.0, 80.0};
    Point2D m_userPoint;
    bool m_hasUserPoint = false;
};
