#ifndef QMAINWIDGET_H
#define QMAINWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include "controllerremapper.h"
#include "controllerwindow.h"

class QMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMainWidget(QWidget *parent = 0);
    void deinitialize();
    
signals:
    
public slots:
    void error(QString msg);
    void showControllerWindow();
    void controllerWindowDestroyed();
    
private:
    void createTrayIcon();
    void startControllerMapperThread();
    void closeEvent(QCloseEvent *event);
    
    ControllerRemapper *controllerRemapper;
    ControllerWindow *controllerWindow;
};

#endif // QMAINWIDGET_H
