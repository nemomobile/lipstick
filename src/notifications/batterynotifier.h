/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012,2015 Jolla Ltd.
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
#include <QStringList>
#include <QElapsedTimer>

class LowBatteryNotifier;
class ContextProperty;

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


    enum NotificationID {
        NotificationCharging,
        NotificationChargingComplete,
        NotificationRemoveCharger,
        NotificationChargingNotStarted,
        NotificationRechargeBattery,
        NotificationEnteringPSM,
        NotificationExitingPSM,
        NotificationLowBattery,
        NotificationNoEnoughPower
    };
    enum PropertyID {
        PropertyFirst_ = 0,
        PropertyLevel = PropertyFirst_,
        PropertyState,
        PropertyCharger,
        PropertyLast_ = PropertyCharger
    };

    enum BatteryLevel {
        BatteryNormal,
        BatteryLow,
        BatteryEmpty,
        BatteryUnknown
    };
    enum ChargingState {
        StateCharging,
        StateDischarging,
        StateIdle,
        StateUnknown
    };
    enum ChargerType {
        ChargerUnknown,
        ChargerUsb,
        ChargerWall,
        ChargerNo
    };

    enum Mode {
        ModeNormal,
        ModePSM
    };

public slots:
    //! Initializes the battery status from the current values given by QBatteryInfo
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
    void onPowerSaveModeChanged();
    void onPropertyChanged();
    void prepareNotification();
    void checkIsChargingStarted();

private:
    //! Sends a notification based on the notification ID
    void sendNotification(BatteryNotifier::NotificationID id);

    //! Removes the current notification if its type is one listed in eventTypes
    typedef QSet<NotificationID> NotificationList;
    void removeNotification(const NotificationList &);

    //! Starts the low battery notifier if not already started
    void startLowBatteryNotifier();

    //! Stops the low battery notifier if not already stopped
    void stopLowBatteryNotifier();

    BatteryLevel getLevel() const;
    ChargingState getState() const;
    ChargerType getCharger() const;

    //! Low battery notifier for getting notifications about low battery state
    LowBatteryNotifier *lowBatteryNotifier;

    struct QueuedNotification {
        uint number;
        NotificationID id;
        qint64 time;
    };
    /*! There can be several queued notifications (e.g. psm and
     *  charging one at the same time) and the only one should be
     *  cancelled
     */
    QList<QueuedNotification> notifications;

    //! Timer for checking whether the current notification can be removed or not
    QElapsedTimer timeline;

    //! Whether the touch screen lock is active or not
    bool touchScreenLockActive;

    ContextProperty *batteryLevel;
    ContextProperty *chargingState;
    ContextProperty *chargerType;

    //! To get device mode
    ContextProperty *psm;

    struct State {
        BatteryLevel level;
        ChargingState state;
        ChargerType charger;
    };
    State lastState;
    Mode mode;
    enum ChargingCompletion {
        NeedsCharging,
        FullyCharged
    };
    ChargingCompletion chargingCompletion;

    /*! Notification is postponed by means of this timer to skip
     *  frequent state changes during energy management state
     *  changes
     */
    QTimer preNotificationTimer;

    /*! used if charging is not signaled as started immediately after
     *  charger is inserted to check is it finally started
     */
    QScopedPointer<QTimer> checkChargingTimer;

#ifdef UNIT_TEST
    friend class Ut_BatteryNotifier;
#endif
};

#endif
