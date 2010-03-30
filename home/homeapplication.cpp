/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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
#include "mainwindow.h"
#include "x11wrapper.h"
#include "x11helper.h"

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
    DuiApplication(argc, argv)
{
    // Get X11 Atoms for different window types
    Display *dpy = QX11Info::display();
    windowTypeAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    windowTypeNormalAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
    windowTypeDesktopAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    windowTypeNotificationAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
    windowTypeCallAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_CALL", False);
    windowTypeDockAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
    clientListAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLIENT_LIST", False);
    closeWindowAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLOSE_WINDOW", False);
    skipTaskbarAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", False);
    windowStateAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_STATE", False);

    // launch a timer for sending a dbus-signal upstart when home is ready
    // and on screen
    upstartMode = isUpstartMode(argc, argv);
    connect(&startupNotificationTimer, SIGNAL(timeout()),
            this, SLOT(sendStartupNotifications()));
    startupNotificationTimer.setSingleShot(true);
    startupNotificationTimer.setInterval(0);
    startupNotificationTimer.start();
}

HomeApplication::~HomeApplication()
{
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
    if (event->type == PropertyNotify &&
            event->xproperty.window == DefaultRootWindow(QX11Info::display()) &&
            event->xproperty.atom == clientListAtom) {
        // The _NET_CLIENT_LIST property of the root window has changed so update the window list
        updateWindowList();
        return true;
    } else if (event->type == VisibilityNotify) {
        if (event->xvisibility.state == VisibilityFullyObscured) {
            // A window was obscured: was it a homescreen window?
            bool homescreenWindowVisibilityChanged = false;
            Q_FOREACH(DuiWindow * win, DuiApplication::windows()) {
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
            windowsBeingClosed.append(event->xclient.window);
        }
        updateWindowList();
        return true;
    } else if (event->type == PropertyNotify &&
               (event->xproperty.atom == windowTypeAtom || event->xproperty.atom == windowStateAtom)) {
        // Window types changed so update the window list
        updateWindowList();
        return true;
    }

    return DuiApplication::x11EventFilter(event);
}

void HomeApplication::updateWindowList()
{
    // Get a list of all windows
    Display *dpy = QX11Info::display();
    XWindowAttributes wAttributes;
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;
    int result = X11Wrapper::XGetWindowProperty(dpy, DefaultRootWindow(dpy), clientListAtom, 0, 0x7fffffff, False, XA_WINDOW,
                 &actualType, &actualFormat, &numWindowItems, &bytesLeft, &windowData);

    if (result == Success && windowData != None) {
        // Go through the list of all windows
        QList<Window> windowsStillBeingClosed;
        QList<WindowInfo> windowList;
        Window *wins = (Window *)windowData;
        for (unsigned int i = 0; i < numWindowItems; i++) {
            // The windows that are bigger than 0x0, are Input/Output windows and are not unmapped are interesting
            if (X11Wrapper::XGetWindowAttributes(dpy, wins[i],
                                                 &wAttributes) != 0 && wAttributes.width > 0 &&
                    wAttributes.height > 0 && wAttributes.c_class == InputOutput &&
                    wAttributes.map_state != IsUnmapped) {
                unsigned char *typeData = NULL;
                unsigned long numTypeItems;

                // Get the window type
                result = X11Wrapper::XGetWindowProperty(dpy, wins[i], windowTypeAtom, 0L, 16L, False, XA_ATOM,
                                                        &actualType, &actualFormat, &numTypeItems, &bytesLeft, &typeData);

                if (result == Success) {
                    Atom *type = (Atom *)typeData;

                    // Only "normal" windows should be included in the window list
                    bool includeInWindowList = false;
                    WindowInfo::WindowPriority priority = WindowInfo::Normal;
                    // plain X windows like xclock and xev have no type
                    if (numTypeItems == 0) {
                        includeInWindowList = true;
                    }
                    for (unsigned int n = 0; n < numTypeItems; n++) {
                        // "Desktop", "Notification" and "Dock" windows should never be included in the window list
                        if (type[n] == windowTypeDesktopAtom || type[n] == windowTypeNotificationAtom || type[n] == windowTypeDockAtom) {
                            includeInWindowList = false;
                            break;
                        }
                        if (type[n] == windowTypeNormalAtom) {
                            includeInWindowList = true;
                        } else if (type[n] == windowTypeCallAtom) {
                            includeInWindowList = true;
                            priority = WindowInfo::Call;
                        }
                    }

                    if (includeInWindowList) {
                        // If _NET_WM_STATE has the _NET_WM_STATE_SKIP_TASKBAR set, don't include the window to the list
                        if (X11Helper::getNetWmState(dpy, wins[i]).contains(skipTaskbarAtom)) {
                            includeInWindowList = false;
                        }
                    }

                    if (includeInWindowList) {
                        Pixmap pixmap = NULL;
                        XTextProperty textProperty;
                        QString title;

                        // Tell X that changes in the visibility of the window
                        // and in the properties of the window are interesting
                        X11Wrapper::XSelectInput(dpy, wins[i],
                                                 VisibilityChangeMask
                                                 | PropertyChangeMask);

                        // Get window title
                        if (X11Wrapper::XGetWMName(dpy, wins[i], &textProperty) != 0) {
                            title = QString((const char *)(textProperty.value));
                            X11Wrapper::XFree(textProperty.value);
                        }

                        // Get window icon
                        XWMHints *wmhints = X11Wrapper::XGetWMHints(dpy, wins[i]);
                        if (wmhints != NULL) {
                            pixmap = wmhints->icon_pixmap;
                            X11Wrapper::XFree(wmhints);
                        }

                        if (!windowsBeingClosed.contains(wins[i])) {
                            // Add a window to the list
                            windowList.append(WindowInfo(title, wins[i], wAttributes, pixmap, priority));
                        } else {
                            // The window is still being closed and can not be removed from the windows being closed list
                            windowsStillBeingClosed.append(wins[i]);
                        }
                    }

                    X11Wrapper::XFree(typeData);
                }
            }
        }

        X11Wrapper::XFree(wins);

        // Remove all windows that have now been closed from the windows being closed list
        for (int i = windowsBeingClosed.count() - 1; i >= 0; i--) {
            Window window = windowsBeingClosed.at(i);
            if (!windowsStillBeingClosed.contains(window)) {
                windowsBeingClosed.removeAt(i);
            }
        }

        // Signal listeners that the window list has changed
        emit windowListUpdated(windowList);
    }
}
