#pragma once

#include <QMainWindow>
#include "robot_view_data.h"

#include <vector>

class Canvas;
class QCheckBox;
class QDoubleSpinBox;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class LabeledDoubleSpinBox;
class QString;
class Point2D;

class RobotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RobotWindow(QWidget* parent = nullptr);
    
    void addSegment();
    void setRobotData(const RobotViewData& data);
    Canvas* canvas() const;
    void createSegmentsGroupBoxes(QHBoxLayout* hbox_layout = nullptr);
    QVBoxLayout* segmentsLayout() const;
    std::vector<RobotSegmentViewControls>& segmentControls();
    LabeledDoubleSpinBox* endPointX() const;
    LabeledDoubleSpinBox* endPointY() const;
    LabeledDoubleSpinBox* targetPointX() const;
    LabeledDoubleSpinBox* targetPointY() const;
    LabeledDoubleSpinBox* userPointX() const;
    LabeledDoubleSpinBox* userPointY() const;
    QPushButton* moveButton() const;
    QCheckBox* animationToggle() const;
    QDoubleSpinBox* speedSpinBox() const;

private:
    void createPointGroupBox(const QString& title,
                             LabeledDoubleSpinBox*& xSpinBox,
                             LabeledDoubleSpinBox*& ySpinBox,
                             QHBoxLayout* hboxLayout);

    Canvas* m_canvas = nullptr;
    QVBoxLayout* m_segmentsLayout = nullptr;
    LabeledDoubleSpinBox* m_currentEndPointX = nullptr;
    LabeledDoubleSpinBox* m_currentEndPointY = nullptr;
    LabeledDoubleSpinBox* m_targetPointX = nullptr;
    LabeledDoubleSpinBox* m_targetPointY = nullptr;
    LabeledDoubleSpinBox* m_userPointX = nullptr;
    LabeledDoubleSpinBox* m_userPointY = nullptr;
    QPushButton* m_moveButton = nullptr;
    QCheckBox* m_animationToggle = nullptr;
    QDoubleSpinBox* m_speedSpinBox = nullptr;
    std::vector<QGroupBox*> m_segmentGroups;
    std::vector<RobotSegmentViewControls> m_segmentControls;
};