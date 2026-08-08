#include <QApplication>
#include <QMainWindow>

#include "robot_controller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow appWindow;
    RobotController controller(&appWindow);

    controller.setup();
    appWindow.show();

    return app.exec();
}