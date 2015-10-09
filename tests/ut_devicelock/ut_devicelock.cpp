/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012, 2015 Jolla Ltd.
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
#include "devicelock.h"
#include "ut_devicelock.h"
#include <mce/mode-names.h>

/** Get number of elements in statically allocated array */
#define numof(a) (sizeof(a)/sizeof*(a))

/** Helper for getting device lock state in human readable form
 */
static const char *reprLockState(int state)
{
    switch (state) {
    case DeviceLock::Unlocked:  return "Unlocked";
    case DeviceLock::Locked:    return "Locked";
    case DeviceLock::Undefined: return "Undefined";
    default: break;
    }
    return "Invalid";
}

/** Helper for printing out device lock state data
 */
QString Ut_DeviceLock::testContext()
{
    char tmp[256];

    snprintf(tmp, sizeof tmp, "devlock=%s delay=%d call=%s displ=%s tklock=%s act=%s tmr=%s",
             reprLockState(deviceLock->m_deviceLockState),
             deviceLock->m_lockingDelay,
             deviceLock->m_callActive ? "yes" : "no",
             deviceLock->m_displayOn ? "on" : "off",
             deviceLock->m_tklockActive ? "yes" : "no",
             deviceLock->m_userActivity ? "yes" : "no",
             lockTimerIsActive() ? "running" : "stopped");

    return QString(tmp);
}

/** Array of device lock states to visit */
static const struct
{
    DeviceLock::LockState inout; // m_deviceLockState member variable
} deviceLockStates[] =
{
    { DeviceLock::Undefined },
    { DeviceLock::Unlocked  },
    { DeviceLock::Locked    },
};

/** Array of device lock timer delays to use */
static const struct
{
    int inout;  // m_lockingDelay member variable
} lockingDelays[] =
{
    { -1 }, // disabled
    {  0 }, // lock immediately on display=off
    {  1 }, // lock in 60 seconds of inactivity
};

/** Array of call states to visit */
static const struct
{
    const char *in;  // call state signal from mce
    bool        out; // -> m_callActive member variable
} callStates[] =
{
    { MCE_CALL_STATE_RINGING, true  },
    { MCE_CALL_STATE_ACTIVE,  true  },
    { MCE_CALL_STATE_NONE,    false },
};

/** Array of display states to visit */
static const struct
{
    const char *in;  // display state signal from mce
    bool        out; // -> m_displayOn member variable
} displayStates[] =
{
    { MCE_DISPLAY_ON_STRING,  true  },
    { MCE_DISPLAY_DIM_STRING, true  },
    { MCE_DISPLAY_OFF_STRING, false },
};

/** Array of tklock states to visit */
static const struct
{
    const char *in;  // tklock state signal from mce
    bool        out; // -> m_tklockActive member variable
} tklockStates[] =
{
    { MCE_TK_LOCKED,   true  },
    { MCE_TK_UNLOCKED, false },
};

/** Array of inactivity states to visit */
static const struct
{
    bool in;         // inactivity state signal from mce
    bool out;        // -> m_userActivity member variable
} inactivityStates[] =
{
    { false,  true  },
    { true,   false },
};

/** Predicate for: Device lock timer has been started
 */
bool Ut_DeviceLock::lockTimerIsActive()
{
    return deviceLock->m_hbTimer->isWaiting();
}

/** Predicate for: Eventually device should be locked
 */
bool Ut_DeviceLock::shouldBeLocked(void)
{
    // Must not lock until init is finished
    if (deviceLock->m_deviceLockState == DeviceLock::Undefined)
        return false;

    // Must not lock if locking is disabled
    if (deviceLock->m_lockingDelay < 0)
        return false;

    // Must not lock during calls
    if (deviceLock->m_callActive)
        return false;

    // Must not lock during active use
    if (deviceLock->m_displayOn &&
        !deviceLock->m_tklockActive &&
        deviceLock->m_userActivity)
        return false;

    // Must not lock in immediate mode while display is on
    if (deviceLock->m_lockingDelay == 0 &&  deviceLock->m_displayOn)
        return false;

    // Otherwise we should be locked via timer
    return true;
}

/** Predicate for: Device lock state vs locking timer staus is ok
 */
bool Ut_DeviceLock::verifyStateVsTimer(void)
{
    // Must not have locking timer in Locked/Undefined state
    if (deviceLock->m_deviceLockState != DeviceLock::Unlocked)
        return !lockTimerIsActive();

    // When possible, we must have lock timer when Unlocked
    if (shouldBeLocked())
        return lockTimerIsActive();

    // Otherwise the timer must not be active
    return !lockTimerIsActive();
}

/** Predicate for: Device lock timer should have been started
 */
bool Ut_DeviceLock::simulateNeedLockingTimer(void)
{
    // Must not lock if locking is disabled
    if (deviceLock->m_lockingDelay < 0)
        return false;

    // Must not use timer if immediate mode is in use
    if (deviceLock->m_lockingDelay == 0)
        return false;

    // Must not lock during calls
    if (deviceLock->m_callActive)
        return false;

    // Can't lock if already locked
    if (deviceLock->m_deviceLockState == DeviceLock::Locked)
        return false;

    // Must not lock until init is finished
    if (deviceLock->m_deviceLockState == DeviceLock::Undefined)
        return false;

    // Can't lock if Undefined or already Locked
    if (deviceLock->m_deviceLockState != DeviceLock::Unlocked)
        return false;

    // Can lock if: display is off, lockscreen is active
    // or user is not actively using the device
    return (!deviceLock->m_displayOn ||
            deviceLock->m_tklockActive ||
            !deviceLock->m_userActivity);
}

/** Filter device lock state transition
 */
DeviceLock::LockState Ut_DeviceLock::simulateLockStateChange(DeviceLock::LockState lockState)
{
    switch (lockState) {
    case DeviceLock::Undefined:
        break;
    case DeviceLock::Locked:
        // can't lock if locking is disabled
        if (deviceLock->m_lockingDelay < 0)
            lockState = DeviceLock::Unlocked;
        break;
    case DeviceLock::Unlocked:
        // can't unlock if display is off with immediate locking
        if (!deviceLock->m_displayOn && deviceLock->m_lockingDelay == 0)
            lockState = DeviceLock::Locked;
        break;
    default:
        break;
    }
    return lockState;
}

/** Simulate device lock state change
 */
void Ut_DeviceLock::applyDeviceLockState(size_t index)
{
    /* Get context before making any changes */
    QString context = testContext();

    /* Device lock state change can be vetoed, so we
     * can't blindly require it to be obeyed */
    DeviceLock::LockState target = deviceLockStates[index].inout;
    DeviceLock::LockState expect = simulateLockStateChange(target);
    deviceLock->setState(target);

    /* Print context only if the test is going to fail */
    if (!verifyStateVsTimer() ||
        deviceLock->m_deviceLockState != expect ||
        lockTimerIsActive() != simulateNeedLockingTimer())
        qDebug("setState(%s) @ %s", reprLockState(target),
               qPrintable(context));

    QVERIFY(verifyStateVsTimer());
    QCOMPARE(deviceLock->m_deviceLockState, expect);
    QCOMPARE(lockTimerIsActive(), simulateNeedLockingTimer());
}

/** Simulate device lock timer delay setting change
 */
void Ut_DeviceLock::applyLockingDelay(size_t index)
{
    /* Get context before making any changes */
    QString context = testContext();

    /* Note: implemented as direct member value set -> no implicit
     *       checking unlike the real code where locking delay
     *       changes via config file change notifier */
    int target = lockingDelays[index].inout;
    deviceLock->m_lockingDelay = target;
    deviceLock->setStateAndSetupLockTimer();

    /* Print context only if the test is going to fail */
    if (!verifyStateVsTimer() ||
        lockTimerIsActive() != simulateNeedLockingTimer())
        qDebug("setLockingDelay(%d) @ %s", target,
               qPrintable(context));

    QVERIFY(verifyStateVsTimer());
    QCOMPARE(lockTimerIsActive(), simulateNeedLockingTimer());
}

/** Simulate call state change signal
 */
void Ut_DeviceLock::applyCallState(size_t index)
{
    /* Get context before making any changes */
    QString context = testContext();

    deviceLock->handleCallStateChanged(QString(callStates[index].in));

    /* Print context only if the test is going to fail */
    if (!verifyStateVsTimer() ||
        deviceLock->m_callActive != callStates[index].out ||
        lockTimerIsActive() != simulateNeedLockingTimer())
        qDebug("callStateSig(%s) @ %s", callStates[index].in,
               qPrintable(context));

    QVERIFY(verifyStateVsTimer());
    QCOMPARE(deviceLock->m_callActive, callStates[index].out);
    QCOMPARE(lockTimerIsActive(), simulateNeedLockingTimer());
}

/** Simulate display state change signal
 */
void Ut_DeviceLock::applyDisplayState(size_t index)
{
    /* Get context before making any changes */
    QString context = testContext();

    deviceLock->handleDisplayStateChanged(QString(displayStates[index].in));

    /* Print context only if the test is going to fail */
    if (!verifyStateVsTimer() ||
        deviceLock->m_displayOn != displayStates[index].out ||
        lockTimerIsActive() != simulateNeedLockingTimer())
        qDebug("displayStateSig(%s) @ %s", displayStates[index].in,
               qPrintable(context));

    QVERIFY(verifyStateVsTimer());
    QCOMPARE(deviceLock->m_displayOn, displayStates[index].out);
    QCOMPARE(lockTimerIsActive(), simulateNeedLockingTimer());
}

/** Simulate tklock state change signal
 */
void Ut_DeviceLock::applyTklockState(size_t index)
{
    /* Get context before making any changes */
    QString context = testContext();

    deviceLock->handleTklockStateChanged(QString(tklockStates[index].in));

    /* Print context only if the test is going to fail */
    if (!verifyStateVsTimer() ||
        deviceLock->m_tklockActive != tklockStates[index].out ||
        lockTimerIsActive() != simulateNeedLockingTimer())
        qDebug("tklockStateSig(%s) @ %s", tklockStates[index].in,
               qPrintable(context));

    QVERIFY(verifyStateVsTimer());
    QCOMPARE(deviceLock->m_tklockActive, tklockStates[index].out);
    QCOMPARE(lockTimerIsActive(), simulateNeedLockingTimer());
}

/** Simulate inactivity state change signal
 */
void Ut_DeviceLock::applyInactivityState(size_t index)
{
    /* Get context before making any changes */
    QString context = testContext();

    deviceLock->handleInactivityStateChanged(inactivityStates[index].in);

    /* Print context only if the test is going to fail */
    if (!verifyStateVsTimer() ||
        deviceLock->m_userActivity != inactivityStates[index].out ||
        lockTimerIsActive() != simulateNeedLockingTimer())
        qDebug("inactivityStateSig(%s) @ %s",
               inactivityStates[index].in ? "inactive" : "active",
               qPrintable(context));

    QVERIFY(verifyStateVsTimer());
    QCOMPARE(deviceLock->m_userActivity, inactivityStates[index].out);
    QCOMPARE(lockTimerIsActive(), simulateNeedLockingTimer());
}

/** Do single steps to/from remaining device lock states
 */
void Ut_DeviceLock::iterateDeviceLockStates(size_t currentIndex)
{
    for (size_t i = currentIndex + 1; i < numof(deviceLockStates); ++i) {
        applyDeviceLockState(i);
        applyDeviceLockState(currentIndex);
    }
}

/** Do single steps to/from remaining locking delay times
 */
void Ut_DeviceLock::iterateLockingDelays(size_t currentIndex)
{
    for (size_t i = currentIndex + 1; i < numof(lockingDelays); ++i) {
        applyLockingDelay(i);
        applyLockingDelay(currentIndex);
    }
}

/** Do single steps to/from remaining call states
 */
void Ut_DeviceLock::iterateCallStates(size_t currentIndex)
{
    for (size_t i = currentIndex + 1; i < numof(callStates); ++i) {
        applyCallState(i);
        applyCallState(currentIndex);
    }
}

/** Do single steps to/from remaining display states
 */
void Ut_DeviceLock::iterateDisplayStates(size_t currentIndex)
{
    for (size_t i = currentIndex + 1; i < numof(displayStates); ++i) {
        applyDisplayState(i);
        applyDisplayState(currentIndex);
    }
}

/** Do single steps to/from remaining tklock states
 */
void Ut_DeviceLock::iterateTklockStates(size_t currentIndex)
{
    for (size_t i = currentIndex + 1; i < numof(tklockStates); ++i) {
        applyTklockState(i);
        applyTklockState(currentIndex);
    }
}

/** Do single steps to/from remaining inactivity states
 */
void Ut_DeviceLock::iterateInactivityStates(size_t currentIndex)
{
    for (size_t i = currentIndex + 1; i < numof(inactivityStates); ++i) {
        applyInactivityState(i);
        applyInactivityState(currentIndex);
    }
}

/** Go through all states that affect device lock
 */
void Ut_DeviceLock::testAllTransitions(void)
{
    /* Go through all possible combinations of state variables */
    for (size_t lockingDelayIndex = 0; lockingDelayIndex < numof(lockingDelays); ++lockingDelayIndex) {
        applyLockingDelay(lockingDelayIndex);

        for (size_t callStateIndex = 0; callStateIndex < numof(callStates); ++callStateIndex) {
            applyCallState(callStateIndex);

            for (size_t displayStateIndex = 0; displayStateIndex < numof(displayStates); ++displayStateIndex) {
                applyDisplayState(displayStateIndex);

                for (size_t tklockStateIndex = 0; tklockStateIndex < numof(tklockStates); ++tklockStateIndex) {
                    applyTklockState(tklockStateIndex);

                    for (size_t inactivityStateIndex = 0; inactivityStateIndex < numof(inactivityStates); ++inactivityStateIndex) {
                        applyInactivityState(inactivityStateIndex);

                        for (size_t deviceLockStateIndex = 0; deviceLockStateIndex < numof(deviceLockStates); ++deviceLockStateIndex) {
                            applyDeviceLockState(deviceLockStateIndex);

                            /* Simulate all one step transitions that can be made
                             * from each state. */
                            iterateDeviceLockStates(deviceLockStateIndex);
                            iterateLockingDelays(lockingDelayIndex);
                            iterateCallStates(callStateIndex);
                            iterateDisplayStates(displayStateIndex);
                            iterateTklockStates(tklockStateIndex);
                            iterateInactivityStates(inactivityStateIndex);
                        }
                    }
                }
            }
        }
    }
}

void Ut_DeviceLock::testSignalConnections()
{
    QCOMPARE(disconnect(deviceLock->m_hbTimer, SIGNAL(running()),
                        deviceLock, SLOT(lock())), true);
    // FIXME: Should the dbus signal listener connections be tested?
}

void Ut_DeviceLock::cleanup()
{
    delete deviceLock; deviceLock = 0;
}

void Ut_DeviceLock::init()
{
    deviceLock = new DeviceLock();

    /* In real use we want to have some transition logging, but
     * for unit tests they just generate huge amounts of noise */
    deviceLock->m_verbosityLevel = 0;
}

QTEST_MAIN(Ut_DeviceLock)
