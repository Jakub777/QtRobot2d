#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include "gui_segment_widget.h"
#include "gui_labeled_double_spinbox.h"

SegmentWidget::SegmentWidget(QString name, QWidget* parent)
    : QWidget(parent)
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
}