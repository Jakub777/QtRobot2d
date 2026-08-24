#include "robot_controller.h"

#include "robot_window.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>

RobotController::RobotController(RobotWindow* window, QObject* parent)
    : QObject(parent)
    , m_window(window)
{
    if (!m_window)
        return;

        m_canvas = m_window->canvas();
        m_segmentsLayout = m_window->segmentsLayout();
        m_currentEndPointX = m_window->endPointX();
        m_currentEndPointY = m_window->endPointY();

        connect(m_window->randomizeButton(), &QPushButton::clicked,
            &m_manager, &RobotManager::randomizeLastAngle);
        connect(m_window->moveButton(), &QPushButton::clicked, this, [this]() {
        m_manager.moveCurrentRobotTo(Point2D(250.0, 80.0));
        });
        connect(m_window->animationToggle(), &QCheckBox::toggled,
            &m_manager, &RobotManager::setAnimateTransitions);
        connect(m_window->speedSpinBox(), QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &m_manager, &RobotManager::setGlobalJointSpeed);
        connect(&m_manager, &RobotManager::robotChanged,
            this, &RobotController::refreshCanvas);

    bindRobotToView();

}

void RobotController::bindRobotToView()
{
    if (!m_canvas)
        return;

    m_canvas->setRobotData(m_manager.robotViewData());
}

void RobotController::refreshCanvas()
{
    if (m_canvas)
    m_canvas->setRobotData(m_manager.robotViewData());

    syncSegmentControls();
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
        if (!segmentControls.signalsConnected)
            connectSegmentControls(segmentControls, &m_manager, static_cast<int>(index));
        setSegmentControls(segmentControls, data.segments[index], data.moving);
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

    return Point2D(m_canvas->width() / 2.0, m_canvas->height());
}

void RobotController::setup()
{
    m_manager.createDefaultRobot(canvasStartPoint());
    m_manager.setAnimateTransitions(true);
    m_manager.setGlobalJointSpeed(20.0);
    bindRobotToView();
    syncSegmentControls();
}

void RobotController::addRobot()
{
    m_manager.addRobot(canvasStartPoint());
    bindRobotToView();
}
