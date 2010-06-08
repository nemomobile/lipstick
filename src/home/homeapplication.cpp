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
#include "homescreenservice.h"
#include "homescreenadaptor.h"

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <QDBusMessage>
#include <QDBusConnection>
#include "homeapplication.h"
#include "mainwindow.h"
#include "x11wrapper.h"

static const QString MEEGO_CORE_HOME_SCREEN_SERVICE_NAME="com.meego.core.HomeScreen";
static const QString MEEGO_CORE_HOME_SCREEN_OBJECT_PATH ="/homescreen";

/*!
 * D-Bus names for the notification that's sent when home is ready
 */
static const QString HOME_READY_SIGNAL_PATH = "/com/nokia/duihome";
static const QString HOME_READY_SIGNAL_INTERFACE =
    "com.nokia.duihome.readyNotifier";
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

HomeApplication::HomeApplication(int &argc, char **argv) :
    MApplication(argc, argv),
    homeScreenService(new HomeScreenService)
{
    // Enable prestart mode
    MApplication::setPrestartMode(M::TerminateOnClose);

    // Get X11 Atoms for different window types
    Display *dpy = QX11Info::display();

    windowTypeAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    clientListAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLIENT_LIST", False);
    stackedClientListAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLIENT_LIST_STACKING", False);
    closeWindowAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLOSE_WINDOW", False);
    windowStateAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_STATE", False);
    netWindowNameAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_NAME", False);
    windowNameAtom = X11Wrapper::XInternAtom(dpy, "WM_NAME", False);

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
}

HomeApplication::~HomeApplication()
{
    delete homeScreenService;
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

void HomeApplication::launchContentSearchService()
{
    /*
    if (contentSearchIf.isValid()) {
        contentSearchIf.launch("");
    }
    */
}

bool HomeApplication::x11EventFilter(XEvent *event)
{
    if (event->type == PropertyNotify && event->xproperty.window == DefaultRootWindow(QX11Info::display())) {
        if (event->xproperty.atom == stackedClientListAtom) {
            updateWindowMapping();
            return true;
        }
    } else if (event->type == VisibilityNotify) {
        if (event->xvisibility.state == VisibilityFullyObscured) {
            // A window was obscured: was it a homescreen window?
            bool homescreenWindowVisibilityChanged = false;
            Q_FOREACH(MWindow * win, MApplication::windows()) {
                if (event->xvisibility.window == win->winId()) {
                    homescreenWindowVisibilityChanged = true;
                    break;
                }
            }

            if (!homescreenWindowVisibilityChanged) {
                // It was some other window, so let interested parties (the SwitcherButtons) know about it
                if (event->xvisibility.send_event) {
                    emit windowVisibilityChanged(event->xvisibility.window);
                }
                return true;
            }
        }
    } else if (event->type == ClientMessage && event->xclient.message_type == closeWindowAtom) {
        // A _NET_CLOSE_WINDOW message was caught so a window is being closed; add it to windows being closed list and update the window list
        if (!windowsBeingClosed.contains(event->xclient.window)) {
            windowsBeingClosed.insert(event->xclient.window);
        }
        updateWindowMapping();
        return true;
    } else if (event->type == PropertyNotify &&
               (event->xproperty.atom == windowTypeAtom || event->xproperty.atom == windowStateAtom)) {

        updateWindowProperties(event->xproperty.window);
    
        return true;
    } else if (event->type == PropertyNotify &&
               (event->xproperty.atom == windowNameAtom || event->xproperty.atom == netWindowNameAtom)) {
        updateWindowTitle(event->xproperty.window);
        return true;
    }

    return MApplication::x11EventFilter(event);
}

void HomeApplication::updateWindowTitle(Window window)
{
    bool updated = windowMap[window].updateWindowTitle();
    if (updated) {
        emit windowTitleChanged(window, windowMap[window].title());
    }
}

void HomeApplication::updateWindowProperties(Window window)
{
    if (windowMap.contains(window)) {
        WindowInfo windowInfo = windowMap[window];
        bool wasApplication = isApplicationWindow(windowInfo);
        windowInfo.updateWindowProperties();
        bool isApplication = isApplicationWindow(windowInfo);
        if (wasApplication != isApplication) {
            // If the window has changed from an app window to some thing else or wise versa
            if (isApplication) {
                applicationWindows.append(windowInfo);
            } else {
                applicationWindows.removeOne(windowInfo);
            }
            emit windowListUpdated(applicationWindows);
        }
    }
}

void HomeApplication::updateWindowMapping()
{
    // Get a list of all windows
    Display *dpy = QX11Info::display();
    XWindowAttributes wAttributes;
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;
    Status result = X11Wrapper::XGetWindowProperty(dpy, DefaultRootWindow(dpy), stackedClientListAtom,
                                                0, 0x7fffffff, False, XA_WINDOW,
                                                &actualType, &actualFormat, &numWindowItems, &bytesLeft, &windowData);

    if (result == Success && windowData != None) {
        // We need to keep the stacking order of the windows -> hence the list
        QList<Window> newWindowList;
        Window *wins = (Window *)windowData;
        for (unsigned int i = 0; i < numWindowItems; i++) {
            result = X11Wrapper::XGetWindowAttributes(dpy, wins[i], &wAttributes);
            // The windows that are bigger than 0x0, are Input/Output windows and are not unmapped are interesting
            if (result != 0 && 
                wAttributes.width > 0 && wAttributes.height > 0 && 
                wAttributes.c_class == InputOutput && 
                wAttributes.map_state != IsUnmapped) {
                newWindowList.append(wins[i]);
            }
        }
        QSet<Window> newWindowSet = newWindowList.toSet() - windowsBeingClosed;

        QSet<Window> oldWindowSet = windowMap.keys().toSet();

        QSet<Window> closedWindowSet = oldWindowSet - newWindowSet;
        windowsBeingClosed -= closedWindowSet;

        QSet<Window> openedWindowSet = newWindowSet - oldWindowSet;

        bool added = createWindowInfos(openedWindowSet);
        bool removed = removeWindowInfos(closedWindowSet);
        if (added || removed) {
            emit windowListUpdated(applicationWindows);
        }
        windowsBeingClosed -= closedWindowSet;

        QList<WindowInfo> stackingWindowList;
        foreach (Window w, newWindowList) {
            if (!windowsBeingClosed.contains(w)) {
                stackingWindowList.append(windowMap.value(w));
            }
        }
        emit windowStackingOrderChanged(stackingWindowList);
    }
}

bool HomeApplication::createWindowInfos(QSet<Window> openedWindowSet)
{
    Display *dpy = QX11Info::display();
    bool appWindowsAppeared = false;
    foreach(Window w, openedWindowSet) {
        WindowInfo newWindow(w);
        windowMap.insert(w, newWindow);
        if (MainWindow::instance()->winId() != w) {
            X11Wrapper::XSelectInput(dpy, w, VisibilityChangeMask | PropertyChangeMask);
        }

        if (isApplicationWindow(newWindow)) {
            applicationWindows.append(newWindow);
            appWindowsAppeared = true;
        }
    }
    return appWindowsAppeared;
}

bool HomeApplication::removeWindowInfos(QSet<Window> closedWindowSet)
{
    bool appWindowsDisappeared = false;
    foreach(Window w, closedWindowSet) {
        if (windowMap.contains(w)) {
            WindowInfo removedWindow = windowMap.take(w);
            if (applicationWindows.removeOne(removedWindow)) {
                appWindowsDisappeared = true;
            }
        }
    }
    return appWindowsDisappeared;
}

bool HomeApplication::isApplicationWindow(const WindowInfo &wi)
{
    // Initialize the sets here, as the atom values in WindowInfo
    // might not have been set earlier
    if (excludeAtoms.count() == 0) {
        excludeAtoms.insert(WindowInfo::DesktopAtom);
        excludeAtoms.insert(WindowInfo::MenuAtom);
        excludeAtoms.insert(WindowInfo::DockAtom);
        excludeAtoms.insert(WindowInfo::DialogAtom);
        excludeAtoms.insert(WindowInfo::NotificationAtom);
        excludeAtoms.insert(WindowInfo::SkipTaskbarAtom);
    }

    QSet<Atom> excludeSet;
    excludeSet += wi.types().toSet();
    excludeSet += wi.states().toSet();

    return excludeSet.intersect(excludeAtoms).isEmpty();
}    
