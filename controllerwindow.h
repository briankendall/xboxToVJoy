#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QMainWindow>

class ControllerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ControllerWindow(QWidget *parent = 0);
    ~ControllerWindow();
};

#endif
