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
#include <QApplication>
#include <QStringList>
#include <QTimer>
#include "lowbatterynotifier.h"
#include "notificationmanager.h"
#include "batterynotifier.h"

BatteryNotifier::BatteryNotifier(QObject *parent) :
    QObject(parent), lowBatteryNotifier(0), notificationId(0), touchScreenLockActive(false)
    ,qmBattery(new MeeGo::QmBattery),
    qmDeviceMode(new MeeGo::QmDeviceMode),
    qmLed(new MeeGo::QmLED),
    chargerType(MeeGo::QmBattery::Unknown)
{
    connect(qmBattery, SIGNAL(batteryStateChanged(MeeGo::QmBattery::BatteryState)), this, SLOT(batteryStateChanged(MeeGo::QmBattery::BatteryState)));
    connect(qmBattery, SIGNAL(chargingStateChanged(MeeGo::QmBattery::ChargingState)), this, SLOT(chargingStateChanged(MeeGo::QmBattery::ChargingState)));
    connect(qmBattery, SIGNAL(chargerEvent(MeeGo::QmBattery::ChargerType)), this, SLOT(batteryChargerEvent(MeeGo::QmBattery::ChargerType)));

    connect(qmDeviceMode, SIGNAL(devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState)), this, SLOT(devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState)));

    // Init battery values delayed...
    initBattery();

    notificationTimer.setInterval(5000);
    notificationTimer.setSingleShot(true);
}

BatteryNotifier::~BatteryNotifier()
{
    delete qmBattery;
    delete qmDeviceMode;
    delete qmLed;
}

void BatteryNotifier::initBattery()
{
    chargingStateChanged(qmBattery->getChargingState());
    batteryStateChanged(qmBattery->getBatteryState());
}

void BatteryNotifier::lowBatteryAlert()
{
    sendNotification(NotificationLowBattery);
}

void BatteryNotifier::chargingStateChanged(MeeGo::QmBattery::ChargingState state)
{
    switch(state) {
    case MeeGo::QmBattery::StateCharging:
        if (qmBattery->getChargerType() == MeeGo::QmBattery::USB_100mA) {
            sendNotification(NotificationNoEnoughPower);
        } else {
            // The low battery notifications should not be sent when the battery is charging
            stopLowBatteryNotifier();

            removeNotification(QStringList() << "x-nemo.battery.removecharger" << "x-nemo.battery.chargingcomplete" << "x-nemo.battery.lowbattery");
            sendNotification(NotificationCharging);
        }
        break;

    case MeeGo::QmBattery::StateNotCharging:
        removeNotification(QStringList() << "x-nemo.battery");
        utiliseLED(false, QString("PatternBatteryCharging"));
        break;

    case MeeGo::QmBattery::StateChargingFailed:
        sendNotification(NotificationChargingNotStarted);
        break;
    }
}

void BatteryNotifier::batteryStateChanged(MeeGo::QmBattery::BatteryState state)
{
    switch(state) {
    case MeeGo::QmBattery::StateFull:
        stopLowBatteryNotifier();
        removeNotification(QStringList() << "x-nemo.battery");
        sendNotification(NotificationChargingComplete);
        break;

    case MeeGo::QmBattery::StateOK:
        stopLowBatteryNotifier();
        break;

    case MeeGo::QmBattery::StateLow:
        if (qmBattery->getChargingState() != MeeGo::QmBattery::StateCharging) {
            // The low battery notifications should be sent only if the battery is not charging
            startLowBatteryNotifier();
        }
        break;

    case MeeGo::QmBattery::StateEmpty:
        sendNotification(NotificationRechargeBattery);
        break;

    case MeeGo::QmBattery::StateError:
        break;
    }
}

void BatteryNotifier::batteryChargerEvent(MeeGo::QmBattery::ChargerType type)
{
    switch(type) {
    case MeeGo::QmBattery::None:
        /*
         * After the user plugs out the charger from the device, this system
         * banner is displayed to remind the users to unplug charger from
         * the power supply for conserving energy.  Remove charger
         * notification should not be shown in case if USB cable is used for
         * charging the device.
         */
        if (chargerType == MeeGo::QmBattery::Wall) {
            removeNotification(QStringList() << "x-nemo.battery" << "x-nemo.battery.chargingcomplete");
            sendNotification(NotificationRemoveCharger);
        }

        if (chargerType != MeeGo::QmBattery::None && chargerType != MeeGo::QmBattery::USB_100mA && qmBattery->getBatteryState() == MeeGo::QmBattery::StateLow && qmBattery->getChargingState() != MeeGo::QmBattery::StateCharging) {
            // A charger was connected but is no longer connected and the battery is low, so start low battery notifier
            startLowBatteryNotifier();
        }
        break;

    case MeeGo::QmBattery::Wall:
        // Wall charger
        break;

    case MeeGo::QmBattery::USB_500mA:
        // USB with 500mA output
        break;

    case MeeGo::QmBattery::USB_100mA:
        // USB with 100mA output
        break;

    default:
        break;
    }

    chargerType = type;
}

void BatteryNotifier::devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState PSMState)
{
    if (PSMState == MeeGo::QmDeviceMode::PSMStateOff) {
        sendNotification(NotificationExitingPSM);
    } else if (PSMState == MeeGo::QmDeviceMode::PSMStateOn) {
        sendNotification(NotificationEnteringPSM);
    }
}

void BatteryNotifier::utiliseLED(bool activate, const QString &pattern)
{
    if (activate) {
        qmLed->activate(pattern);
    } else {
        qmLed->deactivate(pattern);
    }
}

void BatteryNotifier::sendNotification(BatteryNotifier::NotificationID id)
{
    switch(id) {
    case NotificationCharging:
        utiliseLED(true, QString("PatternBatteryCharging"));
        sendNotification("x-nemo.battery",
                //% "Charging"
                qtTrId("qtn_ener_charging"), chargingImageId());
        break;

    case NotificationChargingComplete:
        utiliseLED(true, QString("PatternBatteryFull"));
        sendNotification("x-nemo.battery.chargingcomplete",
                //% "Charging complete"
                qtTrId("qtn_ener_charcomp"));
        break;

    case NotificationRemoveCharger:
        sendNotification("x-nemo.battery.removecharger",
                //% "Disconnect charger from power supply to save energy"
                qtTrId("qtn_ener_remcha"));
        break;

    case NotificationChargingNotStarted:
        utiliseLED(false, QString("PatternBatteryCharging"));
        sendNotification("x-nemo.battery.chargingnotstarted",
                //% "Charging not started. Replace charger."
                qtTrId("qtn_ener_repcharger"));
        break;

    case NotificationRechargeBattery:
        sendNotification("x-nemo.battery.recharge",
                //% "Recharge battery"
                qtTrId("qtn_ener_rebatt"));
        break;

    case NotificationEnteringPSM:
        sendNotification("x-nemo.battery.enterpsm",
                //% "Entering power save mode"
                qtTrId("qtn_ener_ent_psnote"), chargingImageId());
        break;

    case NotificationExitingPSM:
        sendNotification("x-nemo.battery.exitpsm",
                //% "Exiting power save mode"
                qtTrId("qtn_ener_exit_psnote"), chargingImageId());
        break;

    case NotificationLowBattery:
        sendNotification("x-nemo.battery.lowbattery",
                //% "Low battery"
                qtTrId("qtn_ener_lowbatt"));
        break;

    case NotificationNoEnoughPower:
        sendNotification("x-nemo.battery.notenoughpower",
                //% "Not enough power to charge"
                qtTrId("qtn_ener_nopowcharge"), "icon-m-energy-management-insufficient-power");
        break;
    }
}

void BatteryNotifier::sendNotification(const QString &category, const QString &text, const QString &icon)
{
    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_CATEGORY, category);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, text);
    notificationId = manager->Notify(qApp->applicationName(), 0, icon, QString(), QString(), QStringList(), hints, -1);
    notificationCategory = category;
    notificationTimer.start();
}

void BatteryNotifier::removeNotification(const QStringList &categories)
{
    NotificationManager *manager = NotificationManager::instance();
    if (notificationId != 0 && categories.contains(notificationCategory) && notificationTimer.isActive()) {
        manager->CloseNotification(notificationId);
        notificationId = 0;
        notificationCategory.clear();
        notificationTimer.stop();
    }
}

QString BatteryNotifier::chargingImageId()
{
    int percentage = qmBattery->getRemainingCapacityPct();

    if (percentage >= 84) {
        return QString("icon-m-energy-management-charging8");
    } else if (percentage >= 73) {
        return QString("icon-m-energy-management-charging7");
    } else if (percentage >= 62) {
        return QString("icon-m-energy-management-charging6");
    } else if (percentage >= 51) {
        return QString("icon-m-energy-management-charging5");
    } else if (percentage >= 39) {
        return QString("icon-m-energy-management-charging4");
    } else if (percentage >= 28) {
        return QString("icon-m-energy-management-charging3");
    } else if (percentage >= 17) {
        return QString("icon-m-energy-management-charging2");
    } else if (percentage >= 5) {
        return QString("icon-m-energy-management-charging1");
    }
    return QString("icon-m-energy-management-charging-low");
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
