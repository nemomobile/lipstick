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
#ifndef _UT_STATUSINDICATOR_
#define _UT_STATUSINDICATOR_

#include <QObject>

class StatusIndicator;
class TestContext;
class DuiApplication;

class Ut_StatusIndicator : public QObject
{
    Q_OBJECT

private:
    DuiApplication  *app;
    StatusIndicator *statusIndicator;
    TestContext     *testContext;

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    void testModelUpdates();
    void testPhoneNetwork();
};

#endif //_UT_STATUSINDICATOR_
