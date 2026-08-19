#include <QApplication>
#include <QMainWindow>

#include "robot_controller.h"
#include "robot_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    RobotWindow appWindow;
    RobotController controller(&appWindow);

    controller.setup();
    appWindow.show();

    return app.exec();
}