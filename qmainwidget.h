#ifndef QMAINWIDGET_H
#define QMAINWIDGET_H

#include <QWidget>
#include "controllerremapper.h"
#include "controllerwindow.h"

class QMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMainWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    void quit();
    void error(QString msg);
    void showControllerWindow();
    void controllerWindowDestroyed();
    
private:
    void createTrayIcon();
    void startControllerMapperThread();
    
    ControllerRemapper *controllerRemapper;
    ControllerWindow *controllerWindow;
};

#endif // QMAINWIDGET_H
