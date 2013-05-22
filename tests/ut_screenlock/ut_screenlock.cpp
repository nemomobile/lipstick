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
#include <QDBusAbstractInterface>

#define TEST_SERVICE   QString("com.nokia.mcetest")
#define TEST_PATH      QString("/com/nokia/mcetest")
#define TEST_INTERFACE QString("com.nokia.mcetest")
#define TEST_METHOD    QString("testmethod")

#include "ut_screenlock.h"
#include "screenlock.h"
#include "homeapplication.h"
#include "closeeventeater_stub.h"

QDBus::CallMode qDbusAbstractInterfaceCallMode;
QVariant qDbusAbstractInterfaceCallArg1;
QString qDbusAbstractInterfaceCallMethod;
QString qDbusAbstractInterfaceCallPath;
QString qDbusAbstractInterfaceCallService;
QString qDbusAbstractInterfaceCallInterface;
QDBusMessage QDBusAbstractInterface::call(QDBus::CallMode mode, const QString & method, const QVariant & arg1, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &)
{
    qDbusAbstractInterfaceCallMode = mode;
    qDbusAbstractInterfaceCallArg1 = arg1;
    qDbusAbstractInterfaceCallMethod = method;
    qDbusAbstractInterfaceCallPath = path();
    qDbusAbstractInterfaceCallService = service();
    qDbusAbstractInterfaceCallInterface = interface();

    return QDBusMessage();
}

void QTimer::singleShot(int, const QObject *receiver, const char *member)
{
    // The "member" string is of form "1member()", so remove the trailing 1 and the ()
    int memberLength = strlen(member) - 3;
    char modifiedMember[memberLength + 1];
    strncpy(modifiedMember, member + 1, memberLength);
    modifiedMember[memberLength] = 0;
    QMetaObject::invokeMethod(const_cast<QObject *>(receiver), modifiedMember, Qt::DirectConnection);
}

void Ut_ScreenLock::init()
{
    screenLock = new ScreenLock;
}

void Ut_ScreenLock::cleanup()
{
    delete screenLock;
    qDbusAbstractInterfaceCallArg1.clear();
    qDbusAbstractInterfaceCallMethod.clear();
    qDbusAbstractInterfaceCallPath.clear();
    qDbusAbstractInterfaceCallService.clear();
    qDbusAbstractInterfaceCallInterface.clear();
}

void Ut_ScreenLock::initTestCase()
{
}

void Ut_ScreenLock::cleanupTestCase()
{
}

void testStackingLayer(int callCount, long stackingLayer)
{
    Q_UNUSED(callCount)
    Q_UNUSED(stackingLayer)
}

void Ut_ScreenLock::testToggleScreenLockUI()
{
    QSignalSpy spy(screenLock, SIGNAL(screenIsLocked(bool)));

    // When the lock is toggled on, make sure the lock UI is shown
    screenLock->toggleScreenLockUI(true);

    // The title should now be "Screen Lock"
//    QCOMPARE(HomeApplication::instance()->mainWindowInstance()->windowTitle(), QString("Screen Lock"));

    // The stacking layer should be 5
    testStackingLayer(1, 5);

    // Locked state should be set
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toBool(), true);
    QCOMPARE(screenLock->isScreenLocked(), true);

    // When the lock is toggled off, make sure the lock UI is hidden
    screenLock->toggleScreenLockUI(false);

    // The title should now be "Lipstick"
//    QCOMPARE(HomeApplication::instance()->mainWindowInstance()->windowTitle(), QString("Lipstick"));

    // The stacking layer should be 0
    testStackingLayer(2, 0);

    // Locked state should not be set
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.last().at(0).toBool(), false);
    QCOMPARE(screenLock->isScreenLocked(), false);
}

void Ut_ScreenLock::testUnlockScreenWhenLocked()
{
    screenLock->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLock::TkLockModeNone, false, false);
    screenLock->toggleScreenLockUI(true);
    screenLock->toggleEventEater(true);
    screenLock->unlockScreen();

    QCOMPARE(qDbusAbstractInterfaceCallMethod, TEST_METHOD);
    QCOMPARE(qDbusAbstractInterfaceCallPath, TEST_PATH);
    QCOMPARE(qDbusAbstractInterfaceCallService, TEST_SERVICE);
    QCOMPARE(qDbusAbstractInterfaceCallInterface, TEST_INTERFACE);
    QCOMPARE(qDbusAbstractInterfaceCallMode, QDBus::NoBlock);
    QCOMPARE(qDbusAbstractInterfaceCallArg1.toInt(), (int)ScreenLock::TkLockUnlock);
}

void Ut_ScreenLock::testUnlockScreenWhenNotLocked()
{
    screenLock->unlockScreen();

    QCOMPARE(qDbusAbstractInterfaceCallMethod.isEmpty(), true);
    QCOMPARE(qDbusAbstractInterfaceCallPath.isEmpty(), true);
    QCOMPARE(qDbusAbstractInterfaceCallService.isEmpty(), true);
    QCOMPARE(qDbusAbstractInterfaceCallInterface.isEmpty(), true);
}

void Ut_ScreenLock::testTkLockOpen_data()
{
    QTest::addColumn<int>("mode");
    QTest::addColumn<bool>("mainWindowModified");
    QTest::addColumn<QString>("mainWindowTitle");
    QTest::addColumn<int>("mainWindowStackingLayer");
    QTest::addColumn<bool>("eventEaterWindowVisibilityModified");
    QTest::addColumn<bool>("eventEaterWindowVisible");

    QTest::newRow("TkLockModeNone") << (int)ScreenLock::TkLockModeNone << false << "" << 0 << false << false;
    QTest::newRow("TkLockModeEnable") << (int)ScreenLock::TkLockModeEnable << true << "Screen Lock" << 5 << true << false;
    QTest::newRow("TkLockModeHelp") << (int)ScreenLock::TkLockModeHelp << false << "" << 0 << false << false;
    QTest::newRow("TkLockModeSelect") << (int)ScreenLock::TkLockModeSelect << false << "" << 0 << false << false;
    QTest::newRow("TkLockModeOneInput") << (int)ScreenLock::TkLockModeOneInput << false << "" << 0 << true << true;
    QTest::newRow("TkLockEnableVisual") << (int)ScreenLock::TkLockEnableVisual << true << "Screen Lock" << 5 << true << false;
    QTest::newRow("TkLockEnableLowPowerMode") << (int)ScreenLock::TkLockEnableLowPowerMode << true << "Screen Lock" << 5 << true << false;
    QTest::newRow("TkLockRealBlankMode") << (int)ScreenLock::TkLockRealBlankMode << true << "Screen Lock" << 5 << true << false;
}

void Ut_ScreenLock::testTkLockOpen()
{
    QFETCH(int, mode);
    QFETCH(bool, mainWindowModified);
    QFETCH(QString, mainWindowTitle);
    QFETCH(int, mainWindowStackingLayer);
    QFETCH(bool, eventEaterWindowVisibilityModified);
    QFETCH(bool, eventEaterWindowVisible);

    Q_UNUSED(mainWindowStackingLayer)
    // Make sure the event eater is visible so that it will be hidden if necessary
    screenLock->showEventEater();

    // Modify the state
    int reply = screenLock->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, mode, false, false);
    QCOMPARE(reply, (int)ScreenLock::TkLockReplyOk);

    // Check that main window title and stacking layer were only changed if needed (and to the correct state)
    if (mainWindowModified) {
//        QCOMPARE(HomeApplication::instance()->mainWindowInstance()->windowTitle(), mainWindowTitle);
        testStackingLayer(1, mainWindowStackingLayer);
    }

    Q_UNUSED(eventEaterWindowVisibilityModified)
    Q_UNUSED(eventEaterWindowVisible)
}

void Ut_ScreenLock::testTkLockClose()
{
    // Show the screen lock window and the event eater
    screenLock->showScreenLock();
    screenLock->showEventEater();

    // Modify the state
    int reply = screenLock->tklock_close(false);
    QCOMPARE(reply, (int)ScreenLock::TkLockReplyOk);

    // Both windows should be hidden
//    QCOMPARE(HomeApplication::instance()->mainWindowInstance()->windowTitle(), QString("Lipstick"));
    testStackingLayer(2, 0);
}

QTEST_MAIN(Ut_ScreenLock)
