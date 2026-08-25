#include "robot_controller.h"

#include "robot_window.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSignalBlocker>
#include <QTimer>
#include <cmath>
#include "robot_tolerances.h"

RobotController::RobotController(RobotWindow* window, QObject* parent)
    : QObject(parent)
    , m_window(window)
    , m_animationTimer(new QTimer(this))
{
    if (!m_window)
        return;

        m_canvas = m_window->canvas();
        m_segmentsLayout = m_window->segmentsLayout();
        m_currentEndPointX = m_window->endPointX();
        m_currentEndPointY = m_window->endPointY();

    createGuiConnections();

    bindRobotToView();

}

void RobotController::createGuiConnections()
{
    connectMoveButton();
    connectAnimationToggle();
    connectSpeedControl();
    connectManagerSignals();
}

void RobotController::connectMoveButton()
{
    connect(m_window->moveButton(), &QPushButton::clicked, this, [this]() {
        m_manager.moveCurrentRobotTo(m_targetPoint);
    });
}

void RobotController::connectAnimationToggle()
{
    connect(m_window->animationToggle(), &QCheckBox::toggled,
            this, &RobotController::setAnimateTransitions);
}

void RobotController::connectSpeedControl()
{
    connect(m_window->speedSpinBox(),
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &m_manager, &RobotManager::setGlobalJointSpeed);
}

void RobotController::connectManagerSignals()
{
    connect(&m_manager, &RobotManager::robotChanged,
            this, &RobotController::refreshCanvas);

    m_animationTimer->setInterval(16);
    connect(m_animationTimer, &QTimer::timeout,
            this, &RobotController::updateAnimation);
}

void RobotController::connectSegmentControls(
    RobotSegmentViewControls& controls, int segmentIndex)
{
    if (controls.signalsConnected || segmentIndex < 0)
        return;

    connect(controls.angle, &LabeledDoubleSpinBox::valueChanged,
            &m_manager, [this, segmentIndex](double value) {
        m_manager.setSegmentAngle(segmentIndex, value);
    });
    connect(controls.length, &LabeledDoubleSpinBox::valueChanged,
            &m_manager, [this, segmentIndex](double value) {
        m_manager.setSegmentLength(segmentIndex, value);
    });
    connect(controls.width, &LabeledDoubleSpinBox::valueChanged,
            &m_manager, [this, segmentIndex](double value) {
        m_manager.setSegmentWidth(segmentIndex, value);
    });

    controls.signalsConnected = true;
}

void RobotController::updateSegmentControls(
    const RobotSegmentViewControls& controls,
    const RobotSegmentViewData& data,
    bool moving)
{
    const QSignalBlocker angleBlocker(controls.angle);
    const QSignalBlocker lengthBlocker(controls.length);
    const QSignalBlocker widthBlocker(controls.width);

    controls.angle->setValue(data.angle);
    controls.length->setValue(data.length);
    controls.width->setValue(data.width);

    controls.angle->setEnabled(!moving);
    controls.length->setEnabled(!moving);
    controls.width->setEnabled(!moving);
}

void RobotController::bindRobotToView()
{
    if (!m_canvas)
        return;

    RobotViewData data = m_manager.robotViewData();
    data.targetPoint = m_targetPoint;
    m_window->setRobotData(data);
}

void RobotController::refreshCanvas()
{
    if (!m_animateTransitions)
    {
        for (auto& robot : m_manager.robots())
        {
            for (auto& segment : robot.segments)
                segment.joint.angle = segment.joint.targetAngle;
            robot.moving = false;
            robot.calculatePosition();
        }
    }

    if (m_animateTransitions && hasPendingAnimation() && !m_animationTimer->isActive())
        m_animationTimer->start();

    RobotViewData data = m_manager.robotViewData();
    data.targetPoint = m_targetPoint;
    if (m_window)
        m_window->setRobotData(data);

    syncSegmentControls();
}

void RobotController::setAnimateTransitions(bool enabled)
{
    m_animateTransitions = enabled;
    if (!enabled)
    {
        m_animationTimer->stop();
        for (auto& robot : m_manager.robots())
        {
            for (auto& segment : robot.segments)
                segment.joint.angle = segment.joint.targetAngle;
            robot.moving = false;
            robot.calculatePosition();
        }
        emit m_manager.robotChanged();
    }
    else
    {
        refreshCanvas();
    }
}

void RobotController::updateAnimation()
{
    const double deltaSeconds = m_animationTimer->interval() / 1000.0;

    for (auto& robot : m_manager.robots())
    {
        bool robotUpdated = false;
        for (auto& segment : robot.segments)
        {
            auto& joint = segment.joint;
            const double difference = joint.targetAngle - joint.angle;
            if (std::abs(difference) < RobotTolerances::angle)
            {
                joint.angle = joint.targetAngle;
                continue;
            }

            robotUpdated = true;
            const double maxStep = joint.speed * deltaSeconds;
            joint.angle += std::abs(difference) <= maxStep
                ? difference
                : (difference > 0.0 ? maxStep : -maxStep);
        }

        bool robotMoving = false;
        for (const auto& segment : robot.segments)
        {
            if (std::abs(segment.joint.targetAngle - segment.joint.angle)
                >= RobotTolerances::angle)
            {
                robotMoving = true;
                break;
            }
        }

        robot.moving = robotMoving;
        if (robotUpdated)
            robot.calculatePosition();
    }

    emit m_manager.robotChanged();
    if (!hasPendingAnimation())
        m_animationTimer->stop();
}

bool RobotController::hasPendingAnimation() const
{
    for (const auto& robot : m_manager.robots())
    {
        for (const auto& segment : robot.segments)
        {
            if (std::abs(segment.joint.targetAngle - segment.joint.angle)
                >= RobotTolerances::angle)
                return true;
        }
    }

    return false;
}

void RobotController::syncSegmentControls()
{
    if (!m_segmentsLayout)
        return;

    const RobotViewData data = m_manager.robotViewData();

    auto& controls = m_window->segmentControls();
    for (size_t index = 0; index < data.segments.size() && index < controls.size(); ++index)
    {
        auto& segmentControls = controls[index];
        connectSegmentControls(segmentControls, static_cast<int>(index));
        updateSegmentControls(segmentControls, data.segments[index], data.moving);
    }

    if (m_currentEndPointX && m_currentEndPointY)
    {
        m_currentEndPointX->setValue(data.endPoint.x);
        m_currentEndPointY->setValue(data.endPoint.y);
    }
}

Point2D RobotController::canvasStartPoint() const
{
    if (!m_canvas)
        return {};

    return Point2D(m_canvas->width() / 2.0, m_canvas->height() - 1.0);
}

void RobotController::setup()
{
    m_manager.createDefaultRobot(canvasStartPoint());
    m_manager.setGlobalJointSpeed(20.0);
    bindRobotToView();
    syncSegmentControls();
}

void RobotController::addRobot()
{
    m_manager.addRobot(canvasStartPoint());
    bindRobotToView();
}
