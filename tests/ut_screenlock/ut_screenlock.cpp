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
#include "eventeater_stub.h"
#include "closeeventeater_stub.h"
#include "x11wrapper_modified_stub.h"

XEventListener::XEventListener()
{
}

XEventListener::~XEventListener()
{
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

void QTimer::singleShot(int, QObject *receiver, const char *member)
{
    // The "member" string is of form "1member()", so remove the trailing 1 and the ()
    int memberLength = strlen(member) - 3;
    char modifiedMember[memberLength + 1];
    strncpy(modifiedMember, member + 1, memberLength);
    modifiedMember[memberLength] = 0;
    QMetaObject::invokeMethod(receiver, modifiedMember, Qt::DirectConnection);
}

void Ut_ScreenLock::init()
{
    screenLock = new ScreenLock;
    gX11WrapperStub->stubSetReturnValue("XInternAtom", (Atom)1);
}

void Ut_ScreenLock::cleanup()
{
    delete screenLock;
    qDbusAbstractInterfaceCallArg1.clear();
    qDbusAbstractInterfaceCallMethod.clear();
    qDbusAbstractInterfaceCallPath.clear();
    qDbusAbstractInterfaceCallService.clear();
    qDbusAbstractInterfaceCallInterface.clear();
    gEventEaterStub->stubReset();
    gX11WrapperStub->stubReset();
}

void Ut_ScreenLock::initTestCase()
{
    homeApplicationMainWindowInstance = new QDeclarativeView;
}

void Ut_ScreenLock::cleanupTestCase()
{
    delete homeApplicationMainWindowInstance;
}

void testStackingLayer(int callCount, long stackingLayer)
{
    QCOMPARE(gX11WrapperStub->stubCallCount("XChangeProperty"), callCount);
    MethodCall &xcp = gX11WrapperStub->stubLastCallTo("XChangeProperty");

    QCOMPARE(xcp.parameter<Window>(1), homeApplicationMainWindowInstance->winId());
    QCOMPARE(xcp.parameter<Atom>(2), X11Wrapper::XInternAtom(0, "_MEEGO_STACKING_LAYER", False));
    QCOMPARE(xcp.parameter<Atom>(3), XA_CARDINAL);
    QCOMPARE(xcp.parameter<int>(4), 32);
    QCOMPARE(xcp.parameter<int>(5), PropModeReplace);
    QCOMPARE(*reinterpret_cast<long*>(xcp.parameter<QByteArray>(6).data()), stackingLayer);
    QCOMPARE(xcp.parameter<int>(7), 1);
}

void Ut_ScreenLock::testToggleScreenLockUI()
{
    QSignalSpy spy(screenLock, SIGNAL(screenIsLocked(bool)));

    // When the lock is toggled on, make sure the lock UI is shown
    screenLock->toggleScreenLockUI(true);

    // The title should now be "Screen Lock"
    QCOMPARE(HomeApplication::instance()->mainWindowInstance()->windowTitle(), QString("Screen Lock"));

    // The stacking layer should be 5
    testStackingLayer(1, 5);

    // Locked state should be set
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toBool(), true);
    QCOMPARE(screenLock->isScreenLocked(), true);

    // When the lock is toggled off, make sure the lock UI is hidden
    screenLock->toggleScreenLockUI(false);

    // The title should now be "Lipstick"
    QCOMPARE(HomeApplication::instance()->mainWindowInstance()->windowTitle(), QString("Lipstick"));

    // The stacking layer should be 0
    testStackingLayer(2, 0);

    // Locked state should not be set
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.last().at(0).toBool(), false);
    QCOMPARE(screenLock->isScreenLocked(), false);
}

void Ut_ScreenLock::testToggleEventEater()
{
    // Make sure the screen locking signals are sent and the eater UI is shown/hidden
    screenLock->toggleEventEater(true);
    QCOMPARE(gEventEaterStub->stubCallCount("show"), 1);

    screenLock->toggleEventEater(false);
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);
}

void Ut_ScreenLock::testUnlockScreenWhenLocked()
{
    screenLock->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLock::TkLockModeNone, false, false);
    screenLock->toggleScreenLockUI(true);
    screenLock->toggleEventEater(true);
    screenLock->unlockScreen();

    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);

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

void Ut_ScreenLock::testHideEventEater()
{
    screenLock->showEventEater();
    screenLock->hideEventEater();
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);
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
    gEventEaterStub->stubReset();

    // Modify the state
    int reply = screenLock->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, mode, false, false);
    QCOMPARE(reply, (int)ScreenLock::TkLockReplyOk);

    // Check that main window title and stacking layer were only changed if needed (and to the correct state)
    if (mainWindowModified) {
        QCOMPARE(HomeApplication::instance()->mainWindowInstance()->windowTitle(), mainWindowTitle);
        testStackingLayer(1, mainWindowStackingLayer);
    }

    // Check that event eater window visibility was only changed if needed (and to the correct state)
    if (eventEaterWindowVisibilityModified) {
        QCOMPARE(gEventEaterStub->stubCallCount(eventEaterWindowVisible ? "show" : "hide"), 1);
    } else {
        QCOMPARE(gEventEaterStub->stubCallCount("show"), 0);
        QCOMPARE(gEventEaterStub->stubCallCount("hide"), 0);
    }
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
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);
    QCOMPARE(HomeApplication::instance()->mainWindowInstance()->windowTitle(), QString("Lipstick"));
    testStackingLayer(2, 0);
}

QTEST_MAIN(Ut_ScreenLock)
