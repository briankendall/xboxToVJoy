#ifndef DINPUTCORRELATION_H
#define DINPUTCORRELATION_H

#include <QWidget>
#include <windows.h>

#define kNoError 0
#define kErrorNotEnoughVJoyDevices 1
#define kErrorCouldntCorrelate 2

void doStuff(HRESULT &winResult, int &result, HWND dinputWindow, int vjoyControllerCount);

#endif // DINPUTCORRELATION_H
