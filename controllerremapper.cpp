#include <QDebug>
#include "controllerremapper.h"

ControllerRemapper::ControllerRemapper(QObject *parent) :
    QThread(parent)
{
}

void ControllerRemapper::run()
{
    qDebug() << "Starting thread";
    
    exec();
    
    qDebug() << "Thread finished";
}
