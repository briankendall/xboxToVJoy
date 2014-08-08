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
    void on_controllerComboBox_currentIndexChanged(int index);
    
    
    void on_AButton_clicked();
    void on_BButton_clicked();
    void on_XButton_clicked();
    void on_YButton_clicked();
    void on_rightBumperButton_clicked();
    void on_leftBumperButton_clicked();
    void on_startButton_clicked();
    void on_backButton_clicked();
    void on_guideButton_clicked();
    void on_rightTriggerButton_clicked();
    void on_leftTriggerButton_clicked();
    
    void on_dpadLeftButton_clicked();
    void on_dpadUpLeftButton_clicked();
    void on_dpadUpButton_clicked();
    void on_dpadUpRightButton_clicked();
    void on_dpadRightButton_clicked();
    void on_dpadDownRightButton_clicked();
    void on_dpadDownButton_clicked();
    void on_dpadDownLeftButton_clicked();
    
    void on_LLeftButton_clicked();
    void on_LUpLeftButton_clicked();
    void on_LUpButton_clicked();
    void on_LUpRightButton_clicked();
    void on_LRightButton_clicked();
    void on_LDownRightButton_clicked();
    void on_LDownButton_clicked();
    void on_LDownLeftButton_clicked();
    void on_LClickButton_clicked();
    
    void on_RLeftButton_clicked();
    void on_RUpLeftButton_clicked();
    void on_RUpButton_clicked();
    void on_RUpRightButton_clicked();
    void on_RRightButton_clicked();
    void on_RDownRightButton_clicked();
    void on_RDownButton_clicked();
    void on_RDownLeftButton_clicked();
    void on_RClickButton_clicked();
    
private:
    Ui::ControllerWindow *ui;
    int currentDeviceIndex;
};

#endif // CONTROLLERWINDOW_H
