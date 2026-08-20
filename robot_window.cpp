#include "robot_window.h"

#include "canvas.h"
#include "gui_labeled_double_spinbox.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

RobotWindow::RobotWindow(QWidget* parent)
    : QMainWindow(parent)
{
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);
    auto* controlsLayout = new QVBoxLayout;
    m_segmentsLayout = new QVBoxLayout;

    mainLayout->addLayout(controlsLayout);
    controlsLayout->addLayout(m_segmentsLayout);

    auto* label = new QLabel("Welcome to the robot simulator!", centralWidget);
    m_randomizeButton = new QPushButton(
        "Click the button to randomize the last angle of the robot", centralWidget);
    m_moveButton = new QPushButton("Move robot to point B", centralWidget);

    mainLayout->addWidget(label);
    mainLayout->addWidget(m_randomizeButton);
    mainLayout->addWidget(m_moveButton);

    m_canvas = new Canvas(300, 300, centralWidget);
    mainLayout->addWidget(m_canvas);

    auto* endpointLayout = new QHBoxLayout;
    m_currentEndPointX = new LabeledDoubleSpinBox("end X", centralWidget);
    m_currentEndPointY = new LabeledDoubleSpinBox("end Y", centralWidget);
    m_currentEndPointX->setReadOnly(true);
    m_currentEndPointY->setReadOnly(true);
    endpointLayout->addWidget(m_currentEndPointX);
    endpointLayout->addWidget(m_currentEndPointY);
    controlsLayout->addLayout(endpointLayout);

    m_animationToggle = new QCheckBox("Animate transitions", centralWidget);
    m_animationToggle->setChecked(true);
    controlsLayout->addWidget(m_animationToggle);

    auto* speedLayout = new QHBoxLayout;
    auto* speedLabel = new QLabel("Joint speed (deg/s):", centralWidget);
    m_speedSpinBox = new QDoubleSpinBox(centralWidget);
    m_speedSpinBox->setRange(1.0, 360.0);
    m_speedSpinBox->setValue(20.0);
    m_speedSpinBox->setSingleStep(5.0);
    m_speedSpinBox->setSuffix(" deg/s");
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(m_speedSpinBox);
    controlsLayout->addLayout(speedLayout);
}

Canvas* RobotWindow::canvas() const
{
    return m_canvas;
}

QVBoxLayout* RobotWindow::segmentsLayout() const
{
    return m_segmentsLayout;
}

LabeledDoubleSpinBox* RobotWindow::endPointX() const
{
    return m_currentEndPointX;
}

LabeledDoubleSpinBox* RobotWindow::endPointY() const
{
    return m_currentEndPointY;
}

QPushButton* RobotWindow::randomizeButton() const
{
    return m_randomizeButton;
}

QPushButton* RobotWindow::moveButton() const
{
    return m_moveButton;
}

QCheckBox* RobotWindow::animationToggle() const
{
    return m_animationToggle;
}

QDoubleSpinBox* RobotWindow::speedSpinBox() const
{
    return m_speedSpinBox;
}