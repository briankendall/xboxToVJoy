#include "controllerwindow.h"
#include "qmainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWidget *mainWidget;
    //ControllerWindow w;
    //w.show();

    mainWidget = new QMainWidget(NULL);

    int result = a.exec();
    
    delete mainWidget;
    return result;
}
