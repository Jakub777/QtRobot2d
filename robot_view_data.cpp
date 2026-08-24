#include "robot_view_data.h"

#include "gui_labeled_double_spinbox.h"
#include "robot_manager.h"

#include <QSignalBlocker>

void connectSegmentControls(RobotSegmentViewControls& controls,
                            RobotManager* manager, int segmentIndex)
{
    if (controls.signalsConnected || !manager || segmentIndex < 0)
        return;

    QObject::connect(controls.angle, &LabeledDoubleSpinBox::valueChanged,
                     manager, [manager, segmentIndex](double value) {
        manager->setSegmentAngle(segmentIndex, value);
    });
    QObject::connect(controls.length, &LabeledDoubleSpinBox::valueChanged,
                     manager, [manager, segmentIndex](double value) {
        manager->setSegmentLength(segmentIndex, value);
    });
    QObject::connect(controls.width, &LabeledDoubleSpinBox::valueChanged,
                     manager, [manager, segmentIndex](double value) {
        manager->setSegmentWidth(segmentIndex, value);
    });

    controls.signalsConnected = true;
}

void setSegmentControls(const RobotSegmentViewControls& controls,
                        const RobotSegmentViewData& data, bool moving)
{
    const QSignalBlocker angleBlocker(controls.angle);
    const QSignalBlocker lengthBlocker(controls.length);
    const QSignalBlocker widthBlocker(controls.width);

    controls.angle->setValue(data.angle);
    controls.length->setValue(data.length);
    controls.width->setValue(data.width);

    controls.angle->setEnabled(!moving);
    controls.length->setEnabled(!moving);
    controls.width->setEnabled(!moving);
}