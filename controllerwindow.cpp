#include <QDebug>
#include <windows.h>
#include <Xinput.h>
#include "controllerwindow.h"
#include "controllerremapper.h"
#include "ui_controllerwindow.h"

ControllerWindow::ControllerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControllerWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
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
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_A);
}

void ControllerWindow::on_BButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_B);
}

void ControllerWindow::on_XButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_X);
}

void ControllerWindow::on_YButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_Y);
}

void ControllerWindow::on_rightBumperButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_RIGHT_SHOULDER);
}

void ControllerWindow::on_leftBumperButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_LEFT_SHOULDER);
}

void ControllerWindow::on_startButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_START);
}

void ControllerWindow::on_backButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_BACK);
}

void ControllerWindow::on_guideButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_GUIDE);
}

void ControllerWindow::on_rightTriggerButton_clicked()
{
    pressTrigger(currentDeviceIndex, true, 1.0);
}

void ControllerWindow::on_leftTriggerButton_clicked()
{
    pressTrigger(currentDeviceIndex, false, 1.0);
}

void ControllerWindow::on_dpadLeftButton_clicked()
{
    moveDPad(currentDeviceIndex, kLeft);
}

void ControllerWindow::on_dpadUpLeftButton_clicked()
{
    moveDPad(currentDeviceIndex, kUpLeft);
}

void ControllerWindow::on_dpadUpButton_clicked()
{
    moveDPad(currentDeviceIndex, kUp);
}

void ControllerWindow::on_dpadUpRightButton_clicked()
{
    moveDPad(currentDeviceIndex, kUpRight);
}

void ControllerWindow::on_dpadRightButton_clicked()
{
    moveDPad(currentDeviceIndex, kRight);
}

void ControllerWindow::on_dpadDownRightButton_clicked()
{
    moveDPad(currentDeviceIndex, kDownRight);
}

void ControllerWindow::on_dpadDownButton_clicked()
{
    moveDPad(currentDeviceIndex, kDown);
}

void ControllerWindow::on_dpadDownLeftButton_clicked()
{
    moveDPad(currentDeviceIndex, kDownLeft);
}


void ControllerWindow::on_LLeftButton_clicked()
{
    moveJoystick(currentDeviceIndex, false, -1.0, 0.0);
}

void ControllerWindow::on_LUpLeftButton_clicked()
{
    moveJoystick(currentDeviceIndex, false, -1.0, -1.0);
}

void ControllerWindow::on_LUpButton_clicked()
{
    moveJoystick(currentDeviceIndex, false, 0.0, -1.0);
}

void ControllerWindow::on_LUpRightButton_clicked()
{
    moveJoystick(currentDeviceIndex, false, 1.0, -1.0);
}

void ControllerWindow::on_LRightButton_clicked()
{
    moveJoystick(currentDeviceIndex, false, 1.0, 0.0);
}

void ControllerWindow::on_LDownRightButton_clicked()
{
    moveJoystick(currentDeviceIndex, false, 1.0, 1.0);
}

void ControllerWindow::on_LDownButton_clicked()
{
    moveJoystick(currentDeviceIndex, false, 0.0, 1.0);
}

void ControllerWindow::on_LDownLeftButton_clicked()
{
    moveJoystick(currentDeviceIndex, false, -1.0, 1.0);
}

void ControllerWindow::on_LClickButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_LEFT_THUMB);
}


void ControllerWindow::on_RLeftButton_clicked()
{
    moveJoystick(currentDeviceIndex, true, -1.0, 0.0);
}

void ControllerWindow::on_RUpLeftButton_clicked()
{
    moveJoystick(currentDeviceIndex, true, -1.0, -1.0);
}

void ControllerWindow::on_RUpButton_clicked()
{
    moveJoystick(currentDeviceIndex, true, 0.0, -1.0);
}

void ControllerWindow::on_RUpRightButton_clicked()
{
    moveJoystick(currentDeviceIndex, true, 1.0, -1.0);
}

void ControllerWindow::on_RRightButton_clicked()
{
    moveJoystick(currentDeviceIndex, true, 1.0, 0.0);
}

void ControllerWindow::on_RDownRightButton_clicked()
{
    moveJoystick(currentDeviceIndex, true, 1.0, 1.0);
}

void ControllerWindow::on_RDownButton_clicked()
{
    moveJoystick(currentDeviceIndex, true, 0.0, 1.0);
}

void ControllerWindow::on_RDownLeftButton_clicked()
{
    moveJoystick(currentDeviceIndex, true, -1.0, 1.0);
}

void ControllerWindow::on_RClickButton_clicked()
{
    pressButton(currentDeviceIndex, XINPUT_GAMEPAD_RIGHT_THUMB);
}

