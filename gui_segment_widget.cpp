#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include "gui_segment_widget.h"
#include "gui_labeled_double_spinbox.h"
#include <QSignalBlocker>

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
}

void SegmentWidget::setData(const RobotSegmentViewData& data, bool moving)
{
    const QSignalBlocker angleBlocker(angleDsSpinBox);
    const QSignalBlocker lengthBlocker(lengthDsSpinBox);
    const QSignalBlocker widthBlocker(widthDsSpinBox);

    angleDsSpinBox->setValue(data.angle);
    lengthDsSpinBox->setValue(data.length);
    widthDsSpinBox->setValue(data.width);

    angleDsSpinBox->setEnabled(!moving);
    lengthDsSpinBox->setEnabled(!moving);
    widthDsSpinBox->setEnabled(!moving);
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