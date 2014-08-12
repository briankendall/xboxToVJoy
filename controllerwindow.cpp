#include <QDebug>
#include <windows.h>
#include <Xinput.h>
#include <qt_windows.h>
#include "controllerwindow.h"
#include "ui_controllerwindow.h"

ControllerWindow::ControllerWindow(ControllerRemapper *ir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControllerWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_ShowWithoutActivating);
    
    HWND id = (HWND)winId();
    SetWindowLong(id, GWL_EXSTYLE, GetWindowLong(id, GWL_EXSTYLE) | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
    
    remapper = ir;
    currentDeviceIndex = 0;
    ui->controllerComboBox->setCurrentIndex(currentDeviceIndex);
    setFixedSize(size());
}

ControllerWindow::~ControllerWindow()
{
    delete ui;
}

void ControllerWindow::on_closeButton_clicked()
{
    done(0);
}

void ControllerWindow::on_controllerComboBox_currentIndexChanged(int index)
{
    currentDeviceIndex = index;
}

void ControllerWindow::on_AButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_A);
}

void ControllerWindow::on_BButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_B);
}

void ControllerWindow::on_XButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_X);
}

void ControllerWindow::on_YButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_Y);
}

void ControllerWindow::on_rightBumperButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_RIGHT_SHOULDER);
}

void ControllerWindow::on_leftBumperButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_LEFT_SHOULDER);
}

void ControllerWindow::on_startButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_START);
}

void ControllerWindow::on_backButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_BACK);
}

void ControllerWindow::on_guideButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_GUIDE);
}

void ControllerWindow::on_rightTriggerButton_clicked()
{
    remapper->pressTrigger(currentDeviceIndex, true, 1.0);
}

void ControllerWindow::on_leftTriggerButton_clicked()
{
    remapper->pressTrigger(currentDeviceIndex, false, 1.0);
}

void ControllerWindow::on_dpadLeftButton_clicked()
{
    remapper->moveDPad(currentDeviceIndex, kLeft);
}

void ControllerWindow::on_dpadUpLeftButton_clicked()
{
    remapper->moveDPad(currentDeviceIndex, kUpLeft);
}

void ControllerWindow::on_dpadUpButton_clicked()
{
    remapper->moveDPad(currentDeviceIndex, kUp);
}

void ControllerWindow::on_dpadUpRightButton_clicked()
{
    remapper->moveDPad(currentDeviceIndex, kUpRight);
}

void ControllerWindow::on_dpadRightButton_clicked()
{
    remapper->moveDPad(currentDeviceIndex, kRight);
}

void ControllerWindow::on_dpadDownRightButton_clicked()
{
    remapper->moveDPad(currentDeviceIndex, kDownRight);
}

void ControllerWindow::on_dpadDownButton_clicked()
{
    remapper->moveDPad(currentDeviceIndex, kDown);
}

void ControllerWindow::on_dpadDownLeftButton_clicked()
{
    remapper->moveDPad(currentDeviceIndex, kDownLeft);
}


void ControllerWindow::on_LLeftButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, false, -1.0, 0.0);
}

void ControllerWindow::on_LUpLeftButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, false, -1.0, -1.0);
}

void ControllerWindow::on_LUpButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, false, 0.0, -1.0);
}

void ControllerWindow::on_LUpRightButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, false, 1.0, -1.0);
}

void ControllerWindow::on_LRightButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, false, 1.0, 0.0);
}

void ControllerWindow::on_LDownRightButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, false, 1.0, 1.0);
}

void ControllerWindow::on_LDownButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, false, 0.0, 1.0);
}

void ControllerWindow::on_LDownLeftButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, false, -1.0, 1.0);
}

void ControllerWindow::on_LClickButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_LEFT_THUMB);
}


void ControllerWindow::on_RLeftButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, true, -1.0, 0.0);
}

void ControllerWindow::on_RUpLeftButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, true, -1.0, -1.0);
}

void ControllerWindow::on_RUpButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, true, 0.0, -1.0);
}

void ControllerWindow::on_RUpRightButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, true, 1.0, -1.0);
}

void ControllerWindow::on_RRightButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, true, 1.0, 0.0);
}

void ControllerWindow::on_RDownRightButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, true, 1.0, 1.0);
}

void ControllerWindow::on_RDownButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, true, 0.0, 1.0);
}

void ControllerWindow::on_RDownLeftButton_clicked()
{
    remapper->moveJoystick(currentDeviceIndex, true, -1.0, 1.0);
}

void ControllerWindow::on_RClickButton_clicked()
{
    remapper->pressButton(currentDeviceIndex, XINPUT_GAMEPAD_RIGHT_THUMB);
}

