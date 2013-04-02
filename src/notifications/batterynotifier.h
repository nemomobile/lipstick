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
#ifndef BATTERYNOTIFIER_H
#define BATTERYNOTIFIER_H

#include <QObject>
#include <QTimer>
#include <QScopedPointer>
#include <QSystemBatteryInfo>
#include <qmled.h>
#include <qmdevicemode.h>

class LowBatteryNotifier;

using namespace QtMobility;

/*!
 * Implements the configuration and state for the battery, the power save mode.
 */
class BatteryNotifier : public QObject {
    Q_OBJECT

public:
    /*!
     * Creates a new battery business logic.
     *
     * \param the parent QObject
     */
    BatteryNotifier(QObject *parent = NULL);

    /*!
     * Destroys the battery business logic.
     */
    virtual ~BatteryNotifier();

    typedef enum {
        NotificationCharging,
        NotificationChargingComplete,
        NotificationRemoveCharger,
        NotificationChargingNotStarted,
        NotificationRechargeBattery,
        NotificationEnteringPSM,
        NotificationExitingPSM,
        NotificationLowBattery,
        NotificationNoEnoughPower
    } NotificationID;

public slots:
    //! Initializes the battery status from the current values given by QSystemBatteryInfo
    void initBattery();

    //! Sends a low battery notification
    void lowBatteryAlert();

    /*!
     * Sets the touch screen lock active state so notifications can be enabled/disabled based on that.
     *
     * \param active \c true if the touch screen lock is active, \c false otherwise
     */
    void setTouchScreenLockActive(bool active);

private slots:
    void applyBatteryStatus(QSystemBatteryInfo::BatteryStatus status);
    void applyChargingState(QSystemBatteryInfo::ChargingState state);
    void applyChargerType(QSystemBatteryInfo::ChargerType type);
    void applyPSMState(MeeGo::QmDeviceMode::PSMState psmState);
    void utiliseLED(bool activate, const QString &pattern);

private:
    //! Sends a notification based on the notification ID
    void sendNotification(BatteryNotifier::NotificationID id);

    //! Sends a notification
    void sendNotification(const QString &category, const QString &text, const QString &icon = QString(""));

    //! Removes the current notification if its type is one listed in eventTypes
    void removeNotification(const QStringList &categories);

    //! Returns the charging image ID based on the current battery level
    QString chargingImageId();

    //! Starts the low battery notifier if not already started
    void startLowBatteryNotifier();

    //! Stops the low battery notifier if not already stopped
    void stopLowBatteryNotifier();

    //! Low battery notifier for getting notifications about low battery state
    LowBatteryNotifier *lowBatteryNotifier;

    //! The current notification
    uint notificationId;

    //! The category of the current notification
    QString notificationCategory;

    //! Timer for checking whether the current notification can be removed or not
    QTimer notificationTimer;

    //! Whether the touch screen lock is active or not
    bool touchScreenLockActive;

    //! For getting battery state
    QtMobility::QSystemBatteryInfo *batteryInfo;

    //! For getting device mode
    MeeGo::QmDeviceMode *qmDeviceMode;

    //! For controlling the LED
    MeeGo::QmLED *qmLed;

    //! The current charger type
    QtMobility::QSystemBatteryInfo::ChargerType chargerType;

#ifdef UNIT_TEST
    friend class Ut_BatteryNotifier;
#endif
};

#endif
