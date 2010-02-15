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
#ifndef _UT_STATUSBAR_
#define _UT_STATUSBAR_

#include <QtTest/QtTest>
#include <QObject>
#include <QPointF>
#include "statusareamodel.h"
#include "statusareastyle.h"

class StatusArea;

class Ut_StatusArea : public QObject
{
    Q_OBJECT

public:
    static bool duiApplicationIfProxyLaunchCalled;

private:
    StatusArea *statusArea;

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Tests the showing of status indicator menu with swipe
    void testWhenSwipeStatusAreaStatusIndicatorMenuAppears();
    // Test when swipe less than a particular amount, status indicator will not be triggered.
    void testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear();
};

#endif //_UT_STATUSBAR_
