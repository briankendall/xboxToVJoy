#include "controllerwindow.h"
#include "qmainwidget.h"
#include <QApplication>
#include "QtSingleApplication/QtSingleApplication"
#include <QDebug>
#include <windows.h>
#include <QString>

QString getArgCommand(int argc, char *argv[])
{
    for(int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--enable") == 0 || strcmp(argv[i], "-e") == 0) {
            return "enable";
        } else if (strcmp(argv[i], "--disable") == 0 || strcmp(argv[i], "-d") == 0) {
            return "disable";
        }
    }
    return "";
}

int main(int argc, char *argv[])
{
    QtSingleApplication app("org.what.huh.bribri.xboxtovjoy", argc, argv);
    QString command = getArgCommand(argc, argv);
    
    if (app.isRunning()) {
        if (!command.isEmpty()) {
            qDebug() << "Sending running instance command:" << command;
        	app.sendMessage(command, 500);
        } else {
            qDebug() << "Already running";
        }
        return 0;
	}
    
    QMainWidget *mainWidget;
    
    if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)) {
        qWarning() << "Warning: failed to set process priority. Error:" << GetLastError();
    }
    
    qApp->setQuitOnLastWindowClosed(false);    
    
    mainWidget = new QMainWidget(command != "disable", NULL);
    app.connect(&app, SIGNAL(messageReceived(QString)), mainWidget, SLOT(appMessageReceived(QString)));
    
    if (!command.isEmpty()) {
        mainWidget->appMessageReceived(command);
    }
    
    int result = app.exec();
    
    qDebug() << "Shutting down...";
    mainWidget->deinitialize();
    delete mainWidget;
    
    qDebug() << "Done";
    return result;
}
