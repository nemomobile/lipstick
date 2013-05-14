/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QTimer>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QTextStream>
#include <QCursor>
#include <mce/mode-names.h>

#include "homeapplication.h"
#include "screenlock.h"
#include "utilities/closeeventeater.h"

ScreenLock::ScreenLock(QObject* parent) :
    QObject(parent),
    callbackInterface(NULL),
    shuttingDown(false),
    lockscreenVisible(false)
{
}

ScreenLock::~ScreenLock()
{
}

int ScreenLock::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool, bool)
{
    if (shuttingDown) {
        // Don't show the touch screen lock while shutting down
        return TkLockReplyOk;
    }

    // Create a D-Bus interface if one doesn't exist or the D-Bus callback details have changed
    if (callbackInterface == NULL || callbackInterface->service() != service || callbackInterface->path() != path || callbackInterface->interface() != interface) {
        delete callbackInterface;
        callbackInterface = new QDBusInterface(service, path, interface, QDBusConnection::systemBus(), this);
    }

    // Store the callback method name
    callbackMethod = method;

    // MCE needs a response ASAP, so the actions are executed with single shot timers
    switch (mode) {
    case TkLockModeEnable:
        // Create the lock screen already so that it's readily available
        QTimer::singleShot(0, this, SLOT(showScreenLock()));
        break;

    case TkLockModeOneInput:
        QTimer::singleShot(0, this, SLOT(showEventEater()));
        break;

    case TkLockEnableVisual:
        // Raise the lock screen window on top if it isn't already
        QTimer::singleShot(0, this, SLOT(showScreenLock()));
        break;

    case TkLockEnableLowPowerMode:
        // Enable low power mode and raise the lock screen window on top if it isn't already
        QTimer::singleShot(0, this, SLOT(showLowPowerMode()));
        break;

    case TkLockRealBlankMode:
        QTimer::singleShot(0, this, SLOT(setDisplayOffMode()));
        break;

    default:
        break;
    }

    return TkLockReplyOk;
}

int ScreenLock::tklock_close(bool)
{
    QTimer::singleShot(0, this, SLOT(hideScreenLockAndEventEater()));

    return TkLockReplyOk;
}

void ScreenLock::lockScreen()
{
    QDBusMessage message = QDBusMessage::createMethodCall("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", "req_tklock_mode_change");
    message.setArguments(QVariantList() << MCE_TK_LOCKED_DELAY);
    QDBusConnection::systemBus().asyncCall(message);

    showScreenLock();
}

void ScreenLock::unlockScreen()
{
    hideScreenLockAndEventEater();

    if (callbackInterface != NULL && !callbackMethod.isEmpty()) {
        callbackInterface->call(QDBus::NoBlock, callbackMethod, TkLockUnlock);
    }
}

void ScreenLock::showScreenLock()
{
    toggleScreenLockUI(true);
    toggleEventEater(false);
}

void ScreenLock::showLowPowerMode()
{
    toggleScreenLockUI(true);
    toggleEventEater(false);
}

void ScreenLock::setDisplayOffMode()
{
    toggleScreenLockUI(true);
    toggleEventEater(false);
}

void ScreenLock::hideScreenLockAndEventEater()
{
    toggleScreenLockUI(false);
    toggleEventEater(false);
}

void ScreenLock::showEventEater()
{
    toggleEventEater(true);
}

void ScreenLock::hideEventEater()
{
    toggleEventEater(false);
}

void ScreenLock::toggleScreenLockUI(bool toggle)
{
    // TODO Make the view a lock screen view (title? stacking layer?)
    lockscreenVisible = toggle;
    emit screenIsLocked(toggle);
}

void ScreenLock::toggleEventEater(bool toggle)
{
    // TODO
    Q_UNUSED(toggle)
}

bool ScreenLock::isScreenLocked() const
{
    return lockscreenVisible;
}
