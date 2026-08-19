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

    m_syncTimer = new QTimer(this);
    m_syncTimer->setInterval(200);
    connect(m_syncTimer, &QTimer::timeout,
            this, &RobotController::syncSegmentWidgets);
    m_syncTimer->start();
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

    syncSegmentWidgets();
}

void RobotController::syncSegmentWidgets()
{
    if (!m_segmentsLayout)
        return;

    const RobotViewData data = m_manager.robotViewData();

    while (m_segmentWidgets.size() > data.segments.size())
    {
        SegmentWidget* widget = m_segmentWidgets.back();
        m_segmentWidgets.pop_back();
        m_segmentsLayout->removeWidget(widget);
        widget->deleteLater();
    }

    while (m_segmentWidgets.size() < data.segments.size())
    {
        const int index = static_cast<int>(m_segmentWidgets.size());
        auto* widget = new SegmentWidget(
            QString("Segment %1 >>").arg(index + 1),
            &m_manager,
            index,
            m_window->centralWidget());
        m_segmentsLayout->addWidget(widget);
        m_segmentWidgets.push_back(widget);
    }

    for (size_t index = 0; index < data.segments.size(); ++index)
        m_segmentWidgets[index]->setData(data.segments[index], data.moving);

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
    syncSegmentWidgets();
}

void RobotController::addRobot()
{
    m_manager.addRobot(canvasStartPoint());
    bindRobotToView();
}
