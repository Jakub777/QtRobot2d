#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintDevice>
#include <iostream>
#include "canvas.h"
#include "gui_labeled_double_spinbox.h"
#include "gui_segment_widget.h"
// #include "my_button.cpp"
#include <QPushButton>
#include "r_robot.h"
#define CANVAS_X 300
#define CANVAS_Y 300

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow appWindow;

    QWidget* centralWidget = new QWidget();
    appWindow.setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    QVBoxLayout* guiBox = new QVBoxLayout;

    mainLayout->addLayout(guiBox);

    QLabel *label = new QLabel("Welcome to the robot simulator!");
    QPushButton *button = new QPushButton("CLick the button to randomize the last angle of the robot");

    Canvas *canvas300_300 = new Canvas(CANVAS_X, CANVAS_Y); 

    canvas300_300->initRobot();
    
    mainLayout->addWidget(label);
    mainLayout->addWidget(button);
    mainLayout->addWidget(canvas300_300);
    guiBox->addWidget(new SegmentWidget("Segment 1 >>"));
    guiBox->addWidget(new SegmentWidget("Segment 2 >>"));
    guiBox->addWidget(new SegmentWidget("Segment 3 >>"));

    QObject::connect(button, &QPushButton::clicked,
                     canvas300_300, &Canvas::randomizeLastAngle);

    appWindow.show();

    return app.exec();
}