#pragma once

#include <QMainWindow>

class Canvas;
class QCheckBox;
class QDoubleSpinBox;
class QPushButton;
class QVBoxLayout;
class LabeledDoubleSpinBox;

class RobotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RobotWindow(QWidget* parent = nullptr);

    Canvas* canvas() const;
    QVBoxLayout* segmentsLayout() const;
    LabeledDoubleSpinBox* endPointX() const;
    LabeledDoubleSpinBox* endPointY() const;
    QPushButton* randomizeButton() const;
    QPushButton* moveButton() const;
    QCheckBox* animationToggle() const;
    QDoubleSpinBox* speedSpinBox() const;

private:
    Canvas* m_canvas = nullptr;
    QVBoxLayout* m_segmentsLayout = nullptr;
    LabeledDoubleSpinBox* m_currentEndPointX = nullptr;
    LabeledDoubleSpinBox* m_currentEndPointY = nullptr;
    QPushButton* m_randomizeButton = nullptr;
    QPushButton* m_moveButton = nullptr;
    QCheckBox* m_animationToggle = nullptr;
    QDoubleSpinBox* m_speedSpinBox = nullptr;
};