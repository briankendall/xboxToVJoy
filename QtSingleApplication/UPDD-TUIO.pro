#-------------------------------------------------
#
# Project created by QtCreator 2011-12-20T09:29:28
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32 {
    TARGET = "UPDD-TUIO"
} else {
    TARGET = "UPDD TUIO"
}
VERSION = 2.0.16

TEMPLATE = app

SOURCES += main.cpp \
    TUIOServer.cpp \
    TBReceiver.cpp \
    oscpack/osc/OscTypes.cpp \
    oscpack/osc/OscReceivedElements.cpp \
    oscpack/osc/OscPrintReceivedElements.cpp \
    oscpack/osc/OscOutboundPacketStream.cpp \
    oscpack/ip/IpEndpointName.cpp \
    qmainwidget.cpp \
    settingsdialog.cpp \
    prefs.cpp \
    changehistorywindow.cpp \
    QtSingleApplication/qtlocalpeer.cpp \
    QtSingleApplication/qtlockedfile_unix.cpp \
    QtSingleApplication/qtlockedfile.cpp \
    QtSingleApplication/qtsingleapplication.cpp \
    QtSingleApplication/qtsinglecoreapplication.cpp

HEADERS += \
    TUIOServer.h \
    TBReceiver.h \
    qmainwidget.h \
    settingsdialog.h \
    prefs.h \
    multiplatformFunctions.h \
    changehistorywindow.h \
    QtSingleApplication/qtlocalpeer.h \
    QtSingleApplication/QtLockedFile \
    QtSingleApplication/qtlockedfile.h \
    QtSingleApplication/QtSingleApplication \
    QtSingleApplication/qtsingleapplication.h \
    QtSingleApplication/qtsinglecoreapplication.h

# oscpack platform specific headers:
win32 {
    SOURCES += oscpack/ip/win32/UdpSocket.cpp \
               oscpack/ip/win32/NetworkingUtils.cpp
} else {
    SOURCES += oscpack/ip/posix/UdpSocket.cpp \
               oscpack/ip/posix/NetworkingUtils.cpp
}

# Mac OS X:
macx {
    equals(UPDD_VERSION, "502") {
        HEADERS  += \
            $$PWD/mac/502/nonwindows.h \
            $$PWD/mac/502/TBApi.h
        LIBS += -L$$PWD/mac/502/
        INCLUDEPATH += $$PWD/mac/502
        DEPENDPATH += $$PWD/mac/502
        CONFIG -= x86_64
        CONFIG += x86
    }
    equals(UPDD_VERSION, "510") {
        HEADERS  += \
            $$PWD/mac/510/nonwindows.h \
            $$PWD/mac/510/TBApi.h
        LIBS += -L$$PWD/mac/510/
        INCLUDEPATH += $$PWD/mac/510
        DEPENDPATH += $$PWD/mac/510
        CONFIG -= x86
        CONFIG += x86_64
    }

    OBJECTIVE_SOURCES += \
                         mac/cocoaInitializer.mm \
                         mac/osxMisc.mm
    SOURCES += mac/osxCursor.cpp \
               mac/loginitem.cpp \
               mac/osxProcesses.cpp
    HEADERS += mac/cocoaInitializer.h \
               mac/osxCursor.h \
               mac/loginitem.h \
               mac/osxMisc.h \
               mac/osxProcesses.h
    LIBS += -ltbapi \
            -lACE \
            -framework Carbon \
            -framework CoreServices \
            -framework Cocoa
    INCLUDEPATH += mac
    CONFIG -= x86_64
    CONFIG += x86
    QMAKE_INFO_PLIST = mac/Info.plist
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
    ICON = $$PWD/mac/appicon.icns
    OTHER_FILES += mac/Info.plist
}

# Linux:
unix:!macx {
    SOURCES += linux/linuxMisc.cpp
    !contains(QMAKE_HOST.arch, x86_64) {
        LIBS += -L/opt/tbupddlx \
                -ltbapi32 \
                -lACE-5.6.2
    } else {
        LIBS += -L/opt/tbupddlx \
                -ltbapi64 \
                -lACE-5.6.2
        QMAKE_LFLAGS += -z muldefs
    }
    INCLUDEPATH += /opt/tbupddlx
}

# Windows (general):
win32 {
    SOURCES += win-common/winMisc.cpp
    HEADERS += win-common/winMisc.h
    !contains(QMAKE_HOST.arch, x86_64) {
        # 32-bit Windows:
        LIBS += -L$$PWD/win32 -lTBApi32 -lUser32
        INCLUDEPATH += $$PWD/win32
        DEPENDPATH += $$PWD/win32

    } else {
        # 64-bit Windows
        LIBS += -L$$PWD/win64 -lTBApi64
        INCLUDEPATH += $$PWD/win64
        DEPENDPATH += $$PWD/win64
    }
    LIBS += -lwinmm -lws2_32 -lpsapi -lshell32
    DEFINES += TBAPIDLL_IMPORTS
    RC_FILE = win-common/winrc.rc
}

INCLUDEPATH += oscpack

RESOURCES += \
    resources.qrc

FORMS += \
    settingsdialog.ui \
    changehistorywindow.ui

DEFINES += OSC_HOST_LITTLE_ENDIAN









