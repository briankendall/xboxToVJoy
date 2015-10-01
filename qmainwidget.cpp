#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "qmainwidget.h"
#include <tlhelp32.h>   

QMainWidget::QMainWidget(bool remapEnabled, QWidget *parent) :
    QWidget(parent)
{
    createTrayIcon();
    
    controllerWindow = NULL;
    
    controllerRemapper = new ControllerRemapper((HWND)winId(), remapEnabled, NULL);
    controllerRemapper->moveToThread(controllerRemapper);
    connect(controllerRemapper, SIGNAL(initializationError(QString)), this, SLOT(error(QString)));
    controllerRemapper->start();
}

void QMainWidget::deinitialize()
{
    if (controllerWindow) {
        controllerWindow->close();
    }
    
    controllerRemapper->exit(0);
    controllerRemapper->wait(5000);
    delete controllerRemapper;
}

void QMainWidget::createTrayIcon()
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    QMenu *menu = new QMenu(this);
    QAction *windowAction = new QAction("Controller Window", this);
    QAction *quitAction = new QAction("Shutdown xboxToVJoy", this);
    
    enableAction = new QAction("Disable", this);
    connect(enableAction, SIGNAL(triggered()), this, SLOT(toggleEnabled()));
    
    connect(windowAction, SIGNAL(triggered()), this, SLOT(showControllerWindow()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    
    menu->addAction(windowAction);
    menu->addSeparator();
    menu->addAction(enableAction);
    menu->addSeparator();
    menu->addAction(quitAction);
    trayIcon->setContextMenu(menu);
    
    QIcon icon(":/icons/icon.png");
    trayIcon->setIcon(icon);
    
    trayIcon->show();
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

void QMainWidget::closeEvent(QCloseEvent *event)
{
    qApp->quit();
    event->ignore();
}

void QMainWidget::setRemappingEnabled(bool enabled)
{
    controllerRemapper->setEnabled(enabled);
    
    if (enabled) {
        enableAction->setText("Disable");
    } else {
        enableAction->setText("Enable");
    }
}

void QMainWidget::toggleEnabled()
{
    if (controllerRemapper->isEnabled()) {
        setRemappingEnabled(false);
    } else {
        setRemappingEnabled(true);
    }
}

void QMainWidget::appMessageReceived(const QString &message)
{
    if (message == "enable") {
        setRemappingEnabled(true);
    } else if (message == "disable") {
        setRemappingEnabled(false);
    }
}
