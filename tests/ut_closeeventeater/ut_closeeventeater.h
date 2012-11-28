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

#ifndef UT_CLOSEEVENTEATER_H
#define UT_CLOSEEVENTEATER_H

#include <QObject>

class CloseEventEater;

class Ut_CloseEventEater : public QObject
{
    Q_OBJECT

private slots:
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testCloseEventIsIgnored();
    void testOtherEventsArePassedOn_data();
    void testOtherEventsArePassedOn();

private:
    // The object being tested
    CloseEventEater *closeEventEater;
};

#endif
