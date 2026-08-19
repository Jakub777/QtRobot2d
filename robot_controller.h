#pragma once

#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QVBoxLayout>
#include <vector>
#include "canvas.h"
#include "gui_segment_widget.h"
#include "robot_manager.h"

class RobotController : public QObject
{
    Q_OBJECT

public:
    explicit RobotController(QMainWindow* window = nullptr, QObject* parent = nullptr);

    void setup();
    void addRobot();

private:
    void createMainWindow();
    void bindRobotToView();
    void refreshCanvas();
    void syncSegmentWidgets();
    Point2D canvasStartPoint() const;

    QMainWindow* m_window = nullptr;
    RobotManager m_manager;
    Canvas* m_canvas = nullptr;
    QVBoxLayout* m_segmentsLayout = nullptr;
    QTimer* m_syncTimer = nullptr;
    std::vector<SegmentWidget*> m_segmentWidgets;
};
