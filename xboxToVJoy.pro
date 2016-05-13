#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T13:14:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xboxToVJoy
TEMPLATE = app

INCLUDEPATH += $$PWD/vJoySDK/inc

contains(QMAKE_HOST.arch, x86_64) {
    LIBS += -L$$PWD/vJoySDK/lib/amd64
} else {
    LIBS += -L$$PWD/vJoySDK/lib
}

LIBS += -lvJoyInterface dinput8.lib dxguid.lib comctl32.lib Xinput.lib

SOURCES += main.cpp \
    qmainwidget.cpp \
    controllerremapper.cpp \
    controllerwindow.cpp \
    dinputcorrelation.cpp \
    QtSingleApplication/qtlocalpeer.cpp \
    QtSingleApplication/qtlockedfile_unix.cpp \
    QtSingleApplication/qtlockedfile.cpp \
    QtSingleApplication/qtsingleapplication.cpp \
    QtSingleApplication/qtsinglecoreapplication.cpp

HEADERS  += \
    qmainwidget.h \
    controllerremapper.h \
    controllerwindow.h \
    dinputcorrelation.h \
    QtSingleApplication/qtlocalpeer.h \
    QtSingleApplication/QtLockedFile \
    QtSingleApplication/qtlockedfile.h \
    QtSingleApplication/QtSingleApplication \
    QtSingleApplication/qtsingleapplication.h \
    QtSingleApplication/qtsinglecoreapplication.h

RESOURCES += \
    resources.qrc

FORMS += \
    controllerwindow.ui
