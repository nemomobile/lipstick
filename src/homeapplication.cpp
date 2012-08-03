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

#include "mainwindow.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QIcon>
#include <QX11Info>
#include <QDebug>
#include <QEvent>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

#include "homeapplication.h"
#include "windowinfo.h"
#include "xeventlistener.h"

/*!
 * D-Bus names for the notification that's sent when home is ready
 */
static const QString HOME_READY_SIGNAL_PATH = "/com/nokia/duihome";
static const QString HOME_READY_SIGNAL_INTERFACE = "com.nokia.duihome.readyNotifier";
static const QString HOME_READY_SIGNAL_NAME = "ready";

HomeApplication::HomeApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , xEventListeners()
    , iteratorActiveForEventListenerContainer(false)
    , toBeRemovedEventListeners()
    , xDamageEventBase(0)
    , xDamageErrorBase(0)
{
    XDamageQueryExtension(QX11Info::display(), &xDamageEventBase, &xDamageErrorBase);

    // launch a timer for sending a dbus-signal upstart when basic construct is done
    QTimer::singleShot(0, this, SLOT(sendStartupNotifications()));

    // Initialize the X11 atoms used in the UI components
    WindowInfo::initializeAtoms();
}

HomeApplication::~HomeApplication()
{
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

    // For device boot performance reasons initializing Home scene window must be done
    // only after ready signal is sent (NB#277602)
    MainWindow::instance(true)->show();

    XDamageCreate(QX11Info::display(), MainWindow::instance()->effectiveWinId(), XDamageReportNonEmpty);
}

bool HomeApplication::x11EventFilter(XEvent *event)
{
    bool eventHandled = false;
    iteratorActiveForEventListenerContainer = true;

    if (event->xany.window == MainWindow::instance()->effectiveWinId())
    {
        qDebug() << Q_FUNC_INFO << "received event for main window!";
        MainWindow::instance()->viewport()->repaint();
    }

    if (event->type == xDamageEventBase + XDamageNotify) {
        qDebug() << Q_FUNC_INFO << "Processing damage event";
        XDamageNotifyEvent *xevent = (XDamageNotifyEvent *) event;

        // xevent->more would inform us if there is more events for the
        // rendering operation. but there isn't interface to pass the
        // information to damageEvent.
        emit damageEvent(xevent->damage, xevent->area.x, xevent->area.y, xevent->area.width, xevent->area.height);
        eventHandled = true;
    }

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
        eventHandled = QApplication::x11EventFilter(event);
    }

    return eventHandled;
}
