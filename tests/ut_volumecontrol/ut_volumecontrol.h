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

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testConnections();
    void testEventFilter();
    void testAcquireKeys();
    void testMaximumVolume();
    void testCallActive();

private:
    VolumeControl *volumeControl;
};

#endif // UT_VOLUMECONTROL_H_
