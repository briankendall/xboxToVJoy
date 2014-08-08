#ifndef CONTROLLERREMAPPER_H
#define CONTROLLERREMAPPER_H

#include <windows.h>
#include <QThread>
#include <QSet>
#include <QVector>
#include <QTimer>

#define kButtonCount 11
// The following is a value that an XInput axis is guaranteed never to have, and
// is therefore safe to use as a flag indicating an unset value:
#define kAxisUnset -100000
#define kPollingCyclesPerSecond 200

class Controller {
public:
    Controller() {};
    void initialize();
    void doControllerMap();
    void reset();
    
    UINT deviceIndex;
private:
    QVector<bool> buttonsDown;
    QVector<bool> directionDown;
    long lastLeftTrigger, lastRightTrigger, lastLX, lastLY, lastRX, lastRY;
    DWORD lastPacketNumber;
    bool lastConnected;
    bool handledFirstPacket;
};

class ControllerRemapper : public QThread
{
    Q_OBJECT
public:
    explicit ControllerRemapper(QObject *parent = 0);
    
signals:
    
    void initializationError(QString msg);
    
public slots:
    void poll();
    
protected:
    virtual void run() Q_DECL_OVERRIDE;
    void initialize();
    void deinitialize();
    void initializeDevice(UINT deviceId);
    void throwInitError(QString arg);
    bool checkAxisExists(UINT deviceId, UINT axis, QString axisName);
    
    QSet<UINT> initializedDevices;
    Controller controllers[4];
    QTimer *pollTimer;
    
};

#endif // CONTROLLERREMAPPER_H
