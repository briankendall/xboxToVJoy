#include "controllerwindow.h"
#include "qmainwidget.h"
#include <QApplication>
#include <QDebug>
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWidget *mainWidget;
    //ControllerWindow w;
    //w.show();

    if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)) {
        qWarning() << "Warning: failed to set process priority. Error:" << GetLastError();
    } 
    
    mainWidget = new QMainWidget(NULL);

    int result = a.exec();
    
    delete mainWidget;
    return result;
}
