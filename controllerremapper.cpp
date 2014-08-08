#include <QDebug>
#include "controllerremapper.h"

ControllerRemapper::ControllerRemapper(QObject *parent) :
    QThread(parent)
{
}

void ControllerRemapper::run()
{
    qDebug() << "Starting thread";
    
    emit initializationFailed("Testing one two three");
    exit(0);
    
    exec();
    
    qDebug() << "Thread finished";
}
