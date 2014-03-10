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
#include "thermalnotifier.h"
#include "homeapplication.h"
#include "ut_thermalnotifier.h"
#include "notificationmanager_stub.h"

HomeApplication::~HomeApplication()
{
}

HomeApplication *HomeApplication::instance()
{
    return 0;
}

void HomeApplication::restoreSignalHandlers()
{
}

void HomeApplication::sendStartupNotifications()
{
}

int argc = 1;
char *argv[] = { (char *) "./ut_thermalnotifier", NULL };

void Ut_ThermalNotifier::initTestCase()
{
}

void Ut_ThermalNotifier::cleanupTestCase()
{
}

void Ut_ThermalNotifier::init()
{
    thermalNotifier = new ThermalNotifier;

    gNotificationManagerStub->stubReset();
    gNotificationManagerStub->stubSetReturnValue("Notify", (uint)1);
}

void Ut_ThermalNotifier::cleanup()
{
    delete thermalNotifier;
    gNotificationManagerStub->stubReset();
}

void Ut_ThermalNotifier::testThermalState()
{
    thermalNotifier->applyThermalState(MeeGo::QmThermal::Warning);
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.temperature"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_shut_high_temp_warning"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    thermalNotifier->applyThermalState(MeeGo::QmThermal::Alert);
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.temperature"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_shut_high_temp_alert"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());

    thermalNotifier->applyThermalState(MeeGo::QmThermal::LowTemperatureWarning);
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 3);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), QString("x-nemo.battery.temperature"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_shut_low_temp_warning"));
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());
}

QTEST_MAIN (Ut_ThermalNotifier)
