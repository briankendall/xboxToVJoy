#ifndef CONTROLLERREMAPPER_H
#define CONTROLLERREMAPPER_H

#include <QThread>

class ControllerRemapper : public QThread
{
    Q_OBJECT
public:
    explicit ControllerRemapper(QObject *parent = 0);
    
signals:
    
public slots:
    
protected:
    virtual void run() Q_DECL_OVERRIDE;
};

#endif // CONTROLLERREMAPPER_H
