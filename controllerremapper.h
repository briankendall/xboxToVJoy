#ifndef CONTROLLERREMAPPER_H
#define CONTROLLERREMAPPER_H

#include <QThread>

class ControllerRemapper : public QThread
{
    Q_OBJECT
public:
    explicit ControllerRemapper(QObject *parent = 0);
    
signals:
    
    void initializationFailed(QString msg);
    
public slots:
    
protected:
    virtual void run() Q_DECL_OVERRIDE;
    void initialize();
};

#endif // CONTROLLERREMAPPER_H
