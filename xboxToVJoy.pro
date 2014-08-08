#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T13:14:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xboxToVJoy
TEMPLATE = app

INCLUDEPATH += $$PWD/vJoySDK/inc

contains(QMAKE_HOST.arch, x86_64) {
    LIBS += -L$$PWD/vJoySDK/lib/amd64
} else {
    LIBS += -L$$PWD/vJoySDK/lib
}

LIBS += -lvJoyInterface

SOURCES += main.cpp \
    controllerwindow.cpp \
    qmainwidget.cpp \
    controllerremapper.cpp

HEADERS  += \
    controllerwindow.h \
    qmainwidget.h \
    controllerremapper.h

RESOURCES += \
    resources.qrc
