#include "robot_window.h"

#include "canvas.h"
#include "gui_labeled_double_spinbox.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QString>

#define DefaultSegmentCount 3

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
    m_moveButton = new QPushButton("Move robot to point B", centralWidget);

    mainLayout->addWidget(label);
    mainLayout->addWidget(m_moveButton);

    m_canvas = new Canvas(300, 300, centralWidget);
    mainLayout->addWidget(m_canvas);

    auto* pointsLayout = new QHBoxLayout;
    createPointGroupBox("Current Point", m_currentEndPointX,
                        m_currentEndPointY, pointsLayout);
    m_currentEndPointX->setReadOnly(true);
    m_currentEndPointY->setReadOnly(true);

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
    auto* segmentsGroupLayout = new QHBoxLayout;
    createSegmentsGroupBoxes(segmentsGroupLayout);
    controlsLayout->addLayout(segmentsGroupLayout);
    controlsLayout->addLayout(speedLayout);
    createPointGroupBox("Target Point", m_targetPointX,
                        m_targetPointY, pointsLayout);
    m_targetPointX->setReadOnly(true);
    m_targetPointY->setReadOnly(true);
    controlsLayout->addLayout(pointsLayout);
    

    // segmentGroup->setLayout(vbox);
    // m_segmentsLayout->addWidget(segmentGroup);
}

Canvas* RobotWindow::canvas() const
{
    return m_canvas;
}

void RobotWindow::setRobotData(const RobotViewData& data)
{
    if (m_canvas)
        m_canvas->setRobotData(data);

    if (m_currentEndPointX && m_currentEndPointY)
    {
        m_currentEndPointX->setValue(data.endPoint.x);
        m_currentEndPointY->setValue(data.endPoint.y);
    }

    if (m_targetPointX && m_targetPointY)
    {
        m_targetPointX->setValue(data.targetPoint.x);
        m_targetPointY->setValue(data.targetPoint.y);
    }
}

QVBoxLayout* RobotWindow::segmentsLayout() const
{
    return m_segmentsLayout;
}

std::vector<RobotSegmentViewControls>& RobotWindow::segmentControls()
{
    return m_segmentControls;
}

LabeledDoubleSpinBox* RobotWindow::endPointX() const
{
    return m_currentEndPointX;
}

LabeledDoubleSpinBox* RobotWindow::endPointY() const
{
    return m_currentEndPointY;
}

LabeledDoubleSpinBox* RobotWindow::targetPointX() const
{
    return m_targetPointX;
}

LabeledDoubleSpinBox* RobotWindow::targetPointY() const
{
    return m_targetPointY;
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

void RobotWindow::addSegment()
{
    auto* newSegmentGroup = new QGroupBox(QString("Segment: %1").arg(m_segmentGroups.size() + 1), this);
    m_segmentGroups.push_back(newSegmentGroup);

    auto* angle = new LabeledDoubleSpinBox("angle", newSegmentGroup);
    angle->setRange(-360.0, 360.0);
    angle->setSingleStep(1.0);

    auto* length = new LabeledDoubleSpinBox("length", newSegmentGroup);
    length->setRange(0.0, 1000.0);
    length->setSingleStep(1.0);

    auto* width = new LabeledDoubleSpinBox("width", newSegmentGroup);
    width->setRange(0.0, 1000.0);
    width->setSingleStep(1.0);

    auto* vbox = new QVBoxLayout;
    vbox->addWidget(angle);
    vbox->addWidget(length);
    vbox->addWidget(width);
    newSegmentGroup->setLayout(vbox);

    m_segmentControls.push_back({newSegmentGroup, angle, length, width});
}

void RobotWindow::createSegmentsGroupBoxes(QHBoxLayout* hbox_layout)
{
    for (int i = 0; i < DefaultSegmentCount; ++i) {
        addSegment();
        m_segmentsLayout->addWidget(m_segmentGroups.at(i));
        hbox_layout->addWidget(m_segmentGroups.at(i));
    }
}

void RobotWindow::createPointGroupBox(const QString& title,
                                      LabeledDoubleSpinBox*& xSpinBox,
                                      LabeledDoubleSpinBox*& ySpinBox,
                                      QHBoxLayout* hboxLayout)
{
    auto* pointGroup = new QGroupBox(title, this);
    auto* pointLayout = new QVBoxLayout(pointGroup);

    xSpinBox = new LabeledDoubleSpinBox("X", pointGroup);
    ySpinBox = new LabeledDoubleSpinBox("Y", pointGroup);

    xSpinBox->setRange(-1000.0, 1000.0);
    ySpinBox->setRange(-1000.0, 1000.0);
    xSpinBox->setSingleStep(1.0);
    ySpinBox->setSingleStep(1.0);

    pointLayout->addWidget(xSpinBox);
    pointLayout->addWidget(ySpinBox);

    if (hboxLayout)
        hboxLayout->addWidget(pointGroup);
}
