/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary (-ies).
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
#ifndef LOWBATTERYNOTIFIER_H
#define LOWBATTERYNOTIFIER_H

#include <QObject>
#include <QTime>
#include <qmdisplaystate.h>
#include <contextproperty.h>

class QTimer;

/*!
 * The LowBatteryNotifier will emit a lowBatteryAlert() when the user should
 * be alerted about a low battery condition.
 */
class LowBatteryNotifier : public QObject {
    Q_OBJECT

public:
    /*!
     * Creates a low battery notifier.
     */
    LowBatteryNotifier(QObject *parent = NULL);

    /*!
     * Destroys the low battery notifier.
     */
    virtual ~LowBatteryNotifier();

    /*!
     * Sets the touch screen lock active state so notifications can be enabled/disabled based on that.
     *
     * \param active \c true if the touch screen lock is active, \c false otherwise
     */
    void setTouchScreenLockActive(bool active);

public slots:
    //! Requests a low battery notification to be shown
    void sendLowBatteryAlert();

private slots:
    //! Sets the notification interval based on the device activity state
    void setNotificationInterval();

signals:
    //! Sent when the user should be notified about a low battery state
    void lowBatteryAlert();

private:
    //! For getting the display state
    MeeGo::QmDisplayState *displayState;

    //! Call state context framework key
    ContextProperty callContextItem;

    //! Timer for sending low battery alerts
    QTimer *notificationTimer;

    //! Time of the previous notification
    QTime previousNotificationTime;

    //! Notification interval in milliseconds based on the device and call state
    int notificationInterval;

    //! Whether the device is currently inactive or not
    bool deviceInactive;

    //! Whether the touch screen lock is active or not
    bool touchScreenLockActive;

    //! Whether a call is in progress or not
    bool callActive;

#ifdef UNIT_TEST
    friend class Ut_LowBatteryNotifier;
#endif
};
#endif
