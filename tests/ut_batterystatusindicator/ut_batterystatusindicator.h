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
#ifndef UT_BATTERYSTATUSINDICATOR_H
#define UT_BATTERYSTATUSINDICATOR_H

#include <QObject>

class Ut_BatteryStatusIndicator : public QObject
{
    Q_OBJECT

private slots:
    void testConstruction();
    void testSetMode_data();
    void testSetMode();
    void testSetValue_data();
    void testSetValue();
};

#endif
