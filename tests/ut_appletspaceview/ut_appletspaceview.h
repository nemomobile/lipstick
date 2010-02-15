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

#ifndef UT_APPLETSPACEVIEW_H
#define UT_APPLETSPACEVIEW_H

#include <QtTest/QtTest>
#include <QObject>

class DuiApplication;
class DuiWindow;
class AppletSpace;
class AppletSpaceView;

class Ut_AppletSpaceView : public QObject
{
    Q_OBJECT

public:
    static int showWindowCount;
    static int hideWindowCount;

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
    void testCloseButtonVisibility();
private:
    // DuiApplication
    DuiApplication *app;
    // Window
    DuiWindow *window;
    // Controller for the object being tested
    AppletSpace *controller;
    // View for the object being tested
    AppletSpaceView *view;
};

#endif
