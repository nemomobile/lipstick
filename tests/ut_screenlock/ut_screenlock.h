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

#ifndef UT_SCREENLOCK_H
#define UT_SCREENLOCK_H

#include <QGraphicsWidget>

class ScreenLock;

class Ut_ScreenLock : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testToggleScreenLockUI();
    void testToggleEventEater();
    void testUnlockScreenWhenLocked();
    void testUnlockScreenWhenNotLocked();
    void testHideEventEater();
    void testTkLockOpen_data();
    void testTkLockOpen();
    void testTkLockClose();

private:
    ScreenLock *screenLock;
};

#endif

