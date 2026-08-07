#pragma once

#include <QLabel>
#include <QWidget>
#include "gui_labeled_double_spinbox.h"
#include "r_segment.h"

class SegmentWidget : public QWidget
{
public:
    SegmentWidget(QString name, QWidget* parent = nullptr);
    Segment* segment;
    QLabel* nameLabel;

private:
    LabeledDoubleSpinBox* angleDsSpinBox;
    LabeledDoubleSpinBox* lengthDsSpinBox;
    LabeledDoubleSpinBox* widthDsSpinBox;
};