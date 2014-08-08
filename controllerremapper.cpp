#include <windows.h>
#include <XInput.h>
#include <QDebug>
#include "controllerremapper.h"
#include "public.h"
#include "vjoyinterface.h"

typedef DWORD (WINAPI* XInputGetStateEx_t)(DWORD dwUserIndex, XINPUT_STATE *pState);
XInputGetStateEx_t XInputGetStateEx = NULL;

#define XINPUT_GAMEPAD_GUIDE 0x400

ControllerRemapper::ControllerRemapper(QObject *parent) :
    QThread(parent)
{
}

bool ControllerRemapper::checkAxisExists(UINT deviceId, UINT axis, QString axisName)
{
    if (!GetVJDAxisExist(deviceId, axis)) {
        throwInitError(QString("vJoy device %1 does not have the following axis: %2\nPlease make sure the first four vJoy devices have "
                               "this axis before launching.").arg(deviceId).arg(axisName));
        return false;
    } else {
        return true;
    }
}

void ControllerRemapper::initializeDevice(UINT deviceId)
{
    // Get the driver attributes (Vendor ID, Product ID, Version Number)
    if (!vJoyEnabled())
    {
        throwInitError("vJoy driver not enabled: Failed Getting vJoy attributes.");
        return;
    }

    // Get the state of the requested device
    VjdStat status = GetVJDStatus(deviceId);
    switch (status) {
        case VJD_STAT_OWN:
            qDebug("vJoy Device %d is already owned by this feeder", deviceId);
            break;
        case VJD_STAT_FREE:
            qDebug("vJoy Device %d is free", deviceId);
            break;
        case VJD_STAT_BUSY:
            throwInitError(QString("vJoy Device %1 is already owned by another feeder\nCannot continue.").arg(deviceId));
            return;
        case VJD_STAT_MISS:
            throwInitError(QString("vJoy Device %1 is not installed or disabled\nCannot continue").arg(deviceId));
            return;
        default:
            throwInitError(QString("vJoy Device %1 general error. Cannot continue.").arg(deviceId));
            return;
    }
    
    if (!checkAxisExists(deviceId, HID_USAGE_X, "X")) {
        return;
    }
    if (!checkAxisExists(deviceId, HID_USAGE_Y, "Y")) {
        return;
    }
    if (!checkAxisExists(deviceId, HID_USAGE_RX, "RX")) {
        return;
    }
    if (!checkAxisExists(deviceId, HID_USAGE_RY, "RY")) {
        return;
    }
    if (!checkAxisExists(deviceId, HID_USAGE_SL0, "U/Slider")) {
        return;
    }
    if (!checkAxisExists(deviceId, HID_USAGE_SL1, "V/Dial")) {
        return;
    }
    
	// Get the number of buttons and POV Hat switchessupported by this vJoy device
    
    if (GetVJDButtonNumber(deviceId) < 11) {
        throwInitError(QString("vJoy Device %1 does not have at least 11 buttons.\nPlease make sure "
                               "the first four vJoy devices all have 11 buttons or more before launching.").arg(deviceId));
        return;
    }
    
    if (GetVJDContPovNumber(deviceId) < 1) {
        throwInitError(QString("vJoy Device %1 does not have at least one continuous POV hat switch.\nPlease make sure "
                               "the first four vJoy devices all have one or more continuous POV hat switches before launching.").arg(deviceId));
        return;
    }
    
    // Acquire the target
    if ((status == VJD_STAT_OWN) || ((status == VJD_STAT_FREE) && (!AcquireVJD(deviceId)))) {
        throwInitError(QString("Failed to acquire vJoy device number %d.").arg(deviceId));
        return;
    } else {
        qDebug() << "Acquired: vJoy device number" << deviceId;
    }
    
    initializedDevices.insert(deviceId);
}

void ControllerRemapper::initialize()
{
    for(UINT deviceId = 1; deviceId <= 4; ++deviceId) {
        initializeDevice(deviceId);
    }
    
    // Get access to XInputGetStateEx so we can query the state of the Guide/Home button:
    if(!XInputGetStateEx) {
        HINSTANCE hXInput = LoadLibrary(XINPUT_DLL);
        XInputGetStateEx = (XInputGetStateEx_t) GetProcAddress(hXInput, (LPCSTR) 100);
        
        if(!XInputGetStateEx) { // Might help with wrappers compatibility
            XInputGetStateEx = (XInputGetStateEx_t) GetProcAddress(hXInput, "XInputGetState");
        }
    } 
}

void ControllerRemapper::deinitialize()
{
    foreach(UINT deviceId, initializedDevices) {
        qDebug() << "Resetting and relinquishing device" << deviceId;
        ResetVJD(deviceId);
        RelinquishVJD(deviceId);
    }
}

void ControllerRemapper::throwInitError(QString msg)
{
    emit initializationError(msg);
    exit(1);
}

void ControllerRemapper::run()
{
    qDebug() << "Starting thread";
    
    initialize();
    
    exec();
    
    qDebug() << "Thread deinitializing...";
    deinitialize();
    qDebug() << "Thread finished";
}
