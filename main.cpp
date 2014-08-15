#include "controllerwindow.h"
#include "qmainwidget.h"
#include <QApplication>
#include <QDebug>
#include <windows.h>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWidget *mainWidget;

    if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)) {
        qWarning() << "Warning: failed to set process priority. Error:" << GetLastError();
    }
    
    qApp->setQuitOnLastWindowClosed(false);    
    
    mainWidget = new QMainWidget(NULL);

    int result = a.exec();
    
    qDebug() << "Shutting down...";
    mainWidget->deinitialize();
    delete mainWidget;
    
    qDebug() << "Done";
    return result;
}
