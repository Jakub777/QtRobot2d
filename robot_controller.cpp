#include "robot_controller.h"

#include "robot_window.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSignalBlocker>
#include <QString>
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
    connectTryReachUserMousePointButton();
    connectAnimationToggle();
    connectSpeedControl();
    connectCanvasMouse();
    connectManagerSignals();
}

void RobotController::connectTryReachUserMousePointButton()
{
    connect(m_window->tryReachUserMousePointButton(), &QPushButton::clicked,
            this, [this]() {
        if (!m_hasUserPoint)
        {
            m_window->appendMessage("Choose a point with the mouse first.");
            return;
        }

        m_targetPoint = m_userPoint;
        bindRobotToView();

        if (m_manager.isPointReachable(m_targetPoint))
        {
            m_window->appendMessage(QString("Target point set to (%1, %2). Robot is moving.")
                .arg(m_targetPoint.x, 0, 'f', 1)
                .arg(m_targetPoint.y, 0, 'f', 1));
            m_manager.moveCurrentRobotTo(m_targetPoint);
        }
        else
        {
            m_window->appendMessage(QString("Target point (%1, %2) is outside the robot's reach.")
                .arg(m_targetPoint.x, 0, 'f', 1)
                .arg(m_targetPoint.y, 0, 'f', 1));
        }
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

void RobotController::connectCanvasMouse()
{
    connect(m_canvas, &Canvas::pointClicked, this, [this](double x, double y) {
        m_userPoint = Point2D(x, y);
        m_hasUserPoint = true;
        bindRobotToView();
        const bool reachable = m_manager.isPointReachable(m_userPoint);
        m_window->appendMessage(QString("User point selected: (%1, %2) - %3.")
            .arg(x, 0, 'f', 1)
            .arg(y, 0, 'f', 1)
            .arg(reachable ? "reachable" : "outside reach"));
    });
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
    data.userPoint = m_userPoint;
    data.hasUserPoint = m_hasUserPoint;
    data.userPointReachable = m_hasUserPoint
        && m_manager.isPointReachable(m_userPoint);
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
    data.userPoint = m_userPoint;
    data.hasUserPoint = m_hasUserPoint;
    data.userPointReachable = m_hasUserPoint
        && m_manager.isPointReachable(m_userPoint);
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
    const bool wasMoving = hasPendingAnimation();
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
    {
        m_animationTimer->stop();
        if (wasMoving)
            m_window->appendMessage("Robot reached the target point.");
    }
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
    m_window->appendMessage("Robot ready. Select a point on the canvas.");
}

void RobotController::addRobot()
{
    m_manager.addRobot(canvasStartPoint());
    bindRobotToView();
}
