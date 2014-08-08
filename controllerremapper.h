#ifndef CONTROLLERREMAPPER_H
#define CONTROLLERREMAPPER_H

#include <windows.h>
#include <QThread>
#include <QSet>

class ControllerRemapper : public QThread
{
    Q_OBJECT
public:
    explicit ControllerRemapper(QObject *parent = 0);
    
signals:
    
    void initializationError(QString msg);
    
public slots:
    
protected:
    virtual void run() Q_DECL_OVERRIDE;
    void initialize();
    void deinitialize();
    void initializeDevice(UINT deviceId);
    void throwInitError(QString arg);
    bool checkAxisExists(UINT deviceId, UINT axis, QString axisName);
    
    QSet<UINT> initializedDevices;
};

#endif // CONTROLLERREMAPPER_H
