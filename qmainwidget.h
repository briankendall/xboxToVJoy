#ifndef QMAINWIDGET_H
#define QMAINWIDGET_H

#include <QWidget>
#include "controllerremapper.h"

class QMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMainWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    void quit();
    
private:
    void createTrayIcon();
    void startControllerMapperThread();
    
    ControllerRemapper *controllerRemapper;
};

#endif // QMAINWIDGET_H
