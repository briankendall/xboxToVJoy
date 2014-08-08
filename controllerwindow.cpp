#include "controllerwindow.h"
#include "ui_controllerwindow.h"

ControllerWindow::ControllerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControllerWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

ControllerWindow::~ControllerWindow()
{
    delete ui;
}


void ControllerWindow::on_closeButton_clicked()
{
    done(0);
}
