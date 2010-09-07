/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <QDBusMessage>
#include <QDBusConnection>
#include "homeapplication.h"
#include "homescreenservice.h"
#include "homescreenadaptor.h"
#include "windowinfo.h"
#include "xeventlistener.h"

/*!
 * D-Bus names for the home screen service
 */
static const QString MEEGO_CORE_HOME_SCREEN_SERVICE_NAME = "com.meego.core.HomeScreen";
static const QString MEEGO_CORE_HOME_SCREEN_OBJECT_PATH = "/homescreen";

/*!
 * D-Bus names for the notification that's sent when home is ready
 */
static const QString HOME_READY_SIGNAL_PATH = "/com/nokia/duihome";
static const QString HOME_READY_SIGNAL_INTERFACE = "com.nokia.duihome.readyNotifier";
static const QString HOME_READY_SIGNAL_NAME = "ready";

/*!
 * Checks whether an upstart command line parameter was given.
 * This is checked for stopping the application after it's ready,
 * and we only want to do that when run by upstart, not
 * when run by Matti or manually.
 *
 * \param argc number of parameters
 * \param argv parameters
 * \return true if in upstart mode, false otherwise
 */
static bool isUpstartMode(int argc, char *argv[])
{
    if (argc < 2) {
        return false;
    }
    static const char optChar = 'u';
    static const char *optString = "u";
    static struct option optLong[] = {
        { "upstart", 0, NULL, optChar },
        { 0, 0, 0, 0 }
    };
    opterr = 0;
    int c = 0;
    while ((c = getopt_long_only(argc, argv,
                                 optString,
                                 optLong,
                                 0)) != -1) {
        if (c == optChar) {
            return true;
        }
    }
    return false;
}

HomeApplication::HomeApplication(int &argc, char **argv, const QString& appIdentifier) :
    MApplication(argc, argv, appIdentifier),
    homeScreenService(new HomeScreenService),
    xEventListeners(),
    iteratorActiveForEventListenerContainer(false),
    toBeRemovedEventListeners()
{
    // Enable prestart mode
    MApplication::setPrestartMode(M::TerminateOnClose);

    // launch a timer for sending a dbus-signal upstart when home is ready
    // and on screen
    upstartMode = isUpstartMode(argc, argv);
    connect(&startupNotificationTimer, SIGNAL(timeout()),
            this, SLOT(sendStartupNotifications()));
    startupNotificationTimer.setSingleShot(true);
    startupNotificationTimer.setInterval(0);
    startupNotificationTimer.start();

    new HomeScreenAdaptor(homeScreenService);

    QDBusConnection connection = QDBusConnection::sessionBus();

    connection.registerService(MEEGO_CORE_HOME_SCREEN_SERVICE_NAME);
    connection.registerObject(MEEGO_CORE_HOME_SCREEN_OBJECT_PATH, homeScreenService);

    connect(homeScreenService, SIGNAL(focusToLauncherApp(const QString&)), this, SIGNAL(focusToLauncherAppRequested(const QString &)));

    // Initialize the X11 atoms used in the UI components
    WindowInfo::initializeAtoms();
}

HomeApplication::~HomeApplication()
{
    delete homeScreenService;
}

void HomeApplication::addXEventListener(XEventListener *listener)
{
    if (listener != NULL && !xEventListeners.contains(listener)) {
        xEventListeners.append(listener);
    }
}

void HomeApplication::removeXEventListener(XEventListener *listener)
{
    if (iteratorActiveForEventListenerContainer) {
        toBeRemovedEventListeners.append(listener);
    } else {
        xEventListeners.removeOne(listener);
    }
}

void HomeApplication::sendStartupNotifications()
{
    static QDBusConnection systemBus = QDBusConnection::systemBus();
    QDBusMessage homeReadySignal =
        QDBusMessage::createSignal(HOME_READY_SIGNAL_PATH,
                                   HOME_READY_SIGNAL_INTERFACE,
                                   HOME_READY_SIGNAL_NAME);
    systemBus.send(homeReadySignal);
    if (upstartMode) {
        static pid_t selfPid = getpid();
        kill(selfPid, SIGSTOP);
    }
}

bool HomeApplication::x11EventFilter(XEvent *event)
{
    bool eventHandled = false;
    iteratorActiveForEventListenerContainer = true;
    foreach (XEventListener* listener, xEventListeners) {
        if (!toBeRemovedEventListeners.contains(listener)) {
            if (listener->handleXEvent(*event)) {
                eventHandled = true;
            }
        }
    }
    iteratorActiveForEventListenerContainer = false;

    // Remove now any event listeners that got removed while going through the event listeners
    foreach (XEventListener* listener, toBeRemovedEventListeners) {
        xEventListeners.removeOne(listener);
    }
    toBeRemovedEventListeners.clear();

    if (!eventHandled) {
        eventHandled = MApplication::x11EventFilter(event);
    }

    return eventHandled;
}
