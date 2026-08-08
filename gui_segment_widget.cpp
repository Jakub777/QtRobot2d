#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include "gui_segment_widget.h"
#include "gui_labeled_double_spinbox.h"

SegmentWidget::SegmentWidget(QString name, RobotManager* manager, int index, QWidget* parent)
    : QWidget(parent)
    , m_manager(manager)
    , m_segmentIndex(index)
{
    auto* layout = new QHBoxLayout(this);

    nameLabel = new QLabel(name);
    angleDsSpinBox = new LabeledDoubleSpinBox("angle");
    lengthDsSpinBox = new LabeledDoubleSpinBox("length");
    widthDsSpinBox = new LabeledDoubleSpinBox("width");

    layout->addWidget(nameLabel);
    layout->addWidget(angleDsSpinBox);
    layout->addWidget(lengthDsSpinBox);
    layout->addWidget(widthDsSpinBox);

    connect(angleDsSpinBox, &LabeledDoubleSpinBox::valueChanged,
            this, &SegmentWidget::onAngleChanged);
    connect(lengthDsSpinBox, &LabeledDoubleSpinBox::valueChanged,
            this, &SegmentWidget::onLengthChanged);
    connect(widthDsSpinBox, &LabeledDoubleSpinBox::valueChanged,
            this, &SegmentWidget::onWidthChanged);

    if (!m_manager || m_segmentIndex < 0)
        return;

    auto* robot = m_manager->robot();
    if (!robot || m_segmentIndex >= static_cast<int>(robot->segments.size()))
        return;

    const auto& segment = robot->segments[m_segmentIndex];
    angleDsSpinBox->setValue(segment.joint.angle);
    lengthDsSpinBox->setValue(segment.link.length);
    widthDsSpinBox->setValue(segment.link.width);
}

void SegmentWidget::onAngleChanged(double value)
{
    if (!m_manager || m_segmentIndex < 0)
        return;

    m_manager->setSegmentAngle(m_segmentIndex, value);
}

void SegmentWidget::onLengthChanged(double value)
{
    if (!m_manager || m_segmentIndex < 0)
        return;

    m_manager->setSegmentLength(m_segmentIndex, value);
}

void SegmentWidget::onWidthChanged(double value)
{
    if (!m_manager || m_segmentIndex < 0)
        return;

    m_manager->setSegmentWidth(m_segmentIndex, value);
}