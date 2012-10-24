/***************************************************************************
**
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
#include <QTimer>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include "ut_lipsticksettings.h"
#include "lipsticksettings.h"
#include "homeapplication.h"

int qTimerStartMsec = -1;
void QTimer::start(int msec)
{
    qTimerStartMsec = msec;
    id = 1;
}

void QTimer::stop()
{
    qTimerStartMsec = -1;
    id = -1;
}

HomeApplication *HomeApplication::instance()
{
    return 0;
}

QDeclarativeView *homeApplicationMainWindowInstance = 0;
QDeclarativeView *HomeApplication::mainWindowInstance()
{
    return homeApplicationMainWindowInstance;
}

QString qDBusConnectionAsyncCallService;
QString qDBusConnectionAsyncCallPath;
QString qDBusConnectionAsyncCallInterface;
QString qDBusConnectionAsyncCallMember;
QVariantList qDBusConnectionAsyncCallArguments;
QDBusPendingCall QDBusConnection::asyncCall(const QDBusMessage &message, int) const
{
    qDBusConnectionAsyncCallService = message.service();
    qDBusConnectionAsyncCallPath = message.path();
    qDBusConnectionAsyncCallInterface = message.interface();
    qDBusConnectionAsyncCallMember = message.member();
    qDBusConnectionAsyncCallArguments = message.arguments();
    return QDBusPendingCall::fromCompletedCall(QDBusMessage());
}

void Ut_LipstickSettings::initTestCase()
{
    homeApplicationMainWindowInstance = new QDeclarativeView;
}

void Ut_LipstickSettings::cleanupTestCase()
{
    delete homeApplicationMainWindowInstance;
}

void Ut_LipstickSettings::testSetLockScreenVisible()
{
    LipstickSettings settings;

    // Externally making lock screen visible should not start the timer
    settings.setLockscreenVisible(true, true);
    QCOMPARE(qTimerStartMsec, -1);
    settings.setLockscreenVisible(false, true);

    // Internally making lock screen visible should start the timer
    settings.setLockscreenVisible(true, false);
    QCOMPARE(qTimerStartMsec, 5000);

    // Externally making lock screen invisible should stop the timer
    settings.setLockscreenVisible(false, true);
    QCOMPARE(qTimerStartMsec, -1);

    // Internally making lock screen invisible should stop the timer
    settings.setLockscreenVisible(true, false);
    settings.setLockscreenVisible(false, false);
    QCOMPARE(qTimerStartMsec, -1);

    // Making the lock screen visible twice should not start the timer twice
    settings.setLockscreenVisible(true, false);
    qTimerStartMsec = -1;
    settings.setLockscreenVisible(true, false);
    QCOMPARE(qTimerStartMsec, -1);
}

void Ut_LipstickSettings::testRequestScreenToBeLocked()
{
    LipstickSettings settings;
    connect(this, SIGNAL(timeout()), &settings, SLOT(requestScreenToBeLocked()));

    emit timeout();

    QCOMPARE(qDBusConnectionAsyncCallService, QString("com.nokia.mce"));
    QCOMPARE(qDBusConnectionAsyncCallPath, QString("/com/nokia/mce/request"));
    QCOMPARE(qDBusConnectionAsyncCallInterface, QString("com.nokia.mce.request"));
    QCOMPARE(qDBusConnectionAsyncCallMember, QString("req_tklock_mode_change"));
    QCOMPARE(qDBusConnectionAsyncCallArguments.count(), 1);
    QCOMPARE(qDBusConnectionAsyncCallArguments.last(), QVariant("locked"));
}

QTEST_MAIN(Ut_LipstickSettings)
