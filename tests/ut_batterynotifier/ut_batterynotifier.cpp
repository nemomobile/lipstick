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
#include "qmdisplaystate_stub.h"
#include "batterynotifier.h"
#include "ut_batterynotifier.h"

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty_stub.h"
#endif

static void setStubProperty(QString const &name, QString const &value)
{
    getContextPropertyStub(name)->stubSetReturnValue("value", QVariant(value));
}

void Ut_BatteryNotifier::setNewStubState
(QString const &level, QString const &state, QString const &charger)
{
    if (!level.isNull()) setStubProperty("Battery.Level", level);
    if (!state.isNull()) setStubProperty("Battery.ChargingState", state);
    if (!charger.isNull()) setStubProperty("Battery.ChargerType", charger);
    batteryNotifier->prepareNotification();
}

static QString getNotificationHint(QString const &h)
{
    return gNotificationManagerStub->stubLastCallTo("Notify")
        .parameter<QVariantHash>(6).value(h).toString();
}

static QString getNotificationCategory()
{
    return getNotificationHint(NotificationManager::HINT_CATEGORY);
}

static QString getNotificationPreviewBody()
{
    return getNotificationHint(NotificationManager::HINT_PREVIEW_BODY);
}

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
#ifdef HAVE_CONTEXTSUBSCRIBER
    setStubProperty("Phone.Call", "inactive");
#endif
    batteryNotifier.reset(new BatteryNotifier);

    gNotificationManagerStub->stubReset();
    gNotificationManagerStub->stubSetReturnValue("Notify", (uint)1);
}

void Ut_BatteryNotifier::cleanup()
{
    gNotificationManagerStub->stubReset();
    gLowBatteryNotifierStub->stubReset();
}

void Ut_BatteryNotifier::testInitBattery()
{
    batteryNotifier->prepareNotification();
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);
}

void Ut_BatteryNotifier::testLowBatteryAlert()
{
    batteryNotifier->lowBatteryAlert();

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(getNotificationCategory(), QString("x-nemo.battery.lowbattery"));
    QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_lowbatt"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());
}

void Ut_BatteryNotifier::testBatteryStateChanged()
{
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);
    setStubProperty("Battery.ChargingState", "idle");
    batteryNotifier->prepareNotification();

    /* StateFull, full */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(getNotificationCategory(), QString("x-nemo.battery.chargingcomplete"));
    QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_charcomp"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());
    gNotificationManagerStub->stubReset();

    /* StateOK */
    setStubProperty("Battery.ChargingState", "discharging");
    batteryNotifier->prepareNotification();
    /* no notifications should be published, just silently no-op */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);

    /* StateEmpty */
    setStubProperty("Battery.Level", "empty");
    batteryNotifier->prepareNotification();
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(getNotificationCategory(), QString("x-nemo.battery.recharge"));
    QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_rebatt"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());
    gNotificationManagerStub->stubReset();

    /* StateError */
    setStubProperty("Battery.Level", "unknown");
    batteryNotifier->prepareNotification();
    /* no notifications should be published, just silently no-op */
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);

    /* StateLow and charging */
    setNewStubState("low", "charging", "dcp");
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_charging"));
    gNotificationManagerStub->stubReset();

    /* StateLow and not charging */
    batteryNotifier->setTouchScreenLockActive(true);
    setStubProperty("Battery.ChargingState", "discharging");
    batteryNotifier->prepareNotification();

    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);
}

void Ut_BatteryNotifier::testChargingStateChanged()
{
    setNewStubState("unknown", "discharging", "");
    gNotificationManagerStub->stubReset();

    static const QString levels[] = { "normal", "low", "empty" };
    for(size_t i = 0; i < sizeof(levels)/sizeof(levels[0]); ++i) {
        /* StateCharging */
        setNewStubState(levels[i], "charging", "dcp");
        QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
        QCOMPARE(getNotificationCategory(), QString("x-nemo.battery"));
        QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_charging"));
        gNotificationManagerStub->stubReset();

        setNewStubState("unknown", "discharging", "");
        QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
        QCOMPARE(getNotificationCategory(), QString("x-nemo.battery.removecharger"));
        QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_remcha"));
        gNotificationManagerStub->stubReset();
    }

    for(size_t i = 0; i < sizeof(levels)/sizeof(levels[0]); ++i) {
        /* StateCharging */
        setNewStubState(levels[i], "charging", "usb");
        QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
        QCOMPARE(getNotificationCategory(), QString("x-nemo.battery"));
        QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_charging"));
        gNotificationManagerStub->stubReset();

        setNewStubState("unknown", "discharging", "");
        QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);
    }

    /* StateUnknownChargingState */
    setNewStubState(QString(), "unknown", "");
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);
}

void Ut_BatteryNotifier::testPSMStateChanged()
{
    setStubProperty("System.PowerSaveMode", "0");
    batteryNotifier->onPowerSaveModeChanged();
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);

    /* Entering to power-save mode */
    setStubProperty("System.PowerSaveMode", "1");
    batteryNotifier->onPowerSaveModeChanged();

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(getNotificationCategory(), QString("x-nemo.battery.enterpsm"));
    QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_ent_psnote"));

    /* Entering to power-save mode */
    setStubProperty("System.PowerSaveMode", "0");
    batteryNotifier->onPowerSaveModeChanged();

    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(getNotificationCategory(), QString("x-nemo.battery.exitpsm"));
    QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_exit_psnote"));
}

void Ut_BatteryNotifier::testLowBatteryNotifierConnection()
{
    setNewStubState("normal", "charging", "usb");

    /* LowBatteryNotifier shouldn't be instantiated at first */
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* Simulate battery-state-low change */
    batteryNotifier->setTouchScreenLockActive(true);
    setNewStubState("low", "discharging", "");

    /* LowBatteryNotifier should be exists now... */
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    /* And should send a low-battery notification */
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);

    /* Simulate now a charging event */
    setNewStubState(QString(), "charging", "usb");

    /* After this call LowBatteryNotifier should be destroyed */
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* State OK should stop notifications */
    setNewStubState("low", "discharging", "");
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 2);
    setStubProperty("Battery.Level", "normal");
    batteryNotifier->prepareNotification();
    QCOMPARE(batteryNotifier->lowBatteryNotifier, (LowBatteryNotifier *)NULL);
}

void Ut_BatteryNotifier::testWhenChargingStopsThenNotificationRemoved()
{
    setNewStubState("normal", "unknown", "");
    gNotificationManagerStub->stubReset();

    setNewStubState("normal", "charging", "usb");
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(getNotificationCategory(), QString("x-nemo.battery"));
    QCOMPARE(getNotificationPreviewBody(), qtTrId("qtn_ener_charging"));

    setNewStubState("normal", "discharging", "");

    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testWhenChargingStopsWhenConnectedToWallChargerThenNotificationRemoved()
{
    setNewStubState("normal", "unknown", "");
    gNotificationManagerStub->stubReset();

    setNewStubState(QString(), "charging", "dcp");
    setNewStubState(QString(), "discharging", "");
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);

    setNewStubState(QString(), "charging", "dcp");
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 2);
}

void Ut_BatteryNotifier::testWhenChargingStopsAndBatteryIsLowNotifierIsCreated()
{
    setNewStubState("low", "charging", "dcp");
    setNewStubState(QString(), "discharging", "");
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
}

void Ut_BatteryNotifier::testWhenStateChargingLowBatteryNotificationRemoved()
{
    setNewStubState("low", "discharging", "");
    QVERIFY(batteryNotifier->lowBatteryNotifier != NULL);
    gNotificationManagerStub->stubReset();
    batteryNotifier->notifications.clear();
    batteryNotifier->lowBatteryAlert();
    setNewStubState("low", "charging", "usb");
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testWhenBatteryFullWhenChargingNotifiedThenNotificationRemoved()
{
    setNewStubState("normal", "discharging", "");
    setNewStubState("normal", "charging", "usb");
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 0);
    setNewStubState("normal", "idle", "usb");
    QCOMPARE(gNotificationManagerStub->stubCallCount("CloseNotification"), 1);
}

void Ut_BatteryNotifier::testSetTouchScreenLockActive()
{
    setNewStubState("normal", "discharging", "");
    batteryNotifier->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 0);

    setNewStubState("low", QString(), QString());

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
