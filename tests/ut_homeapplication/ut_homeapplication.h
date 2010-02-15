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

#ifndef UT_HOMEAPPLICATION_H
#define UT_HOMEAPPLICATION_H

#include <QtTest/QtTest>
#include <QtGui>
#include "windowinfo.h"
#include "contextframeworkcontext.h"

class TestHomeApplication;

class WindowListReceiver : public QObject
{
    Q_OBJECT

public:
    WindowListReceiver() : count(0) { }
    QList<WindowInfo> windowList;
    int count;

private slots:
    void windowListUpdated(const QList<WindowInfo> &windowList) {
        this->windowList = windowList;
        this->count++;
    }
};

class WindowVisibilityReceiver : public QObject
{
    Q_OBJECT

public:
    WindowVisibilityReceiver() { }
    QList<Window> windowList;

private slots:
    void windowVisibilityChanged(Window window) {
        this->windowList.append(window);
    }
};

class Ut_HomeApplication : public QObject
{
    Q_OBJECT

public:
    static QList<Window> visibilityNotifyWindows;
    static QList<QString> validInterfaces;
    static QList<QString> serviceInterfaces;
    static QList<QString> asyncCallMethods;
    static QList< QList<QVariant> > asyncCallArguments;
    static int clientListNumberOfWindows;

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test the startup, so that the process stopping and d-bus
    // notifications work
    void testUpstartStartup();
    // Test the startup, so that the process stopping and d-bus
    // notifications don't happen when the --upstart switch is not given
    void testNonUpstartStartup();
    // Test the Use.Mode context property
    void testUseMode();
    // Test X11EventFilter with PropertyNotify events
    void testX11EventFilterWithPropertyNotify();
    // Test X11EventFilter with VisibilityNotify events
    void testX11EventFilterWithVisibilityNotify();
    // Test X11EventFilter with ClientMessage events
    void testX11EventFilterWithClientMessage();
    // Test content search service launch
    void testContentSearchLaunch();
    // Test content search service launch without service framework
    void testContentSearchLaunchWithoutServiceFW();
    // Test updating windows list
    void testUpdateWindowList();

private:
    // The object being tested
    TestHomeApplication *m_subject;
};

#endif
