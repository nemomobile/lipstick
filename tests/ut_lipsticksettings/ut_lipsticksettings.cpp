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
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <mce/mode-names.h>
#include "ut_lipsticksettings.h"
#include "lipsticksettings.h"
#include "homeapplication.h"

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

    // Externally making lock screen visible should not lock the screen
    settings.setLockscreenVisible(true, true);
    QCOMPARE(qDBusConnectionAsyncCallService.isEmpty(), true);
    settings.setLockscreenVisible(false, true);

    // Internally making lock screen visible should start the timer
    settings.setLockscreenVisible(true, false);
    QCOMPARE(qDBusConnectionAsyncCallService, QString("com.nokia.mce"));
    QCOMPARE(qDBusConnectionAsyncCallPath, QString("/com/nokia/mce/request"));
    QCOMPARE(qDBusConnectionAsyncCallInterface, QString("com.nokia.mce.request"));
    QCOMPARE(qDBusConnectionAsyncCallMember, QString("req_tklock_mode_change"));
    QCOMPARE(qDBusConnectionAsyncCallArguments.count(), 1);
    QCOMPARE(qDBusConnectionAsyncCallArguments.last(), QVariant(MCE_TK_LOCKED_DELAY));

    // Reset the state
    qDBusConnectionAsyncCallService.clear();

    // Externally making lock screen invisible should not lock the screen
    settings.setLockscreenVisible(false, true);
    QCOMPARE(qDBusConnectionAsyncCallService.isEmpty(), true);

    // Reset the state
    settings.setLockscreenVisible(true, false);
    qDBusConnectionAsyncCallService.clear();

    // Internally making lock screen invisible should not lock the screen
    settings.setLockscreenVisible(false, false);
    QCOMPARE(qDBusConnectionAsyncCallService.isEmpty(), true);
}

QTEST_MAIN(Ut_LipstickSettings)
