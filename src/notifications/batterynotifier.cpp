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
#include <QCoreApplication>
#include <QStringList>
#include <QTimer>
#include <QDebug>
#include "lowbatterynotifier.h"
#include "notificationmanager.h"
#include "batterynotifier.h"
#include <contextproperty.h>

// timeout to check is charging being started
//
// TODO There should be some environment variable used by different
// parties to share charging start-up timeouts
//
// - for usb cable - enough time for user to choose usb mode or
// mode the be chosen automatically
static int checkChargingStartedTimeoutUsb = 15 * 1000;

// - for wall charger - just enough time for charging being started
static int checkChargingStartedTimeoutWall = 3 * 1000;


static inline QString propertyString(ContextProperty *p)
{
    return p->value().toString().trimmed();
}

BatteryNotifier::BatteryNotifier(QObject *parent) :
    QObject(parent),
    lowBatteryNotifier(0),
    touchScreenLockActive(false),
    batteryLevel(new ContextProperty("Battery.Level", this)),
    chargingState(new ContextProperty("Battery.ChargingState", this)),
    chargerType(new ContextProperty("Battery.ChargerType", this)),
    psm(new ContextProperty("System.PowerSaveMode", this)),
    lastState({BatteryUnknown, StateUnknown, ChargerNo}),
    mode(ModeNormal)
{
    connect(batteryLevel, SIGNAL(valueChanged()), this, SLOT(onPropertyChanged()));
    connect(chargingState, SIGNAL(valueChanged()), this, SLOT(onPropertyChanged()));
    connect(chargerType, SIGNAL(valueChanged()), this, SLOT(onPropertyChanged()));
    connect(psm, SIGNAL(valueChanged()), this, SLOT(onPowerSaveModeChanged()));

    timeline.start();

    preNotificationTimer.setInterval(1000);
    preNotificationTimer.setSingleShot(true);
    connect(&preNotificationTimer, SIGNAL(timeout()),
            this, SLOT(prepareNotification()));

    QTimer::singleShot(0, this, SLOT(initBattery()));
}

BatteryNotifier::~BatteryNotifier()
{
}

void BatteryNotifier::initBattery()
{
    onPropertyChanged();
}

void BatteryNotifier::lowBatteryAlert()
{
    sendNotification(NotificationLowBattery);
}

void BatteryNotifier::prepareNotification()
{
    State newState;
    newState.state = getState();
    newState.level = getLevel();
    newState.charger = getCharger();

    bool isStateChanged = newState.state != lastState.state,
        isLevelChanged = newState.level != lastState.level,
        isChargerChanged = newState.charger != lastState.charger,
        isAnyChargingState = (newState.state == StateCharging
                          || newState.state == StateIdle);
    NotificationList toRemove;
    QList<NotificationID> toSend;

    if (isStateChanged) {
        if (newState.state == StateIdle) {
            stopLowBatteryNotifier();
            toRemove << NotificationCharging;
            toSend << NotificationChargingComplete;
        } else if (newState.state == StateCharging) {
            stopLowBatteryNotifier();
            toRemove << NotificationRemoveCharger
                     << NotificationChargingComplete
                     << NotificationLowBattery;
            toSend << NotificationCharging;
        } else {
            toRemove << NotificationChargingComplete
                     << NotificationRemoveCharger
                     << NotificationCharging
                     << NotificationChargingNotStarted;
        }
    }

    if ((isLevelChanged || isStateChanged) && !isAnyChargingState) {
        if (newState.level == BatteryLow) {
            startLowBatteryNotifier();
            toSend << NotificationLowBattery;
        } else if (isLevelChanged && lastState.state == StateDischarging) {
            toRemove << NotificationLowBattery;

            if (newState.level == BatteryEmpty) {
                toSend << NotificationRechargeBattery;
            } else {
                stopLowBatteryNotifier();
                toRemove << NotificationRechargeBattery;
            }
        }
    }

    if (isChargerChanged) {
        if (newState.charger == ChargerNo) {
            checkChargingTimer.reset();
            toRemove << NotificationCharging
                     << NotificationChargingComplete;

            if (lastState.charger == ChargerWall)
                toSend << NotificationRemoveCharger;
        } else if (lastState.charger == ChargerNo && !isAnyChargingState) {
            // charger is inserted but battery is still discharging
            checkChargingTimer.reset(new QTimer());
            checkChargingTimer->setSingleShot(true);
            // unknown charger is also checked after wall charger
            // timeout
            int timeout = (newState.charger == ChargerUsb
                           ? checkChargingStartedTimeoutUsb
                           : checkChargingStartedTimeoutWall);
            checkChargingTimer->setInterval(timeout);
            connect(checkChargingTimer.data(), SIGNAL(timeout()),
                    this, SLOT(checkIsChargingStarted()));
            checkChargingTimer->start();
        }
    }

    // call always to cleanup expired notifications
    removeNotification(toRemove);

    foreach(NotificationID id, toSend)
        sendNotification(id);

    lastState = newState;
}

void BatteryNotifier::checkIsChargingStarted()
{
    if (lastState.charger != ChargerNo && lastState.state == StateDischarging) {
        sendNotification(NotificationChargingNotStarted);
    }
}

void BatteryNotifier::onPropertyChanged()
{
    if (!preNotificationTimer.isActive()) {
        preNotificationTimer.start();
    }
}

void BatteryNotifier::onPowerSaveModeChanged()
{
    Mode newMode = psm->value().toInt() ? ModePSM : ModeNormal;
    if (mode != newMode) {
        sendNotification(newMode == ModePSM
                         ? NotificationEnteringPSM
                         : NotificationExitingPSM);
        mode = newMode;
    }
}

void BatteryNotifier::sendNotification(BatteryNotifier::NotificationID id)
{
    static const struct NotificationInfo {
        QString category;
        QString message;
        QString icon;
    } description[] = {
        {"x-nemo.battery", // NotificationCharging
         //% "Charging"
         qtTrId("qtn_ener_charging"),
         ""},
        {"x-nemo.battery.chargingcomplete", // NotificationChargingComplete
         //% "Charging complete"
         qtTrId("qtn_ener_charcomp"),
         ""},
        {"x-nemo.battery.removecharger", // NotificationRemoveCharger
         //% "Disconnect charger from power supply to save energy"
         qtTrId("qtn_ener_remcha"),
         ""},
        {"x-nemo.battery.chargingnotstarted", // NotificationChargingNotStarted
         //% "Charging not started. Replace charger."
         qtTrId("qtn_ener_repcharger"),
         ""},
        {"x-nemo.battery.recharge", // NotificationRechargeBattery
         //% "Recharge battery"
         qtTrId("qtn_ener_rebatt"),
         ""},
        {"x-nemo.battery.enterpsm", // NotificationEnteringPSM
         //% "Entering power save mode"
         qtTrId("qtn_ener_ent_psnote"),
         ""},
        {"x-nemo.battery.exitpsm", // NotificationExitingPSM
         //% "Exiting power save mode"
         qtTrId("qtn_ener_exit_psnote"),
         ""},
        {"x-nemo.battery.lowbattery", // NotificationLowBattery
         //% "Low battery"
         qtTrId("qtn_ener_lowbatt"),
         ""},
        {"x-nemo.battery.notenoughpower", // NotificationNoEnoughPower
         //% "Not enough power to charge"
         qtTrId("qtn_ener_nopowcharge"),
         "icon-m-energy-management-insufficient-power"}
    };
    Q_ASSERT(id < sizeof(description) / sizeof(description[0]));
    NotificationInfo const &info = description[id];

    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_CATEGORY, info.category);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, info.message);
    QueuedNotification queued;
    queued.number = manager->Notify(qApp->applicationName(), 0, info.icon,
                                    QString(), QString(), QStringList(), hints, -1);
    queued.id = id;
    queued.time = timeline.elapsed();
    if (notifications.size() == 3) // saves only last 3
        notifications.pop_front();
    notifications.push_back(queued);
}

void BatteryNotifier::removeNotification(const NotificationList &ids)
{
    NotificationManager *manager = NotificationManager::instance();
    qint64 now = timeline.elapsed();
    for (QList<QueuedNotification>::iterator it = notifications.begin();
         it != notifications.end();) {
        // inherited: notification is shown for < 5 sec?
        if (now - it->time < 5000) {
            if (!ids.contains(it->id)) {
                ++it;
                continue;
            }

            manager->CloseNotification(it->number);
        }
        it = notifications.erase(it);
    }
}

void BatteryNotifier::setTouchScreenLockActive(bool active)
{
    touchScreenLockActive = active;
    if (lowBatteryNotifier != NULL) {
        lowBatteryNotifier->setTouchScreenLockActive(active);
    }
}

void BatteryNotifier::startLowBatteryNotifier()
{
    if (lowBatteryNotifier == NULL) {
        lowBatteryNotifier = new LowBatteryNotifier();
        connect(lowBatteryNotifier, SIGNAL(lowBatteryAlert()), this, SLOT(lowBatteryAlert()));
    }

    lowBatteryNotifier->setTouchScreenLockActive(touchScreenLockActive);
    lowBatteryNotifier->sendLowBatteryAlert();
}

void BatteryNotifier::stopLowBatteryNotifier()
{
    if (lowBatteryNotifier != NULL) {
        delete lowBatteryNotifier;
        lowBatteryNotifier = NULL;
    }
}

BatteryNotifier::BatteryLevel BatteryNotifier::getLevel() const
{
    QString name(propertyString(batteryLevel));
    return (name == "normal"
            ? BatteryNormal
            : (name == "low"
               ? BatteryLow
               : (name == "empty"
                  ? BatteryEmpty
                  : BatteryUnknown)));
}

BatteryNotifier::ChargingState BatteryNotifier::getState() const
{
    QString name(propertyString(chargingState));
    return (name == "charging"
            ? StateCharging
            : (name == "discharging"
               ? StateDischarging
               : (name == "idle"
                  ? StateIdle
                  : StateUnknown)));
}

BatteryNotifier::ChargerType BatteryNotifier::getCharger() const
{
    QString name(propertyString(chargerType));
    return ((name == "dcp" || name == "cdp")
            ? ChargerWall
            : (name == "usb"
               ? ChargerUsb
               : (name == ""
                  ? ChargerNo
                  : ChargerUnknown)));
}
