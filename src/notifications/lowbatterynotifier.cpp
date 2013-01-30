/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (directui@nokia.com)
 **
 ** This file is part of systemui.
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
#include "lowbatterynotifier.h"
#include <QTimer>

static const int CALL_ACTIVE_NOTIFICATION_INTERVAL = 2 * 60 * 1000;
static const int DEVICE_ACTIVE_NOTIFICATION_INTERVAL = 5 * 60 * 1000;
static const int DEVICE_INACTIVE_NOTIFICATION_INTERVAL = 30 * 60 * 1000;

LowBatteryNotifier::LowBatteryNotifier(QObject *parent) :
    QObject(parent),
    displayState(new MeeGo::QmDisplayState(this)),
    callContextItem("Phone.Call"),
    notificationTimer(new QTimer(this)),
    previousNotificationTime(QTime::currentTime()),
    notificationInterval(DEVICE_ACTIVE_NOTIFICATION_INTERVAL),
    deviceInactive(false),
    touchScreenLockActive(false),
    callActive(false)
{
    connect(notificationTimer, SIGNAL(timeout()), this, SLOT(sendLowBatteryAlert()));

    setNotificationInterval();

    connect(displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(setNotificationInterval()));

    connect(&callContextItem, SIGNAL(valueChanged()), this, SLOT(setNotificationInterval()));
    callContextItem.subscribe();
}

LowBatteryNotifier::~LowBatteryNotifier()
{
}

void LowBatteryNotifier::sendLowBatteryAlert()
{
    emit lowBatteryAlert();

    previousNotificationTime.start();
    notificationTimer->start(notificationInterval);
}

void LowBatteryNotifier::setNotificationInterval()
{
    bool deviceCurrentlyInactive = touchScreenLockActive;
    bool callCurrentlyActive = callContextItem.value().toString() == "active";

    // Device can be considered inactive only if the touch screen lock is active AND the display is off
    deviceCurrentlyInactive &= displayState->get() == MeeGo::QmDisplayState::Off;

    if (deviceCurrentlyInactive != deviceInactive || callCurrentlyActive != callActive) {
        // Device activity or call activity has changed
        deviceInactive = deviceCurrentlyInactive;
        callActive = callCurrentlyActive;

        // Set the new notification interval based on the device and call state
        if (callActive) {
            notificationInterval = CALL_ACTIVE_NOTIFICATION_INTERVAL;
        } else {
            notificationInterval = deviceInactive ? DEVICE_INACTIVE_NOTIFICATION_INTERVAL : DEVICE_ACTIVE_NOTIFICATION_INTERVAL;
        }

        if (previousNotificationTime.elapsed() < notificationInterval) {
            // Elapsed time has not reached the notification interval so just set the new interval
            notificationTimer->setInterval(notificationInterval - previousNotificationTime.elapsed());
        } else {
            // Elapsed time has reached the notification interval, so send the notification immediately (which will also set the new interval)
            sendLowBatteryAlert();
        }
    }
}

void LowBatteryNotifier::setTouchScreenLockActive(bool active)
{
    touchScreenLockActive = active;
    setNotificationInterval();
}
