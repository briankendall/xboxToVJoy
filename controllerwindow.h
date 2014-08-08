#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QDialog>

namespace Ui {
class ControllerWindow;
}

class ControllerWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit ControllerWindow(QWidget *parent = 0);
    ~ControllerWindow();
    
private slots:
    void on_closeButton_clicked();
    
private:
    Ui::ControllerWindow *ui;
};

#endif // CONTROLLERWINDOW_H
