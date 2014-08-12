#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "qmainwidget.h"
#include <tlhelp32.h>

int processesWithName(QString name)
{
    int count = 0;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (QString::fromWCharArray(entry.szExeFile) == name) {
                count += 1;
            }
        }
    }

    return count;
}    

QMainWidget::QMainWidget(QWidget *parent) :
    QWidget(parent)
{
    // This is a poor way to check to see if the app is already running, but
    // quite frankly I'm lazy:
    if (processesWithName("xboxToVJoy.exe") > 1) {
        error("There is already an instance of xboxToVJoy running.");
        return;
    }
    
    createTrayIcon();
    
    controllerWindow = NULL;
    
    controllerRemapper = new ControllerRemapper((HWND)winId(), this);
    connect(controllerRemapper, SIGNAL(initializationError(QString)), this, SLOT(error(QString)));
    controllerRemapper->start();
}

void QMainWidget::createTrayIcon()
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    QMenu *menu = new QMenu(this);
    QAction *windowAction = new QAction("Controller Window", this);
    QAction *quitAction = new QAction("Shutdown xboxToVJoy", this);
    
    connect(windowAction, SIGNAL(triggered()), this, SLOT(showControllerWindow()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
    
    menu->addAction(windowAction);
    menu->addSeparator();
    menu->addAction(quitAction);
    trayIcon->setContextMenu(menu);
    
    QIcon icon(":/icons/icon.png");
    trayIcon->setIcon(icon);
    
    trayIcon->show();
}

void QMainWidget::quit()
{
    if (controllerWindow) {
        controllerWindow->close();
    }
    
    controllerRemapper->exit(0);
    controllerRemapper->wait(5000);
    qApp->quit();
}

void QMainWidget::error(QString msg)
{
    QMessageBox msgBox;
	msgBox.setText("xboxToVJoy failed to start");
	msgBox.setInformativeText(msg);
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.addButton("Quit", QMessageBox::AcceptRole);
	msgBox.exec();
    qApp->quit();
}

void QMainWidget::showControllerWindow()
{
    if (!controllerWindow) {
        controllerWindow = new ControllerWindow(controllerRemapper, this);
        connect(controllerWindow, SIGNAL(destroyed()), this, SLOT(controllerWindowDestroyed()));
    }
    
    controllerWindow->show();
    controllerWindow->raise();
}

void QMainWidget::controllerWindowDestroyed()
{
    controllerWindow = NULL;
}

