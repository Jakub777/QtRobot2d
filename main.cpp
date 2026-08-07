#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintDevice>
#include <iostream>
#include "canvas.h"
#include "gui_labeled_double_spinbox.h"
// #include "my_button.cpp"
#include <QPushButton>
#include "r_robot.h"
#define CANVAS_X 300
#define CANVAS_Y 300

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow appWindow;

    QWidget *centralWidget = new QWidget();
    appWindow.setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *label = new QLabel("Hello, Qt!");
    QPushButton *button = new QPushButton("hhhh");

    LabeledDoubleSpinBox *spinBox = new LabeledDoubleSpinBox("Angle:");
    Canvas *canvas300_300 = new Canvas(CANVAS_X, CANVAS_Y); 

    canvas300_300->initRobot();
    
    layout->addWidget(label);
    layout->addWidget(button);
    layout->addWidget(canvas300_300);
    layout->addWidget(spinBox);

    QObject::connect(button, &QPushButton::clicked,
                     canvas300_300, &Canvas::randomizeLastAngle);

    appWindow.show();

    return app.exec();
}