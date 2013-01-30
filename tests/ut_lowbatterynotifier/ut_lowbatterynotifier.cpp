/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (directui@nokia.com)
 **
 ** This file is part of systemui.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at directui@nokia.com.
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License version 2.1 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.LGPL included in the packaging
 ** of this file.
 **
 ****************************************************************************/
#include <QtTest/QtTest>

#include "qmdisplaystate_stub.h"
#include "contextproperty_stub.h"
#include "lowbatterynotifier.h"
#include "ut_lowbatterynotifier.h"

void QTimer::start()
{
    id = 1;
}

int qTimerStartMsec = 0;
void QTimer::start(int msec)
{
    qTimerStartMsec = msec;
    setInterval(msec);
    id = 1;
}

int qTimeElapsed = 0;
int QTime::elapsed() const
{
    return qTimeElapsed;
}

void Ut_LowBatteryNotifier::init()
{
    gContextPropertyStub->stubSetReturnValue("value", QVariant("inactive"));
    m_subject = new LowBatteryNotifier;
}

void Ut_LowBatteryNotifier::cleanup()
{
    delete m_subject;
    qTimerStartMsec = 0;
    qTimeElapsed = 0;
}

void Ut_LowBatteryNotifier::initTestCase()
{
}

void Ut_LowBatteryNotifier::cleanupTestCase()
{
}

void Ut_LowBatteryNotifier::testSignalConnections()
{
    QCOMPARE(disconnect(m_subject->displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), m_subject, SLOT(setNotificationInterval())), true);
    QCOMPARE(disconnect(&m_subject->callContextItem, SIGNAL(valueChanged()), m_subject, SLOT(setNotificationInterval())), true);
    QCOMPARE(disconnect(m_subject->notificationTimer, SIGNAL(timeout()), m_subject, SLOT(sendLowBatteryAlert())), true);
}

void Ut_LowBatteryNotifier::testSendLowBatteryAlert()
{
    QSignalSpy spy(m_subject, SIGNAL(lowBatteryAlert()));
    m_subject->notificationInterval = 12345;
    m_subject->sendLowBatteryAlert();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qTimerStartMsec, m_subject->notificationInterval);
}

Q_DECLARE_METATYPE(MeeGo::QmDisplayState::DisplayState)

void Ut_LowBatteryNotifier::testSetNotificationInterval_data()
{
    QTest::addColumn<bool>("touchScreenLockActive");
    QTest::addColumn<MeeGo::QmDisplayState::DisplayState>("displayState");
    QTest::addColumn<bool>("callActive");
    QTest::addColumn<int>("timeSincePreviousNotification");
    QTest::addColumn<int>("notificationTimerInterval");
    QTest::addColumn<int>("lowBatteryAlertCount");

    QTest::newRow("Device inactive because touch screen lock is active and display is off") << true << MeeGo::QmDisplayState::Off << false << 1000 << (30 * 60 * 1000 - 1000) << 0;
    QTest::newRow("Device active because touch screen lock is active but display is on") << true << MeeGo::QmDisplayState::On << false << 2000 << (5 * 60 * 1000 - 2000) << 0;
    QTest::newRow("Device active because touch screen lock is not active but display is off") << false << MeeGo::QmDisplayState::Off << false << 3000 << (5 * 60 * 1000 - 3000) << 0;
    QTest::newRow("Device inactive but call active") << true << MeeGo::QmDisplayState::Off << true << 4000 << (2 * 60 * 1000 - 4000) << 0;
    QTest::newRow("Device active but call active") << false << MeeGo::QmDisplayState::On << true << 5000 << (2 * 60 * 1000 - 5000) << 0;
    QTest::newRow("Device inactive, interval elapsed") << true << MeeGo::QmDisplayState::Off << false << (31 * 60 * 1000) << (30 * 60 * 1000) << 1;
    QTest::newRow("Device active, interval elapsed") << false << MeeGo::QmDisplayState::On << false << (6 * 60 * 1000) << (5 * 60 * 1000) << 1;
    QTest::newRow("Call active, interval elapsed") << false << MeeGo::QmDisplayState::On << true << (3 * 60 * 1000) << (2 * 60 * 1000) << 1;
}

void Ut_LowBatteryNotifier::testSetNotificationInterval()
{
    QFETCH(bool, touchScreenLockActive);
    QFETCH(MeeGo::QmDisplayState::DisplayState, displayState);
    QFETCH(bool, callActive);
    QFETCH(int, timeSincePreviousNotification);
    QFETCH(int, notificationTimerInterval);
    QFETCH(int, lowBatteryAlertCount);

    // Force a change by first setting the activity state to the opposite value
    gQmDisplayStateStub->stubSetReturnValue("get", displayState == MeeGo::QmDisplayState::On ? MeeGo::QmDisplayState::Off : MeeGo::QmDisplayState::On);
    gContextPropertyStub->stubSetReturnValue("value", QVariant(callActive ? "inactive" : "active"));
    m_subject->setTouchScreenLockActive(!touchScreenLockActive);

    // Set the values according to the test data and verify
    QSignalSpy spy(m_subject, SIGNAL(lowBatteryAlert()));
    gQmDisplayStateStub->stubSetReturnValue("get", displayState);
    gContextPropertyStub->stubSetReturnValue("value", QVariant(callActive ? "active" : "inactive"));
    qTimeElapsed = timeSincePreviousNotification;
    m_subject->setTouchScreenLockActive(touchScreenLockActive);
    QCOMPARE(m_subject->notificationTimer->interval(), notificationTimerInterval);
    QCOMPARE(spy.count(), lowBatteryAlertCount);
}

void Ut_LowBatteryNotifier::testSetNotificationIntervalDoesNothingWhenStateDoesNotChange()
{
    // Set some initial values
    gQmDisplayStateStub->stubSetReturnValue("get", MeeGo::QmDisplayState::On);
    gContextPropertyStub->stubSetReturnValue("value", QVariant("inactive"));
    qTimeElapsed = 0;
    m_subject->setTouchScreenLockActive(false);

    // Set the timer to a new value but don't change anything else: setting the notification interval should do nothing
    QSignalSpy spy(m_subject, SIGNAL(lowBatteryAlert()));
    m_subject->notificationTimer->setInterval(12345);
    m_subject->setNotificationInterval();
    QCOMPARE(spy.count(), 0);
    QCOMPARE(m_subject->notificationTimer->interval(), 12345);
}

QTEST_MAIN(Ut_LowBatteryNotifier)
