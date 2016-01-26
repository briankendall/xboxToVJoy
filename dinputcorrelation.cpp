#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#define _CRT_SECURE_NO_DEPRECATE
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <commctrl.h>
#include <basetsd.h>

#pragma warning(push)
#pragma warning(disable:6000 28251)
#include <dinput.h>
#pragma warning(pop)

#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>

#include <QDebug>
#include <QList>
#include <QElapsedTimer>
#include "dinputcorrelation.h"
#include "public.h"
#include "vjoyinterface.h"
#include "controllerremapper.h"
#include "qmainwidget.h"

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *userData);

LPDIRECTINPUT8 directInput8Interface = nullptr;
QList<LPDIRECTINPUTDEVICE8> vjoyControllers;

void determineCorrelation(HRESULT &winResult, int &result, QMap<UINT, UINT> &correlationMap, HWND dinputWindow, int vjoyControllerCount)
{
    HRESULT enumerationResult = ERROR_SUCCESS;
    winResult = ERROR_SUCCESS;
    result = kNoError;
    
    directInput8Interface = nullptr;
    vjoyControllers.clear();
    correlationMap.clear();
    
    winResult = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID **)&directInput8Interface, nullptr);
    if (FAILED(winResult)) {
        qCritical() << "DirectInput8Create failed, winResult:" << winResult;
        goto cleanup;
    }
    
    winResult = directInput8Interface->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, (void *)enumerationResult, DIEDFL_ATTACHEDONLY);

    if (FAILED(winResult)) {
        qCritical() << "EnumDevices failed, winResult:" << winResult;
        goto cleanup;
    }
    
    if (FAILED(enumerationResult)) {
        qCritical() << "EnumDevices failed in callback function, enumerationResult:" << enumerationResult;
        winResult = enumerationResult;
        goto cleanup;
    }
    
    if (vjoyControllers.count() < vjoyControllerCount) {
        result = kErrorNotEnoughVJoyDevices;
        goto cleanup;
    }
    
    foreach(LPDIRECTINPUTDEVICE8 controller, vjoyControllers) {
        winResult = controller->SetDataFormat(&c_dfDIJoystick2);
        if (FAILED(winResult)) {
            qCritical() << "SetDataFormat failed, winResult:" << winResult;
            goto cleanup;
        }
        
        winResult = controller->SetCooperativeLevel(dinputWindow, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
        if (FAILED(winResult)) {
            qCritical() << "SetCooperativeLevel failed, winResult:" << winResult;
            goto cleanup;
        }
        
        winResult = controller->Acquire();
        if (FAILED(winResult)) {
            qCritical() << "Acquire failed, winResult:" << winResult;
            goto cleanup;
        }
    }
    
    for(int i = 0; i < vjoyControllerCount; ++i) {
        SetBtn(true, i+1, i+1);
    }
    
    QElapsedTimer timer;
    timer.start();
    while(correlationMap.count() < vjoyControllerCount && timer.elapsed() < 2000) {
        Sleep(20);
        
        for(int dinputIndex = 0; dinputIndex < vjoyControllers.count(); ++dinputIndex) {
            DIJOYSTATE2 state;
            
            if (correlationMap.contains(dinputIndex)) {
                // Already figured out which controller this is, so we can move on...
                continue;
            }
            
            winResult = vjoyControllers[dinputIndex]->Poll();
            if (FAILED(winResult)) {
                qCritical() << "Poll failed, winResult:" << winResult;
                goto cleanup;
            }
            
            winResult = vjoyControllers[dinputIndex]->GetDeviceState(sizeof(DIJOYSTATE2), &state);
            if (FAILED(winResult)) {
                qCritical() << "GetDeviceState failed, winResult:" << winResult;
                goto cleanup;
            }
            
            for(int i = 0; i < vjoyControllerCount; ++i) {
                if (state.rgbButtons[i] & 0x80) {
                    qDebug() << "Matched dinput controller" << dinputIndex << "to vJoy Device #" << i+1;
                    correlationMap[dinputIndex] = i+1;
                    break;
                }
            }
        }
    }
    
    for(int i = 0; i < vjoyControllerCount; ++i) {
        SetBtn(false, i+1, i+1);
    }
    
    if (correlationMap.count() < vjoyControllerCount) {
        result = kErrorCouldntCorrelate;
        goto cleanup;
    }
    
    // Yes, yes, I've heard people hate on goto's before. But the truth is that we have a function
    // with multiple points where it could return, but at each point it could have any number of
    // objects that need to be cleaned up. Frankly this seems like the best method to deal with
    // this, because I'll be damned if I'm going to create a class for handling creation and
    // cleanup of all of this stuff just so I can make use of its constructor / destructor
    // mechanisms. Nor do I want to use exceptions. Nor do I want to use a million nested
    // conditionals.
cleanup:
    foreach(LPDIRECTINPUTDEVICE8 controller, vjoyControllers) {
        controller->Unacquire();
        controller->Release();
    }
    
    if (directInput8Interface) {
        directInput8Interface->Release();
    }
}

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *userData)
{
    HRESULT result;
    HRESULT *enumerationResult = (HRESULT *)userData;

    if (QString::fromWCharArray(pdidInstance->tszProductName) != "vJoy Device") {
        return DIENUM_CONTINUE;
    }
    
    LPDIRECTINPUTDEVICE8 controller;
    
    result = directInput8Interface->CreateDevice(pdidInstance->guidInstance, &controller, nullptr);
    
    if (FAILED(result)) {
        qCritical() << "CreateDevice failed, result:" << result;
        (*enumerationResult) = result;
        return DIENUM_STOP;
    }
    
    vjoyControllers.append(controller);
    
    if (vjoyControllers.count() >= 4) {
        return DIENUM_STOP;
    } else {
        return DIENUM_CONTINUE;
    }
}
