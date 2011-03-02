/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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
#ifndef _UT_SWITCHERBUTTON_
#define _UT_SWITCHERBUTTON_

#include <QtGui>
#include <QtTest/QtTest>
#include <QObject>
#include "x11wrapper.h"

class HomeApplication;
class MainWindow;
class SwitcherButton;

class TestSignalReceiver : public QObject
{
    Q_OBJECT

public:
    Window switchedWindow;
    Window closedWindow;

public slots:
    void windowToFront(Window);
    void closeWindow(Window);
};

class Ut_SwitcherButton : public QObject
{
    Q_OBJECT

public:
    // Whether QTimer times out immediately
    static bool timerImmediateTimeout;
    // Whether prepareGeometryChange has been called
    static bool prepareGeometryChangeCalled;

    // For catching XChangeProperty parameters
    static Display *xChangePropertyDisplay;
    static Window xChangePropertyWindow;
    static Atom xChangePropertyProperty;
    static Atom xChangePropertyType;
    static int xChangePropertyFormat;
    static int xChangePropertyMode;
    static unsigned char xChangePropertyData[16];
    static int xChangePropertyNElements;
    static MainWindow *mainWindow;

private:
    HomeApplication *app;

    // SwitcherButton instance under testing
    SwitcherButton *button;

    // Helper for receiving signals
    TestSignalReceiver signalReceiver;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testXWindow();
    void testSwitchToWindow();
    void testClosingWithTimeout();
    void testClosingWithoutTimeout();
    void testPrepareGeometryChange();
    void testSetVisibleInSwitcherProperty_data();
    void testSetVisibleInSwitcherProperty();
    void testSetVisibleInSwitcherPropertyNotUpdatedWhenValueDoesNotChange();
    void testWhenVisibilityPropertyDisabledThenPropertyChangesOnlyWhenEnabledAgain();
};

#endif //_UT_SWITCHERBUTTON_
