#include <windows.h>
#include <XInput.h>
#include <QDebug>
#include "controllerremapper.h"
#include "public.h"
#include "vjoyinterface.h"
#include "dinputcorrelation.h"
#include <comdef.h>
#include <QSignalMapper>

typedef DWORD (WINAPI* XInputGetStateEx_t)(DWORD dwUserIndex, XINPUT_STATE *pState);
XInputGetStateEx_t XInputGetStateEx = NULL;

const int buttonFlags[kButtonCount] = {XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y,
                                       XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER, XINPUT_GAMEPAD_BACK,
                                       XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB,
                                       XINPUT_GAMEPAD_GUIDE};
const int directionFlags[4] = {XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_UP, XINPUT_GAMEPAD_DPAD_RIGHT,
                               XINPUT_GAMEPAD_DPAD_DOWN};

#define XBOX_REMAP_LT HID_USAGE_Z
#define XBOX_REMAP_RT HID_USAGE_RZ
#define XBOX_REMAP_LX HID_USAGE_X
#define XBOX_REMAP_LY HID_USAGE_Y
#define XBOX_REMAP_RX HID_USAGE_RX
#define XBOX_REMAP_RY HID_USAGE_RY

#define XBOX_REMAP_LT_NAME "Z"
#define XBOX_REMAP_RT_NAME "RZ"
#define XBOX_REMAP_LX_NAME "X"
#define XBOX_REMAP_LY_NAME "Y"
#define XBOX_REMAP_RX_NAME "RX"
#define XBOX_REMAP_RY_NAME "RY"

long clamp(long val, long min, long max)
{
	if (val < min) {
		val = min;
	}
	if (val > max) {
		val = max;
	}
	return val;
}

long xboxAxisToVJoy(SHORT val, bool reverse)
{
	long n = long(val) / 2 + 16384;

	if (reverse) {
		n = 32767 - n;
	}

	return clamp(n + 1, 1, 32768);
}

long xboxTriggerToVJoy(BYTE val)
{
    return long((double(val) / 255.0) * 16383) + 16384;
}

bool directionPressed(const QVector<bool> &buttons, bool left, bool up, bool right, bool down)
{
	return buttons[0] == left && buttons[1] == up && buttons[2] == right && buttons[3] == down;
}

void resetVJoyDevice(int deviceId)
{
    ResetVJD(deviceId);
    SetContPov(-1, deviceId, 1);
    SetAxis(xboxTriggerToVJoy(0), deviceId, XBOX_REMAP_LT);
    SetAxis(xboxTriggerToVJoy(0), deviceId, XBOX_REMAP_RT);
    SetAxis(xboxAxisToVJoy(0, false), deviceId, XBOX_REMAP_LX);
    SetAxis(xboxAxisToVJoy(0, true), deviceId, XBOX_REMAP_LY);
    SetAxis(xboxAxisToVJoy(0, false), deviceId, XBOX_REMAP_RX);
    SetAxis(xboxAxisToVJoy(0, true), deviceId, XBOX_REMAP_RY);
    
    for (int i = 0; i < kButtonCount; ++i) {
        SetBtn(false, deviceId, i+1);
    }
}

#define kInteractionWaitTime 100

void Controller::initialize()
{
    buttonsDown.fill(false, kButtonCount);
    directionDown.fill(false, 4);
    lastLeftTrigger = kAxisUnset;
    lastRightTrigger = kAxisUnset;
    lastLX = kAxisUnset;
    lastLY = kAxisUnset;
    lastRX = kAxisUnset;
    lastRY = kAxisUnset;
	lastPacketNumber = 0;
	handledFirstPacket = false;
    lastConnected = false;
}

void Controller::doControllerMap(UINT vjoyDeviceId)
{
    XINPUT_STATE controllerState;
    ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
    DWORD result = XInputGetStateEx(deviceIndex, &controllerState);
    bool connected = (result == ERROR_SUCCESS);

    if (!connected) {
        if (lastConnected) {
            qDebug() << "reseting device:" << vjoyDeviceId;
            reset(vjoyDeviceId);
        }
        lastConnected = false;
        
        return;
    } else {
        lastConnected = true;
    }
    
    if (lastPacketNumber == controllerState.dwPacketNumber && handledFirstPacket) {
        // Xbox controller state hasn't changed, no need to do anything
        return;
    }
    
    const XINPUT_GAMEPAD &state = controllerState.Gamepad;
    handledFirstPacket = true;
    lastPacketNumber = controllerState.dwPacketNumber;

    for (int i = 0; i < kButtonCount; ++i) {
        bool playerButtonDown = (state.wButtons & buttonFlags[i]) != 0;

        if (playerButtonDown && !buttonsDown[i]) {
            SetBtn(true, vjoyDeviceId, i+1);

        } else if (!playerButtonDown && buttonsDown[i]) {
            SetBtn(false, vjoyDeviceId, i+1);
        }

        buttonsDown[i] = playerButtonDown;
    }

    bool directionChanged = false;
    for (int i = 0; i < 4; ++i) {
        bool playerButtonDown = (state.wButtons & directionFlags[i]) != 0;
        
        if (directionDown[i] != playerButtonDown) {
            directionChanged = true;
            directionDown[i] = playerButtonDown;
        }
    }

    if (directionChanged) {
        if (directionPressed(directionDown, true, false, false, false)) {
            SetContPov(27000, vjoyDeviceId, 1);
        } else if (directionPressed(directionDown, true, true, false, false)) {
            SetContPov(31500, vjoyDeviceId, 1);
        } else if (directionPressed(directionDown, false, true, false, false)) {
            SetContPov(0, vjoyDeviceId, 1);
        } else if (directionPressed(directionDown, false, true, true, false)) {
            SetContPov(4500, vjoyDeviceId, 1);
        } else if (directionPressed(directionDown, false, false, true, false)) {
            SetContPov(9000, vjoyDeviceId, 1);
        } else if (directionPressed(directionDown, false, false, true, true)) {
            SetContPov(13500, vjoyDeviceId, 1);
        } else if (directionPressed(directionDown, false, false, false, true)) {
            SetContPov(18000, vjoyDeviceId, 1);
        } else if (directionPressed(directionDown, true, false, false, true)) {
            SetContPov(22500, vjoyDeviceId, 1);
        } else {
            SetContPov(-1, vjoyDeviceId, 1);
        }
    }

    if (state.bLeftTrigger != lastLeftTrigger) {
        SetAxis(xboxTriggerToVJoy(state.bLeftTrigger), vjoyDeviceId, XBOX_REMAP_LT);
        lastLeftTrigger = state.bLeftTrigger;
    }

    if (state.bRightTrigger != lastRightTrigger) {
        SetAxis(xboxTriggerToVJoy(state.bRightTrigger), vjoyDeviceId, XBOX_REMAP_RT);
        lastRightTrigger = state.bRightTrigger;
    }

    if (state.sThumbLX != lastLX) {
        SetAxis(xboxAxisToVJoy(state.sThumbLX, false), vjoyDeviceId, XBOX_REMAP_LX);
        lastLX = state.sThumbLX;
    }

    if (state.sThumbLY != lastLY) {
        SetAxis(xboxAxisToVJoy(state.sThumbLY, true), vjoyDeviceId, XBOX_REMAP_LY);
        lastLY = state.sThumbLY;
    }

    if (state.sThumbRX != lastRX) {
        SetAxis(xboxAxisToVJoy(state.sThumbRX, false), vjoyDeviceId, XBOX_REMAP_RX);
        lastRX = state.sThumbRX;
    }
    
    if (state.sThumbRY != lastRY) {
        SetAxis(xboxAxisToVJoy(state.sThumbRY, true), vjoyDeviceId, XBOX_REMAP_RY);
        lastRY = state.sThumbRY;
    }
}

void Controller::reset(UINT vjoyDeviceId)
{
    initialize();
    resetVJoyDevice(vjoyDeviceId);
}

ControllerRemapper::ControllerRemapper(HWND win, bool inEnabled, QObject *parent) :
    QThread(parent), dinputWindow(win), errorOccurred(false), enabled(inEnabled)
{
}

void ControllerRemapper::throwInitError(QString msg)
{
    emit initializationError(msg);
    errorOccurred = true;
    exit(1);
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

bool ControllerRemapper::initializeVJoy()
{
    if (!vJoyEnabled()) {
        throwInitError("Failed Getting vJoy attributes. vJoy is not enabled.");
        return false;
    }

    // Test interface DLL matches vJoy driver
    // Compare versions
    WORD VerDll, VerDrv;

    if (!DriverMatch(&VerDll, &VerDrv)) {
        throwInitError(QString("vJoy Driver (version %1) does not match vJoyInterface DLL (version %2)")
                       .arg(uint(VerDrv), 4, 16, QChar('0')).arg(uint(VerDll), 4, 16, QChar('0')));
        return false;
    }
    
    return true;
}

void ControllerRemapper::initializeDevice(UINT deviceId)
{
    if (!initializeVJoy()) {
        return;
    }

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
    
    if (!checkAxisExists(deviceId, XBOX_REMAP_LX, XBOX_REMAP_LX_NAME)) {
        return;
    }
    if (!checkAxisExists(deviceId, XBOX_REMAP_LY, XBOX_REMAP_LY_NAME)) {
        return;
    }
    if (!checkAxisExists(deviceId, XBOX_REMAP_RX, XBOX_REMAP_RX_NAME)) {
        return;
    }
    if (!checkAxisExists(deviceId, XBOX_REMAP_RY, XBOX_REMAP_RY_NAME)) {
        return;
    }
    if (!checkAxisExists(deviceId, XBOX_REMAP_LT, XBOX_REMAP_LT_NAME)) {
        return;
    }
    if (!checkAxisExists(deviceId, XBOX_REMAP_RT, XBOX_REMAP_RT_NAME)) {
        return;
    }
    
	// Get the number of buttons and POV Hat switchessupported by this vJoy device
    
    if (GetVJDButtonNumber(deviceId) < kButtonCount) {
        throwInitError(QString("vJoy Device %1 does not have at least %2 buttons.\nPlease make sure "
                               "the first four vJoy devices all have %3 buttons or more before "
                               "launching.").arg(deviceId).arg(kButtonCount).arg(kButtonCount));
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

HINSTANCE ControllerRemapper::getXInputDLLHandle()
{
    HINSTANCE result = LoadLibrary(XINPUT_DLL);

    if (result) {
        return result;
    }

    result = LoadLibrary(L"xinput1_4.dll");

    if (result) {
        return result;
    }

    result = LoadLibrary(L"xinput9_1_0.dll");

    if (result) {
        return result;
    }

    result = LoadLibrary(L"xinput1_3");

    if (result) {
        return result;
    }

    return NULL;
}

void ControllerRemapper::initialize()
{
    controllerCount = 0;
    for(UINT index = 0; index < 4; ++index) {
        if (GetVJDStatus(index+1) == VJD_STAT_MISS) {
            break;
        } else {
            controllerCount++;
        }
    }
    
    if (controllerCount == 0) {
        throwInitError("There doesn't seem to be any vJoy controllers.");
        return;
    }
    
    for(UINT index = 0; index < controllerCount; ++index) {
        initializeDevice(index+1);
        
        if (errorOccurred) {
            return;
        }
        
        controllers[index].deviceIndex = index;
        controllers[index].initialize();
    }
    
    // Get access to XInputGetStateEx so we can query the state of the Guide/Home button:
    if(!XInputGetStateEx) {
        HINSTANCE hXInput = getXInputDLLHandle();

        if (!hXInput) {
            throwInitError("Could not get a handle to the XInput DLL.");
            return;
        }

        XInputGetStateEx = (XInputGetStateEx_t) GetProcAddress(hXInput, (LPCSTR) 100);
        
        if(!XInputGetStateEx) { // Might help with wrappers compatibility
            XInputGetStateEx = (XInputGetStateEx_t) GetProcAddress(hXInput, "XInputGetState");
        }
    }
    
    HRESULT winResult;
    int result;
    determineCorrelation(winResult, result, xboxToVJoyMap, dinputWindow, controllerCount);
    
    if (FAILED(winResult)) {
        _com_error err(winResult);
        QString errorString = (LPSTR)err.ErrorMessage();
        throwInitError(QString("vJoy controller correlation failed. Windows error: ") + errorString);
        return;
    }
    
    if (result == kErrorNotEnoughVJoyDevices) {
        throwInitError("There were fewer vJoy devices as reported by DirectInput than the number of vJoy Devices "
                       "as reported by vJoy itself.");
        return;
    }
    
    if (result == kErrorCouldntCorrelate) {
        throwInitError("One or more vJoy Devices could not be correlated with their DirectInput counterpart.");
        return;
    }
    
    for(UINT index = 0; index < controllerCount; ++index) {
        controllers[index].reset(xboxToVJoyMap[index]);
    }
}

void ControllerRemapper::deinitialize()
{
    foreach(UINT deviceId, initializedDevices) {
        qDebug() << "Resetting and relinquishing device" << deviceId;
        resetVJoyDevice(deviceId);
        RelinquishVJD(deviceId);
    }
}

void ControllerRemapper::poll()
{
    for(UINT index = 0; index < controllerCount; ++index) {
        controllers[index].doControllerMap(xboxToVJoyMap[index]); 
    }
}

void ControllerRemapper::run()
{
    initialize();
    
    setPriority(QThread::HighestPriority);
    
    pollTimer = new QTimer();
    pollTimer->moveToThread(this);
    connect(pollTimer, SIGNAL(timeout()), this, SLOT(poll()), Qt::DirectConnection);
    pollTimer->setTimerType(Qt::PreciseTimer);
    pollTimer->setInterval(4);
    
    if (enabled) {
        pollTimer->start();
    }
    
    exec();
    
    qDebug() << "Remap thread deinitializing...";
    
    pollTimer->stop();
    delete pollTimer;
    deinitialize();
    
    qDebug() << "Remap thread finished";
}

// Note: the following four functions are only meant to be used from the controller window,
// especially because they will block

void ControllerRemapper::pressButton(UINT controller, UINT xboxButton)
{
    UINT vjoyDeviceId = xboxToVJoyMap[controller];
    UCHAR button = 0;
    
    for(UCHAR i = 0; i < kButtonCount; ++i) {
        if (buttonFlags[i] == xboxButton) {
            button = i+1;
            break;
        }
    }
    
    if (button == 0) {
        qDebug() << "Error: tried to press invalid button";
        return;
    }
    
    SetBtn(true, vjoyDeviceId, button);
    Sleep(kInteractionWaitTime);
    SetBtn(false, vjoyDeviceId, button);
}

void ControllerRemapper::moveJoystick(UINT controller, bool right, double xVal, double yVal)
{
    UINT vjoyDeviceId = xboxToVJoyMap[controller];
    UINT xAxis, yAxis;
    
    if (right) {
        xAxis = XBOX_REMAP_RX;
        yAxis = XBOX_REMAP_RY;
    } else {
        xAxis = XBOX_REMAP_LX;
        yAxis = XBOX_REMAP_LY;
    }
    
    int steps = 20;
    int moveTimeMS = 100;
    
    // We're gradually moving the joystick's position in order to simulate it actually
    // being moved. This was necessary to get the controller window working with ePSXe.
    for(int i = 0; i < steps; ++i) {
        double u = double(i) / double(steps -1 );
        SetAxis(LONG((xVal*u + 1.0)*16383.5) + 1, vjoyDeviceId, xAxis);
        SetAxis(LONG((yVal*u + 1.0)*16383.5) + 1, vjoyDeviceId, yAxis);
        Sleep(moveTimeMS/steps);
    }
    
    SetAxis(LONG((xVal + 1.0)*16383.5) + 1, vjoyDeviceId, xAxis);
    SetAxis(LONG((yVal + 1.0)*16383.5) + 1, vjoyDeviceId, yAxis);
    
    Sleep(kInteractionWaitTime);
    
    for(int i = 0; i < steps; ++i) {
        double u = 1.0 - (double(i) / double(steps -1 ));
        SetAxis(LONG((xVal*u + 1.0)*16383.5) + 1, vjoyDeviceId, xAxis);
        SetAxis(LONG((yVal*u + 1.0)*16383.5) + 1, vjoyDeviceId, yAxis);
        Sleep(moveTimeMS/steps);
    }
    
    SetAxis(0x4000, vjoyDeviceId, xAxis);
    SetAxis(0x4000, vjoyDeviceId, yAxis);
}

void ControllerRemapper::pressTrigger(UINT controller, bool right, double val)
{
    UINT vjoyDeviceId = xboxToVJoyMap[controller];
    UINT axis;
    
    if (right) {
        axis = XBOX_REMAP_RT;
    } else {
        axis = XBOX_REMAP_LT;
    }
    
    SetAxis(LONG(val*32767.0) + 1, vjoyDeviceId, axis);
    Sleep(kInteractionWaitTime);
    SetAxis(16384, vjoyDeviceId, axis);
}

void ControllerRemapper::moveDPad(UINT controller, int direction)
{    
    UINT vjoyDeviceId = xboxToVJoyMap[controller];
    
    SetContPov(direction * 4500, vjoyDeviceId, 1);
    Sleep(kInteractionWaitTime);
    SetContPov(-1, vjoyDeviceId, 1);
}

bool ControllerRemapper::isEnabled()
{
    return enabled;
}

void ControllerRemapper::setEnabled(bool inEnabled)
{
    if (QThread::currentThread() != this) {
        QMetaObject::invokeMethod(this, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, inEnabled));
        return;
    }
    
    if (!enabled && inEnabled) {
        qDebug() << "Enabling remapping...";
        pollTimer->start();
        XInputEnable(true);
    } else if (enabled && !inEnabled) {
        qDebug() << "Disabling remapping...";
        pollTimer->stop();
        XInputEnable(false);
        
        foreach(UINT deviceId, initializedDevices) {
            resetVJoyDevice(deviceId);
        }
    }
    
    enabled = inEnabled;
}
