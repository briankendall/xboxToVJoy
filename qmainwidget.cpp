#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "qmainwidget.h"

QMainWidget::QMainWidget(QWidget *parent) :
    QWidget(parent)
{
    createTrayIcon();
    controllerRemapper = new ControllerRemapper(this);
    connect(controllerRemapper, SIGNAL(initializationError(QString)), this, SLOT(remapperError(QString)));
    controllerRemapper->start();
}

void QMainWidget::createTrayIcon()
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    QMenu *menu = new QMenu(this);
    QAction *windowAction = new QAction("Controller Window", this);
    QAction *quitAction = new QAction("Shutdown xboxToVJoy", this);
    
    // connect(windowAction, SIGNAL(triggered()), );
    QObject::connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
    
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
    controllerRemapper->exit(0);
    controllerRemapper->wait(5000);
    qApp->quit();
}

void QMainWidget::remapperError(QString msg)
{
    QMessageBox msgBox;
	msgBox.setText("xboxToVJoy failed to start");
	msgBox.setInformativeText(msg);
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.addButton("Quit", QMessageBox::AcceptRole);
	msgBox.exec();
    qApp->quit();
}
