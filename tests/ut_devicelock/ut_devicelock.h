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

class DeviceLock;

class Ut_DeviceLock : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testSignalConnections();
    void testInitialState();
    void testSetState();
    void testLockTimerWhenDeviceIsLocked();
    void testLockTimerWhenDeviceIsUnlocked_data();
    void testLockTimerWhenDeviceIsUnlocked();
    void testDisplayStateWhenDeviceScreenIsLocked_data();
    void testDisplayStateWhenDeviceScreenIsLocked();
    void testLockTimerTimeout();
    void testStateOnAutomaticLockingAndTouchScreenLockState_data();
    void testStateOnAutomaticLockingAndTouchScreenLockState();

private:
    DeviceLock *deviceLock;
};

#endif
