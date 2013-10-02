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

#include <QtTest/QtTest>

#include "lowbatterynotifier_stub.h"
#include "notificationmanager_stub.h"
#include "qbatteryinfo_stub.h"
#include "qmdevicemode_stub.h"
#include "qmdisplaystate_stub.h"
#include "batterynotifier.h"
#include "ut_batterynotifier.h"

void QTimer::start()
{
    id = 1;
}

void Ut_BatteryNotifier::initTestCase()
{
}

void Ut_BatteryNotifier::cleanupTestCase()
{
}

void Ut_BatteryNotifier::init()
{
    batteryNotifier = new BatteryNotifier;

    gNotificationManagerStub->stubReset();
    gNotificationManagerStub->stubSetReturnValue("Notify", (uint)1);
}

void Ut_BatteryNotifier::cleanup()
{
    delete batteryNotifier;
    batteryNotifier = NULL;
    gNotificationManagerStub->stubReset();
    gLowBatteryNotifierStub->stubReset();
    gQBatteryInfoStub->stubReset();
}

void Ut_BatteryNotifier::testInitBattery()
{
    gQBatteryInfoStub->stubSetReturnValue<QBatteryInfo::ChargingState>("chargingState", QBatteryInfo::UnknownChargingState);
    gQBatteryInfoStub->stubSetReturnValue<QBatteryInfo::BatteryStatus>("batteryStatus", QBatteryInfo::BatteryOk);

    // no notification should be shown and battery charging pattern should be deactivated
    batteryNotifier->initBattery();

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);
}

void Ut_BatteryNotifier::testLowBatteryAlert()
{
    batteryNotifier->lowBatteryAlert();

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.lowbattery"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_lowbatt"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());
}

void Ut_BatteryNotifier::testBatteryStateChanged()
{
    QList<QVariant> arguments;

    gQBatteryInfoStub->stubReset();

    /* StateFull */
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryFull);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.chargingcomplete"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charcomp"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* StateOK */
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryOk);

    /* no notifications should be published, just silently no-op */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);

    /* StateEmpty */
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryEmpty);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.recharge"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_rebatt"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* StateError */
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryStatusUnknown);

    /* no notifications should be published, just silently no-op */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);

    /* StateLow and charging */
    gQBatteryInfoStub->stubSetReturnValue<QBatteryInfo::ChargingState>("chargingState", QBatteryInfo::Charging);
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryLow);

    /* no notifications should be published, because battery is charging... */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);

    /* StateLow and not charging */
    gQBatteryInfoStub->stubSetReturnValue<QBatteryInfo::ChargingState>("chargingState", QBatteryInfo::UnknownChargingState);
    batteryNotifier->setTouchScreenLockActive(true);
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryLow);

    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);
}

void Ut_BatteryNotifier::testChargingStateChanged()
{
    QList<QVariant> arguments;

    gQBatteryInfoStub->stubReset();

    gQBatteryInfoStub->stubSetReturnValue("chargerType", QBatteryInfo::WallCharger);

    for(int i = 0; i <= 100; i += 5) {
        /* StateCharging */
        gQBatteryInfoStub->stubSetReturnValue<int>("remainingCapacityPercent", i);
        batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);

        QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
        QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery"));
        QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charging"));

        gNotificationManagerStub->stubReset();
    }

    /* StateUnknownChargingState */
    batteryNotifier->applyChargingState(0, QBatteryInfo::UnknownChargingState);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);

    /* StateChargingFailed */
    batteryNotifier->applyChargingState(0, QBatteryInfo::NotCharging);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.chargingnotstarted"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_repcharger"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* Test "not enough power to charge" situation... */
    gQBatteryInfoStub->stubSetReturnValue("chargerType", QBatteryInfo::USBCharger);
    gQBatteryInfoStub->stubSetReturnValue("currentFlow", 100);
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.notenoughpower"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_nopowcharge"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString("icon-m-energy-management-insufficient-power"));
}

void Ut_BatteryNotifier::testBatteryChargerEvent()
{
    QList<QVariant> arguments;

    /* Wall charger */
    batteryNotifier->applyChargerType(QBatteryInfo::WallCharger);
    QCOMPARE(batteryNotifier->chargerType, QBatteryInfo::WallCharger);

    /* Plug out : charger type = none */
    batteryNotifier->applyChargerType(QBatteryInfo::UnknownCharger);
    QCOMPARE(batteryNotifier->chargerType, QBatteryInfo::UnknownCharger);

    /* Look for the notification: "Disconnect the charger from..." */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.removecharger"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_remcha"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* USB 500mA */
    batteryNotifier->applyChargerType(QBatteryInfo::USBCharger);
    QCOMPARE(batteryNotifier->chargerType, QBatteryInfo::USBCharger);

    /* USB 100mA */
    gQBatteryInfoStub->stubSetReturnValue("currentFlow", 100);
    batteryNotifier->applyChargerType(QBatteryInfo::USBCharger);
    QCOMPARE(batteryNotifier->chargerType, QBatteryInfo::USBCharger);

    /* Unknown */
    batteryNotifier->applyChargerType(QBatteryInfo::UnknownCharger);
    QCOMPARE(batteryNotifier->chargerType, QBatteryInfo::UnknownCharger);
}

void Ut_BatteryNotifier::testPSMStateChanged()
{
    QList<QVariant> arguments;

    /* Entering to power-save mode */
    batteryNotifier->applyPSMState(MeeGo::QmDeviceMode::PSMStateOn);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.enterpsm"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_ent_psnote"));

    /* Exiting from power-save mode */
    batteryNotifier->applyPSMState(MeeGo::QmDeviceMode::PSMStateOff);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.exitpsm"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_exit_psnote"));
}

void Ut_BatteryNotifier::testLowBatteryNotifierConnection()
{
    QList<QVariant> arguments;

    gQBatteryInfoStub->stubSetReturnValue("chargerType", QBatteryInfo::USBCharger);

    /* LowBatteryNotifier shouldn't be instantiated at first */
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* Simulate battery-state-low change */
    gQBatteryInfoStub->stubSetReturnValue<QBatteryInfo::ChargingState>("chargingState", QBatteryInfo::UnknownChargingState);
    batteryNotifier->setTouchScreenLockActive(true);
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryLow);

    /* LowBatteryNotifier should be exists now... */
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    /* And should send a low-battery notification */
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);

    /* Simulate now a charging event */
    gQBatteryInfoStub->stubSetReturnValue("currentFlow", 500);
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);

    /* After this call LowBatteryNotifier should be destroyed */
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* State OK should stop notifications */
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryLow);
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryOk);
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* State Full should stop notifications */
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryLow);
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryFull);
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);
}

void Ut_BatteryNotifier::testWhenChargingStopsThenNotificationRemoved()
{
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charging"));

    batteryNotifier->applyChargingState(0, QBatteryInfo::UnknownChargingState);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
    QCOMPARE(batteryNotifier->notificationTimer.isActive(), false);
}

void Ut_BatteryNotifier::testWhenChargingStopsWhenConnectedToWallChargerThenNotificationRemoved()
{
    batteryNotifier->applyChargerType(QBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);
    batteryNotifier->applyChargerType(QBatteryInfo::UnknownCharger);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);

    batteryNotifier->applyChargerType(QBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryFull);
    batteryNotifier->applyChargerType(QBatteryInfo::UnknownCharger);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 4);
}

void Ut_BatteryNotifier::testWhenChargingStopsMoreThanNSecondAfterBeingStartedThenNotificationNotRemoved()
{
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);
    batteryNotifier->notificationTimer.stop();
    batteryNotifier->applyChargingState(0, QBatteryInfo::UnknownChargingState);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 0);
}

void Ut_BatteryNotifier::testWhenChargingStartsWhenRemoveChargerNotifiedThenNotificationRemoved()
{
    batteryNotifier->applyChargerType(QBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);
    batteryNotifier->applyChargerType(QBatteryInfo::UnknownCharger);
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 2);
}

void Ut_BatteryNotifier::testWhenChargingStopsAndBatteryIsLowNotifierIsCreated()
{
    gQBatteryInfoStub->stubSetReturnValue<QBatteryInfo::BatteryStatus>("batteryStatus", QBatteryInfo::BatteryLow);
    batteryNotifier->applyChargerType(QBatteryInfo::WallCharger);
    batteryNotifier->applyChargerType(QBatteryInfo::UnknownCharger);
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
}

void Ut_BatteryNotifier::testWhenStateChargingLowBatteryNotificationRemoved()
{
    batteryNotifier->lowBatteryAlert();
    batteryNotifier->applyChargerType(QBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testWhenBatteryFullWhenChargingNotifiedThenNotificationRemoved()
{
    batteryNotifier->applyChargerType(QBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(0, QBatteryInfo::Charging);
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryFull);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testSetTouchScreenLockActive()
{
    batteryNotifier->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 0);

    gQBatteryInfoStub->stubSetReturnValue("chargerType", QBatteryInfo::USBCharger);
    gQBatteryInfoStub->stubSetReturnValue<QBatteryInfo::ChargingState>("chargingState", QBatteryInfo::UnknownChargingState);
    batteryNotifier->applyBatteryStatus(0, QBatteryInfo::BatteryLow);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    batteryNotifier->setTouchScreenLockActive(false);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 2);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), false);

    batteryNotifier->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 3);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
}

QTEST_MAIN(Ut_BatteryNotifier)
