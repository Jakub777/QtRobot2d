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
}

void RobotController::createMainWindow()
{
    QWidget* centralWidget = new QWidget(m_window);
    m_window->setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);
    auto* guiBox = new QVBoxLayout;

    mainLayout->addLayout(guiBox);

    auto* label = new QLabel("Welcome to the robot simulator!", centralWidget);
    auto* button = new QPushButton("CLick the button to randomize the last angle of the robot", centralWidget);

    mainLayout->addWidget(label);
    mainLayout->addWidget(button);

    m_canvas = new Canvas(300, 300, centralWidget);
    mainLayout->addWidget(m_canvas);

    auto* segment1 = new SegmentWidget("Segment 1 >>", &m_manager, 0, centralWidget);
    auto* segment2 = new SegmentWidget("Segment 2 >>", &m_manager, 1, centralWidget);
    auto* segment3 = new SegmentWidget("Segment 3 >>", &m_manager, 2, centralWidget);

    m_segmentWidgets = {segment1, segment2, segment3};
    guiBox->addWidget(segment1);
    guiBox->addWidget(segment2);
    guiBox->addWidget(segment3);

    auto* animationToggle = new QCheckBox("Animate transitions", centralWidget);
    animationToggle->setChecked(false);

    auto* speedLayout = new QHBoxLayout;
    auto* speedLabel = new QLabel("Joint speed (deg/s):", centralWidget);
    auto* speedSpinBox = new QDoubleSpinBox(centralWidget);
    speedSpinBox->setRange(1.0, 360.0);
    speedSpinBox->setValue(90.0);
    speedSpinBox->setSingleStep(5.0);
    speedSpinBox->setSuffix(" deg/s");
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(speedSpinBox);

    guiBox->addWidget(animationToggle);
    guiBox->addLayout(speedLayout);

    connect(button, &QPushButton::clicked,
            &m_manager, &RobotManager::randomizeLastAngle);
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

    m_canvas->setRobot(m_manager.robot());
}

void RobotController::refreshCanvas()
{
    if (m_canvas)
        m_canvas->update();
}

void RobotController::setup()
{
    m_manager.createDefaultRobot();
    bindRobotToView();
}

void RobotController::addRobot()
{
    m_manager.addRobot();
    bindRobotToView();
}
