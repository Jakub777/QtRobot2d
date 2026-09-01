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
class QPlainTextEdit;
class QString;
class Point2D;

class RobotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RobotWindow(QWidget* parent = nullptr);
    
    void addSegment();
    void removeSegment();
    void setSegmentControlCount(int count);
    int segmentCount() const;
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
    QPushButton* tryReachUserMousePointButton() const;
    QPushButton* addSegmentButton() const;
    QPushButton* removeSegmentButton() const;
    QCheckBox* animationToggle() const;
    QDoubleSpinBox* speedSpinBox() const;
    void appendMessage(const QString& message);

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
    QPushButton* m_tryReachUserMousePointButton = nullptr;
    QCheckBox* m_animationToggle = nullptr;
    QDoubleSpinBox* m_speedSpinBox = nullptr;
    QPlainTextEdit* m_messageOutput = nullptr;
    QPushButton* m_addSegmentButton = nullptr;
    QPushButton* m_removeSegmentButton = nullptr;
    QHBoxLayout* m_segmentsRowLayout = nullptr;
    std::vector<QGroupBox*> m_segmentGroups;
    std::vector<RobotSegmentViewControls> m_segmentControls;
};