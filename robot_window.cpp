#include "robot_window.h"

#include "canvas.h"
#include "gui_labeled_double_spinbox.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QString>

#define DefaultSegmentCount 3

RobotWindow::RobotWindow(QWidget* parent)
    : QMainWindow(parent)
{
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("Welcome to the robot simulator!");

    auto* mainLayout = new QVBoxLayout(centralWidget);
    auto* columnsLayout = new QHBoxLayout;
    auto* controlsLayout = new QVBoxLayout;
    m_segmentsLayout = new QVBoxLayout;

    columnsLayout->addLayout(controlsLayout);
    columnsLayout->addLayout(m_segmentsLayout);
    mainLayout->addLayout(columnsLayout);

    m_tryReachUserMousePointButton = new QPushButton(
        "Try reaching user mouse point", centralWidget);

    m_canvas = new Canvas(300, 300, centralWidget);
    mainLayout->addWidget(m_canvas);

    auto* messageLabel = new QLabel("Messages", centralWidget);
    m_messageOutput = new QPlainTextEdit(centralWidget);
    m_messageOutput->setReadOnly(true);
    m_messageOutput->setMaximumBlockCount(100);
    m_messageOutput->setFixedHeight(
        3 * m_messageOutput->fontMetrics().lineSpacing() + 24);
    mainLayout->addWidget(messageLabel);
    mainLayout->addWidget(m_messageOutput);

    auto* pointsLayout = new QHBoxLayout;
    createPointGroupBox("Current Point", m_currentEndPointX,
                        m_currentEndPointY, pointsLayout);
    m_currentEndPointX->setReadOnly(true);
    m_currentEndPointY->setReadOnly(true);

    m_animationToggle = new QCheckBox("Animate transitions", centralWidget);
    m_animationToggle->setChecked(true);

    auto* robotControlsGroup = new QGroupBox("Robot Controls", centralWidget);
    auto* robotControlsLayout = new QVBoxLayout(robotControlsGroup);
    robotControlsLayout->addWidget(m_animationToggle);

    auto* speedLayout = new QHBoxLayout;
    auto* speedLabel = new QLabel("Joint speed (deg/s):", centralWidget);
    m_speedSpinBox = new QDoubleSpinBox(centralWidget);
    m_speedSpinBox->setRange(1.0, 360.0);
    m_speedSpinBox->setValue(20.0);
    m_speedSpinBox->setSingleStep(5.0);
    m_speedSpinBox->setSuffix(" deg/s");
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(m_speedSpinBox);
    robotControlsLayout->addLayout(speedLayout);
    robotControlsLayout->addWidget(m_tryReachUserMousePointButton);

    controlsLayout->addWidget(robotControlsGroup);
    auto* segmentsRowLayout = new QHBoxLayout;
    createSegmentsGroupBoxes(segmentsRowLayout);
    m_segmentsLayout->addLayout(segmentsRowLayout);
    createPointGroupBox("Target Point", m_targetPointX,
                        m_targetPointY, pointsLayout);
    m_targetPointX->setReadOnly(true);
    m_targetPointY->setReadOnly(true);
    createPointGroupBox("User Point", m_userPointX,
                        m_userPointY, pointsLayout);
    m_userPointX->setReadOnly(true);
    m_userPointY->setReadOnly(true);
    m_segmentsLayout->addLayout(pointsLayout);
    

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

    if (m_userPointX && m_userPointY)
    {
        m_userPointX->setEnabled(data.hasUserPoint);
        m_userPointY->setEnabled(data.hasUserPoint);
        m_userPointX->setValue(data.userPoint.x);
        m_userPointY->setValue(data.userPoint.y);
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

LabeledDoubleSpinBox* RobotWindow::userPointX() const
{
    return m_userPointX;
}

LabeledDoubleSpinBox* RobotWindow::userPointY() const
{
    return m_userPointY;
}

QPushButton* RobotWindow::tryReachUserMousePointButton() const
{
    return m_tryReachUserMousePointButton;
}

QCheckBox* RobotWindow::animationToggle() const
{
    return m_animationToggle;
}

QDoubleSpinBox* RobotWindow::speedSpinBox() const
{
    return m_speedSpinBox;
}

void RobotWindow::appendMessage(const QString& message)
{
    if (m_messageOutput)
        m_messageOutput->appendPlainText(message);
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
        if (hbox_layout)
            hbox_layout->addWidget(m_segmentGroups.at(i));
        else
            m_segmentsLayout->addWidget(m_segmentGroups.at(i));
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
