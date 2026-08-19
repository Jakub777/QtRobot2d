#include "robot_controller.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QDoubleSpinBox>

RobotController::RobotController(QMainWindow* window, QObject* parent)
    : QObject(parent)
    , m_window(window)
{
    if (!m_window)
        return;

    createMainWindow();
    bindRobotToView();

    m_syncTimer = new QTimer(this);
    m_syncTimer->setInterval(200);
    connect(m_syncTimer, &QTimer::timeout,
            this, &RobotController::syncSegmentWidgets);
    m_syncTimer->start();
}

void RobotController::createMainWindow()
{
    QWidget* centralWidget = new QWidget(m_window);
    m_window->setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);
    auto* guiBox = new QVBoxLayout;
    m_segmentsLayout = new QVBoxLayout;

    mainLayout->addLayout(guiBox);
    guiBox->addLayout(m_segmentsLayout);

    auto* label = new QLabel("Welcome to the robot simulator!", centralWidget);
    auto* button = new QPushButton("CLick the button to randomize the last angle of the robot", centralWidget);
    auto* moveButton = new QPushButton("Move robot to point B", centralWidget);

    mainLayout->addWidget(label);
    mainLayout->addWidget(button);
    mainLayout->addWidget(moveButton);

    m_canvas = new Canvas(300, 300, centralWidget);
    mainLayout->addWidget(m_canvas);

    auto* animationToggle = new QCheckBox("Animate transitions", centralWidget);
    animationToggle->setChecked(true);

    auto* speedLayout = new QHBoxLayout;
    auto* speedLabel = new QLabel("Joint speed (deg/s):", centralWidget);
    auto* speedSpinBox = new QDoubleSpinBox(centralWidget);
    speedSpinBox->setRange(1.0, 360.0);
    speedSpinBox->setValue(20.0);
    speedSpinBox->setSingleStep(5.0);
    speedSpinBox->setSuffix(" deg/s");
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(speedSpinBox);

    guiBox->addWidget(animationToggle);
    guiBox->addLayout(speedLayout);

    connect(button, &QPushButton::clicked,
            &m_manager, &RobotManager::randomizeLastAngle);
    connect(moveButton, &QPushButton::clicked, this, [this]() {
        m_manager.moveCurrentRobotTo(Point2D(250.0, 80.0));
    });
    connect(animationToggle, &QCheckBox::toggled,
            &m_manager, &RobotManager::setAnimateTransitions);
    connect(speedSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &m_manager, &RobotManager::setGlobalJointSpeed);
    connect(&m_manager, &RobotManager::robotChanged,
            this, &RobotController::refreshCanvas);
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
}

void RobotController::setup()
{
    m_manager.createDefaultRobot();
    m_manager.setAnimateTransitions(true);
    m_manager.setGlobalJointSpeed(20.0);
    bindRobotToView();
    syncSegmentWidgets();
}

void RobotController::addRobot()
{
    m_manager.addRobot();
    bindRobotToView();
}
