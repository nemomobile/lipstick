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
#include "qsystembatteryinfo_stub.h"
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
    gQSystemBatteryInfoStub->stubReset();
}

void Ut_BatteryNotifier::testInitBattery()
{
    gQSystemBatteryInfoStub->stubSetReturnValue<QtMobility::QSystemBatteryInfo::ChargingState>("chargingState", QtMobility::QSystemBatteryInfo::NotCharging);
    gQSystemBatteryInfoStub->stubSetReturnValue<QtMobility::QSystemBatteryInfo::BatteryStatus>("batteryStatus", QtMobility::QSystemBatteryInfo::BatteryOk);

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

    gQSystemBatteryInfoStub->stubReset();
    gQmLEDStub->stubReset();

    /* StateFull */
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryFull);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.chargingcomplete"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charcomp"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());
    QCOMPARE(gQmLEDStub->stubLastCallTo("activate").parameter<QString>(0), QString("PatternBatteryFull"));

    /* StateOK */
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryOk);

    /* no notifications should be published, just silently no-op */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);

    /* StateEmpty */
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryEmpty);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.recharge"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_rebatt"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* StateError */
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryUnknown);

    /* no notifications should be published, just silently no-op */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);

    /* StateLow and charging */
    gQSystemBatteryInfoStub->stubSetReturnValue<QtMobility::QSystemBatteryInfo::ChargingState>("chargingState", QtMobility::QSystemBatteryInfo::Charging);
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryLow);

    /* no notifications should be published, because battery is charging... */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);

    /* StateLow and not charging */
    gQSystemBatteryInfoStub->stubSetReturnValue<QtMobility::QSystemBatteryInfo::ChargingState>("chargingState", QtMobility::QSystemBatteryInfo::NotCharging);
    batteryNotifier->setTouchScreenLockActive(true);
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryLow);

    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);
}

void Ut_BatteryNotifier::testChargingStateChanged()
{
    QList<QVariant> arguments;

    gQSystemBatteryInfoStub->stubReset();
    gQmLEDStub->stubReset();

    gQSystemBatteryInfoStub->stubSetReturnValue("chargerType", QtMobility::QSystemBatteryInfo::WallCharger);

    for(int i = 0; i <= 100; i += 5) {
        /* StateCharging */
        gQSystemBatteryInfoStub->stubSetReturnValue<int>("remainingCapacityPercent", i);
        batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);

        QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
        QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery"));
        QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charging"));
        QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), batteryNotifier->chargingImageId());
        QCOMPARE(gQmLEDStub->stubLastCallTo("activate").parameter<QString>(0), QString("PatternBatteryCharging"));

        gNotificationManagerStub->stubReset();
    }

    /* StateNotCharging */
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::NotCharging);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);
    QCOMPARE(gQmLEDStub->stubLastCallTo("deactivate").parameter<QString>(0), QString("PatternBatteryCharging"));

    /* StateChargingFailed */
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::ChargingError);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.chargingnotstarted"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_repcharger"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* Test "not enough power to charge" situation... */
    gQSystemBatteryInfoStub->stubSetReturnValue("chargerType", QtMobility::QSystemBatteryInfo::USB_100mACharger);
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.notenoughpower"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_nopowcharge"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString("icon-m-energy-management-insufficient-power"));
}

void Ut_BatteryNotifier::testBatteryChargerEvent()
{
    QList<QVariant> arguments;

    /* Wall charger */
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::WallCharger);
    QCOMPARE(batteryNotifier->chargerType, QtMobility::QSystemBatteryInfo::WallCharger);

    /* Plug out : charger type = none */
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::NoCharger);
    QCOMPARE(batteryNotifier->chargerType, QtMobility::QSystemBatteryInfo::NoCharger);

    /* Look for the notification: "Disconnect the charger from..." */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.removecharger"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_remcha"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    /* USB 500mA */
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::USB_500mACharger);
    QCOMPARE(batteryNotifier->chargerType, QtMobility::QSystemBatteryInfo::USB_500mACharger);

    /* USB 100mA */
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::USB_100mACharger);
    QCOMPARE(batteryNotifier->chargerType, QtMobility::QSystemBatteryInfo::USB_100mACharger);

    /* Unknown */
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::UnknownCharger);
    QCOMPARE(batteryNotifier->chargerType, QtMobility::QSystemBatteryInfo::UnknownCharger);
}

void Ut_BatteryNotifier::testPSMStateChanged()
{
    QList<QVariant> arguments;

    /* Entering to power-save mode */
    batteryNotifier->applyPSMState(MeeGo::QmDeviceMode::PSMStateOn);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.enterpsm"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_ent_psnote"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), batteryNotifier->chargingImageId());

    /* Exiting from power-save mode */
    batteryNotifier->applyPSMState(MeeGo::QmDeviceMode::PSMStateOff);

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.exitpsm"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_exit_psnote"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), batteryNotifier->chargingImageId());
}

void Ut_BatteryNotifier::testLowBatteryNotifierConnection()
{
    QList<QVariant> arguments;

    gQSystemBatteryInfoStub->stubSetReturnValue("chargerType", QtMobility::QSystemBatteryInfo::USB_500mACharger);

    /* LowBatteryNotifier shouldn't be instantiated at first */
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* Simulate battery-state-low change */
    gQSystemBatteryInfoStub->stubSetReturnValue<QtMobility::QSystemBatteryInfo::ChargingState>("chargingState", QtMobility::QSystemBatteryInfo::NotCharging);
    batteryNotifier->setTouchScreenLockActive(true);
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryLow);

    /* LowBatteryNotifier should be exists now... */
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    /* And should send a low-battery notification */
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);

    /* Simulate now a charging event */
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);

    /* After this call LowBatteryNotifier should be destroyed */
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* State OK should stop notifications */
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryLow);
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryOk);
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* State Full should stop notifications */
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryLow);
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryFull);
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);
}

void Ut_BatteryNotifier::testWhenChargingStopsThenNotificationRemoved()
{
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_ener_charging"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), batteryNotifier->chargingImageId());

    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::NotCharging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
    QCOMPARE(batteryNotifier->notificationTimer.isActive(), false);
}

void Ut_BatteryNotifier::testWhenChargingStopsWhenConnectedToWallChargerThenNotificationRemoved()
{
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::NoCharger);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);

    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryFull);
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::NoCharger);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 4);
}

void Ut_BatteryNotifier::testWhenChargingStopsMoreThanNSecondAfterBeingStartedThenNotificationNotRemoved()
{
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);
    batteryNotifier->notificationTimer.stop();
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::NotCharging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 0);
}

void Ut_BatteryNotifier::testWhenChargingStartsWhenRemoveChargerNotifiedThenNotificationRemoved()
{
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::NoCharger);
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 2);
}

void Ut_BatteryNotifier::testWhenChargingStopsAndBatteryIsLowNotifierIsCreated()
{
    gQSystemBatteryInfoStub->stubSetReturnValue<QtMobility::QSystemBatteryInfo::BatteryStatus>("batteryStatus", QtMobility::QSystemBatteryInfo::BatteryLow);
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::WallCharger);
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::NoCharger);
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
}

void Ut_BatteryNotifier::testWhenStateChargingLowBatteryNotificationRemoved()
{
    batteryNotifier->lowBatteryAlert();
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testWhenBatteryFullWhenChargingNotifiedThenNotificationRemoved()
{
    batteryNotifier->applyChargerType(QtMobility::QSystemBatteryInfo::WallCharger);
    batteryNotifier->applyChargingState(QtMobility::QSystemBatteryInfo::Charging);
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryFull);
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testSetTouchScreenLockActive()
{
    batteryNotifier->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 0);

    gQSystemBatteryInfoStub->stubSetReturnValue("chargerType", QtMobility::QSystemBatteryInfo::USB_500mACharger);
    gQSystemBatteryInfoStub->stubSetReturnValue<QtMobility::QSystemBatteryInfo::ChargingState>("chargingState", QtMobility::QSystemBatteryInfo::NotCharging);
    batteryNotifier->applyBatteryStatus(QtMobility::QSystemBatteryInfo::BatteryLow);
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
