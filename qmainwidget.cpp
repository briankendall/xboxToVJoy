#include "qmainwidget.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>

QMainWidget::QMainWidget(QWidget *parent) :
    QWidget(parent)
{
    createTrayIcon();
}

void QMainWidget::createTrayIcon()
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    QMenu *menu = new QMenu(this);
    QAction *windowAction = new QAction("Controller Window", this);
    QAction *quitAction = new QAction("Shutdown xboxToVJoy", this);
    
    // connect(windowAction, SIGNAL(triggered()), );
    QObject::connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    
    menu->addAction(windowAction);
    menu->addSeparator();
    menu->addAction(quitAction);
    trayIcon->setContextMenu(menu);
    
    QIcon icon(":/icons/icon.png");
    trayIcon->setIcon(icon);
    
    trayIcon->show();
}
