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
#include <QTimer>
#include <QSettings>
#include "qmlocks_stub.h"
#include "qmactivity_stub.h"
#include "qmdisplaystate_stub.h"
#include "devicelock.h"
#include "ut_devicelock.h"

QVariant qSettingsValue;
QVariant QSettings::value(const QString &, const QVariant &) const
{
    return qSettingsValue;
}

bool qProcessWaitForFinished = false;
bool QProcess::waitForFinished(int)
{
    return qProcessWaitForFinished;
}

QString qProcessStartProgram;
QStringList qProcessStartArguments;
void QProcess::start(const QString &program, const QStringList &arguments, OpenMode)
{
    qProcessStartProgram = program;
    qProcessStartArguments = arguments;
}

int qProcessExitCode = 0;
int QProcess::exitCode() const
{
    return qProcessExitCode;
}

QByteArray QProcess::readAllStandardError()
{
    return QByteArray();
}

QByteArray QProcess::readAllStandardOutput()
{
    return QByteArray();
}

QList<int> qTimerStartMsec;
void QTimer::start(int msec)
{
    qTimerStartMsec.append(msec);
}

int qTimerStopCount = 0;
void QTimer::stop()
{
    qTimerStopCount++;
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

void Ut_DeviceLock::init()
{
    qSettingsValue = "test";
    qProcessStartProgram.clear();
    qProcessStartArguments.clear();
    qProcessWaitForFinished = true;
    qProcessExitCode = 1;

    deviceLock = new DeviceLock();
}

void Ut_DeviceLock::cleanup()
{
    delete deviceLock;

    qTimerStartMsec.clear();
    qTimerStopCount = 0;
}

void Ut_DeviceLock::testSignalConnections()
{
    QCOMPARE(disconnect(deviceLock->lockTimer, SIGNAL(timeout()), deviceLock, SLOT(lock())), true);
    QCOMPARE(disconnect(deviceLock->qmActivity, SIGNAL(activityChanged(MeeGo::QmActivity::Activity)), deviceLock, SLOT(setStateAndSetupLockTimer())), true);
    QCOMPARE(disconnect(deviceLock->qmLocks, SIGNAL(stateChanged(MeeGo::QmLocks::Lock,MeeGo::QmLocks::State)), deviceLock, SLOT(setStateAndSetupLockTimer())), true);
    QCOMPARE(disconnect(deviceLock->qmDisplayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), deviceLock, SLOT(checkDisplayState(MeeGo::QmDisplayState::DisplayState))), true);
}

void Ut_DeviceLock::testInitialState()
{
    delete deviceLock;

    qSettingsValue.clear();
    qProcessWaitForFinished = false;
    qProcessExitCode = 0;
    deviceLock = new DeviceLock();

    QCOMPARE(deviceLock->state(), (int)DeviceLock::Undefined);

    delete deviceLock;
    qSettingsValue = "-1";
    deviceLock = new DeviceLock();
    deviceLock->init();

    QCOMPARE(deviceLock->state(), (int)DeviceLock::Unlocked);
    QCOMPARE(qProcessStartProgram, qSettingsValue.toString());
    QCOMPARE(qProcessStartArguments, QStringList() << "--is-set" << "lockcode");

    delete deviceLock;
    qProcessWaitForFinished = true;
    deviceLock = new DeviceLock();
    deviceLock->init();

    QCOMPARE(deviceLock->state(), (int)DeviceLock::Unlocked);

    delete deviceLock;
    qProcessExitCode = 1;
    deviceLock = new DeviceLock();
    deviceLock->init();

    QCOMPARE(deviceLock->state(), (int)DeviceLock::Unlocked);
}

void Ut_DeviceLock::testSetState()
{
    QSignalSpy spy(deviceLock, SIGNAL(stateChanged(int)));

    deviceLock->setState(DeviceLock::Locked);
    QCOMPARE(deviceLock->state(), (int)DeviceLock::Locked);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toInt(), (int)DeviceLock::Locked);

    deviceLock->setState(DeviceLock::Locked);
    QCOMPARE(deviceLock->state(), (int)DeviceLock::Locked);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toInt(), (int)DeviceLock::Locked);

    deviceLock->setState(DeviceLock::Unlocked);
    QCOMPARE(deviceLock->state(), (int)DeviceLock::Unlocked);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.last().at(0).toInt(), (int)DeviceLock::Unlocked);

    deviceLock->setState(DeviceLock::Unlocked);
    QCOMPARE(deviceLock->state(), (int)DeviceLock::Unlocked);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.last().at(0).toInt(), (int)DeviceLock::Unlocked);
}

void Ut_DeviceLock::testLockTimerWhenDeviceIsLocked()
{
    qTimerStartMsec.clear();
    qTimerStopCount = 0;

    deviceLock->setState(DeviceLock::Locked);
    QCOMPARE(qTimerStopCount, 1);
    QCOMPARE(qTimerStartMsec.count(), 0);
}

Q_DECLARE_METATYPE(MeeGo::QmActivity::Activity)

void Ut_DeviceLock::testLockTimerWhenDeviceIsUnlocked_data()
{
    QTest::addColumn<int>("lockingDelayValue");
    QTest::addColumn<MeeGo::QmActivity::Activity>("activity");
    QTest::addColumn<int>("stopCount");
    QTest::addColumn<int>("startMSec");

    QTest::newRow("Automatic locking disabled, active") << -1 << MeeGo::QmActivity::Active << 1 << 0;
    QTest::newRow("Automatic locking immediate, active") << 0 << MeeGo::QmActivity::Active << 1 << 0;
    QTest::newRow("Automatic locking in 5 minutes, active") << 5 << MeeGo::QmActivity::Active << 1 << 0;
    QTest::newRow("Automatic locking disabled, inactive") << -1 << MeeGo::QmActivity::Inactive << 1 << 0;
    QTest::newRow("Automatic locking immediate, inactive") << 0 << MeeGo::QmActivity::Inactive << 1 << 0;
    QTest::newRow("Automatic locking in 5 minutes, inactive") << 5 << MeeGo::QmActivity::Inactive << 0 << (5 * 60 * 1000);
}

void Ut_DeviceLock::testLockTimerWhenDeviceIsUnlocked()
{
    QFETCH(int, lockingDelayValue);
    QFETCH(MeeGo::QmActivity::Activity, activity);
    QFETCH(int, stopCount);
    QFETCH(int, startMSec);

    deviceLock->setState(DeviceLock::Locked);
    qTimerStartMsec.clear();
    qTimerStopCount = 0;

    deviceLock->lockingDelay = lockingDelayValue;
    gQmActivityStub->stubSetReturnValue("get", activity);

    deviceLock->setState(DeviceLock::Unlocked);
    QCOMPARE(qTimerStopCount, stopCount);
    QCOMPARE(qTimerStartMsec.count(), startMSec > 0 ? 1 : 0);
}

Q_DECLARE_METATYPE(MeeGo::QmLocks::State)
Q_DECLARE_METATYPE(MeeGo::QmDisplayState::DisplayState)
Q_DECLARE_METATYPE(DeviceLock::LockState)

void Ut_DeviceLock::testDisplayStateWhenDeviceScreenIsLocked_data()
{
    QTest::addColumn<int>("lockingDelayValue");
    QTest::addColumn<MeeGo::QmDisplayState::DisplayState>("state");
    QTest::addColumn<MeeGo::QmLocks::State>("touchScreenLockState");
    QTest::addColumn<int>("stopCount");
    QTest::addColumn<DeviceLock::LockState>("deviceLockState");

    QTest::newRow("Automatic locking disabled, display on, screen unlocked")
            << -1 << MeeGo::QmDisplayState::DisplayState::On << MeeGo::QmLocks::Unlocked << 0 << DeviceLock::Unlocked;
    QTest::newRow("Automatic locking immediate, display on, screen unlocked")
            << 0 << MeeGo::QmDisplayState::DisplayState::On << MeeGo::QmLocks::Unlocked << 0 << DeviceLock::Unlocked;
    QTest::newRow("Automatic locking immediate, display on, screen locked")
            << 0 << MeeGo::QmDisplayState::DisplayState::On << MeeGo::QmLocks::Locked << 0 << DeviceLock::Unlocked;
    QTest::newRow("Automatic locking immediate, display off, screen locked")
            << 0 << MeeGo::QmDisplayState::DisplayState::Off << MeeGo::QmLocks::Locked << 1 << DeviceLock::Locked;
    QTest::newRow("Automatic locking immediate, display off, screen unlocked")
            << 0 << MeeGo::QmDisplayState::DisplayState::Off << MeeGo::QmLocks::Unlocked << 1 << DeviceLock::Locked;
    QTest::newRow("Automatic locking in 5 minutes, display off, screen locked")
            << 5 << MeeGo::QmDisplayState::DisplayState::Off << MeeGo::QmLocks::Locked << 0 << DeviceLock::Unlocked;
    QTest::newRow("Automatic locking disabled, display off, screen locked")
            << -1 << MeeGo::QmDisplayState::DisplayState::Off << MeeGo::QmLocks::Locked << 1 << DeviceLock::Unlocked;
}

void Ut_DeviceLock::testDisplayStateWhenDeviceScreenIsLocked()
{
    QFETCH(int, lockingDelayValue);
    QFETCH(MeeGo::QmDisplayState::DisplayState, state);
    QFETCH(MeeGo::QmLocks::State, touchScreenLockState);
    QFETCH(int, stopCount);
    QFETCH(DeviceLock::LockState, deviceLockState);

    deviceLock->setState(DeviceLock::Unlocked);
    qTimerStartMsec.clear();
    qTimerStopCount = 0;

    deviceLock->lockingDelay = lockingDelayValue;
    gQmLocksStub->stubSetReturnValue("getState", touchScreenLockState);
    gQmDisplayStateStub->stubSetReturnValue("get", state);

    deviceLock->checkDisplayState(state);

    QCOMPARE(deviceLock->state(), (int)deviceLockState);
    QCOMPARE(qTimerStopCount, stopCount);
}

void Ut_DeviceLock::testLockTimerTimeout()
{
    QSignalSpy spy(deviceLock, SIGNAL(stateChanged(int)));

    deviceLock->lock();

    QCOMPARE(deviceLock->state(), (int)DeviceLock::Locked);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toInt(), (int)DeviceLock::Locked);
}

void Ut_DeviceLock::testStateOnAutomaticLockingAndTouchScreenLockState_data()
{
    QTest::addColumn<int>("lockingDelayValue");
    QTest::addColumn<MeeGo::QmLocks::State>("touchScreenLockState");
    QTest::addColumn<DeviceLock::LockState>("deviceLockState");

    QTest::newRow("Automatic locking disabled, touch screen lock unlocked") << -1 << MeeGo::QmLocks::Unlocked << DeviceLock::Unlocked;
}

void Ut_DeviceLock::testStateOnAutomaticLockingAndTouchScreenLockState()
{
    QFETCH(int, lockingDelayValue);
    QFETCH(MeeGo::QmLocks::State, touchScreenLockState);
    QFETCH(DeviceLock::LockState, deviceLockState);

    deviceLock->setState(DeviceLock::Locked);

    deviceLock->lockingDelay = lockingDelayValue;
    gQmLocksStub->stubSetReturnValue("getState", touchScreenLockState);
    deviceLock->setStateAndSetupLockTimer();

    QCOMPARE(deviceLock->state(), (int)deviceLockState);
}

QTEST_MAIN(Ut_DeviceLock)
