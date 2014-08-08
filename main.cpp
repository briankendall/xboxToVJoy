#include "controllerwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControllerWindow w;
    w.show();

    return a.exec();
}
