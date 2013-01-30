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
#include "qmled_stub.h"
#include "qmbattery_stub.h"
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
    gQmBatteryStub->stubReset();
}

void Ut_BatteryNotifier::testInitBattery()
{
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::BatteryState>("getBatteryState", MeeGo::QmBattery::StateOK);

    // no notification should be shown and battery charging pattern should be deactivated
    batteryNotifier->initBattery();

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);
    QCOMPARE(gQmLEDStub->stubLastCallTo("deactivate").parameter<QString>(0), QString("PatternBatteryCharging"));
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

    gQmBatteryStub->stubReset();
    gQmLEDStub->stubReset();

    /* StateFull */
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateFull);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.chargingcomplete"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charcomp"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());
    QCOMPARE(gQmLEDStub->stubLastCallTo("activate").parameter<QString>(0), QString("PatternBatteryFull"));

    /* StateOK */
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateOK);

    /* no notifications should be published, just silently no-op */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);

    /* StateEmpty */
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateEmpty);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.recharge"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_rebatt"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* StateError */
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateError);

    /* no notifications should be published, just silently no-op */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);

    /* StateLow and charging */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateCharging);
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateLow);

    /* no notifications should be published, because battery is charging... */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);

    /* StateLow and not charging */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    batteryNotifier->setTouchScreenLockActive(true);
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateLow);

    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);
}

void Ut_BatteryNotifier::testChargingStateChanged()
{
    QList<QVariant> arguments;

    gQmBatteryStub->stubReset();
    gQmLEDStub->stubReset();

    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::Wall);

    for(int i = 0; i <= 100; i += 5) {
        /* StateCharging */
        gQmBatteryStub->stubSetReturnValue<int>("getRemainingCapacityPct", i);
        batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);

        QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
        QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery"));
        QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charging"));
        QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), batteryNotifier->chargingImageId());
        QCOMPARE(gQmLEDStub->stubLastCallTo("activate").parameter<QString>(0), QString("PatternBatteryCharging"));

        gNotificationManagerStub->stubReset();
    }

    /* StateNotCharging */
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);
    QCOMPARE(gQmLEDStub->stubLastCallTo("deactivate").parameter<QString>(0), QString("PatternBatteryCharging"));

    /* StateChargingFailed */
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateChargingFailed);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.chargingnotstarted"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_repcharger"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* Test "not enough power to charge" situation... */
    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_100mA);
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.notenoughpower"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_nopowcharge"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString("icon-m-energy-management-insufficient-power"));
}

void Ut_BatteryNotifier::testBatteryChargerEvent()
{
    QList<QVariant> arguments;

    /* Wall charger */
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::Wall);
    QCOMPARE(batteryNotifier->chargerType, MeeGo::QmBattery::Wall);

    /* Plug out : charger type = none */
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::None);
    QCOMPARE(batteryNotifier->chargerType, MeeGo::QmBattery::None);

    /* Look for the notification: "Disconnect the charger from..." */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.removecharger"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_remcha"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* USB 500mA */
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::USB_500mA);
    QCOMPARE(batteryNotifier->chargerType, MeeGo::QmBattery::USB_500mA);

    /* USB 100mA */
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::USB_100mA);
    QCOMPARE(batteryNotifier->chargerType, MeeGo::QmBattery::USB_100mA);

    /* Unknown */
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::Unknown);
    QCOMPARE(batteryNotifier->chargerType, MeeGo::QmBattery::Unknown);
}

void Ut_BatteryNotifier::testPSMStateChanged()
{
    QList<QVariant> arguments;

    /* Entering to power-save mode */
    batteryNotifier->devicePSMStateChanged(MeeGo::QmDeviceMode::PSMStateOn);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.enterpsm"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_ent_psnote"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), batteryNotifier->chargingImageId());

    /* Exiting from power-save mode */
    batteryNotifier->devicePSMStateChanged(MeeGo::QmDeviceMode::PSMStateOff);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.exitpsm"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_exit_psnote"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), batteryNotifier->chargingImageId());
}

void Ut_BatteryNotifier::testLowBatteryNotifierConnection()
{
    QList<QVariant> arguments;

    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_500mA);

    /* LowBatteryNotifier shouldn't be instantiated at first */
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* Simulate battery-state-low change */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    batteryNotifier->setTouchScreenLockActive(true);
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateLow);

    /* LowBatteryNotifier should be exists now... */
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    /* And should send a low-battery notification */
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);

    /* Simulate now a charging event */
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);

    /* After this call LowBatteryNotifier should be destroyed */
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* State OK should stop notifications */
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateLow);
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateOK);
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* State Full should stop notifications */
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateLow);
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateFull);
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);
}

void Ut_BatteryNotifier::testWhenChargingStopsThenNotificationRemoved()
{
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charging"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), batteryNotifier->chargingImageId());

    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
    QCOMPARE(batteryNotifier->notificationTimer.isActive(), false);
}

void Ut_BatteryNotifier::testWhenChargingStopsWhenConnectedToWallChargerThenNotificationRemoved()
{
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::Wall);
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::None);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);

    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::Wall);
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateFull);
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::None);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 4);
}

void Ut_BatteryNotifier::testWhenChargingStopsMoreThanNSecondAfterBeingStartedThenNotificationNotRemoved()
{
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    batteryNotifier->notificationTimer.stop();
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 0);
}

void Ut_BatteryNotifier::testWhenChargingStartsWhenRemoveChargerNotifiedThenNotificationRemoved()
{
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::Wall);
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::None);
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 2);
}

void Ut_BatteryNotifier::testWhenChargingStopsAndBatteryIsLowNotifierIsCreated()
{
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::BatteryState>("getBatteryState", MeeGo::QmBattery::StateLow);
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::Wall);
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::None);
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
}

void Ut_BatteryNotifier::testWhenStateChargingLowBatteryNotificationRemoved()
{
    batteryNotifier->lowBatteryAlert();
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::Wall);
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testWhenBatteryFullWhenChargingNotifiedThenNotificationRemoved()
{
    batteryNotifier->batteryChargerEvent(MeeGo::QmBattery::Wall);
    batteryNotifier->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateFull);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testSetTouchScreenLockActive()
{
    batteryNotifier->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 0);

    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_500mA);
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    batteryNotifier->batteryStateChanged(MeeGo::QmBattery::StateLow);
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
