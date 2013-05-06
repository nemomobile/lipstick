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

#ifndef UT_VOLUMECONTROL_H_
#define UT_VOLUMECONTROL_H_

#include <QObject>

class VolumeControl;

class Ut_VolumeControl : public QObject
{
Q_OBJECT

signals:
    void timeout();

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testConnections();
    void testKeyRepeatSetup();
    void testHwKeyEvent_data();
    void testHwKeyEvent();
    void testHwKeyEventWhenKeyRepeatDelayIsInProgress();
    void testHwKeyEventWhenKeyRepeatIsInProgress();
    void testHwKeyEventWhenKeyReleaseIsInProgress();
    void testAcquireKeys();
    void testReleaseKeys();

private:
    VolumeControl *volumeControl;
};

#endif // UT_VOLUMECONTROL_H_
