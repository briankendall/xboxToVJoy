#ifndef CONTROLLERREMAPPER_H
#define CONTROLLERREMAPPER_H

#include <windows.h>
#include <QThread>
#include <QSet>
#include <QVector>
#include <QTimer>
#include <QMap>

#define XINPUT_GAMEPAD_GUIDE 0x400
#define kButtonCount 11
// The following is a value that an XInput axis is guaranteed never to have, and
// is therefore safe to use as a flag indicating an unset value:
#define kAxisUnset -100000

enum {
    kUp = 0,
    kUpRight,
    kRight,
    kDownRight,
    kDown,
    kDownLeft,
    kLeft,
    kUpLeft,
    kNoDirection
};

class Controller {
public:
    Controller() {};
    void initialize();
    void doControllerMap(UINT vjoyDeviceId);
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
    explicit ControllerRemapper(HWND win, bool inEnabled = true, QObject *parent = 0);
    
    void pressButton(UINT deviceIndex, UINT xboxButton);
    void moveJoystick(UINT deviceIndex, bool right, double xVal, double yVal);
    void pressTrigger(UINT deviceIndex, bool right, double val);
    void moveDPad(UINT deviceIndex, int direction);
    bool isEnabled();
    
signals:
    
    void initializationError(QString msg);
    
public slots:
    void poll();
    void setEnabled(bool inEnabled);
    
protected:
    virtual void run() Q_DECL_OVERRIDE;
    void initialize();
    void deinitialize();
    void initializeDevice(UINT deviceId);
    void throwInitError(QString arg);
    bool checkAxisExists(UINT deviceId, UINT axis, QString axisName);
    HINSTANCE getXInputDLLHandle();
    
    QSet<UINT> initializedDevices;
    Controller controllers[4];
    QTimer *pollTimer;
    unsigned int controllerCount;
    HWND dinputWindow;
    QMap<UINT, UINT> xboxToVJoyMap;
    bool errorOccurred;
    bool enabled;
};

#endif // CONTROLLERREMAPPER_H
