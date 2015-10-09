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

#ifndef UT_DEVICELOCK_H
#define UT_DEVICELOCK_H

#include <QObject>

#include "devicelock.h"

class Ut_DeviceLock : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testSignalConnections();
    void testAllTransitions(void);

private:
    bool lockTimerIsActive();
    bool verifyStateVsTimer(void);
    bool shouldBeLocked(void);
    DeviceLock::LockState simulateLockStateChange(DeviceLock::LockState lockState);
    bool simulateNeedLockingTimer(void);
    void applyDeviceLockState(size_t index);
    void applyLockingDelay(size_t index);
    void applyCallState(size_t index);
    void applyDisplayState(size_t index);
    void applyTklockState(size_t index);
    void applyInactivityState(size_t index);
    void iterateDeviceLockStates(size_t currentIndex);
    void iterateLockingDelays(size_t currentIndex);
    void iterateCallStates(size_t currentIndex);
    void iterateDisplayStates(size_t currentIndex);
    void iterateTklockStates(size_t currentIndex);
    void iterateInactivityStates(size_t currentIndex);

    QString testContext();

    DeviceLock *deviceLock;
};

#endif
