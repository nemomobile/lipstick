/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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
#ifndef UT_CLOCKVIEW_H
#define UT_CLOCKVIEW_H

#include <QtTest/QtTest>
#include <QObject>

class ClockView;
class Clock;
class DuiApplication;

class Ut_ClockView : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testUpdate();

public:
    static QString timeAsString;

private:
    DuiApplication *app;
    ClockView *m_subject;
    Clock *testClock;
};

#endif
