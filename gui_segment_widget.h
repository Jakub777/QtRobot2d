#pragma once

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include "gui_labeled_double_spinbox.h"
#include "robot_manager.h"
#include "robot_view_data.h"

class SegmentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SegmentWidget(QString name, RobotManager* manager = nullptr, int index = -1, QWidget* parent = nullptr);
    void setData(const RobotSegmentViewData& data, bool moving);

private slots:
    void onAngleChanged(double value);
    void onLengthChanged(double value);
    void onWidthChanged(double value);

private:
    QLabel* nameLabel = nullptr;
    LabeledDoubleSpinBox* angleDsSpinBox = nullptr;
    LabeledDoubleSpinBox* lengthDsSpinBox = nullptr;
    LabeledDoubleSpinBox* widthDsSpinBox = nullptr;

    RobotManager* m_manager = nullptr;
    int m_segmentIndex = -1;
};