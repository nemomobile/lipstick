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

#ifndef UT_STATUSINDICATORLABELVIEW_H
#define UT_STATUSINDICATORLABELVIEW_H

#include <QtTest/QtTest>
#include <QObject>

#include "statusindicatorlabelview.h"

class DuiApplication;

class TestStatusIndicatorLabelView : public StatusIndicatorLabelView
{
    Q_OBJECT
    DUI_VIEW(StatusIndicatorModel, StatusIndicatorLabelStyle)

public:
    TestStatusIndicatorLabelView(StatusIndicator *controller);

    StatusIndicatorModel *getModel();

};


class Ut_StatusIndicatorLabelView : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testLabelChanged();
    void testModelChanged();

private:
    // DuiApplication
    DuiApplication *app;
    // A controller for the test subject
    StatusIndicator *controller;
    // The object being tested
    TestStatusIndicatorLabelView *m_subject;
};

#endif
