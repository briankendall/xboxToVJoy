#ifndef DINPUTCORRELATION_H
#define DINPUTCORRELATION_H

#include <QMap>
#include <windows.h>

#define kNoError 0
#define kErrorNotEnoughVJoyDevices 1
#define kErrorCouldntCorrelate 2

void determineCorrelation(HRESULT &winResult, int &result, QMap<UINT, UINT> &correlationMap, HWND dinputWindow, int vjoyControllerCount);

#endif // DINPUTCORRELATION_H
