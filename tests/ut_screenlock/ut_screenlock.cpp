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
#include <QMouseEvent>

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

void Ut_ScreenLock::testToggleScreenLockUI()
{
    QSignalSpy spy(screenLock, SIGNAL(screenIsLocked(bool)));

    // When the lock is toggled on, make sure the lock UI is shown
    screenLock->toggleScreenLockUI(true);

    // Locked state should be set
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toBool(), true);
    QCOMPARE(screenLock->isScreenLocked(), true);

    // When the lock is toggled off, make sure the lock UI is hidden
    screenLock->toggleScreenLockUI(false);

    // Locked state should not be set
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.last().at(0).toBool(), false);
    QCOMPARE(screenLock->isScreenLocked(), false);
}

void Ut_ScreenLock::testToggleEventEater()
{
    QMouseEvent event(QEvent::MouseButtonPress, QPointF(), Qt::NoButton, 0, 0);

    // Make sure the screen locking signals are sent and the eater UI is shown/hidden
    screenLock->toggleEventEater(true);
    QCOMPARE(screenLock->eventFilter(0, &event), true);

    screenLock->toggleEventEater(false);
    QCOMPARE(screenLock->eventFilter(0, &event), false);
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
    QTest::addColumn<bool>("screenIsLocked");
    QTest::addColumn<bool>("eventEaterWindowVisibilityModified");
    QTest::addColumn<bool>("eventEaterWindowVisible");

    QTest::newRow("TkLockModeNone") << (int)ScreenLock::TkLockModeNone << false << false << false << false;
    QTest::newRow("TkLockModeEnable") << (int)ScreenLock::TkLockModeEnable << true << true << true << false;
    QTest::newRow("TkLockModeHelp") << (int)ScreenLock::TkLockModeHelp << false << false << false << false;
    QTest::newRow("TkLockModeSelect") << (int)ScreenLock::TkLockModeSelect << false << false << false << false;
    QTest::newRow("TkLockModeOneInput") << (int)ScreenLock::TkLockModeOneInput << false << false << true << true;
    QTest::newRow("TkLockEnableVisual") << (int)ScreenLock::TkLockEnableVisual << true << true << true << false;
    QTest::newRow("TkLockEnableLowPowerMode") << (int)ScreenLock::TkLockEnableLowPowerMode << true << true << true << false;
    QTest::newRow("TkLockRealBlankMode") << (int)ScreenLock::TkLockRealBlankMode << true << true << true << false;
}

void Ut_ScreenLock::testTkLockOpen()
{
    QFETCH(int, mode);
    QFETCH(bool, mainWindowModified);
    QFETCH(bool, screenIsLocked);
    QFETCH(bool, eventEaterWindowVisibilityModified);
    QFETCH(bool, eventEaterWindowVisible);

    // Make sure the event eater is visible so that it will be hidden if necessary
    screenLock->showEventEater();

    // Modify the state
    QSignalSpy spy(screenLock, SIGNAL(screenIsLocked(bool)));
    int reply = screenLock->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, mode, false, false);
    QCOMPARE(reply, (int)ScreenLock::TkLockReplyOk);

    // Check that main window title and stacking layer were only changed if needed (and to the correct state)
    if (mainWindowModified) {
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.last().at(0).toBool(), screenIsLocked);
    }

    if (eventEaterWindowVisibilityModified) {
        QMouseEvent event(QEvent::MouseButtonPress, QPointF(), Qt::NoButton, 0, 0);
        QCOMPARE(screenLock->eventFilter(0, &event), eventEaterWindowVisible);
    }
}

void Ut_ScreenLock::testTkLockClose()
{
    // Show the screen lock window and the event eater
    screenLock->showScreenLock();
    screenLock->showEventEater();

    // Modify the state
    QSignalSpy spy(screenLock, SIGNAL(screenIsLocked(bool)));
    int reply = screenLock->tklock_close(false);
    QCOMPARE(reply, (int)ScreenLock::TkLockReplyOk);

    // The screen should no longer be locked
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toBool(), false);

    // Events should still be eaten
    QMouseEvent event(QEvent::MouseButtonPress, QPointF(), Qt::NoButton, 0, 0);
    QCOMPARE(screenLock->eventFilter(0, &event), true);
}

QTEST_MAIN(Ut_ScreenLock)
