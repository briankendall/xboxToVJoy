#ifndef QMAINWIDGET_H
#define QMAINWIDGET_H

#include <QWidget>
#include <QAction>
#include <QCloseEvent>
#include "controllerremapper.h"
#include "controllerwindow.h"

class QMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMainWidget(bool remapEnabled, QWidget *parent = 0);
    void deinitialize();
    
signals:
    
public slots:
    void error(QString msg);
    void showControllerWindow();
    void controllerWindowDestroyed();
    void appMessageReceived(const QString &message);
    void toggleEnabled();
    void setRemappingEnabled(bool enabled);
    
private:
    void createTrayIcon();
    void startControllerMapperThread();
    void closeEvent(QCloseEvent *event);
    
    ControllerRemapper *controllerRemapper;
    ControllerWindow *controllerWindow;
    QAction *enableAction;
    QAction *disableAction;
};

#endif // QMAINWIDGET_H
