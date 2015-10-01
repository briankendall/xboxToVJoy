//
// UPDD TUIO
//
// main.cpp
// Copyright 2013 Touch-Base Ltd. All Rights Reserved
//

#include <iostream>

#include <QtGui>
#include <QMessageBox>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>
#include <QStringList>
#include <QThread>

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "qmainwidget.h"
#include "TBReceiver.h"
#include "prefs.h"
#include "QtSingleApplication/QtSingleApplication"

#ifdef Q_OS_MAC
#include "cocoaInitializer.h"
#include "osxCursor.h"
#include "osxMisc.h"
#endif

#ifndef Q_OS_WIN
#include "nonwindows.h"
#endif
#include "tbapi.h"

int exitcode=0;

void signalHandler(int signum)
{
#ifdef Q_OS_WIN
    // strsignal is not defined in Windows
    qDebug() << "Received signal " << signum << "... exiting...";
#else
    qDebug() << "Received " << strsignal(signum) << "... exiting...";
#endif
    exitcode=signum;
    qApp->quit();
}

// Convenience function for comparing command line arguments to two values
bool argEqualsAny(char *arg, const char *value1, const char *value2)
{
    return (strcmp(arg, value1) == 0 || strcmp(arg, value2) == 0);
}

// Parses a single arg for a boolean value. If arg is not "on", "off", 1, or 0, will
// set showUsage to true and return false
bool boolValueFromArg(char *carg, bool &showUsage)
{
	QString arg(carg);
	if (arg.compare(QString("on"), Qt::CaseInsensitive) == 0) {
		return true;
    } else if (arg.compare(QString("off"), Qt::CaseInsensitive) == 0) {
		return false;
	} else {
        bool ok;
        int i = arg.toInt(&ok);
        if (ok && (i == 0 || i == 1)) {
			return i;
        } else {
            showUsage = true;
        }
    }
	return false;
}

// Parses command line arguments and sets tuioHost and tuioPort if user has
// used command line arguments to define them.
// Returns true if the program should continue executing, false if it should quit.
// TODO: This is not a particularly good method for parsing command line arguments.
//    Replace with a better one soon, perhaps the free Qt library that's available.
bool parseArgs(int argc, char **argv)
{
    bool showUsage = false;

	for(int i = 1; i < argc; ++i) {
		// Check if this parameter is an argument:
		if (argv[i][0] != '-') {
			showUsage = true;
			break;
		}

		// Stop and show usage text if argument is for help:
		if (argEqualsAny(argv[i], "-h", "--help")) {
			showUsage = true;
			break;
		}

		// Make sure this isn't the 'psn' argument that gets passed by default in Mac OS X. If so, skip it
		if (strlen(argv[i]) > 4 && strncmp(argv[i], "-psn", 4) == 0) {
			continue;
		}
		
		// Check if argument is for no-icon:
		if (argEqualsAny(argv[i], "--no-icon", "-i")) {
			prefOverrideSetting(kPrefShowIcon, QVariant(false));
			continue;
		}
		
		// Check if argument is for force-icon:
		if (argEqualsAny(argv[i], "--force-icon", "-I")) {
			prefOverrideSetting(kPrefShowIcon, QVariant(true));
			continue;
		}

		// Stop if there is no argument following the current one:
		if ((i+1) >= argc) {
			showUsage = true;
			break;
		}

		// Check if argument is for host address:
		if (argEqualsAny(argv[i], "--bind-address", "-b")) {
			// Prevent buffer overrun:
			prefOverrideSetting(kPrefTuioHost, QVariant(QString(argv[i+1])));
			// Skip next argument (since we just processed it) and continue:
			i++;
			continue;
		}

		// Check if argument is for port:
		if (argEqualsAny(argv[i], "--port", "-p")) {
			prefOverrideSetting(kPrefTuioPort, QVariant(atoi(argv[i+1])));
			// Skip next argument (since we just processed it) and continue:
			i++;
			continue;
		}

		// Check if argument is for mouse emulation:
		if (argEqualsAny(argv[i], "--mouse-emulation", "-m")) {
			bool arg = boolValueFromArg(argv[i+1], showUsage);
			if (showUsage)
				break;
			prefOverrideSetting(kPrefMouseEmulation, arg);
			// Skip next argument (since we just processed it) and continue:
			i++;
			continue;
		}
		
		// Check if argument is for hiding the cursor:
		if (argEqualsAny(argv[i], "--hide-cursor", "-c")) {
			bool arg = boolValueFromArg(argv[i+1], showUsage);
			if (showUsage)
				break;
			prefOverrideSetting(kPrefHideCursor, arg);
			// Skip next argument (since we just processed it) and continue:
			i++;
			continue;
		}

		// Check if argument is for processes with no mouse emulation:
		if (argEqualsAny(argv[i], "--no-mouse-in-apps", "-n")) {
			int j = i+1;
			QStringList apps;
			while(j < argc && argv[j][0] != '-') {
				//qDebug() << "no mouse in: " << argv[j];
				apps.append(QString(argv[j]));
				++j;
			}
			prefOverrideSetting(kPrefAppsWithoutMouseEmulation, apps);
			i = j-1;
			continue;
		}
		
		// Check if argument is for setting data rate:
		if (argEqualsAny(argv[i], "--use-updd-rate", "-r")) {
			bool arg = boolValueFromArg(argv[i+1], showUsage);
			if (showUsage)
				break;
			prefOverrideSetting(kPrefSendAtUPDDRate, arg);
			// Skip next argument (since we just processed it) and continue:
			i++;
			continue;
		}
		
		// If we get here, it's an invalud argument:
		showUsage = true;
		break;
	}
    if (showUsage) {
        QString message;
        message.append("Usage:  UPDD TUIO [OPTIONS]\n");
        message.append("Options:\n");
        message.append(" -b, --bind-address      Specify host / bind address of TUIO server\n");
        message.append(" -p, --port              Specify port of TUIO server\n");
		message.append(" -i, --no-icon           Runs application purely in the background with no menu\n");
        message.append("                         bar / system tray icon.\n");
		message.append(" -I, --force-icon        Forces there to be a menu bar / system tray icon even if\n");
        message.append("                         UPDD TUIO is configured to hide it.\n");
        message.append(" -m, --mouse-emulation   Specify 1 or 0 to set whether mouse emulation will\n");
        message.append("                         be enabled at startup.\n");
        message.append(" -n, --no-mouse-in-apps  Specify one or more process names.  Whenever a process\n");
        message.append("                         matching one of the names is frontmost mouse emulation\n");
        message.append("                         will be automatically disabled.\n");
#ifdef Q_OS_MAC
		message.append(" -c, --hide-cursor       Specify 1 or 0 to set whether the mouse cursor will be\n");
        message.append("                         be hidden during touches.\n");
#endif		
        message.append(" -r, --use-updd-rate	 Specify 1 or 0 to set whether TUIO packets will be sent");
        message.append("                         out at the same rate as the UPDD data rate.");
        message.append(" -h, --help              Displays this message\n");
		
#ifdef Q_OS_WIN
        QMessageBox msgBox;
		 // Ensure the text is in a fixed-width font:
		message.prepend("<pre>");
		message.append("</pre>");
		msgBox.setText(message);
		msgBox.exec();
#else
		qDebug() << message;
#endif
        return false;
    } else
        return true;
}

int main(int argc, char **argv)
{
#ifdef Q_OS_MAC
	CocoaInitializer initializer;
#endif
	QtSingleApplication app("com.touch-base.upddtuio", argc, argv);
	
	if (app.isRunning()) {
		app.sendMessage("instance started", 500);
		qDebug() << "Already another UPDD TUIO instance running.\nWill open the Settings window in the original instance and exit.";
        return 0;
	}
	
	app.setQuitOnLastWindowClosed(false);
	
	if (!parseArgs(argc, argv))
        return 0;

#ifdef SIGHUP
    signal(SIGHUP, signalHandler);
#endif
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
#ifdef SIGQUIT
    signal(SIGQUIT, signalHandler);
#endif

    qDebug() << "Starting UPDD TUIO... built" << __TIME__ << __DATE__;

	qDebug() << "...connecting to the UPDD";
	if (!TBInitializeUPDD()) {
		qDebug() << "Failed to connect to the UPDD! exiting...";
		return 0;
	}
#ifdef Q_OS_MAC
	InitializeCursorHiding();
	SetupUserSwitchNotifications();
#endif
	TBLoadSettings();
    TBInitializeTUIOServer();

    qDebug() << "Running...";

	QMainWidget::initializeMainWidget();
	app.connect(&app, SIGNAL(messageReceived(QString)), QMainWidget::get(), SLOT(appMessageReceived(QString)));

    app.exec();
	
	QMainWidget::get()->stopThreads();
	
    qDebug() << "Exiting...";
#ifdef Q_OS_MAC
	DeinitializeCursorHiding();
#endif
    TBDeinitializeUPDD(true);
	TBDeinitializeTUIO();

    return exitcode;
}
